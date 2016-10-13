#include <iostream>

using namespace std;

template <int A, int B, int C, int D>

//Self created average. Essentially takes the weighted average
double selfAvg (double (&similarity)[A], int (&top)[B], int (&train)[C][D], int ID) {

    double dividend = 0, divisor = 0;

    for ( int i = 0; i < B;  i++ ) {

      if ( train[top[i]][ID - 1] != 0 ) {
        dividend += similarity[top[i]] * train[top[i]][ID - 1];
        divisor += similarity[top[i]];
      }
    }

    //If no similar user contains rating for specific ID, than average all IDs
    if (divisor == 0) {
      divisor = 1;
      dividend = 0;
      int count = 0;

      for ( int i = 0; i < 200; i++ ) {
        if ( train[i][ID - 1] != 0 ) {
          dividend += train[i][ID - 1];
          ++count;
        }
      }

      if ( dividend == 0 )
        dividend = 3;
      else
        dividend /= count;
    }

    return dividend / divisor;
}

//Calculates the weighted average for an item, in order to make a prediction
template <int A, int B, int C, int D>
double weightedAvg (double (&similarity)[A], int (&top)[B], int (&train)[C][D], int ID) {

  double dividend = 0, divisor = 0;

  for ( int i = 0; i < B;  i++ ) {

    if ( train[top[i]][ID - 1] != 0 ) {
      dividend += similarity[top[i]] * train[top[i]][ID - 1];
      divisor += similarity[top[i]];
    }
  }

  //If no similar user contains rating for specific ID, than average all IDs
  if (divisor == 0) {
    divisor = 1;
    dividend = 0;
    int count = 0;

    for ( int i = 0; i < 200; i++ ) {
      if ( train[i][ID - 1] != 0 ) {
        dividend += train[i][ID - 1];
        ++count;
      }
    }

    if ( dividend == 0 )
      dividend = 3;
    else
      dividend /= count;
  }

  return dividend / divisor;
}

//Pearson's correlation prediciton. Makes a prediciton, based on pearson's correlation similarities.
template <int A, int B, int C, int D, int E>
double pearsonAvg (double (&similarity)[A], int (&top)[B], int (&train)[C][D], int ID, double tAvg, double (&Avg)[E]) {

  double dividend = 0, divisor = 0;

  for ( int i = 0; i < B;  i++ ) {
    if ( train[top[i]][ID - 1] != 0 ) {
      //cout << "Similarity:  " << similarity[top[i]] << endl << "Ranking:  " << train[top[i]][ID-1] <<endl << "Average:  " << Avg[top[i]] << endl << endl;
      dividend += similarity[top[i]] * ( train[top[i]][ID - 1] - Avg[top[i]] );
      divisor += abs ( similarity[top[i]] );
    }
  }

  if ( divisor == 0 ) {
    dividend = 0;

    for ( int i = 0; i < 200; i++ ) {
        if ( train[i][ID - 1] != 0 ) {
          dividend += (train[i][ID - 1] - Avg[i]);
          divisor++;
        }
    }
    if (divisor == 0) {
      dividend = 0;
      divisor = 1;
    }
  }

  return tAvg + ( dividend / divisor );
}

//Item based collaborative filtering for adjust cosine similarity, prediction algorithm
template <int A, int B, int C>
double itemAdjCosRP(double (&similarity)[A], int (&ratings)[B][C], int size, double tAvg) {
  double dividend = 0, divisor = 0;

  for ( int i = 0; i < size; i++ ) {
    dividend += similarity[i] * ( ratings[i][1] - tAvg );
    divisor += abs ( similarity[i] );
  }

  if ( divisor == 0 ) {
    dividend = 0;
    for ( int i = 0; i < size; i++ )
      dividend += ratings[i][1];
    return dividend / size;
  }

  return tAvg + dividend / divisor;
}

//Item based collaborative filtering for cosine similarity, prediction algorithm
template <int A, int B, int C>
double itemAvg(double (&similarity)[A], int (&ratings)[B][C], int size) {
  double dividend = 0, divisor = 0;

  for ( int i = 0; i < size; i++ ) {
    dividend += similarity[i] * ratings[i][1];
    divisor += similarity[i];
  }

  if (divisor == 0) {
    dividend = 0;
    for ( int i = 0; i < size; i++ )
      dividend += ratings[i][1];
    return dividend / size;
  }

  return dividend / divisor;
}
