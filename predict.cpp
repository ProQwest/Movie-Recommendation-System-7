#include <iostream>
#include <cmath>
#include "similarity.cpp"
#include "helper.cpp"
#include "avg.cpp"
#include "write.cpp"

using namespace std;

//Self made algorithm which takes into account both item based collaborative filtering, and user
//based collaborative filtering, as well as my own algorithm which makes users similar if they rather
//the same movies high/low/neutral.
template <int A, int B, int C, int D>
void selfAlgorithm(int (&train)[A][B], int (&test)[C][D], int max, const int size) {
  double ro[C];

  //Lazy code -- should implement inside code, rather than separating, and making variables out of scope
  if ( true ) {
    //Setting initial cursor for the test data.
    int idx = 0;

    //Continue iterating through test data, until out of range
    while ( idx < max ) {

      int ratings[20][2];
      int a = 0;
      double tAvg = 0;

      //Iterating through each of the given data for user
      while ( test[idx][2] != 0 ) {

        //Adding movie ID
        ratings[a][0] = test[idx][1];

        //Adding movie ratings
        ratings[a][1] = test[idx][2];
        tAvg += test[idx][2];

        ++a;
        ++idx;
      }
      tAvg /= size;

      double Avg[200];
      //Finding the average rating for every user
      for ( int i = 0; i < 200; i++ ) {
        int ac = 0;
        Avg[i] = 0;

        for ( int j = 0; j < 1000; j++ ) {
          Avg[i] += train[i][j];
          if ( train[i][j] != 0 )
            ++ac;
        }

        if ( ac != 0 )
          Avg[i] /= ac;
        else
          Avg[i] = 0;
      }

      while ( test[idx][2] == 0 ) {

        double similarity[20];
        double sim[20];
        //Calculating the similarity
        for ( int i = 0; i < size; i++ ) {
          similarity[i] = itemAdjCosSimilarity(train, ratings[i][0], test[idx][1], Avg);
          sim[i] = itemSimilarity(train, ratings[i][0], test[idx][1]);
        }

        double r = itemAdjCosRP(similarity, ratings, size, tAvg);
        double r2 = itemAvg(sim, ratings, size);

        double r_new;

        if ( size == 5 ) {
          r_new = .5 * r + .5 * r2;
        } else if ( size == 10 ) {
          r_new = .6 * r + .4 * r2;
        } else if ( size == 20 ) {
          r_new = .75 * r + .25 * r2;
        }

        ro[idx] = r_new;

        ++idx;
      }
    }
  }

  //Setting initial cursor for the test data.
  int idx = 0;

  //Continue iterating through test data, until out of range
  while ( idx < max ) {

    int ratings[20][3];
    int a = 0;
    double tAvg = 0;

    //Iterating through known ratings
    while ( test[idx][2] != 0 ) {

      //Storing item ID in ratings[i][0]
      ratings[a][0] = test[idx][1];

      //Storing test rating in ratings[i][1]
      ratings[a][1] = test[idx][2];

      //Caclulating average for test user
      tAvg += test[idx][2];

      ++a;
      ++idx;
    }
    tAvg /= size;

    double Avg[200];
    //Used for calculating the average across all items
    for ( int i = 0;  i < 200; i++ ) {
      int c = 0;

      for ( int j = 0; j < 1000; j++ ) {
        if ( train[i][j] != 0 ) {
          Avg[i] += train[i][j];
          ++c;
        }
      }
      if ( c != 0 )
        Avg[i] /= c;
      else
        Avg[i] = 0;
    }

    //Geting the cosine similarity
    double coSimilarity[200], pSimilarity[200];
    //Finding the similarity for every user
    for ( int i = 0; i < 200; i++ ) {

      //Storing user i rating in ratings[j][2]
      for ( int j = 0; j < size; j++ )
        ratings[j][2] = train[i][ratings[j][0] - 1];

      coSimilarity[i] = cosSimilarity(ratings, size);
      coSimilarity[i] = caseAmp(coSimilarity[i]);
      pSimilarity[i] = pearsonSimilarity(ratings, size, tAvg, Avg[i]);
    }

    //Finding the similarity, from self-made algorithm
    double similarity[200];
    for ( int i = 0; i < 200; i++ ) {
      similarity[i] = selfSimilarity(train[i], ratings, size);
      similarity[i] = caseAmp(similarity[i]);
    }

    int top[200], cosTop[200], pTop[200];
    topSim(similarity, top);
    topSim(coSimilarity, cosTop);
    topSim(pSimilarity, pTop);

    while ( test[idx][2] == 0 ) {

      double r2 = selfAvg(similarity, top, train, test[idx][1]);
      double r = weightedAvg(coSimilarity, cosTop, train, test[idx][1]);
      double r3 = pearsonAvg(pSimilarity, pTop, train, test[idx][1], tAvg, Avg);
      double r4 = ro[idx];

      //double r_new = ( r * .44 ) + ( r3 * .24 ) + ( r2 * .08 ) + (r4 * .24);
      double pc;
      if ( size == 5 ) {
        pc = r * .75 + r3 *.25;
      } else if ( size == 10 ) {
        pc = r * .65 + r3 * .35;
      } else if ( size == 20 ) {
        pc = r * .55 + r3 * .45;
      }

      double r_new;

      if ( size == 5 ) {
        r_new = ( pc * .8 ) + ( r4 * .2 );// + ( r * .01);
      } else if ( size == 10 ) {
        r_new = ( pc * .65 ) + ( r4 * .35 );// + ( r * .05);
      } else if ( size == 20 ) {
        r_new = ( pc * .7 ) + ( r4 * .3 );// + ( r * .02);
      }

      int rating = rnd ( r_new );

      if (rating > 5)
        rating = 5;
      if (rating < 1)
        rating = 1;


      writeOutput(test[idx][0], test[idx][1], rating, size);

      ++idx;
    }
  }
}

//User based collaborative filtering for cosine similarity
template <int A, int B, int C, int D>
void cosPredict(int (&train)[A][B], int (&test)[C][D], int max, const int size) {
  double IUF[1000];

  //Calculating IUF
  for ( int j = 0; j < 1000; j++ ) {
    int rc = 0;

    for ( int i = 0; i < 200; i++ )
      if ( train[i][j] != 0 )
        ++rc;
    if ( rc == 0 )
      IUF[j] = 1;
    else
      IUF[j] = log ( 1000 / rc );
  }

  //Setting initial cursor for the test data.
  int idx = 0;

  //Continue iterating through test data, until out of range
  while ( idx < max ) {

    int ratings[20][3];
    int i = 0;

    //Iterating through each of the given data for user
    while ( test[idx][2] != 0 ) {

      //Storing item ID in ratings[i][0]
      ratings[i][0] = test[idx][1];

      //Storing test rating in ratings[i][1]
      ratings[i][1] = test[idx][2];

      ++i;
      ++idx;
    }

    double similarity[200];

    //Finding the similarity for every user
    for ( int i = 0; i < 200; i++ ) {

      //Storing user i rating in ratings[j][2]
      for ( int j = 0; j < size; j++ )
        ratings[j][2] = train[i][ratings[j][0] - 1];

      similarity[i] = cosSimilarity(ratings, size);
      similarity[i] = caseAmp(similarity[i]);
    }

    //Retrieve K most similar users
    int top[200];
    topSim(similarity, top);

    //Based off of k most similar users, predict users ratings for specific items;
    while ( test[idx][2] == 0 ) {

      double r = weightedAvg(similarity, top, train, test[idx][1]);
      //r *= IUF[test[idx][1] - 1];
      int rating = rnd(r);

      //Write to output file.
      writeOutput(test[idx][0], test[idx][1], rating, size);

      ++idx;
    }
  }

}

//User based collaborative filtering for pearon's correlation
template <int A, int B, int C, int D>
void pearsonPredict(int (&train)[A][B], int (&test)[C][D], int max, const int size) {
  double IUF[1000];

  //Calculating IUF
  for ( int j = 0; j < 1000; j++ ) {
    int rc = 0;

    for ( int i = 0; i < 200; i++ )
      if ( train[i][j] != 0 )
        ++rc;
    if ( rc == 0 )
      IUF[j] = 1;
    else
      IUF[j] = log ( 1000 / rc );
  }

  //Setting initial cursor for the test data.
  int idx = 0;

  //Continue iterating through test data, until out of range
  while ( idx < max ) {

    int ratings[20][3];
    int a = 0;
    double tAvg = 0;

    //Iterating through each of the given data for user
    while ( test[idx][2] != 0 ) {

      //Storing item ID in ratings[i][0]
      ratings[a][0] = test[idx][1];

      //Storing test rating in ratings[i][1]
      ratings[a][1] = test[idx][2];

      //Caclulating average for test user
      tAvg += test[idx][2];

      ++a;
      ++idx;
    }
    tAvg /= size;

    double similarity[200];
    //Used for calculating user avg, for specific movies
    double Avg[200];
    //Used for calculating the average across all items
    for ( int i = 0;  i < 200; i++ ) {
      int c = 0;

      for ( int j = 0; j < 1000; j++ ) {
        if ( train[i][j] != 0 ) {
          Avg[i] += train[i][j];
          ++c;
        }
      }
      if ( c != 0 )
        Avg[i] /= c;
      else
        Avg[i] = 0;
    }

    //Finding the similarity for every user
    for ( int i = 0; i < 200; i++ ) {

      //Storing user i rating in ratings[j][2]
      for ( int j = 0; j < size; j++ )
        ratings[j][2] = train[i][ratings[j][0] - 1];

      similarity[i] = pearsonSimilarity(ratings, size, tAvg, Avg[i]);
      //similarity[i] = caseAmp(similarity[i]);
    }

    //Retrieve K most similar users
    int top[200];
    topSim(similarity, top);
    for (int i = 0; i < 200; i++)
    //  cout << similarity[top[i]] << "   " << top[i] << "   " << i << endl;

    //Based off of k most similar users, predict users ratings for specific items;
    while ( test[idx][2] == 0 ) {

      double r = pearsonAvg(similarity, top, train, test[idx][1], tAvg, Avg);
      //r *= IUF[test[idx][1] - 1]
      int rating = rnd(r);

      if (rating > 5)
        rating = 5;
      if (rating < 1)
        rating = 1;

      //Write to output file.
      writeOutput(test[idx][0], test[idx][1], rating, size);

      ++idx;
    }
  }

}

//Item based collaborative filtering for cosine similarity
template <int A, int B, int C, int D>
void itemAdjCos(int (&train)[A][B], int (&test)[C][D], int max, const int size) {
  //Setting initial cursor for the test data.
  int idx = 0;

  //Continue iterating through test data, until out of range
  while ( idx < max ) {

    int ratings[20][2];
    int a = 0;
    double tAvg = 0;

    //Iterating through each of the given data for user
    while ( test[idx][2] != 0 ) {

      //Adding movie ID
      ratings[a][0] = test[idx][1];

      //Adding movie ratings
      ratings[a][1] = test[idx][2];
      tAvg += test[idx][2];

      ++a;
      ++idx;
    }
    tAvg /= size;

    double Avg[200];
    //Finding the average rating for every user
    for ( int i = 0; i < 200; i++ ) {
      int ac = 0;
      Avg[i] = 0;

      for ( int j = 0; j < 1000; j++ ) {
        Avg[i] += train[i][j];
        if ( train[i][j] != 0 )
          ++ac;
      }

      if ( ac != 0 )
        Avg[i] /= ac;
      else
        Avg[i] = 0;
    }

    while ( test[idx][2] == 0 ) {

      double similarity[20];
      double sim[20];
      //Calculating the similarity
      for ( int i = 0; i < size; i++ ) {
        similarity[i] = itemAdjCosSimilarity(train, ratings[i][0], test[idx][1], Avg);
        sim[i] = itemSimilarity(train, ratings[i][0], test[idx][1]);
      }

      double r = itemAdjCosRP(similarity, ratings, size, tAvg);
      double r2 = itemAvg(sim, ratings, size);

      double r_new;

      if ( size == 5 ) {
        r_new = .5 * r + .5 * r2;
      } else if ( size == 10 ) {
        r_new = .6 * r + .4 * r2;
      } else if ( size == 20 ) {
        r_new = .75 * r + .25 * r2;
      }

      int rating = rnd ( r_new );

      if ( rating > 5 )
        rating = 5;
      if ( rating < 1 )
        rating = 1;

      writeOutput(test[idx][0], test[idx][1], rating, size);

      ++idx;
    }
  }
}

//Item based collaborative filtering for adjusted cosine similarity
template <int A, int B, int C, int D>
void item(int (&train)[A][B], int (&test)[C][D], int max, const int size) {
  //Setting initial cursor for the test data.
  int idx = 0;

  //Continue iterating through test data, until out of range
  while ( idx < max ) {

    int ratings[20][2];
    int a = 0;
    double tAvg = 0;

    //Iterating through each of the given data for user
    while ( test[idx][2] != 0 ) {

      //Adding movie ID
      ratings[a][0] = test[idx][1];

      //Adding movie ratings
      ratings[a][1] = test[idx][2];
      tAvg += test[idx][2];

      ++a;
      ++idx;
    }
    tAvg /= size;

    while ( test[idx][2] == 0 ) {

      double similarity[20];
      //Calculating the similarity
      for ( int i = 0; i < size; i++ ) {
        similarity[i] = itemSimilarity(train, ratings[i][0], test[idx][1]);
      }

      double r = itemAvg(similarity, ratings, size);
      int rating = abs ( r );

      writeOutput(test[idx][0], test[idx][1], rating, size);

      ++idx;
    }
  }
}
