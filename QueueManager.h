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

    std::vector<Patient*> emergencyQueue;
    std::vector<Patient*> criticalQueue;
    std::vector<Patient*> checkupQueue;
    std::unordered_map<int, Patient*> patientTable;

    int maxWaitTime;
    float boostMultiplier;

    std::vector<Patient*> serviceHistory;

    std::unordered_map<int, int> patientVisitCount;

    void heapifyUp(std::vector<Patient*>& heap, int index);
    void heapifyDown(std::vector<Patient*>& heap, int index);
    void rebuildHeap(std::vector<Patient*>& heap);

    std::vector<Patient*>& getQueueByType(const std::string& serviceType);

public:
    QueueManager(PriorityEngine* engine);
    ~QueueManager();

    void addPatient(Patient* patient);
    Patient* serveNextPatient();
    Patient* servePatientById(int patientId); 
    void updatePriorities(time_t currentTime);

    void mergeQueues();
    bool isQueueEmpty(const std::string& serviceType);
    int getQueueSize(const std::string& serviceType);
    std::string getNextServiceType(); 

    void printQueue();
    void printAllQueues();

    void setFairnessParams(int maxWait, float boost);

    std::vector<Patient*> getServiceHistory(time_t startTime, time_t endTime);
    std::vector<Patient*> getServiceHistoryByPriority(float minPriority, float maxPriority);
    std::vector<Patient*> getServiceHistoryByQueueType(const std::string& queueType);
    std::vector<Patient*> getServiceHistoryByQueueType(const std::string& queueType, time_t startTime, time_t endTime);
    void recordServiceCompletion(Patient* patient, time_t serviceTime);

    void addPatientAtTime(Patient* patient, time_t timestamp);
    std::string getQueueStatus();

    void incrementVisitCount(int patientId);
    int getVisitCount(int patientId) const;
    std::vector<int> getFrequentVisitors(int threshold) const;
};

#endif