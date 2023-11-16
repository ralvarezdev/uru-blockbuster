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

void AddMovie(Movie movies[], int &n)
{
    if (n < 1500)
    {
        int id, duration;
        string name, genres, director, releaseDate, rentTo = "", rentOn = "", status = "Available";

        cout << "Enter the ID of the movie: ";
        cin >> id;
        cout << "Enter the name of the movie: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter the genres (comma-separated): ";
        getline(cin, genres);
        cout << "Enter the duration of the movie: ";
        cin >> duration;
        cout << "Enter the director of the movie: ";
        cin.ignore();
        getline(cin, director);
        cout << "Enter the release date (YYYY-MM-DD): ";
        cin >> releaseDate;

        Movie newMovie = {id, name, genres, duration, director, releaseDate, rentTo, rentOn, status};
        movies[n++] = newMovie;
        n++;

        ofstream outFile("movies.csv", ios::app);
        outFile << id << "," << name << "," << genres << "," << duration << "," << director << "," << releaseDate << ","
                << rentTo << "," << rentOn << "," << status << endl;
        outFile.close();

        cout << "Movie added successfully!" << endl;
    }
    else
    {
        cout << "The maximum number of movies has been reached." << endl;
    }
}

/*
void RentMovie(Movie movies[], int n) {
    int id;
    string clientName;

    cout << "Enter the ID of the movie to rent: ";
    cin >> id;

    // Open the movies.csv file for reading
    ifstream inFile("movies.csv");
    if (!inFile) {
        cout << "Failed to open movies.csv" << endl;
        return;
    }

    int start = 0, end = n - 1;

    bool movieFound = false;

    while(start <= end){
        int mid = start + (end - start) / 2;

            string line;
            getline(inFile, line);
            stringstream ss(line);
            string token;
            getline(ss, token, ';');
            int movieId = stoi(token);

            if (movieId == id) {
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
                if (!outFile) {
                    cout << "Failed to open movies.csv for writing" << endl;
                    return ;
                }
                for (int i = 0; i < n; i++) {
                    outFile << movies[i].id << "," << movies[i].name << "," << movies[i].Genre << ","
                        << movies[i].duration << "," << movies[i].director << "," << movies[i].releaseDate << ","
                        << movies[i].rentTo << "," << movies[i].rentOn << "," << movies[i].status << endl;
                }
                outFile.close();

                cout << "Movie rented successfully to " << clientName << endl;
                break;
            }else if (movieId < id){
                start = mid + 1;
            } else {
                end = mid - 1;
            }
        }

    inFile.close();

    if (!movieFound) {
        cout << "Movie not found." << endl;
    }
}

*/