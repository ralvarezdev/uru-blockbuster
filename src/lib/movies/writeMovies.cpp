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

// Function to Rent Movies
void RentMovie(Movie movies[], int n)
{
    int id;
    string clientName;

    cout << "Enter the ID of the movie to rent: ";
    cin >> id;

    // Open the movies.csv file for reading
    ifstream inFile("movies.csv");
    if (!inFile)
    {
        cout << "Failed to open movies.csv" << endl;
        return;
    }

    int start = 0, end = n - 1;

    bool movieFound = false;

    while (start <= end)
    {
        int mid = start + (end - start) / 2;

        string line;
        getline(inFile, line);
        stringstream ss(line);
        string token;
        getline(ss, token, ';');
        int movieId = stoi(token);

        if (movieId == id)
        {
            movieFound = true;
            getline(ss, token, ';');
            string name = token;
            cout << "Movie found: " << name << endl;
            cout << "ID: " << movieId << endl;

            cout << "Enter your name: ";
            cin.ignore();
            getline(cin, clientName);

            movies[mid].rentTo = clientName;
            movies[mid].status = "Rented";

            // Update the movies.csv file with the rented movie
            ofstream outFile("movies.csv");
            if (!outFile)
            {
                cout << "Failed to open movies.csv for writing" << endl;
                return;
            }
            for (int i = 0; i < n; i++)
            {
                outFile << movies[i].id << "," << movies[i].name << "," << movies[i].Genre << ","
                        << movies[i].duration << "," << movies[i].director << "," << movies[i].releaseDate << ","
                        << movies[i].rentTo << "," << movies[i].rentOn << "," << movies[i].status << endl;
            }
            outFile.close();

            cout << "Movie rented successfully to " << clientName << endl;
            break;
        }
        else if (movieId < id)
        {
            start = mid + 1;
        }
        else
        {
            end = mid - 1;
        }
    }

    inFile.close();

    if (!movieFound)
    {
        cout << "Movie not found." << endl;
    }
}
