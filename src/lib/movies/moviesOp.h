#include "../namespaces.h"

using namespace std;
using namespace commands;
using namespace movies;

#ifndef MOVIES_H
#define MOVIES_H

// --- Functions
movieStatus getMovieId(Movies *movies, int *id, int *index, string message);
void addMovieToFile(Movies *movies);
void getMovies(Movies *movies);
movieStatus checkMovie(Movies *movies, int id, int *index);
int getGenreIndex(string genre);
int getGenreIndexLower(string genre);
void filterMoviesData(Movies *movies, string **params, bool fields[], int sortBy[]);
void sortMovies(Movies *movies, int sortBy[], int n);
string getGenresStr(int genres[]);
string getDateStr(int date[3]);
string getCurrentDate(int date[]);

#endif