#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

//Writes the results to corresponding file
void writeOutput(int user, int ID, int rating, int size) {
  //Creating file output
  stringstream ss;
  ss << size;
  string file = "output/result" + ss.str() + ".txt";

  //Writing data to output
  ofstream of;
  of.open(file.c_str(), std::ios_base::app);
  of << user << " " << ID << " " << rating << endl;
//  if (!(rating <= 5 && rating >= 1))
//  of << rating <<endl;
  of.close();
}
