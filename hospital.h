#pragma once
#include <string>
#include "priorityqueue.h"  // or your heap class
#include "patient.h"

using namespace std;

enum HospitalStatus { OPEN, CLOSED };

class Hospital {
private:
    string city;
    HospitalStatus status;
    PriorityQueue patients; // max-heap by severity

public:
    Hospital(const string& cityName);

    const string& getCity() const;
    HospitalStatus getStatus() const;
    void setStatus(HospitalStatus newStatus);

    void addPatient(const Patient& p);
    Patient treatNext();
    bool hasPatients() const;
    int numPatients() const;

    // For merging when hospital closes
    void mergePatients(Hospital& other);

    // peeking the next patient (highest priority)
    const Patient& peekNext() const;

};
