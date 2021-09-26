#include <iostream>
#include <string>
int main() {

    // https://www.acmicpc.net/problem/1032

    int n;

    std::cin >> n;

    std::string str[50];

    for (int i = 0; i < n; i++) {
        std::cin >> str[i];
    }

    char c;

    for (int i = 0; i < str[0].length(); i++) {
        c = str[0][i];
        for (int j = 0; j < n; j++) {
            if (c != str[j][i]) {
                c = '?';
                break;
            }
        }
        std::cout << c;
    }

}