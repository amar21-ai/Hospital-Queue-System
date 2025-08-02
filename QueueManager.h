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

    // Service type-based queues
    std::vector<Patient*> emergencyQueue;
    std::vector<Patient*> criticalQueue;
    std::vector<Patient*> checkupQueue;
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

    // Get queue by service type
    std::vector<Patient*>& getQueueByType(const std::string& serviceType);

public:
    QueueManager(PriorityEngine* engine);
    ~QueueManager();

    // Core queue operations
    void addPatient(Patient* patient);
    Patient* serveNextPatient();
    void updatePriorities(time_t currentTime);

    // Service type queue operations
    void mergeQueues();
    bool isQueueEmpty(const std::string& serviceType);
    int getQueueSize(const std::string& serviceType);
    std::string getNextServiceType(); // Determines priority order for serving

    // Display functions
    void printQueue();
    void printAllQueues();

    // Configuration
    void setFairnessParams(int maxWait, float boost);

    // Reporting with queue type filtering
    std::vector<Patient*> getServiceHistory(time_t startTime, time_t endTime);
    std::vector<Patient*> getServiceHistoryByPriority(float minPriority, float maxPriority);
    std::vector<Patient*> getServiceHistoryByQueueType(const std::string& queueType);
    std::vector<Patient*> getServiceHistoryByQueueType(const std::string& queueType, time_t startTime, time_t endTime);
    void recordServiceCompletion(Patient* patient, time_t serviceTime);

    // Simulation support
    void addPatientAtTime(Patient* patient, time_t timestamp);
    std::string getQueueStatus();
};

#endif