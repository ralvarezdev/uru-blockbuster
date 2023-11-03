#include <string>
#include <iostream>
#include <iomanip>
#include "data.h"
#include "ansiEsc.h"
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
char cmdsChar[cmdEnd] = {'a', 'r', 's', 'v', 'f', 'c', 'F', 'S', 'C', 'u', 'U', 'v', 'A', 'h', 'e'};
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
const int paramPerLine = 3;                               // Number of Parameters Printed by Line
const int maxSpacing = 4;                                 // If the Maximum Number Characters is Reached this is the Spacing between Paratemeters
const int nCharTitle = 20;                                // Number of Characters of the Parameter Title Printed
const int nCharParam = (nWp - nCharTitle) / paramPerLine; // Number of Maximum Characters per Parameter

// --- Global Variables
string fieldCmdsStr[fieldEnd] = {"Director", "Duration", "Genre", "Id", "Price", "Release"};
string clientCmdsStr[clientEnd] = {"Id", "Name", "Account Number"};
string sortByCmdsStr[sortByEnd] = {"Duration", "Id", "Price", "Release Date", "Title"};

// --- Function Prototypes
int isCharOnArray(char character, char array[], int n);
void addMovie();
void rentMovie();
void movieStatus();
void viewMovies(bool fields[], int m, char sortBy[], int n);
void filterMovies(string **fieldParams, int l, int m, char *sortBy, int n);
void searchClient(string **clientParams, int maxParams);
void fields();
void sortByParameters();
void clientParameters();
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

  // Field and Sort By String Array Length
  m--; // We don't Count the '.' Command
  n = getSortByStr(sortBy, sortByStr, n);

  cout << clear;
  printTitle("Movie Fields", applyBgColor, applyFgColor, false);
  printArray(fieldsStr, m, "Fields");
  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  printArray(sortByStr, n, "Sort By");
  pressEnterToContinue("Press ENTER to Continue", false);
}

void filterMovies(string **fieldParams, int l, int m, char *sortBy, int n)
{
  string sortByStr[n];

  n = getSortByStr(sortBy, sortByStr, n);

  cout << clear;
  printTitle("Movie Fields Parameters", applyBgColor, applyFgColor, false);
  print2DArray(fieldParams, l, m, fieldCmdsStr);
  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  printArray(sortByStr, n, "Sort By");
  pressEnterToContinue("Press ENTER to Continue", false);
}

void searchClient(string **clientParams, int m, int n)
{
  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
      cout << clientParams[i][j] << '\n';

  cout << clear;
  printTitle("Search Client Parameters", applyBgColor, applyFgColor, false);
  print2DArray(clientParams, m, n, clientCmdsStr);
  pressEnterToContinue("Press ENTER to Continue", false);
}

void fields()
{
  cout << "F";
  /*
  cout << clear; // Clear Terminal
  printTitle("Search Command Possible Parameters", applyBgColor, applyFgColor);
  cout << tab1 << "[--" << searchCmdsChar[searchClientId] << "] Client ID\n"
       << tab1 << "[--" << searchCmdsChar[searchClientName] << "] Client Name\n"
       << tab1 << "[--" << searchCmdsChar[searchAccountNumber] << "] Account Number\n";
  pressEnterToContinue();
  */
}

void sortByParameters()
{
  /*
  'd', 'D', 'i', 'I', 'p', 'P', 'r', 'R', 't', 'T';

  cout << clear; // Clear Terminal

  // Valid Subcommands for Search Command
  printTitle("Sort By Ascending Order", applyBgColor, applyFgColor, false);
  cout << tab1 << "[--" << sortByCmdsChar[searchClientId] << "] Client ID\n"
       << tab1 << "[--" << sortByCmdsChar[searchClientName] << "] Client Name\n"
       << tab1 << "[--" << sortByCmdsChar[searchAccountNumber] << "] Account Number\n\n";

  // Valid Parameters for Search Command
  printTitle("Sort By Descending Order", applyBgColor, applyFgColor, false);
  cout << tab1 << "[word] Word to Search on Data\n"
       << tab1 << '[' << '"' << "sentence to search" << '"' << "] Sentence to Search on Data\n\n";

  pressEnterToContinue("Press ENTER to Continue", false);
  */
  cout << "S";
}

void clientParameters()
{
  cout << "C";
}

void howToUseViewMovies()
{
  cout << "u";
}

void howToUseFilterMovies()
{
  cout << "U";
}

void howToUseSearchClient()
{
  cout << 'v';
}

void addClient()
{
  cout << "A";
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