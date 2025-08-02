#include "Patient.h"
#include <ctime>

Patient::Patient(int id, int urgency, string serviceType) {
    this->id = id;
    this->urgency = urgency;
    this->serviceType = serviceType;
    this->arrivalTime = time(0);
    this->serviceTime = 0;
    this->priorityScore = 0.0f;
}

Patient::Patient(const Patient& other) {
    this->id = other.id;
    this->urgency = other.urgency;
    this->serviceType = other.serviceType;
    this->arrivalTime = other.arrivalTime;
    this->serviceTime = other.serviceTime;
    this->priorityScore = other.priorityScore;
}

int Patient::getId() {
    return id;
}

int Patient::getUrgency() {
    return urgency;
}

string Patient::getServiceType() {
    return serviceType;
}

time_t Patient::getArrivalTime() {
    return arrivalTime;
}

time_t Patient::getServiceTime() {
    return serviceTime;
}

float Patient::getPriorityScore() {
    return priorityScore;
}

void Patient::setPriorityScore(float score) {
    priorityScore = score;
}

void Patient::setArrivalTime(time_t time) {
    arrivalTime = time;
}

void Patient::setServiceTime(time_t time) {
    serviceTime = time;
}

void Patient::updateWaitTime(time_t currentTime) {
    // This method is used by QueueManager to update priorities
    // The actual wait time calculation is done in other methods
}

int Patient::getWaitTimeMinutes(time_t currentTime) {
    return (currentTime - arrivalTime) / 60;
}

int Patient::getTotalWaitTimeMinutes() {
    if (serviceTime == 0) return 0; // Not served yet
    return (serviceTime - arrivalTime) / 60;
}