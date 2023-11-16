#include <string>

using namespace std;

// --- Extern Variables and Constants Declaration
extern string *genrePtr;

// --- Enums

// - Genre
enum genres
{
  genreAction,
  genreAdventure,
  genreAnimation,
  genreChildren,
  genreComedy,
  genreCrime,
  genreDocumentary,
  genreDrama,
  genreFantasy,
  genreFilmNoir,
  genreHorror,
  genreMistery,
  genreMusical,
  genreRomance,
  genreSciFi,
  genreThriller,
  genreWar,
  genreWestern,
  noneGenre,
  errorGenre,
  genreEnd
};

// --- Structs

// - Movie

struct Movie
{
  int id;               // Movie ID
  string name;          // Movie Name
  int genres[genreEnd]; // Movie Genres
  int duration;         // Movie Duration
  string director[2];   // Movie Director First and Last Name
  float price;          // Movie Price
  int releaseDate[3];   // Movie Release Date
  int rentTo;           // Client Id
  int rentOn[3];        // Rent Date
  bool rentStatus;      // Rent Status
};

// - Client
struct Client
{
  int id;            // Client ID
  string name;       // CLient Name
  int accountNumber; // CLient Account Number
};

#ifndef READ_FILE_H
#define READ_FILE_H

int getMovies(Movie movies[], int m, bool fields[], int n);
void sortMovies(Movie *movies, int sortBy[], int n);

#endif