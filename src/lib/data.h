#include <string>

using namespace std;

// --- Extern Variables Declaration
extern char *cmdsPtr, *subCmdsPtr, *fieldsCmdsPtr, *clientCmdsPtr, *sortByCmdsPtr;
extern const string clear, tab1;
extern const bool applyBgColor, applyFgColor;
extern const int nChar;

#ifndef DATA_H
#define DATA_H

// --- Structs
struct Movie
{
  int id;
  string name;
  string Genre;
  int duration;
  string director;
  int releaseDate;
  string Prented;
  int rentDate;
};

// --- Global Variables

// - Main Commands
enum cmds
{
  cmdAddMovie,
  cmdRentMovie,
  cmdMovieStatus,
  cmdViewMovies,
  cmdFilterMovies,
  cmdSearchClient,
  cmdFieldParameters,
  cmdSortByParameters,
  cmdClientParameters,
  cmdHowToUseViewMovies,
  cmdHowToUseFilterMovies,
  cmdHowToUseSearchClient,
  cmdAddClient,
  cmdHelp,
  cmdExit,
  cmdEnd // To get the number of Commands. SHOULD BE AT THE END
};

// - Subcommands
enum subCmds
{
  subCmdField,
  subCmdSortBy,
  subCmdEnd
};

// - Movie Fields
// Used to Select which Fields to Print in View Data Command
// or Used to Specify where to Filter the Parameter, if it's
// Typed Right After the Field
enum cmdFields
{
  fieldDirector,
  fieldDuration,
  fieldGenre,
  fieldId,
  fieldPrice,
  fieldRelease,
  fieldAll,
  fieldEnd // To get the number of Fields. SHOULD BE AT THE END
};

// - Client Commands
enum clientCmds
{
  clientId,
  clientName,
  clientAccountNumber,
  clientEnd
};

// - Sort By Commands
// A: Ascending
// D: Descending
enum sortByCmds
{
  sortByDurationA,
  sortByDurationD,
  sortByIdA,
  sortByIdD,
  sortByPriceA,
  sortByPriceD,
  sortByReleaseA,
  sortByReleaseD,
  sortByTitleA,
  sortByTitleB,
  sortByEnd // To get the number of Sort By Commands. SHOULD BE AT THE END
};

// --- Functions
int isCharOnArray(char character, char array[], int n);
void addMovie();
void rentMovie();
void movieStatus();
void viewMovies(bool fields[], int m, char sortBy[], int n);
void filterMovies(string **fieldParams, int l, int m, char *sortBy, int n);
void searchClient(string **clientParams, int m, int n);
void fields(); // As a Parameter or as a Subcommand
void sortByParameters();
void clientParameters();
void howToUseViewMovies();
void howToUseFilterMovies();
void howToUseSearchClient();
void addClient();

#endif