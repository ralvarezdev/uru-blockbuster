#include <string>
#include "..\namespaces.h"

using namespace std;
using namespace movies;
using namespace clients;

#ifndef OUTPUT_H
#define OUTPUT_H

// --- Functions
void printExamples(string cmds[], string explanations[], int n);
void printArray(string *params, int m, string paramTitle);
void print2DArray(string **params, int m, int n, string paramsTitle[]);
void printMovies(Movie movies[], int m, bool *fields, int n);

#endif