#include "PriorityEngine.h"

PriorityEngine::PriorityEngine() {
    urgencyWeight = 0.5f;
    waitTimeWeight = 0.3f;
    serviceTypeWeight = 0.2f;

    serviceTypeScores["Emergency"] = 10.0f;
    serviceTypeScores["Critical"] = 8.0f;
    serviceTypeScores["Checkup"] = 5.0f;
}

void PriorityEngine::setWeights(float urgency, float waitTime, float serviceType) {
    urgencyWeight = urgency;
    waitTimeWeight = waitTime;
    serviceTypeWeight = serviceType;
}

void PriorityEngine::setServiceTypeScore(string type, float score) {
    serviceTypeScores[type] = score;
}

float PriorityEngine::calculatePriorityScore(Patient& patient, time_t currentTime) {
    time_t waitTime = currentTime - patient.getArrivalTime();
    float serviceScore = serviceTypeScores[patient.getServiceType()];

    return (patient.getUrgency() * urgencyWeight) +
        (waitTime * waitTimeWeight) +
        (serviceScore * serviceTypeWeight);
}