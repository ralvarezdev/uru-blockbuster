#include <iostream>
#include "ansiEsc.h"
#include "..\namespaces.h"

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
      wrongCommand(wrongBooleanAnswer);
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
  case wrongSearchClientCmd:
  case wrongSortByParam:
  case wrongFieldParam:
  case wrongField:
    if (cmdStatus == wrongFilterMoviesCmd || cmdStatus == wrongViewMoviesCmd || cmdStatus == wrongSearchClientCmd)
      message.append("Wrong Command");
    else if (cmdStatus == wrongSortByParam || cmdStatus == wrongFieldParam || cmdStatus == wrongField)
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
  }
  pressEnterToCont(message, true);
}

// Function to Check if the Client Data Typed by the User is Correct
void wrongClientData(invalidClient wrongData)
{
  string message = "Invalid: Client ";

  switch (wrongData)
  {
  case invalidClientId:
    message.append("ID");
    break;
  case clientExists:
    message = "Error: Client Exists";
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