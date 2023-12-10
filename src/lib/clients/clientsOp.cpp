#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// #define NDEBUG
#include <assert.h>

#include "clientsOp.h"
#include "../namespaces.h"
#include "../datatables/output.h"
#include "../terminal/ansiEsc.h"
#include "../terminal/input.h"

using namespace std;
using namespace clients;
using namespace files;
using namespace terminal;

// --- Extern Variables Declaration
extern bool clientValidFieldsToFilter[];

// --- Function Prototypes
clientStatus getClientId(Clients *clients, int *id, int *index, string message);
void getClients(Clients *clients);
void addClientToFile(Clients *clients);
void createClientWithId(Clients *clients, Client newClient, int *index);
void filterClientsData(Clients *clients, string **params, bool fields[], int sortBy[]);
void sortClients(Clients *clients, int sortBy[], int n);
void clientsMergeSort(Clients *client, int sortByIndex);
void clientsMerge(Clients *clients, Client sorted[], int low, int mid, int high, int sortByIndex);

// Function to Get an Array of Clients from clients.csv
void getClients(Clients *clients)
{ // Should be in clients.cpp
  int count = 0, nClients = 0;
  string line, word;

  ifstream clientsCSV(clientsFilename, ios::out | ios::binary);

  if (!clientsCSV.is_open())
  { // Couldn't Access to clientsCSV
    clientsCSV.close();
    pressEnterToCont("Error: File Not Found. Press ENTER to go Back to Main Menu", true);
    return;
  }

  while (true)
  { // Get Clients
    Client clientRead;

    clientsCSV.read((char *)&clientRead, sizeof(Client)); // Read Structure
    if (clientsCSV.eof())                                 // Reached End of File
      break;

#ifndef NDEBUG
    nClients = (*clients).getNumberClients();
#endif

    (*clients).pushBack(clientRead);
    assert(nClients == (*clients).getNumberClients() - 1); // Check if the Number of Clients Gets Increased
  }

  clientsCSV.close();
}

// Function to Add Client
void addClientToFile(Clients *clients)
{
  Client newClient = Client();

  int check, id, index = -1;

  check = getClientId(clients, &id, &index, "ID"); // Get Client Id

  if (check != clientNotFound) // The Id has been Added to that File
  {
    assert(id > 0 && index >= 0); // Check Client Id and Index
    wrongClientData(clientExists);
    return; // End this Function
  }

  assert(id > 0 && index == -1); // Check Client Id and Index
  newClient.id = id;             // Assign Client ID

  createClientWithId(clients, newClient, &index);
}

// Function to Create Client after Assigning an Id to the Client
void createClientWithId(Clients *clients, Client newClient, int *index)
{
  int nClients = (*clients).getNumberClients();
  string temp;

  cout << "Name: "; // Get Client Name
  getline(cin, temp);
  for (int i = 0; i < nFieldChars && i < temp.length(); i++)
    newClient.name[i] = temp[i];

  newClient.phoneNumber = getDouble("Phone Number", 0, maxPhoneNumber, 0); // Get Client Phone Number

  // Get Client Account Number. Previously, Sorted by Id in Ascending Order (in getClientId Function)
  if (nClients > 0)
    newClient.account = (*clients).getClient(nClients - 1).account + 1;
  else
    newClient.account = 0;

  (*clients).pushBack(newClient);

  ofstream outfile(clientsFilename, ios::app | ios::binary); // Write to File

  outfile.write((char *)&newClient, sizeof(Client));

  outfile.close();
  pressEnterToCont("Client Added Successfully!", false);
}

// Function that Returns Clients Indexes that Matched with the Parameters
void filterClientsData(Clients *clients, string **params, bool fields[], int sortBy[])
{
  clientStatus clientStatus;
  double phoneNumber;
  int i, id, account, index = 0, counter = 0, nClientsRead = (*clients).getNumberClients();
  string nameLower;

  assert(nClientsRead >= 0); // Check if the Number of Clients is Greater or Equal to 0

  bool *filteredIndexes = new bool[nClientsRead];               // Allocate Memory
  fill(filteredIndexes, filteredIndexes + nClientsRead, false); // Fill Array with False Values

  for (int field = 0; field < clientFieldEnd - 1; field++)
  {
    if (!clientValidFieldsToFilter[field]) // Check if the Function can Filter that Field, and if there are Parameters
      continue;

    for (int param = 0; param < maxParamPerSubCmd && params[field][param] != "null"; param++)
      if (field == clientFieldId || field == clientFieldAccountNumber)
      {
        if (field == clientFieldAccountNumber)
        {
          account = stoi(params[field][param]);
          clientStatus = checkClient(clients, account, clientFieldAccountNumber, &index); // Binary Search
        }
        else if (field == clientFieldId)
        {
          id = stoi(params[field][param]);
          clientStatus = checkClient(clients, id, clientFieldId, &index); // Binary Search
        }

        if (clientStatus != clientNotFound && !filteredIndexes[index])
        {                                // Checks if the Client has Already being Filtered
          assert(index >= 0);            // Check the Client Index
          filteredIndexes[index] = true; // Store Index
          counter++;
        }
      }
      else if (field == clientFieldName)
      {
        nameLower = getLower(params[field][param]); // Get Client Name To Search for in Lowercase

        clientsMergeSort(clients, clientFieldId * 2); // Sort Clients by Id
        for (i = 0; i < nClientsRead; i++)
          if (!filteredIndexes[i] && getLower((*clients).getClient(i).name).find(nameLower) != string::npos)
          {                            // Checks if the Client Name in Lowercase Contains the Parameter that is being Searched by Linear Search
            filteredIndexes[i] = true; // Save Id
            counter++;
          }
      }
      else if (field == clientFieldPhoneNumber)
      {
        phoneNumber = stod(params[field][param]);

        clientsMergeSort(clients, clientFieldId * 2); // Sort Clients by Id
        for (i = 0; i < nClientsRead; i++)
          if (!filteredIndexes[i] && (*clients).getClient(i).phoneNumber == phoneNumber)
          {                            // Checks if the Client has the Phone Number that is being Searched by Linear Search
            filteredIndexes[i] = true; // Save Index
            counter++;
          }
      }
  }

  Clients filteredClients = Clients(counter);

  for (int i = 0; i < nClientsRead; i++)
    if (filteredIndexes[i])
      filteredClients.pushBack((*clients).getClient(i)); // Save Client that has been Filtered to Array

  sortClients(&filteredClients, sortBy, clientFieldEnd - 1); // Sort Clients
  printClients(&filteredClients, fields);                    // Print Clients

  string message = "Number of Coincidences: ";
  message.append(to_string(counter));

  if (counter == 0)
    cout << string(nChar, '-') << '\n';

  cout << '\n';
  printTitle(message, applyBgColor, applyFgColor, (counter == 0) ? true : false); // Print Number of Coincidences

  filteredClients.deallocate(); // Deallocate Memory
  delete[] filteredIndexes;
}

// Function to Sort Clients (Uses Merge Sort)
void sortClients(Clients *clients, int sortBy[], int n)
{
  for (int i = 0; i < n; i++)
    if (sortBy[i] != -1)
      clientsMergeSort(clients, sortBy[i]);
}

// - Merge Sort
// O(n*logn)
// Stable
void clientsMergeSort(Clients *clients, int sortByIndex)
{
  int pass, low, high, mid, i, n = (*clients).getNumberClients();
  assert(n >= 0); // Check that the Array Has Some Clients

  Client *sorted = new Client[n]; // Store the Array in the Heap Memory

  for (pass = 2; pass <= n; pass *= 2)
  {
    for (i = 0; i + pass - 1 < n; i += pass)
    {
      low = i;
      high = i + pass - 1;
      mid = (low + high) / 2;
      clientsMerge(clients, sorted, low, mid, high, sortByIndex);
    }

    if (n - i > pass / 2)
    {
      low = i;
      high = i + pass - 1;
      mid = (low + high) / 2;
      clientsMerge(clients, sorted, low, mid, n - 1, sortByIndex);
    }
  }

  if (pass / 2 < n)
    clientsMerge(clients, sorted, 0, pass / 2 - 1, n - 1, sortByIndex);

  delete[] sorted; // Deallocate Memory

  if (sortByIndex % 2 != 0) // True if it's an Odd Number, which is Used for Descending Order
    (*clients).reverse();   // Reverse Array
}

// Function to Merge Clients Subarrays
void clientsMerge(Clients *clients, Client sorted[], int low, int mid, int high, int sortByIndex)
{
  int i = low, j = mid + 1, k = low;
  assert(low <= high); // Check if value of low variable is less or equal to high

  switch (sortByIndex / 2)
  {
  case clientFieldAccountNumber:
    while (i <= mid && j <= high)
      sorted[k++] = (*clients).compare(&i, &j, clientFieldAccountNumber, 1);
    break;

  case clientFieldId:
    while (i <= mid && j <= high)
      sorted[k++] = (*clients).compare(&i, &j, clientFieldId, 1);
    break;

  case clientFieldName:
    while (i <= mid && j <= high)
      sorted[k++] = (*clients).compare(&i, &j, clientFieldName, 1);
    break;

  case clientFieldPhoneNumber:
    while (i <= mid && j <= high)
      sorted[k++] = (*clients).compare(&i, &j, clientFieldPhoneNumber, 1);
    break;
  }

  while (i <= mid)
    sorted[k++] = (*clients).getClient(i++);
  while (j <= high)
    sorted[k++] = (*clients).getClient(j++);

  for (i = low; i <= high; i++)
    (*clients).insertAt(i, sorted[i]);
}

// Function to Ask for Client Id
clientStatus getClientId(Clients *clients, int *id, int *index, string message)
{
  string temp;

  while (true) // Get Client ID
    try
    {
      cout << message << ": ";
      getline(cin, temp);
      *id = stoi(temp);

      if (*id <= 0)
        throw(-1); // ID Must be in the Range 0<ID<n

      return checkClient(clients, *id, clientFieldId, index);
    }
    catch (...)
    {
      wrongClientData(invalidClientId);
    }
}

/*
int CountRentedMoviesByClient(const string &clientName)
{
  int count = 0;

  ifstream inFile("movies.csv");
  if (!inFile)
  {
    cout << "Failed to open movies.csv" << endl;
    return count;
  }

  string line;
  while (getline(inFile, line))
  {
    stringstream ss(line);
    string token;
    getline(ss, token, ';');
    int movieId = stoi(token);
    getline(ss, token, ';');

    getline(ss, token, ';');
    string rentedTo = token;
    if (rentedTo == clientName)
    {
      count++;
    }
  }

  inFile.close();

  return count;
}

void SearchClientRentals()
{
  string clientName;
  cout << "Enter client name: ";
  cin.ignore();
  getline(cin, clientName);

  ifstream clientFile("clients.bin", ios::binary);
  if (clientFile)
  {
    string line;
    while (getline(clientFile, line))
    {
      stringstream ss(line);
      string name;
      getline(ss, name, ';');
      if (name == clientName)
      {
        int rentedMovies = CountRentedMoviesByClient(clientName);
        cout << "Client " << clientName << " has rented " << rentedMovies << " movies." << endl;
        clientFile.close();
        return;
      }
    }
    clientFile.close();
  }

  cout << "Client not found. Registering new client..." << endl;
  RegisterClient();
}
*/