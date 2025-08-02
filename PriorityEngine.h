#pragma once
#include "Patient.h"
#include <unordered_map>

using namespace std;

class QueueManager; // Forward declaration

class PriorityEngine {
private:
    float urgencyWeight;
    float waitTimeWeight;
    float serviceTypeWeight;
    unordered_map<string, float> serviceTypeScores;

public:
    PriorityEngine();
    void setWeights(float urgency, float waitTime, float serviceType);
    void setServiceTypeScore(string type, float score);
    // New: Accept QueueManager for visit count
    float calculatePriorityScore(Patient& patient, time_t currentTime, const QueueManager* queueManager = nullptr);
};