#include <string>
#include <iomanip>

#include "../namespaces.h"

using namespace std;
using namespace clients;
using namespace commands;
using namespace movies;
using namespace terminal;

#ifndef OUTPUT_H
#define OUTPUT_H

// --- Functions
void printExamples(cmdExplanation examples[], int n);
void printDate(int date[], bool rightJustified);
void printMovies(Movies *movies, bool *fields);
void printClientInfo(Client client, bool censoreInfo);
void printClients(Clients *clients, bool *fields);

// --- Templates

// Template to Convert Float to String with a Set Precision of N Digits
template <typename T>
string toStringWithPrecision(T number, int precision)
{
  ostringstream stream;
  stream << fixed << setprecision(precision) << number;
  return stream.str();
}

// Template to Print a 1D Array
template <typename T, typename L>
void printArray(T *params, int n, L paramTitle)
{
  string param;

  assert(nCharTitle > 0 && nCharParam > 0 && maxSpacing > 0); // Check nCharTitle, nCharParam and maxSpacing

  cout << setw(nCharTitle) << setfill(' ') << left << paramTitle;
  for (int i = 0; i < n && string(params[i]) != "null"; i++)
  {
    param = params[i];

    if (i % paramPerLine == 0 && i != 0)
      cout << '\n' // Break the Line, and Add Some Indentation if there's more than N Parameters on the Current Line
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

// Template to Print a 2D Data Table
template <typename T, typename L>
void print2DArray(T **params, int m, int n, L paramsTitle[])
{
  for (int i = 0; i < m; i++)
    if (string(params[i][0]) != "null")
      printArray(params[i], n, paramsTitle[i]);
}

#endif