#include "Patient.h"
#include <ctime>

Patient::Patient(int id, int urgency, string serviceType) {
    this->id = id;
    this->urgency = urgency;
    this->serviceType = serviceType;
    this->arrivalTime = time(0);
    this->priorityScore = 0.0f;
}

int Patient::getId() { return id; }
int Patient::getUrgency() { return urgency; }
string Patient::getServiceType() { return serviceType; }
time_t Patient::getArrivalTime() { return arrivalTime; }
float Patient::getPriorityScore() { return priorityScore; }
void Patient::setPriorityScore(float score) { priorityScore = score; }

void Patient::updateWaitTime(time_t currentTime) {
    // Empty for now (used later in QueueManager)
}