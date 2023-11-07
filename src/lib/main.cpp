#include <iostream>
#include <filesystem>
#include "ansiEsc.h"
#include "data.h"
#include "input.h"

using namespace std;

/*
--- NOTES
- For this proyect we're not allowed to use maps, vectors, classes, linked lists, tuples.
That's the reason why the program is written like this
*/

// --- Extern Variables and Constants Declaration
extern const string clear, tab1;
extern const bool applyBgColor, applyFgColor;
extern char *cmdsPtr, *subCmdsPtr, *fieldCmdsPtr, *clientCmdsPtr, *sortByCmdsPtr;

// --- Global variables
const int maxParamPerSubCmd = 6; // Max Number of Parameters per Subcommand

// --- Structs

// - View Movies Command Parameters
struct ViewMoviesCmd
{
  bool params[fieldEnd];     // 1D Array to Save the Fields to Show in View Movies
  int sortBy[sortByEnd / 2]; // For a Field, only Allowed Ascending or Descending Order, not Both at the Same Time
};

// - Filter Movies Command Parameters
struct FilterMoviesCmd
{
  string params[fieldEnd][maxParamPerSubCmd]; // 2D String Array of Field Parameters
  string *paramsPtr[fieldEnd];                // 1D Pointer Array to to the 2D Array
  int counter[fieldEnd];                      // Filter Movies Field Parameters Counter
  int sortBy[sortByEnd / 2];                  // For a Field, only Allowed Ascending or Descending Order, not Both at the Same Time
};

// - Search Client Command Parameters
struct SearchClientCmd
{
  string params[clientEnd][maxParamPerSubCmd]; // 2D String Array of Clients Parameters
  string *paramsPtr[clientEnd];                // 1D Pointer Array to to the 2D Array
  int counter[clientEnd];
};

// - Command
struct Cmd
{ // Command, SubCommand, Field, Parameter
  char main;
  char sub;
  char field;
  char param;
};

// - Command Indexes
struct CmdIndex
{ // Used to Save the Index of the Command on its Corresponding Array
  int main = 0;
  int sub = 0;
  int field = 0;
  int param = 0;
};

// --- Function Prototypes
void helpMessage();
void initPtrArray(string **ptrArray, string array[][maxParamPerSubCmd], int arrayCounter[], int n);
void changeCwdToData(string path);

int main(int argc, char **argv)
{
  ViewMoviesCmd viewMoviesCmd; // Used to Save the Parameters Typed by the User for the given Command
  FilterMoviesCmd filterMoviesCmd;
  SearchClientCmd searchClientCmd;
  Cmd cmd;
  CmdIndex index;

  bool exit = false;                      // Tells the Program if the User wants to Exit the Program
  bool moreInput, isField;                // Boolean to Check if there's More Input
  int timesExec = 0;                      // Number of times the Main While Loop has been executed
  int isCmd = validCmd;                   // Used for Checking if the Command is Valid or Not. If not, it Stores the Reason
  string inputLine, inputWord, inputLong; // Saves the Input of the User, before being Processed by the Program

  // Change Current Working Path to 'src/data'
  changeCwdToData(argv[0]); // Change Current Working Directory

  // Main While Loop of the Program
  while (!exit)
  {
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

        // Whitespace Found
        if (found != string::npos) // Insert a Double Quote at the Beginning and the End of the Input
          inputWord.insert(0, 1, '"').insert(inputWord.length(), 1, '"');

        if (i != argc - 1) // Insert Whitespace between Parameters
          inputWord.insert(inputWord.length(), 1, ' ');

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

    if (index.main == -1)
    { // If it's not a Valid Command
      isCmd = wrongMainCmd;
    }
    else if (index.main == cmdViewMovies || index.main == cmdFilterMovies || index.main == cmdSearchClient)
    {                                                           // Checks if the Filter or View Movies Command is Typed Correctly
      bool isViewMoviesCmd = (index.main == cmdViewMovies);     // Boolean to Check if the Current Command is View Movies
      bool isSearchClientCmd = (index.main == cmdSearchClient); // To Shorten the If-Else Statements

      if (isViewMoviesCmd)
      { // Initialize viewMovies Sruct
        viewMoviesCmd = ViewMoviesCmd();
        fill(viewMoviesCmd.sortBy, viewMoviesCmd.sortBy + sortByEnd, -1); // Initialize Sort By Array

        /*
        int indexFieldTrue[4] = {fieldName, fieldStatus, fieldRentOn, fieldRentTo}; // Fields that are Always Printed in the Terminal
        for (int i = 0; i < sizeof(indexFieldTrue) / sizeof(indexFieldTrue[0]); i++)
          viewMoviesCmd.params[indexFieldTrue[i]] = true;
        */
      }
      else if (!isSearchClientCmd)
      { // Initialize filterMovies Struct
        filterMoviesCmd = FilterMoviesCmd();
        fill(filterMoviesCmd.sortBy, filterMoviesCmd.sortBy + sortByEnd, -1); // Initialize Sort By Array
        initPtrArray(filterMoviesCmd.paramsPtr, filterMoviesCmd.params, filterMoviesCmd.counter, fieldEnd);
      }
      else
      { // Initialize searchClient Struct
        searchClientCmd = SearchClientCmd();
        initPtrArray(searchClientCmd.paramsPtr, searchClientCmd.params, searchClientCmd.counter, clientEnd);
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
          index.field = isCharOnArray(cmd.field, clientCmdsPtr, clientEnd);
        }
        else
        { // Check if the Command is i the Array of subCommands
          cmd.sub = inputWord[1];
          index.sub = isCharOnArray(cmd.sub, subCmdsPtr, subCmdEnd);
        }

        if (inputWord[0] != '-' || index.sub == -1 || index.field == -1)
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
            index.param = isCharOnArray(cmd.param, sortByCmdsPtr, sortByEnd); // Check if the Command is in the Sort By Array

            if (index.param == -1)
            { // Wrong Sort By Command Parameter
              isCmd = wrongSortByParam;
              break;
            }

            if (isViewMoviesCmd) // It will Overwrite the Previous Sorting for this Parameter, if it was Specified
              viewMoviesCmd.sortBy[index.param / 2] = index.param;
            else
              filterMoviesCmd.sortBy[index.param / 2] = index.param;
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
            index.field = isCharOnArray(cmd.field, fieldCmdsPtr, fieldEnd); // Check if the Command is in the Field Parameters Array

            if (index.field == -1 || (!isViewMoviesCmd && index.field == fieldAll))
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
            else if (inputWord.length() == 0)
            { // To Prevent Adding Whitespaces as Parameters-
              continue;
            }

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

      if (isCmd == validCmd)
      {
        switch (index.main)
        {
        case cmdViewMovies:
          viewMovies(viewMoviesCmd.params, fieldEnd, viewMoviesCmd.sortBy, sortByEnd / 2);
          break;
        case cmdFilterMovies:
          filterMovies(filterMoviesCmd.paramsPtr, fieldEnd - 1, maxParamPerSubCmd, filterMoviesCmd.sortBy, sortByEnd / 2);
          break;
        case cmdSearchClient:
          searchClient(searchClientCmd.paramsPtr, clientEnd, maxParamPerSubCmd);
          break;
        }
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
      addMovie();
      break;
    case cmdRentMovie:
      rentMovie();
      break;
    case cmdMovieStatus:
      movieStatus();
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
      addClient();
      break;
    case cmdExit:
      exit = true;
      break;
    }
  }
}

// --- Functions

// Function to Output Help Message in the Terminal
void helpMessage()
{
  cout << clear;
  printTitle("WELCOME TO BLOCKBUSTER", applyBgColor, applyFgColor, false);
  cout << "Database Manipulation Commands\n"
       << tab1 << "[" << cmdsPtr[cmdAddMovie] << "] Add Movie\n"
       << tab1 << "[" << cmdsPtr[cmdRentMovie] << "] Rent Movie\n"
       << "Database Search Commands:\n"
       << tab1 << "[" << cmdsPtr[cmdMovieStatus] << "] Movie Status\n"
       << tab1 << "[" << cmdsPtr[cmdViewMovies] << "] View Movies\n"
       << tab1 << "[" << cmdsPtr[cmdFilterMovies] << "] Filter Movies\n"
       << tab1 << "[" << cmdsPtr[cmdSearchClient] << "] Search Client\n"
       << "Command Parameters:\n"
       << tab1 << "[" << cmdsPtr[cmdFieldParameters] << "] Movie Field Parameters\n"
       << tab1 << "[" << cmdsPtr[cmdSortByParameters] << "] Sort By Parameters\n"
       << tab1 << "[" << cmdsPtr[cmdClientParameters] << "] Search Client Parameters\n"
       << "How-To:\n"
       << tab1 << "[" << cmdsPtr[cmdHowToUseViewMovies] << "] How to Use the View Movie Command\n"
       << tab1 << "[" << cmdsPtr[cmdHowToUseFilterMovies] << "] How to Use the Filter Movie Command\n"
       << tab1 << "[" << cmdsPtr[cmdHowToUseSearchClient] << "] How to Use the Search Client Command\n"
       << "Other Commands:\n"
       << tab1 << "[" << cmdsPtr[cmdHelp] << "] Help\n"
       << tab1 << "[" << cmdsPtr[cmdExit] << "] Exit\n"
       << "Admin Privileges:\n"
       << tab1 << "[" << cmdsPtr[cmdAddClient] << "] Add Client\n";
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
  filesystem::path mainPath = path.substr(0, path.length() - 13); // Path to Main Folder
  filesystem::path dataDir = "src/data";
  filesystem::path dataPath = mainPath / dataDir; // Concatenate mainPath with Data Dir

  filesystem::current_path(dataPath); // Chenge cwd to '.../src/data'
}