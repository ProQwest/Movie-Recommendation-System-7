# Movie Recommendation System

This program makes movies recommendations to different users.

## Training Data
The training data for the predictions is a set of movie ratings by 200 different users and 1000 movies. A rating is a number between 1 through 5. One of the training data files can be seen [here](https://github.com/kvelcich/Movie-Recommendation-System/blob/master/text/test20.txt). The first column is the user id, the second column is the movie number and the third column is the rating of that row's user for that row's movie.

## Test Data
For testing, I was given for 100 different users, 5 movie ratings, and based off of these 5 ratings, I attempted to predict what their ratings would be on other movies they haven't rated.

## Methods
In attempt to predict a users ratings, a used the following methods:
1. User-based collaborative filtering using Pearson's correlation method (with inverse user frequency and case modification)
2. Item-based collaborative filtering using adjusted cosine similarity
3. Medley between user-based and item-based collaborative filtering

## Results
This project was a classwide competition for where I placed first giving the most accurate predictions to the 'actual truth'.
