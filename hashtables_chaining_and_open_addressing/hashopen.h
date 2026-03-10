// Vorzeichenlose ganze Zahl.
using uint = unsigned int;


template <typename K>
struct LinProb {
    uint size;
    uint step;
    uint index;
    uint prev;
    // Initialisierung der Sequenz mit SchlÃ¼ssel k und TabellengrÃ¶ÃŸe n.
    // Konstruktur
    LinProb (K k, uint n) {
        size = n;
        step = 0;
        index = (hashval(k)>= 0 ? hashval(k) : -hashval(k)) % size;
        prev = index;
    }

    uint next () {
        if(step >= size) return size;
        if(step == 0) {
            prev = index % size;
            step++;
            return prev;
        } else {
            prev = ((prev + 1) % size);
            return prev;
        }
    }
};

// Sondierungssequenz mit SchlÃ¼sseltyp K fÃ¼r quadratische Sondierung,
// analog zu LinProb.
template <typename K>
struct QuadProb {
    uint size;
    uint step;
    uint index;
    uint prev;
    // Initialisierung der Sequenz mit SchlÃ¼ssel k und TabellengrÃ¶ÃŸe n.
    QuadProb (K k, uint n) {
        size = n;
        step = 0;
        index = (hashval(k)>= 0 ? hashval(k) : -hashval(k)) % size;
    }

    uint next () {
        if(step >= size) return size;
        if(step == 0) {
            prev = index % size;
            step++;
            return prev;
        } else {
            prev = (prev + step) % size;
            step++;
            return prev;
        }
    }
};


template <typename K>
struct DblHash {
    uint size;
    uint step;
    uint index1;
    uint index2;
    uint prev;
    // Initialisierung der Sequenz mit SchlÃ¼ssel k und TabellengrÃ¶ÃŸe n.
    DblHash (K k, uint n) {
        size = n;
        step = 0;
        index1 = (hashval(k)>= 0 ? hashval(k) : - hashval(k)) % size;
        index2 = (hashval2(k, size)>= 0 ? hashval2(k, size) : - hashval2(k, size)) % size;
    }

    uint next () {
        if(step >= size) return size;
        if(step == 0) {
            prev = index1 % size;
            step++;
            return prev;
        } else {
            prev = (prev + index2) % size;
            step++;
            return prev;
        }
    }
};


template <typename K, typename V, typename S>
struct HashOpen {
    struct Elem {
        K key;
        V val;
        int kind; // Art des Eintrags leer ider nicht leer oder löschmarkierung
    };
    Elem* tab;
    uint size;

    HashOpen(uint n) {
        tab = new Elem [n] ();
        size = n;
    }

    int search (K k,int& pgd) {
        S s(k, size);
        uint i = s.next();
        int speichern = -1;
        for (uint j = 0; j < size; j++) {
            if(i >= size) break;
            int type = tab[i].kind;
            if(type==0) {
                if(speichern == -1) {
                    return i;
                } else {
                    return speichern;
                }
            } else if (type==1) {
                if (tab[i].key == k) {
                    pgd = 1;
                    return i;
                }
                i = s.next();
                continue;
            } else if (type==2) {
                if (speichern == -1) {
                    speichern = i;
                }
                i = s.next();
                continue;
            }
        }
        if (speichern != -1 ){
            return speichern;
        } else {
            return -1;
        }
    }

    bool put(K k, V v) {
        int p = 0;
        int result = search(k, p);
        if (result != -1) { // found und not found 
            tab[result].key = k;
            tab[result].val = v;
            tab[result].kind = 1;
        } else {
            return false;
        }
        return true;
    }

    bool get(K k, V& out) {
        int g = 0;
        int result = search(k, g);
        if (result != -1 && g == 1) {
            out = tab[result].val;  
            return true;
        }
        return false; // Key nicht gefunden
    }

    bool remove(K k) {
        int r = 0;
        int result = search(k, r);
        if (result != -1 && r == 1) {
            tab[result].kind = 2;
            return true;
        }
        return false;
    }

    void dump() {
        for (uint i = 0; i < size; i++) {
            if(tab[i].kind == 1) {
                cout << i << " " << tab[i].key << " " << tab[i].val << endl;
            } else if (tab[i].kind == 2) {
                cout << i << endl;
            }
        }
    }
};
