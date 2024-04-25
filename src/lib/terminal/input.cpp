#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>

// #define NDEBUG
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
    if (cmdStatus == wrongViewMoviesCmd)
      message.append("Wrong View Movies Command");
    else
      message.append("Wrong Filter Movies Command");

    message.append(". Press ENTER to Display Command Examples");
    break;
  case wrongViewClientsCmd:
  case wrongSearchClientsCmd:
    if (cmdStatus == wrongViewClientsCmd)
      message.append("Wrong View Clients Command");
    else
      message.append("Wrong Search Clients Command");

    message.append(". Press ENTER to Display Command Examples");
    break;
  case wrongMovieFieldParam:
  case wrongMovieField:
  case wrongClientField:
  case wrongClientFieldParam:
    if (cmdStatus == wrongMovieFieldParam || cmdStatus == wrongMovieField)
      message.append("Wrong Movie Field");
    else
      message.append("Wrong Client Field");

    message.append(". Press ENTER to Display Valid Field Parameters");
    break;
  case wrongSortByParam:
    message.append("Wrong Sort By Parameter. Press ENTER to Display Sort By Parameters Message");
    break;
  }
  pressEnterToCont(message, true);
}

// Function to Check if the Movie Data Typed by the User is Correct
void wrongMovieData(invalidMovie wrongData)
{
  string message = "Invalid Movie ";

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
  string message = "Invalid Client ";

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
  string _temp;

  printTitle(message, applyBgColor, applyFgColor, warning);
  getline(cin, _temp);
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
      cout << message << ": ";
      getline(cin, temp);
      amount = stoi(temp);

      // Check if the Number is Out of Range
      if (amount < low || amount > high)
        throw out_of_range("Amount Out of Range");

      return amount;
    }
    catch (const invalid_argument &e)
    {
      pressEnterToCont("ERROR: Invalid Argument. It Must be an Integer", true);
    }
    catch (const out_of_range &e)
    {
      ostringstream stream;

      stream << "The Number is Out of that Range"
             << " [" << low << '-' << high << "]";

      pressEnterToCont(stream.str(), true);
    }
}

// Function to Ask for a Double Input
double getDouble(string message, double low, double high, int precision)
{
  string temp;
  double amount;

  while (true)
    try // Get Double
    {
      cout << message << ": ";
      getline(cin, temp);
      amount = stod(temp);

      // Check if the Number is Out of Range
      if (amount < low || amount > high)
        throw out_of_range("Amount Out of Range");

      return amount;
    }
    catch (const invalid_argument &e)
    {
      pressEnterToCont("ERROR: Invalid Argument. It Must be a Double", true);
    }
    catch (const out_of_range &e)
    {
      ostringstream stream;

      stream << "The Number is Out of that Range"
             << " [" << fixed << setprecision(precision) << low << '-' << high << "]";

      pressEnterToCont(stream.str(), true);
    }
}

// Function to Get a Lowercase String
string getLower(string word)
{
  string wordToLower;

  for (int i = 0; i < word.length(); i++)
    wordToLower += tolower(word[i]); // Append Character in Lowercase

  return wordToLower;
}