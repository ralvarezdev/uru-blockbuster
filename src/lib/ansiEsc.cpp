#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

// --- Extern Variables and Constants Definition

// - ANSI Escape Code
extern const string reset = "\x1b[0m"; // Reset Terminal Text Format to Default
extern const string clear = "\033c";   // Clear Terminal

// - Format Constants
extern const string tab1 = string(3, ' '); // String with 3 whitespaces
extern const string tab2 = string(8, ' '); // String with 8 whitespaces
extern const bool applyBgColor = true;     // Add Customed Background Color
extern const bool applyFgColor = true;     // Add Customed Foreground Color

// - Size of the Title
extern const int nWp = 90; // If the string length is smaller, it'll be filled with whitespaces

// --- Color Command Created Using the ralvarezdev's Minigrep Version. URL:https://github.com/ralvarezdev/minigrep

// - Normal State
const string sgrBgCmd = "\x1b[48;2;24;100;171m"; // Command to Change the Text Background Color
const string sgrFgCmd = "\x1b[38;2;255;212;59m"; // Command to Change the Text Foreground Color

// - Warning (On Error)
const string sgrBgCmdError = "\x1b[48;2;224;49;49m";
const string sgrFgCmdError = "\x1b[38;2;255;212;59m";

// --- Function Prototypes
void printTitle(string message, bool applySgrBgCmd, bool applySgrFgCmd, bool warning);

// --- Functions

// Function to Print Title with Customed Colors
void printTitle(string message, bool applySgrBgCmd, bool applySgrFgCmd, bool warning)
{
  string sgrCmd; // String that Stores the SGR Command to Change the Text Format

  if (applySgrBgCmd)
    sgrCmd.append((!warning) ? sgrBgCmd : sgrBgCmdError);
  if (applySgrFgCmd)
    sgrCmd.append((!warning) ? sgrFgCmd : sgrFgCmdError);

  cout << sgrCmd << setw(nWp) << setfill(' ') << left << message << reset << '\n';
}