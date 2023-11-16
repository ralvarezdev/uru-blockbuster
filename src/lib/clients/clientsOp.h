#include "..\namespaces.h"

using namespace clients;

#ifndef CLIENTS_H
#define CLIENTS_H

// --- Functions
int getClients(Client clients[]);
int getClientId(string message);
void addClientToFile(Client clients[], int *nClientsRead);
clientStatus checkClientStatusById(Client clients[], int nClientsRead, int id, int *index);
void createClientWithId(Client clients[], Client newClient, int *nClientsRead, int *index);

#endif