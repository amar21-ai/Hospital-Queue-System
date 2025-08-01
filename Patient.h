#pragma once
#include <string>

using namespace std;

class Patient {
private:
    int id;
    int urgency;
    string serviceType;
    time_t arrivalTime;
    float priorityScore;

public:
    Patient(int id, int urgency, string serviceType);
    int getId();
    int getUrgency();
    string getServiceType();
    time_t getArrivalTime();
    float getPriorityScore();
    void setPriorityScore(float score);
    void updateWaitTime(time_t currentTime);
    void setArrivalTime(time_t time) { arrivalTime = time; }  // For testing fairness
};