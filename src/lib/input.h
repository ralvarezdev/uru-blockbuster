#include <string>

using namespace std;

// --- Extern Variables and Constants Declaration
extern const bool applyBgColor, applyFgColor;

#ifndef INPUT_H
#define INPUT_H

// --- Enums

// - Command Status if it's Valid or not
enum cmdStatus
{
  validCmd,
  noCmd,
  wrongMainCmd,
  wrongSubCmd,
  wrongViewMoviesCmd,
  wrongFilterMoviesCmd,
  wrongSearchClientCmd,
  wrongField,
  wrongFieldParam,
  wrongSortByParam
};

// --- Functions
void ignoreInput();
void wrongCommand(int cmdStatus);
void pressEnterToContinue(string message, bool warning);

#endif