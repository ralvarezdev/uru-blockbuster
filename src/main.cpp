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
extern bool movieValidFieldsToFilter[], clientValidFieldsToFilter[],
    movieValidFieldsToSort[], clientValidFieldsToSort[];
extern int cmdsChar[], subCmdsChar[], movieFieldCmdsChar[], clientFieldCmdsChar[];
extern char *movieFieldCmdsStr[], *clientFieldCmdsStr[], *genreStr[];

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
void checkCmd(bool isViewCmd, string input, bool *moreInput, bool *isField);
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

  Movies movies = Movies(); // Allocate Memory
  getMovies(&movies);       // Get Movies

  Clients clients = Clients(); // Allocate Memory
  getClients(&clients);        // Get Clients

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
        clientFields(); // Print the Valid Client Fields as Parameters
        break;
      case wrongSortByParam:
        sortByParameters(); // Print the Valid Sort By Parameters
        break;
      case wrongViewMoviesCmd:
        howToUseViewMovies(); // Print the Usage Examples of the View Movies Command
        break;
      case wrongViewClientsCmd:
        howToUseViewClients(); // Print Usage Examples of the View Client Command
        break;
      case wrongFilterMoviesCmd:
        howToUseFilterMovies(); // Print Usage Examples of the Filter Movies Command
        break;
      case wrongSearchClientsCmd:
        howToUseSearchClients(); // Print Usage Examples of the Search Client Command
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

    timesExec++; // Increase the Counter of Commands Executed
    assert(timesExec > 0);

    if (!getline(stream, inputWord, ' '))
    { // Couldn't Get the Command
      isCmd = noCmd;
      continue;
    }

    cmd.main = inputWord[0];
    index.main = isCharOnArray(cmd.main, cmdsChar, cmdEnd); // Check if the Command is in the Array of Main Commands. Returns -1 if it doesn't exist

    if (index.main == -1) // If it's not a Valid Command
      isCmd = wrongMainCmd;
    else if (index.main == cmdViewMovies || index.main == cmdFilterMovies || index.main == cmdViewClients || index.main == cmdSearchClients)
    { // Checks if the Filter, View Movies or Search Clients Command is Typed Correctly

      bool isViewCmd = (index.main == cmdViewMovies || index.main == cmdViewClients); // Boolean to Check if the Current Command is View Movies or View Clients
      bool isMovieData = (index.main == cmdViewMovies || index.main == cmdFilterMovies);

      int nSortBy = (isMovieData) ? movieFieldEnd - 1 : clientFieldEnd - 1; // Number of Possible Sort By Commands that can be Applied at the Same Time
      assert(nSortBy > 0);                                                  // Check if Enum sortByEnd is Greater than 0

      int sortByOrder[nSortBy], sortByCounter = 0; // Save Sorting Order
      fill(sortByOrder, sortByOrder + nSortBy, -1);

      if (isViewCmd && isMovieData)
      { // It's View Movies Command. Initialize viewMovies Sruct
        viewMoviesCmd = ViewMoviesCmd();
        fill(viewMoviesCmd.sortBy, viewMoviesCmd.sortBy + nSortBy, -1); // Initialize Sort By Array
      }
      else if (isViewCmd)
      { // It's View Clients Command. Initialize viewClients Struct
        viewClientsCmd = ViewClientsCmd();
        fill(viewClientsCmd.sortBy, viewClientsCmd.sortBy + nSortBy, -1); // Initialize Sort By Array
      }
      else if (isMovieData)
      { // It's Filter Movies Command. Initialize filterMovies Struct
        filterMoviesCmd = FilterMoviesCmd();
        for (int i = 0; i < movieFieldEnd - 1; i++)
          fill(filterMoviesCmd.params[i], filterMoviesCmd.params[i] + maxParamPerSubCmd, "null"); // Initialize Params to Filter Array
        initPtrArray(filterMoviesCmd.paramsPtr, filterMoviesCmd.params, filterMoviesCmd.counter, movieFieldEnd - 1);
        fill(filterMoviesCmd.sortBy, filterMoviesCmd.sortBy + nSortBy, -1); // Initialize Sort By Array
      }
      else
      { // // It's Search Clients Command. Initialize searchClients Struct
        searchClientsCmd = SearchClientsCmd();
        for (int i = 0; i < clientFieldEnd - 1; i++)
          fill(searchClientsCmd.params[i], searchClientsCmd.params[i] + maxParamPerSubCmd, "null"); // Initialize Params to Search Array
        initPtrArray(searchClientsCmd.paramsPtr, searchClientsCmd.params, searchClientsCmd.counter, clientFieldEnd - 1);
        fill(searchClientsCmd.sortBy, searchClientsCmd.sortBy + nSortBy, -1); // Initialize Sort By Array
      }

      try
      {
        int fieldsCounter = 0, sortByCounter = 0;

        while (true)
        {
          while (!moreInput && getline(stream, inputWord, ' '))
            if (inputWord[0] == '-')
              moreInput = true; // First Parameter Must be a Command

          if (!moreInput)
            break; // Exit this While-loop
          moreInput = false;
          isField = true;

          cmd.sub = inputWord[1]; // Check if the Command is in the Array of subCommands
          index.sub = isCharOnArray(cmd.sub, subCmdsChar, subCmdEnd);

          if (index.sub == -1) // Wrong  Command
            throw(wrongSubCmd);

          if (index.sub == subCmdSortBy)
          { // Get Sort By Parameters
            while (getline(stream, inputWord, ' '))
            {
              if (inputWord[0] == '-')
              { // It's not a Sort By Parameter
                moreInput = true;
                break;
              }

              // Check if the Command is in the Sort By Array
              cmd.param = inputWord[0];
              if (isMovieData)
                index.param = isCharOnArray(tolower(cmd.param), movieFieldCmdsChar, movieFieldEnd - 1);
              else
                index.param = isCharOnArray(tolower(cmd.param), clientFieldCmdsChar, clientFieldEnd - 1);

              if (index.param == -1 || (isMovieData && !movieValidFieldsToSort[index.param]) || (!isMovieData && !clientValidFieldsToSort[index.param])) // Wrong Sort By Command Parameter
                throw(wrongSortByParam);

              // If the Character is Uppercase, Increase the Index by One to Match with the Descending Order Command Index
              index.param *= 2;
              if (isupper(cmd.param))
                index.param++;
              sortByCounter++;

              if (isMovieData && isViewCmd)
                overwriteSortByParam(&viewMoviesCmd, &sortByCounter, sortByOrder, index.param);
              else if (isMovieData)
                overwriteSortByParam(&filterMoviesCmd, &sortByCounter, sortByOrder, index.param);
              else if (isViewCmd)
                overwriteSortByParam(&viewClientsCmd, &sortByCounter, sortByOrder, index.param);
              else
                overwriteSortByParam(&searchClientsCmd, &sortByCounter, sortByOrder, index.param);
            }

            if (sortByCounter != 0)
              continue;
            break;
          }

          if (!getline(stream, inputWord, ' '))
            if (isMovieData)
              throw((isViewCmd) ? wrongMovieField : wrongMovieFieldParam); // Missing Movie Parameter
            else
              throw((isViewCmd) ? wrongClientField : wrongClientFieldParam); // Missing Client Parameter

          while (isField) // Get Field Parameters
          {
            isField = false;

            checkCmd(isViewCmd, inputWord, &moreInput, &isField); // Check Command

            if (moreInput) // It's not a Field Command or a Parameter
              break;

            cmd.field = (isViewCmd) ? inputWord[0] : inputWord[2];
            if (isMovieData) // Check if the Command is in the Fields Array
              index.field = isCharOnArray(cmd.field, movieFieldCmdsChar, movieFieldEnd);
            else
              index.field = isCharOnArray(cmd.field, clientFieldCmdsChar, clientFieldEnd);

            if (isMovieData && (index.field == -1 || (!isViewCmd && !movieValidFieldsToFilter[index.field]))) // Wrong Field Parameter or Field Command
              throw((isViewCmd) ? wrongMovieFieldParam : wrongMovieField);
            else if (!isMovieData && (index.field == -1 || (!isViewCmd && !clientValidFieldsToFilter[index.field])))
              throw((isViewCmd) ? wrongClientFieldParam : wrongClientField);

            if (isViewCmd)
            {
              if (isMovieData)
                viewMoviesCmd.params[index.field] = true;
              else
                viewClientsCmd.params[index.field] = true;
              fieldsCounter++;

              if (getline(stream, inputWord, ' '))
                // Get the Next Argument
                checkCmd(isViewCmd, inputWord, &moreInput, &isField); // Check Command
              else if (sortByCounter == 0)
                throw(wrongSortByParam); // Missing Sort By Param

              continue;
            }

            int *paramCounter; // Counter

            if (isMovieData)
              paramCounter = &filterMoviesCmd.counter[index.field];
            else
              paramCounter = &searchClientsCmd.counter[index.field];

            while (*paramCounter < maxParamPerSubCmd && getline(stream, inputWord, ' '))
            {                              // Iterate while there's a Parameter and can be Added to the Array
              if (inputWord.length() == 0) // To Prevent Adding Whitespaces as Parameters-
                continue;
              else if (inputWord[0] != '"')
              {
                checkCmd(isViewCmd, inputWord, &moreInput, &isField); // Check Command
                if (moreInput)
                  break;
              }
              else
              { // If it Starts with Double Quote, it's a Long Sentence (a Parameter with Multiple Words)
                if (!getline(stream, inputLong, '"'))
                  throw((isMovieData) ? wrongFilterMoviesCmd : wrongSearchClientsCmd); // Incomplete Long Parameter

                inputWord.insert(inputWord.length(), 1, ' ');                             // Insert Whitespace at the End
                inputWord = inputLong.insert(0, inputWord.substr(1, inputWord.length())); // Insert the Parameter at the Beginning of the String, without the Double Quote
              }

              try
              {
                assert(inputWord.length() != 0); // Check inputWord String Length

                if (!isMovieData)
                { // Add Parameter to Search Client
                  if (index.field == clientFieldId)
                    stoi(inputWord); // Check if the String can be Converted into an Integer

                  searchClientsCmd.params[index.field][*paramCounter] = inputWord;
                }
                else // Add Parameter to Filter Movies
                {
                  if (index.field == movieFieldId || index.field == movieFieldDuration)
                    stoi(inputWord); // Check if the String can be Converted into an Integer
                  else if (index.field == movieFieldPrice)
                    stof(inputWord); // Check if the String can be Converted into a Float
                  else if (index.field == movieFieldGenre)
                  {
                    genreIndex = getGenreIndexLower(inputWord);
                    if (genreIndex == -1)
                      throw(-1); // Invalid Genre

                    filterMoviesCmd.params[index.field][*paramCounter] = genreStr[genreIndex];
                    continue;
                  }
                  filterMoviesCmd.params[index.field][*paramCounter] = inputWord;
                }
                fieldsCounter++;
              }
              catch (...)
              {
                continue; // Ignore the Parameter
              }

              *paramCounter += 1; // Parameter Counter
            }

            if (*paramCounter == 0) // Missing Field Param
              throw((isMovieData) ? wrongMovieFieldParam : wrongClientFieldParam);

            // Check if *paramCounter doesn't have Invalid Values
            assert(*paramCounter > 0 && *paramCounter <= maxParamPerSubCmd);

            while (!isField && !moreInput) // Reached Max Number of Parameters for Command
              if (!getline(stream, inputWord, ' '))
                break;
              else
                checkCmd(isViewCmd, inputWord, &moreInput, &isField); // Check Command
          }
        }

        for (int i = 0; i < nSortBy; i++) // Save the Sort By Array based on the Order they were Introduced
        {
          if (sortByOrder[i] == -1)
            continue;

          if (isViewCmd && isMovieData)
            viewMoviesCmd.sortBy[i] = sortByOrder[i];
          else if (isMovieData)
            filterMoviesCmd.sortBy[i] = sortByOrder[i];
          else if (isViewCmd)
            viewClientsCmd.sortBy[i] = sortByOrder[i];
          else
            searchClientsCmd.sortBy[i] = sortByOrder[i];
        }

        if (fieldsCounter == 0 && isMovieData)
          throw((isViewCmd) ? wrongMovieField : wrongMovieFieldParam);
        else if (fieldsCounter == 0)
          throw((isViewCmd) ? wrongClientField : wrongClientFieldParam);
        else if (sortByCounter == 0)
          throw(wrongSortByParam);
      }
      catch (cmdStatus cmdStatus)
      {
        isCmd = cmdStatus;
        assert(isCmd != validCmd); // Check if the Command is Invalid
      }
    }

    if (isCmd != validCmd)
    { // Chekc if the Command is Valid
      if (timesExec > 1 || (timesExec == 0 && argc == 1))
        cout << '\n'; // Print a New Line
      continue;
    }

    assert(index.main >= 0 && index.main < cmdEnd); // Check if the Command is Valid

    switch (index.main)
    {
    case cmdViewMovies:
      viewMovies(&movies, viewMoviesCmd.params, viewMoviesCmd.sortBy);
      break;
    case cmdFilterMovies:
      filterMovies(&movies, filterMoviesCmd.paramsPtr, filterMoviesCmd.sortBy);
      break;
    case cmdViewClients:
      viewClients(&clients, viewClientsCmd.params, viewClientsCmd.sortBy);
      break;
    case cmdSearchClients:
      searchClients(&clients, searchClientsCmd.paramsPtr, searchClientsCmd.sortBy);
      break;
    case cmdDisplayRentedMovies:
      displayRentedMovies(&movies);
      break;
    case cmdClientRentedMovies:
      clientRentedMovies();
      break;
    case cmdAddMovie:
      addMovie(&movies);
      break;
    case cmdRentMovie:
      rentMovie(&movies, &clients);
      break;
    case cmdReturnMovie:
      returnMovie(&movies, &clients);
      break;
    case cmdRemoveMovie:
      removeMovie(&movies);
      break;
    case cmdRemoveClient:
      removeClient(&movies, &clients);
      break;
    case cmdMovieStatus:
      getMovieStatus(&movies);
      break;
    case cmdMovieParameters:
      movieFields();
      break;
    case cmdClientParameters:
      clientFields();
      break;
    case cmdSortByParameters:
      sortByParameters();
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
    case cmdHowToUseViewClients:
      howToUseViewClients();
      break;
    case cmdHowToUseSearchClients:
      howToUseSearchClients();
      break;
    case cmdAddClient:
      addClient(&clients);
      break;
    case cmdExit:
      exit = true;
      break;
    }
  }

  movies.deallocate(); // Deallocate memory
  clients.deallocate();
}

// --- Functions

// Function to Output Help Message in the Terminal
void helpMessage()
{
  cout << clear;
  printTitle("WELCOME TO BLOCKBUSTER", applyBgColor, applyFgColor, false);
  cout << "Database Manipulation Commands\n"
       << tab1 << addBrackets(cmdsChar[cmdAddMovie]) << " Add Movie\n"
       << tab1 << addBrackets(cmdsChar[cmdRentMovie]) << " Rent Movie\n"
       << tab1 << addBrackets(cmdsChar[cmdReturnMovie]) << " Return Movie\n"
       << tab1 << addBrackets(cmdsChar[cmdAddClient]) << " Add Client\n"
       << tab1 << addBrackets(cmdsChar[cmdRemoveMovie]) << " Remove Movie\n"
       << tab1 << addBrackets(cmdsChar[cmdRemoveClient]) << " Remove Client\n"
       << "Database Search Commands:\n"
       << tab1 << addBrackets(cmdsChar[cmdMovieStatus]) << " Movie Status\n"
       << tab1 << addBrackets(cmdsChar[cmdViewMovies]) << " View Movies\n"
       << tab1 << addBrackets(cmdsChar[cmdFilterMovies]) << " Filter Movies\n"
       << tab1 << addBrackets(cmdsChar[cmdViewClients]) << " View Clients\n"
       << tab1 << addBrackets(cmdsChar[cmdSearchClients]) << " Search Client\n"
       << tab1 << addBrackets(cmdsChar[cmdDisplayRentedMovies]) << " Display Rented Movies\n"
       << tab1 << addBrackets(cmdsChar[cmdClientRentedMovies]) << " Display Client Rented Movies\n"
       << "Command Parameters:\n"
       << tab1 << addBrackets(cmdsChar[cmdSortByParameters]) << " Sort By Parameters\n"
       << tab1 << addBrackets(cmdsChar[cmdMovieParameters]) << " Movie Field Parameters\n"
       << tab1 << addBrackets(cmdsChar[cmdClientParameters]) << " Client Field Parameters\n"
       << tab1 << addBrackets(cmdsChar[cmdGenres]) << " Genres\n"
       << "How-To:\n"
       << tab1 << addBrackets(cmdsChar[cmdHowToUseViewMovies]) << " How to Use the View Movies Command\n"
       << tab1 << addBrackets(cmdsChar[cmdHowToUseFilterMovies]) << " How to Use the Filter Movies Command\n"
       << tab1 << addBrackets(cmdsChar[cmdHowToUseViewClients]) << " How to Use the View Clients Command\n"
       << tab1 << addBrackets(cmdsChar[cmdHowToUseSearchClients]) << " How to Use the Search Clients Command\n"
       << "Other Commands:\n"
       << tab1 << addBrackets(cmdsChar[cmdHelp]) << " Help\n"
       << tab1 << addBrackets(cmdsChar[cmdExit]) << " Exit\n";
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
void checkCmd(bool isViewCmd, string input, bool *moreInput, bool *isField)
{
  bool isCmd = input[0] == '-';
  bool isFieldCommand = isCmd && input[1] == '-';
  int inputLength = input.length();

  if (isViewCmd)
  {
    if (isCmd) // It's a Subcommand
      *moreInput = true;
    else // It's a Field Parameter
      *isField = true;
  }
  else if (isCmd && !isFieldCommand) // It's a Subcommand
    *moreInput = true;
  else if (isFieldCommand) // It's a Field Parameter
    *isField = true;
}

// --- Extern Variables and Constants Assignment

int cmdsChar[cmdEnd] = { // Commands Character
    [cmdAddMovie] = 'a',
    [cmdMovieStatus] = 's',
    [cmdRentMovie] = 'r',
    [cmdReturnMovie] = 'R',
    [cmdRemoveMovie] = 'm',
    [cmdAddClient] = 'A',
    [cmdRemoveClient] = 'c',
    [cmdViewMovies] = 'v',
    [cmdFilterMovies] = 'f',
    [cmdViewClients] = 'V',
    [cmdSearchClients] = 'F',
    [cmdDisplayRentedMovies] = 'd',
    [cmdClientRentedMovies] = 'D',
    [cmdMovieParameters] = 'M',
    [cmdClientParameters] = 'C',
    [cmdSortByParameters] = 'S',
    [cmdGenres] = 'G',
    [cmdHowToUseViewMovies] = 'w',
    [cmdHowToUseFilterMovies] = 'x',
    [cmdHowToUseViewClients] = 'y',
    [cmdHowToUseSearchClients] = 'z',
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
    [movieFieldDuration] = 'm',
    [movieFieldPrice] = 'p',
    [movieFieldRelease] = 'r',
    [movieFieldStatus] = 's',
    [movieFieldRentOn] = 'o',
    [movieFieldRentTo] = 'c',
    [movieFieldAll] = '.'};

char *movieFieldCmdsStr[movieFieldEnd] = { // Movie Fields Name
    [movieFieldId] = "Id",
    [movieFieldName] = "Title",
    [movieFieldDirector] = "Director",
    [movieFieldGenre] = "Genre",
    [movieFieldDuration] = "Min",
    [movieFieldPrice] = "Price",
    [movieFieldRelease] = "Release",
    [movieFieldStatus] = "Rented",
    [movieFieldRentOn] = "Rent On",
    [movieFieldRentTo] = "Rent To",
    [movieFieldAll] = "All"};

bool movieValidFieldsToSort[movieFieldEnd] = { // Fields that can be Used in Sort By Movies Subcommand
    [movieFieldId] = true,
    [movieFieldName] = true,
    [movieFieldDirector] = true,
    [movieFieldGenre] = false,
    [movieFieldDuration] = true,
    [movieFieldPrice] = true,
    [movieFieldRelease] = true,
    [movieFieldStatus] = true,
    [movieFieldRentOn] = true,
    [movieFieldRentTo] = true,
    [movieFieldAll] = false};

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

bool clientValidFieldsToSort[clientFieldEnd] = { // Fields that can be Used in Sort By Clients Subcommand
    [clientFieldAccountNumber] = true,
    [clientFieldId] = true,
    [clientFieldName] = true,
    [clientFieldPhoneNumber] = true,
    [clientFieldAll] = false};

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