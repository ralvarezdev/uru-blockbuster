/*
#ifdef _WIN32 // For Windows
#include <windows.h>
#endif
*/

#include <iostream>
#include <filesystem>
#include "lib\namespaces.h"
#include "lib\clients\clientsOp.h"
#include "lib\movies\moviesOp.h"
#include "lib\data\dataOp.h"
#include "lib\terminal\ansiEsc.h"
#include "lib\terminal\input.h"

using namespace std;
using namespace clients;
using namespace commands;
using namespace movies;
using namespace terminal;

/*
--- NOTES
- For this proyect we're not allowed to use maps, vectors, classes, linked lists, tuples.
That's the reason why the program is written like this
*/

// --- Extern Variables Declaration (ASSIGNMENT AT THE END OF THIS FILE)
extern int *cmdsPtr, *subCmdsPtr, *movieFieldCmdsPtr, *clientFieldCmdsPtr, *movieSortByCmdsPtr, *clientSortByCmdsPtr;
extern string *movieFieldCmdsStrPtr, *clientFieldCmdsStrPtr, *movieSortByCmdsStrPtr, *clientSortByCmdsStrPtr, *genrePtr;

// --- Global Variables
int nMoviesRead = 0;  // Number of Movies that had been Read and Copied from movies.csv
int nClientsRead = 0; // Number of Clients that had been Read and Copied from clients.csv

// --- Function Prototypes
void helpMessage();
void initPtrArray(string **ptrArray, string array[][maxParamPerSubCmd], int arrayCounter[], int n);
void changeCwdToData(string path);
// void winSetChcpUtf8();

// Main Function
int main(int argc, char **argv)
{
  // winSetChcpUtf8();                 // Change Codepage While Running the Program to UTF-8  if it's Running in Windows
  std::ios::sync_with_stdio(false); // Desynchronize C++ Streams from C I/O Operations to Increase Performance

  ViewMoviesCmd viewMoviesCmd; // Used to Save the Parameters Typed by the User for the Given Command
  FilterMoviesCmd filterMoviesCmd;
  SearchClientCmd searchClientCmd;
  Cmd cmd;
  CmdIndex index;

  bool exit = false;                      // Tells the Program if the User wants to Exit the Program
  bool moreInput, isField;                // Boolean to Check if there's More Input
  int timesExec = 0;                      // Number of times the Main While Loop has been executed
  cmdStatus isCmd = validCmd;             // Used for Checking if the Command is Valid or Not. If not, it Stores the Reason
  string inputLine, inputWord, inputLong; // Saves the Input of the User, before being Processed by the Program

  changeCwdToData(argv[0]); // Change Current Working Path to 'src/data'

  Movie *movies = new Movie[nMovies]; // Allocate Memory
  nMoviesRead = getMovies(movies);    // Get Movies

  Client *clients = new Client[nClients]; // Allocate Memory
  nClientsRead = getClients(clients);     // Get Clients

  while (!exit) // Main While Loop of the Program
  {
    cmd = Cmd();        // Initialize Command Structure to 0
    index = CmdIndex(); // Initialize Command Indexes to 0

    if (isCmd != validCmd)
    { // If in the Last Execution the User Typed a Wrong Command
      wrongCommand(isCmd);

      switch (isCmd)
      {
      case wrongField:
      case wrongFieldParam:
        fields(); // Print the Valid Field as Commands or as Parameters
        break;
      case wrongSortByParam:
        sortByParameters(); // Print the Valid Sort By Parameters
        break;
      case wrongViewMoviesCmd:
        howToUseViewMovies(); // Print the Usage Examples of the View Movies Command
        break;
      case wrongFilterMoviesCmd:
        howToUseFilterMovies(); // Print Usage Examples of the Filter Movies Commands
        break;
      case wrongSearchClientCmd:
        howToUseSearchClient(); // Print Usage Examples of the Search Client Commands
        break;
      }

      isCmd = validCmd;
    }

    if (timesExec == 0 && argc > 1)
    { // Checks if it's a Command
      for (int i = 1; i < argc; i++)
      {
        inputWord = argv[i];
        size_t found = inputWord.find(' '); // Check if the string contains Whitespaces

        if (found != string::npos)                                        // Whitespace Found
          inputWord.insert(0, 1, '"').insert(inputWord.length(), 1, '"'); // Insert a Double Quote at the Beginning and the End of the Input

        if (i != argc - 1)
          inputWord.insert(inputWord.length(), 1, ' '); // Insert Whitespace between Parameters

        inputLine.append(inputWord); // Append the Parameter
      }
    }
    else
    {
      helpMessage();
      cout << '\n';
      printTitle("ENTER A COMMAND", applyBgColor, applyFgColor, false);

      getline(cin, inputLine); // Get User Input

      string temp;                    // Temporary String
      stringstream stream(inputLine); // Stream the Input to Remove Empty Input (that only has Whitespaces)

      while (getline(stream, inputWord, ' '))
      {
        if (temp.length() != 0 && inputWord.length() > 0)
          inputWord.insert(0, 1, ' '); // Insert a Whitespace at the Beginning of the Input

        temp.append(inputWord); // Append Input
      }
      inputLine = temp;
    }

    stringstream stream(inputLine); // Stream the Input
    timesExec++;                    // Increase the Counter of Commands Executed

    if (!getline(stream, inputWord, ' '))
    { // Couldn't Get the Command
      isCmd = noCmd;
      continue;
    }

    cmd.main = inputWord[0];
    index.main = isCharOnArray(cmd.main, cmdsPtr, cmdEnd); // Check if the Command is in the Array of Main Commands. Returns -1 if it doesn't exist

    if (index.main == -1) // If it's not a Valid Command
      isCmd = wrongMainCmd;
    else if (index.main == cmdViewMovies || index.main == cmdFilterMovies || index.main == cmdSearchClient)
    {                                                           // Checks if the Filter, View Movies or Search Clients Command is Typed Correctly
      bool isViewMoviesCmd = (index.main == cmdViewMovies);     // Boolean to Check if the Current Command is View Movies
      bool isSearchClientCmd = (index.main == cmdSearchClient); // To Shorten the If-Else Statements

      int nSortBy = (isSearchClientCmd) ? movieSortByEnd : clientSortByEnd / 2; // Number of Possible Sort By Commands that can be Applied at the Same Time

      int sortByOrder[nSortBy]; // Save Sorting Order
      int sortByCounter = 0;

      for (int i = 0; i < nSortBy / 2; i++)
        sortByOrder[i] = -1;

      if (isViewMoviesCmd)
      { // Initialize viewMovies Sruct
        viewMoviesCmd = ViewMoviesCmd();
        fill(viewMoviesCmd.sortBy, viewMoviesCmd.sortBy + movieSortByEnd, -1); // Initialize Sort By Array
      }
      else if (!isSearchClientCmd)
      { // Initialize filterMovies Struct
        filterMoviesCmd = FilterMoviesCmd();
        fill(filterMoviesCmd.sortBy, filterMoviesCmd.sortBy + movieSortByEnd, -1); // Initialize Sort By Array
        initPtrArray(filterMoviesCmd.paramsPtr, filterMoviesCmd.params, filterMoviesCmd.counter, movieFieldEnd);
      }
      else
      { // Initialize searchClient Struct
        searchClientCmd = SearchClientCmd();
        initPtrArray(searchClientCmd.paramsPtr, searchClientCmd.params, searchClientCmd.counter, clientFieldEnd);
      }

      while (true)
      {
        if (isCmd != validCmd)
          break; // Exit this While-loop
        isCmd = validCmd;

        while (!moreInput && getline(stream, inputWord, ' '))
          if (inputWord[0] == '-')
            moreInput = true; // First Parameter Must be a Command

        if (!moreInput)
          break; // Exit this While-loop
        moreInput = false;
        isField = true;

        if (inputWord.length() < (isSearchClientCmd ? 3 : 2))
        { // Check if the Command has the Minimum String Length
          isCmd = (isSearchClientCmd) ? wrongSearchClientCmd : wrongSubCmd;
          break;
        }

        if (isSearchClientCmd)
        { // Check if the Client Field is in the Array of Client Commands
          cmd.field = inputWord[2];
          index.field = isCharOnArray(cmd.field, clientFieldCmdsPtr, clientFieldEnd);
        }
        else
        { // Check if the Command is in the Array of subCommands
          cmd.sub = inputWord[1];
          index.sub = isCharOnArray(cmd.sub, subCmdsPtr, subCmdEnd);
        }

        if (index.sub == -1 || index.field == -1)
        { // Wrong  Command
          isCmd = (isSearchClientCmd) ? wrongSearchClientCmd : wrongSubCmd;
          break;
        }

        if (index.sub == subCmdSortBy)
        { // Get Sort By Parameters
          while (getline(stream, inputWord, ' '))
          {
            if (inputWord[0] == '-')
            { // It's not a Sort By Parameter
              moreInput = true;
              break;
            }

            cmd.param = inputWord[0];
            if (isSearchClientCmd)
              index.param = isCharOnArray(cmd.param, clientSortByCmdsPtr, clientSortByEnd); // Check if the Command is in the Sort By Array
            else
              index.param = isCharOnArray(cmd.param, movieSortByCmdsPtr, movieSortByEnd);

            if (index.param == -1)
            { // Wrong Sort By Command Parameter
              isCmd = wrongSortByParam;
              break;
            }

            if (isViewMoviesCmd)
            { // It will Overwrite the Previous Sorting for this Parameter, if it was Specified
              if (viewMoviesCmd.sortBy[index.param / 2] != -1)
                sortByOrder[sortByCounter++] = index.param;

              viewMoviesCmd.sortBy[index.param / 2] = index.param;
            }
            else
            {
              if (filterMoviesCmd.sortBy[index.param / 2] != -1)
                sortByOrder[sortByCounter++] = index.param;

              filterMoviesCmd.sortBy[index.param / 2] = index.param;
            }
          }
          continue;
        }

        if (!isSearchClientCmd)
          getline(stream, inputWord, ' ');

        while (isField) // Get Field Parameters
        {
          isField = false;

          if (!isSearchClientCmd)
          {
            if (inputWord[0] == '-' && (isViewMoviesCmd || (!isViewMoviesCmd && inputWord.length() < 3)))
            { // It's not a Field Command or a Parameter
              moreInput = true;
              break;
            }

            cmd.field = (isViewMoviesCmd) ? inputWord[0] : inputWord[2];
            index.field = isCharOnArray(cmd.field, movieFieldCmdsPtr, movieFieldEnd); // Check if the Command is in the Field Parameters Array

            if (index.field == -1 || (!isViewMoviesCmd && index.field == movieFieldAll))
            { // Wrong Field Parameter or Field Command
              isCmd = (isViewMoviesCmd) ? wrongFieldParam : wrongField;
              break;
            }

            if (isViewMoviesCmd)
            {
              viewMoviesCmd.params[index.field] = true;

              if (getline(stream, inputWord, ' '))                 // Get the Next Argument
                if (inputWord[0] == '-' && inputWord.length() < 3) // It's a Subcommand
                  moreInput = true;
                else if (inputWord[0] != '-' && inputWord.length() > 0)
                  isField = true;

              continue;
            }
          }

          int *paramCounter; // Counter

          if (isSearchClientCmd)
            paramCounter = &searchClientCmd.counter[index.field];
          else
            paramCounter = &filterMoviesCmd.counter[index.field];

          while (*paramCounter < maxParamPerSubCmd && getline(stream, inputWord, ' '))
          { // Iterate while there's a Parameter and can be Added to the Array
            if (inputWord[0] == '"')
            { // If it Starts with Double Quote, it's a Long Sentence (a Parameter with Multiple Words)
              if (!getline(stream, inputLong, '"'))
              { // Incomplete Long Parameter
                isCmd = (isSearchClientCmd) ? wrongSearchClientCmd : wrongFilterMoviesCmd;
                break;
              }

              inputWord.insert(inputWord.length(), 1, ' ');                 // Insert Whitespace at the End
              inputLong.insert(0, inputWord.substr(1, inputWord.length())); // Insert the Parameter at the Beginning of the String, without the Double Quote
              inputWord = inputLong;
            }
            else if (inputWord[0] == '-')
            { // If it's a Command break this For-loop
              if (isSearchClientCmd)
                moreInput = true;
              else if (inputWord.length() < 3)
                moreInput = true;
              else if (inputWord.length() > 2)
                isField = true;
              break;
            }
            else if (inputWord.length() == 0) // To Prevent Adding Whitespaces as Parameters-
              continue;

            if (isSearchClientCmd) // Add Parameter to Search Client
              searchClientCmd.params[index.field][*paramCounter] = inputWord;
            else // Add Parameter to Filter Movies
              filterMoviesCmd.params[index.field][*paramCounter] = inputWord;

            *paramCounter += 1; // Parameter Counter
          }

          while (!isField && !moreInput) // Reached Max Number of Parameters for Command
            if (!getline(stream, inputWord, ' '))
              break;
            else if (inputWord[0] == '-') // Got a Command
              if (inputWord.length() < 3 && !isSearchClientCmd)
                moreInput = true;
              else if (inputWord.length() > 2)
                if (isSearchClientCmd)
                  moreInput = true;
                else
                  isField = true;
        }
      }

      for (int i = 0; i < nSortBy; i++) // Save the Sort By Array based on the Order they were Introduced
        if (sortByOrder[i] != -1)
          if (isViewMoviesCmd)
            viewMoviesCmd.sortBy[i] = sortByOrder[i];
          else if (!isSearchClientCmd)
            filterMoviesCmd.sortBy[i] = sortByOrder[i];

      if (isCmd == validCmd)
        switch (index.main)
        {
        case cmdViewMovies:
          viewMovies(movies, nMoviesRead, viewMoviesCmd.params, viewMoviesCmd.sortBy);
          break;
        case cmdFilterMovies:
          filterMovies(movies, nMoviesRead, filterMoviesCmd.paramsPtr, filterMoviesCmd.sortBy);
          break;
        case cmdSearchClient:
          searchClient(searchClientCmd.paramsPtr);
          break;
        }
    }

    if (isCmd != validCmd)
    { // Chekc if the Command is Valid
      if (timesExec > 1 || (timesExec == 0 && argc == 1))
        cout << '\n'; // Print a New Line
      continue;
    }

    switch (index.main)
    {
    case cmdAddMovie:
      addMovie(movies, &nMoviesRead);
      break;
    case cmdRentMovie:
      rentMovie(movies, nMoviesRead, clients, &nClientsRead);
      break;
    case cmdMovieStatus:
      getMovieStatus(movies, nMoviesRead);
      break;
    case cmdFieldParameters:
      fields();
      break;
    case cmdSortByParameters:
      sortByParameters();
      break;
    case cmdClientParameters:
      clientParameters();
      break;
    case cmdHowToUseViewMovies:
      howToUseViewMovies();
      break;
    case cmdHowToUseFilterMovies:
      howToUseFilterMovies();
      break;
    case cmdHowToUseSearchClient:
      howToUseSearchClient();
      break;
    case cmdAddClient:
      addClient(clients, &nClientsRead);
      break;
    case cmdExit:
      exit = true;
      break;
    }
  }

  delete[] movies; // Deallocate memory
  delete[] clients;
}

// --- Functions

// Function to Output Help Message in the Terminal
void helpMessage()
{
  cout << clear;
  printTitle("WELCOME TO BLOCKBUSTER", applyBgColor, applyFgColor, false);
  cout << "Database Manipulation Commands\n"
       << tab1 << addBrackets(cmdsPtr[cmdAddMovie]) << " Add Movie\n"
       << tab1 << addBrackets(cmdsPtr[cmdRentMovie]) << " Rent Movie\n"
       << "Database Search Commands:\n"
       << tab1 << addBrackets(cmdsPtr[cmdMovieStatus]) << " Movie Status\n"
       << tab1 << addBrackets(cmdsPtr[cmdViewMovies]) << " View Movies\n"
       << tab1 << addBrackets(cmdsPtr[cmdFilterMovies]) << " Filter Movies\n"
       << tab1 << addBrackets(cmdsPtr[cmdSearchClient]) << " Search Client\n"
       << "Command Parameters:\n"
       << tab1 << addBrackets(cmdsPtr[cmdFieldParameters]) << " Movie Field Parameters\n"
       << tab1 << addBrackets(cmdsPtr[cmdSortByParameters]) << " Sort By Parameters\n"
       << tab1 << addBrackets(cmdsPtr[cmdClientParameters]) << " Search Client Parameters\n"
       << "How-To:\n"
       << tab1 << addBrackets(cmdsPtr[cmdHowToUseViewMovies]) << " How to Use the View Movie Command\n"
       << tab1 << addBrackets(cmdsPtr[cmdHowToUseFilterMovies]) << " How to Use the Filter Movie Command\n"
       << tab1 << addBrackets(cmdsPtr[cmdHowToUseSearchClient]) << " How to Use the Search Client Command\n"
       << "Other Commands:\n"
       << tab1 << addBrackets(cmdsPtr[cmdHelp]) << " Help\n"
       << tab1 << addBrackets(cmdsPtr[cmdExit]) << " Exit\n"
       << "Admin Privileges:\n"
       << tab1 << addBrackets(cmdsPtr[cmdAddClient]) << " Add Client\n";
}

// Function to Assign 2D Array to 1D Pointer, and Reset the Counters
void initPtrArray(string **ptrArray, string array[][maxParamPerSubCmd], int arrayCounter[], int n)
{
  for (int i = 0; i < n; i++)
  {
    ptrArray[i] = array[i];
    arrayCounter[i] = 0;
  }
}

// Function to Change Current Working Directory to 'src/data'
void changeCwdToData(string path)
{
  try
  {
    filesystem::path mainPath = path;                                // Path to main.exe
    filesystem::path binPath = mainPath.parent_path().parent_path(); // Path to Main Folder

    filesystem::path dataDir = "src/data";
    filesystem::path dataPath = binPath / dataDir; // Concatenate binPath with DataDir to get the FUll Path to the .csv Files

    filesystem::current_path(dataPath); // Change cwd to '.../src/data'
  }
  catch (...)
  {
    pressEnterToCont("Error: Executable File is not Inside 'bin' Folder", true);
  }
}

/*
// Function to Set CMD Terminal Codepage to UTF-8 if the OS is Windows
void winSetChcpUtf8()
{
#ifdef _WIN32                  // For Windows
  SetConsoleOutputCP(CP_UTF8); // Set Console Codepage to UTF-8 so Console knows how to Interpret String Data
#endif
}
*/

// --- Extern Variables and Constants Definition

// Lowercase for Ascending Order, Uppercase for Descending
int cmdsChar[cmdEnd] = {'a', 'r', 's', 'v', 'f', 'c', 'F', 'S', 'C', 'x', 'y', 'z', 'A', 'h', 'e'};
int subCmdsChar[subCmdEnd] = {'f', 's'};
int movieFieldCmdsChar[movieFieldEnd] = {'i', 't', 'd', 'g', 'D', 'p', 'r', 's', 'R', 'c', '.'};
int clientFieldCmdsChar[clientFieldEnd] = {'i', 'n', 'a'};
int movieSortByCmdsChar[movieSortByEnd] = {'d', 'D', 'i', 'I', 'p', 'P', 'r', 'R', 't', 'T'};
int clientSortByCmdsChar[clientSortByEnd] = {'i', 'I', 'n', 'N'};

// Command Title
string movieFieldCmdsStr[movieFieldEnd - 1] = {"Id", "Title", "Director", "Genre", "Min", "Price",
                                               "Release", "Rented", "Rent On", "Rent To"};
string clientFieldCmdsStr[clientFieldEnd] = {"Id", "Name", "Account Number"};

string movieSortByCmdsStr[movieSortByEnd] = {"Duration", "Id", "Price", "Release Date", "Title"};
string clientSortByCmdsStr[movieSortByEnd] = {"Id", "Name"};

// Genres
string genreStr[genreEnd] = {"Action", "Adventure", "Animation", "Children", "Comedy",
                             "Crime", "Documentary", "Drama", "Fantasy", "Film-Noir",
                             "Horror", "IMAX", "Mystery", "Musical", "Romance", "Sci-Fi",
                             "Thriller", "War", "Western", "(no genres listed)", "ERROR"};

// --- Extern Variables and Constants Assignment
int *cmdsPtr = cmdsChar;
int *subCmdsPtr = subCmdsChar;
int *movieFieldCmdsPtr = movieFieldCmdsChar;
int *clientFieldCmdsPtr = clientFieldCmdsChar;
int *movieSortByCmdsPtr = movieSortByCmdsChar;
int *clientSortByCmdsPtr = clientSortByCmdsChar;

string *movieFieldCmdsStrPtr = movieFieldCmdsStr;
string *clientFieldCmdsStrPtr = clientFieldCmdsStr;

string *movieSortByCmdsStrPtr = movieSortByCmdsStr;
string *clientSortByCmdsStrPtr = clientSortByCmdsStr;

string *genrePtr = genreStr;