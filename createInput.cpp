#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

int main () {
    ofstream file; 
    file.open("input");

    for (int i = 0; i < 100; i++) {
        int num = rand() % 26;
        file << (char) (97+num);
  //      cout << (char)(num + 97) << endl;
    }
    return 0;
}
