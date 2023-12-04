#include <algorithm>
#include <iomanip>
#include <iostream>
#include "dataOp.h"
#include <fstream>

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

// --- Function Prototypes
int isCharOnArray(int character, int array[], int n);
void addMovie(Movie movies[], int *nMoviesRead);
void rentMovie(Movie movies[], int nMoviesRead, Client clients[], int *nClientsRead);
void printMovieInfo(Movie movie);
void viewMovies(Movie movies[], int nMoviesRead, bool fields[], int sortBy[]);
void filterMovies(Movie movies[], int nMoviesRead, string **params, int counter[], int sortBy[]);
void getMovieStatus(Movie movies[], int nMoviesRead);
void searchClient(Client clients[], int nClientsRead, string **params, int counter[], int sortBy[]);
void validParameters(int nCharTitle);
void movieFields();
void sortByParameters();
void clientParameters();
void validGenres();
void howToUseViewMovies();
void howToUseFilterMovies();
void howToUseSearchClient();
void addClient(Client clients[], int *nClientsRead);
int getMovieSortByStr(int sortBy[], string sortByStr[], int n);
int getClientSortByStr(int sortBy[], string sortByStr[], int n);

// --- Functions

// Function to check if the Character is on Char Array
int isCharOnArray(int character, int array[], int n)
{
  for (int i = 0; i < n; i++)
    if (array[i] == character)
      return i;
  return -1;
}

// Function to Add Some Movies to movies.csv
void addMovie(Movie movies[], int *nMoviesRead)
{
  while (true)
  {
    cout << clear;
    validGenres();
    printTitle("Add Movie", applyBgColor, applyFgColor, false);

    cout << '\n';
    addMovieToFile(movies, nMoviesRead);

    if (!booleanQuestion("Do you want to Add more Movies?"))
      break;
  }
}

// Function to Rent Movie
void rentMovie(Movie movies[], int nMoviesRead, Client clients[], int *nClientsRead)
{
  Client client = Client();
  int movieId, movieIndex, movieStatus, clientId, clientIndex, clientStatus;
  string message, temp;

  cout << clear;
  printTitle("Rent Movie", applyBgColor, applyFgColor, false);

  cout << '\n';
  while (true) // Get Movie ID
    try
    {
      cout << "Movie ID to Rent: ";
      getline(cin, temp);
      movieId = stoi(temp);
      break;
    }
    catch (...)
    {
      wrongMovieData(invalidMovieId);
    }

  movieStatus = checkMovieStatusById(movies, nMoviesRead, movieId, &movieIndex);

  cout << '\n';
  if (movieStatus == movieNotFound)
    message = "Error 404: Movie Not Found. Run \"Add Movie Command\"";
  else if (movieStatus == movieNotRented)
    message = "Movie Found: Avalaible for Rent";
  else if (movieStatus == movieRented)
    message = "Movie Found: Already Rented";

  if (movieStatus != movieNotFound)
  {
    printMovieInfo(movies[movieIndex]);
    cout << '\n';
  }

  pressEnterToCont(message, (movieStatus == movieNotRented) ? false : true);

  if (movieStatus == movieNotRented)
  {
    cout << clear;
    printTitle("Rent Movie", applyBgColor, applyFgColor, false);

    cout << '\n';
    while (true) // Get Client ID
      try
      {
        cout << "Client ID: ";
        getline(cin, temp);
        clientId = stoi(temp);
        break;
      }
      catch (...)
      {
        wrongClientData(invalidClientId);
      }

    clientStatus = checkClientStatusById(clients, *nClientsRead, clientId, &clientIndex); // Should be in clients.cpp

    if (clientStatus == clientNotFound)
    {
      client.id = clientId;
      createClientWithId(clients, client, nClientsRead, &clientIndex);
    }

    movies[movieIndex].rentTo = clientId;
    movies[movieIndex].rentStatus = true; // Rented

    ofstream outfile(moviesFilename); // Update the movies.csv file with the Movie that was Rented

    int rentOn[3];
    string genresStr, dateStr, rentOnStr;

    rentOnStr = getCurrentDate(movies[movieIndex].rentOn); // Get Current Date

    if (!outfile.is_open())
      pressEnterToCont("Error: File Not Found. Press ENTER to go Back to Main Menu", true);
    else
      for (int i = 0; i < nMoviesRead; i++)
      {
        genresStr = getGenresStr(movies[i].genres, genreEnd);
        dateStr = getDateStr(movies[i].releaseDate);

        outfile << movies[i].id << sep << movies[i].name << sep << genresStr
                << sep << movies[i].duration << sep << movies[i].director
                << sep << movies[i].price << sep << dateStr;

        if (movies[i].rentStatus)
          outfile << sep << movies[i].rentTo << sep << rentOnStr
                  << sep << true << '\n';
        else
          outfile << string(3, sep) << '\n';
      }
    outfile.close();

    pressEnterToCont("Movie Rented Successfully!", false);
  }
}

// Function to View Movies
void viewMovies(Movie movies[], int nMoviesRead, bool fields[], int sortBy[])
{
  int m = movieFieldEnd - 1, n = movieSortByEnd / 2;
  string fieldsStr[m], sortByStr[n], applied;

  if (fields[movieFieldAll])
    for (int i = 0; i < m; i++)
      fields[i] = true;

  for (int i = 0; i < m; i++)
  {
    applied = fields[i] ? "[Y] " : "[N] ";
    fieldsStr[i] = applied.append(movieFieldCmdsStrPtr[i]); // Data to Print in the Field Parameters Row
  }

  n = getMovieSortByStr(sortBy, sortByStr, n); // Get Sort By Array Length

  cout << clear;
  printTitle("Movie Fields", applyBgColor, applyFgColor, false);
  printArray(fieldsStr, m, "Fields");

  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  printArray(sortByStr, n, "Sort By");

  pressEnterToCont("Press ENTER to Continue", false);

  sortMovies(movies, nMoviesRead, sortBy, movieSortByEnd / 2); // Sort Movies
  printMovies(movies, nMoviesRead, fields);                    // Print Movies

  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Filter Movies
void filterMovies(Movie movies[], int nMoviesRead, string **params, int counter[], int sortBy[])
{
  int l = movieFieldEnd - 1, m = maxParamPerSubCmd, n = movieSortByEnd / 2;
  bool fields[l];
  string sortByStr[n];

  fill(fields, fields + l, true); // Movie Fields to Print (All)
  n = getMovieSortByStr(sortBy, sortByStr, n);

  cout << clear;
  printTitle("Movie Fields Parameters", applyBgColor, applyFgColor, false);
  print2DArray(params, l, m, movieFieldCmdsStrPtr);

  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  printArray(sortByStr, n, "Sort By");

  pressEnterToCont("Press ENTER to Continue", false);

  filterMovies(movies, nMoviesRead, params, counter, fields, sortBy); // Filter Movies
  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Check Movie Rent Status
void getMovieStatus(Movie movies[], int nMoviesRead)
{
  int id, movieStatus, index;
  string temp, message;

  cout << clear;
  printTitle("Get Movie Status", applyBgColor, applyFgColor, false);

  cout << '\n';
  while (true) // Get Movie ID
    try
    {
      cout << "ID: ";
      getline(cin, temp);
      id = stoi(temp);
      break;
    }
    catch (...)
    {
      wrongMovieData(invalidMovieId);
    }

  movieStatus = checkMovieStatusById(movies, nMoviesRead, id, &index);

  cout << '\n';
  if (movieStatus == movieNotFound)
    message = "Error 404: Movie Not Found. Run \"Add Movie Command\"";
  else if (movieStatus == movieNotRented)
    message = "Movie Found: Avalaible for Rent";
  else if (movieStatus == movieRented)
    message = "Movie Found: Already Rented";

  if (movieStatus != movieNotFound)
    printMovieInfo(movies[index]);

  pressEnterToCont(message, false);
}

// Function to Filter Clients
void searchClient(Client clients[], int nClientsRead, string **params, int counter[], int sortBy[])
{
  int l = clientFieldEnd, m = maxParamPerSubCmd, n = clientSortByEnd / 2, nClientsFiltered;
  bool fields[l];
  string sortByStr[n];

  fill(fields, fields + l, true); // Client Fields to Print (All)
  n = getClientSortByStr(sortBy, sortByStr, n);

  cout << clear;
  printTitle("Client Fields Parameters", applyBgColor, applyFgColor, false);
  print2DArray(params, l, m, clientFieldCmdsStrPtr);
  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  printArray(sortByStr, n, "Sort By");

  pressEnterToCont("Press ENTER to Continue", false);

  filterClients(clients, nClientsRead, params, counter, fields, sortBy); // Filter Clients
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
  for (int i = 0; i < movieFieldEnd - 1; i++)
    cout << tab1 << setw(nCharTitle) << setfill(' ') << addBrackets(movieFieldCmdsPtr[i]) << movieFieldCmdsStrPtr[i] << '\n';
  cout << '\n';

  printTitle("Field as a Command (for Filter Movies)", applyBgColor, applyFgColor, false);
  for (int i = 0; i < movieFieldEnd - 1; i++)
    if (movieValidFieldFilterPtr[i])
    {
      temp = addBrackets(movieFieldCmdsPtr[i]).append(" [param...]");
      cout << tab1 << setw(nCharTitle) << setfill(' ') << temp << "Parameters for Movie's " << movieFieldCmdsStrPtr[i] << '\n';
    }
  cout << '\n';

  printTitle("Valid Field Parameters (for Filter Movies)", applyBgColor, applyFgColor, false);
  validParameters(nCharTitle);

  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Print Movie and Client Sort By Parameters
void sortByParameters()
{
  string ascending, descending;

  cout << clear; // Clear Terminal
  printTitle("Movie Sort By Parameters", applyBgColor, applyFgColor, false);
  for (int i = 0; i < movieSortByEnd / 2; i++)
  {
    ascending = addBrackets(movieSortByCmdsPtr[i * 2]).append(" Ascending");
    descending = addBrackets(movieSortByCmdsPtr[i * 2 + 1]).append(" Descending");

    cout << tab1 << setw(nCharTitle) << setfill(' ') << movieSortByCmdsStrPtr[i]
         << setw(nCharTitle) << setfill(' ') << ascending
         << setw(nCharTitle) << setfill(' ') << descending << '\n';
  }
  cout << '\n';

  printTitle("Client Sort By Parameters", applyBgColor, applyFgColor, false);
  for (int i = 0; i < clientSortByEnd / 2; i++)
  {
    ascending = addBrackets(clientSortByCmdsPtr[i * 2]).append(" Ascending");
    descending = addBrackets(clientSortByCmdsPtr[i * 2 + 1]).append(" Descending");

    cout << tab1 << setw(nCharTitle) << setfill(' ') << clientSortByCmdsStrPtr[i]
         << setw(nCharTitle) << setfill(' ') << ascending
         << setw(nCharTitle) << setfill(' ') << descending << '\n';
  }
  cout << '\n';

  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Print Search Client as a Command
void clientParameters()
{
  string temp;

  cout << clear;
  printTitle("Search Client as a Command", applyBgColor, applyFgColor, false);
  for (int i = 0; i < clientFieldEnd; i++)
  {
    temp = addBrackets(clientFieldCmdsPtr[i]).append(" [param...]");
    cout << tab1 << setw(nCharTitle) << setfill(' ') << temp << "Parameters for Client's " << clientFieldCmdsStrPtr[i] << '\n';
  }
  cout << '\n';

  printTitle("Valid Client Parameters", applyBgColor, applyFgColor, false);
  validParameters(nCharTitle);

  pressEnterToCont("Press ENTER to Continue", false);
}

// Function that Prints the Valid Genres when Adding a New Movie
void validGenres()
{
  string temp;

  printTitle("Movies Parameters", applyBgColor, applyFgColor, false);

  cout << '\n';
  printArray(genrePtr, genreEnd - 1, "Genre");
}

// Function that Prints Examples of View Movies Command
void howToUseViewMovies()
{
  const int nCmds = 2; // Number of Code Examples
  string cmds[nCmds] = {"v -f . -s i",
                        "v -f g p r -s I p"};

  string explanations[nCmds] = {"View All Movie Fields. Sort them by Id in Ascending Order",
                                "View Genre, Price and Release Date Movie Field. Sort them by Id in Descending Order and by Price in Ascending Order"};

  printExamples(cmds, explanations, nCmds);

  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

void howToUseFilterMovies()
{
  const int nCmds = 4; // Number of Code Examples
  string cmds[nCmds] = {"f -f --g Horror Fiction -s i",
                        "f -f --p 20 -s P",
                        "f -f --i 101 --g Thriller Mistery -s R",
                        "f -f --d \"David Quiroz\" -s t"};

  string explanations[nCmds] = {
      "Search for Movies that their Genre is Horror and Fiction. Sort them by Id in Ascending Order",
      "Search for Movies that their Price is 20 or less. Sort them by Price in Descending Order",
      "Search for Movie with Id 101 and Movies which has as Genre Either Thriller or Mistery. Sort them by Release Date in Descending Order",
      "Search for Movies that their Director is David Quiroz. Sort them by Title in Ascending Order"};

  printExamples(cmds, explanations, nCmds);

  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

void howToUseSearchClient()
{
  const int nCmds = 3; // Number of Code Examples
  string cmds[nCmds] = {"c -f --i 101 --n Ramon -s i",
                        "c -f --n \"Andres Avila\" \"Ronald Lopez\" Ivana Grecia -s n",
                        "c -f --n  Roberto --i 123456789 -s N"};

  string explanations[nCmds] = {
      "Search for Clients Named as Ramon and the Client with Id 101. Sort them by Id in Ascending Order",
      "Search for Clients Named as Andres Avila, Ronald Lopez, Ivana and Grecia. Sort them by Name in Ascending Order",
      "Search for Clients whose Name is Roberto and the Client whose Id is 123456789. Sort them by Name in Descending Order"};

  printExamples(cmds, explanations, nCmds);

  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Add Some Clients to clients.csv
void addClient(Client clients[], int *nClientsRead)
{
  while (true)
  {
    cout << clear;
    printTitle("Add Client", applyBgColor, applyFgColor, false);

    cout << '\n';
    addClientToFile(clients, nClientsRead);

    if (!booleanQuestion("Do you want to Add more Clients?"))
      break;
  }
}

// Function to Get a String Array from a Int Array of the Sort By Commands that will be Applied to the Movies
int getMovieSortByStr(int sortBy[], string sortByStr[], int n)
{
  bool nullParam;
  int charIndex, nParams = 0;
  string order;

  for (int j = 0; j < n; j++)
  {
    charIndex = sortBy[j];
    nullParam = false;

    if (charIndex == -1)
      nullParam = true;
    else if (isupper(movieSortByCmdsPtr[charIndex]))
      order = "[D] ";
    else
      order = "[A] ";

    if (!nullParam)
      sortByStr[nParams++] = order.append(movieSortByCmdsStrPtr[charIndex / 2]); // Data to Print in the Sort By Parameters Row
  }
  return nParams;
}

// Function to Get a String Array from a Int Array of the Sort By Commands that will be Applied to the Clients
int getClientSortByStr(int sortBy[], string sortByStr[], int n)
{
  bool nullParam;
  int charIndex, nParams = 0;
  string order;

  for (int j = 0; j < n; j++)
  {
    charIndex = sortBy[j];
    nullParam = false;

    if (charIndex == -1)
      nullParam = true;
    else if (isupper(clientSortByCmdsPtr[charIndex]))
      order = "[D] ";
    else
      order = "[A] ";

    if (!nullParam)
      sortByStr[nParams++] = order.append(clientSortByCmdsStrPtr[charIndex / 2]); // Data to Print in the Sort By Parameters Row
  }
  return nParams;
}