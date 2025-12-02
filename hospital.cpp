#include "hospital.h"

Hospital::Hospital(const string& cityName)
    : city(cityName), status(OPEN) {}

const string& Hospital::getCity() const { 
    return city; 
}

HospitalStatus Hospital::getStatus() const { 
    return status; 
}

void Hospital::setStatus(HospitalStatus newStatus) { 
    status = newStatus; 
}

void Hospital::addPatient(const Patient& p) {
    patients.insert(p);
}

Patient Hospital::treatNext() {
    return patients.extractMax();
}

bool Hospital::hasPatients() const {
    return !patients.isEmpty();
}

int Hospital::numPatients() const {
    return patients.size();
}

void Hospital::mergePatients(Hospital& other) {
    patients.merge(other.patients);
}

const Patient& Hospital::peekNext() const {
    return patients.peek();
}
