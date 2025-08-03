#ifndef SIMULATIONMANAGER_H
#define SIMULATIONMANAGER_H

#include "QueueManager.h"
#include "Patient.h"
#include <vector>
#include <string>
#include <ctime>

struct SimulationEvent {
    int timestamp = 0;      
    int patientId = -1;    
    int urgency = 0;      
    std::string serviceType = ""; 

    SimulationEvent() = default;

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

    bool loadSimulation(const std::string& filename);

    void runSimulation();

    void addEvent(int timestamp, int patientId, int urgency, const std::string& serviceType);

    void printEvents();
};

#endif