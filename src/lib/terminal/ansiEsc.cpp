#include <iomanip>
#include <iostream>
#include <string>

#include "../namespaces.h"

using namespace std;
using namespace terminal;

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

  cout << sgrCmd << setw(nChar) << setfill(' ') << left << message << reset << '\n';
}