#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

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
};

void RegisterClient() {
    Client client;
    
    cout << "Enter client name: ";
    cin.ignore();
    getline(cin, client.name);

    cout << "Enter client phone number: ";
    getline(cin, client.phoneNumber);

    cout << "Enter client address: ";
    getline(cin, client.address);

    ofstream clientFile("clients.bin", ios::binary | ios::app);

    if (clientFile.is_open()) {
        clientFile << client.name << ";" << client.phoneNumber << ";" <<  client.address << "\n";
        clientFile.close();
        cout << "Client registered successfully." << endl;
    } else {
        cout << "Unable to open the client file." << endl;
    }
}

int CountRentedMoviesByClient(const string& clientName) {
    int count = 0;

    ifstream inFile("movies.csv");
    if (!inFile) {
        cout << "Failed to open movies.csv" << endl;
        return count;
    }

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, ';');
        int movieId = stoi(token);
        getline(ss, token, ';'); 
        
        getline(ss, token, ';');
        string rentedTo = token;
        if (rentedTo == clientName) {
            count++;
        }
    }

    inFile.close();

    return count;
}

void SearchClientRentals() {
    string clientName;
    cout << "Enter client name: ";
    cin.ignore();
    getline(cin, clientName);

    ifstream clientFile("clients.bin", ios::binary);
    if (clientFile) {
        string line;
        while (getline(clientFile, line)) {
            stringstream ss(line);
            string name;
            getline(ss, name, ';');
            if (name == clientName) {
                int rentedMovies = CountRentedMoviesByClient(clientName);
                cout << "Client " << clientName << " has rented " << rentedMovies << " movies." << endl;
                clientFile.close();
                return;
            }
        }
        clientFile.close();
    }

    cout << "Client not found. Registering new client..." << endl;
    RegisterClient();
}


