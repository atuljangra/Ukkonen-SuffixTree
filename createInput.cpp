#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

int main () {
    ofstream file; 
    file.open("input");
    srand (time(NULL));
    for (int i = 0; i < 1000000000; i++) {
        int num = rand() % 26;
        file << (char) (97+num);
        // cout << (char)(num + 97) ;
    }
    file<<"$";
    file.close();
    return 0;
}
