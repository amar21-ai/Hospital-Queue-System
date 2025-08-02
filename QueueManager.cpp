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
    for (auto patient : vipQueue) {
        delete patient;
    }
    for (auto patient : regularQueue) {
        delete patient;
    }
    // Clean up service history
    for (auto patient : serviceHistory) {
        delete patient;
    }
}

std::string QueueManager::determineQueueType(Patient* patient) {
    float score = patient->getPriorityScore();
    // VIP threshold - adjust based on your needs
    // Patients with high priority scores (>= 8.0) go to VIP
    if (score >= 8.0f || patient->getServiceType() == "Emergency") {
        return "VIP";
    }
    return "Regular";
}

void QueueManager::addPatient(Patient* patient) {
    // Calculate initial score
    time_t now = time(0);
    patient->updateWaitTime(now);
    float score = engine->calculatePriorityScore(*patient, now);
    patient->setPriorityScore(score);

    // Determine which queue to add to
    std::string queueType = determineQueueType(patient);

    if (queueType == "VIP") {
        vipQueue.push_back(patient);
        heapifyUp(vipQueue, vipQueue.size() - 1);
        std::cout << "Patient " << patient->getId() << " added to VIP queue (Score: "
            << score << ")\n";
    }
    else {
        regularQueue.push_back(patient);
        heapifyUp(regularQueue, regularQueue.size() - 1);
        std::cout << "Patient " << patient->getId() << " added to Regular queue (Score: "
            << score << ")\n";
    }

    patientTable[patient->getId()] = patient;
}

Patient* QueueManager::serveNextPatient() {
    Patient* next = nullptr;

    // Always serve from VIP queue first
    if (!vipQueue.empty()) {
        next = vipQueue.front();
        std::swap(vipQueue[0], vipQueue.back());
        vipQueue.pop_back();
        heapifyDown(vipQueue, 0);
        std::cout << "Serving from VIP queue: Patient " << next->getId() << "\n";
    }
    // If VIP is empty, check if we should merge regular queue to VIP
    else if (!regularQueue.empty()) {
        mergeQueues();
        if (!vipQueue.empty()) {
            next = vipQueue.front();
            std::swap(vipQueue[0], vipQueue.back());
            vipQueue.pop_back();
            heapifyDown(vipQueue, 0);
            std::cout << "VIP queue was empty. Merged regular queue. Serving Patient "
                << next->getId() << "\n";
        }
    }

    if (next) {
        patientTable.erase(next->getId());
        // Record service completion time
        recordServiceCompletion(next, time(0));
    }

    return next;
}

void QueueManager::mergeQueues() {
    if (vipQueue.empty() && !regularQueue.empty()) {
        std::cout << "VIP queue is now empty. Redirecting individuals from regular queue to VIP service counter.\n";

        // Move all patients from regular to VIP queue
        vipQueue = std::move(regularQueue);
        regularQueue.clear();

        // Rebuild heap for VIP queue
        rebuildHeap(vipQueue);
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
    // Update VIP queue priorities
    for (auto& patient : vipQueue) {
        time_t waitTimeSec = currentTime - patient->getArrivalTime();
        patient->updateWaitTime(currentTime);

        float newScore = engine->calculatePriorityScore(*patient, currentTime);

        // Apply fairness boost if needed
        int waitTimeMin = waitTimeSec / 60;
        if (waitTimeMin > maxWaitTime) {
            float extraWait = waitTimeMin - maxWaitTime;
            newScore += extraWait * boostMultiplier;
        }

        patient->setPriorityScore(newScore);
    }

    // Update Regular queue priorities
    for (auto& patient : regularQueue) {
        time_t waitTimeSec = currentTime - patient->getArrivalTime();
        patient->updateWaitTime(currentTime);

        float newScore = engine->calculatePriorityScore(*patient, currentTime);

        // Apply fairness boost if needed
        int waitTimeMin = waitTimeSec / 60;
        if (waitTimeMin > maxWaitTime) {
            float extraWait = waitTimeMin - maxWaitTime;
            newScore += extraWait * boostMultiplier;
        }

        patient->setPriorityScore(newScore);
    }

    // Rebuild both heaps
    rebuildHeap(vipQueue);
    rebuildHeap(regularQueue);
}

void QueueManager::printQueue() {
    printAllQueues();
}

void QueueManager::printAllQueues() {
    std::cout << "\n=== VIP Queue ===\n";
    if (vipQueue.empty()) {
        std::cout << "Empty\n";
    }
    else {
        for (const auto& patient : vipQueue) {
            std::cout << "ID: " << patient->getId()
                << " | Score: " << std::fixed << std::setprecision(2)
                << patient->getPriorityScore()
                << " | Type: " << patient->getServiceType() << "\n";
        }
    }

    std::cout << "\n=== Regular Queue ===\n";
    if (regularQueue.empty()) {
        std::cout << "Empty\n";
    }
    else {
        for (const auto& patient : regularQueue) {
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

bool QueueManager::isVipQueueEmpty() {
    return vipQueue.empty();
}

bool QueueManager::isRegularQueueEmpty() {
    return regularQueue.empty();
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

void QueueManager::addPatientAtTime(Patient* patient, time_t timestamp) {
    // Set the arrival time to the specified timestamp
    patient->setArrivalTime(timestamp);

    // Calculate priority score based on the timestamp
    float score = engine->calculatePriorityScore(*patient, timestamp);
    patient->setPriorityScore(score);

    // Determine which queue to add to
    std::string queueType = determineQueueType(patient);

    if (queueType == "VIP") {
        vipQueue.push_back(patient);
        heapifyUp(vipQueue, vipQueue.size() - 1);
    }
    else {
        regularQueue.push_back(patient);
        heapifyUp(regularQueue, regularQueue.size() - 1);
    }

    patientTable[patient->getId()] = patient;
}

std::string QueueManager::getQueueStatus() {
    return "VIP: " + std::to_string(vipQueue.size()) +
        ", Regular: " + std::to_string(regularQueue.size());
}