#include "QueueManager.h"
#include <algorithm>
#include<iostream>
QueueManager::QueueManager(PriorityEngine* engine) {
    this->engine = engine;
}

void QueueManager::addPatient(Patient* patient) {
    // Calculate initial score
    time_t now = time(0);
    patient->updateWaitTime(now);
    float score = engine->calculatePriorityScore(*patient, now);
    patient->setPriorityScore(score);

    // Add to queue
    maxHeap.push_back(patient);
    patientTable[patient->getId()] = patient;
    heapifyUp(maxHeap.size() - 1);
}

Patient* QueueManager::serveNextPatient() {
    if (maxHeap.empty()) return nullptr;

    Patient* next = maxHeap.front();
    std::swap(maxHeap[0], maxHeap.back());
    maxHeap.pop_back();
    heapifyDown(0);
    patientTable.erase(next->getId());
    return next;
}

void QueueManager::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (maxHeap[index]->getPriorityScore() <= maxHeap[parent]->getPriorityScore()) break;
        std::swap(maxHeap[index], maxHeap[parent]);
        index = parent;
    }
}

void QueueManager::heapifyDown(int index) {
    int size = maxHeap.size();
    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;

        if (left < size && maxHeap[left]->getPriorityScore() > maxHeap[largest]->getPriorityScore())
            largest = left;
        if (right < size && maxHeap[right]->getPriorityScore() > maxHeap[largest]->getPriorityScore())
            largest = right;
        if (largest == index) break;

        std::swap(maxHeap[index], maxHeap[largest]);
        index = largest;
    }
}

void QueueManager::printQueue() {
    for (const auto& patient : maxHeap) {
        std::cout << "ID: " << patient->getId()
            << " | Score: " << patient->getPriorityScore()
            << " | Type: " << patient->getServiceType() << "\n";
    }
}
void QueueManager::setFairnessParams(int maxWait, float boost) {
    maxWaitTime = maxWait;
    boostMultiplier = boost;
}

void QueueManager::updatePriorities(time_t currentTime) {
    // Dynamic priority updates + fairness in one pass
    for (auto& patient : maxHeap) {
        // Update wait time
        time_t waitTimeSec = currentTime - patient->getArrivalTime();
        patient->updateWaitTime(currentTime);

        // Calculate base priority
        float newScore = engine->calculatePriorityScore(*patient, currentTime);

        // Apply fairness boost if needed
        int waitTimeMin = waitTimeSec / 60;
        if (waitTimeMin > maxWaitTime) {
            float extraWait = waitTimeMin - maxWaitTime;
            newScore += extraWait * boostMultiplier;
        }

        patient->setPriorityScore(newScore);
    }

    // Rebuild heap
    std::make_heap(maxHeap.begin(), maxHeap.end(),
        [](Patient* a, Patient* b) { return a->getPriorityScore() < b->getPriorityScore(); });
}