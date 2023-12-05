/*
#ifdef _WIN32 // For Windows
#include <windows.h>
#endif
*/

#include <iostream>
#include <filesystem>

#include "lib/namespaces.h"
#include "lib/clients/clientsOp.h"
#include "lib/movies/moviesOp.h"
#include "lib/data/dataOp.h"
#include "lib/terminal/ansiEsc.h"
#include "lib/terminal/input.h"

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
extern bool movieValidFieldsToFilter[], clientValidFieldsToFilter[];
extern char *movieFieldCmdsStr[], *clientFieldCmdsStr[], *genreStr[];
extern int cmdsChar[], subCmdsChar[], movieFieldCmdsChar[], clientFieldCmdsChar[];

// --- Templates
template <typename T>
void overwriteSortByParam(T *cmdStruct, int *counter, int sortByOrder[], int indexParam)
{ // It will Overwrite the Previous Sorting for this Parameter, if it was Specified
  if ((*cmdStruct).sortBy[indexParam / 2] != -1)
  {
    sortByOrder[*counter] = indexParam;
    *counter = *counter + 1;
  }

  (*cmdStruct).sortBy[indexParam / 2] = indexParam;
}

// --- Function Prototypes
void helpMessage();
void changeCwdToData();
void checkCmd(bool isViewClientsCmd, string input, bool *moreInput, bool *isField);
void initPtrArray(string **ptrArray, string array[][maxParamPerSubCmd], int arrayCounter[], int n);

// Main Function
int main(int argc, char **argv)
{
  // winSetChcpUtf8();                 // Change Codepage While Running the Program to UTF-8  if it's Running in Windows
  std::ios::sync_with_stdio(false); // Desynchronize C++ Streams from C I/O Operations to Increase Performance

  ViewMoviesCmd viewMoviesCmd; // Used to Save the Parameters Typed by the User for the Given Command
  FilterMoviesCmd filterMoviesCmd;
  ViewClientsCmd viewClientsCmd;
  SearchClientsCmd searchClientsCmd;
  Cmd cmd;
  CmdIndex index;

  bool exit = false;                      // Tells the Program if the User wants to Exit the Program
  bool moreInput, isField;                // Boolean to Check if there's More Input
  cmdStatus isCmd = validCmd;             // Used for Checking if the Command is Valid or Not. If not, it Stores the Reason
  int genreIndex, timesExec = 0;          // Number of times the Main While Loop has been executed
  string inputLine, inputWord, inputLong; // Saves the Input of the User, before being Processed by the Program

  changeCwdToData(); // Change Current Working Path to 'src/data'

  Movies movies = new Movie[nMovies]; // Allocate Memory
  nMoviesRead = getMovies(&movies);   // Get Movies

  Client clients = new Client[nClients]; // Allocate Memory
  nClientsRead = getClients(&clients);   // Get Clients

  if (clients.getNumberClients() < 0 || movies.getNumberMovies() < 0)
    return -1; // An Error Ocurred

  while (!exit) // Main While Loop of the Program
  {
    cmd = Cmd();        // Initialize Command Structure to 0
    index = CmdIndex(); // Initialize Command Indexes to 0

    if (isCmd != validCmd)
    { // If in the Last Execution the User Typed a Wrong Command
      wrongCommand(isCmd);

      switch (isCmd)
      {
      case wrongMovieField:
      case wrongMovieFieldParam:
        movieFields(); // Print the Valid Movie Fields as Commands or as Parameters
        break;
      case wrongClientFieldParam:
        clientParameters(); // Print the Valid Client Fields as Parameters
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

      int nSortBy = (isSearchClientCmd) ? clientSortByEnd / 2 : movieSortByEnd / 2; // Number of Possible Sort By Commands that can be Applied at the Same Time

      int sortByOrder[nSortBy], sortByCounter = 0; // Save Sorting Order
      for (int i = 0; i < nSortBy; i++)
        sortByOrder[i] = -1;

      if (isViewMoviesCmd)
      { // Initialize viewMovies Sruct
        viewMoviesCmd = ViewMoviesCmd();
        fill(viewMoviesCmd.sortBy, viewMoviesCmd.sortBy + nSortBy, -1); // Initialize Sort By Array
      }
      else if (!isSearchClientCmd)
      { // Initialize filterMovies Struct
        filterMoviesCmd = FilterMoviesCmd();
        for (int i = 0; i < movieFieldEnd - 1; i++)
          fill(filterMoviesCmd.params[i], filterMoviesCmd.params[i] + maxParamPerSubCmd, "null"); // Initialize Params to Filter Array
        initPtrArray(filterMoviesCmd.paramsPtr, filterMoviesCmd.params, filterMoviesCmd.counter, movieFieldEnd);
        fill(filterMoviesCmd.sortBy, filterMoviesCmd.sortBy + nSortBy, -1); // Initialize Sort By Array
      }
      else
      { // Initialize searchClient Struct
        searchClientsCmd = SearchClientCmd();
        for (int i = 0; i < clientFieldEnd; i++)
          fill(searchClientsCmd.params[i], searchClientsCmd.params[i] + maxParamPerSubCmd, "null"); // Initialize Params to Search Array
        initPtrArray(searchClientsCmd.paramsPtr, searchClientsCmd.params, searchClientsCmd.counter, clientFieldEnd);
        fill(searchClientsCmd.sortBy, searchClientsCmd.sortBy + nSortBy, -1); // Initialize Sort By Array
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

        if (inputWord.length() < 2)
        { // Check if the Command has the Minimum String Length
          isCmd = wrongSubCmd;
          break;
        }

        cmd.sub = inputWord[1]; // Check if the Command is in the Array of subCommands
        index.sub = isCharOnArray(cmd.sub, subCmdsPtr, subCmdEnd);

        if (inputWord[0] != '-' || index.sub == -1)
        { // Wrong  Command
          isCmd = wrongSubCmd;
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
            if (isSearchClientCmd) // Check if the Command is in the Sort By Array
              index.param = isCharOnArray(cmd.param, clientSortByCmdsPtr, clientSortByEnd);
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
            else if (!isSearchClientCmd)
            {
              if (filterMoviesCmd.sortBy[index.param / 2] != -1)
                sortByOrder[sortByCounter++] = index.param;

              filterMoviesCmd.sortBy[index.param / 2] = index.param;
            }
            else
            {
              if (searchClientsCmd.sortBy[index.param / 2] != -1)
                sortByOrder[sortByCounter++] = index.param;

              searchClientsCmd.sortBy[index.param / 2] = index.param;
            }
          }
          continue;
        }

        getline(stream, inputWord, ' ');

        while (isField) // Get Field Parameters
        {
          isField = false;

          if (inputWord[0] == '-' && (isViewMoviesCmd || (!isViewMoviesCmd && inputWord.length() < 3)))
          { // It's not a Field Command or a Parameter
            moreInput = true;
            break;
          }

          cmd.field = (isViewMoviesCmd) ? inputWord[0] : inputWord[2];
          if (isSearchClientCmd) // Check if the Command is in the Fields Array
            index.field = isCharOnArray(cmd.field, clientFieldCmdsPtr, clientFieldEnd);
          else
            index.field = isCharOnArray(cmd.field, movieFieldCmdsPtr, movieFieldEnd);

          if (index.field == -1 || (!isViewMoviesCmd && !isSearchClientCmd && !movieValidFieldFilterPtr[index.field]))
          { // Wrong Field Parameter or Field Command
            if (!isSearchClientCmd)
              isCmd = (isViewMoviesCmd) ? wrongMovieFieldParam : wrongMovieField;
            else
              isCmd = wrongClientFieldParam;
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

          int *paramCounter; // Counter

          if (isSearchClientCmd)
            paramCounter = &searchClientsCmd.counter[index.field];
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

              inputWord.insert(inputWord.length(), 1, ' ');                             // Insert Whitespace at the End
              inputWord = inputLong.insert(0, inputWord.substr(1, inputWord.length())); // Insert the Parameter at the Beginning of the String, without the Double Quote
            }
            else if (inputWord[0] == '-')
            { // If it's a Command break this For-loop
              if (inputWord.length() < 3)
                moreInput = true;
              else if (inputWord.length() > 2)
                isField = true;
              break;
            }
            else if (inputWord.length() == 0) // To Prevent Adding Whitespaces as Parameters-
              continue;

            try
            {
              if (isSearchClientCmd)
              { // Add Parameter to Search Client
                if (index.field == clientFieldId)
                  stoi(inputWord); // Check if the String can be Converted into an Integer

                searchClientsCmd.params[index.field][*paramCounter] = inputWord;
              }
              else // Add Parameter to Filter Movies
              {
                if (index.field == movieFieldId || index.field == movieFieldPrice || index.field == movieFieldDuration)
                  stoi(inputWord); // Check if the String can be Converted into an Integer
                else if (index.field == movieFieldGenre)
                {
                  genreIndex = getGenreIndexLower(inputWord);
                  if (genreIndex == -1)
                    throw(-1); // Invalid Genre

                  filterMoviesCmd.params[index.field][*paramCounter] = genrePtr[genreIndex];
                  *paramCounter = *paramCounter + 1; // Parameter Counter
                  continue;
                }
                filterMoviesCmd.params[index.field][*paramCounter] = inputWord;
              }
            }
            catch (...)
            {
              continue; // Ignore the Parameter
            }

            *paramCounter = *paramCounter + 1; // Parameter Counter
          }

          while (!isField && !moreInput) // Reached Max Number of Parameters for Command
            if (!getline(stream, inputWord, ' '))
              break;
            else if (inputWord[0] == '-') // Got a Command
              if (inputWord.length() < 3)
                moreInput = true;
              else if (inputWord.length() > 2)
                isField = true;
        }
      }

      for (int i = 0; i < nSortBy; i++) // Save the Sort By Array based on the Order they were Introduced
        if (sortByOrder[i] != -1)
          if (isViewMoviesCmd)
            viewMoviesCmd.sortBy[i] = sortByOrder[i];
          else if (!isSearchClientCmd)
            filterMoviesCmd.sortBy[i] = sortByOrder[i];
          else
            searchClientsCmd.sortBy[i] = sortByOrder[i];

      if (isCmd == validCmd)
        switch (index.main)
        {
        case cmdViewMovies:
          viewMovies(movies, nMoviesRead, viewMoviesCmd.params, viewMoviesCmd.sortBy);
          break;
        case cmdFilterMovies:
          filterMovies(movies, nMoviesRead, filterMoviesCmd.paramsPtr, filterMoviesCmd.counter, filterMoviesCmd.sortBy);
          break;
        case cmdSearchClient:
          searchClient(clients, nClientsRead, searchClientsCmd.paramsPtr, searchClientsCmd.counter, searchClientsCmd.sortBy);
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
    case cmdMovieParameters:
      movieFields();
      break;
    case cmdSortByParameters:
      sortByParameters();
      break;
    case cmdClientParameters:
      clientParameters();
      break;
    case cmdGenres:
      cout << clear; // Clear Terminal
      validGenres();
      pressEnterToCont("Press ENTER to Continue", false);
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
       << tab1 << addBrackets(cmdsPtr[cmdSortByParameters]) << " Sort By Parameters\n"
       << tab1 << addBrackets(cmdsPtr[cmdMovieParameters]) << " Movie Field Parameters\n"
       << tab1 << addBrackets(cmdsPtr[cmdClientParameters]) << " Search Client Parameters\n"
       << tab1 << addBrackets(cmdsPtr[cmdGenres]) << " Genres\n"
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
void changeCwdToData()
{
  try
  {
    filesystem::path mainCppPath = __FILE__;              // Path to main.cpp File
    filesystem::path srcPath = mainCppPath.parent_path(); // Path to 'src' Folder

    filesystem::path dataDir = "data";
    filesystem::path dataPath = srcPath / dataDir; // Concatenate srcPath with DataDir to get the FUll Path to the .csv Files

    filesystem::current_path(dataPath); // Change cwd to '.../src/data'
  }
  catch (...)
  {
    pressEnterToCont("Error: main.cpp File is not Inside 'lib' Folder", true);
  }
}

// Function to Check Command Typed as Input
void checkCmd(bool isViewClientsCmd, string input, bool *moreInput, bool *isField)
{
  bool isCmd = input[0] == '-';
  int inputLength = input.length();

  if (isViewClientsCmd)
  {
    if (isCmd && inputLength < 3 && inputLength > 0) // It's a Subcommand
      *moreInput = true;
    else if (!isCmd && inputLength > 0) // It's a Field Parameter
      *isField = true;
  }
  else if (isCmd)        // Got a Command
    if (inputLength < 3) // It's a Subcommand
      *moreInput = true;
    else if (inputLength > 2) // It's a Field Parameter
      *isField = true;
}

// --- Extern Variables and Constants Assignment

int cmdsChar[cmdEnd] = { // Commands Character
    [cmdAddMovie] = 'a',
    [cmdRentMovie] = 'r',
    [cmdMovieStatus] = 's',
    [cmdViewMovies] = 'v',
    [cmdFilterMovies] = 'f',
    [cmdSearchClient] = 'c',
    [cmdMovieParameters] = 'F',
    [cmdSortByParameters] = 'S',
    [cmdClientParameters] = 'C',
    [cmdGenres] = 'G',
    [cmdHowToUseViewMovies] = 'x',
    [cmdHowToUseFilterMovies] = 'y',
    [cmdHowToUseSearchClient] = 'z',
    [cmdAddClient] = 'A',
    [cmdHelp] = 'h',
    [cmdExit] = 'e'};

int subCmdsChar[subCmdEnd] = { // Subcommands
    [subCmdField] = 'f',
    [subCmdSortBy] = 's'};

int movieFieldCmdsChar[movieFieldEnd] = { // Movie Fields Command Character
    [movieFieldId] = 'i',
    [movieFieldName] = 't',
    [movieFieldDirector] = 'd',
    [movieFieldGenre] = 'g',
    [movieFieldDuration] = 'D',
    [movieFieldPrice] = 'p',
    [movieFieldRelease] = 'r',
    [movieFieldStatus] = 's',
    [movieFieldRentOn] = 'R',
    [movieFieldRentTo] = 'c',
    [movieFieldAll] = '.'};

char *movieFieldCmdsStr[movieFieldEnd - 1] = { // Movie Fields Name
    [movieFieldId] = "Id",
    [movieFieldName] = "Title",
    [movieFieldDirector] = "Director",
    [movieFieldGenre] = "Genre",
    [movieFieldDuration] = "Min",
    [movieFieldPrice] = "Price",
    [movieFieldRelease] = "Release",
    [movieFieldStatus] = "Rented",
    [movieFieldRentOn] = "Rent On",
    [movieFieldRentTo] = "Rent To"};

bool movieValidFieldsToFilter[movieFieldEnd] = { // Fields that can be Used in Filter Movies Command
    [movieFieldId] = true,
    [movieFieldName] = true,
    [movieFieldDirector] = true,
    [movieFieldGenre] = true,
    [movieFieldDuration] = true,
    [movieFieldPrice] = true,
    [movieFieldRelease] = false,
    [movieFieldStatus] = false,
    [movieFieldRentOn] = false,
    [movieFieldRentTo] = false,
    [movieFieldAll] = false};

int clientFieldCmdsChar[clientFieldEnd] = { // Client Fields Command Character
    [clientFieldAccountNumber] = 'a',
    [clientFieldId] = 'i',
    [clientFieldName] = 'n',
    [clientFieldPhoneNumber] = 'p',
    [clientFieldAll] = '.'};

char *clientFieldCmdsStr[clientFieldEnd] = { // Client Fields Name
    [clientFieldAccountNumber] = "Account Number",
    [clientFieldId] = "Id",
    [clientFieldName] = "Name",
    [clientFieldPhoneNumber] = "Phone Number",
    [clientFieldAll] = "All"};

bool clientValidFieldsToFilter[clientFieldEnd] = { // Fields that can be Used in Filter Clients Command
    [clientFieldAccountNumber] = true,
    [clientFieldId] = true,
    [clientFieldName] = true,
    [clientFieldPhoneNumber] = true,
    [clientFieldAll] = false};

char *genreStr[genreEnd] = { // Genres
    [genreAction] = "Action",
    [genreAdventure] = "Adventure",
    [genreAnimation] = "Animation",
    [genreChildren] = "Children",
    [genreComedy] = "Comedy",
    [genreCrime] = "Crime",
    [genreDocumentary] = "Documentary",
    [genreDrama] = "Drama",
    [genreFantasy] = "Fantasy",
    [genreFilmNoir] = "Film-Noir",
    [genreHorror] = "Horror",
    [genreIMax] = "IMAX",
    [genreMusical] = "Musical",
    [genreMystery] = "Mystery",
    [genreRomance] = "Romance",
    [genreSciFi] = "Sci-Fi",
    [genreThriller] = "Thriller",
    [genreWar] = "War",
    [genreWestern] = "Western",
    [noneGenre] = "(no genres listed)",
    [errorGenre] = "ERROR"};