#include <string>

using namespace std;

// --- Extern Variables Declaration
extern int *cmdsPtr, *subCmdsPtr, *fieldsCmdsPtr, *clientCmdsPtr, *sortByCmdsPtr;
extern string *genrePtr;
extern const string clear, reset, tab1, sgrBgCmd, sgrFgCmd;
extern const bool applyBgColor, applyFgColor;
extern const int nChar, nMaxGenres;

#ifndef DATA_H
#define DATA_H

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
  fieldId,
  fieldName,
  fieldDirector,
  fieldGenre,
  fieldDuration,
  fieldPrice,
  fieldRelease,
  fieldStatus,
  fieldRentOn,
  fieldRentTo,
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

// --- Templates
template <typename T>
string addBrackets(T message);

// --- Functions
int isCharOnArray(int character, int array[], int n);
void addMovie();
void rentMovie();
void movieStatus();
void viewMovies(bool fields[], int m, int sortBy[], int n);
void filterMovies(string **fieldParams, int l, int m, int sortBy[], int n);
void searchClient(string **clientParams, int m, int n);
void fields(); // As a Parameter or as a Subcommand
void sortByParameters();
void clientParameters();
void howToUseViewMovies();
void howToUseFilterMovies();
void howToUseSearchClient();
void addClient();

#endif