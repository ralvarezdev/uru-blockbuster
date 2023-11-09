#include <iostream>
#include "ansiEsc.h"
#include "input.h"

using namespace std;

// --- Function Prototypes
// void ignoreInput();
void wrongCommand(int cmdStatus);
void pressEnterToContinue(string message, bool warning);

// --- Functions

/*
// This prevents the program to crash if there's more input
void ignoreInput()
{
  while (getchar() != '\n')
    cin.clear();
}
*/

// Function to Check if the Command Entered by the User is Correct
void wrongCommand(int cmdStatus)
{
  string message = "ERROR: ", err;

  switch (cmdStatus)
  {
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
  pressEnterToContinue(message, true);
}

// Function to Stop the Program Flow while the User doesn't press the ENTER key
void pressEnterToContinue(string message, bool warning)
{
  string temp;

  printTitle(message, applyBgColor, applyFgColor, warning);
  getline(cin, temp);
  // ignoreInput();
}