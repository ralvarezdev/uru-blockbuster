#include <assert.h>

#include "../namespaces.h"

using namespace clients;

#ifndef CLIENTS_H
#define CLIENTS_H

// --- Functions
clientStatus getClientId(Clients *clients, int *id, int *index, string message);
void getClients(Clients *clients);
void addClientToFile(Clients *clients);
void createClientWithId(Clients *clients, Client newClient, int *index);
void filterClients(Clients *clients, string **params, bool fields[], int sortBy[]);
void sortClients(Clients *clients, int sortBy[], int n);

// --- Templates

// Template to Convert Float to String with a Set Precision of N Digits
template <typename T>
string toStringWithPrecision(T number, int precision)
{
  ostringstream stream;
  stream << fixed << setprecision(precision) << number;
  return stream.str();
}

// Function to Check if Client Unique Fields have been Ocuppied
template <typename T>
clientStatus checkClient(Clients *clients, T unique, cmdClientFields field, int *index)
{
  if (field != clientFieldId && field != clientFieldAccountNumber)
    return errorStatus;
  else
    clientsMergeSort(clients, field * 2); // Sort Clients by Id or by Account Number

  Client client;

  bool found = false;
  T value;
  int mid, start = 0, end = (*clients).getNumberClients() - 1;
  string line;

  assert(end > start); // Check if the Last Index of the Array is Greater than the First One

  while (start <= end)
  { // Binary Search
    mid = start + (end - start) / 2;

    try
    {
      client = (*clients).getClient(mid); // Get Client at the Given Index

      if (field == clientFieldId)
        value = client.id;
      else if (field == clientFieldAccountNumber)
        value = client.account;

      found = (value == unique);
    }
    catch (...)
    {
      if (field == clientFieldId)
        wrongClientData(invalidClientId);
      else if (field == clientFieldAccountNumber)
        wrongClientData(invalidClientAccountNumber);
    }

    if (found)
    {
      if (field == clientFieldId)
        *index = mid;
      else if (field == clientFieldAccountNumber)
        checkClient(clients, client.id, clientFieldId, index); // Get Index of Client with that Account Number when the Array is Sorted by Id

      return clientFound;
    }
    else if (unique > value)
      start = mid + 1;
    else
      end = mid - 1;
  }
  return clientNotFound;
}

#endif