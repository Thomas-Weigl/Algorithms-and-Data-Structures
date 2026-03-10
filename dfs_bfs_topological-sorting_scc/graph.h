#include <limits>
#include <list>
#include <map>
#include <utility> // pair
using namespace std;

// Vorzeichenlose ganze Zahl.
using uint = unsigned int;

/*
 *  Graphen
 */

// Gerichteter Graph mit Knoten des Typs V.
// (Ein ungerichteter Graph kann als gerichteter Graph reprÃ¤sentiert
// werden, bei dem jede Kante in beiden Richtungen vorhanden ist.)
template <typename V> struct Graph {
  // Adjazenzlistendarstellung des Graphen als Tabelle (map),
  // die zu jedem Knoten die Liste seiner Nachfolger enthÃ¤lt.
  map<V, list<V>> adj;

  // Initialisierung mit der Adjazenzlistendarstellung a.
  // Damit ist auch eine Initialisierung mit einer passenden
  // (verschachtelten) Initialisiererliste in geschweiften Klammern
  // mÃ¶glich, zum Beispiel:
  // { { "A", { "B", "C" } }, { "B", { } }, { "C", { "C" } } }
  Graph(map<V, list<V>> a) : adj(a) {}

  // Container mit allen Knoten des Graphen liefern.
  list<V> vertices() {
    // Alle Paare p der Tabelle adj durchlaufen
    // und jeweils ihren ersten Bestandteil p.first
    // am Ende der Liste vs anfÃ¼gen.
    list<V> vs;
    for (pair<V, list<V>> p : adj)
      vs.push_back(p.first);
    return vs;
  }

  // Container mit allen Nachfolgern des Knotens v liefern.
  list<V> successors(V v) {
    // Die zum Knoten v in der Tabelle adj gespeicherte
    // Liste von Nachfolgern liefern.
    return adj[v]; // zugreifen auf den Wert der in v gespeichert ist = die
                   // liste der nachfolger
  }

  // Transponierten Graphen als neues, unabhÃ¤ngiges Objekt liefern.
  Graph<V> transpose() {
    map<V, list<V>> transposed;
    for (auto p : adj) {        // alle Keys durchsuchen
      for (auto q : p.second) { // alle Elemente in den Values durchsuchen
        transposed[q].push_back(p.first);
        transposed[p.first]; // er meinte vertrau mir brudi du brauchst das
      }
    }
    return transposed;

    // Idee: In einer Ã¤uÃŸeren Schleife alle Knoten u des Graphen
    // durchlaufen. In einer inneren Schleife alle Nachfolger v
    // von u durchlaufen und dabei jeweils u als Nachfolger von v
    // zu einer neuen Adjazenzlistendarstellung a des
    // transponierten Graphen hinzufÃ¼gen.
    // Zum Schluss a an den Konstruktor von Graph<V> Ã¼bergeben.
    // Hinweis: Wenn die Tabelle a noch keinen Eintrag fÃ¼r den
    // Knoten v enthÃ¤lt, erzeugt a[v] automatisch einen neuen
    // Eintrag mit einer leeren Liste von Nachfolgern, auf die
    // direkt push_back angewandt werden kann.
  }
};

/*
 *  Datenstrukturen zur Speicherung der Ergebnisse der Algorithmen
 */

// Teil des Ergebnisses einer Breitensuche sowie spÃ¤ter auch Ergebnis
// von Prim und Teil des Ergebnisses von Bellman-Ford und Dijkstra.
template <typename V> struct Pred {
  // Tabelle zur Speicherung des VorgÃ¤ngers pred[v] eines Knotens v.
  map<V, V> pred;

  // Ersatzwert NIL, der in pred[v] gespeichert wird, wenn es zu
  // einem Knoten v keinen VorgÃ¤nger gibt.
  // StandardmÃ¤ÃŸig der Wert, den der parameterlose Konstruktor von V
  // liefert. Bei Bedarf kann vor der AusfÃ¼hrung eines Algorithmus
  // aber auch ein anderer Wert an NIL zugewiesen werden, der nicht
  // als echter Knotenwert auftritt.
  V NIL = V();
};

// Teil des Ergebnisses einer Breitensuche (mit N gleich uint)
// sowie spÃ¤ter auch von Bellman-Ford und Dijkstra (mit N gleich double).
template <typename V, typename N> struct Dist {
  // Tabelle zur Speicherung der Distanz dist[v] mit numerischem Typ N
  // zwischen dem Startknoten s und dem Knoten v.
  map<V, N> dist;

  // Ersatzwert INF, der in dist[v] gespeichert wird, wenn die Distanz
  // zwischen s und v unendlich ist.
  // Der Wert INF ist entweder "unendlich" (falls verfÃ¼gbar) oder der
  // grÃ¶ÃŸtmÃ¶gliche endliche Wert des Typs N.
  // (Gleitkommatypen sind Ã¼blicherweise gemÃ¤ÃŸ IEEE 754 implementiert
  // und besitzen dann "unendlich" als Wert, ganzzahlige Typen jedoch
  // nicht.)
  static constexpr N INF = numeric_limits<N>::has_infinity
                               ? numeric_limits<N>::infinity()
                               : numeric_limits<N>::max();
};

// Ergebnis einer Breitensuche:
// Durch Mehrfachvererbung gebildete Kombination von Pred<V> und
// Dist<V, uint> mit Elementvariablen pred, NIL, dist und INF.
template <typename V> struct BFS : Pred<V>, Dist<V, uint> {};

// Ergebnis einer Tiefensuche.
template <typename V> struct DFS {
  // Tabellen zur Speicherung der Entdeckungszeit det[v] und der
  // Abschlusszeit fin[v] eines Knotens v.
  // Beide Zeitwerte liegen zwischen 1 und der doppelten Knotenzahl
  // des Graphen.
  map<V, uint> det, fin;

  // Liste zur Speicherung aller Knoten des Graphen nach aufsteigenden
  // Abschlusszeiten, die damit gleichzeitig das Ergebnis einer
  // erfolgreichen topologischen Sortierung ist.
  // (Die Liste muss aber von jeder Tiefensuche gefÃ¼llt werden, auch
  // wenn sie nicht zur topologischen Sortierung verwendet wird.)
  list<V> seq;
};

/*
 *  Algorithmen
 */

// Breitensuche im Graphen g mit Startknoten s ausfÃ¼hren
// und das Ergebnis in res speichern.
template <typename V, typename G> void bfs(G g, V s, BFS<V> &res) {
  for (V v : g.vertices()) {
    res.dist[v] = res.INF;
    res.pred[v] = res.NIL;
  }
  res.dist[s] = 0;

  list<V> q;
  q.push_back(s);

  while (q.size() > 0) {
    V u = q.front();
    q.pop_front();

    for (V v : g.successors(u)) {
      if (res.dist[v] == res.INF) {
        res.dist[v] = res.dist[u] + 1;
        res.pred[v] = u;
        q.push_back(v);
      }
    }
  }
}

// Tiefensuche im Graphen g ausfÃ¼hren und das Ergebnis in res speichern.
// In der Hauptschleife des Algorithmus werden die Knoten in der
// Reihenfolge des Containers g.vertices() durchlaufen.
template <typename V, typename G>
void dfs(G g, DFS<V> &res) { // in beliebiger reihenfolge durchlaufen
  bool cycle = false;
  map<V, int> status; // 0 = weiß, 1 = grau, 2 = schwarz
  for (V u : g.vertices()) {
    status[u] = 0;
  }

  uint counter = 1;

  for (V u : g.vertices()) {
    if (status[u] == 0) {
      counter = recursive(g, res, status, counter, u, cycle) + 1;
    }
  }
}

template <typename V, typename G>
uint recursive(G &g, DFS<V> &res, map<V, int> &status, uint counter, V u,
               bool &cycle) {
  // ev prüfung ob < 2 * size()
  res.det[u] = counter;
  status[u] = 1;
  for (V v : g.successors(u)) {
    if (status[v] == 0) {
      counter++;
      counter = recursive(g, res, status, counter, v, cycle);
    } else if (status[v] == 1) {
      cycle = true;
    }
  }
  counter++;
  res.fin[u] = counter;
  res.seq.push_back(u);
  status[u] = 2;
  return counter;
}

// Tiefensuche im Graphen g ausfÃ¼hren und das Ergebnis in res speichern.
// In der Hauptschleife des Algorithmus werden die Knoten in der
// Reihenfolge der Liste vs durchlaufen.
template <typename V, typename G>
void dfs(
    G g, list<V> vs,
    DFS<V>
        &res) { // zusätzlich vs, wird in reihenfolge der liste zu durchlaufen,
                //  für 2. tiefensuche bei zusammenhangskomponente, nämlchg nach
                //  absteigenden abschlussziten aus der 1. tiefensuche
                // ev ruft eine die andere funktion auf
                //  ich gehe nicht über vertices sondern übr liste
  map<V, int> status;
  for (V u : g.vertices()) {
    status[u] = 0;
  }

  uint counter = 1;
  bool cycle = false;
  for (V u : vs) {
    if (status[u] == 0) {
      counter = recursive(g, res, status, counter, u, cycle) + 1;
    }
  }
}

// Topologische Sortierung des Graphen g ausfÃ¼hren und das Ergebnis
// als Liste von Knoten in res speichern; fÃ¼r jeden Knoten befinden
// sich alle seine Nachfolger weiter vorne in der Liste.
// Resultatwert true, wenn dies mÃ¶glich ist,
// false, wenn der Graph einen Zyklus enthÃ¤lt.
// (Im zweiten Fall darf der Inhalt von res danach undefiniert sein.)
template <typename V, typename G>
bool topsort(
    G g, list<V> &res) { // kann dfs aufrufen //checkt ob es geht deshalb bool
  DFS<V> DFS_object;
  bool cycle = false;
  map<V, int> status; // 0 = weiß, 1 = grau, 2 = schwarz
  for (V u : g.vertices()) {
    status[u] = 0;
  }
  uint counter = 1;
  for (V u : g.vertices()) {
    if (status[u] == 0) {
      counter = recursive(g, DFS_object, status, counter, u, cycle) + 1;
    }
  }

  if (!cycle) {
    for (V x : DFS_object.seq) {
      res.push_back(x);
    }
    return true;
  } else {
    return false;
  }
}

// Die starken Zusammenhangskomponenten des Graphen g ermitteln
// und das Ergebnis als Liste von Listen von Knoten in res speichern.
// Jedes Element von res enthÃ¤lt die Knoten einer starken
// Zusammenhangskomponente in beliebiger Reihenfolge.
// Die Reihenfolge der Elemente von res kann ebenfalls beliebig sein.
template <typename V, typename G> void scc(G g, list<list<V>> &res) {
  DFS<V> DFS_object;
  DFS<V> DFS_object2;
  dfs(g, DFS_object);
  list<V> reversed_order = DFS_object.seq;
  reversed_order.reverse();
  dfs(g.transpose(), reversed_order, DFS_object2);
  map<V, bool> finished;
  for (V z : reversed_order) {
    finished[z] = false;
  }
  for (V x : reversed_order) {
    if (!finished[x]) {
      res.push_back({x});
      finished[x] = true;
      for (V y : reversed_order) {
        if (DFS_object2.fin[y] < DFS_object2.fin[x] && !finished[y]) {
          res.back().push_back(y);
          finished[y] = true;
        }
      }
    }
  }
}