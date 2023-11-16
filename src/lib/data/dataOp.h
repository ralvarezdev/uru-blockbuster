#include <string>
#include <typeinfo>
#include "..\namespaces.h"

using namespace std;
using namespace movies;

// --- Extern Variables Declaration
extern int *cmdsPtr, *subCmdsPtr,
    *movieFieldCmdsPtr, *clientFieldCmdsPtr,
    *movieSortByCmdsPtr, *clientSortByCmdsPtr;
extern string *genrePtr,
    *movieFieldCmdsStrPtr, *clientFieldCmdsStrPtr, *movieSortByCmdsStrPtr, *clientSortByCmdsStrPtr;

#ifndef DATA_H
#define DATA_H

// --- Templates

// Template to Return a String that's Surrounded by Brackets
template <typename T>
string addBrackets(T message)
{
  string addedBrackets;

  if (typeid(T) == typeid(const int)) // Checks if it's a Char
    addedBrackets += message;         // Works fine it Message is a Char
  else
    addedBrackets = message; // Message is of type String´

  addedBrackets.insert(0, 1, '[');

  return addedBrackets.insert(addedBrackets.length(), 1, ']');
}

// --- Functions
int isCharOnArray(int character, int array[], int n);
void addMovie(Movie movies[], int *nMoviesRead);
void rentMovie(Movie movies[], int nMoviesRead, Client clients[], int *nClientsRead);
void getMovieStatus(Movie movies[], int nMoviesRead);
void viewMovies(Movie movies[], int nMoviesRead, bool fields[], int sortBy[]);
void filterMovies(Movie movies[], int nMoviesRead, string **fieldParams, int sortBy[]);
void searchClient(string **clientParams);
void fields(); // As a Parameter or as a Subcommand
void sortByParameters();
void clientParameters();
void howToUseViewMovies();
void howToUseFilterMovies();
void howToUseSearchClient();
void addClient(Client clients[], int *nClientsRead);

#endif