#ifndef REPORTMANAGER_H
#define REPORTMANAGER_H

#include "QueueManager.h"
#include "Patient.h"
#include <vector>
#include <string>
#include <ctime>

enum class SortOrder {
    ASCENDING,
    DESCENDING
};

enum class SortBy {
    ENTRY_TIME,
    WAITING_TIME,
    PRIORITY_SCORE
};

class ReportManager {
private:
    QueueManager* queueManager;

    // Helper functions
    std::vector<Patient*> filterByTimeInterval(const std::vector<Patient*>& patients,
        time_t startTime, time_t endTime);
    std::vector<Patient*> filterByPriority(const std::vector<Patient*>& patients,
        float minPriority, float maxPriority);
    void sortPatients(std::vector<Patient*>& patients, SortBy sortBy, SortOrder order);
    std::string formatTime(time_t timestamp);
    std::string formatDuration(int minutes);

    // Helper for user input
    int getIntInput(int min, int max);
    float getFloatInput(float min, float max);
    time_t getTimeInput();
    void displayPatients(const std::vector<Patient*>& patients, const std::string& title);

public:
    ReportManager(QueueManager* qm);

    // Generate reports
    void generateTimeIntervalReport(time_t startTime, time_t endTime,
        SortBy sortBy = SortBy::ENTRY_TIME,
        SortOrder order = SortOrder::ASCENDING);
    void generatePriorityReport(float minPriority, float maxPriority,
        SortBy sortBy = SortBy::PRIORITY_SCORE,
        SortOrder order = SortOrder::DESCENDING);
    void generateQueueTypeReport(const std::string& queueType,
        SortBy sortBy = SortBy::ENTRY_TIME,
        SortOrder order = SortOrder::ASCENDING);
    void generateQueueTypeTimeReport(const std::string& queueType, time_t startTime, time_t endTime,
        SortBy sortBy = SortBy::ENTRY_TIME,
        SortOrder order = SortOrder::ASCENDING);
    void generateFullReport(SortBy sortBy = SortBy::WAITING_TIME,
        SortOrder order = SortOrder::DESCENDING);

    // Interactive report generation
    void showReportMenu();

    // Statistics
    void showStatistics();
    double getAverageWaitTime();
    int getTotalPatientsServed();
};

#endif