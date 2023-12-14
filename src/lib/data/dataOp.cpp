#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>

// #define NDEBUG
#include <assert.h>

#include "dataOp.h"
#include "../terminal/ansiEsc.h"
#include "../terminal/input.h"
#include "../movies/moviesOp.h"
#include "../clients/clientsOp.h"
#include "../datatables/output.h"
#include "../namespaces.h"

using namespace std;
using namespace clients;
using namespace commands;
using namespace files;
using namespace movies;
using namespace terminal;

// --- Extern Variables Declaration
extern bool movieValidFieldsToFilter[], clientValidFieldsToFilter[],
    movieValidFieldsToSort[], clientValidFieldsToSort[];
extern char *movieFieldCmdsStr[], *clientFieldCmdsStr[], *genreStr[];
extern int movieFieldCmdsChar[], clientFieldCmdsChar[];

// --- Function Prototypes
int isCharOnArray(int character, int array[], int n);
void addMovie(Movies *movies);
void rentMovie(Movies *movies, Clients *clients);
void returnMovie(Movies *movies, Clients *clients);
void removeMovie(Movies *movies);
void addClient(Clients *clients);
movieStatus getClientRent(Movies *movies, int clientId, int *movieId);
void removeClient(Movies *movies, Clients *clients);
void viewMovies(Movies *movies, bool fields[], int sortBy[]);
void filterMovies(Movies *movies, string **params, int sortBy[]);
void displayRentedMovies(Movies *movies);
void clientRentedMovies(Clients *clients);
void getMovieStatus(Movies *movies);
void viewClients(Clients *clients, bool fields[], int sortBy[]);
void searchClients(Clients *clients, string **params, int sortBy[]);
void validParameters(int nCharTitle);
void movieFields();
void clientFields();
void sortByParameters();
void validGenres();
void howToUseViewMovies();
void howToUseFilterMovies();
void howToUseViewClients();
void howToUseSearchClients();
int getMovieSortByStr(int sortBy[], string sortByStr[], int n);
int getClientSortByStr(int sortBy[], string sortByStr[], int n);
void deleteHistory(int id, bool isMovieId);

// --- Functions

// Function to check if the Integer that Represents a Character is on Char Array
int isCharOnArray(int character, int array[], int n)
{
  for (int i = 0; i < n; i++)
    if (array[i] == character)
      return i;
  return -1;
}

// Function to Add Some Movies to movies.csv
void addMovie(Movies *movies)
{
  while (true)
  {
    cout << clear;
    validGenres();
    printTitle("Add Movie", applyBgColor, applyFgColor, false);

    cout << '\n';
    addMovieToFile(movies);

    if (!booleanQuestion("Do you want to Add more Movies?"))
      break;
  }
}

// Function to Rent Movie
void rentMovie(Movies *movies, Clients *clients)
{
  Client client;
  int movieId, movieIndex, clientId, clientIndex, nMoviesRead = (*movies).getNumberMovies(), nClientsRead = (*clients).getNumberClients();
  int date[3];
  clientStatus clientStatus;
  movieStatus movieStatus;
  string message, time;

  cout << clear;
  printTitle("Rent Movie", applyBgColor, applyFgColor, false);

  cout << '\n';
  movieStatus = getMovieId(movies, &movieId, &movieIndex, "Movie ID to Rent"); // Get Movie ID

  assert(movieId > 0); // Check Movie ID

  cout << '\n';
  if (movieStatus == movieNotFound)
    message = "Error 404: Movie Not Found. Run \"Add Movie Command\"";
  else if (movieStatus == movieNotRented)
    message = "Movie Found: Avalaible for Rent";
  else if (movieStatus == movieRented)
    message = "Movie Found: Already Rented";

  if (movieStatus != movieNotFound)
  {
    printMovieInfo((*movies).getMovie(movieIndex));
    cout << '\n';
  }

  assert((movieStatus == movieNotFound) ? movieIndex == -1 : movieIndex >= 0); // Check Movie Index

  pressEnterToCont(message, (movieStatus == movieNotRented) ? false : true);

  if (movieStatus != movieNotRented)
    return; // End this Function
  else if (!booleanQuestion("Is this the Movie you want to Rent?"))
    return; // End this Function

  cout << clear;
  printTitle("Rent Movie", applyBgColor, applyFgColor, false);

  while (true)
  {
    cout << '\n';
    clientStatus = getClientId(clients, &clientId, &clientIndex, "Client ID"); // Get Client ID

    if (clientStatus == clientNotFound)
    {
      client.id = clientId;
      cout << '\n';
      createClientWithId(clients, client, &clientIndex);
      break;
    }
    else if (getClientRent(movies, clientId, &movieId) == movieRented) // Check if the Client has Already a Rent
    {
      message = "Error: Client Hasn't Returned Some Movies";
      pressEnterToCont(message, true);
      return; // End this Function
    }
    else
    {
      client = (*clients).getClient(clientIndex); // Get Client Index

      cout << '\n';
      printClientInfo(client, true);
      cout << '\n';

      if (booleanQuestion("Is this your Account?"))
        break;
    }
  }

  assert(clientId > 0 && clientIndex >= 0); // Check Client ID and Index

  moviesMergeSort(movies, movieFieldId * 2);
  (*movies).rentMovie(clientId, movieIndex); // Rent Movie

  time = getCurrentDate(date);
  storeMovieMovement(time, clientId, movieId, true); // Store Movie Rent in rents.csv

  overwriteMovies(movies);

  pressEnterToCont("Movie Rented Successfully!", false);
}

// Function to Return Movie
void returnMovie(Movies *movies, Clients *clients)
{
  Client client;
  Movie movie;
  int movieId, movieIndex, clientId, clientIndex, date[3];
  clientStatus clientStatus;
  movieStatus movieStatus;
  string time;

  cout << clear;
  printTitle("Return Movie", applyBgColor, applyFgColor, false);

  while (true)
  {
    cout << '\n';
    clientStatus = getClientId(clients, &clientId, &clientIndex, "Client ID"); // Get Client ID

    assert(clientId > 0); // Check Client ID

    cout << '\n';
    if (clientStatus == clientNotFound)
    {
      pressEnterToCont("Error 404: Client Not Found. Run \"Add Client Command\"", true);
      return; // End this Function
    }

    assert(clientIndex >= 0); // Check Client Index

    printClientInfo((*clients).getClient(clientIndex), true);
    cout << '\n';

    if (booleanQuestion("Is this your Account?"))
      break;
  }

  movieStatus = getClientRent(movies, clientId, &movieId); // Get Movie Rent ID

  cout << '\n';
  if (movieStatus != movieRented) // Client doesn't have Any Movie Rented
  {
    pressEnterToCont("Error 404:Client doesn't have Any Rent", true);
    return; // End this Function
  }

  checkMovie(movies, movieId, &movieIndex); // Get Movie Index

  assert(movieId > 0 && movieIndex >= 0); // Check Movie ID and Index

  movie = (*movies).getMovie(movieIndex); // Get Movie
  printMovieInfo(movie);
  cout << '\n';

  if (!booleanQuestion("Do you want to Return the Movie?"))
    return; // End of this Function

  (*movies).returnMovie(movieIndex); // Set Movie to Available for Rent

  time = getCurrentDate(date);
  storeMovieMovement(time, clientId, movieId, false); // Store Movie Return in rents.csv

  overwriteMovies(movies); // Overwrite Movies

  pressEnterToCont("Movie Returned Successfully!", false);
}

// Function to Remove Movie from movies.csv
void removeMovie(Movies *movies)
{
  bool deleteMovie;
  movieStatus check;
  int id, index = -1;

  cout << clear;
  printTitle("Remove Movie", applyBgColor, applyFgColor, false);

  cout << '\n';
  check = getMovieId(movies, &id, &index, "Movie ID");

  if (check == movieNotFound) // The ID hasn't been Added to that File
  {
    assert(id > 0 && index == -1); // Check Movie ID and Index
    cout << '\n';
    pressEnterToCont("Error 404: Movie Not Found. Run \"Add Movie Command\"", true);
    return; // End this Function
  }

  assert(id > 0 && index >= 0); // Check Movie ID and Index

  cout << '\n';
  printMovieInfo((*movies).getMovie(index)); // Print Movie Info
  cout << '\n';

  if (booleanQuestion("Is this the Movie you want to Delete?"))
    if (booleanQuestion("Are you 100% Sure?"))
      deleteMovie = true;

  if (deleteMovie)
  {
    (*movies).deleteAt(index); // Delete Movie
    deleteHistory(id, true);   // Delete Movie Rent History
    overwriteMovies(movies);   // Overwrite Movies

    pressEnterToCont("Movie Deleted", true);
  }
}

// Function to Add Some Clients to clients.csv
void addClient(Clients *clients)
{
  while (true)
  {
    cout << clear;
    printTitle("Add Client", applyBgColor, applyFgColor, false);

    cout << '\n';
    addClientToFile(clients);

    if (!booleanQuestion("Do you want to Add more Clients?"))
      break;
  }
}

// Get Movie ID Rented by the Given Client ID
movieStatus getClientRent(Movies *movies, int clientId, int *movieId)
{
  int mid, startA = 0, endA = (*movies).getNumberMovies() - 1;
  Movie movie;

  moviesMergeSort(movies, movieFieldRentTo * 2); // Sort Movies by Rent Field

  while (startA <= endA)
  {
    mid = startA + (endA - startA) / 2;
    movie = (*movies).getMovie(mid);

    if (movie.rentStatus && movie.rentTo == clientId)
    {
      *movieId = movie.id;
      break;
    }
    else if (movie.rentTo < clientId)
      startA = mid + 1;
    else
      endA = mid - 1;
  }

  return (clientId == movie.rentTo) ? movieRented : movieNotFound;
}

// Function to Remove Client from clients.bin
void removeClient(Movies *movies, Clients *clients)
{
  bool deleteClient;
  clientStatus check;
  int clientId, movieId, index = -1;

  cout << clear;
  printTitle("Remove Client", applyBgColor, applyFgColor, false);

  cout << '\n';
  check = getClientId(clients, &clientId, &index, "Client ID");

  if (check == clientNotFound) // The ID hasn't been Added to that File
  {
    assert(clientId > 0 && index == -1); // Check Client ID and Index
    cout << '\n';
    pressEnterToCont("Error 404: Client Not Found. Run \"Add Client Command\"", true);
    return; // End this Function
  }
  else if (getClientRent(movies, clientId, &movieId) == movieRented)
  { // Check if the Client has Already a Rent
    pressEnterToCont("Error: Client Hasn't Returned Some Movies", true);
    return; // End this Function
  }

  assert(clientId > 0 && index >= 0); // Check Client ID and Index

  cout << '\n';
  printClientInfo((*clients).getClient(index), true); // Print Client Info
  cout << '\n';

  if (booleanQuestion("Is this the Client Account you want to Delete?"))
    if (booleanQuestion("Are you 100% Sure?"))
      deleteClient = true;

  if (deleteClient)
  {
    (*clients).deleteAt(index);     // Delete Client
    deleteHistory(clientId, false); // Delete Client Rent History
    overwriteClients(clients);      // Overwrite Clients

    pressEnterToCont("Client Deleted", true);
  }
}

// Function to View Movies
void viewMovies(Movies *movies, bool fields[], int sortBy[])
{
  int m = movieFieldEnd - 1, n = movieFieldEnd - 1;
  string fieldsStr[m], sortByStr[n], applied;

  assert(m > 0 && n > 0); // Check Variables

  if (fields[movieFieldAll])
    for (int i = 0; i < m; i++)
      fields[i] = true;

  for (int i = 0; i < m; i++)
  {
    applied = fields[i] ? "[Y] " : "[N] ";
    fieldsStr[i] = applied.append(movieFieldCmdsStr[i]); // Data to Print in the Field Parameters Row
  }

  n = getMovieSortByStr(sortBy, sortByStr, n); // Get Sort By Array Length

  assert(n > 0); // Check Variables

  cout << clear;
  printTitle("Movie Fields", applyBgColor, applyFgColor, false);
  printArray(fieldsStr, m, "Fields");

  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  printArray(sortByStr, n, "Sort By");

  pressEnterToCont("Press ENTER to Continue", false);

  sortMovies(movies, sortBy, movieFieldEnd - 1); // Sort Movies
  printMovies(movies, fields);                   // Print Movies

  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Filter Movies
void filterMovies(Movies *movies, string **params, int sortBy[])
{
  int l = movieFieldEnd - 1, m = maxParamPerSubCmd, n = movieFieldEnd - 1;
  bool fields[l];
  string sortByStr[n];

  assert(l > 0 && m > 0); // Check Variables

  fill(fields, fields + l, true); // Movie Fields to Print (All)
  n = getMovieSortByStr(sortBy, sortByStr, n);

  assert(n > 0); // Check Variables

  cout << clear;
  printTitle("Movie Fields Parameters", applyBgColor, applyFgColor, false);
  print2DArray(params, l, m, movieFieldCmdsStr);

  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  printArray(sortByStr, n, "Sort By");

  pressEnterToCont("Press ENTER to Continue", false);

  filterMoviesData(movies, params, fields, sortBy); // Filter Movies

  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Display Either Rented or Avalaible Movies
void displayRentedMovies(Movies *movies)
{
  bool isRented;
  int counter;
  string message;
  Movie movie;
  Movies filteredMovies = Movies();

  int m = movieFieldEnd - 1;
  assert(m > 0); // Check Variables

  bool fields[m];
  for (int i = 0; i < m; i++)
    fields[i] = true;

  cout << clear;
  printTitle("Rented Movies", applyBgColor, applyFgColor, false);

  cout << '\n';
  isRented = booleanQuestion("Do you want to Display Rented Movies?");

  if (isRented)
    moviesMergeSort(movies, movieFieldStatus * 2 + 1); // Sort Movies by Rent Field in Descending Order
  else
    moviesMergeSort(movies, movieFieldStatus * 2); // Sort Movies by Rent Field in Ascending Order

  for (int i = 0; i < (*movies).getNumberMovies(); i++)
  {
    movie = (*movies).getMovie(i); // Get Movie

    if (movie.rentStatus == isRented)
      filteredMovies.pushBack(movie);
  }

  counter = filteredMovies.getNumberMovies();

  cout << clear;
  printMovies(&filteredMovies, fields); // Print Movies

  message = "Number of Coincidences: ";
  message.append(to_string(counter));

  cout << '\n';
  printTitle(message, applyBgColor, applyFgColor, (counter == 0) ? true : false); // Print Number of Coincidences

  filteredMovies.deallocate(); // Deallocate Memory

  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Print Client Movie Rent History
void clientRentedMovies(Clients *clients)
{
  clientStatus check;
  bool isClient;
  int clientId, movieId, tempId, count, movCounter = 0, index = -1, date[3];
  string line, word;

  const int nMovieId = 20; // Number of Characters for Movie ID
  const int nType = 20;    // ... for Movement Type

  int fieldsNChar[3] = {nMovieId, nType, nChar - nMovieId - nType}; // Separators Character Size
  string rentTitle[3] = {"Movie ID", "Movement", "Date"};

  cout << clear;
  printTitle("Client Rent History", applyBgColor, applyFgColor, false);

  cout << '\n';
  check = getClientId(clients, &clientId, &index, "Client ID");

  if (check == clientNotFound) // The ID hasn't been Added to that File
  {
    assert(clientId > 0 && index == -1); // Check Client ID and Index
    cout << '\n';
    pressEnterToCont("Error 404: Client Not Found. Run \"Add Client Command\"", true);
    return; // End this Function
  }

  assert(clientId > 0 && index >= 0); // Check Client ID and Index

  cout << '\n';
  printClientInfo((*clients).getClient(index), true); // Print Client Info
  cout << '\n';

  if (!booleanQuestion("Is this your Account?"))
    return; // End this Function

  ostringstream rents;
  ifstream ifRentsCSV(rentsFilename);

  rents << clear << sgrBgCmd << sgrFgCmd << left;
  for (int i = 0; i < 3; i++) // Rents Title
    rents << setw(fieldsNChar[i]) << setfill(' ') << rentTitle[i];
  rents << reset << '\n';

  while (getline(ifRentsCSV, line)) // Get Movie Rents
    try
    {
      if (line.length() == 0)
        continue;

      stringstream file(line);

      count = 0;
      isClient = false;
      while (getline(file, word, sep))
      {
        if (word.length() != 0 && (count == 0 || isClient))
          switch (count)
          {
          case 0:
            try
            {
              tempId = stoi(word); // Client ID
              isClient = (tempId == clientId);

              if (isClient)
                movCounter++;
            }
            catch (...)
            {
              // Ignore Movie Movement
            }
            break;
          default:
            rents << setw(fieldsNChar[count - 1]) << setfill(' ') << word;
          }
        count++;
      }
      if (isClient)
        rents << '\n';
    }
    catch (...)
    {
      // It will Ignore the Line that was Read from rents.csv
    }
  ifRentsCSV.close();

  if (movCounter == 0)
  {
    printTitle("No Movement Found", true, true, true);
    cout << '\n';
    pressEnterToCont("Press ENTER to Continue", false);
    return; // End this Function
  }

  cout << rents.str();

  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Check Movie Rent Status
void getMovieStatus(Movies *movies)
{
  int id, movieStatus, index;
  string temp, message;

  cout << clear;
  printTitle("Get Movie Status", applyBgColor, applyFgColor, false);

  cout << '\n';
  movieStatus = getMovieId(movies, &id, &index, "Movie ID"); // Get Movie ID

  assert(id > 0); // Check Movie ID

  cout << '\n';
  if (movieStatus == movieNotFound)
    message = "Error 404: Movie Not Found. Run \"Add Movie Command\"";
  else if (movieStatus == movieNotRented)
    message = "Movie Found: Avalaible for Rent";
  else if (movieStatus == movieRented)
    message = "Movie Found: Already Rented";

  assert((movieStatus == movieNotFound) ? index == -1 : index >= 0); // Check Movie Index

  if (movieStatus != movieNotFound)
    printMovieInfo((*movies).getMovie(index));

  pressEnterToCont(message, false);
}

// Function to View Clients
void viewClients(Clients *clients, bool fields[], int sortBy[])
{
  int m = clientFieldEnd - 1, n = clientFieldEnd - 1;
  string fieldsStr[m], sortByStr[n], applied;

  assert(m > 0 && n > 0); // Check Variables

  if (fields[clientFieldAll])
    for (int i = 0; i < m; i++)
      fields[i] = true;

  for (int i = 0; i < m; i++)
  {
    applied = fields[i] ? "[Y] " : "[N] ";
    fieldsStr[i] = applied.append(clientFieldCmdsStr[i]); // Data to Print in the Field Parameters Row
  }

  n = getClientSortByStr(sortBy, sortByStr, n); // Get Sort By Array Length

  assert(n > 0); // Check Variables

  cout << clear;
  printTitle("Client Fields", applyBgColor, applyFgColor, false);
  printArray(fieldsStr, m, "Fields");

  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  printArray(sortByStr, n, "Sort By");

  pressEnterToCont("Press ENTER to Continue", false);

  sortClients(clients, sortBy, clientFieldEnd - 1); // Sort Clients
  printClients(clients, fields);                    // Print Clients

  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Filter Clients
void searchClients(Clients *clients, string **params, int sortBy[])
{
  int l = clientFieldEnd - 1, m = maxParamPerSubCmd, n = clientFieldEnd - 1;
  bool fields[l];
  string sortByStr[n];

  assert(l > 0 && m > 0); // Check Variables

  fill(fields, fields + l, true); // Client Fields to Print (All)
  n = getClientSortByStr(sortBy, sortByStr, n);

  assert(n > 0); // Check Variables

  cout << clear;
  printTitle("Client Fields Parameters", applyBgColor, applyFgColor, false);
  print2DArray(params, l, m, clientFieldCmdsStr);

  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  printArray(sortByStr, n, "Sort By");

  pressEnterToCont("Press ENTER to Continue", false);

  filterClientsData(clients, params, fields, sortBy); // Filter Clients

  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Print the Two Types of Parameters (Used in Filter Movies and Search Clients Command)
void validParameters(int nCharTitle)
{
  string temp = "\"sentence to search\"";

  cout << tab1 << setw(nCharTitle) << setfill(' ') << addBrackets<string>("word") << "Word to Search\n"
       << tab1 << setw(nCharTitle) << setfill(' ') << addBrackets<string>(temp) << "Sentence to Search\n\n";
}

// Function to Print Field as a Parameter and as a Command
void movieFields()
{
  string temp;

  cout << clear; // Clear Terminal
  printTitle("Field as a Parameter (for View Movies)", applyBgColor, applyFgColor, false);
  for (int i = 0; i < movieFieldEnd; i++)
    cout << tab1 << setw(nCharTitle) << setfill(' ') << addBrackets(movieFieldCmdsChar[i]) << movieFieldCmdsStr[i] << '\n';
  cout << '\n';

  printTitle("Field as a Command (for Filter Movies)", applyBgColor, applyFgColor, false);
  for (int i = 0; i < movieFieldEnd - 1; i++)
    if (movieValidFieldsToFilter[i])
    {
      temp = addBrackets(movieFieldCmdsChar[i]).append(" [param...]");
      cout << tab1 << setw(nCharTitle) << setfill(' ') << temp << "Parameters for Movie's " << movieFieldCmdsStr[i] << '\n';
    }
  cout << '\n';

  printTitle("Valid Field Parameters (for Filter Movies)", applyBgColor, applyFgColor, false);
  validParameters(nCharTitle);

  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Print Movie and Client Sort By Parameters
void sortByParameters()
{
  char cmd;
  string ascending, descending;

  cout << clear; // Clear Terminal
  printTitle("Movie Sort By Parameters", applyBgColor, applyFgColor, false);

  for (int i = 0; i < movieFieldEnd; i++)
  {
    if (!movieValidFieldsToSort[i]) // Invalid Field to Sort
      continue;

    cmd = movieFieldCmdsChar[i]; // Get Field Command

    ascending = addBrackets(cmd).append(" Ascending");            // Get Ascending Order Sort By Command
    descending = addBrackets(toupper(cmd)).append(" Descending"); // Get Descending Order Sort By Command

    cout << tab1 << setw(nCharTitle) << setfill(' ') << movieFieldCmdsStr[i]
         << setw(nCharTitle) << setfill(' ') << ascending
         << setw(nCharTitle) << setfill(' ') << descending << '\n';
  }
  cout << '\n';

  printTitle("Client Sort By Parameters", applyBgColor, applyFgColor, false);

  for (int i = 0; i < clientFieldEnd; i++)
  {
    if (!clientValidFieldsToSort[i]) // Invalid Field to Sort By
      continue;

    cmd = clientFieldCmdsChar[i]; // Get Field Command

    ascending = addBrackets(cmd).append(" Ascending");           // Get Ascending Order Sort By Command
    descending = addBrackets(toupper(cmd)).append(" Decending"); // Get Descending Order Sort By Command

    cout << tab1 << setw(nCharTitle) << setfill(' ') << clientFieldCmdsStr[i]
         << setw(nCharTitle) << setfill(' ') << ascending
         << setw(nCharTitle) << setfill(' ') << descending << '\n';
  }
  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Print Search Client as a Command
void clientFields()
{
  string temp;

  cout << clear; // Clear Terminal
  printTitle("Field as a Parameter (for View Clients)", applyBgColor, applyFgColor, false);
  for (int i = 0; i < clientFieldEnd; i++)
    cout << tab1 << setw(nCharTitle) << setfill(' ') << addBrackets(clientFieldCmdsChar[i]) << clientFieldCmdsStr[i] << '\n';
  cout << '\n';

  printTitle("Field as a Command (for Search Clients)", applyBgColor, applyFgColor, false);
  for (int i = 0; i < clientFieldEnd; i++)
    if (clientValidFieldsToFilter[i])
    {
      temp = addBrackets(clientFieldCmdsChar[i]).append(" [param...]");
      cout << tab1 << setw(nCharTitle) << setfill(' ') << temp << "Parameters for Client's " << clientFieldCmdsStr[i] << '\n';
    }
  cout << '\n';

  printTitle("Valid Field Parameters (for Search Clients)", applyBgColor, applyFgColor, false);
  validParameters(nCharTitle);

  pressEnterToCont("Press ENTER to Continue", false);
}

// Function that Prints the Valid Genres when Adding a New Movie
void validGenres()
{
  string temp;

  printTitle("Movies Parameters", applyBgColor, applyFgColor, false);

  cout << '\n';
  printArray(genreStr, genreEnd - 1, "Genre");
}

// Function that Prints Examples of View Movies Command
void howToUseViewMovies()
{
  const int nCmds = 2; // Number of Code Examples

  cmdExplanation examples[nCmds] = {
      cmdExplanation{
          "v -f . -s i", "View All Movie Fields. Sort them by ID in Ascending Order"},
      cmdExplanation{
          "v -f g p r -s I p", "View Genre, Price and Release Date Movie Field. Sort them by ID in Descending Order and by Price in Ascending Order"}};

  printExamples(examples, nCmds);

  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// Function that Prints Examples of Filter Movies Command
void howToUseFilterMovies()
{
  const int nCmds = 4; // Number of Code Examples

  cmdExplanation examples[nCmds] = {
      cmdExplanation{
          "f -f --g Horror Fiction -s i", "Search for Movies that their Genre is Horror and Fiction. Sort them by ID in Ascending Order"},
      cmdExplanation{
          "f -f --p 20 -s P", "Search for Movies that their Price is 20 or less. Sort them by Price in Descending Order"},
      cmdExplanation{
          "f -f --i 101 --g Thriller Mistery -s R", "Search for Movie with ID 101 and Movies which has as Genre Either Thriller or Mistery. Sort them by Release Date in Descending Order"},
      cmdExplanation{
          "f -f --d \"David Quiroz\" -s t", "Search for Movies that their Director is David Quiroz. Sort them by Title in Ascending Order"}};

  printExamples(examples, nCmds);

  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// Function that Prints Examples of View Clients Command
void howToUseViewClients()
{
  const int nCmds = 2; // Number of Code Examples

  cmdExplanation examples[nCmds] = {
      cmdExplanation{"V -f . -s i", "View All Client Fields. Sort them by ID in Ascending Order"},
      cmdExplanation{"V -f a i n -s P", "View Client Account Number, ID and Name Field. Sort them by Phone Number in Descending Order"}};

  printExamples(examples, nCmds);

  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// Function that Prints Examples of Search Clients Command
void howToUseSearchClients()
{
  const int nCmds = 3; // Number of Code Examples

  cmdExplanation examples[nCmds] = {
      cmdExplanation{
          "F -f --i 101 --n Ramon -s i", "Search for Clients Named as Ramon and the Client with ID 101. Sort them by ID in Ascending Order"},
      cmdExplanation{
          "F -f --n \"Andres Avila\" \"Ronald Lopez\" Ivana Grecia -s n", "Search for Clients Named as Andres Avila, Ronald Lopez, Ivana and Grecia. Sort them by Name in Ascending Order"},
      cmdExplanation{"F -f --n  Roberto --i 123456789 -s N", "Search for Clients whose Name is Roberto and the Client whose ID is 123456789. Sort them by Name in Descending Order"}};

  printExamples(examples, nCmds);

  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Get a String Array from a Int Array of the Sort By Commands that will be Applied to the Movies
int getMovieSortByStr(int sortBy[], string sortByStr[], int n)
{
  bool nullParam, ascOrder;
  int sortByIndex, fieldIndex, nParams = 0;
  string order;

  for (int j = 0; j < n; j++)
  {
    sortByIndex = sortBy[j];
    nullParam = false;
    assert(sortByIndex == -1 || sortByIndex >= 0 && sortByIndex < (movieFieldEnd - 1) * 2); // Check Sort By Index

    ascOrder = sortByIndex % 2 == 0; // If the Index is an Odd Number, it's in Descending Order
    fieldIndex = sortByIndex / 2;

    if (sortByIndex == -1)
      nullParam = true;
    else if (ascOrder)
      order = "[A] ";
    else
      order = "[D] ";

    if (!nullParam)
      sortByStr[nParams++] = order.append(movieFieldCmdsStr[fieldIndex]); // Data to Print Sort By Parameters Row
  }
  assert(nParams >= 0); // Check nParams
  return nParams;
}

// Function to Get a String Array from an Integer Array of the Sort By Commands that will be Applied to the Clients
int getClientSortByStr(int sortBy[], string sortByStr[], int n)
{
  bool nullParam, ascOrder;
  int sortByIndex, fieldIndex, nParams = 0;
  string order;

  for (int j = 0; j < n; j++)
  {
    sortByIndex = sortBy[j];
    nullParam = false;

    assert(sortByIndex == -1 || sortByIndex >= 0 && sortByIndex < (clientFieldEnd - 1) * 2); // Check Sort By Index

    ascOrder = sortByIndex % 2 == 0; // If the Index is an Odd Number, it's in Descending Order
    fieldIndex = sortByIndex / 2;

    if (sortByIndex == -1)
      nullParam = true;
    else if (ascOrder)
      order = "[A] ";
    else
      order = "[D] ";

    if (!nullParam)
      sortByStr[nParams++] = order.append(clientFieldCmdsStr[fieldIndex]); // Data to Print Sort By Parameters Row
  }
  assert(nParams >= 0); // Check nParams
  return nParams;
}

// Function to Delete Rent History
void deleteHistory(int id, bool isMovieId)
{
  string line, word;
  int tempId, count;

  ostringstream rents;
  ifstream ifRentsCSV(rentsFilename);

  while (getline(ifRentsCSV, line)) // Get Movie Rents
    try
    {
      if (line.length() == 0)
        continue;

      stringstream file(line);

      count = 0;
      while (getline(file, word, sep))
      {
        if (word.length() != 0)
          switch (count)
          {
          case 0:
            try
            {
              tempId = stoi(word); // Client ID

              if (!isMovieId && tempId == id)
                rents << deletedClient;
              else
                rents << tempId;
            }
            catch (...)
            {
              rents << deletedClient;
            }
            break;
          case 1:
            try
            {
              tempId = stoi(word); // Movie ID

              if (isMovieId && tempId == id)
                rents << deletedMovie;
              else
                rents << tempId;
            }
            catch (...)
            {
              rents << deletedMovie;
            }
            break;
          default:
            rents << word; // Movie Movement Type, Movie Movement Time
          }
        rents << sep;
        count++;
      }
      rents << '\n';
    }
    catch (...)
    {
      // It will Ignore the Line that was Read from rents.csv
    }
  ifRentsCSV.close();

  ofstream ofRentsCSV(rentsFilename);

  ofRentsCSV << rents.str(); // Write Rents Stream Content to rents.csv
  ofRentsCSV.close();
}