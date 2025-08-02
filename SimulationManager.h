#ifndef SIMULATIONMANAGER_H
#define SIMULATIONMANAGER_H

#include "QueueManager.h"
#include "Patient.h"
#include <vector>
#include <string>
#include <ctime>

struct SimulationEvent {
    int timestamp = 0;      // Time in minutes from start
    int patientId = -1;     // Default to -1 to indicate uninitialized
    int urgency = 0;        // Default urgency
    std::string serviceType = ""; // Default to an empty string

    // Default constructor
    SimulationEvent() = default;

    // Parameterized constructor for convenience
    SimulationEvent(int ts, int pid, int urg, const std::string& st)
        : timestamp(ts), patientId(pid), urgency(urg), serviceType(st) {
    }
};


class SimulationManager {
private:
    QueueManager* queueManager;
    std::vector<SimulationEvent> events;
    time_t simulationStartTime;

    void loadEventsFromJson(const std::string& filename);
    void parseJsonEvents(const std::string& jsonContent);

public:
    SimulationManager(QueueManager* qm);

    // Load simulation from JSON file
    bool loadSimulation(const std::string& filename);

    // Run the simulation
    void runSimulation();

    // Manual event addition (for testing)
    void addEvent(int timestamp, int patientId, int urgency, const std::string& serviceType);

    // Display simulation status
    void printEvents();
};

#endif