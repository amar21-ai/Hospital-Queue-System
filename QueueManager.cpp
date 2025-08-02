#include "QueueManager.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

QueueManager::QueueManager(PriorityEngine* engine) {
    this->engine = engine;
    this->maxWaitTime = 25;
    this->boostMultiplier = 0.5f;
}

QueueManager::~QueueManager() {
    // Clean up remaining patients in queues
    for (auto patient : emergencyQueue) {
        delete patient;
    }
    for (auto patient : criticalQueue) {
        delete patient;
    }
    for (auto patient : checkupQueue) {
        delete patient;
    }
    // Clean up service history
    for (auto patient : serviceHistory) {
        delete patient;
    }
}

std::vector<Patient*>& QueueManager::getQueueByType(const std::string& serviceType) {
    if (serviceType == "Emergency") {
        return emergencyQueue;
    }
    else if (serviceType == "Critical") {
        return criticalQueue;
    }
    else { // "Checkup"
        return checkupQueue;
    }
}

void QueueManager::addPatient(Patient* patient) {
    // Calculate initial score
    time_t now = time(0);
    patient->updateWaitTime(now);
    float score = engine->calculatePriorityScore(*patient, now, this);
    patient->setPriorityScore(score);

    // Add to appropriate service type queue
    std::vector<Patient*>& targetQueue = getQueueByType(patient->getServiceType());
    targetQueue.push_back(patient);
    heapifyUp(targetQueue, targetQueue.size() - 1);

    std::cout << "Patient " << patient->getId() << " added to " << patient->getServiceType()
        << " queue (Score: " << score << ")\n";

    patientTable[patient->getId()] = patient;

    // Update visit count
    incrementVisitCount(patient->getId());
}

// Patient visit frequency methods
void QueueManager::incrementVisitCount(int patientId) {
    patientVisitCount[patientId]++;
}

int QueueManager::getVisitCount(int patientId) const {
    auto it = patientVisitCount.find(patientId);
    return (it != patientVisitCount.end()) ? it->second : 0;
}

std::vector<int> QueueManager::getFrequentVisitors(int threshold) const {
    std::vector<int> frequent;
    for (const auto& entry : patientVisitCount) {
        if (entry.second >= threshold)
            frequent.push_back(entry.first);
    }
    return frequent;
}

std::string QueueManager::getNextServiceType() {
    // Priority order: Emergency > Critical > Checkup
    if (!emergencyQueue.empty()) {
        return "Emergency";
    }
    else if (!criticalQueue.empty()) {
        return "Critical";
    }
    else if (!checkupQueue.empty()) {
        return "Checkup";
    }
    return ""; // No patients
}

Patient* QueueManager::serveNextPatient() {
    std::string nextServiceType = getNextServiceType();

    if (nextServiceType.empty()) {
        return nullptr; // No patients to serve
    }

    std::vector<Patient*>& queue = getQueueByType(nextServiceType);
    Patient* next = queue.front();

    std::swap(queue[0], queue.back());
    queue.pop_back();
    heapifyDown(queue, 0);

    std::cout << "Serving from " << nextServiceType << " queue: Patient " << next->getId() << "\n";

    // Check if we need to merge queues
    if (queue.empty()) {
        mergeQueues();
    }

    patientTable.erase(next->getId());
    recordServiceCompletion(next, time(0));

    return next;
}

void QueueManager::mergeQueues() {
    // Merge lower priority queues to higher priority service counters when they become empty

    if (emergencyQueue.empty() && !criticalQueue.empty()) {
        std::cout << "Emergency queue is now empty. Redirecting individuals from critical queue to emergency service counter.\n";
        emergencyQueue = std::move(criticalQueue);
        criticalQueue.clear();
        rebuildHeap(emergencyQueue);
    }

    if (criticalQueue.empty() && !checkupQueue.empty()) {
        std::cout << "Critical queue is now empty. Redirecting individuals from checkup queue to critical service counter.\n";
        criticalQueue = std::move(checkupQueue);
        checkupQueue.clear();
        rebuildHeap(criticalQueue);
    }

    // If emergency is empty but critical has patients that were moved from checkup
    if (emergencyQueue.empty() && !criticalQueue.empty()) {
        std::cout << "Emergency queue is now empty. Redirecting individuals from critical queue to emergency service counter.\n";
        emergencyQueue = std::move(criticalQueue);
        criticalQueue.clear();
        rebuildHeap(emergencyQueue);
    }
}

void QueueManager::heapifyUp(std::vector<Patient*>& heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index]->getPriorityScore() <= heap[parent]->getPriorityScore()) break;
        std::swap(heap[index], heap[parent]);
        index = parent;
    }
}

void QueueManager::heapifyDown(std::vector<Patient*>& heap, int index) {
    int size = heap.size();
    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;

        if (left < size && heap[left]->getPriorityScore() > heap[largest]->getPriorityScore())
            largest = left;
        if (right < size && heap[right]->getPriorityScore() > heap[largest]->getPriorityScore())
            largest = right;

        if (largest == index) break;
        std::swap(heap[index], heap[largest]);
        index = largest;
    }
}

void QueueManager::rebuildHeap(std::vector<Patient*>& heap) {
    std::make_heap(heap.begin(), heap.end(),
        [](Patient* a, Patient* b) { return a->getPriorityScore() < b->getPriorityScore(); });
}

void QueueManager::updatePriorities(time_t currentTime) {
    // Update Emergency queue priorities
    for (auto& patient : emergencyQueue) {
        time_t waitTimeSec = currentTime - patient->getArrivalTime();
        patient->updateWaitTime(currentTime);

        float newScore = engine->calculatePriorityScore(*patient, currentTime, this);

        // Apply fairness boost if needed
        int waitTimeMin = waitTimeSec / 60;
        if (waitTimeMin > maxWaitTime) {
            float extraWait = waitTimeMin - maxWaitTime;
            newScore += extraWait * boostMultiplier;
        }

        patient->setPriorityScore(newScore);
    }

    // Update Critical queue priorities
    for (auto& patient : criticalQueue) {
        time_t waitTimeSec = currentTime - patient->getArrivalTime();
        patient->updateWaitTime(currentTime);

        float newScore = engine->calculatePriorityScore(*patient, currentTime, this);

        // Apply fairness boost if needed
        int waitTimeMin = waitTimeSec / 60;
        if (waitTimeMin > maxWaitTime) {
            float extraWait = waitTimeMin - maxWaitTime;
            newScore += extraWait * boostMultiplier;
        }

        patient->setPriorityScore(newScore);
    }

    // Update Checkup queue priorities
    for (auto& patient : checkupQueue) {
        time_t waitTimeSec = currentTime - patient->getArrivalTime();
        patient->updateWaitTime(currentTime);

        float newScore = engine->calculatePriorityScore(*patient, currentTime, this);

        // Apply fairness boost if needed
        int waitTimeMin = waitTimeSec / 60;
        if (waitTimeMin > maxWaitTime) {
            float extraWait = waitTimeMin - maxWaitTime;
            newScore += extraWait * boostMultiplier;
        }

        patient->setPriorityScore(newScore);
    }

    // Rebuild all heaps
    rebuildHeap(emergencyQueue);
    rebuildHeap(criticalQueue);
    rebuildHeap(checkupQueue);
}

void QueueManager::printQueue() {
    printAllQueues();
}

void QueueManager::printAllQueues() {
    std::cout << "\n=== Emergency Queue ===\n";
    if (emergencyQueue.empty()) {
        std::cout << "Empty\n";
    }
    else {
        for (const auto& patient : emergencyQueue) {
            std::cout << "ID: " << patient->getId()
                << " | Score: " << std::fixed << std::setprecision(2)
                << patient->getPriorityScore()
                << " | Type: " << patient->getServiceType() << "\n";
        }
    }

    std::cout << "\n=== Critical Queue ===\n";
    if (criticalQueue.empty()) {
        std::cout << "Empty\n";
    }
    else {
        for (const auto& patient : criticalQueue) {
            std::cout << "ID: " << patient->getId()
                << " | Score: " << std::fixed << std::setprecision(2)
                << patient->getPriorityScore()
                << " | Type: " << patient->getServiceType() << "\n";
        }
    }

    std::cout << "\n=== Checkup Queue ===\n";
    if (checkupQueue.empty()) {
        std::cout << "Empty\n";
    }
    else {
        for (const auto& patient : checkupQueue) {
            std::cout << "ID: " << patient->getId()
                << " | Score: " << std::fixed << std::setprecision(2)
                << patient->getPriorityScore()
                << " | Type: " << patient->getServiceType() << "\n";
        }
    }
}

void QueueManager::setFairnessParams(int maxWait, float boost) {
    maxWaitTime = maxWait;
    boostMultiplier = boost;
}

bool QueueManager::isQueueEmpty(const std::string& serviceType) {
    return getQueueByType(serviceType).empty();
}

int QueueManager::getQueueSize(const std::string& serviceType) {
    return getQueueByType(serviceType).size();
}

void QueueManager::recordServiceCompletion(Patient* patient, time_t serviceTime) {
    // Create a copy of the patient for history
    Patient* historyPatient = new Patient(*patient);
    historyPatient->setServiceTime(serviceTime);
    serviceHistory.push_back(historyPatient);
}

std::vector<Patient*> QueueManager::getServiceHistory(time_t startTime, time_t endTime) {
    std::vector<Patient*> filtered;
    for (auto patient : serviceHistory) {
        time_t serviceTime = patient->getServiceTime();
        if (serviceTime >= startTime && serviceTime <= endTime) {
            filtered.push_back(patient);
        }
    }
    return filtered;
}

std::vector<Patient*> QueueManager::getServiceHistoryByPriority(float minPriority, float maxPriority) {
    std::vector<Patient*> filtered;
    for (auto patient : serviceHistory) {
        float priority = patient->getPriorityScore();
        if (priority >= minPriority && priority <= maxPriority) {
            filtered.push_back(patient);
        }
    }
    return filtered;
}

std::vector<Patient*> QueueManager::getServiceHistoryByQueueType(const std::string& queueType) {
    std::vector<Patient*> filtered;
    for (auto patient : serviceHistory) {
        if (patient->getServiceType() == queueType) {
            filtered.push_back(patient);
        }
    }
    return filtered;
}

std::vector<Patient*> QueueManager::getServiceHistoryByQueueType(const std::string& queueType, time_t startTime, time_t endTime) {
    std::vector<Patient*> filtered;
    for (auto patient : serviceHistory) {
        time_t serviceTime = patient->getServiceTime();
        if (patient->getServiceType() == queueType &&
            serviceTime >= startTime && serviceTime <= endTime) {
            filtered.push_back(patient);
        }
    }
    return filtered;
}

void QueueManager::addPatientAtTime(Patient* patient, time_t timestamp) {
    // Set the arrival time to the specified timestamp
    patient->setArrivalTime(timestamp);

    // Calculate priority score based on the timestamp
    float score = engine->calculatePriorityScore(*patient, timestamp, this);
    patient->setPriorityScore(score);

    // Add to appropriate service type queue
    std::vector<Patient*>& targetQueue = getQueueByType(patient->getServiceType());
    targetQueue.push_back(patient);
    heapifyUp(targetQueue, targetQueue.size() - 1);

    patientTable[patient->getId()] = patient;
}

std::string QueueManager::getQueueStatus() {
    return "Emergency: " + std::to_string(emergencyQueue.size()) +
        ", Critical: " + std::to_string(criticalQueue.size()) +
        ", Checkup: " + std::to_string(checkupQueue.size());
}