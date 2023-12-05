#include <iomanip>
#include <iostream>
#include <string>
#include <cstring>

#include "../terminal/ansiEsc.h"
#include "../namespaces.h"

using namespace std;
using namespace clients;
using namespace commands;
using namespace movies;
using namespace terminal;

// --- Extern Variables Declaration
extern string *genrePtr, *movieFieldCmdsStrPtr, *clientFieldCmdsStrPtr;

// --- Function Prototypes
void printExamples(string cmds[], string explanations[], int n);
void printArray(string *params, int m, string paramTitle);
void print2DArray(string **params, int m, int n, string paramsTitle[]);
void printDate(int date[], bool rightJustified);
void addDateToStream(int date[], bool rightJustified, ostringstream *stream);
void printMovies(Movie movies[], int m, bool *fields);
void printClientInfo(Client client, bool censoreInfo);
void printClients(Client clients[], int m, bool *fields);

// --- Functions For Output Styling

// Function to Print the Commands and its Explanations while Taking Care of nChar
void printExamples(string cmds[], string explanations[], int n)
{
  const int nCharLine = nChar - tab1.length(); // Number of Characters for Each Line

  int printedChar, messageLength; // Number of Characters Printed in the Current Line and the Length of Message String
  string message;                 // Temp Data

  cout << clear;                                             // Clear Terminal
  printTitle("Examples", applyBgColor, applyFgColor, false); // Examples of the Usage of the Search Command
  for (int i = 0; i < n; i++)
  {
    printedChar = 0;

    for (int j = 0; j < examples[i].cmd.length(); j += nCharLine)
    {
      message = examples[i].cmd.substr(j, nCharLine + j);
      assert(message.length() <= nCharLine); // Check message String Length

      cout << '\n'
           << tab1 << message << '\n';
    }

    cout << tab1 << string(nCharTitle, '-') << '\n';

    stringstream stream(explanations[i]); // To Print the Message with New Line each time it Reaches nCharLine
    
    while (getline(stream, message, ' '))
    {
      messageLength = message.length();

      if (printedChar + messageLength >= nCharLine)
      { // Exceeds the Maximum Number of Characters per Line
        cout << '\n'
             << tab1 << message;
        printedChar = messageLength;
        continue;
      }

      if (printedChar != 0)
      {
        cout << ' ' << message;
        printedChar++;
      }
      else
        cout << tab1 << message;

      printedChar += messageLength;
    }
    cout << '\n';
  }
}

// Function to Print a 1D Array
void printArray(string *params, int n, string paramTitle)
{
  string param;

  cout << setw(nCharTitle) << setfill(' ') << left << paramTitle;
  for (int i = 0; i < n && params[i] != "null"; i++)
  {
    param = params[i];

    if (i % paramPerLine == 0 && i != 0)
      cout << '\n' // Break the Line, and Add Some Indentation if there's more than N Parameters on the Current Line
           << string(nCharTitle, ' ');

    if (param.length() <= nCharParam - maxSpacing)
    {
      cout << setw(nCharParam) << setfill(' ') << left << param;
      continue;
    }

    param = param.substr(0, nCharParam - maxSpacing); // Substract from String
    param.append("...");                              // Append to the String
    cout << setw(nCharParam) << setfill(' ') << left << param;
  }
  cout << "\n\n";
}

// Function to Print a 2D Data Table
void print2DArray(string **params, int m, int n, string paramsTitle[])
{
  for (int i = 0; i < m; i++)
    if (params[i][0] != "null")
      printArray(params[i], n, paramsTitle[i]);
}

// Function to Print Movie Info
void printMovieInfo(Movie movie)
{
  bool rented = (movie.rentStatus == 1);
  int nCharContent = nChar - nCharField;

  printTitle("Movie Info", applyBgColor, applyFgColor, false);

  cout << setw(nCharField) << setfill(' ') << movieFieldCmdsStrPtr[movieFieldId] << movie.id << '\n'; // Print Id

  cout << setw(nCharField) << setfill(' ') << movieFieldCmdsStrPtr[movieFieldName]; // Print Title
  if (movie.name.length() < nCharContent)
    cout << setw(nCharContent) << setfill(' ') << movie.name << '\n';
  else
    cout << movie.name.substr(0, nCharContent - 4) << "... " << '\n';

  cout << setw(nCharField) << setfill(' ') << movieFieldCmdsStrPtr[movieFieldGenre]; // Print Genres
  for (int i = 0; i < nMaxGenres && movie.genres[i] != -1; i++)
    if (i != 0)
      cout << ", " << genrePtr[movie.genres[i]];
    else
      cout << genrePtr[movie.genres[i]];

  cout << '\n'
       << setw(nCharField) << setfill(' ') << movieFieldCmdsStrPtr[movieFieldDirector] << movie.director << '\n' // Print Director
       << setw(nCharField) << setfill(' ') << movieFieldCmdsStrPtr[movieFieldDuration] << movie.duration << '\n' // Print Duration
       << setw(nCharField) << setfill(' ') << movieFieldCmdsStrPtr[movieFieldPrice] << movie.price << '\n'       // Print Price
       << setw(nCharField) << setfill(' ') << movieFieldCmdsStrPtr[movieFieldRelease];
  printDate(movie.releaseDate, false); // Print Release Date
  cout << '\n';

  cout << setw(nCharField) << setfill(' ') << movieFieldCmdsStrPtr[movieFieldStatus]; // Print Rent Status
  if (!rented)
    cout << "No" << '\n';
  else
  {
    cout << "Yes" << '\n'
         << setw(nCharField) << setfill(' ') << movieFieldCmdsStrPtr[movieFieldRentTo] << movie.rentTo << '\n' // Print Client Id that Rented the Movie
         << setw(nCharField) << setfill(' ') << movieFieldCmdsStrPtr[movieFieldRentOn];                        // Print Rent On Date
    printDate(movie.rentOn, false);
    cout << '\n';
  }
}

// Function to Print Movies
void printMovies(Movie movies[], int m, bool *fields)
{
  const int nPerGenre = 3;                    // Number of Characters Per Genre
  const int nId = 5;                          // ... for Id
  const int nDuration = 4;                    // ... for Duration
  const int nPrice = 8;                       // ... for Price
  const int nGenre = (nPerGenre + 1) * 4 + 1; // ... for Genres
  const int nStatus = 7;                      // ... for Rent Status
  const int nRentTo = 8;                      // ... for Rent to Client (Id)
  const int nDirector = 15;                   // ... for Director Name

  int nTitle = nChar; // Decrease the Number of Characters Used by the Title Field
  int n = movieFieldEnd - 1;

  int fieldsNChar[n] = {nId, 0, nDirector, nGenre, nDuration, nPrice,
                        nDate, nStatus, nDate, nRentTo}; // Number of Characters per Field

  for (int i = 0; i < n; i++)
    if (fields[i] && i != movieFieldName)
      nTitle -= fieldsNChar[i]; // Decrease Number of Characters for Movie's Name Field

  fieldsNChar[movieFieldName] = nTitle; // Assign Number of Characters for Movie's Name

  cout << clear << sgrBgCmd << sgrFgCmd;
  for (int i = 0; i < n; i++)
    if (fields[i])
      cout << setw(fieldsNChar[i]) << setfill(' ') << movieFieldCmdsStrPtr[i]; // Field Title
  cout << reset << '\n';

  // Print Movies
  int *date;

  ostringstream stream;
  stream << left;
  for (int i = 0; i < m; i++)
  {
    // Movie Id
    if (fields[movieFieldId])
      stream << setw(nId) << setfill(' ') << movies[i].id;

    // Movie Title
    if (fields[movieFieldName])
      if (movies[i].name.length() < nTitle)
        stream << setw(nTitle) << setfill(' ') << movies[i].name;
      else
        stream << movies[i].name.substr(0, nTitle - 4) << "... ";

    // Movie Director
    if (fields[movieFieldDirector])
    {
      if (movies[i].director.length() < nDirector)
        stream << setw(nDirector) << setfill(' ') << movies[i].director;
      else
        stream << movies[i].director.substr(0, nDirector - 4) << "... ";
    }

    // Movie Genres
    if (fields[movieFieldGenre])
    {
      if (movies[i].genres[0] == noneGenre)
        stream << string(nGenre - 1, '-') << ' '; // No Genre
      else
      {
        string genres, genre;
        bool firstIter = true; // First Iteration

        for (int j = 0; j < nMaxGenres && movies[i].genres[j] != -1; j++)
        {
          if (!firstIter)
            genre = ',';
          else
            firstIter = false;

          genre.append(genrePtr[movies[i].genres[j]].substr(0, nPerGenre)); // Substract the First N Characters
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
      stream << setw(nDuration) << setfill(' ') << movies[i].duration;

    // Movie Price
    if (fields[movieFieldPrice])
      stream << '$' << setw(nPrice - 1) << setfill(' ') << movies[i].price;

    // Movie Release Date
    if (fields[movieFieldRelease])
      addDateToStream(movies[i].releaseDate, true, &stream);

    // Movie Rent Status
    if (fields[movieFieldStatus])
      if (!movies[i].rentStatus)
        stream << setw(nStatus) << setfill(' ') << "No";
      else
      {
        stream << setw(nStatus) << setfill(' ') << "Yes";
        addDateToStream(movies[i].rentOn, true, &stream);
        stream << setw(nRentTo) << setfill(' ') << left << movies[i].rentTo;
      }
    stream << '\n';
  }
  cout << stream.str();
}

// Function to Print Dates
void printDate(int date[], bool rightJustified)
{
  if (rightJustified)
    cout << setw(nYear) << setfill('0') << right << date[0] << '-'
         << setw(nMonth) << setfill('0') << date[1] << '-'
         << setw(nDay) << setfill('0') << date[2] << string(nSep, ' ') << left;
  else
    cout << setw(nYear) << setfill('0') << date[0] << '-'
         << setw(nMonth) << setfill('0') << date[1] << '-'
         << setw(nDay) << setfill('0') << date[2] << string(nSep, ' ');
}

// Function to Add Dates to ostringstream
void addDateToStream(int date[], bool rightJustified, ostringstream *stream)
{
  if (rightJustified)
    *stream << setw(nYear) << setfill('0') << right << date[0] << '-'
            << setw(nMonth) << setfill('0') << date[1] << '-'
            << setw(nDay) << setfill('0') << date[2] << string(nSep, ' ') << left;
  else
    *stream << setw(nYear) << setfill('0') << date[0] << '-'
            << setw(nMonth) << setfill('0') << date[1] << '-'
            << setw(nDay) << setfill('0') << date[2] << string(nSep, ' ');
}

// Function to Print Client Info
void printClientInfo(Client client, bool censoreInfo)
{
  string name = client.name;       // Client Name
  string address = client.address; // Client Address

  int nCharContent = nChar - nCharField;

  printTitle("Client Info", applyBgColor, applyFgColor, false);

  cout << setw(nCharField) << setfill(' ') << clientFieldCmdsStrPtr[clientFieldId] << client.id << '\n'; // Print Client Id

  cout << setw(nCharField) << setfill(' ') << clientFieldCmdsStrPtr[clientFieldName]; // Print Client Name
  if (name.length() < nCharContent)
    cout << setw(nCharContent) << setfill(' ') << name << '\n';
  else
    cout << name.substr(0, nCharContent - 4) << "... " << '\n';

  if (!censoreInfo)
  {
    cout << setw(nCharField) << setfill(' ') << clientFieldCmdsStrPtr[clientFieldPhoneNumber] // Print Client Phone Number
         << setw(nCharContent) << setfill(' ') << client.phoneNumber << '\n'
         << setw(nCharField) << setfill(' ') << clientFieldCmdsStrPtr[clientFieldAddress]; // Print Client Address

    if (address.length() < nCharContent)
      cout << setw(nCharContent) << setfill(' ') << address << '\n';
    else
      cout << address.substr(0, nCharContent - 4) << "... " << '\n';
  }
}

// Function to Print Clients
void printClients(Client clients[], int m, bool *fields)
{
  string name;    // Client Name
  string address; // Client Address

  const int nId = 15;          // Number of Characters for Id
  const int nPhoneNumber = 15; // ... for Phone Number
  const int nAddress = 35;     // ... for Address

  int nName = nChar; // Decrease the Number of Characters Used by the Name Field
  int n = clientFieldEnd;

  int fieldsNChar[n] = {nId, 0, nPhoneNumber, nAddress}; // Number of Characters per Field

  for (int i = 0; i < n; i++)
    if (fields[i] && i != clientFieldName)
      nName -= fieldsNChar[i]; // Decrease Number of Characters for Client's Name Field

  fieldsNChar[clientFieldName] = nName;

  cout << clear << sgrBgCmd << sgrFgCmd;
  for (int i = 0; i < n; i++)
    if (fields[i])
      cout << setw(fieldsNChar[i]) << setfill(' ') << clientFieldCmdsStrPtr[i]; // Field Title
  cout << reset << '\n';

  // Print Clients
  string temp;

  ostringstream stream;
  stream << left;
  for (int i = 0; i < m; i++)
  {
    name = clients[i].name;       // Client Name
    address = clients[i].address; // Client Address

    // Client Id
    if (fields[clientFieldId])
      stream << setw(nId) << setfill(' ') << clients[i].id;

    // Client Title
    if (fields[clientFieldName])
      if (name.length() < nName)
        stream << setw(nName) << setfill(' ') << name;
      else
        stream << name.substr(0, nName - 4) << "... ";

    // Client Phone Number
    if (fields[clientFieldPhoneNumber])
      stream << setw(nPhoneNumber) << setfill(' ') << clients[i].phoneNumber;

    // Client Address
    if (fields[clientFieldAddress])
      if (address.length() < nName)
        stream << setw(nName) << setfill(' ') << address;
      else
        stream << address.substr(0, nName - 4) << "... ";

    stream << '\n';
  }
  cout << stream.str();
}