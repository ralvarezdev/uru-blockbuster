#include <string>

using namespace std;

#ifndef NAMESPACES_H
#define NAMESPACES_H

// --- Function Prototypes
bool getOlderDate(int date1[3], int date2[3]);

namespace files
{
  const char sep = ';', genreSep = '|', dateSep = '-'; // Separators
  const int newLine = '\n';
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
    invalidClientAccountNumber,
    invalidClientId,
    invalidClientPhoneNumber,
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
    int account;            // Client Account Number
    int id;                 // Client ID
    double phoneNumber;     // Client Phone Number
    char name[nFieldChars]; // Client Name
  };

  // Clients Dynamic Array Class
  class Clients
  {
  private:
    Client *array = NULL; // Initializes as Null Pointer
    int capacity;
    int occupied; // Number of Elements that have been Added to the Array

  public:
    Clients()
    { // Default Constructor
      this->capacity = 2;
      this->occupied = 0;
      this->array = new Client[capacity];
    }

    Clients(int inputCapacity)
    { // Constructor with Capacity Given by the User
      this->capacity = inputCapacity;
      this->occupied = 0;
      this->array = new Client[capacity];
    }

    int getCapacity() { return this->capacity; } // Return Array Capacity

    int getNumberClients() { return this->occupied; } // Return Number of Clients Appended to the Array

    Client getClient(int index) { return this->array[index]; } // Return Client Structure at the Given Index

    void pushBack(Client newClient)
    {
      if (this->occupied == this->capacity)
        increaseArrayCapacity(); // Double the Capacity of the Array

      this->array[this->occupied] = newClient;
      this->occupied++;
    }

    void increaseArrayCapacity() // Function to Double the Array Capacity
    {
      Client *temp = new Client[2 * this->capacity];
      for (int i = 0; i < this->occupied; i++)
        temp[i] = this->array[i]; // Copy Pointer to Client Structures

      delete[] this->array; // Delete Old Array

      this->array = temp; // Assign New Array
      this->capacity *= 2;
    }

    void insertAt(int index, Client client)
    { // Function to Insert Client. If the Index hasn't been Occupied. The Client will be Pushed Back
      if (index >= this->occupied)
        pushBack(client);
      else
        this->array[index] = client; // Insert Client at Given Index
    }

    void deleteAt(int index) // Function to Delete Client at Given Index
    {
      for (int i = index; i < this->occupied; i++)
        this->array[i] = this->array[i + 1]; // Move Clients

      this->occupied--; // Reduce Occupied Variable
    }

    void reverse() // Function to Swap the Order of the Array
    {
      int j = 0, n = this->getNumberClients();
      Client temp;

      for (int i = n - 1; i > (n - 1) / 2; i-- && j++)
      {
        temp = this->array[j];

        this->array[j] = this->array[i];
        this->array[i] = temp;
      }
    }

    Client compare(int *i, int *j, cmdClientFields field, int increaseIndexBy = 0) // Function to Compare Client Fields
    {
      bool isI = false;

      switch (field)
      {
      case commands::clientFieldAccountNumber:
        isI = (this->array[*i].account < this->array[*j].account);
        break;
      case commands::clientFieldId:
        isI = (this->array[*i].id < this->array[*j].id);
        break;
      case commands::clientFieldName:
        string clientName1 = this->array[*i].name;
        string clientName2 = this->array[*j].name;

        for (int n = 0; n < nFieldChars; n++)
          if (n + 1 != nFieldChars && clientName1[n] == clientName2[n])
            continue;
          else
            isI = clientName1[n] <= clientName2[n]; // To Make the Algorithm Stable
        break;
      case commands::clientFieldPhoneNumber:
        isI = (this->array[*i].phoneNumber < this->array[*j].phoneNumber);
        break;
      }

      if (increaseIndexBy != 0)
        if (isI)
          *i += 1;
        else
          *j += 1;

      return (isI) ? this->array[*i - 1] : this->array[*j - 1];
    }

    void deallocate() // Function to Deallocate Memory
    {
      delete[] this->array;
    }
  };

  const string clientsFilename = "clients.bin"; // Clients Filename
  const int precision = 2;                      // Precision for Floats and Doubles (Used on Client Phone Number)
  const int nFieldChars = 50;                   // Number of Characters for Each Field
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
    invalidMovieDate,
    movieExists
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
    genreMusical,
    genreMystery,
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
  };

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

  // Movies Dynamic Array Class
  class Movies
  {
  private:
    Movie *array = NULL; // Initializes as Null Pointer
    int capacity;
    int occupied; // Number of Elements that have been Added to the Array

  public:
    Movies()
    { // Default Constructor
      this->capacity = 2;
      this->occupied = 0;
      this->array = new Movie[capacity];
    }

    Movies(int inputCapacity)
    { // Constructor with Capacity Given by the User
      this->capacity = inputCapacity;
      this->occupied = 0;
      this->array = new Movie[capacity];
    }

    int getCapacity() { return this->capacity; } // Return Array Capacity

    int getNumberMovies() { return this->occupied; } // Return Number of Movies Appended to the Array

    Movie getMovie(int index) { return this->array[index]; } // Return Movie Structure at the Given Index

    void pushBack(Movie newMovie)
    {
      if (this->occupied == this->capacity)
        increaseArrayCapacity(); // Double the Capacity of the Array

      this->array[this->occupied] = newMovie;
      this->occupied++;
    }

    void increaseArrayCapacity() // Function to Double the Array Capacity
    {
      Movie *temp = new Movie[2 * this->capacity];
      for (int i = 0; i < this->occupied; i++)
        temp[i] = this->array[i]; // Copy Pointer to Movie Structures

      delete[] this->array; // Delete Old Array

      this->array = temp; // Assign New Array
      this->capacity *= 2;
    }

    void insertAt(int index, Movie movie)
    { // Function to Insert Movie. If the Index hasn't been Occupied. The Movie will be Pushed Back
      if (index >= this->occupied)
        this->pushBack(movie);
      else
        this->array[index] = movie; // Insert Movie at Given Index
    }

    void deleteAt(int index) // Function to Delete Movie at Given Index
    {
      for (int i = index; i < this->occupied; i++)
        this->array[i] = this->array[i + 1]; // Move Movies

      this->occupied--; // Reduce Occupied Variable
    }

    void reverse() // Function to Swap the Order of the Array
    {
      int j = 0, n = this->getNumberMovies();
      Movie temp;

      for (int i = n - 1; i > (n - 1) / 2; i-- && j++)
      {
        temp = this->array[j];

        this->array[j] = this->array[i];
        this->array[i] = temp;
      }
    }

    Movie compare(int *i, int *j, cmdMovieFields field, int increaseIndexBy = 0) // Function to Compare Movie Fields
    {
      bool isI = false;

      switch (field)
      {
      case commands::movieFieldId:
        isI = (this->array[*i].id < this->array[*j].id);
        break;
      case commands::movieFieldName:
        isI = (this->array[*i].name.compare(this->array[*j].name) < 0);
        break;
      case commands::movieFieldDirector:
        isI = (this->array[*i].director.compare(this->array[*j].director) < 0);
        break;
      // case commands::movieFieldGenre: // NOT DEFINED
      case commands::movieFieldDuration:
        isI = (this->array[*i].duration < this->array[*j].duration);
        break;
      case commands::movieFieldPrice:
        isI = (this->array[*i].price < this->array[*j].price);
        break;
      case commands::movieFieldRelease:
        isI = getOlderDate(this->array[*i].releaseDate, this->array[*j].releaseDate);
        break;
      case commands::movieFieldStatus:
        isI = (this->array[*i].rentStatus < this->array[*j].rentStatus);
        break;
      case commands::movieFieldRentOn:
        isI = getOlderDate(this->array[*i].rentOn, this->array[*j].rentOn);
        break;
      case commands::movieFieldRentTo:
        isI = (this->array[*i].rentTo < this->array[*j].rentTo);
        break;
      }

      if (increaseIndexBy != 0)
        if (isI)
          *i += 1;
        else
          *j += 1;

      return (isI) ? this->array[*i - 1] : this->array[*j - 1];
    }

    void deallocate() // Function to Deallocate Memory
    {
      delete[] this->array;
    }
  };

  const int nMaxGenres = 5;                   // Max Number of Genres Per Movie
  const string moviesFilename = "movies.csv"; // Movies Filename
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
    cmdViewClients,
    cmdSearchClients,
    cmdMovieParameters,
    cmdSortByParameters,
    cmdClientParameters,
    cmdGenres,
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
    clientFieldAccountNumber,
    clientFieldId,
    clientFieldName,
    clientFieldPhoneNumber,
    clientFieldAll,
    clientFieldEnd // To get the number of Fields. SHOULD BE AT THE END
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
    wrongViewClientsCmd,
    wrongSearchClientsCmd,
    wrongMovieField,
    wrongMovieFieldParam,
    wrongClientFieldParam,
    wrongSortByParam,
    wrongBooleanAnswer
  };

  const int maxParamPerSubCmd = 6; // Max Number of Parameters per Subcommand

  // - View Movies Command Parameters Structure
  struct ViewMoviesCmd
  {
    bool params[movieFieldEnd];    // 1D Array to Save the Fields to Show in View Movies
    int sortBy[movieFieldEnd - 1]; // For a Field, only Allowed Ascending or Descending Order, not Both at the Same Time
  };

  // - Filter Movies Command Parameters Structure
  struct FilterMoviesCmd
  {
    string params[movieFieldEnd][maxParamPerSubCmd]; // 2D String Array of Field Parameters
    string *paramsPtr[movieFieldEnd];                // 1D Pointer Array to to the 2D Array
    int counter[movieFieldEnd];                      // Filter Movies Field Parameters Counter
    int sortBy[movieFieldEnd - 1];                   // For a Field, only Allowed Ascending or Descending Order, not Both at the Same Time
  };

  // - View Clients Command Parameters Structure
  struct ViewClientsCmd
  {
    bool params[clientFieldEnd];    // 1D Array to Save the Fields to Show in View Clients
    int sortBy[clientFieldEnd - 1]; // For a Field, only Allowed Ascending or Descending Order, not Both at the Same Time
  };

  // - Search Clients Command Parameters Structure
  struct SearchClientsCmd
  {
    string params[clientFieldEnd][maxParamPerSubCmd]; // 2D String Array of Clients Parameters
    string *paramsPtr[clientFieldEnd];                // 1D Pointer Array to to the 2D Array
    int counter[clientFieldEnd];
    int sortBy[clientFieldEnd - 1]; // For a Field, only Allowed Ascending or Descending Order, not Both at the Same Time
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

  // - Command Explanation
  struct cmdExplanation
  {
    string cmd;
    string explanation;
  };
}

#endif