#include <iostream>
#include <string>
using namespace std;

#ifdef WGRAPH
#include "wgraph.h"
#else
#include "graph.h"
#endif

// V als AbkÃ¼rzung fÃ¼r den hier konkret verwendeten Knotentyp string.
// (Nicht zu verwechseln mit dem Schablonenparameter V der in graph.h
// definierten Typen und Funktionen.)
using V = string;

// Reihe mit Testgraphen des Typs Graph<V>.
// (FÃ¼r eigene Tests kÃ¶nnen beliebige weitere Graphen hinzugefÃ¼gt werden.)
// Damit spÃ¤ter auch Graphen des Untertyps WeightedGraph<V> in derselben
// Reihe gespeichert werden kÃ¶nnen, werden Zeiger auf den Basistyp Graph<V>
// gespeichert, die dann bei Bedarf in Zeiger auf den Untertyp
// WeightedGraph<V> umgewandelt werden.
Graph<V> *graphs[] = {
    // Beispiel eines ungewichteten Graphen.
    new Graph<V>({
        {"A", {"B", "C"}}, // Knoten A hat Nachfolger B und C.
        {"B", {}},         // Knoten B hat keine Nachfolger.
        {"C", {"C"}}       // Knoten C hat sich selbst als Nachfolger.
    }),
    // Ein weiterer ungewichteter Graph.
    new Graph<V>({{"A", {"B", "C", "D"}},
                  {"B", {"E", "F"}},
                  {"C", {"G"}},
                  {"D", {"H"}},
                  {"E", {"I", "J"}},
                  {"F", {}},
                  {"G", {"K"}},
                  {"H", {"L"}},
                  {"I", {}},
                  {"J", {"M"}},
                  {"K", {}},
                  {"L", {}},
                  {"M", {"N", "O"}},
                  {"N", {}},
                  {"O", {}}}),
    new Graph<V>({{"A", {"B", "D", "F"}},
                  {"B", {}},
                  {"C", {"D", "G"}},
                  {"D", {"E", "F"}},
                  {"E", {"A", "F"}},
                  {"F", {"B"}},
                  {"G", {"C", "E", "G"}}}),

#ifdef WGRAPH
    // Beispiel eines gewichteten Graphen.
    new WeightedGraph<V>({
        {"A", {{"B", 2}, {"C", 3}}},
        // Kanten A -> B mit Gewicht 2 und A -> C mit Gewicht 3.
        {"B", {}},        // Keine Kanten B -> ...
        {"C", {{"C", 4}}} // Kante C -> C mit Gewicht 4.
    }),
    new WeightedGraph<V>({{"A", {{"B", 4}, {"C", 2}, {"D", 7}}},
                          {"B", {{"A", 4}, {"E", 1}, {"F", 3}}},
                          {"C", {{"A", 2}, {"D", 3}, {"G", 8}}},
                          {"D", {{"A", 7}, {"C", 3}, {"H", 4}}},
                          {"E", {{"B", 1}, {"F", 6}}},
                          {"F", {{"B", 3}, {"E", 6}, {"G", 2}}},
                          {"G", {{"C", 8}, {"F", 2}, {"H", 1}}},
                          {"H", {{"D", 4}, {"G", 1}}},
                          {"I", {{"J", 5}}},
                          {"J", {{"I", 5}, {"K", 7}}},
                          {"K", {{"J", 7}}}}),
    new WeightedGraph<V>( // 5
        {{"A", {{"B", 3}, {"E", 1}, {"F", 5}}},
         {"B", {{"A", 3}, {"F", 5}, {"E", 2}, {"D", 7}, {"C", 8}}},
         {"C", {{"B", 8}, {"E", 7}, {"D", 5}}},
         {"D", {{"C", 5}, {"B", 7}, {"E", 8}}},
         {"E", {{"F", 4}, {"A", 1}, {"B", 2}, {"C", 7}, {"D", 8}}},
         {"F", {{"A", 5}, {"B", 5}, {"E", 4}}}}
    ),
    new WeightedGraph<V>({
        {"A", {{"D", 4}}},
        {"B", {{"A", 3}}},
        {"C", {{"A", 6}, {"B", 2}}},
        {"D", {{"B", 5}, {"C", 1}}}
    }),
    new WeightedGraph<V>(
        {
            {"E", {{"G", 1}, {"F", 1}}},
            {"F", {{"G", -1}}},
            {"G", {{"F", -1}}}
        }
    )
#endif
};

// Weg vom Startknoten s zum Knoten v anhand der VorgÃ¤ngerinformation
// in res ausgeben.
void path(V s, V v, Pred<V> &res) {
  if (s != v && res.pred[v] != res.NIL) {
    path(s, res.pred[v], res);
    cout << " -> ";
  }
  cout << v;
}

// Hauptprogramm.
// Auswahl des Algorithmus durch das erste Kommandozeilenargument:
// bfs -> breadth first search
// dfs -> depth first search
// sort -> topological sort
// scc -> strongly connected components
// prim -> Prim
// bell -> Bellman-Ford
// dijk -> Dijkstra
// Auswahl des Testgraphen durch das zweite Kommandozeilenargument.
// (Bei den Algorithmen prim, bell und dijk muss ein gewichteter
// Graph ausgewÃ¤hlt werden.)
// Auswahl des Startknotens durch das optionale dritte
// Kommandozeilenargument (Standardwert ist "A").
int main(int argc, char *argv[]) {
  // Kommandozeilenargumente.
  string a = argv[1];                  // Algorithmus.
  Graph<V> *g = graphs[stoi(argv[2])]; // Graph.
  V s = argc > 3 ? argv[3] : "A";      // Startknoten.

  // GewÃ¼nschten Algorithmus ausfÃ¼hren und sein Ergebnis ausgeben.
  if (a == "bfs") {
    BFS<V> res;

    bfs(*g, s, res);
    for (V v : g->vertices()) {
      path(s, v, res);
      uint d = res.dist[v];
      if (d == res.INF)
        cout << " inf" << endl;
      else
        cout << " " << d << endl;
    }
  } else if (a == "dfs") {
    DFS<V> res;
    dfs(*g, res);
    for (V v : res.seq) {
      cout << v << " " << res.det[v] << " " << res.fin[v] << endl;
    }
  } else if (a == "sort") {
    list<V> res;
    if (topsort(*g, res)) {
      for (V v : res)
        cout << v << endl;
    } else {
      cout << "cycle" << endl;
    }
  } else if (a == "scc") {
    list<list<V>> res;
    scc(*g, res);
    for (list<V> c : res) {
      c.sort();
      for (V v : c)
        cout << v << " ";
      cout << endl;
    }
  }
#ifdef WGRAPH
  else if (a == "prim") {
    Pred<V> res;
    WeightedGraph<V> extractedGraph = *(WeightedGraph<V> *)g;
    prim(extractedGraph, s, res);

    for (V v : g->vertices()) {
      path(s, v, res);
      cout << endl;
    }
  } else if (a == "bell") {
    SP<V> res;
    if (bellmanFord(*(WeightedGraph<V> *)g, s, res)) {
      for (V v : g->vertices()) {
        path(s, v, res);
        cout << " " << res.dist[v] << endl;
      }
    } else {
      cout << "negative cycle" << endl;
    }
  } else if (a == "dijk") {
    SP<V> res;
    dijkstra(*(WeightedGraph<V> *)g, s, res);
    for (V v : g->vertices()) {
      path(s, v, res);
      cout << " " << res.dist[v] << endl;
    }
  }
#endif
  else {
    cout << "unknown algorithm: " << a << endl;
  }
}
