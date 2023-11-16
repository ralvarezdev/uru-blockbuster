#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
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
void getDate(string word, int date[], int n);
void sortMovies(Movie *movies, int m, int sortBy[], int n);
void moviesMergeSort(Movie *movies, int n, int sortByIndex);
void moviesMerge(Movie *movies, Movie *sorted, int low, int mid, int high, int sortByIndex);
string getLower(string word);
string getGenresStr(int genres[], int nGenres);
string getDateStr(int date[3]);
int getMovieId(string message);

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
      fill(genres, genres + genreEnd, -1); // Initialize Genres Array

      stringstream file(line);

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
      count = 0;
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
  int counter = 0;
  string genre;

  stringstream stream(word);
  while (getline(stream, genre, genreSep) && counter < n) // Get Genre
  {
    for (int i = 0; i < genreEnd; i++)
    {
      if (genre == genrePtr[i] || i == errorGenre)
        genres[counter] = i;
      else
        continue;
      break;
    }
    counter++;
  }
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

/*
// Function that Returns Movie Indexes that Matched with the Parameters
int *filterMovies(Movie movies[], int nMoviesRead, FilterMoviesCmd filterCmd, bool matchAll)
{
  bool matches;

  int *filteredMovies = new int[nMoviesRead]; // Allocate Memory
  bool *moviesMatched = new bool[nMoviesRead];

  int *workingArray; // Depending on the Value of matchAll Parameters. The Array that will be Used to Work with, will be Different

  fill(moviesMatched, moviesMatched + nMoviesRead, false);

  if (matchAll)
    fill(filteredMovies, filteredMovies + nMoviesRead, 1);
  else
    fill(filteredMovies, filteredMovies + nMoviesRead, -1);

  for (int field = 0; field < fieldEnd; field++)
    for (int param = 0; param < maxParamPerSubCmd; param++)
      switch (field)
      {
      case fieldId:
      }
}

// Function to Filter Movies
void Filter(Movie movies[], int n, int size)
{
  Movie strG[size], strD[size], strDire[size];
  int filter = 0, id = 0, cont = 1, l = 0, r = n - 1, mid, duration;
  string names, genre, director;

  for (int i = 0; i < n; i++)
  {
    cout << movies[i].id << ' ' << movies[i].name << ' ' << movies[i].Genre << ' ' << movies[i].duration << ' '
         << movies[i].director << ' ' << movies[i].releaseDate;
  }

  do
  {
    cout << "Select a filter:\n1. ID\n2. Name of the movie\n3. Genre\n4. Duration\n5. Director\n6. Release Date\n7. Turn off";
    cin >> filter;

    switch (filter)
    {
    case 1:
      do
      {
        cout << "Enter the ID of the movie: ";
        cin >> id;
        if (id > 0 && id < 1001)
          cout << movies[id - 1].id << ' ' << movies[id - 1].name << ' ' << movies[id - 1].Genre << ' ' << movies[id - 1].duration << ' '
               << movies[id - 1].director << ' ' << movies[id - 1].releaseDate;

        cout << "Do you wish to continue?: (1.Y/2.N)";
        cin >> cont;
      } while (cont == 1);
      break;
    case 2:
      do
      {
        cout << "Enter the name of the movie: ";
        getline(cin, names);

        while (l <= r)
        {
          mid = l + (r - 1) / 2;
          int res = -1000;

          if (names == (movies[mid].name))
            res = 0;

          if (res == 0)
            cout << movies[mid].id << ' ' << movies[mid].name << ' ' << movies[mid].Genre << ' ' << movies[mid].duration << ' '
                 << movies[mid].director << ' ' << movies[mid].releaseDate;
          if (names > (movies[mid].name))
            l = mid + 1;
          else
            r = mid - 1;

          if (!(names == (movies[mid].name)))
            cout << "The movie was not found";
        }

        cout << "Do you wish to continue?: (1.Y/2.N)";
        cin >> cont;
      } while (cont == 1);
      break;
    case 3:
      cout << "Enter the genre: ";
      getline(cin, genre);
      do
      {
        for (int i = 0; i < n; i++)
        {
          if (movies[i].Genre == genre)
          {
            switch (i)
            {
            case 0:
              strG[i].id = movies[i].id;
              break;
            case 1:
              strG[i].name = movies[i].name;
              break;
            case 2:
              strG[i].Genre = movies[i].Genre;
              break;
            case 3:
              strG[i].duration = movies[i].duration;
              break;
            case 4:
              strG[i].director = movies[i].director;
              break;
            case 5:
              strG[i].releaseDate = movies[i].releaseDate;
              break;
            }
          }
        }

        for (int i = 0; i < n; i++)
        {
          cout << strG[i].id << ' ' << strG[i].name << ' ' << strG[i].Genre << ' ' << strG[i].duration << ' '
               << strG[i].director << ' ' << strG[i].releaseDate;
        }

        cout << "Do you wish to continue?: (1.Y/2.N)";
        cin >> cont;
      } while (cont == 1);
      break;
    case 4:
      cout << "Enter the duration of the movie/s: ";
      cin >> duration;
      do
      {
        for (int i = 0; i < n; i++)
        {
          if (movies[i].duration == duration)
          {
            switch (i)
            {
            case 0:
              strD[i].id = movies[i].id;
              break;
            case 1:
              strD[i].name = movies[i].name;
              break;
            case 2:
              strD[i].Genre = movies[i].Genre;
              break;
            case 3:
              strD[i].duration = movies[i].duration;
              break;
            case 4:
              strD[i].director = movies[i].director;
              break;
            case 5:
              strD[i].releaseDate = movies[i].releaseDate;
              break;
            }
          }
        }

        for (int i = 0; i < n; i++)
        {
          cout << strD[i].id << ' ' << strD[i].name << ' ' << strD[i].Genre << ' ' << strD[i].duration << ' '
               << strD[i].director << ' ' << strD[i].releaseDate;
        }

        cout << "Do you wish to continue?: (1.Y/2.N)";
        cin >> cont;
      } while (cont == 1);
      break;
    case 5:
      cout << "Enter the director to search: ";
      cin >> director;

      do
      {
        for (int i = 0; i < n; i++)
        {
          if (movies[i].director == director)
          {
            switch (i)
            {
            case 0:
              strDire[i].id = movies[i].id;
              break;
            case 1:
              strDire[i].name = movies[i].name;
              break;
            case 2:
              strDire[i].Genre = movies[i].Genre;
              break;
            case 3:
              strDire[i].duration = movies[i].duration;
              break;
            case 4:
              strDire[i].director = movies[i].director;
              break;
            case 5:
              strDire[i].releaseDate = movies[i].releaseDate;
              break;
            }
          }
        }

        for (int i = 0; i < n; i++)
        {
          cout << strDire[i].id << ' ' << strDire[i].name << ' ' << strDire[i].Genre << ' ' << strDire[i].duration << ' '
               << strDire[i].director << ' ' << strDire[i].releaseDate;
          ;
        }

        cout << "Do you wish to continue?: (1.Y/2.N)";
        cin >> cont;
      } while (cont == 1);
      break;
    }
    while (filter != 7)
      ;
  } while ((filter == 1) || (filter == 2) || (filter == 3) || (filter == 4) || (filter == 5) || (filter == 6));
}
*/

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
        break;
      }
      catch (...)
      {
        wrongMovieData(invalidMovieId);
      }

    if (check != clientNotFound) // The Id has been Added to that File
      wrongClientData(clientExists);
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
          for (int i = 0; i < genreEnd; i++)
            if (getLower(genrePtr[i]) == getLower(temp))
              wrongGenre = false; // Valid Genre

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
    else if (value > id)
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
  }

  while (i <= mid)
    sorted[k++] = movies[i++];
  while (j <= high)
    sorted[k++] = movies[j++];

  for (i = low; i <= high; i++)
    movies[i] = sorted[i];
}

// Function to Get a Lowercase String
string getLower(string word)
{
  string wordToLower;

  for (int i = 0; i < word.length(); i++)
    wordToLower += tolower(word[i]); // Append Character in Lowercase

  return wordToLower;
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