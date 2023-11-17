#include <string>
#include "..\namespaces.h"

using namespace std;
using namespace clients;
using namespace commands;
using namespace movies;

#ifndef INPUT_H
#define INPUT_H

// --- Functions
// void ignoreInput();
bool booleanQuestion(string message);
void wrongCommand(cmdStatus cmdStatus);
void wrongMovieData(invalidMovie wrongData);
void wrongClientData(invalidClient wrongData);
void pressEnterToCont(string message, bool warning);
string getLower(string word);

#endif