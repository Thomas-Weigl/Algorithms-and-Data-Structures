
#include <iostream>
#include <string>
using namespace std;

#include "hashchain.h"
#ifdef HASHOPEN
#include "hashopen.h"
#endif

// Punkt im zweidimensionalen Raum.
struct Point {
  // Koordinaten des Punkts.
  int x, y;

  // Initialisierung mit den Koordinaten x und y.
  Point(int x, int y) : x(x), y(y) {}

  // Parameterloser Konstruktor, der z. B. benÃ¶tigt wird, wenn Point
  // als Elementtyp (oder als Bestandteil des Elementtyps) einer Reihe
  // verwendet wird.
  Point() : x(0), y(0) {}
};

// Sind die Punkte p1 und p2 inhaltlich gleich?
// Durch diese Definition von operator== kÃ¶nnen Point-Objekte
// mit dem normalen Gleichheitsoperator (==) verglichen werden,
// was insbesondere in HashChain und HashOpen benÃ¶tigt wird.
bool operator==(Point p1, Point p2) {
  // Hier wird zweimal der Gleichheitsoperator fÃ¼r int verwendet.
  return p1.x == p2.x && p1.y == p2.y;
}

// Der Ungleichheitsoperator (!=) muss aber bei Bedarf zusÃ¤tzlich
// definiert werden.
bool operator!=(Point p1, Point p2) {
  // Hier wird der zuvor definierte Gleichheitsoperator fÃ¼r Point
  // verwendet.
  return !(p1 == p2);
}

// Streuwert des Punkts p berechnen und zurÃ¼ckliefern.
uint hashval(Point p) {
  // Die Berechnung ist bewusst (zu) einfach, damit man zum Testen
  // leicht Punkte mit dem gleichen Streuwert erzeugen kann.
  return p.x + p.y;
}

#ifdef HASHOPEN
// Zweite Streuwertfunktion fÃ¼r Punkte p bei TabellengrÃ¶ÃŸe n.
uint hashval2(Point p, uint n) {
  // Damit die Werte h immer teilerfremd zur TabellengrÃ¶ÃŸe n sind,
  // wird immer eine ungerade Zahl von 1 bis n-1 geliefert.
  // Dann kÃ¶nnen sowohl Zweierpotenzen als auch Primzahlen als
  // TabellengrÃ¶ÃŸen verwendet werden.
  uint h = hashval(p) % (n - 1);
  if (h % 2 == 0)
    h++;
  return h;
}
#endif

// Punkt p in der Form (x, y) auf dem Ausgabestrom os ausgeben.
// Durch diese Definition kÃ¶nnen Point-Objekte mit dem normalen
// Ausgabeoperator (<<) auf cout ausgegeben werden.
ostream &operator<<(ostream &os, Point p) {
  return os << "(" << p.x << ", " << p.y << ")";
}

// Interaktiver Test einer Streuwerttabelle tab des Typs H.
// Wenn prompt gleich true ist, wird vor jedem Befehl eine
// Eingabeaufforderung ausgegeben.
template <typename H> void test(H tab, bool prompt) {
  // Befehle von der Standardeingabe lesen
  // und entsprechende Operationen auf der Tabelle ausfÃ¼hren.
  string cmd, name;
  int x, y;
  while (true) {
    if (prompt)
      cout << "cmd: ";
    cin >> cmd;
    switch (cmd[0]) {
    case 'p': // put x y name
      cin >> x >> y >> name;
      tab.put(Point(x, y), name);
      break;
    case 'g': // get x y
      cin >> x >> y;
      if (tab.get(Point(x, y), name))
        cout << name << endl;
      break;
    case 'r': // remove x y
      cin >> x >> y;
      tab.remove(Point(x, y));
      break;
    case 'd': // dump
      tab.dump();
      break;
    case 'q': // quit
      return;
    }
  }
}

// Hauptprogramm.
// Das erste Kommandozeilenargument ist die TabellengrÃ¶ÃŸe n,
// das zweite ist entweder c (chaining), l (linear probing),
// q (quadratic probing) oder d (double hashing),
// das dritte entweder leer oder q
// (quiet, d. h. keine Eingabeaufforderung).
int main(int argc, char *argv[]) {
  // Korrekten Aufruf Ã¼berprÃ¼fen.
  if (argc < 3) {
    cout << "Usage: hashtest size (c|l|q|d) [q]" << endl;
    return 1;
  }

  // Die obige Funktionsschablone test mit einer durch die
  // Kommandozeilenargumente definierten Streuwerttabelle aufrufen.
  uint n = stoi(argv[1]);
  bool prompt = argc == 3;
  switch (argv[2][0]) {
  case 'c':
    test(HashChain<Point, string>(n), prompt);
    break;
#ifdef HASHOPEN
  case 'l':
    test(HashOpen<Point, string, LinProb<Point>>(n), prompt);
    break;
  case 'q':
    test(HashOpen<Point, string, QuadProb<Point>>(n), prompt);
    break;
  case 'd':
    test(HashOpen<Point, string, DblHash<Point>>(n), prompt);
    break;
#endif
  }
}
