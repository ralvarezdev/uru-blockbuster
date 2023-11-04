#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <typeinfo>
#include "ansiEsc.h"
#include "data.h"
#include "input.h"

using namespace std;

// --- Structs

// - Client
struct Client
{
  int id;
  string name;
  int accountNumber;
};

// --- Extern Variables and Constants Definition
char cmdsChar[cmdEnd] = {'a', 'r', 's', 'v', 'f', 'c', 'F', 'S', 'C', 'x', 'y', 'z', 'A', 'h', 'e'};
char subCmdsChar[subCmdEnd] = {'f', 's'};
char fieldCmdsChar[fieldEnd] = {'d', 'D', 'g', 'i', 'p', 'r', '.'};
char clientCmdsChar[clientEnd] = {'i', 'n', 'a'};
char sortByCmdsChar[sortByEnd] = {'d', 'D', 'i', 'I', 'p', 'P', 'r', 'R', 't', 'T'}; // Lowercase for Ascending Order, Uppercase for Descending

// --- Extern Variables and Constants Assignment
char *cmdsPtr = cmdsChar;
char *subCmdsPtr = subCmdsChar;
char *fieldCmdsPtr = fieldCmdsChar;
char *clientCmdsPtr = clientCmdsChar;
char *sortByCmdsPtr = sortByCmdsChar;

// --- Global Constants
const int paramPerLine = 3;                                 // Number of Parameters Printed by Line
const int maxSpacing = 4;                                   // If the Maximum Number Characters is Reached this is the Spacing between Paratemeters
const int nCharTitle = 30;                                  // Number of Characters of the Parameter Title Printed
const int nCharParam = (nChar - nCharTitle) / paramPerLine; // Number of Maximum Characters per Parameter

// --- Global Variables
string fieldCmdsStr[fieldEnd] = {"Director", "Duration", "Genre", "Id", "Price", "Release"};
string clientCmdsStr[clientEnd] = {"Id", "Name", "Account Number"};
string sortByCmdsStr[sortByEnd] = {"Duration", "Id", "Price", "Release Date", "Title"};

// --- Templates

// Template to Return a String that's Surrounded by Brackets
template <typename T>
string addBrackets(T message)
{
  string addedBrackets;

  if (typeid(T) == typeid(const char)) // Checks if it's a Char
    addedBrackets += message;          // Works fine it Message is a Char
  else
    addedBrackets = message; // Message is of type String´

  addedBrackets.insert(0, 1, '[');

  return addedBrackets.insert(addedBrackets.length(), 1, ']');
}

// --- Function Prototypes
int isCharOnArray(char character, char array[], int n);
void addMovie();
void rentMovie();
void movieStatus();
void viewMovies(bool fields[], int m, char sortBy[], int n);
void filterMovies(string **fieldParams, int l, int m, char *sortBy, int n);
void searchClient(string **clientParams, int m, int n);
void validParameters(int nCharTitle);
void fields();
void sortByParameters();
void clientParameters();
void printExamples(string cmds[], string explanations[], int n);
void howToUseViewMovies();
void howToUseFilterMovies();
void howToUseSearchClient();
void addClient();
void printArray(string *params, int m, string paramTitle);
void print2DArray(string **params, int m, int n, string paramsTitle[]);
int getSortByStr(char sortBy[], string sortByStr[], int n);

// --- Functions

// Function to check if the Character is on Char Array
int isCharOnArray(char character, char array[], int n)
{
  for (int i = 0; i < n; i++)
  {
    if (array[i] == character)
      return i;
  }
  return -1;
}

// TO DEVELOP

void addMovie()
{
  cout << "a";
}

void rentMovie()
{
  cout << "r";
}

void movieStatus()
{
  cout << "s";
}

// Function to View Movies
void viewMovies(bool fields[], int m, char sortBy[], int n)
{
  string fieldsStr[m - 1], sortByStr[n], applied;

  if (fields[fieldAll] == true)
    for (int i = 0; i < m - 1; i++)
      fields[i] = true;

  for (int i = 0; i < m - 1; i++)
  {
    applied = fields[i] ? "[Y] " : "[N] ";
    fieldsStr[i] = applied.append(fieldCmdsStr[i]); // Data to Print in the Field Parameters Row
  }

  m--;                                    // Get Field Array Length. We don't Count the '.' Command
  n = getSortByStr(sortBy, sortByStr, n); // Get Sort By Array Length

  cout << clear;
  printTitle("Movie Fields", applyBgColor, applyFgColor, false);
  printArray(fieldsStr, m, "Fields");
  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  printArray(sortByStr, n, "Sort By");
  pressEnterToContinue("Press ENTER to Continue", false);

  // Parameter with Highest Priority: None
}

// Function to Filter Movies
void filterMovies(string **fieldParams, int l, int m, char *sortBy, int n)
{
  string sortByStr[n];

  n = getSortByStr(sortBy, sortByStr, n);

  cout << clear;
  printTitle("Movie Fields Parameters", applyBgColor, applyFgColor, false);
  print2DArray(fieldParams, l, m, fieldCmdsStr);
  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  printArray(sortByStr, n, "Sort By");

  // Parameters with Highest Priority: Id

  pressEnterToContinue("Press ENTER to Continue", false);
}

// Function to Search Client
void searchClient(string **clientParams, int m, int n)
{
  cout << clear;
  printTitle("Search Client Parameters", applyBgColor, applyFgColor, false);
  print2DArray(clientParams, m, n, clientCmdsStr);
  pressEnterToContinue("Press ENTER to Continue", false);

  // Parameters with Highest Priority: Id, Account Number
}

// Function to Print the Two Types of Parameters (Used in Filter Movies and Search Clients Command)
void validParameters(int nCharTitle)
{
  string temp = "`sentence to search`";
  replace(temp.begin(), temp.end(), '`', '"'); // Replace Backsticks with Double Quotes

  cout << tab1 << setw(nCharTitle) << setfill(' ') << addBrackets<string>("word") << "Word to Search\n"
       << tab1 << setw(nCharTitle) << setfill(' ') << addBrackets<string>(temp) << "Sentence to Search\n\n";
}

// Function to Print Field as a Parameter and as a Command
void fields()
{
  string temp;

  cout << clear; // Clear Terminal
  printTitle("Field as a Parameter (for View Movies)", applyBgColor, applyFgColor, false);
  for (int i = 0; i < fieldEnd - 1; i++)
    cout << tab1 << setw(nCharTitle) << setfill(' ') << addBrackets(fieldCmdsChar[i]) << fieldCmdsStr[i] << '\n';
  cout << '\n';

  printTitle("Field as a Command (for Filter Movies)", applyBgColor, applyFgColor, false);
  for (int i = 0; i < fieldEnd - 1; i++)
  {
    temp = addBrackets(fieldCmdsChar[i]).append(" [param...]");
    cout << tab1 << setw(nCharTitle) << setfill(' ') << temp << "Parameters for Movie's " << fieldCmdsStr[i] << '\n';
  }
  cout << '\n';

  printTitle("Valid Field Parameters (for Filter Movies)", applyBgColor, applyFgColor, false);
  validParameters(nCharTitle);

  pressEnterToContinue("Press ENTER to Continue", false);
}

// Function to Print Sort By Parameters
void sortByParameters()
{
  string ascending, descending;

  cout << clear; // Clear Terminal
  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  for (int i = 0; i < sortByEnd / 2; i++)
  {
    ascending = addBrackets(sortByCmdsChar[i * 2]).append(" Ascending");
    descending = addBrackets(sortByCmdsChar[i * 2 + 1]).append(" Descending");

    cout << tab1 << setw(nCharTitle) << setfill(' ') << sortByCmdsStr[i]
         << setw(nCharTitle) << setfill(' ') << ascending
         << setw(nCharTitle) << setfill(' ') << descending << '\n';
  }
  cout << '\n';

  pressEnterToContinue("Press ENTER to Continue", false);
}

// Function to Print Search Client as a Command
void clientParameters()
{
  string temp;

  cout << clear;
  printTitle("Search Client as a Command", applyBgColor, applyFgColor, false);
  for (int i = 0; i < clientEnd; i++)
  {
    temp = addBrackets(clientCmdsChar[i]).append(" [param...]");
    cout << tab1 << setw(nCharTitle) << setfill(' ') << temp << "Parameters for Client's " << clientCmdsStr[i] << '\n';
  }
  cout << '\n';

  printTitle("Valid Client Parameters", applyBgColor, applyFgColor, false);
  validParameters(nCharTitle);

  pressEnterToContinue("Press ENTER to Continue", false);
}

// Function that Prints Examples of View Movies Command
void howToUseViewMovies()
{
  const int nCmds = 2; // Number of Code Examples
  string cmds[nCmds] = {"v -f . -s i",
                        "v -f g p r -s I p"};

  string explanations[nCmds] = {"View All Movie Fields. Sort them by Id in Ascending Order",
                                "View Genre, Price and Release Date Movie Field. Sort them by Id in Descending Order and by Price in Ascending Order"};

  printExamples(cmds, explanations, nCmds);

  cout << '\n';
  pressEnterToContinue("Press ENTER to Continue", false);
}

void howToUseFilterMovies()
{
  const int nCmds = 4; // Number of Code Examples
  string cmds[nCmds] = {"f -f --g Horror Fiction -s i",
                        "f -f --p 20 -s P",
                        "f -f --i 101 --g Thriller Mistery -s R",
                        "f -f --d `David Quiroz` -s t"};

  string explanations[nCmds] = {
      "Search for Movies that their Genre is Horror and Fiction. Sort them by Id in Ascending Order",
      "Search for Movies that their Price is 20 or less. Sort them by Price in Descending Order",
      "Search for Movie with Id 101. Because the Id is Unique, it'll Ignore the Parameters Left. Sort them by Release Date in Descending Order",
      "Search for Movies that their Director is David Quiroz. Sort them by Title in Ascending Order"};

  printExamples(cmds, explanations, nCmds);

  cout << '\n';
  pressEnterToContinue("Press ENTER to Continue", false);
}

void howToUseSearchClient()
{
  const int nCmds = 3; // Number of Code Examples
  string cmds[nCmds] = {"c --i 101 --n Ramon",
                        "c --n `Andres Avila` `Ronald Lopez` Ivana Grecia",
                        "c --n  Roberto --a 123456789"};

  string explanations[nCmds] = {
      "Search for Client with Id 101. Because this Field is Unique, it ignores the Client Name",
      "Search for Clients Named Andres Avila, Ronald Lopez, Ivana and Grecia",
      "Search for Client with Account Number 123456789. Because the Account Number is also Unique, it ignores the Client Name"};

  printExamples(cmds, explanations, nCmds);

  cout << '\n';
  pressEnterToContinue("Press ENTER to Continue", false);
}

void addClient()
{
  cout << "A";
}

// Function to Print the Commands and its Explanations while Taking Care of nChar
void printExamples(string cmds[], string explanations[], int n)
{
  const int nCharLine = nChar - tab1.length(); // Number of Characters for Each Line

  int printedChar, messageLength; // Number of Characters Printed in the Current Line and the Length of Message String
  string message, lastIter;       // Temp Data

  cout << clear;                                             // Clear Terminal
  printTitle("Examples", applyBgColor, applyFgColor, false); // Examples of the Usage of the Search Command
  for (int i = 0; i < n; i++)
  {
    printedChar = 0;

    for (int j = 0; j < cmds[i].length(); j += nCharLine)
    {
      message = cmds[i].substr(j, nCharLine + j);
      replace(message.begin(), message.end(), '`', '"'); // Replace Backsticks with Double Quotes
      cout << '\n'
           << tab1 << message << '\n';
    }
    message = ""; // Clear String

    cout << tab1 << string(nCharTitle, '-') << '\n';

    stringstream stream(explanations[i]); // To Print the Message with New Line each time it Reaches nCharLine
    while (getline(stream, message, ' '))
    {
      messageLength = message.length();

      if (printedChar + messageLength >= nCharLine)
      { // Exceeds the Maximum Number of Characters per Line
        cout << '\n'
             << tab1 << message;
        printedChar = messageLength;
        continue;
      }

      if (printedChar != 0)
      {
        cout << ' ' << message;
        printedChar++;
      }
      else
        cout << tab1 << message;

      printedChar += messageLength;
    }
    cout << '\n';
  }
}

// Function to Print a 1D Array
void printArray(string *params, int n, string paramTitle)
{
  string param;

  cout << setw(nCharTitle) << setfill(' ') << left << paramTitle;
  for (int i = 0; i < n && params[i].length() != 0; i++)
  {
    param = params[i];

    if (i % paramPerLine == 0 && i != 0)
      cout << '\n' // Break the Line, and Add Some Indentation if there's more than 3 Parameters on the Current Line
           << string(nCharTitle, ' ');

    if (param.length() <= nCharParam - maxSpacing)
    {
      cout << setw(nCharParam) << setfill(' ') << left << param;
      continue;
    }

    param = param.substr(0, nCharParam - maxSpacing); // Substract from String
    param.append("...");                              // Append to the String
    cout << setw(nCharParam) << setfill(' ') << left << param;
  }
  cout << "\n\n";
}

// Function to Print a 2D Data Table
void print2DArray(string **params, int m, int n, string paramsTitle[])
{
  for (int i = 0; i < m; i++)
  {
    if (params[i][0].length() != 0)
      printArray(params[i], n, paramsTitle[i]);
  }
}

// Function to Get a String Array from a Char Array of the Sort By Commands that will be Applied to the Movies
int getSortByStr(char sortBy[], string sortByStr[], int n)
{
  bool nullParam;
  int nParams = 0;
  string order, temp;

  for (int j = 0; j < n; j++)
  {
    nullParam = false;

    if (isupper(sortBy[j]))
      order = "[D] ";
    else if (islower(sortBy[j]))
      order = "[A] ";
    else
      nullParam = true;

    if (!nullParam)
    {
      sortByStr[nParams] = order.append(sortByCmdsStr[j]); // Data to Print in the Sort By Parameters Row
      nParams++;
    }
  }
  return nParams;
}