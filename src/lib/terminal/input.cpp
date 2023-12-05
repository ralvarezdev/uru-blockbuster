#include <iostream>
#include <assert.h>

#include "ansiEsc.h"
#include "../namespaces.h"

using namespace std;
using namespace clients;
using namespace commands;
using namespace movies;
using namespace terminal;

// --- Function Prototypes
// void ignoreInput();
bool booleanQuestion(string message);
void wrongCommand(cmdStatus cmdStatus);
void wrongMovieData(invalidMovie wrongData);
void wrongClientData(invalidClient wrongData);
void pressEnterToCont(string message, bool warning);
int getInteger(string message, int low, int high);
double getDouble(string message, double low, double high, int precision);
string getLower(string word);

// --- Functions

/*
// This prevents the program to crash if there's more input
void ignoreInput()
{
  while (getchar() != '\n')
    cin.clear();
}
*/

// Boolean Question
bool booleanQuestion(string message)
{
  string input;
  char c;

  while (true)
  {
    cout << "- " << message << " [y/N] ";
    getline(cin, input);

    c = tolower(input[0]);

    if (c == 'y')
    {
      cout << '\n';
      return true;
    }
    else if (c == 'n')
    {
      cout << '\n';
      return false;
    }
    else
    {
      assert(c != 'y' && c != 'n'); // c Must be Different from Y or N
      wrongCommand(wrongBooleanAnswer);
    }
  }
}

// Function to Check if the Command Typed by the User is Correct
void wrongCommand(cmdStatus cmdStatus)
{
  string message = "ERROR: ";

  switch (cmdStatus)
  {
  case wrongBooleanAnswer:
    message.append("It's a Yes/No Question. You must type 'y', 'Y' or 'n', 'N'");
    break;
  case noCmd:
  case wrongMainCmd:
  case wrongSubCmd:
    if (cmdStatus == noCmd)
      message.append("No Command");
    else
      message.append((cmdStatus == wrongMainCmd) ? "Wrong Main Command" : "Wrong Subcommand");
    message.append(". Press ENTER to Display Help Message");
    break;
  case wrongViewMoviesCmd:
  case wrongFilterMoviesCmd:
  case wrongViewClientsCmd: // TO ADD MESSAGE
  case wrongSearchClientsCmd:
  case wrongSortByParam:
  case wrongMovieFieldParam:
  case wrongMovieField:
  case wrongClientFieldParam:
    if (cmdStatus == wrongFilterMoviesCmd || cmdStatus == wrongViewMoviesCmd || cmdStatus == wrongSearchClientCmd)
      message.append("Wrong Command");
    else if (cmdStatus == wrongSortByParam || cmdStatus == wrongMovieFieldParam || cmdStatus == wrongMovieField || wrongClientFieldParam)
      message.append((cmdStatus == wrongSortByParam) ? "Wrong Sort By Parameter" : "Wrong Field");
    message.append(". Press ENTER to Display Search Data Parameters Message");
    break;
  }
  pressEnterToCont(message, true);
}

// Function to Check if the Movie Data Typed by the User is Correct
void wrongMovieData(invalidMovie wrongData)
{
  string message = "Invalid: Movie ";

  switch (wrongData)
  {
  case invalidMovieId:
    message.append("ID");
    break;
  case invalidMovieDuration:
    message.append("Duration");
    break;
  case invalidMovieGenre:
    message.append("Genre");
    break;
  case invalidMoviePrice:
    message.append("Price");
    break;
  case invalidMovieDate:
    message.append("Release Date");
    break;
  case movieExists:
    message = "Exists";
    break;
  }
  pressEnterToCont(message, true);
}

// Function to Check if the Client Data Typed by the User is Correct
void wrongClientData(invalidClient wrongData)
{
  string message = "Invalid: Client ";

  switch (wrongData)
  {
  case invalidClientAccountNumber:
    message.append("Account Number");
    break;
  case invalidClientId:
    message.append("ID");
    break;
  case invalidClientPhoneNumber:
    message.append("Phone Number");
    break;
  case clientExists:
    message = "Exists";
    break;
  }
  pressEnterToCont(message, true);
}

// Function to Stop the Program Flow while the User doesn't press the ENTER key
void pressEnterToCont(string message, bool warning)
{
  string temp;

  printTitle(message, applyBgColor, applyFgColor, warning);
  getline(cin, temp);
  // ignoreInput();
}

// Function to Ask for a Integer Input
int getInteger(string message, int low, int high)
{
  string temp;
  int amount;

  while (true)
    try // Get Integer
    {
      cout << message << " : ";
      getline(cin, temp);
      amount = stoi(temp);

      if (amount > low && amount <= high)
        return amount;
    }
    catch (...)
    {
      ostringstream stream;

      stream << "The Number is Out of that Range"
             << " [" << low << '-' << high << "]";

      pressEnterToCont(stream.str(), true);
    }
  assert(stoi(temp) == -1); // Function Must Not Get to this Assert
}

// Function to Ask for a Double Input
double getDouble(string message, double low, double high, int precision)
{
  string temp;
  double amount;

  while (true)
    try // Get Double
    {
      cout << message << " : ";
      getline(cin, temp);
      amount = stod(temp);

      if (amount > low && amount <= high)

        return amount;
    }
    catch (...)
    {
      ostringstream stream;

      stream << "The Number is Out of that Range"
             << " [" << fixed << setprecision(precision) << low << '-' << high << "]";

      pressEnterToCont(stream.str(), true);
    }
  assert(stoi(temp) == -1); // Function Must Not Get to this Assert
}

// Function to Get a Lowercase String
string getLower(string word)
{
  string wordToLower;

  for (int i = 0; i < word.length(); i++)
    wordToLower += tolower(word[i]); // Append Character in Lowercase

  return wordToLower;
}