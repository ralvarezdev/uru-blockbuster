#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "..\datatables\output.h"
#include "..\terminal\input.h"
#include "..\namespaces.h"

using namespace std;
using namespace commands;
using namespace files;
using namespace movies;

// --- Extern Variables Declaration
extern string *genrePtr;

// --- Functions Prototypes
int getMovies(Movie movies[]);
void addMovieToFile(Movie movies[], int *nMoviesRead);
movieStatus checkMovieStatusById(Movie movies[], int nMoviesRead, int id, int *index);
void getGenres(string word, int genres[], int n);
int getGenreIndex(string genre);
int getGenreIndexLower(string genre);
void getDate(string word, int date[], int n);
void filterMovies(Movie movies[], int nMoviesRead, string **params, bool fields[], int sortBy[]);
void addFilteredMovie(int filteredIds[], int nMoviesRead, int id, int *counter);
int binarySearch(int A[], int n, int num);
void insertionSort(int A[], int n);
void sortMovies(Movie *movies, int m, int sortBy[], int n);
void moviesMergeSort(Movie *movies, int n, int sortByIndex);
void moviesMerge(Movie *movies, Movie *sorted, int low, int mid, int high, int sortByIndex);
string getGenresStr(int genres[], int nGenres);
string getDateStr(int date[3]);
int getMovieId(string message);
string getCurrentDate(int date[]);

// Function to Get an Array of Movies from movies.csv
int getMovies(Movie movies[])
{
  int genres[genreEnd], date[3];
  string director[2];
  int count = 0, nline = 0, nMoviesRead;
  string line, word;

  ifstream infile(moviesFilename);

  if (!infile.is_open())
  { // Couldn't Access to infile
    infile.close();
    pressEnterToCont("Error: File Not Found. Press ENTER to go Back to Main Menu", true);
  }

  while (getline(infile, line) && nMovies >= nMoviesRead)
    try
    {
      if (line.length() == 0)
        continue;

      fill(genres, genres + genreEnd, -1); // Initialize Genres Array

      stringstream file(line);

      count = 0;
      while (getline(file, word, sep))
      {
        if (word.length() != 0)
          switch (count)
          {
          case 0:
            movies[nline].id = stoi(word);
            break;
          case 1:
            movies[nline].name = word;
            break;
          case 2:
            getGenres(word, genres, nMaxGenres);
            for (int i = 0; i < genreEnd; i++)
              movies[nline].genres[i] = genres[i];
            break;
          case 3:
            movies[nline].duration = stoi(word);
            break;
          case 4:
            movies[nline].director = word;
            break;
          case 5:
            movies[nline].price = stof(word);
            break;
          case 6:
            getDate(word, date, 3);
            for (int i = 0; i < 3; i++)
              movies[nline].releaseDate[i] = date[i];
            break;
          case 7:
            movies[nline].rentTo = stoi(word); // Client ID
            break;
          case 8:
            getDate(word, date, 3);
            for (int i = 0; i < 3; i++)
              movies[nline].rentOn[i] = date[i]; // Rent Date
            break;
          case 9:
            movies[nline].rentStatus = stoi(word); // Rent Status (Yes for Rented)
            break;
          }
        count++;
      }
      nline++;
      nMoviesRead = nline;
    }
    catch (...)
    {
      // It will Ignore the Line that was Read from movies.csv
    }

  infile.close();

  return nMoviesRead;
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
    if (genre == genrePtr[i] || i == errorGenre)
      return i;
    else
      continue;

  return -1;
}

// Function to Get Movie Genre Index in the Array with the Different Genres (Comparing the Genre in Lowercase)
int getGenreIndexLower(string genre)
{
  for (int i = 0; i < genreEnd; i++)
    if (getLower(genre) == getLower(genrePtr[i]) || i == errorGenre)
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
  {
    date[counter] = stoi(temp);
    counter++;
  }
}

// Function that Returns Movie Indexes that Matched with the Parameters
void filterMovies(Movie movies[], int nMoviesRead, string **params, bool fields[], int sortBy[])
{
  movieStatus movieStatus;
  int i, j, n, id, compare, sortByInt, index, genreIndex, counter = 0;
  string compareLower, nameLower;

  int *filteredIds = new int[nMoviesRead];          // Allocate Memory
  fill(filteredIds, filteredIds + nMoviesRead, -1); // Fill Array with -1 Values

  for (int field = 0; field < movieFieldEnd - 1; field++)
  {
    if (params[field][0] == "null") // Check if the Function can Filter that Field, and if there are Parameters
      continue;

    for (int param = 0; param < maxParamPerSubCmd && params[field][param] != "null"; param++)
      switch (field)
      {
      case movieFieldId:
        id = stoi(params[field][param]);
        movieStatus = checkMovieStatusById(movies, nMoviesRead, id, &index); // Binary Search

        // Checks if the Movie has Already being Filtered
        if (movieStatus != movieNotFound && binarySearch(filteredIds, nMoviesRead, id) == -1)
          addFilteredMovie(filteredIds, nMoviesRead, id, &counter);

      case movieFieldName:
      case movieFieldDirector:
        compareLower = getLower(params[field][param]); // Get Movie Name or Director To Search for in Lowercase

        // Checks if the Movie Name or Director Name in Lowercase Contains the Parameter that is being Searched by Linear Search
        for (i = 0; i < nMoviesRead; i++)
        {
          nameLower = (field == movieFieldName) ? getLower(movies[i].name) : getLower(movies[i].director);

          if (binarySearch(filteredIds, nMoviesRead, movies[i].id) == -1 && nameLower.find(compareLower) != string::npos)
            addFilteredMovie(filteredIds, nMoviesRead, movies[i].id, &counter);
        }
        break;

      case movieFieldDuration:
      case movieFieldPrice:
        compare = stoi(params[field][param]); // Integer to Compare, it Could be Either the Duration or the Price
        sortByInt = (field == movieFieldDuration) ? movieSortByDurationA : movieSortByPriceA;

        moviesMergeSort(movies, nMoviesRead, sortByInt); // Sort Movies by Either the Duration or the Price

        if (field == movieFieldDuration)
        {
          for (i = 0; i < nMoviesRead && movies[i].duration <= compare; i++)
            if (binarySearch(filteredIds, nMoviesRead, movies[i].id) == -1)
              addFilteredMovie(filteredIds, nMoviesRead, movies[i].id, &counter);
        }
        else if (field == movieFieldPrice)
        {
          for (i = 0; i < nMoviesRead && movies[i].price <= compare; i++)
            if (binarySearch(filteredIds, nMoviesRead, movies[i].id) == -1)
              addFilteredMovie(filteredIds, nMoviesRead, movies[i].id, &counter);
        }
        break;

      case movieFieldGenre:
        genreIndex = getGenreIndex(params[field][param]); // Get Genre Index in Type of Genres Array

        for (i = 0; i < nMoviesRead; i++)
          for (j = 0; j < nMaxGenres && movies[i].genres[j] != -1; j++)
            if (movies[i].genres[j] == genreIndex) // Coincidence
              if (binarySearch(filteredIds, nMoviesRead, movies[i].id) == -1)
                addFilteredMovie(filteredIds, nMoviesRead, movies[i].id, &counter);
        break;

        // case movieFieldRelease:
      }
  }

  Movie *filteredMovies = new Movie[counter];

  n = 0;
  for (i = nMoviesRead - counter; i < nMoviesRead; i++)
  {
    checkMovieStatusById(movies, nMoviesRead, filteredIds[i], &index);
    filteredMovies[n++] = movies[index]; // Save Movie that has been Filtered to Array
  }

  sortMovies(filteredMovies, counter, sortBy, clientSortByEnd); // Sort Movies
  printMovies(filteredMovies, counter, fields);                 // Print Movies

  delete[] filteredMovies;
  delete[] filteredIds;
}

// Function to Add Movie to Filtered Movies
void addFilteredMovie(int filteredIds[], int nMoviesRead, int id, int *counter)
{
  filteredIds[0] = id;
  insertionSort(filteredIds, nMoviesRead); // Sort Array
  *counter = *counter + 1;
}

// Insertion Sort
// O(n^2)
// Adaptive and Stable
// Method: Insertion
void insertionSort(int A[], int n)
{
  int j, x;

  for (int i = 1; i < n; i++)
  {
    j = i - 1;
    x = A[i];

    while (j > -1 && A[j] > x)
    {
      A[j + 1] = A[j];
      j--;
    }
    A[j + 1] = x;
  }
}

// Function to Add Movie
void addMovieToFile(Movie movies[], int *nMoviesRead)
{
  if (*nMoviesRead >= nMovies)
    pressEnterToCont("The Maximum Number of Movies has been Reached", true);
  else
  {
    Movie newMovie = Movie();
    fill(newMovie.genres, newMovie.genres + genreEnd, -1); // Initialize Genres Array

    bool wrongGenre;
    int nGenres = 0, i, index;
    movieStatus check;
    string temp, date;

    string release;

    while (true) // Get Movie ID
      try
      {
        cout << "ID: ";
        getline(cin, temp);
        newMovie.id = stoi(temp);

        check = checkMovieStatusById(movies, *nMoviesRead, newMovie.id, &index);
        if (newMovie.id <= 0)
          throw(-1); // ID Must be in the Range 0<ID<n
        break;
      }
      catch (...)
      {
        wrongMovieData(invalidMovieId);
      }

    if (check != movieNotFound) // The Id has been Added to that File
      wrongMovieData(movieExists);
    else
    {
      cout << "Title: "; // Get Movie Title
      getline(cin, newMovie.name);

      while (true) // Get Movie Genres
        try
        {
          cout << "Genre: ";
          getline(cin, temp);

          wrongGenre = true;
          for (i = 0; i < genreEnd; i++)
            if (getLower(genrePtr[i]) == getLower(temp))
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

      while (true) // Get Movie Duration
        try
        {
          cout << "Movie Duration: ";
          getline(cin, temp);
          newMovie.duration = stoi(temp);

          if (newMovie.duration <= 0)
            throw(-1); // Wrong Duration
          break;
        }
        catch (...)
        {
          wrongMovieData(invalidMovieDuration);
        }

      cout << "Enter Name of Director: ";
      getline(cin, temp);
      newMovie.director = temp;

      while (true) // Get Movie Price
        try
        {
          cout << "Price: ";
          cin >> temp;
          newMovie.price = stoi(temp);

          if (newMovie.price < 0)
            throw(-1); // Wrong Price
          break;
        }
        catch (...)
        {
          wrongMovieData(invalidMoviePrice);
        }

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

      movies[*nMoviesRead] = newMovie;
      *nMoviesRead = *nMoviesRead + 1;

      string genresStr = getGenresStr(newMovie.genres, nGenres);
      string dateStr = getDateStr(newMovie.releaseDate);

      ofstream outfile(moviesFilename, ios::app); // Write to File
      outfile << newMovie.id << sep << newMovie.name << sep << genresStr
              << sep << newMovie.duration << sep << newMovie.director
              << sep << newMovie.price << sep << dateStr << ";;;\n";

      outfile.close();

      cout << '\n';
      pressEnterToCont("Movie Added Successfully!", false);
    }
  }
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

// Function to Get Movie Rent Status
movieStatus checkMovieStatusById(Movie movies[], int nMoviesRead, int id, int *index)
{
  moviesMergeSort(movies, nMoviesRead, movieSortByIdA); // Sort Movies by Id

  bool found;
  int value;
  int mid, start = 0, end = nMoviesRead - 1;
  string line;

  while (start <= end)
  { // Binary Search
    mid = start + (end - start) / 2;

    value = movies[mid].id;
    found = (value == id);

    if (found)
    {
      *index = mid;
      return (movies[mid].rentStatus == 0) ? movieNotRented : movieRented;
    }
    else if (id > value)
      start = mid + 1;
    else
      end = mid - 1;
  }
  return movieNotFound;
}

// Function to Sort Movies (Uses Merge Sort)
void sortMovies(Movie *movies, int m, int sortBy[], int n)
{
  for (int i = 0; i < n; i++)
    if (sortBy[i] != -1)
      moviesMergeSort(movies, m, sortBy[i]);
}

// - Merge Sort
// O(n*logn)
// Stable
void moviesMergeSort(Movie *movies, int n, int sortByIndex)
{
  int pass, low, high, mid, i;
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
  {
    int j = 0;
    Movie temp;

    for (i = n - 1; i > (n - 1) / 2; i--)
    {
      temp = movies[j];
      movies[j] = movies[i];
      movies[i] = temp;
      j++;
    }
  }
}

// Function to Merge Movies Subarrays
void moviesMerge(Movie *movies, Movie *sorted, int low, int mid, int high, int sortByIndex)
{
  int i = low, j = mid + 1, k = low;
  string firstTitle, secondTitle; // Used when Sorting the Array by Title
  bool op;

  switch (sortByIndex / 2)
  {
  case movieSortByDurationA / 2:
    while (i <= mid && j <= high)
      if (movies[i].duration < movies[j].duration)
        sorted[k++] = movies[i++];
      else
        sorted[k++] = movies[j++];
    break;

  case movieSortByIdA / 2:
    while (i <= mid && j <= high)
      if (movies[i].id < movies[j].id)
        sorted[k++] = movies[i++];
      else
        sorted[k++] = movies[j++];
    break;

  case movieSortByPriceA / 2:
    while (i <= mid && j <= high)
      if (movies[i].price < movies[j].price)
        sorted[k++] = movies[i++];
      else
        sorted[k++] = movies[j++];
    break;

  case movieSortByTitleA / 2:
    while (i <= mid && j <= high)
    {
      firstTitle = movies[i].name;
      secondTitle = movies[j].name;

      if (firstTitle.compare(secondTitle) < 0)
        sorted[k++] = movies[i++];
      else
        sorted[k++] = movies[j++];
    }
    break;

  case movieSortByReleaseA / 2:
    while (i <= mid && j <= high)
    {
      for (int n = 0; n < 3; n++)
        if (movies[i].releaseDate[n] != movies[j].releaseDate[n])
        {
          op = movies[i].releaseDate[n] < movies[j].releaseDate[n];
          break;
        }
        else if (n == 2)
          op = false;

      if (op)
        sorted[k++] = movies[i++];
      else
        sorted[k++] = movies[j++];
    }
    break;

  case movieSortByRentToA / 2:
    while (i <= mid && j <= high)
      if (movies[i].rentTo < movies[j].rentTo)
        sorted[k++] = movies[i++];
      else
        sorted[k++] = movies[j++];
    break;
  }

  while (i <= mid)
    sorted[k++] = movies[i++];
  while (j <= high)
    sorted[k++] = movies[j++];

  for (i = low; i <= high; i++)
    movies[i] = sorted[i];
}

// Function to Get Genres as a Str
string getGenresStr(int genres[], int nGenres)
{
  string genresStr;

  for (int i = 0; i < nGenres && genres[i] != -1; i++) // Get Movie Genres as a String
    if (i != 0)
    {
      genresStr += genreSep;
      genresStr.append(genrePtr[genres[i]]);
    }
    else
      genresStr = genrePtr[genres[0]];

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
int getMovieId(string message)
{
  string temp;

  while (true)
    try // Get Movie ID
    {
      cout << message << ": ";
      getline(cin, temp);
      return stoi(temp);
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

  date[0] = tm.tm_year; // Save Year as a Integer
  date[1] = tm.tm_mon;  // Save Month ...
  date[2] = tm.tm_mday; // Save Day ...

  ostringstream stream; // Get Date as a String

  stream << put_time(&tm, "%Y-%m-%d");
  return stream.str(); // Return Stream as a String
}