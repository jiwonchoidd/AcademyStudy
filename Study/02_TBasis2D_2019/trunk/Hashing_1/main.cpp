#include "HashTable.h"

int main()
{
	LinkedList link;
	cout << LinkedList::g_iCount << " " << link.g_iMaxCount << endl;

    Item * A = new Item {"Apple", NULL};
    Item * B = new Item {"Banana", NULL};
    Item * C = new Item {"Caterpillar", NULL};
    Item * D = new Item {"Dog", NULL};
    Item * E = new Item {"Elephant", NULL};
    Item * F = new Item {"Fedora", NULL};
    Item * G = new Item {"Goosebumps", NULL};
    Item * H = new Item {"House", NULL};
    Item * I = new Item {"Insects", NULL};
    Item * J = new Item {"Jam", NULL};
    Item * K = new Item {"Kite", NULL};
    Item * L = new Item {"Limestone", NULL};
    Item * M = new Item {"Mountaineering", NULL};
    Item * N = new Item {"Night", NULL};
    Item * O = new Item {"Open Sesame", NULL};
    Item * P = new Item {"Potatoes", NULL};
    Item * Q = new Item {"Quantum Mechanics", NULL};
    Item * R = new Item {"Rrrrrrrrrrawr", NULL};
    Item * S = new Item {"Snakes", NULL};
    Item * T = new Item {"Tizzy Tube", NULL};
    Item * U = new Item {"Underworld", NULL};
    Item * V = new Item {"Volcanic Ash", NULL};
    Item * W = new Item {"Who When What Why", NULL};
    Item * X = new Item {"XXX", NULL};
    Item * Y = new Item {"Yellow", NULL};
    Item * Z = new Item {"Zest of Lemon", NULL};
    
    HashTable table;

    table.insertItem(A);
    table.insertItem(B);
    table.insertItem(C);
    table.printTable();
    table.printHistogram();
    
    table.removeItem("Apple");
    table.printTable();
    table.printHistogram();
    
    
    table.insertItem(D);
    table.insertItem(E);
    table.insertItem(F);
    table.insertItem(G);
    table.insertItem(H);
    table.insertItem(I);
    table.insertItem(J);
    table.insertItem(K);
    table.insertItem(L);
    table.insertItem(M);
    table.insertItem(N);
    table.insertItem(O);
    table.insertItem(P);
    table.insertItem(Q);
    table.insertItem(R);
    table.insertItem(S);
    table.insertItem(T);
    table.insertItem(U);
    table.insertItem(V);
    table.insertItem(W);
    table.insertItem(X);
    table.insertItem(Y);
    table.insertItem(Z);
    table.printTable();
    table.printHistogram();
    
    Item * result = table.getItemByKey("Snakes");
    cout << result -> key << endl;    
    return 0;
}

