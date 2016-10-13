#include <iostream>
#include <math.h>

using namespace std;

double ab(double);

//Performs case amplification
double caseAmp(double sim) {
  return sim * pow ( ab ( sim ), 2.5 );
}

template <int A, int B>
//Stores top similarity index in array 'top'
void topSim(double (&similarity)[A], int (&top)[B]) {

  //initializing all values to be 0, of top B ammount
  for ( int i = 0; i < B; i++ )
    top[i] = -2;

  //Sorting the top 75 users
  for ( int i = 0; i < A; i++ ) {

      bool swap = false;
      int temp;

      for ( int j = 0; j < B; j++ ) {

        if ( !swap && ( ab(similarity[i]) > ab(similarity[top[j]]) || top[j] == -2 ) ) {
          temp = top[j];
          top[j] = i;
          swap = true;
        }

        else if ( swap ) {
          int t = top[j];
          top[j] = temp;
          temp = t;
        }

      }
  }

}

//Rounding a double, if .5 or greater, rounds up, otherwise rounds down
int rnd(double r) {

  //Checks if in between 0 and 1, and returning 1 if so
  if (r > 0 && r < 1)
    return 1;

  //Casts double as int; truncates to closest integer down
  int s = r;

  //If the difference is greater than or equal to .5 round up, otherwise, remain.
  if ( (r - s)  >= .5 )
    return s + 1;

  return s;

}

//Returning the absolute value of a number
double ab(double num) {
  if (num < 0)
    return -1 * num;
  return num;
}

//Finds the standard deviation of an item
template <int A, int B>
double Deviation(int (&item)[A][B], double ave, int idx) {
  if ( ave == 0 )
    return -1;

  double E = 0;
  int count = 0;

  for ( int i = 0; i < A; i++ )
    if ( item[i][idx] != 0 ) {
      E += ( item[i][idx] - ave )*( item[i][idx] - ave );
      ++count;
    }

  if (count == 0)
    return -1;

  return sqrt ( E / count );
}
