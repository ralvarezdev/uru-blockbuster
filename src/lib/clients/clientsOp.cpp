#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "..\namespaces.h"
#include "..\terminal\input.h"
#include "..\datatables\output.h"

using namespace std;
using namespace clients;
using namespace files;

// --- Function Prototypes
int getClients(Client clients[]);
int getClientId(string message);
void addClientToFile(Client clients[], int *nClientsRead);
clientStatus checkClientStatusById(Client clients[], int nClientsRead, int id, int *index);
void createClientWithId(Client clients[], Client newClient, int *nClientsRead, int *index);
void filterClients(Client clients[], int nClientsRead, string **params, bool fields[], int sortBy[]);
void sortClients(Client *clients, int m, int sortBy[], int n);
void clientsMergeSort(Client clients[], int n, int sortByIndex);
void clientsMerge(Client clients[], Client sorted[], int low, int mid, int high, int sortByIndex);

// Function to Get an Array of Clients from clients.csv
int getClients(Client clients[])
{ // Should be in clients.cpp
  int count = 0, nline = 0, nClientsRead = 0;
  string line, word;

  ifstream infile(clientsFilename, ios::out | ios::binary);

  if (!infile.is_open())
  { // Couldn't Access to infile
    infile.close();
    pressEnterToCont("Error: File Not Found. Press ENTER to go Back to Main Menu", true);
  }

  while (nClients >= nClientsRead)
  {
    Client clientRead;

    infile.read((char *)&clientRead, sizeof(Client)); // Read Structure
    if (infile.eof())                                 // Reached End of File
      break;
    clients[nline] = clientRead;

    nline++;
    nClientsRead = nline;
  }

  infile.close();

  return nClientsRead;
}

// Function to Add Client
void addClientToFile(Client clients[], int *nClientsRead)
{
  if (*nClientsRead >= nClients)
    pressEnterToCont("The Maximum Number of Clients has been Reached", true);
  else
  {
    Client newClient = Client();

    int iter, check, index;
    string temp, date, accountType;

    while (true) // Get Client ID
      try
      {
        cout << "ID: ";
        getline(cin, temp);
        newClient.id = stoi(temp);

        check = checkClientStatusById(clients, *nClientsRead, newClient.id, &index);
        if (newClient.id <= 0)
          throw(-1); // ID Must be in the Range 0<ID<n
        break;
      }
      catch (...)
      {
        wrongClientData(invalidClientId);
      }

    if (check != clientNotFound) // The Id has been Added to that File
      wrongClientData(clientExists);
    else
      createClientWithId(clients, newClient, nClientsRead, &index);
  }
}

// Function to Check if Client Id have been Ocuppied
clientStatus checkClientStatusById(Client clients[], int nClientsRead, int id, int *index)
{
  clientsMergeSort(clients, nClientsRead, clientSortByIdA); // Sort Clients by Id

  bool found = false;
  int value;
  int mid, start = 0, end = nClientsRead - 1;
  string line;

  while (start <= end)
  { // Binary Search
    mid = start + (end - start) / 2;

    try
    {
      value = clients[mid].id;
      found = (value == id);
    }
    catch (...)
    {
      wrongClientData(invalidClientId);
    }

    if (found)
    {
      *index = mid;
      return clientFound;
    }
    else if (id > value)
      start = mid + 1;
    else
      end = mid - 1;
  }
  return clientNotFound;
}

// Function to Create Client after Assigning an Id to the Client
void createClientWithId(Client clients[], Client newClient, int *nClientsRead, int *index)
{
  string temp;

  cout << "Name: "; // Get Client Name
  getline(cin, temp);
  for (int i = 0; i < nFieldChars && i < temp.length(); i++)
    newClient.name[i] = temp[i];

  while (true) // Get Client Phone Number
    try
    {
      cout << "Phone Number: ";
      getline(cin, temp);
      newClient.phoneNumber = stoi(temp);
      break;
    }
    catch (...)
    {
      wrongClientData(invalidClientPhoneNumber);
    }

  cout << "Address: "; // Get Client Address
  getline(cin, temp);
  for (int i = 0; i < nFieldChars && i < temp.length(); i++)
    newClient.address[i] = temp[i];

  clients[*nClientsRead] = newClient;
  *index = *nClientsRead;
  *nClientsRead = *nClientsRead + 1;

  ofstream outfile(clientsFilename, ios::app | ios::binary); // Write to File

  outfile.write((char *)&newClient, sizeof(Client));

  outfile.close();

  pressEnterToCont("Client Added Successfully!", false);
}

// Function that Returns Clients Indexes that Matched with the Parameters
void filterClients(Client clients[], int nClientsRead, string **params, bool fields[], int sortBy[])
{
  clientStatus clientStatus;
  double account;
  int i, id, index, counter = 0;
  string nameLower;

  bool *filteredIndexes = new bool[nClientsRead];               // Allocate Memory
  fill(filteredIndexes, filteredIndexes + nClientsRead, false); // Fill Array with False Values

  for (int field = 0; field < clientFieldEnd - 1; field++)
  {
    if (params[field][0] == "null") // Check if the Function can Filter that Field, and if there are Parameters
      continue;

    for (int param = 0; param < maxParamPerSubCmd && params[field][param] != "null"; param++)
      if (field == clientFieldId)
      {
        id = stoi(params[field][param]);
        clientStatus = checkClientStatusById(clients, nClientsRead, id, &index); // Binary Search

        if (clientStatus != clientNotFound && !filteredIndexes[index])
        {                                // Checks if the Client has Already being Filtered
          filteredIndexes[index] = true; // Store Index
          counter++;
        }
      }
      else
      {
        nameLower = getLower(params[field][param]); // Get Client Name To Search for in Lowercase

        clientsMergeSort(clients, nClientsRead, clientSortByIdA); // Sort Clients by Id
        for (i = 0; i < nClientsRead; i++)
          if (!filteredIndexes[i] && getLower(clients[i].name).find(nameLower) != string::npos)
          {                            // Checks if the Client Name in Lowercase Contains the Parameter that is being Searched by Linear Search
            filteredIndexes[i] = true; // Save Id
            counter++;
          }
      }
  }

  Client *filteredClients = new Client[counter];

  counter = 0;
  for (int i = 0; i < nClientsRead; i++)
    if (filteredIndexes[i])
      filteredClients[counter++] = clients[i]; // Save Client that has been Filtered to Array

  sortClients(filteredClients, counter, sortBy, clientSortByEnd); // Sort Clients
  printClients(filteredClients, counter, fields);                 // Print Clients

  delete[] filteredClients;
  delete[] filteredIndexes;
}

// Function to Sort Clients (Uses Merge Sort)
void sortClients(Client *clients, int m, int sortBy[], int n)
{
  for (int i = 0; i < n; i++)
    if (sortBy[i] != -1)
      clientsMergeSort(clients, m, sortBy[i]);
}

// - Merge Sort
// O(n*logn)
// Stable
void clientsMergeSort(Client clients[], int n, int sortByIndex)
{
  int pass, low, high, mid, i;
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
  {
    int j = 0;
    Client temp;

    for (i = n - 1; i > (n - 1) / 2; i--)
    {
      temp = clients[j];
      clients[j] = clients[i];
      clients[i] = temp;
      j++;
    }
  }
}

// Function to Merge Clients Subarrays
void clientsMerge(Client clients[], Client sorted[], int low, int mid, int high, int sortByIndex)
{
  int i = low, j = mid + 1, k = low;
  string name1, name2;

  switch (sortByIndex / 2)
  {
  case clientSortByIdA / 2:
    while (i <= mid && j <= high)
      if (clients[i].id < clients[j].id)
        sorted[k++] = clients[i++];
      else
        sorted[k++] = clients[j++];
    break;

  case clientSortByNameA / 2:
    while (i <= mid && j <= high)
    {
      name1 = clients[i].name;
      name2 = clients[j].name;

      if (name1.compare(name2) < 0)
        sorted[k++] = clients[i++];
      else
        sorted[k++] = clients[j++];
    }
    break;
  }

  while (i <= mid)
    sorted[k++] = clients[i++];
  while (j <= high)
    sorted[k++] = clients[j++];

  for (i = low; i <= high; i++)
    clients[i] = sorted[i];
}

// Function to Ask for Client Id
int getClientId(string message)
{
  string temp;

  while (true)
    try // Get Client ID
    {
      cout << message << ": ";
      getline(cin, temp);
      return stoi(temp);
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