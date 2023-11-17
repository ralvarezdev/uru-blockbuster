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
void printDate(int date[], bool rightJustified);
void printMovies(Movie movies[], int m, bool *fields);
void printClientInfo(Client client, bool censoreInfo);
void printClients(Client clients[], int m, bool *fields);

#endif