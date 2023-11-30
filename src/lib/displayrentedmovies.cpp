#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

const int MAX_CLIENTS = 1000;

struct Movie {
    int id;
    string name;
    string Genre;
    int duration;
    string director;
    double price;
    string releaseDate;
    string rentTo;
    string rentOn;
    string status;  
};

struct Client {
    string name;
    string phoneNumber;
    string address;
    string id;
    string cedula;
};

void DisplayRentedMovies(Movie movies[], int n) {
    cout << "Rented Movies:" << endl;
    bool found = false;

    for (int i = 0; i < n; ++i) {
        if (movies[i].status == "Rented") {
            cout << "Movie ID: " << movies[i].id << " - Name: " << movies[i].name << " - Rented to: " << movies[i].rentTo << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No movies are currently rented." << endl;
    }
}


void DisplayAvailableMovies(Movie movies[], int n) {
    cout << "Available Movies:" << endl;
    bool found = false;

    for (int i = 0; i < n; ++i) {
        if (!(movies[i].status == "Rented")) {
            cout << "Movie ID: " << movies[i].id << " - Name: " << movies[i].name << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "All movies are currently rented." << endl;
    }
}
