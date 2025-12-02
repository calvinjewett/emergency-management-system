/*
 * PA5: Micro Project - Emergency Management System
 * CPTS 223 Advanced Data Structures
 * 
 * Skeleton Code - main.cpp
 * 
 * This file provides the basic REPL structure and command parsing.
 * You need to implement the actual functionality for each command.
 * 
 * AI USAGE POLICY:
 * - You MAY use AI assistance for: priority queue, graph structure, REPL logic
 * - You MUST NOT use AI for: Dijkstra's algorithm (use your MA3 implementation)
 */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "graph.h"
#include "hospital.h"
#include "priorityqueue.h"
#include <unordered_map>
#include "patient.h"

using namespace std;

// Global data structures
Graph hospitalGraph;
vector<Hospital> hospitals;
unordered_map<string, int> cityIndex;

// Precomputed distances: 5x5 matrix
vector<vector<int>> shortestDistances;

using namespace std;

/*
 * Initialize the hospital network graph with the 5 cities and their connections.
 * 
 * Cities: Pullman, Moscow, Colfax, Lewiston, Spokane
 * 
 * Connections:
 * - Pullman <-> Moscow: 8 miles
 * - Pullman <-> Colfax: 17 miles
 * - Pullman <-> Lewiston: 30 miles
 * - Moscow <-> Lewiston: 32 miles
 * - Moscow <-> Spokane: 85 miles
 * - Colfax <-> Spokane: 50 miles
 */
void initializeNetwork() {
    // Step 1: Define cities
    vector<string> cityNames = {
        "Pullman", "Moscow", "Colfax", "Lewiston", "Spokane"
    };

    // Step 2: Add cities to graph & index map
    for (size_t i = 0; i < cityNames.size(); i++) {
        hospitalGraph.addCity(cityNames[i]);
        cityIndex[cityNames[i]] = i;
        hospitals.push_back(Hospital(cityNames[i]));
    }

    // Step 3: Add all graph connections
    hospitalGraph.addConnection("Pullman", "Moscow", 8);
    hospitalGraph.addConnection("Pullman", "Colfax", 17);
    hospitalGraph.addConnection("Pullman", "Lewiston", 30);
    hospitalGraph.addConnection("Moscow", "Lewiston", 32);
    hospitalGraph.addConnection("Moscow", "Spokane", 85);
    hospitalGraph.addConnection("Colfax", "Spokane", 50);

    // Step 4: Precompute all shortest paths
    int n = cityNames.size();
    shortestDistances.assign(n, vector<int>(n, INT_MAX));

    for (int i = 0; i < n; i++) {
        auto distMap = hospitalGraph.dijkstra(cityNames[i]);
        for (int j = 0; j < n; j++) {
            shortestDistances[i][j] = distMap[cityNames[j]];
        }
    }

    cout << "Hospital network initialized." << endl;
}



/*
 * Handle the emergency command.
 * Format: emergency "<name>" <severity> <city>
 * 
 * @param name - Patient name
 * @param severity - Severity level (1-10)
 * @param city - City where emergency occurred
 */
void handleEmergency(const string& name, int severity, const string& city) {
    if (cityIndex.find(city) == cityIndex.end()) {
        cout << "Error: Unknown city " << city << endl;
        return;
    }

    int originIndex = cityIndex[city];
    int bestHospital = -1;
    int bestDistance = INT_MAX;

    for (size_t i = 0; i < hospitals.size(); i++) {
        if (hospitals[i].getStatus() == OPEN) {
            int dist = shortestDistances[originIndex][i]; 
            if (dist < bestDistance) {
                bestDistance = dist;
                bestHospital = i;
            }
        }
    }

    if (bestHospital == -1) {
        cout << "Error: No open hospitals available!" << endl;
        return;
    }

    Patient p{name, severity, city};
    hospitals[bestHospital].addPatient(p);

    cout << "Patient " << name << " (severity " << severity << ") from "
         << city << " admitted to " << hospitals[bestHospital].getCity()
         << " hospital." << endl;
}



/*
 * Handle the hospital close command.
 * Format: hospital close <city>
 * 
 * @param city - City whose hospital to close
 */
void handleHospitalClose(const string& city) {
    if (cityIndex.find(city) == cityIndex.end()) {
        cout << "Error: Unknown city " << city << endl;
        return;
    }

    size_t closingIndex = cityIndex[city];
    Hospital& closingHospital = hospitals[closingIndex];

    if (closingHospital.getStatus() == CLOSED) {
        cout << city << " hospital is already closed." << endl;
        return;
    }

    // Find nearest OPEN hospital to transfer to
    int bestIndex = -1;
    int bestDist = INT_MAX;

    for (size_t i = 0; i < hospitals.size(); i++) {
        if (i != closingIndex && hospitals[i].getStatus() == OPEN) {
            int dist = shortestDistances[closingIndex][i];
            if (dist < bestDist) {
                bestDist = dist;
                bestIndex = i;
            }
        }
    }

    if (bestIndex == -1) {
        cout << "Error: Cannot close the last open hospital!" << endl;
        return;
    }

    int transferred = closingHospital.numPatients();

    hospitals[bestIndex].mergePatients(closingHospital);
    closingHospital.setStatus(CLOSED);

    cout << city << " hospital closed. "
         << transferred << " patients transferred to "
         << hospitals[bestIndex].getCity() << " hospital." << endl;
}


/*
 * Handle the hospital open command.
 * Format: hospital open <city>
 * 
 * @param city - City whose hospital to reopen
 */
void handleHospitalOpen(const string& city) {
    if (cityIndex.find(city) == cityIndex.end()) {
        cout << "Error: Unknown city " << city << endl;
        return;
    }

    int idx = cityIndex[city];
    Hospital& h = hospitals[idx];

    if (h.getStatus() == OPEN) {
        cout << city << " hospital is already open." << endl;
        return;
    }

    h.setStatus(OPEN);
    cout << city << " hospital is now open." << endl;
}


/*
 * Handle the treat command.
 * Format: treat <city>
 * 
 * @param city - City whose hospital should treat next patient
 */
void handleTreat(const string& city) {
    if (cityIndex.find(city) == cityIndex.end()) {
        cout << "Error: Unknown city " << city << endl;
        return;
    }

    int idx = cityIndex[city];
    Hospital& h = hospitals[idx];

    if (h.getStatus() == CLOSED) {
        cout << city << " hospital is CLOSED and cannot treat patients." << endl;
        return;
    }

    if (!h.hasPatients()) {
        cout << "No patients to treat at " << city << " hospital." << endl;
        return;
    }

    Patient p = h.treatNext();
    cout << "Treating " << p.name << " (severity " << p.severity
         << ") originally from " << p.originCity
         << " at " << city << " hospital." << endl;
}


/*
 * Handle the status command.
 * Format: status
 * 
 * Displays for each hospital:
 * - Hospital name/city
 * - Status (Open or Closed)
 * - Number of patients waiting
 * - Next patient info (if any): name, severity, original city
 */
void handleStatus() {
    cout << "\n=== Hospital Status ===" << endl;

    for (const Hospital& hospital : hospitals) {
        cout << "Hospital: " << hospital.getCity() << endl;

        if (hospital.getStatus() == OPEN)
            cout << "Status: OPEN" << endl;
        else
            cout << "Status: CLOSED" << endl;

        cout << "Patients Waiting: " << hospital.numPatients() << endl;

        if (hospital.hasPatients()) {
            const Patient& next = hospital.peekNext();
            cout << "Next Patient: " << next.name
                 << ", Severity: " << next.severity
                 << ", From: " << next.originCity << endl;
        }

        cout << endl;
    }
}



/*
 * Parse and execute a command from user input.
 * 
 * @param input - The full command line entered by user
 * @return true if should continue, false if should quit
 */
bool processCommand(const string& input) {
    istringstream iss(input);
    string command;
    iss >> command;
    
    if (command == "quit" || command == "exit") {
        return false;
    }
    else if (command == "emergency") {
        // Parse: emergency "<name>" <severity> <city>
        string name;
        int severity;
        string city;
        
        // Handle quoted name
        char ch;
        iss >> ch; // Get opening quote
        if (ch == '"') {
            getline(iss, name, '"'); // Read until closing quote
        } else {
            // Name might not be quoted, read as single word
            iss.putback(ch);
            iss >> name;
        }
        
        iss >> severity >> city;
        
        if (severity < 1 || severity > 10) {
            cout << "Error: Severity must be between 1 and 10." << endl;
        } else {
            handleEmergency(name, severity, city);
        }
    }
    else if (command == "hospital") {
        string action, city;
        iss >> action >> city;
        
        if (action == "close") {
            handleHospitalClose(city);
        }
        else if (action == "open") {
            handleHospitalOpen(city);
        }
        else {
            cout << "Unknown hospital action: " << action << endl;
        }
    }
    else if (command == "treat") {
        string city;
        iss >> city;
        handleTreat(city);
    }
    else if (command == "status") {
        handleStatus();
    }
    else if (command == "help") {
        cout << "\nAvailable commands:" << endl;
        cout << "  emergency \"<name>\" <severity> <city> - Register new emergency" << endl;
        cout << "  hospital close <city>                 - Close a hospital" << endl;
        cout << "  hospital open <city>                  - Reopen a hospital" << endl;
        cout << "  treat <city>                          - Treat next patient" << endl;
        cout << "  status                                - Show all hospital status" << endl;
        cout << "  help                                  - Show this help message" << endl;
        cout << "  quit/exit                             - Exit the program" << endl;
        cout << endl;
    }
    else {
        cout << "Unknown command: " << command << endl;
        cout << "Type 'help' for available commands." << endl;
    }
    
    return true;
}


int main() {
    cout << "========================================" << endl;
    cout << "  Emergency Management System" << endl;
    cout << "  PA5 - CPTS 223" << endl;
    cout << "========================================" << endl;
    
    // Initialize the hospital network
    initializeNetwork();
    
    cout << "\nType 'help' for available commands." << endl;
    cout << "Type 'quit' or 'exit' to end the program.\n" << endl;
    
    // Main REPL loop
    string input;
    bool running = true;
    
    while (running) {
        cout << "> ";
        getline(cin, input);
        
        // Skip empty input
        if (input.empty()) {
            continue;
        }
        
        running = processCommand(input);
    }
    
    cout << "Exiting Emergency Management System. Goodbye!" << endl;
    
    return 0;
}
