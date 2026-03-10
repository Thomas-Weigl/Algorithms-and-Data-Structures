#include "graph.h"
#include "prioqueue.h"

/*
 *  Gewichtete Graphen
 */

// Gerichteter gewichteter Graph als Unterklasse von Graph<V>.
// (Ein ungerichteter gewichteter Graph kann als gerichteter gewichteter
// Graph reprÃ¤sentiert werden, bei dem jede Kante in beiden Richtungen
// mit dem gleichen Gewicht vorhanden ist.)
template <typename V>
struct WeightedGraph : Graph<V> {
    // Tabelle mit Kantengewichten.
    map<pair<V, V>, double> wt;

    // Initialisierung mit der um Kantengewichte erweiterten
    // Adjazenzlistendarstellung a.
    // Damit ist auch eine Initialisierung mit einer passenden
    // (verschachtelten) Initialisiererliste in geschweiften Klammern
    // mÃ¶glich, zum Beispiel:
    // { { "A", { { "B", 2 }, { "C", 3 } } }, { "B", { } },
    //					{ "C", { { "C", 4 } } } }
    WeightedGraph (map<V, list<pair<V, double>>> a) : Graph<V>({}) {
	// Die erweiterte Adjazenzlistendarstellung a durchlaufen und
	// mit der darin enthaltenen Information die (von Graph<V>
	// geerbte) einfache Adjazenzlistendarstellung adj und die
	// Gewichtstabelle wt passend fÃ¼llen. 
	// Wenn ein Knoten u keine Nachfolger besitzt, muss er trotzdem
	// (mit einer leeren Nachfolgerliste) in adj eingetragen werden,
	// damit er von vertices gefunden wird.
	for (auto p : a) {
	    V u = p.first;
	    Graph<V>::adj[u];
	    for (auto q : p.second) {
		V v = q.first;
		double w = q.second;
		Graph<V>::adj[u].push_back(v);
		wt[{u,v}] = w;
	    }
	}
    }

    // Gewicht der Kante (u, v) liefern.
    double weight (V u, V v) {
	return wt[{u,v}];
    }
};

/*
 *  Datenstrukturen zur Speicherung der Ergebnisse der Algorithmen
 */

// Ergebnis der Shortest-path-Algorithmen Bellman-Ford und Dijkstra:
// Durch Mehrfachvererbung gebildete Kombination von Pred<V> und
// Dist<V, double> mit Elementvariablen pred, NIL, dist und INF.
template <typename V>
struct SP : Pred<V>, Dist<V, double> {};

/*
 *  Algorithmen
 */

// MinimalgerÃ¼st des Graphen g mit dem modifizierten Algorithmus von
// Prim mit Startknoten s bestimmen und das Ergebnis in res speichern.
// Der Graph muss ungerichtet sein, d. h. jede Kante muss in beiden
// Richtungen mit dem gleichen Gewicht vorhanden sein.
// (Dies muss nicht Ã¼berprÃ¼ft werden.)
// Achtung: res enthÃ¤lt keine Tabelle dist und damit auch keinen Wert
// INF, weil die dist-Werte nur wÃ¤hrend der AusfÃ¼hrung des Algorithmus
// benÃ¶tigt werden, aber nicht fÃ¼r das Ergebnis.
// Trotzdem kann die Funktion intern natÃ¼rlich ein entsprechendes
// Dist-Objekt verwenden.
template <typename V, typename G>
void prim(G g , V s, Pred<V>& res) {
    Dist<V,double> dist;
    PrioQueue<double, V> pq;
    map<V, Entry<double, V>*> entries; // zu jedem knoten entrypointer

    for(V v : g.vertices()) if (v != s) {
        dist.dist[v] = dist.INF;
        entries[v] = pq.insert(dist.INF, v);
        res.pred[v] = res.NIL;
    }
    res.pred[s] = res.NIL;

    V u = s;
    while(!pq.isEmpty()) {
        for (V v : g.successors(u)) {
            if(pq.contains(entries[v]) && g.weight(u, v) < dist.dist[v]) {
                pq.changePrio(entries[v], g.weight(u,v));
                dist.dist[v] = g.weight(u, v);
                res.pred[v] = u;
            }
        }
        u = pq.extractMinimum()->data; 
    }

}

// KÃ¼rzeste Wege vom Startknoten s zu allen Knoten des Graphen g mit
// dem Algorithmus von Bellman-Ford ermitteln und das Ergebnis in res
// speichern.
// Resultatwert true, wenn es im Graphen keinen vom Startknoten aus
// erreichbaren Zyklus mit negativem Gewicht gibt, andernfalls false.
// (Im zweiten Fall darf der Inhalt von res danach undefiniert sein.)
template <typename V, typename G>
bool bellmanFord (G g, V s, SP<V>& res) {
    for(V v : g.vertices()){
        res.dist[v] = res.INF;
        res.pred[v] = res.NIL;
    }
    res.dist[s] = 0;

    V u = s;
    for(int rep = 0; rep < (g.vertices().size() -1); rep++) {
       for (V u : g.vertices()) {
            for (V v : g.successors(u)) {
                //Verwerten einer Kante
                if(res.dist[u] + g.weight(u, v) < res.dist[v]) {
                    res.dist[v] = res.dist[u] + g.weight(u, v);
                    res.pred[v] = u; // speichere Weg u als neuen kürzesten Weg nach v
                }
            }
       }
    }

    for (V u : g.vertices()) {
        for (V v : g.successors(u)) {
            if(res.dist[u] + g.weight(u, v) < res.dist[v]) {
                    return false;
            }
        }
    }
    return true;
}

// KÃ¼rzeste Wege vom Startknoten s zu allen Knoten des Graphen g mit
// dem Algorithmus von Dijkstra ermitteln und das Ergebnis in res
// speichern.
// Die Kanten des Graphen dÃ¼rfen keine negativen Gewichte besitzen.
// (Dies muss nicht Ã¼berprÃ¼ft werden.)
template <typename V, typename G>
void dijkstra (G g, V s, SP<V>& res) {
    PrioQueue<double, V> pq;
    map<V, Entry<double, V>*> entries;


    for(V v : g.vertices()){
        res.dist[v] = res.INF;
        res.pred[v] = res.NIL;
    }
    res.dist[s] = 0;

    for (V v : g.vertices()) {
        entries[v] = pq.insert(res.dist[v], v);
    }

    V u;
    while(!pq.isEmpty()) {
        u = pq.extractMinimum()->data;
        for(V v : g.successors(u)) if(pq.contains(entries[v])) {
            Dist<V,double> res_prev;
            res_prev.dist[v] = res.dist[v];
            if(res.dist[u] + g.weight(u, v) < res.dist[v]) {
                    res.dist[v] = res.dist[u] + g.weight(u, v);
                    res.pred[v] = u; // speichere Weg u als neuen kürzesten Weg nach v
            }
            if(res.dist[v] < res_prev.dist[v]) {
                pq.changePrio(entries[v], res.dist[v]);
            }
        }
    }
}