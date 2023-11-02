#include <iostream>
#include <iomanip>
#include <string>
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
extern char *cmdsPtr, *subCmdsPtr, *fieldsCmdsPtr, *clientCmdsPtr, *sortByCmdsPtr;

// --- Global variables
const int maxParamPerSubCmd = 3; // Max Number of Parameters per Subcommand

// --- Structs

struct cmdParams
{
  string clients[clientEnd][maxParamPerSubCmd]; // 2D String Array of Clients Parameters
  string *ptrClients[clientEnd];                // 1D Pointer Array to to the 2D Array
  int clientsCounter[clientEnd];

  bool fields[fieldEnd];          // 1D Array to Save the Fields to Show in View Movies
  char sortBy[sortByEnd / 2 + 1]; // For a Field, only Allowed Ascending or Descending Order, not Both at the Same Time

  string fieldParams[fieldEnd][maxParamPerSubCmd]; // 2D String Array of Field Parameters
  string *ptrFieldParams[fieldEnd];                // 1D Pointer Array to to the 2D Array
  int fieldParamsCounter[fieldEnd];                // Filter Movies Field Parameters Counter
};

// --- Function Prototypes
void helpMessage();
void initParamArray(string **ptrArray, string array[][maxParamPerSubCmd], int arrayCounter[], int n);

int main(int argc, char **argv)
{
  bool exit = false;                                 // Tells the Program if the User wants to Exit the Program
  char cmd, subCmd, field, param;                    // Command, SubCommand, Field, Parameter
  int isCmd = validCmd;                              // Used for Checking if the Command is Valid or Not. If not, it Stores the Reason
  int cmdIndex, subCmdIndex, fieldIndex, paramIndex; // Used to Save the Index of the Command on its Corresponding Array
  int timesExec = 0;                                 // Number of times the Main While Loop has been executed
  string inputLine, inputWord, inputLong;            // Saves the Input of the User, before being Processed by the Program

  // Main While Loop of the Program
  while (!exit)
  {
    cmdIndex = subCmdIndex = fieldIndex = paramIndex = 0; // Reset Values

    if (isCmd != validCmd)
    { // If in the Last Execution the User Typed a Wrong Command
      wrongCommand(isCmd);

      switch (isCmd)
      {
      case wrongFieldParam:
        fieldParameters(); // Print the Valid Field Parameters
        break;
      case wrongSortByParam:
        sortByParameters(); // Print the Valid Sort By Parameters
        break;
      case wrongViewCmd:
        howToUseViewMovies(); // Print the Usage Examples of the View Movies Command
        break;
      case wrongFilterCmd:
        howToUseFilterMovies(); // Print Usage Examples of the Filter Movies Commands
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
        if (found != string::npos)
        {                                               // Whitespace Found
          inputWord.insert(0, 1, '"');                  // Insert a Double Quote to the Beggining of the Input
          inputWord.insert(inputWord.length(), 1, '"'); // Insert a Double Quote at the End
        }

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
    }

    stringstream stream(inputLine); // Stream the Input
    timesExec++;                    // Increase the Counter of Commands Executed

    if (!getline(stream, inputWord, ' '))
    { // Couldn't Get the Command
      isCmd = noCmd;
      continue;
    }

    cmd = inputWord[0];
    cmdIndex = isCharOnArray(cmd, cmdsPtr, cmdEnd); // Check if the Command is in the Array of Main Commands. Returns -1 if it doesn't exist

    if (cmdIndex == -1)
    { // If it's not a Valid Command
      isCmd = wrongMainCmd;
    }
    else if (cmdIndex == cmdViewMovies || cmdIndex == cmdFilterMovies || cmdIndex == cmdSearchClient)
    {                      // Checks if the Filter or View Movies Command is Typed Correctly
      cmdParams userInput; // Parameters given by the User

      if (cmdIndex == cmdFilterMovies)
        initParamArray(userInput.ptrClients, userInput.clients, userInput.clientsCounter, clientEnd);
      else if (cmdIndex == cmdSearchClient)
        initParamArray(userInput.ptrFieldParams, userInput.fieldParams, userInput.fieldParamsCounter, fieldEnd);

      bool moreInput = true;                                  // Boolean to Check if there's More Input
      bool isViewMoviesCmd = (cmdIndex == cmdViewMovies);     // Boolean to Check if the Current Command is View Movies
      bool isSearchClientCmd = (cmdIndex == cmdSearchClient); // To Shorten the If-Else Statements

      getline(stream, inputWord, ' '); // Get Subcommand

      while (moreInput)
      {
        moreInput = false;

        if (!isSearchClientCmd)
        {
          if (inputWord.length() < 2)
          { // Check if the Subcommand has the Minimum String Length, which is 2
            isCmd = wrongSubCmd;
            break; // Break the for-loop
          }

          subCmd = inputWord[1];
          subCmdIndex = isCharOnArray(subCmd, subCmdsPtr, subCmdEnd); // Check if the Command is i the Array of subCommands
        }
        else if (isSearchClientCmd)
        {
          if (inputWord.length() < 3)
          { // Check if the Search Client Command has the Minimum String Length, which is 3
            isCmd = wrongSubCmd;
            break;
          }

          field = inputWord[2];
          fieldIndex = isCharOnArray(field, clientCmdsPtr, subCmdEnd); // Check if the Client Field is in the Array of Client Commands
        }

        if (inputWord[0] != '-' || subCmdIndex == -1 || fieldIndex == -1)
        { // Wrong Search Subcommand
          isCmd = (isSearchClientCmd) ? wrongSearchClientCmd : wrongSubCmd;
          break;
        }

        if (subCmdIndex == subCmdSortBy)
        { // Get Sort By Parameters
          while (getline(stream, inputWord, ' '))
          {
            if (inputWord[0] == '-')
            { // It's not a Sort By Parameter
              moreInput = true;
              break;
            }

            param = inputWord[0];
            paramIndex = isCharOnArray(param, sortByCmdsPtr, sortByEnd); // Check if the Command is in the Sort By Array

            if (paramIndex == -1)
            { // Wrong Sort By Command Parameter
              isCmd = wrongSortByParam;
              break;
            }
            userInput.sortBy[paramIndex / 2] = param; // It will Overwrite the Previous Sorting for this Parameter, if it was Specified
          }
        }
        else
        { // Get Field Parameters
          while (getline(stream, inputWord, ' '))
          {
            if (!isSearchClientCmd)
            {
              if (isViewMoviesCmd && inputWord[0] == '-' || !isViewMoviesCmd && inputWord[0] != '-')
              { // It's not a Field Command or a Parameter
                moreInput = true;
                break;
              }

              param = (isViewMoviesCmd) ? inputWord[0] : inputWord[1];
              paramIndex = isCharOnArray(param, fieldsCmdsPtr, fieldEnd); // Check if the Command is in the Field Parameters Array

              if (paramIndex == -1 || (!isViewMoviesCmd && paramIndex == fieldAll))
              { // Wrong Field Parameter or Field Command
                isCmd = (isViewMoviesCmd) ? wrongFieldParam : wrongField;
                break;
              }
            }

            if (isViewMoviesCmd)
            {
              userInput.fields[paramIndex] = true;
            }
            else
            {
              int paramCounter = userInput.fieldParamsCounter[subCmdIndex]; // Counter
              while (paramCounter < maxParamPerSubCmd && getline(stream, inputWord, ' '))
              { // Iterate while there's a Parameter and can be Added to the Array
                if (inputWord[0] == '"')
                { // If it Starts with Double Quote, it's a Long Sentence (a Parameter with Multiple Words)
                  if (!getline(stream, inputLong, '"'))
                  {
                    isCmd = (isSearchClientCmd) ? wrongSearchClientCmd : wrongFilterCmd; // Incomplete Long Parameter
                    break;
                  }

                  inputWord.insert(inputWord.length(), 1, ' ');                 // Insert Whitespace at the End
                  inputLong.insert(0, inputWord.substr(1, inputWord.length())); // Insert the Parameter at the Beginning of the String, without the Double Quote

                  userInput.fieldParams[paramIndex][paramCounter] = inputLong; // Add Sentence Search Data Parameter
                }
                else
                {                              // Get Word Input
                  if (inputWord.length() == 0) // To Prevent Adding Whitespaces as Parameters
                    continue;

                  if (inputWord[0] == '-')
                  {
                    moreInput = true;
                    break; // If it's a Command break this For-loop
                  }

                  userInput.fieldParams[paramIndex][paramCounter] = inputWord; // Add Word Search Data Parameter
                }
                paramCounter++; // Parameter Counter
              }
              userInput.fieldParamsCounter[paramIndex] = paramCounter; // Update Parameter Counter
            }
          }
        }

        if (isCmd == wrongViewCmd || isCmd == wrongFilterCmd)
          break; // Exit this While-loop

        while (!moreInput && getline(stream, inputWord, ' '))
        {
          if (inputWord[0] == '-')
            moreInput = true; // First Parameter Must be a Command
        }
        isCmd = validCmd;
      }

      switch (cmdIndex)
      {
      case cmdViewMovies:
        viewMovies(userInput.fields, fieldEnd, userInput.sortBy, sortByEnd);
        break;
      case cmdFilterMovies:
        filterMovies(userInput.ptrFieldParams, fieldEnd, maxParamPerSubCmd, userInput.sortBy, sortByEnd);
        break;
      case cmdSearchClient:
        searchClient(userInput.ptrClients, clientEnd, maxParamPerSubCmd);
        break;
      default:
        break;
      }
    }

    if (isCmd != validCmd)
    { // Chekc if the Command is Valid
      if (timesExec > 1 || (timesExec == 0 && argc == 1))
        cout << '\n'; // Print a New Line
      continue;
    }

    switch (cmdIndex)
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
      fieldParameters();
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
      howToUseFilterMovies;
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
    default: // Help Message
      timesExec++;
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
       << tab1 << "[-" << cmdsPtr[cmdAddMovie] << "] Add Movie\n"
       << tab1 << "[-" << cmdsPtr[cmdRentMovie] << "] Rent Movie\n"
       << "Database Search Commands:\n"
       << tab1 << "[-" << cmdsPtr[cmdMovieStatus] << "] Movie Status\n"
       << tab1 << "[-" << cmdsPtr[cmdViewMovies] << "] View Movies\n"
       << tab1 << "[-" << cmdsPtr[cmdFilterMovies] << "] Filter Movies\n"
       << tab1 << "[-" << cmdsPtr[cmdSearchClient] << "] Search Client\n"
       << "Command Parameters:\n"
       << tab1 << "[-" << cmdsPtr[cmdFieldParameters] << "] Movie Field Parameters\n"
       << tab1 << "[-" << cmdsPtr[cmdSortByParameters] << "] Sort By Parameters\n"
       << tab1 << "[-" << cmdsPtr[cmdClientParameters] << "] Search Client Parameters\n"
       << "How-To:\n"
       << tab1 << "[-" << cmdsPtr[cmdHowToUseViewMovies] << "] How to Use the View Movie Command\n"
       << tab1 << "[-" << cmdsPtr[cmdHowToUseFilterMovies] << "] How to Use the Filter Movie Command\n"
       << tab1 << "[-" << cmdsPtr[cmdHowToUseSearchClient] << "] How to Use the Search Client Command\n"
       << "Other Commands:\n"
       << tab1 << "[-" << cmdsPtr[cmdHelp] << "] Help\n"
       << tab1 << "[-" << cmdsPtr[cmdExit] << "] Exit\n"
       << "Admin Privileges:\n"
       << tab1 << "[-" << cmdsPtr[cmdAddClient] << "] Add Client\n";
}

// Function to Assign 2D Array to 1D Pointer, and Reset the Counters
void initParamArray(string **ptrArray, string array[][maxParamPerSubCmd], int arrayCounter[], int n)
{
  for (int i = 0; i < n; i++)
  {
    ptrArray[i] = array[i];
    arrayCounter[i] = 0;
  }
}
