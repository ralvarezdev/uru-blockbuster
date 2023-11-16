#include <string>

using namespace std;

#ifndef ANSI_ESC_H
#define ANSI_ESC_H

// --- Functions
void printTitle(string message, bool applySgrBgCmd, bool applySgrFgCmd, bool warning);

#endif