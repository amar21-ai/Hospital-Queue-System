#pragma once
#include "Patient.h"
#include "PriorityEngine.h"
#include <vector>
#include <unordered_map>

class QueueManager {
private:
    std::vector<Patient*> maxHeap;
    std::unordered_map<int, Patient*> patientTable;
    PriorityEngine* engine;
    int maxWaitTime = 25; // Default fairness threshold (mins)
    float boostMultiplier = 0.5f;

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    QueueManager(PriorityEngine* engine);
    void addPatient(Patient* patient);
    Patient* serveNextPatient();
    void updatePriorities(time_t currentTime);
    void printQueue();
    void setFairnessParams(int maxWait, float boost);
};