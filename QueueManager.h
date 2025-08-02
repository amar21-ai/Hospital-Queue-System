#ifndef QUEUEMANAGER_H
#define QUEUEMANAGER_H

#include "Patient.h"
#include "PriorityEngine.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <ctime>

class QueueManager {
private:
    PriorityEngine* engine;

    // Multiple queues support
    std::vector<Patient*> vipQueue;
    std::vector<Patient*> regularQueue;
    std::unordered_map<int, Patient*> patientTable;

    // Fairness parameters
    int maxWaitTime;
    float boostMultiplier;

    // Service history for reporting
    std::vector<Patient*> serviceHistory;

    // Helper functions for heap operations
    void heapifyUp(std::vector<Patient*>& heap, int index);
    void heapifyDown(std::vector<Patient*>& heap, int index);
    void rebuildHeap(std::vector<Patient*>& heap);

    // Determine which queue a patient should go to
    std::string determineQueueType(Patient* patient);

public:
    QueueManager(PriorityEngine* engine);
    ~QueueManager();

    // Core queue operations
    void addPatient(Patient* patient);
    Patient* serveNextPatient();
    void updatePriorities(time_t currentTime);

    // Multiple queue operations
    void mergeQueues();
    bool isVipQueueEmpty();
    bool isRegularQueueEmpty();

    // Display functions
    void printQueue();
    void printAllQueues();

    // Configuration
    void setFairnessParams(int maxWait, float boost);

    // Reporting
    std::vector<Patient*> getServiceHistory(time_t startTime, time_t endTime);
    std::vector<Patient*> getServiceHistoryByPriority(float minPriority, float maxPriority);
    void recordServiceCompletion(Patient* patient, time_t serviceTime);

    // Simulation support
    void addPatientAtTime(Patient* patient, time_t timestamp);
    std::string getQueueStatus();
};

#endif