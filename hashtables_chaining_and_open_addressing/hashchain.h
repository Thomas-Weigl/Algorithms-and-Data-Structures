#include <iostream>
using namespace std;

// Vorzeichenlose ganze Zahl.
using uint = unsigned int;

template <typename K, typename V> struct HashChain {

  struct Elem {
    K key;
    V val;
    Elem *next;
    Elem(K k, V v) : key(k), val(v) {}
  };

  Elem **tab; // elem pointer pointer
  uint size;

  HashChain(uint n) : size(n) {
    tab = new Elem *[n](); // tab zeigt auf das erste Element des Array, welches
                           // aus Elem Zeigern besteht
  }

  bool put(K k, V v) {
    int h = hashval(k);
    int index = (h >= 0 ? h : -h) % size;

    Elem *current = tab[index];
    while (current != nullptr) {
      if (current->key == k) {
        current->val = v; // gibt es schon ->ersetzen
        return true;
      } else {
        current = current->next;
      }
    }
    Elem *newElem = new Elem(k, v); // erstes Listenelement -> einfÃ¼gen
    newElem->next = tab[index];
    tab[index] = newElem;
    return true;
  }

  bool get(K k, V &v) {
    int h = hashval(k);
    int index = (h >= 0 ? h : -h) % size;

    Elem *current = tab[index];
    while (current != nullptr) {
      if (current->key == k) {
        v = current->val;
        return true;
      } else {
        current = current->next;
      }
    }
    return false;
  }

  bool remove(K k) {
    int h = hashval(k);
    int index = (h >= 0 ? h : -h) % size;

    Elem *current = tab[index];
    Elem *prev = nullptr;
    while (current != nullptr) {
      if (current->key == k) {
        if (prev == nullptr) {
          tab[index] = current->next;

        } else {
          prev->next = current->next;
        }
        delete current;
        return true;
      } else {
        prev = current;
        current = current->next;
      }
    }
    return false;
  }

  void dump() {
    for (uint i = 0; i < size; i++) {
      Elem *current = tab[i];
      while (current != nullptr) {
        std::cout << i << " " << current->key << " " << current->val
                  << std::endl;
        current = current->next;
      }
    }
  }
};
