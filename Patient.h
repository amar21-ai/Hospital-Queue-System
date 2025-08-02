#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <ctime>
using namespace std;

class Patient {
private:
    int id;
    int urgency;
    string serviceType;
    time_t arrivalTime;
    time_t serviceTime;  // When the patient was served
    float priorityScore;

public:
    Patient(int id, int urgency, string serviceType);
    Patient(const Patient& other); // Copy constructor for history

    // Getters
    int getId();
    int getUrgency();
    string getServiceType();
    time_t getArrivalTime();
    time_t getServiceTime();
    float getPriorityScore();

    // Setters
    void setPriorityScore(float score);
    void setArrivalTime(time_t time);
    void setServiceTime(time_t time);
    void updateWaitTime(time_t currentTime);

    // Utility functions
    int getWaitTimeMinutes(time_t currentTime);
    int getTotalWaitTimeMinutes(); // From arrival to service
};

#endif