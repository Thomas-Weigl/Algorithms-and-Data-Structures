#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "binheap.h"

// ZufÃ¤lligen Index zwischen 0 und n - 1 liefern.
uint rand_idx (uint n) {
    return rand() % n;
}

// ZufÃ¤llige Zeichenkette bestehend aus n Kleinbuchstaben liefern.
string rand_str (uint n) {
    string s;
    for (uint i = 0; i < n; i++) {
	char c = 'a' + rand() % 26;
	s += c;
    }
    return s;
}

// Interaktives Testprogramm fÃ¼r die Klasse BinHeap.
int main (int argc, char** argv) {
    // Wenn ein beliebiges Kommandozeilenargument angegeben ist, wird
    // angenommen, dass die Befehle aus einer Datei oder Pipe gelesen
    // werden und deshalb nicht sichtbar sind (Batch-Modus), sonst von
    // der Konsole (interaktiver Modus).
    // Im Batch-Modus ist batch ein Alias fÃ¼r cout und inter ein
    // Dummy-Ausgabestrom, der ins Leere geht, im interaktiven Modus
    // umgekehrt.
    ostream batch(argc > 1 ? cout.rdbuf() : nullptr);
    ostream inter(argc == 1 ? cout.rdbuf() : nullptr);

    // Halde mit PrioritÃ¤ten des Typs string und zugehÃ¶rigen Daten des
    // Typs uint.
    // (Die Implementierung muss aber natÃ¼rlich auch mit anderen
    // Typen funktionieren.)
    // Der Kleiner-Operator fÃ¼r string implementiert einen
    // lexikographischen Vergleich.
    BinHeap<string, uint> heap;

    // Container mit allen jemals eingefÃ¼gten EintrÃ¤gen, damit sie
    // spÃ¤ter fÃ¼r remove und changePrio verwendet werden kÃ¶nnen.
    vector<BinHeap<string, uint>::Entry*> entrys;

    // Anzahl aller bis jetzt eingefÃ¼gten EintrÃ¤ge.
    uint n = 0;

    // Anzahl der EintrÃ¤ge, die jetzt noch in der Halde sein mÃ¼ssen.
    uint m = 0;

    // Endlosschleife.
    while (true) {
	// GrÃ¶ÃŸe und Inhalt der Halde ausgeben und die GrÃ¶ÃŸe Ã¼berprÃ¼fen.
	heap.dump();
	uint s = heap.size();
	cout << s << " entry(s)" << endl;
	if (s != m) {
	    cout << "heap size should be " << m << endl;
	    return 1;
	}

	// Befehl +/-/=/?/!, ggf. mit zugehÃ¶rigen Werten, von der
	// Standardeingabe lesen und die entsprechende Operation auf
	// der Halde ausfÃ¼hren.
	// ++/--/== anstelle von +/-/= fÃ¼hrt die entsprechende
	// Operation mit zufÃ¤lligen Werten aus.
	// Der Ausgabestrom args ist bei zufÃ¤lligen Werten ein Alias
	// fÃ¼r cout, sonst fÃ¼r batch, damit zufÃ¤llige Werte immer und
	// interaktiv eingegebene Werte nur im Batch-Modus ausgegeben
	// werden.

	string cmd, prio;
	uint entry;
	BinHeap<string, uint>::Entry* e;

	inter << "cmd: ";
	if (!(cin >> cmd)) return 0;
	batch << cmd;
	bool rand = cmd.size() == 2;
	if (rand) inter << "       ";
	ostream& args = rand ? cout : batch;

	switch (cmd[0]) {
	case '+': // insert mit Wert prio
	    // Die laufende Nummer n wird als zusÃ¤tzliche Daten
	    // verwendet.
	    if (!rand) cin >> prio;
	    else prio = rand_str(2);
	    args << " " << prio << endl;
	    entrys.push_back(heap.insert(prio, n));
	    n++;
	    m++;
	    break;
	case '-': // remove mit Wert entry
	    if (!rand) cin >> entry;
	    else entry = rand_idx(n);
	    args << " " << entry << endl;
	    if (heap.remove(entrys[entry])) {
		cout << "true" << endl;
		m--;
	    }
	    else {
		cout << "false" << endl;
	    }
	    break;
	case '=': // changePrio mit Werten entry und prio
	    if (!rand) cin >> entry >> prio;
	    else entry = rand_idx(n), prio = rand_str(2);
	    args << " " << entry << " " << prio << endl;
	    if (heap.changePrio(entrys[entry], prio)) {
		cout << "true" << endl;
	    }
	    else {
		cout << "false" << endl;
	    }
	    break;
	case '?': // minimum
	    batch << endl;
	    if (e = heap.minimum()) {
		cout << "minimum: " << e->prio << " " << e->data
							    << endl;
	    }
	    break;
	case '!': // extractMin
	    batch << endl;
	    if (e = heap.extractMin()) {
		cout << "minimum: " << e->prio << " " << e->data
							    << endl;
		m--;
	    }
	    break;
	case 'q': // quit
	    batch << endl;
	    return 0;
	}
    }
}
