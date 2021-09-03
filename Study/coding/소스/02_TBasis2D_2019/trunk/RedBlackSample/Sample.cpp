#pragma once
#include "mySet.cpp"
#include "myMap.cpp"
#include <iostream>
#include <string>
using namespace std;

int main( int argc, char* argv[] )
{
    cout << "set test" << endl;
    mySet<int> s;
    s.insert(1);
    cout << (s.find(1))->key << endl;
    s.insert(10);
    cout << (s.end())->key << endl;
    s.insert(23);
    cout << (s.begin())->key << endl;
    s.erase(23);
    cout << (s.end())->key << endl;

    cout << endl << "map test" << endl;
    myMap<int, int> m;
    m.insert(1, 100);
    m[5] = 132;
    m.insert(3, 11);
    m.insert(11, 44);
    cout << "m[5]=" << m[5] << endl;
    cout << "m[3]=" << m[3] << endl;
    m.erase(5);
    m.erase(100);

    cout << endl << "rbTree test" << endl;
    RBTree<int, int> tree;
    string option;
    for (int i=1; i<=10000; i++)
    {
        tree.insert( i );
    }
    for (int i=1; i<=10000; i++)
    {
        tree.erase( i );
    }
    while( true )
    {
        cout << "> ";
        cin >> option;
        if ( option == "insert" )
        {
            int k, val;
            cin >> k >> val;
            tree.insert( k, val );
            cout << "finish insert" << endl;
            tree.isRBT();
        }
        else if ( option == "erase" )
        {
            int val;
            cin >> val;
            tree.erase( val );
            cout << "finish erase" << endl;
            tree.isRBT();
        }
        else if ( option == "draw" )
        {
            tree.draw();
        }
        else if ( option == "exit" )
        {
            break;
        }
        
    }
    return 0;
}