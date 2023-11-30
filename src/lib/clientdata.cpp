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


int ReadClientsData(Client clients[]) {
    ifstream clientFile("clients.bin", ios::binary);
    if (!clientFile) {
        cout << "Unable to open the client file." << endl;
        return 0;
    }

    string line;
    getline(clientFile, line); // Descarta la primera línea (título)

    int nClientsRead = 0;
    while (getline(clientFile, line) && nClientsRead < MAX_CLIENTS) {
        stringstream ss(line);
        string token;
        int count = 0;

        Client newClient;
        while (getline(ss, token, ';')) {
            if (token.length() != 0) {
                switch (count) {
                    case 0:
                        newClient.id = stoi(token);
                        break;
                    case 1:
                        newClient.name = token;
                        break;
                    case 2:
                        newClient.phoneNumber = token;
                        break;
                    case 3:
                        newClient.address = token;
                        break;
                    case 4:
                        newClient.id = token;
                        break;
                    case 5:
                        newClient.cedula = token;
                        break;
                }
                count++;
            }
        }

        clients[nClientsRead] = newClient;
        nClientsRead++;
    }

    clientFile.close();
    return nClientsRead;
}


// Función para buscar cliente por nombre
void SearchClientByName(const string& name, const Client clients[], int numClients) {
    bool found = false;
    for (int i = 0; i < numClients; ++i) {
        if (clients[i].name == name) {
            cout << "Client found:" << endl;
            cout << "Name: " << clients[i].name << endl;
            cout << "ID: " << clients[i].id << endl;
            cout << "Phone Number: " << clients[i].phoneNumber << endl;
            cout << "Address: " << clients[i].address << endl;
            cout << "Cedula: " << clients[i].cedula << endl;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Client with name '" << name << "' not found." << endl;
    }
}

// Función para buscar cliente por cédula
void SearchClientByCedula(const string& cedula, const Client clients[], int numClients) {
    bool found = false;
    for (int i = 0; i < numClients; ++i) {
        if (clients[i].cedula == cedula) {
            cout << "Client found:" << endl;
            cout << "Name: " << clients[i].name << endl;
            cout << "ID: " << clients[i].id << endl;
            cout << "Phone Number: " << clients[i].phoneNumber << endl;
            cout << "Address: " << clients[i].address << endl;
            cout << "Cedula: " << clients[i].cedula << endl;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Client with cedula '" << cedula << "' not found." << endl;
    }
}

// Función para buscar cliente por teléfono
void SearchClientByPhoneNumber(const string& phoneNumber, const Client clients[], int numClients) {
    bool found = false;
    for (int i = 0; i < numClients; ++i) {
        if (clients[i].phoneNumber == phoneNumber) {
            cout << "Client found:" << endl;
            cout << "Name: " << clients[i].name << endl;
            cout << "ID: " << clients[i].id << endl;
            cout << "Phone Number: " << clients[i].phoneNumber << endl;
            cout << "Address: " << clients[i].address << endl;
            cout << "Cedula: " << clients[i].cedula << endl;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Client with phone number '" << phoneNumber << "' not found." << endl;
    }
}

// Función para buscar cliente por ID
void SearchClientByID(const string& id, const Client clients[], int numClients) {
    bool found = false;
    for (int i = 0; i < numClients; ++i) {
        if (clients[i].id == id) {
            cout << "Client found:" << endl;
            cout << "Name: " << clients[i].name << endl;
            cout << "ID: " << clients[i].id << endl;
            cout << "Phone Number: " << clients[i].phoneNumber << endl;
            cout << "Address: " << clients[i].address << endl;
            cout << "Cedula: " << clients[i].cedula << endl;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Client with ID '" << id << "' not found." << endl;
    }
}
