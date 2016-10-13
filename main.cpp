#include <iostream>
#include "data.cpp"
#include "predict.cpp"

using namespace std;

int main() {
  //cout << "Starting program..." << endl;

  int trainData[200][1000];
  int test5Data[8497][3];
  int test10Data[7000][3];
  int test20Data[12367][3];

  //Filling in Arrays with data from .txt files
  readData(trainData, test5Data, test10Data, test20Data);

  //Deleting previous results
  remove ( "output/result5.txt" );
  remove ( "output/result10.txt" );
  remove ( "output/result20.txt" );

  selfAlgorithm(trainData, test5Data, 8497, 5);
  selfAlgorithm(trainData, test10Data, 7000, 10);
  selfAlgorithm(trainData, test20Data, 12367, 20);

  return 0;
}
