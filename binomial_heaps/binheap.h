#include <iostream>

// Vorzeichenlose ganze Zahl.
using uint = unsigned int;

// passt = check1
// geprüft = check2

template <typename P, typename D> struct BinHeap {
  // Notwendige Vorabdeklaration.
  struct Node;

  struct Entry {
    // PrioritÃ¤t, zusÃ¤tzliche Daten und zugehÃ¶riger Knoten.
    P prio;
    D data;
    Node *node;

    // Initialisierung mit PrioritÃ¤t p und zusÃ¤tzlichen Daten d.
    Entry(P p, D d) : prio(p), data(d), node(nullptr) {}
  };

  struct Node {
    // ZugehÃ¶riger Eintrag.
    Entry *entry;

    // Grad des Knotens.
    uint degree;

    // VorgÃ¤nger (falls vorhanden; Nullzeiger bei einem Wurzelknoten).
    Node *parent;

    Node *child;

    Node *sibling;

    // Initialisierung als Wurzelknoten mit Grad 0 und Eintrag e,
    // der dann auf diesen Knoten zurÃ¼ckverweist.
    Node(Entry *e)
        : entry(e), degree(0), parent(nullptr), child(nullptr),
          sibling(nullptr) {
      e->node = this;
    }
  };

  // Anfang der Wurzelliste (Nullzeiger bei einer leeren Halde).
  Node *head;

  // Initialisierung als leere Halde.
  BinHeap() : head(nullptr) {}

  //------------------------------------------------------------------------------------
  // geprüft

  // Ist die Halde momentan leer?
  bool isEmpty() { return head == nullptr; }

  //------------------------------------------------------------------------------------
  // geprüft geg nochmals bearbeiten passt es für größere oder übrlauf?

  uint pow2(unsigned int grad) {
    unsigned int result = 1;
    for (int i = 0; i < grad; i++) {
      result = result * 2;
    }
    return result;
  }

  // (*)
  uint size() {
    if (isEmpty()) {
      return 0;
    } else {
      uint count = 0;
      Node *current = head;
      do {
        count = count + pow2(current->degree);
        current = current->sibling;
      } while (current != nullptr);
      return count;
    }
  }

  //------------------------------------------------------------------------------------
  // geprüft

  Entry *insert(P p, D d) {
    BinHeap<P, D> *newHeap = new BinHeap<P, D>();
    Entry *e = new Entry(p, d);
    Node *n = new Node(e);
    e->node = n;
    if (head == nullptr) {
      n->sibling = nullptr;
      head = n;
      return e;
    }
    n->sibling = nullptr;
    newHeap->head = n;
    head = merge(*this, *newHeap);
    delete newHeap; // so jetzt bei jedem + schleifen raus !!!!!
    return e;
  }

  //------------------------------------------------------------------------------------
  // geprüft

  Entry *minimum() {
    if (isEmpty()) {
      return nullptr;
    } else {
      Node *current = head;
      Node *min = head;
      Entry *result = min->entry;
      current = current->sibling;
      while (current != nullptr) {
        if (current->entry->prio < min->entry->prio) {
          min = current;
          result = current->entry;
        }
        current = current->sibling;
      }
      return result;
    }
  }

  //------------------------------------------------------------------------------------
  // geprüft

  Entry *extractMin() {
    if (head == nullptr) {
      return nullptr;
    }
    Node *current = head;
    Node *prev = nullptr;
    Node *minPrev = nullptr;
    Node *minNode = head;
    Entry *minEntry = minNode->entry;
    prev = current;
    current = current->sibling;
    while (current != nullptr) {
      if (current->entry->prio < minEntry->prio) {
        minNode = current;
        minEntry = current->entry;
        minPrev = prev;
      }
      prev = current;
      current = current->sibling;
    }

    // gkeich anschauen
    if (minPrev == nullptr) {            // Fall erstes Element ist kleinstes
      if (minNode->sibling == nullptr) { // einziger knoten
        head = nullptr;
      } else {
        // minPrev = current;
        head = minNode->sibling; // head zeiger muss verschoben werden
        minNode->sibling = nullptr;
      }
    } else {
      //------ bis hier rausfinden welcher kleinster ist + min prev richtig
      // setzen
      minPrev->sibling = minNode->sibling; // umbiegen des zeigers über min
      minNode->sibling = nullptr;
    }
    minNode->entry = nullptr;
    minEntry->node = nullptr;

    //------ bis hier wurden jetzt dann alle mit min zeiger gelöscht
    if (minNode->child != nullptr) { // hat ein child //geg falls hier nullptr
      BinHeap<P, D> *newHeap = new BinHeap<P, D>();
      if (minNode->child->sibling == minNode->child) { // hat genau ein kind
        newHeap->head = minNode->child;
        minNode->child->parent = nullptr;
        minNode->child->sibling = nullptr;
        minNode->child = nullptr;
        head = merge(*this, *newHeap);
      } else { // hat mehrere Kinder
        Node *children = minNode->child->sibling;
        newHeap->head = minNode->child->sibling;
        do {
          children->parent = nullptr;
          children = children->sibling;
        } while (children != minNode->child->sibling);
        minNode->child->sibling = nullptr;
        minNode->child = nullptr; // entfernen des zirkulärens
        head = merge(*this, *newHeap);
      }
      delete newHeap;
    }
    delete minNode;
    return minEntry;
  }

  //------------------------------------------------------------------------------------
  // geprüft

  bool contains(Entry *e) {
    if (e == nullptr || e->node == nullptr || head == nullptr) {
      return false;
    } else {
      Node *currentNode = e->node;
      Node *root = nullptr;
      while (currentNode->parent != nullptr) {
        currentNode = currentNode->parent;
      }
      root = currentNode;
      Node *current = head;
      do {
        if (current == root) {
          return true;
        }
        current = current->sibling;
      } while (current != nullptr);
      return false;
    }
  }

  //------------------------------------------------------------------------------------
  // decreases geprüft

  bool changePrio(Entry *e, P p) {
    if (contains(e) == true) {
      if (!(e->prio < p)) { // decrease (objekt nach oben vertauschen)
        e->prio = p;
        while (e->node->parent != nullptr &&
               e->prio < e->node->parent->entry->prio) {
          e->node->parent->entry->node = e->node;
          e->node->entry = e->node->parent->entry;
          e->node->parent->entry = e;
          e->node = e->node->parent;
        }
        return true;
      } else { // increase
        if (remove(e)) {
          insert(p, e->data);
          return true;
        } else {
          return false;
        }
      }
    } else {
      return false;
    }
  }

  //------------------------------------------------------------------------------------
  // geprüft

  bool remove(Entry *e) {
    if (e == nullptr || contains(e) == false) {
      return false;
    }
    Entry *MinE = extractMin();
    if (MinE != e) {
      if (changePrio(e, MinE->prio) == true) { // 1 1
        Node *targetNode = e->node;
        targetNode->entry = MinE;
        e->node = nullptr;
        MinE->node = targetNode;
        return true;
      } else {
        return false;
      }
    } else {
      return true;
    }
  }

  //------------------------------------------------------------------------------------
  // Passt //am ende anschauen fehler da ganz oben anders wie alle drunter

  //------------------------------------------------------------------------------------
  // passt

  // (*)
  Node *combine(Node *B1, Node *B2) {
    if (B2->entry->prio < B1->entry->prio) {
      B2->parent = nullptr;
      B2->sibling = nullptr;
      B2->degree = B2->degree + 1;
      B1->parent = B2;
      if (B2->child == nullptr) {
        B1->sibling = B1;
        B2->child = B1;
      } else {
        B1->sibling = B2->child->sibling;
        B2->child->sibling = B1;
        B2->child = B1;
      }
      return B2;
    } else {
      B1->parent = nullptr;
      B1->sibling = nullptr;
      B1->degree = B1->degree + 1;
      B2->parent = B1;
      if (B1->child == nullptr) {
        B2->sibling = B2; // Zeiger auf sich selbst
        B1->child = B2;
      } else {
        B2->sibling = B1->child->sibling;
        B1->child->sibling = B2;
        B1->child = B2;
      }
      return B1;
    }
  }

  //------------------------------------------------------------------------------------
  Node *merge(BinHeap h1, BinHeap h2) { // geändert
    BinHeap<P, D> *result = new BinHeap<P, D>();
    Node *zwischenspeicher3 = nullptr;
    int k = 0;

    Node *current1 = h1.head;
    Node *current2 = h2.head;
    Node *dummy;
    while (current1 != nullptr || current2 != nullptr ||
           zwischenspeicher3 !=
               nullptr) { // zwischenspeicher3 versthe ich nicht
      if (zwischenspeicher3 != nullptr) {
        if (current1 != nullptr && current2 != nullptr &&
            current1->degree == k && current2->degree == k) {
          Node *child_current1 = current1->sibling;
          Node *child_current2 = current2->sibling;
          result->add(zwischenspeicher3);
          zwischenspeicher3 = nullptr;
          zwischenspeicher3 = combine(current1, current2);
          current1 = child_current1;
          current2 = child_current2;
          k = k + 1;
        } else if (current1 != nullptr && current1->degree == k) {
          Node *child_current1 = current1->sibling;
          zwischenspeicher3 = combine(current1, zwischenspeicher3);
          current1 = child_current1;
          k = k + 1;
        } else if (current2 != nullptr && current2->degree == k) {
          Node *child_current2 = current2->sibling;
          zwischenspeicher3 = combine(current2, zwischenspeicher3);
          current2 = child_current2;
          k = k + 1;
        } else {
          result->add(zwischenspeicher3);
          zwischenspeicher3 = nullptr;
          k = k + 1;
        }
      } else {
        if (current1 != nullptr && current2 != nullptr &&
            current1->degree == k && current2->degree == k) {
          Node *child_current1 = current1->sibling;
          Node *child_current2 = current2->sibling;
          zwischenspeicher3 = combine(current1, current2);
          current1 = child_current1;
          current2 = child_current2;
          k = k + 1;
        } else if (current1 != nullptr && current1->degree == k) {
          dummy = current1->sibling;
          result->add(current1);
          current1 = dummy;
          k = k + 1;
        } else if (current2 != nullptr && current2->degree == k) {
          dummy = current2->sibling;
          result->add(current2);
          current2 = dummy;
          k = k + 1;
        } else {
          k = k + 1;
        }
      }
    }

    return result->head;
  };

  void add(Node *n) {
    if (n == nullptr)
      return;
    n->sibling = nullptr;
    if (head == nullptr) {
      head = n;
      return;
    }
    Node *cur = head;
    while (cur->sibling != nullptr)
      cur = cur->sibling;
    cur->sibling = n;
  }

  void display_nodes_of_tree(Node *node, uint level) {
    if (node == nullptr) {
      return;
    }
    Node *node_where_to_stop = node;
    Node *current = node;
    // do the extra step to the smallest sibling if it is not a root node
    if (current->parent != nullptr) {
      node_where_to_stop = current->sibling;
      current = current->sibling;
    }
    do {
      uint number_of_whitespaces = 2 * level;
      char *line_output_space = new char[number_of_whitespaces]();
      for (uint i = 0; i < number_of_whitespaces; i++) {
        line_output_space[i] = ' ';
      }
      std::cout << line_output_space << current->entry->prio << " "
                << current->entry->data << std::endl;
      delete[] line_output_space;
      display_nodes_of_tree(current->child, level + 1);
      current = current->sibling;
    } while (current != node_where_to_stop && current != nullptr);
  }

  // Inhalt der Halde zu Testzwecken ausgeben.
  void dump() {
    // display the nodes in the binheap
    display_nodes_of_tree(head, 0);
  };
};

//------------------------------------------------------------------------------------