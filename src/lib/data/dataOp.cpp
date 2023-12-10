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
extern bool movieValidFieldsToFilter[], clientValidFieldsToFilter[];
extern char *movieFieldCmdsStr[], *clientFieldCmdsStr[], *genreStr[];
extern int movieFieldCmdsChar[], clientFieldCmdsChar[];

// --- Function Prototypes
int isCharOnArray(int character, int array[], int n);
void addMovie(Movies *movies);
void rentMovie(Movies *movies, Clients *clients);
void printMovieInfo(Movie movie);
void viewMovies(Movies *movies, bool fields[], int sortBy[]);
void filterMovies(Movies *movies, string **params, int sortBy[]);
void getMovieStatus(Movies *movies);
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
void addClient(Clients *clients);
int getMovieSortByStr(int sortBy[], string sortByStr[], int n);
int getClientSortByStr(int sortBy[], string sortByStr[], int n);

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
  Client client = Client();
  int movieId, movieIndex, clientId, clientIndex, nMoviesRead = (*movies).getNumberMovies(), nClientsRead = (*clients).getNumberClients();
  clientStatus clientStatus;
  movieStatus movieStatus;
  string message, temp;

  cout << clear;
  printTitle("Rent Movie", applyBgColor, applyFgColor, false);

  cout << '\n';

  movieStatus = getMovieId(movies, &movieId, &movieIndex, "Movie ID to Rent: "); // Get Movie ID

  assert(movieId > 0); // Check Movie Id

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

  cout << clear;
  printTitle("Rent Movie", applyBgColor, applyFgColor, false);

  clientStatus = getClientId(clients, &clientId, &clientIndex, "Client ID: "); // Get Client ID

  if (clientStatus == clientNotFound)
  {
    client.id = clientId;
    createClientWithId(clients, client, &clientIndex);
  }
  assert(clientId > 0 && clientIndex >= 0); // Check Client Id and Index

  (*movies).rentMovie(clientId, movieIndex); // Rent Movie

  ofstream outfile(moviesFilename); // Update the movies.csv file with the Movie that was Rented

  int rentOn[3];
  Movie movie;
  string genresStr, dateStr, rentOnStr;

  if (!outfile.is_open())
    pressEnterToCont("Error: File Not Found. Press ENTER to go Back to Main Menu", true);
  else
    for (int i = 0; i < nMoviesRead; i++)
    {
      movie = (*movies).getMovie(i); // Get Movie
      genresStr = getGenresStr(movie.genres);
      dateStr = getDateStr(movie.releaseDate);
      rentOnStr = getCurrentDate(movie.rentOn); // Get Current Date

      outfile << movie.id << sep << movie.name << sep << genresStr
              << sep << movie.duration << sep << movie.director
              << sep << movie.price << sep << dateStr;

      if (movie.rentStatus)
        outfile << sep << movie.rentTo << sep << rentOnStr
                << sep << true << '\n';
      else
        outfile << string(3, sep) << '\n';
    }
  outfile.close();

  pressEnterToCont("Movie Rented Successfully!", false);
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

// Function to Check Movie Rent Status
void getMovieStatus(Movies *movies)
{
  int id, movieStatus, index;
  string temp, message;

  cout << clear;
  printTitle("Get Movie Status", applyBgColor, applyFgColor, false);

  cout << '\n';
  movieStatus = getMovieId(movies, &id, &index, "ID: "); // Get Movie ID

  assert(id > 0); // Check Movie Id

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

  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Filter Clients
void searchClients(Clients *clients, string **params, int sortBy[])
{
  int l = clientFieldEnd - 1, m = maxParamPerSubCmd, n = clientFieldEnd - 1, nClientsFiltered;
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

  for (int i = 0; i <= movieFieldEnd - 1; i++)
  {
    if (!movieValidFieldsToFilter[i]) // Invalid Field to Filter
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

  for (int i = 0; i <= clientFieldEnd - 1; i++)
  {
    if (!clientValidFieldsToFilter[i]) // Invalid Field to Filter
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
  for (int i = 0; i < clientFieldEnd - 1; i++)
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
          "v -f . -s i", "View All Movie Fields. Sort them by Id in Ascending Order"},
      cmdExplanation{
          "v -f g p r -s I p", "View Genre, Price and Release Date Movie Field. Sort them by Id in Descending Order and by Price in Ascending Order"}};

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
          "f -f --g Horror Fiction -s i", "Search for Movies that their Genre is Horror and Fiction. Sort them by Id in Ascending Order"},
      cmdExplanation{
          "f -f --p 20 -s P", "Search for Movies that their Price is 20 or less. Sort them by Price in Descending Order"},
      cmdExplanation{
          "f -f --i 101 --g Thriller Mistery -s R", "Search for Movie with Id 101 and Movies which has as Genre Either Thriller or Mistery. Sort them by Release Date in Descending Order"},
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
      cmdExplanation{"V -f . -s i", "View All Client Fields. Sort them by Id in Ascending Order"},
      cmdExplanation{"V -f a i n -s P", "View Client Account Number, Id and Name Field. Sort them by Phone Number in Descending Order"}};

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
          "F -f --i 101 --n Ramon -s i", "Search for Clients Named as Ramon and the Client with Id 101. Sort them by Id in Ascending Order"},
      cmdExplanation{
          "F -f --n \"Andres Avila\" \"Ronald Lopez\" Ivana Grecia -s n", "Search for Clients Named as Andres Avila, Ronald Lopez, Ivana and Grecia. Sort them by Name in Ascending Order"},
      cmdExplanation{"F -f --n  Roberto --i 123456789 -s N", "Search for Clients whose Name is Roberto and the Client whose Id is 123456789. Sort them by Name in Descending Order"}};

  printExamples(examples, nCmds);

  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
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
      sortByStr[nParams++] = order.append(movieFieldCmdsStr[fieldIndex]); // Data to Print in the Sort By Parameters Row
  }
  assert(nParams >= 0); // Check nParams
  return nParams;
}

// Function to Get a String Array from a Int Array of the Sort By Commands that will be Applied to the Clients
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
    ascOrder = sortByIndex % 2 == 0;                                                         // If the Index is an Odd Number, it's in Descending Order
    fieldIndex = sortByIndex / 2;

    if (sortByIndex == -1)
      nullParam = true;
    else if (ascOrder)
      order = "[A] ";
    else
      order = "[D] ";

    if (!nullParam)
      sortByStr[nParams++] = order.append(clientFieldCmdsStr[fieldIndex]); // Data to Print in the Sort By Parameters Row
  }
  assert(nParams >= 0); // Check nParams
  return nParams;
}