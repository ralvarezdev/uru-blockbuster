#include "../namespaces.h"

using namespace clients;

#ifndef CLIENTS_H
#define CLIENTS_H

// --- Functions
int getClients(Client clients[]);
int getClientId(string message);
void addClientToFile(Client clients[], int *nClientsRead);
clientStatus checkClientStatusById(Client clients[], int nClientsRead, int id, int *index);
void createClientWithId(Client clients[], Client newClient, int *nClientsRead, int *index);
void filterClients(Client clients[], int nClientsRead, string **params, int counter[], bool fields[], int sortBy[]);
void sortClients(Client clients[], int m, int sortBy[], int n);

#endif