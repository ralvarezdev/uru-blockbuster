#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "..\namespaces.h"
#include "..\terminal\input.h"

using namespace std;
using namespace clients;
using namespace files;

// --- Function Prototypes
int getClients(Client clients[]);
int getClientId(string message);
void addClientToFile(Client clients[], int *nClientsRead);
clientStatus checkClientStatusById(Client clients[], int nClientsRead, int id, int *index);
void createClientWithId(Client clients[], Client newClient, int *nClientsRead, int *index);
void clientsMergeSort(Client clients[], int n, int sortByIndex);
void clientsMerge(Client clients[], Client sorted[], int low, int mid, int high, int sortByIndex);

// Function to Get an Array of Clients from clients.csv
int getClients(Client clients[])
{ // Should be in clients.cpp
  int count = 0, nline = 0, nClientsRead;
  string line, word;

  ifstream infile(clientsFilename);

  if (!infile.is_open())
  { // Couldn't Access to infile
    infile.close();
    pressEnterToCont("Error: File Not Found. Press ENTER to go Back to Main Menu", true);
  }

  while (getline(infile, line) && nClients >= nClientsRead)
    try
    {
      stringstream file(line);

      while (getline(file, word, sep))
      {
        if (word.length() != 0)
          switch (count)
          {
          case 0:
            clients[nline].id = stoi(word);
            break;
          case 1:
            clients[nline].name = word;
            break;
          case 2:
            clients[nline].phoneNumber = word;
            break;
          case 3:
            clients[nline].address = word;
            break;
          }
        count++;
      }
      nline++;
      nClientsRead = nline;
      count = 0;
    }
    catch (...)
    {
      // It will Ignore the Line that was Read from clients.csv
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
  cout << "Name: "; // Get Client Name
  getline(cin, newClient.name);

  cout << "Phone Number: "; // Get Client Phone Number
  getline(cin, newClient.phoneNumber);

  cout << "Address: "; // Get Client Address
  getline(cin, newClient.address);

  clients[*nClientsRead] = newClient;
  *index = *nClientsRead;
  *nClientsRead = *nClientsRead + 1;

  ofstream outfile(clientsFilename, ios::app); // Write to File
  outfile << newClient.id << sep
          << newClient.name << sep
          << newClient.phoneNumber << sep
          << newClient.address << '\n';

  outfile.close();

  pressEnterToCont("Client Added Successfully!", false);
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
      if (clients[i].name.compare(clients[j].name) < 0)
        sorted[k++] = clients[i++];
      else
        sorted[k++] = clients[j++];
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

* /
*/