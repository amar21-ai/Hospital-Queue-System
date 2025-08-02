#include "ReportManager.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <ctime>

ReportManager::ReportManager(QueueManager* qm) {
    this->queueManager = qm;
}

std::vector<Patient*> ReportManager::filterByTimeInterval(const std::vector<Patient*>& patients,
    time_t startTime, time_t endTime) {
    std::vector<Patient*> filtered;
    for (auto patient : patients) {
        time_t arrivalTime = patient->getArrivalTime();
        if (arrivalTime >= startTime && arrivalTime <= endTime) {
            filtered.push_back(patient);
        }
    }
    return filtered;
}

std::vector<Patient*> ReportManager::filterByPriority(const std::vector<Patient*>& patients,
    float minPriority, float maxPriority) {
    std::vector<Patient*> filtered;
    for (auto patient : patients) {
        float priority = patient->getPriorityScore();
        if (priority >= minPriority && priority <= maxPriority) {
            filtered.push_back(patient);
        }
    }
    return filtered;
}

void ReportManager::sortPatients(std::vector<Patient*>& patients, SortBy sortBy, SortOrder order) {
    std::sort(patients.begin(), patients.end(), [sortBy, order](Patient* a, Patient* b) {
        bool ascending = (order == SortOrder::ASCENDING);

        switch (sortBy) {
        case SortBy::ENTRY_TIME:
            return ascending ? a->getArrivalTime() < b->getArrivalTime()
                : a->getArrivalTime() > b->getArrivalTime();
        case SortBy::WAITING_TIME:
            return ascending ? a->getTotalWaitTimeMinutes() < b->getTotalWaitTimeMinutes()
                : a->getTotalWaitTimeMinutes() > b->getTotalWaitTimeMinutes();
        case SortBy::PRIORITY_SCORE:
            return ascending ? a->getPriorityScore() < b->getPriorityScore()
                : a->getPriorityScore() > b->getPriorityScore();
        default:
            return false;
        }
        });
}

std::string ReportManager::formatTime(time_t timestamp) {
    char buffer[100];
    struct tm timeinfo;
    localtime_s(&timeinfo, &timestamp);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return std::string(buffer);
}

std::string ReportManager::formatDuration(int minutes) {
    int hours = minutes / 60;
    int mins = minutes % 60;
    if (hours > 0) {
        return std::to_string(hours) + "h " + std::to_string(mins) + "m";
    }
    return std::to_string(mins) + "m";
}

void ReportManager::generateTimeIntervalReport(time_t startTime, time_t endTime,
    SortBy sortBy, SortOrder order) {
    std::vector<Patient*> allPatients = queueManager->getServiceHistory(startTime, endTime);

    if (allPatients.empty()) {
        std::cout << "\nNo patients found in the specified time interval.\n";
        return;
    }

    sortPatients(allPatients, sortBy, order);

    std::string title = "Service Report - Time Interval (" +
        formatTime(startTime) + " to " + formatTime(endTime) + ")";
    displayPatients(allPatients, title);
}

void ReportManager::generatePriorityReport(float minPriority, float maxPriority,
    SortBy sortBy, SortOrder order) {
    std::vector<Patient*> allPatients = queueManager->getServiceHistoryByPriority(minPriority, maxPriority);

    if (allPatients.empty()) {
        std::cout << "\nNo patients found in the specified priority range.\n";
        return;
    }

    sortPatients(allPatients, sortBy, order);

    std::string title = "Service Report - Priority Range (" +
        std::to_string(minPriority) + " to " + std::to_string(maxPriority) + ")";
    displayPatients(allPatients, title);
}

void ReportManager::generateQueueTypeReport(const std::string& queueType,
    SortBy sortBy, SortOrder order) {
    std::vector<Patient*> allPatients = queueManager->getServiceHistoryByQueueType(queueType);

    if (allPatients.empty()) {
        std::cout << "\nNo patients found for the specified queue type.\n";
        return;
    }

    sortPatients(allPatients, sortBy, order);

    std::string title = "Service Report - Queue Type (" + queueType + ")";
    displayPatients(allPatients, title);
}

void ReportManager::generateQueueTypeTimeReport(const std::string& queueType,
    time_t startTime, time_t endTime, SortBy sortBy, SortOrder order) {
    std::vector<Patient*> allPatients = queueManager->getServiceHistoryByQueueType(queueType, startTime, endTime);

    if (allPatients.empty()) {
        std::cout << "\nNo patients found for the specified queue type and time interval.\n";
        return;
    }

    sortPatients(allPatients, sortBy, order);

    std::string title = "Service Report - " + queueType + " (" +
        formatTime(startTime) + " to " + formatTime(endTime) + ")";
    displayPatients(allPatients, title);
}

void ReportManager::generateFullReport(SortBy sortBy, SortOrder order) {
    time_t now = time(0);
    time_t dayAgo = now - (24 * 60 * 60); // Last 24 hours

    std::vector<Patient*> allPatients = queueManager->getServiceHistory(dayAgo, now);

    if (allPatients.empty()) {
        std::cout << "\nNo patients served in the last 24 hours.\n";
        return;
    }

    sortPatients(allPatients, sortBy, order);
    displayPatients(allPatients, "Full Service Report (Last 24 Hours)");
}

void ReportManager::displayPatients(const std::vector<Patient*>& patients, const std::string& title) {
    std::cout << "\n" << title << "\n";
    std::cout << std::string(title.length() + 10, '=') << "\n\n";

    // Header
    std::cout << std::left << std::setw(8) << "ID"
        << std::setw(12) << "Service Type"
        << std::setw(10) << "Urgency"
        << std::setw(10) << "Priority"
        << std::setw(12) << "Wait Time"
        << std::setw(20) << "Entry Time"
        << std::setw(20) << "Service Time" << "\n";
    std::cout << std::string(92, '-') << "\n";

    // Patient data
    for (const auto& patient : patients) {
        std::cout << std::left << std::setw(8) << patient->getId()
            << std::setw(12) << patient->getServiceType()
            << std::setw(10) << patient->getUrgency()
            << std::setw(10) << std::fixed << std::setprecision(2) << patient->getPriorityScore()
            << std::setw(12) << formatDuration(patient->getTotalWaitTimeMinutes())
            << std::setw(20) << formatTime(patient->getArrivalTime())
            << std::setw(20) << formatTime(patient->getServiceTime()) << "\n";
    }

    std::cout << std::string(92, '-') << "\n";
    std::cout << "Total patients: " << patients.size() << "\n";

    // Calculate statistics for this subset
    if (!patients.empty()) {
        int totalWaitTime = 0;
        for (const auto& patient : patients) {
            totalWaitTime += patient->getTotalWaitTimeMinutes();
        }
        double avgWaitTime = static_cast<double>(totalWaitTime) / patients.size();
        std::cout << "Average wait time: " << formatDuration(static_cast<int>(avgWaitTime)) << "\n";
    }
}

void ReportManager::showReportMenu() {
    while (true) {
        std::cout << "\nReport Generation Menu\n";
        std::cout << "=========================\n";
        std::cout << "1. Time Interval Report\n";
        std::cout << "2. Priority Range Report\n";
        std::cout << "3. Queue Type Report\n";
        std::cout << "4. Queue Type + Time Report\n";
        std::cout << "5. Full Report (Last 24h)\n";
        std::cout << "6. Show Statistics\n";
        std::cout << "7. Return to Main Menu\n";
        std::cout << "Choice (1-7): ";

        int choice = getIntInput(1, 7);
        if (choice == 7) break;

        switch (choice) {
        case 1: {
            std::cout << "\nTime Interval Report\n";
            std::cout << "Enter start time:\n";
            time_t startTime = getTimeInput();
            std::cout << "Enter end time:\n";
            time_t endTime = getTimeInput();

            std::cout << "\nSort by:\n1. Entry Time\n2. Waiting Time\n3. Priority Score\n";
            std::cout << "Choice (1-3): ";
            int sortChoice = getIntInput(1, 3);
            SortBy sortBy = static_cast<SortBy>(sortChoice - 1);

            std::cout << "\nSort order:\n1. Ascending\n2. Descending\n";
            std::cout << "Choice (1-2): ";
            int orderChoice = getIntInput(1, 2);
            SortOrder order = (orderChoice == 1) ? SortOrder::ASCENDING : SortOrder::DESCENDING;

            generateTimeIntervalReport(startTime, endTime, sortBy, order);
            break;
        }
        case 2: {
            std::cout << "\nPriority Range Report\n";
            std::cout << "Enter minimum priority (0-100): ";
            float minPriority = getFloatInput(0, 100);
            std::cout << "Enter maximum priority (" << minPriority << "-100): ";
            float maxPriority = getFloatInput(minPriority, 100);

            std::cout << "\nSort by:\n1. Entry Time\n2. Waiting Time\n3. Priority Score\n";
            std::cout << "Choice (1-3): ";
            int sortChoice = getIntInput(1, 3);
            SortBy sortBy = static_cast<SortBy>(sortChoice - 1);

            std::cout << "\nSort order:\n1. Ascending\n2. Descending\n";
            std::cout << "Choice (1-2): ";
            int orderChoice = getIntInput(1, 2);
            SortOrder order = (orderChoice == 1) ? SortOrder::ASCENDING : SortOrder::DESCENDING;

            generatePriorityReport(minPriority, maxPriority, sortBy, order);
            break;
        }
        case 3: {
            std::cout << "\nQueue Type Report\n";
            std::cout << "Select queue type:\n";
            std::cout << "1. Emergency\n2. Critical\n3. Checkup\n";
            std::cout << "Choice (1-3): ";
            int queueChoice = getIntInput(1, 3);
            std::string queueTypes[] = { "Emergency", "Critical", "Checkup" };
            std::string selectedQueue = queueTypes[queueChoice - 1];

            std::cout << "\nSort by:\n1. Entry Time\n2. Waiting Time\n3. Priority Score\n";
            std::cout << "Choice (1-3): ";
            int sortChoice = getIntInput(1, 3);
            SortBy sortBy = static_cast<SortBy>(sortChoice - 1);

            std::cout << "\nSort order:\n1. Ascending\n2. Descending\n";
            std::cout << "Choice (1-2): ";
            int orderChoice = getIntInput(1, 2);
            SortOrder order = (orderChoice == 1) ? SortOrder::ASCENDING : SortOrder::DESCENDING;

            generateQueueTypeReport(selectedQueue, sortBy, order);
            break;
        }
        case 4: {
            std::cout << "\nQueue Type + Time Report\n";
            std::cout << "Select queue type:\n";
            std::cout << "1. Emergency\n2. Critical\n3. Checkup\n";
            std::cout << "Choice (1-3): ";
            int queueChoice = getIntInput(1, 3);
            std::string queueTypes[] = { "Emergency", "Critical", "Checkup" };
            std::string selectedQueue = queueTypes[queueChoice - 1];

            std::cout << "\nEnter start time:\n";
            time_t startTime = getTimeInput();
            std::cout << "Enter end time:\n";
            time_t endTime = getTimeInput();

            std::cout << "\nSort by:\n1. Entry Time\n2. Waiting Time\n3. Priority Score\n";
            std::cout << "Choice (1-3): ";
            int sortChoice = getIntInput(1, 3);
            SortBy sortBy = static_cast<SortBy>(sortChoice - 1);

            std::cout << "\nSort order:\n1. Ascending\n2. Descending\n";
            std::cout << "Choice (1-2): ";
            int orderChoice = getIntInput(1, 2);
            SortOrder order = (orderChoice == 1) ? SortOrder::ASCENDING : SortOrder::DESCENDING;

            generateQueueTypeTimeReport(selectedQueue, startTime, endTime, sortBy, order);
            break;
        }
        case 5: {
            std::cout << "\nSort by:\n1. Entry Time\n2. Waiting Time\n3. Priority Score\n";
            std::cout << "Choice (1-3): ";
            int sortChoice = getIntInput(1, 3);
            SortBy sortBy = static_cast<SortBy>(sortChoice - 1);

            std::cout << "\nSort order:\n1. Ascending\n2. Descending\n";
            std::cout << "Choice (1-2): ";
            int orderChoice = getIntInput(1, 2);
            SortOrder order = (orderChoice == 1) ? SortOrder::ASCENDING : SortOrder::DESCENDING;

            generateFullReport(sortBy, order);
            break;
        }
        case 6:
            showStatistics();
            break;
        }
    }
}

void ReportManager::showStatistics() {
    std::cout << "\nSystem Statistics\n";
    std::cout << "===================\n";

    int totalServed = getTotalPatientsServed();
    double avgWait = getAverageWaitTime();

    std::cout << "Total patients served: " << totalServed << "\n";
    std::cout << "Average wait time: " << formatDuration(static_cast<int>(avgWait)) << "\n";

    if (totalServed > 0) {
        // Additional statistics
        time_t now = time(0);
        time_t dayAgo = now - (24 * 60 * 60);
        std::vector<Patient*> recentPatients = queueManager->getServiceHistory(dayAgo, now);

        std::cout << "Patients served (last 24h): " << recentPatients.size() << "\n";

        // Service type breakdown
        int emergency = 0, critical = 0, checkup = 0;
        for (auto patient : recentPatients) {
            if (patient->getServiceType() == "Emergency") emergency++;
            else if (patient->getServiceType() == "Critical") critical++;
            else if (patient->getServiceType() == "Checkup") checkup++;
        }

        std::cout << "\nService Type Breakdown (24h):\n";
        std::cout << "  Emergency: " << emergency << "\n";
        std::cout << "  Critical: " << critical << "\n";
        std::cout << "  Checkup: " << checkup << "\n";
    }
}

double ReportManager::getAverageWaitTime() {
    time_t now = time(0);
    time_t weekAgo = now - (7 * 24 * 60 * 60); // Last week
    std::vector<Patient*> patients = queueManager->getServiceHistory(weekAgo, now);

    if (patients.empty()) return 0.0;

    int totalWaitTime = 0;
    for (auto patient : patients) {
        totalWaitTime += patient->getTotalWaitTimeMinutes();
    }

    return static_cast<double>(totalWaitTime) / patients.size();
}

int ReportManager::getTotalPatientsServed() {
    time_t now = time(0);
    time_t weekAgo = now - (7 * 24 * 60 * 60); // Last week
    return queueManager->getServiceHistory(weekAgo, now).size();
}

int ReportManager::getIntInput(int min, int max) {
    int choice;
    while (true) {
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number (" << min << "-" << max << "): ";
            continue;
        }
        if (choice < min || choice > max) {
            std::cout << "Input out of range. Please enter (" << min << "-" << max << "): ";
            continue;
        }
        break;
    }
    return choice;
}

float ReportManager::getFloatInput(float min, float max) {
    float value;
    while (true) {
        if (!(std::cin >> value)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number (" << min << "-" << max << "): ";
            continue;
        }
        if (value < min || value > max) {
            std::cout << "Input out of range. Please enter (" << min << "-" << max << "): ";
            continue;
        }
        break;
    }
    return value;
}

time_t ReportManager::getTimeInput() {
    std::cout << "Enter hours ago (0-168): ";
    int hoursAgo = getIntInput(0, 168); // Up to 1 week ago

    time_t now = time(0);
    return now - (hoursAgo * 60 * 60);
}