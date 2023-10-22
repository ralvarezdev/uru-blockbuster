#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct Movie
{
  int id;
  string name;
  string Genre;
  int duration;
  string director;
  string releaseDate;
  string rentTo;
  string rentOn;
  string status;
};

int main()
{
  Movie Movie[1001];
  int count = 0, nline = 0;
  string line, word;

  ifstream infile("movies.csv", ifstream::in);
  ofstream outfile("movies.csv", ofstream::out | ios_base::app);

  if (!outfile.is_open() || !infile.is_open())
  {
    cout << "no se encontraron los archivos";
    return 0;
  }

  while (getline(infile, line))
  {
    stringstream str(line);
    while (getline(str, word, ','))
    {
      switch (count)
      {
      case 0:
        Movie[nline].id = stoi(word);
        break;
      case 1:
        Movie[nline].name = word;
        break;
      case 2:
        Movie[nline].Genre = word;
        break;
      case 3:
        Movie[nline].duration = stoi(word);
        break;
      case 4:
        Movie[nline].director = word;
        break;
      case 5:
        Movie[nline].releaseDate = stoi(word);
        break;
      case 6:
        Movie[nline].rentTo = word;
        break;
      case 7:
        Movie[nline].rentOn = word;
        break;
      case 8:
        Movie[nline].status = word;
        break;
      }
      count++;
    }
    nline++;
    count = 0;
  }

  for (int i = 0; i < nline; i++)
  {
    cout << Movie[nline].id << ' ' << Movie[nline].name << ' ' << Movie[nline].Genre << ' ' << Movie[nline].duration << ' '
         << Movie[nline].director << ' ' << Movie[nline].releaseDate << ' ' << Movie[nline].rentTo << ' ' << Movie[nline].rentOn << ' ' << Movie[nline].status << endl;
  }

  infile.close();
  outfile.close();

  return 0;
}