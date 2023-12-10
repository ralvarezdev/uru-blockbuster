#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

// #define NDEBUG
#include <assert.h>

#include "../namespaces.h"
#include "../datatables/output.h"
#include "../terminal/ansiEsc.h"
#include "../terminal/input.h"

using namespace std;
using namespace commands;
using namespace files;
using namespace movies;
using namespace terminal;

// --- Extern Variables Declaration
extern bool movieValidFieldsToFilter[];
extern char *genreStr[];

// --- Functions Prototypes
movieStatus getMovieId(Movies *movies, int *id, int *index, string message);
void getMovies(Movies *movies);
void addMovieToFile(Movies *movies);
movieStatus checkMovie(Movies *movies, int id, int *index);
void getGenres(string word, int genres[], int n);
int getGenreIndex(string genre);
int getGenreIndexLower(string genre);
void getDate(string word, int date[], int n);
void filterMoviesData(Movies *movies, string **params, bool fields[], int sortBy[]);
void addFilteredMovie(int filteredIds[], int nMoviesRead, int id, int *counter);
int binarySearch(int A[], int n, int num);
void insertionSortWithPivot(int A[], int n, int pivot);
void sortMovies(Movies *movies, int sortBy[], int n);
void moviesMergeSort(Movies *movies, int sortByIndex);
void moviesMerge(Movies *movies, Movie sorted[], int low, int mid, int high, int sortByIndex);
string getGenresStr(int genres[]);
string getDateStr(int date[3]);
string getCurrentDate(int date[]);

// Function to Get an Array of Movies from movies.csv
void getMovies(Movies *movies)
{
  int genres[genreEnd], date[3];
  string director[2];
  int nMovies, count;
  string line, word;

  ifstream moviesCSV(moviesFilename);

  if (!moviesCSV.is_open())
  { // Couldn't Access to moviesCSV
    moviesCSV.close();
    pressEnterToCont("Error: File Not Found. Press ENTER to go Back to Main Menu", true);
    return; // End this Function
  }

  while (getline(moviesCSV, line)) // Get Movies
    try
    {
      if (line.length() == 0)
        continue;

      fill(genres, genres + genreEnd, -1); // Initialize Genres Array

      stringstream file(line);
      Movie newMovie;

      count = 0;
      while (getline(file, word, sep))
      {
        if (word.length() != 0)
          switch (count)
          {
          case 0:
            newMovie.id = stoi(word);
            break;
          case 1:
            newMovie.name = word;
            break;
          case 2:
            getGenres(word, genres, nMaxGenres);
            for (int i = 0; i < genreEnd; i++)
              newMovie.genres[i] = genres[i];
            break;
          case 3:
            newMovie.duration = stoi(word);
            break;
          case 4:
            newMovie.director = word;
            break;
          case 5:
            newMovie.price = stof(word);
            break;
          case 6:
            getDate(word, date, 3);
            for (int i = 0; i < 3; i++)
              newMovie.releaseDate[i] = date[i];
            break;
          case 7:
            newMovie.rentTo = stoi(word); // Client ID
            break;
          case 8:
            getDate(word, date, 3);
            for (int i = 0; i < 3; i++)
              newMovie.rentOn[i] = date[i]; // Rent Date
            break;
          case 9:
            newMovie.rentStatus = stoi(word); // Rent Status (Yes for Rented)
            break;
          }
        count++;
      }

#ifndef NDEBUG
      nMovies = (*movies).getNumberMovies();
#endif

      (*movies).pushBack(newMovie);
      assert(nMovies == (*movies).getNumberMovies() - 1); // Checks if the Number of Movies Gets Increased
    }
    catch (...)
    {
      // It will Ignore the Line that was Read from movies.csv
    }

  moviesCSV.close();
}

// Function to Get Movie Genres
void getGenres(string word, int genres[], int n)
{
  int index, counter = 0;
  string genre;

  stringstream stream(word);
  while (getline(stream, genre, genreSep) && counter < n) // Get Genre
  {
    index = getGenreIndex(genre);
    if (index != -1)
      genres[counter++] = index;
  }
}

// Function to Get Movie Genre Index in the Array with the Different Genres
int getGenreIndex(string genre)
{
  for (int i = 0; i < genreEnd; i++)
    if (genre == genreStr[i] || i == errorGenre)
      return i;
    else
      continue;

  return -1;
}

// Function to Get Movie Genre Index in the Array with the Different Genres (Comparing the Genre in Lowercase)
int getGenreIndexLower(string genre)
{
  for (int i = 0; i < genreEnd; i++)
    if (getLower(genre) == getLower(genreStr[i]) || i == errorGenre)
      return i;
    else
      continue;

  return -1;
}

// Function to Get Date
void getDate(string word, int date[], int n)
{
  int counter = 0;
  string temp;

  stringstream stream(word);
  while (getline(stream, temp, dateSep) && counter < n) // Get Date
    date[counter++] = stoi(temp);
}

// Function that Returns Movie Indexes that Matched with the Parameters
void filterMoviesData(Movies *movies, string **params, bool fields[], int sortBy[])
{
  movieStatus movieStatus;
  Movie movie;
  int i, j, id, compare, index, genreIndex, sortByInt, counter = 0, nMoviesRead = (*movies).getNumberMovies();
  string compareLower, nameLower;

  assert(nMoviesRead > 0); // Check if the Number of Movies is Greater than 0

  int *filteredIds = new int[nMoviesRead];          // Allocate Memory
  fill(filteredIds, filteredIds + nMoviesRead, -1); // Fill Array with -1 Values

  for (int field = 0; field < movieFieldEnd - 1; field++)
  {
    if (!movieValidFieldsToFilter[field]) // Check if the Function can Filter that Field
      continue;

    for (int param = 0; param < maxParamPerSubCmd && params[field][param] != "null"; param++)
    {
      switch (field)
      {
      case movieFieldId:
        id = stoi(params[field][param]);
        movieStatus = checkMovie(movies, id, &index); // Binary Search

        // Checks if the Movie has Already being Filtered
        if (movieStatus != movieNotFound && binarySearch(filteredIds, nMoviesRead, id) == -1)
          addFilteredMovie(filteredIds, nMoviesRead, id, &counter);

      case movieFieldName:
      case movieFieldDirector:
        compareLower = getLower(params[field][param]); // Get Movie Name or Director To Search for in Lowercase

        // Checks if the Movie Name or Director Name in Lowercase Contains the Parameter that is being Searched by Linear Search
        for (i = 0; i < nMoviesRead && counter != nMoviesRead; i++)
        {
          movie = (*movies).getMovie(i);

          nameLower = (field == movieFieldName) ? getLower(movie.name) : getLower(movie.director);

          if (binarySearch(filteredIds, nMoviesRead, movie.id) == -1 && nameLower.find(compareLower) != string::npos)
            addFilteredMovie(filteredIds, nMoviesRead, movie.id, &counter);
        }
        break;

      case movieFieldDuration:
      case movieFieldPrice:
        compare = stoi(params[field][param]); // Integer to Compare, it Could be Either the Duration or the Price
        sortByInt = (field == movieFieldDuration) ? movieFieldDuration * 2 : movieFieldPrice * 2;

        moviesMergeSort(movies, sortByInt); // Sort Movies by Either the Duration or the Price

        if (field == movieFieldDuration)
          for (i = 0; i < nMoviesRead && (*movies).getMovie(i).duration <= compare && counter != nMoviesRead; i++)
          {
            movie = (*movies).getMovie(i);

            if (binarySearch(filteredIds, nMoviesRead, movie.id) == -1)
              addFilteredMovie(filteredIds, nMoviesRead, movie.id, &counter);
          }
        else if (field == movieFieldPrice)
          for (i = 0; i < nMoviesRead && movie.price <= compare && counter != nMoviesRead; i++)
          {
            movie = (*movies).getMovie(i);

            if (binarySearch(filteredIds, nMoviesRead, movie.id) == -1)
              addFilteredMovie(filteredIds, nMoviesRead, movie.id, &counter);
          }
        break;

      case movieFieldGenre:
        genreIndex = getGenreIndex(params[field][param]); // Get Genre Index in Type of Genres Array

        for (i = 0; i < nMoviesRead && counter != nMoviesRead; i++)
        {
          movie = (*movies).getMovie(i);

          for (j = 0; j < nMaxGenres && movie.genres[j] != -1; j++)
            if (movie.genres[j] == genreIndex) // Coincidence
              if (binarySearch(filteredIds, nMoviesRead, movie.id) == -1)
              {
                addFilteredMovie(filteredIds, nMoviesRead, movie.id, &counter);
                break;
              }
        }
        break;

        // case movieFieldRelease: // NOT DEFINED
      }

      if (counter == nMoviesRead)
        break; // Max Number of Coincidences Reached
    }

    if (counter == nMoviesRead)
      break; // Max Number of Coincidences Reached
  }

  /* // For Testing
  string temp;
  cout << counter;
  cin >> temp;
  */

  Movies filteredMovies = Movies(counter);

  for (i = nMoviesRead - counter; i < nMoviesRead; i++)
  {
    checkMovie(movies, filteredIds[i], &index);         // Get Movie Index
    filteredMovies.pushBack((*movies).getMovie(index)); // Save Movie that has been Filtered to Array
  }

  sortMovies(&filteredMovies, sortBy, movieFieldEnd - 1); // Sort Movies
  printMovies(&filteredMovies, fields);                   // Print Movies

  string message = "Number of Coincidences: ";
  message.append(to_string(counter));

  if (counter == 0)
    cout << string(nChar, '-') << '\n';

  cout << '\n';
  printTitle(message, applyBgColor, applyFgColor, (counter == 0) ? true : false); // Print Number of Coincidences

  filteredMovies.deallocate(); // Deallocate Memory
  delete[] filteredIds;
}

// Function to Add Movie to Filtered Movies
void addFilteredMovie(int filteredIds[], int nMoviesRead, int id, int *counter)
{
  *counter = *counter + 1;
  filteredIds[nMoviesRead - *counter] = id; // Insert Movie ID Right Before the Other IDs

  int pivot = nMoviesRead - *counter;
  insertionSortWithPivot(filteredIds, nMoviesRead, pivot); // Sort Array

  /*/ // For Testing
  for (int i = nMoviesRead - *counter; i < nMoviesRead; i++)
    cout << filteredIds[i] << '\n';
  cout << '\n';
  */
}

// Insertion Sort
// O(n^2)
// Adaptive and Stable
// Method: Insertion
void insertionSortWithPivot(int A[], int n, int pivot)
{
  int i, j, x;

  i = pivot;
  j = i + 1;
  x = A[i];

  while (j < n && A[j] < x)
  {
    A[j - 1] = A[j];
    j++;
  }
  A[j - 1] = x;
}

// Binary Search Algorithm
int binarySearch(int A[], int n, int num)
{
  int startA = 0, endA = n - 1;

  while (startA <= endA)
  {
    int mid = startA + (endA - startA) / 2;

    if (A[mid] == num)
      return mid;
    else if (A[mid] < num)
      startA = mid + 1;
    else
      endA = mid - 1;
  }
  return -1;
}

// Function to Add Movie
void addMovieToFile(Movies *movies)
{

  Movie newMovie = Movie();
  fill(newMovie.genres, newMovie.genres + genreEnd, -1); // Initialize Genres Array

  bool wrongGenre;
  int nGenres = 0, i, id, index;
  movieStatus check;
  string temp, date;

  string release;

  check = getMovieId(movies, &id, &index, "ID"); // Get Movie ID

  if (check != movieNotFound)
  { // The Id has been Added to that File
    wrongMovieData(movieExists);
    assert(id > 0 && index >= 0); // Check Client Id and Index
    return;                       // End this Function
  }

  assert(id > 0 && index == -1); // Check Client Id and Index
  newMovie.id = id;              // Assign Movie ID

  cout << "Title: "; // Get Movie Title
  getline(cin, newMovie.name);

  while (true) // Get Movie Genres
    try
    {
      cout << "Genre: ";
      getline(cin, temp);

      wrongGenre = true;
      for (i = 0; i < genreEnd; i++)
        if (getLower(genreStr[i]) == getLower(temp))
        {
          wrongGenre = false; // Valid Genre
          break;
        }

      if (wrongGenre)
        throw(-1); // Invalid Genre

      newMovie.genres[nGenres++] = i;

      if (nGenres == nMaxGenres)
        cout << "- Maximum Ammount of Genres Added";
      else if (!booleanQuestion("Do you want to Add more Genres?"))
        break;
    }
    catch (...)
    {
      wrongMovieData(invalidMovieGenre);
    }

  newMovie.duration = getInteger("Movie Duration", 0, 500); // Get Movie Duration

  cout << "Enter Name of Director: ";
  getline(cin, temp);
  newMovie.director = temp;

  newMovie.price = getInteger("Movie Price", 0, 1000); // Get Movie Price

  while (true) // Get Release Date
  {
    try
    {
      cout << "Release Date (YYYY-MM-DD): ";
      cin >> temp;

      stringstream stream(temp);

      for (int i = 0; i < 3; i++)
      {
        getline(stream, date, dateSep);
        newMovie.releaseDate[i] = stoi(date);
      }

      if (newMovie.releaseDate[0] < 1800)
        throw(-1); // Wrong Year of Release
      else if (newMovie.releaseDate[1] < 0 || newMovie.releaseDate[1] > 12)
        throw(-1); // Wrong Month of Release
      else
      {
        int year = newMovie.releaseDate[0];
        int month = newMovie.releaseDate[1];
        int day = newMovie.releaseDate[2];

        if (day < 0)
          throw(-1); // Wrong Day of Release
        else if ((month % 2 == 1 && month <= 7) || (month % 2 == 0 && month > 7))
        {
          if (day < 0 || day > 31)
            throw(-1); // That Month should have 31 Days
        }
        else if (month != 2 && (day < 0 || day > 30))
          throw(-1); // That Month should have 30 Days
        else if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
        {
          if (day < 0 || day > 29)
            throw(-1); // That Month should have 29 Days
        }
        else if (day < 0 || day > 28)
          throw(-1); // That Month should have 28 Days
        break;
      }
    }
    catch (...)
    {
      wrongMovieData(invalidMovieDate);
    }
  }

  (*movies).pushBack(newMovie);

  string genresStr = getGenresStr(newMovie.genres);
  string dateStr = getDateStr(newMovie.releaseDate);

  ofstream moviesCSV(moviesFilename, ios::app); // Write to File
  moviesCSV << newMovie.id << sep << newMovie.name << sep << genresStr
            << sep << newMovie.duration << sep << newMovie.director
            << sep << newMovie.price << sep << dateStr << ";;;\n";

  moviesCSV.close();

  cout << '\n';
  pressEnterToCont("Movie Added Successfully!", false);
}

// Function to Get Movie Rent Status
movieStatus checkMovie(Movies *movies, int id, int *index)
{
  bool found;
  int value;
  int mid, start = 0, end = (*movies).getNumberMovies() - 1;
  string line;

  moviesMergeSort(movies, movieFieldId * 2); // Sort Movies by Id

  while (start <= end)
  { // Binary Search
    mid = start + (end - start) / 2;

    value = (*movies).getMovie(mid).id;
    found = (value == id);

    if (found)
    {
      *index = mid;
      return ((*movies).getMovie(mid).rentStatus == 0) ? movieNotRented : movieRented;
    }
    else if (id > value)
      start = mid + 1;
    else
      end = mid - 1;
  }
  return movieNotFound;
}

// Function to Sort Movies (Uses Merge Sort)
void sortMovies(Movies *movies, int sortBy[], int n)
{
  for (int i = 0; i < n; i++)
    if (sortBy[i] != -1)
      moviesMergeSort(movies, sortBy[i]);
}

// - Merge Sort
// O(n*logn)
// Stable
void moviesMergeSort(Movies *movies, int sortByIndex)
{
  int pass, low, high, mid, i, n = (*movies).getNumberMovies();
  Movie *sorted = new Movie[n]; // Store the Array in the Heap Memory

  for (pass = 2; pass <= n; pass *= 2)
  {
    for (i = 0; i + pass - 1 < n; i += pass)
    {
      low = i;
      high = i + pass - 1;
      mid = (low + high) / 2;
      moviesMerge(movies, sorted, low, mid, high, sortByIndex);
    }
    if (n - i > pass / 2)
    {
      low = i;
      high = i + pass - 1;
      mid = (low + high) / 2;
      moviesMerge(movies, sorted, low, mid, n - 1, sortByIndex);
    }
  }

  if (pass / 2 < n)
    moviesMerge(movies, sorted, 0, pass / 2 - 1, n - 1, sortByIndex);

  delete[] sorted; // Deallocate Memory

  if (sortByIndex % 2 != 0) // True if it's an Odd Number, which is Used for Descending Order
    (*movies).reverse();    // Reverse Array
}

// Function to Merge Movies Subarrays
void moviesMerge(Movies *movies, Movie sorted[], int low, int mid, int high, int sortByIndex)
{
  int i = low, j = mid + 1, k = low;
  string firstTitle, secondTitle; // Used when Sorting the Array by Title
  bool op;

  switch (sortByIndex / 2)
  {
  case commands::movieFieldId:
    while (i <= mid && j <= high)
      sorted[k++] = (*movies).compare(&i, &j, movieFieldId, 1);
    break;
  case commands::movieFieldName:
    while (i <= mid && j <= high)
      sorted[k++] = (*movies).compare(&i, &j, movieFieldName, 1);
    break;
  case commands::movieFieldDirector:
    while (i <= mid && j <= high)
      sorted[k++] = (*movies).compare(&i, &j, movieFieldDirector, 1);
    break;
  // case commands::movieFieldGenre: // NOT DEFINED
  case commands::movieFieldDuration:
    while (i <= mid && j <= high)
      sorted[k++] = (*movies).compare(&i, &j, movieFieldDuration, 1);
    break;
  case commands::movieFieldPrice:
    while (i <= mid && j <= high)
      sorted[k++] = (*movies).compare(&i, &j, movieFieldPrice, 1);
    break;
  case commands::movieFieldRelease:
    while (i <= mid && j <= high)
      sorted[k++] = (*movies).compare(&i, &j, movieFieldRelease, 1);
    break;
  case commands::movieFieldStatus:
    while (i <= mid && j <= high)
      sorted[k++] = (*movies).compare(&i, &j, movieFieldStatus, 1);
    break;
  case commands::movieFieldRentOn:
    while (i <= mid && j <= high)
      sorted[k++] = (*movies).compare(&i, &j, movieFieldRentOn, 1);
    break;
  case commands::movieFieldRentTo:
    while (i <= mid && j <= high)
      sorted[k++] = (*movies).compare(&i, &j, movieFieldRentTo, 1);
    break;
  }

  while (i <= mid)
    sorted[k++] = (*movies).getMovie(i++);
  while (j <= high)
    sorted[k++] = (*movies).getMovie(j++);

  for (i = low; i <= high; i++)
    (*movies).insertAt(i, sorted[i]);
}

// Function to Get Genres as a Str
string getGenresStr(int genres[])
{
  string genresStr;

  for (int i = 0; i < nMaxGenres && genres[i] != -1; i++) // Get Movie Genres as a String
    if (i != 0)
    {
      genresStr += genreSep;
      genresStr.append(genreStr[genres[i]]);
    }
    else
      genresStr = genreStr[genres[0]];

  return genresStr;
}

// Function to Get Date as Str
string getDateStr(int date[3])
{
  ostringstream stream;

  for (int i = 0; i < 3; i++) // Get Movie Release Date as a String
    if (i != 0)
      stream << dateSep << setw(2) << setfill('0') << date[i];
    else
      stream << date[0];

  return stream.str();
}

// Function to Ask for Movie Id
movieStatus getMovieId(Movies *movies, int *id, int *index, string message)
{
  string temp;

  while (true) // Get Movie ID
    try
    {
      cout << message << ": ";
      getline(cin, temp);
      *id = stoi(temp);

      if (*id <= 0)
        throw(-1); // ID Must be in the Range 0<ID<n

      return checkMovie(movies, *id, index);
    }
    catch (...)
    {
      wrongMovieData(invalidMovieId);
    }
}

// Function to Get Movie Rent Date as an Array of Integers
string getCurrentDate(int date[])
{
  time_t t = time(nullptr); // Get Time since Epoch as a Textual Representation
  tm tm = *localtime(&t);   // Get tm Structure

  ostringstream stream; // Get Date as a String

  for (int i = 0; i < 3; i++)
  {
    if (i == 0)
      stream << put_time(&tm, "%Y"); // Save Year
    else if (i == 1)
      stream << put_time(&tm, "%m"); // Save Month
    else
      stream << put_time(&tm, "%d"); // Save Day

    date[i] = stoi(stream.str());
    stream.str(""); // Clear String
  }

  stream << put_time(&tm, "%Y-%m-%d");
  return stream.str(); // Return Stream as a String
}