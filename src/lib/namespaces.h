#include <string>

using namespace std;

#ifndef NAMESPACES_H
#define NAMESPACES_H

namespace files
{
  const char sep = ';', genreSep = '|', dateSep = '-'; // Separators
}

namespace terminal
{
  // - ANSI Escape Code
  const string reset = "\x1b[0m"; // Reset Terminal Text Format to Default
  const string clear = "\033c";   // Clear Terminal

  // - Format Constants
  const string tab1 = string(3, ' '); // String with 3 whitespaces
  const string tab2 = string(8, ' '); // String with 8 whitespaces
  const bool applyBgColor = true;     // Add Customed Background Color
  const bool applyFgColor = true;     // Add Customed Foreground Color

  // - Size of the Title
  const int nChar = 100; // If the string length is smaller, it'll be filled with whitespaces

  // --- Color Command Created Using the ralvarezdev's Minigrep Version. URL:https://github.com/ralvarezdev/minigrep

  // - Normal State
  const string sgrBgCmd = "\x1b[48;2;24;100;171m"; // Command to Change the Text Background Color
  const string sgrFgCmd = "\x1b[38;2;255;212;59m"; // Command to Change the Text Foreground Color

  // - Warning (On Error)
  const string sgrBgCmdError = "\x1b[48;2;224;49;49m";
  const string sgrFgCmdError = "\x1b[38;2;255;212;59m";

  // - Number of Characters
  const int paramPerLine = 3;                                 // Number of Parameters Printed by Line
  const int maxSpacing = 4;                                   // If the Maximum Number Characters is Reached this is the Spacing between Paratemeters
  const int nCharField = 15;                                  // Number of Characters for Field Title
  const int nCharTitle = 30;                                  // Number of Characters of the Parameter Title Printed
  const int nCharParam = (nChar - nCharTitle) / paramPerLine; // Number of Maximum Characters per Parameter

  // - Dates
  const int nYear = 4; // Number of Characters for Dates
  const int nMonth = 2;
  const int nDay = 2;
  const int nSep = 1;                                 // Separator Number of Characters after Dates
  const int nDate = nYear + nMonth + nDay + 2 + nSep; // Number of Characters for Release Date and Rent On
}

namespace clients
{ // Enums Should be at the Beginning
  // - Invalid Client Data
  enum invalidClient
  {
    invalidClientId,
    clientExists
  };

  // - Client Status
  enum clientStatus
  {
    clientFound,
    clientNotFound
  };

  // - Client Structure
  struct Client
  {
    int id;             // Client ID
    string name;        // Client Name
    string phoneNumber; // Client Phone Number
    string address;     // Client Address
  };

  const int nClients = 1000;                    // Max Number of Clients
  const string clientsFilename = "clients.csv"; // Clients Filename
}

namespace movies
{ // Enums Should be at the Beginning
  // - Invalid Movie Data
  enum invalidMovie
  {
    invalidMovieId,
    invalidMovieDuration,
    invalidMovieGenre,
    invalidMoviePrice,
    invalidMovieDate
  };

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
    genreIMax,
    genreMistery,
    genreMusical,
    genreRomance,
    genreSciFi,
    genreThriller,
    genreWar,
    genreWestern,
    noneGenre,
    errorGenre,
    genreEnd // To get the number of Genres. SHOULD BE AT THE END
  };

  // - Movie Status
  enum movieStatus
  {
    movieNotFound,
    movieRented,
    movieNotRented,
    movieStatusEnd
  };

  const int nMovies = 10000;                  // Max Number of Movies
  const int nMaxGenres = 5;                   // Max Number of Genres Per Movie
  const string moviesFilename = "movies.csv"; // Movies Filename

  // - Movie Structure
  struct Movie
  {
    int id;               // Movie ID
    string name;          // Movie Name
    int genres[genreEnd]; // Movie Genres
    int duration;         // Movie Duration
    string director;      // Movie Director First and Last Name
    float price;          // Movie Price
    int releaseDate[3];   // Movie Release Date
    int rentTo;           // Client Id
    int rentOn[3];        // Rent Date
    bool rentStatus;      // Rent Status
  };
}

namespace commands
{ // Enums Should be at the Beginning
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
    subCmdEnd // To get the number of Subcommands. SHOULD BE AT THE END
  };

  // - Movie Fields
  // Used to Select which Fields to Print in View Movies Command
  // or Used to Specify where to Filter the Parameter from, if it's
  // Typed Right After the Field
  enum cmdMovieFields
  {
    movieFieldId,
    movieFieldName,
    movieFieldDirector,
    movieFieldGenre,
    movieFieldDuration,
    movieFieldPrice,
    movieFieldRelease,
    movieFieldStatus,
    movieFieldRentOn,
    movieFieldRentTo,
    movieFieldAll,
    movieFieldEnd // To get the number of Fields. SHOULD BE AT THE END
  };

  // - Client Fields
  enum cmdClientFields
  {
    clientFieldId,
    clientFieldName,
    clientFieldPhoneNumber,
    clientFieldAddress,
    clientFieldEnd // To get the number of Fields. SHOULD BE AT THE END
  };

  // - Sort By Movie Commands
  // A: Ascending
  // D: Descending
  enum sortByMovieCmds
  {
    movieSortByDurationA,
    movieSortByDurationD,
    movieSortByIdA,
    movieSortByIdD,
    movieSortByPriceA,
    movieSortByPriceD,
    movieSortByReleaseA,
    movieSortByReleaseD,
    movieSortByTitleA,
    movieSortByTitleB,
    movieSortByEnd // To get the number of Sort By Commands. SHOULD BE AT THE END
  };

  // - Sort By Client Commands
  enum sortByClientCmds
  {
    clientSortByIdA,
    clientSortByIdD,
    clientSortByNameA,
    clientSortByNameB,
    clientSortByEnd // To get the number of Sort By Commands. SHOULD BE AT THE END
  };

  // - Command Status if it's Valid or not
  enum cmdStatus
  {
    validCmd,
    noCmd,
    wrongMainCmd,
    wrongSubCmd,
    wrongViewMoviesCmd,
    wrongFilterMoviesCmd,
    wrongSearchClientCmd,
    wrongField,
    wrongFieldParam,
    wrongSortByParam,
    wrongBooleanAnswer
  };

  const int maxParamPerSubCmd = 6; // Max Number of Parameters per Subcommand

  // - View Movies Command Parameters Structure
  struct ViewMoviesCmd
  {
    bool params[movieFieldEnd];     // 1D Array to Save the Fields to Show in View Movies
    int sortBy[movieSortByEnd / 2]; // For a Field, only Allowed Ascending or Descending Order, not Both at the Same Time
  };

  // - Filter Movies Command Parameters Structure
  struct FilterMoviesCmd
  {
    string params[movieFieldEnd][maxParamPerSubCmd]; // 2D String Array of Field Parameters
    string *paramsPtr[movieFieldEnd];                // 1D Pointer Array to to the 2D Array
    int counter[movieFieldEnd];                      // Filter Movies Field Parameters Counter
    int sortBy[movieSortByEnd / 2];                  // For a Field, only Allowed Ascending or Descending Order, not Both at the Same Time
  };

  // - Search Client Command Parameters Structure
  struct SearchClientCmd
  {
    string params[clientFieldEnd][maxParamPerSubCmd]; // 2D String Array of Clients Parameters
    string *paramsPtr[clientFieldEnd];                // 1D Pointer Array to to the 2D Array
    int counter[clientFieldEnd];
    int sortBy[clientSortByEnd / 2]; // For a Field, only Allowed Ascending or Descending Order, not Both at the Same Time
  };

  // - Command Structure
  struct Cmd
  { // Command, SubCommand, Field, Parameter
    int main;
    int sub;
    int field;
    int param;
  };

  // - Command Indexes Structure
  struct CmdIndex
  { // Used to Save the Index of the Command on its Corresponding Array
    int main = 0;
    int sub = 0;
    int field = 0;
    int param = 0;
  };
}

#endif