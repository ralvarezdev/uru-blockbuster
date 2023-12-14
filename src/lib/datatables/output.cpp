#include <iomanip>
#include <iostream>
#include <string>
#include <cstring>

// #define NDEBUG
#include <assert.h>

#include "output.h"
#include "../terminal/ansiEsc.h"
#include "../namespaces.h"

using namespace std;
using namespace clients;
using namespace commands;
using namespace movies;
using namespace terminal;

// --- Extern Variables Declaration
extern char *genreStr[], *movieFieldCmdsStr[], *clientFieldCmdsStr[];

// --- Function Prototypes
void printExamples(cmdExplanation examples[], int n);
void printMessageMaxPerLine(string message, int nCharLine);
void printDate(int date[]);
void addDateToStream(int date[], ostringstream *stream);
void printMovies(Movies *movies, bool *fields);
void printMovieInfo(Movie movie);
void printClientInfo(Client client, bool censoreInfo);
void printClients(Clients *clients, bool *fields);

// --- Functions For Output Styling

// Function to Print the Commands and its Explanations while Taking Care of nChar
void printExamples(cmdExplanation examples[], int n)
{
  const int nCharLine = nChar - tab1.length(); // Number of Characters for Each Line

  cout << clear;                                             // Clear Terminal
  printTitle("Examples", applyBgColor, applyFgColor, false); // Usage Examples of the Search Command
  for (int i = 0; i < n; i++)
  {
    cout << '\n';
    printMessageMaxPerLine(examples[i].cmd, nCharLine);
    cout << tab1 << string(nCharTitle, '-') << '\n'; // Separator

    printMessageMaxPerLine(examples[i].explanation, nCharLine);
  }
}

// Function to Print Messages with New Line each time it Reaches nCharLine
void printMessageMaxPerLine(string message, int nCharLine)
{
  string content;
  int printedChar = 0, contentLength; // Number of Characters Printed in the Current Line and the Length of Content String

  stringstream stream(message); // Stream Message

  while (getline(stream, content, ' '))
  {
    contentLength = content.length();

    if (printedChar + contentLength >= nCharLine) // Exceeds the Maximum Number of Characters per Line
    {
      cout << '\n'
           << tab1 << content;
      printedChar = contentLength;
      continue;
    }

    if (printedChar != 0)
    {
      cout << ' ' << content;
      printedChar++;
    }
    else
      cout << tab1 << content;

    printedChar += contentLength;
  }
  cout << '\n';
}

// Function to Print Movie Info
void printMovieInfo(Movie movie)
{
  bool rented = (movie.rentStatus);
  int nCharContent = nChar - nCharField;
  string fieldTitle;

  ostringstream stream;

  printTitle("Movie Info", applyBgColor, applyFgColor, false);

  stream << left
         << setw(nCharField) << setfill(' ') << movieFieldCmdsStr[movieFieldId] << movie.id << '\n'; // Print ID

  stream << setw(nCharField) << setfill(' ') << movieFieldCmdsStr[movieFieldName]; // Print Title
  if (movie.name.length() < nCharContent)
    stream << setw(nCharContent) << setfill(' ') << movie.name << '\n';
  else
    stream << movie.name.substr(0, nCharContent - 4) << "... " << '\n';

  stream << setw(nCharField) << setfill(' ') << movieFieldCmdsStr[movieFieldGenre]; // Print Genres
  for (int i = 0; i < nMaxGenres && movie.genres[i] != -1; i++)
    if (i != 0)
      stream << ", " << genreStr[movie.genres[i]];
    else
      stream << genreStr[movie.genres[i]];
  stream << '\n';

  stream << setw(nCharField) << setfill(' ') << movieFieldCmdsStr[movieFieldDirector] << movie.director << '\n' // Print Director
         << setw(nCharField) << setfill(' ') << movieFieldCmdsStr[movieFieldDuration] << movie.duration << '\n' // Print Duration
         << setw(nCharField) << setfill(' ') << movieFieldCmdsStr[movieFieldPrice] << movie.price << '\n'       // Print Price
         << setw(nCharField) << setfill(' ') << movieFieldCmdsStr[movieFieldRelease];
  addDateToStream(movie.releaseDate, &stream); // Print Release Date
  stream << '\n';

  stream << setw(nCharField) << setfill(' ') << movieFieldCmdsStr[movieFieldStatus]; // Print Rent Status
  if (!rented)
    stream << "No" << '\n';
  else
  {
    stream << "Yes" << '\n'
           << setw(nCharField) << setfill(' ') << movieFieldCmdsStr[movieFieldRentTo] << movie.rentTo << '\n' // Print Client ID that Rented the Movie
           << setw(nCharField) << setfill(' ') << movieFieldCmdsStr[movieFieldRentOn];                        // Print Rent On Date
    addDateToStream(movie.rentOn, &stream);
    stream << '\n';
  }
  cout << stream.str();
}

// Function to Print Movies
void printMovies(Movies *movies, bool *fields)
{
  // Print Movies
  ostringstream stream;
  Movie movie;

  const int nPerGenre = 3;                    // Number of Characters Per Genre
  const int nId = 7;                          // ... for ID
  const int nDuration = 4;                    // ... for Duration
  const int nPrice = 8;                       // ... for Price
  const int nGenre = (nPerGenre + 1) * 4 + 1; // ... for Genres
  const int nStatus = 7;                      // ... for Rent Status
  const int nRentTo = 8;                      // ... for Rent to Client (ID)
  const int nDirector = 15;                   // ... for Director Name

  int nTitle = nChar; // Decrease the Number of Characters Used by Title Field
  int n = movieFieldEnd - 1;

  int fieldsNChar[n] = {nId, 0, nDirector, nGenre, nDuration, nPrice,
                        nDate, nStatus, nDate, nRentTo}; // Number of Characters per Field

  for (int i = 0; i < n; i++)
    if (fields[i] && i != movieFieldName)
      nTitle -= fieldsNChar[i]; // Decrease Number of Characters for Movie's Name Field

  assert(nTitle > 0);                   // Check nName
  fieldsNChar[movieFieldName] = nTitle; // Assign Number of Characters for Movie's Name

  stream << clear << sgrBgCmd << sgrFgCmd << left;
  for (int i = 0; i < n; i++)
    if (fields[i])
      stream << setw(fieldsNChar[i]) << setfill(' ') << movieFieldCmdsStr[i]; // Field Title
  stream << reset << '\n';

  if ((*movies).getNumberMovies() == 0) // No Movies
  {
    stream << string(nChar, '-') << '\n';
    cout << stream.str();
    return; // End this Function
  }

  stream << left;
  for (int i = 0; i < (*movies).getNumberMovies(); i++)
  {
    // Get Movie
    movie = (*movies).getMovie(i);

    // Movie ID
    if (fields[movieFieldId])
      stream << setw(nId) << setfill(' ') << movie.id;

    // Movie Title
    if (fields[movieFieldName])
      if (movie.name.length() < nTitle)
        stream << setw(nTitle) << setfill(' ') << movie.name;
      else
        stream << movie.name.substr(0, nTitle - 4) << "... ";

    // Movie Director
    if (fields[movieFieldDirector])
    {
      if (movie.director.length() < nDirector)
        stream << setw(nDirector) << setfill(' ') << movie.director;
      else
        stream << movie.director.substr(0, nDirector - 4) << "... ";
    }

    // Movie Genres
    if (fields[movieFieldGenre])
    {
      if (movie.genres[0] == noneGenre)
        stream << string(nGenre - 1, '-') << ' '; // No Genre
      else
      {
        string genres, genre, temp;
        bool firstIter = true; // First Iteration

        for (int j = 0; j < nMaxGenres && movie.genres[j] != -1; j++)
        {
          if (!firstIter)
            genre = ',';
          else
            firstIter = false;

          temp = genreStr[movie.genres[j]];        // Convert Char Array to String
          genre.append(temp.substr(0, nPerGenre)); // Substract the First N Characters
          genres.append(genre);
        }

        if (genres.length() < nGenre)
          stream << setw(nGenre) << setfill(' ') << genres;
        else
          stream << setw(nGenre - 4) << setfill(' ') << genres.substr(0, nGenre - 4) << "... ";
      }
    }

    // Movie Duration
    if (fields[movieFieldDuration])
      stream << setw(nDuration) << setfill(' ') << movie.duration;

    // Movie Price
    if (fields[movieFieldPrice])
      stream << '$' << setw(nPrice - 1) << setfill(' ') << movie.price;

    // Movie Release Date
    if (fields[movieFieldRelease])
      addDateToStream(movie.releaseDate, &stream);

    // Movie Rent Status
    if (fields[movieFieldStatus])
      if (!movie.rentStatus)
        stream << setw(nStatus) << setfill(' ') << "No";
      else
      {
        stream << setw(nStatus) << setfill(' ') << "Yes";
        addDateToStream(movie.rentOn, &stream);
        stream << setw(nRentTo) << setfill(' ') << left << movie.rentTo;
      }
    stream << '\n';
  }
  cout << stream.str();
}

// Function to Print Dates
void printDate(int date[])
{
  cout << right
       << setw(nYear) << setfill('0') << date[0] << '-'
       << setw(nMonth) << setfill('0') << date[1] << '-'
       << setw(nDay) << setfill('0') << date[2] << string(nSep, ' ')
       << left;
}

// Function to Add Dates to ostringstream
void addDateToStream(int date[], ostringstream *stream)
{
  *stream << right
          << setw(nYear) << setfill('0') << date[0] << '-'
          << setw(nMonth) << setfill('0') << date[1] << '-'
          << setw(nDay) << setfill('0') << date[2] << string(nSep, ' ')
          << left;
}

// Function to Print Client Info
void printClientInfo(Client client, bool censoreInfo)
{
  string name = client.name; // Client Name
  int nCharContent = nChar - nCharField;

  ostringstream stream;

  printTitle("Client Info", applyBgColor, applyFgColor, false);

  stream << left
         << setw(nCharField) << setfill(' ') << clientFieldCmdsStr[clientFieldAccountNumber] << client.account << '\n'; // Print Client ID

  stream << setw(nCharField) << setfill(' ') << clientFieldCmdsStr[clientFieldId] << client.id << '\n'; // Print Client ID

  stream << setw(nCharField) << setfill(' ') << clientFieldCmdsStr[clientFieldName]; // Print Client Name
  if (name.length() < nCharContent)
    stream << setw(nCharContent) << setfill(' ') << name << '\n';
  else
    stream << name.substr(0, nCharContent - 4) << "... " << '\n';

  if (!censoreInfo)
  {
    stream << setw(nCharField) << setfill(' ') << clientFieldCmdsStr[clientFieldPhoneNumber] // Print Client Phone Number
           << setw(nCharContent) << setfill(' ') << toStringWithPrecision(client.phoneNumber, 0) << '\n';
  }
  cout << stream.str();
}

// Function to Print Clients
void printClients(Clients *clients, bool *fields)
{
  // Print Clients
  ostringstream stream;
  Client client;
  string name; // Client Name

  const int nAccountNumber = 20; // Number of Characters for Account Number
  const int nId = 15;            // ... for ID
  const int nPhoneNumber = 15;   // ... for Phone Number

  int nName = nChar; // Decrease the Number of Characters Used by the Name Field
  int n = clientFieldEnd - 1;

  int fieldsNChar[n] = {nAccountNumber, nId, 0, nPhoneNumber}; // Number of Characters per Field

  for (int i = 0; i < n; i++)
    if (fields[i] && i != clientFieldName)
      nName -= fieldsNChar[i]; // Decrease Number of Characters for Client's Name Field

  assert(nName > 0); // Check nName
  fieldsNChar[clientFieldName] = nName;

  stream << clear << sgrBgCmd << sgrFgCmd << left;
  for (int i = 0; i < n; i++)
    if (fields[i])
      stream << setw(fieldsNChar[i]) << setfill(' ') << clientFieldCmdsStr[i]; // Field Title
  stream << reset << '\n';

  if ((*clients).getNumberClients() == 0) // No Clients
  {
    stream << string(nChar, '-') << '\n';
    cout << stream.str();
    return; // End this Function
  }

  for (int i = 0; i < (*clients).getNumberClients(); i++)
  {
    client = (*clients).getClient(i); // Get Client
    name = client.name;               // Get Client Name as a String

    // Client Account Number
    if (fields[clientFieldAccountNumber])
      stream << setw(nAccountNumber) << setfill(' ') << client.account;

    // Client ID
    if (fields[clientFieldId])
      stream << setw(nId) << setfill(' ') << client.id;

    // Client Title
    if (fields[clientFieldName])
      if (name.length() < nName)
        stream << setw(nName) << setfill(' ') << name;
      else
        stream << name.substr(0, nName - 4) << "... ";

    // Client Phone Number
    if (fields[clientFieldPhoneNumber])
      stream << setw(nPhoneNumber) << setfill(' ') << toStringWithPrecision(client.phoneNumber, 0);

    stream << '\n';
  }
  cout << stream.str();
}