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
Graph<V>* graphs [] = {
    // Beispiel eines ungewichteten Graphen.
	new Graph<V>({
     { "A", { "B", "C" } },    // Knoten A hat Nachfolger B und C.
    { "B", { } },        // Knoten B hat keine Nachfolger.
    { "C", { "C" } }    // Knoten C hat sich selbst als Nachfolger.
    }),

    new Graph<V>({ //vorlesung G1
    { "A", {"B","D", "F"}},
    { "B", {} },
    { "C", {"D", "G"} },
    { "D", {"E", "F"} },
    {"E", {"A","F"} },
    {"F", {"B"} },
    { "G", {"C", "E", "G"} }
    }),

    new Graph<V>({ //Übung 1 G2
    { "A", {"A","B"}},
    { "B", {"A","C", "D"} },
    { "C", {"B", "D"} },
    { "D", {"B"} },
    {"E", {"D","F"} },
    {"F", {"E", "F"} }
    }),

    new Graph<V>({ //Übung 2 G3
    { "A", {"B","H"}},
    { "B", {"C", "F"} },
    { "C", {"D", "G"} },
    { "D", {"E", "G", "H"} },
    {"E", {} },
    {"F", {"A", "B", "E", "G"} },
    { "G", {"H"} },
    { "H", {"C"} },
    }),

    new Graph<V>({ //Vorlesung G4
    { "Hose", {"Unterwaesche"}}, //passt
    { "Jacke", {"Hemd"}}, //passt
    {"Guertel", {"Hose"} }, //passt
    {"Hemd", {"Unterwaesche"}}, //passt
    {"Unterwaesche", {}}, //passt
    {"Uhr", {}}, //passt
    {"Krawatte", {"Hemd"}}, //passt
    {"Socken", {}}, //past
    {"Schuhe", {"Socken", "Hose"}},
    }),
 

    #ifdef WGRAPH
    // Beispiel eines gewichteten Graphen.
    new WeightedGraph<V>({
	{ "A", { { "B", 2 }, { "C", 3 } } },
		// Kanten A -> B mit Gewicht 2 und A -> C mit Gewicht 3.
	{ "B", { } },		// Keine Kanten B -> ...
	{ "C", { { "C", 4 } } }	// Kante C -> C mit Gewicht 4.
    }),
    #endif
};

// Weg vom Startknoten s zum Knoten v anhand der VorgÃ¤ngerinformation
// in res ausgeben.
void path (V s, V v, Pred<V>& res) {
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
int main (int argc, char* argv []) {
    // Kommandozeilenargumente.
    string a = argv[1];				// Algorithmus.
    Graph<V>* g = graphs[stoi(argv[2])];	// Graph.
    V s = argc > 3 ? argv[3] : "A";		// Startknoten.

    // GewÃ¼nschten Algorithmus ausfÃ¼hren und sein Ergebnis ausgeben.
    if (a == "bfs") {
	BFS<V> res;
	bfs(*g, s, res);
	for (V v : g->vertices()) {
	    path(s, v, res);
	    uint d = res.dist[v];
	    if (d == res.INF) cout << " inf" << endl;
	    else cout << " " << d << endl;
	}
    }
    else if (a == "dfs") {
	DFS<V> res;
	dfs(*g, res);
	for (V v : res.seq) {
	    cout << v << " " << res.det[v] << " " << res.fin[v] << endl;
	}
    }
    else if (a == "sort") {
	list<V> res;
	if (topsort(*g, res)) {
	    for (V v : res) cout << v << endl;
	}
	else {
	    cout << "cycle" << endl;
	}
    }
    else if (a == "scc") {
	list<list<V>> res;
	scc(*g, res);
	for (list<V> c : res) {
	    c.sort();
	    for (V v : c) cout << v << " ";
	    cout << endl;
	}
    }
    #ifdef WGRAPH
    else if (a == "prim") {
	Pred<V> res;
	prim(*(WeightedGraph<V>*)g, s, res);
	for (V v : g->vertices()) {
	    path(s, v, res);
	    cout << endl;
	}
    }
    else if (a == "bell") {
	SP<V> res;
	if (bellmanFord(*(WeightedGraph<V>*)g, s, res)) {
	    for (V v : g->vertices()) {
		path(s, v, res);
		cout << " " << res.dist[v] << endl;
	    }
	}
	else {
	    cout << "negative cycle" << endl;
	}
    }
    else if (a == "dijk") {
	SP<V> res;
	dijkstra(*(WeightedGraph<V>*)g, s, res);
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
