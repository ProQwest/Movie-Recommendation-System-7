#include <iostream>
#include <fstream>
#include <assert.h>

using namespace std;

template <int A, int B, int C, int D, int E, int F, int G, int H>
//Reads the input files and stores the data into an array
void readData(int (&train)[A][B], int (&test5)[C][D], int (&test10)[E][F], int (&test20)[G][H]) {
  //cout << "Importing train.txt into an array...\n";
  ifstream File;
  File.open("text/train.txt");

  int num, a = 0, b = 0;
  while (File >> num) {
    train[a][b] = num;
    if (++b == 1000) {
      b = 0;
      ++a;
    }
  }

  File.close();
  //cout << "End of train.txt\n";

  //cout << "Importing test5.txt into an array...\n";
  File.open("text/test5.txt");

  a = 0;
  b = 0;
  while (File >> num) {
    test5[a][b] = num;

    assert(a != 8497 && b != 3);

    if (++b == 3) {
      b = 0;
      ++a;
    }
  }

  File.close();
  //cout << "End of test5.txt\n";

  //cout << "Importing test10.txt into an array...\n";
  File.open("text/test10.txt");

  a = 0;
  b = 0;
  while (File >> num) {
    test10[a][b] = num;

    assert(a != 7000 && b != 3);

    if (++b == 3) {
      b = 0;
      ++a;
    }
  }

  File.close();
  //cout << "End of test10.txt\n";

  //cout << "Importing test20.txt into an array...\n";
  File.open("text/test20.txt");

  a = 0;
  b = 0;
  while (File >> num) {
    test20[a][b] = num;

    assert(a != 12367 && b != 3);

    if (++b == 3) {
      b = 0;
      ++a;
    }
  }

  File.close();
  //cout << "End of test20.txt\n";
}
