#include <iostream>
#include <math.h>

using namespace std;

double a(double);

template <int A, int B, int C>
double selfSimilarity(int (&train)[A], int (&ratings)[B][C], int size) {
  int hi = 0, hiT = 0, n = 0, nT = 0, lo = 0, loT = 0;

  for ( int i = 0; i < size; i++ ) {
    int data = train[ratings[i][1]];
    int test = ratings[i][1];

    //Procedures of data, if rating exists
    if ( data != 0 ) {
      //Both rated high
      if ( (data == 4 || data == 5) && (test == 4 || test == 5) )
        ++hi;
      //One rated high
      if ( (data == 4 || data == 5) || (test == 4 || test == 5) )
        ++hiT;
      if ( (data == 3) && (test == 3) )
          ++n;
        //One rated high
      if ( (data == 3) || (test == 3) )
          ++nT;
      if ( (data == 1 || data == 2) && (test == 1 || test == 2) )
        ++lo;
      //One rated high
      if ( (data == 1 || data == 2) || (test == 1 || test == 2) )
        ++loT;
    }
  }

    double l, m, h;

    if ( loT == 0 )
      l = 0;
    else
      l = lo / loT;

    if ( nT == 0 )
      m = 0;
    else
      m = n / nT;

    if ( hiT == 0 )
      h = 0;
    else
      h = hi / hiT;

    return 0.4 * h + 0.4 * l + 0.2 * m;
}

template <int A, int B>
double cosSimilarity(int (&ratings)[A][B], int size) {

  int r[size];

  //Ensuring only movies that have been rated is being calculated
  for ( int i = 0; i < size; i++ ) {
    if ( ratings[i][2] == 0 )
      r[i] = 0;
    else
      r[i] = ratings[i][1];
  }

  //Calculating the cosine similarity
  double dividend = 0, divisor1 = 0, divisor2 = 0;

  for ( int i = 0; i < size; i++ ) {
    dividend += r[i] * ratings[i][2];
    divisor1 += r[i] * r[i];
    divisor2 += ratings[i][2] * ratings[i][2];
  }

  double divisor = sqrt ( divisor1 ) * sqrt ( divisor2 );

  if (divisor == 0)
    return 0;

  return dividend / divisor;
}

template <int A, int B>
double pearsonSimilarity(int (&ratings)[A][B], int size, double tAvg, double Avg) {

  //Calculating the cosine similarity
  double dividend = 0, divisor1 = 0, divisor2 = 0;

  for ( int i = 0; i < size; i++ ) {
    if ( ratings[i][2] != 0 ) {
      dividend += ( ratings[i][1] - tAvg ) * ( ratings[i][2] - Avg );
      divisor1 += ( ratings[i][1] - tAvg ) * ( ratings[i][1] - tAvg );
      divisor2 += ( ratings[i][2] - Avg ) * ( ratings[i][2] - Avg );
    }
  }

  double divisor = sqrt ( divisor1 ) * sqrt ( divisor2 );

  if (divisor == 0)
    return 0;

  return dividend / divisor;
}

template <int A, int B, int C>
double itemAdjCosSimilarity(int (&train)[A][B], int i1, int i2, double (&Avg)[C]) {
  double dividend = 0, divisor1 = 0, divisor2 = 0;

  for ( int i = 0; i < A; i++ ) {
    if ( train[i][i1 - 1] != 0 && train[i][i2 - 1] != 0 ) {
      dividend += ( train[i][i1 - 1] - Avg[i] ) * ( train[i][i2 - 1] - Avg[i] );
      divisor1 += ( train[i][i1 - 1] - Avg[i] ) * ( train[i][i1 - 1] - Avg[i] );
      divisor2 += ( train[i][i2 - 1] - Avg[i] ) * ( train[i][i2 - 1] - Avg[i] );
    }
  }

  double divisor = sqrt ( divisor1 ) * sqrt ( divisor2 );

  if ( divisor == 0 )
    return 0;

  return dividend / divisor;
}

template <int A, int B>
double itemSimilarity(int (&train)[A][B], int i1, int i2) {
  double dividend = 0, divisor1 = 0, divisor2 = 0;

  for ( int i = 0; i < A; i++ ) {
    if ( train[i][i1 - 1] != 0 && train[i][i2 - 1] != 0 ) {
      dividend += train[i][i1 - 1] * train[i][i2 - 1];
      divisor1 += train[i][i1 - 1] * train[i][i1 - 1];
      divisor2 += train[i][i2 - 1] * train[i][i2 - 1];
    }
  }

  double divisor = sqrt ( divisor1 ) * sqrt ( divisor2 );

  if ( divisor == 0 )
    return 0;

  return dividend / divisor;
}
