#include "SimulationManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <thread>
#include <chrono>

SimulationManager::SimulationManager(QueueManager* qm) {
    this->queueManager = qm;
    this->simulationStartTime = time(0);
}

bool SimulationManager::loadSimulation(const std::string& filename) {
    try {
        loadEventsFromJson(filename);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading simulation: " << e.what() << std::endl;
        return false;
    }
}

void SimulationManager::loadEventsFromJson(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    parseJsonEvents(buffer.str());
}

void SimulationManager::parseJsonEvents(const std::string& jsonContent) {
    events.clear();

    // Simple JSON parsing (basic implementation)
    // Looking for patterns like: "timestamp": 5, "patientId": 101, etc.

    std::istringstream iss(jsonContent);
    std::string line;

    SimulationEvent currentEvent;
    bool inEvent = false;

    while (std::getline(iss, line)) {
        // Remove whitespace
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

        if (line.find("{") != std::string::npos) {
            inEvent = true;
            currentEvent = SimulationEvent();
        }
        else if (line.find("}") != std::string::npos && inEvent) {
            events.push_back(currentEvent);
            inEvent = false;
        }
        else if (inEvent) {
            // Parse key-value pairs
            if (line.find("\"timestamp\":") != std::string::npos) {
                size_t pos = line.find(":") + 1;
                size_t end = line.find(",");
                if (end == std::string::npos) end = line.length();
                currentEvent.timestamp = std::stoi(line.substr(pos, end - pos));
            }
            else if (line.find("\"patientId\":") != std::string::npos) {
                size_t pos = line.find(":") + 1;
                size_t end = line.find(",");
                if (end == std::string::npos) end = line.length();
                currentEvent.patientId = std::stoi(line.substr(pos, end - pos));
            }
            else if (line.find("\"urgency\":") != std::string::npos) {
                size_t pos = line.find(":") + 1;
                size_t end = line.find(",");
                if (end == std::string::npos) end = line.length();
                currentEvent.urgency = std::stoi(line.substr(pos, end - pos));
            }
            else if (line.find("\"serviceType\":") != std::string::npos) {
                size_t start = line.find("\"", line.find(":")) + 1;
                size_t end = line.find("\"", start);
                currentEvent.serviceType = line.substr(start, end - start);
            }
        }
    }

    // Sort events by timestamp
    std::sort(events.begin(), events.end(),
        [](const SimulationEvent& a, const SimulationEvent& b) {
            return a.timestamp < b.timestamp;
        });
}

void SimulationManager::runSimulation() {
    if (events.empty()) {
        std::cout << "No simulation events loaded!\n";
        return;
    }

    std::cout << "\n?? Starting simulation with " << events.size() << " events...\n";
    std::cout << "================================================\n";

    simulationStartTime = time(0);
    int currentTime = 0;
    size_t eventIndex = 0;

    while (eventIndex < events.size()) {
        SimulationEvent& event = events[eventIndex];

        // Wait until it's time for this event
        if (event.timestamp > currentTime) {
            std::cout << "\n? Time: " << event.timestamp << " minutes\n";
            currentTime = event.timestamp;
        }

        // Process all events at this timestamp
        while (eventIndex < events.size() && events[eventIndex].timestamp == currentTime) {
            SimulationEvent& e = events[eventIndex];

            // Create patient and add to queue
            Patient* patient = new Patient(e.patientId, e.urgency, e.serviceType);
            time_t eventTime = simulationStartTime + (e.timestamp * 60);

            std::cout << "?? New Patient Arrival:\n";
            std::cout << "   ID: " << e.patientId
                << " | Urgency: " << e.urgency
                << " | Type: " << e.serviceType << "\n";

            queueManager->addPatientAtTime(patient, eventTime);
            std::cout << "   Queue Status: " << queueManager->getQueueStatus() << "\n";

            eventIndex++;
        }

        // Update priorities for current time
        time_t currentActualTime = simulationStartTime + (currentTime * 60);
        queueManager->updatePriorities(currentActualTime);

        // Small delay for visualization
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::cout << "\n? Simulation completed!\n";
    std::cout << "Final queue status:\n";
    queueManager->printAllQueues();
}

void SimulationManager::addEvent(int timestamp, int patientId, int urgency, const std::string& serviceType) {
    SimulationEvent event;
    event.timestamp = timestamp;
    event.patientId = patientId;
    event.urgency = urgency;
    event.serviceType = serviceType;

    events.push_back(event);

    // Re-sort events
    std::sort(events.begin(), events.end(),
        [](const SimulationEvent& a, const SimulationEvent& b) {
            return a.timestamp < b.timestamp;
        });
}

void SimulationManager::printEvents() {
    std::cout << "\n?? Simulation Events:\n";
    std::cout << "=====================\n";

    for (const auto& event : events) {
        std::cout << "Time " << event.timestamp << "min: Patient "
            << event.patientId << " (" << event.serviceType
            << ", Urgency " << event.urgency << ")\n";
    }
}