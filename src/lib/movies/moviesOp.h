#include "namespaces.h"

using namespace std;
using namespace commands;
using namespace movies;

#ifndef MOVIES_H
#define MOVIES_H

// --- Functions
void addMovieToFile(Movie movies[], int *nMoviesRead);
int getMovies(Movie movies[]);
movieStatus checkMovieStatusById(Movie movies[], int nMoviesRead, int id, int *index);
void sortMovies(Movie *movies, int m, int sortBy[], int n);
string getGenresStr(int genres[], int nGenres);
string getDateStr(int date[3]);
int getMovieId(string message);

#endif