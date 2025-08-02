#include "Patient.h"
#include "PriorityEngine.h"
#include "QueueManager.h"
#include "AdminConsole.h"
#include "AdminUI.h"
#include "SimulationManager.h"
#include "ReportManager.h"
#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>
using namespace std;

// Helper function for safe integer input
int getIntInput(int min, int max) {
    int choice;
    while (true) {
        cin >> choice;
        if (cin.fail() || choice < min || choice > max) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Enter a number (" << min << "-" << max << "): ";
        }
        else {
            break;
        }
    }
    return choice;
}

void simulateTimePassing(QueueManager& queue, int minutes) {
    cout << "\n⏳ Simulating " << minutes << " minutes...\n";
    this_thread::sleep_for(chrono::seconds(1)); // Visual feedback
    queue.updatePriorities(time(0) + minutes * 60);
}

void addPatient(QueueManager& queue) {
    cout << "\n➕ Add New Patient\n";
    cout << "Enter ID (1-9999): ";
    int id = getIntInput(1, 9999);

    cout << "\n🩺 Urgency Level:\n"
        << "1. Lowest (1)\n2. Low (2)\n3. Medium (3)\n4. High (4)\n5. Highest (5)\n"
        << "Select (1-5): ";
    int urgency = getIntInput(1, 5);

    cout << "\n🏷️ Service Type:\n"
        << "1. Emergency\n2. Critical\n3. Checkup\n"
        << "Select (1-3): ";
    int type = getIntInput(1, 3);

    string types[] = { "Emergency", "Critical", "Checkup" };
    Patient* p = new Patient(id, urgency, types[type - 1]);
    queue.addPatient(p);

    cout << "\n✅ Patient added! (ID: " << id
        << ", " << types[type - 1]
        << ", Urgency: " << urgency << ")\n";
}

void runSimulation(QueueManager& queue) {
    SimulationManager simManager(&queue);

    cout << "\n🎬 Simulation Options:\n";
    cout << "1. Load from JSON file\n";
    cout << "2. Quick demo simulation\n";
    cout << "Choice (1-2): ";

    int choice = getIntInput(1, 2);

    if (choice == 1) {
        cout << "Enter JSON filename (default: simulation_data.json): ";
        string filename;
        cin.ignore();
        getline(cin, filename);
        if (filename.empty()) filename = "simulation_data.json";

        if (simManager.loadSimulation(filename)) {
            simManager.runSimulation();
        }
        else {
            cout << "❌ Failed to load simulation file.\n";
        }
    }
    else {
        // Quick demo - add some events manually
        simManager.addEvent(1, 201, 5, "Emergency");
        simManager.addEvent(3, 202, 3, "Critical");
        simManager.addEvent(5, 203, 2, "Checkup");
        simManager.addEvent(7, 204, 4, "Emergency");
        simManager.addEvent(10, 205, 1, "Checkup");

        cout << "\n🚀 Running quick demo simulation...\n";
        simManager.runSimulation();
    }
}

void servePatients(QueueManager& queue) {
    cout << "\n🔄 Serve Patients Mode\n";
    cout << "How many patients to serve? (1-10): ";
    int count = getIntInput(1, 10);

    for (int i = 0; i < count; i++) {
        Patient* p = queue.serveNextPatient();
        if (p) {
            cout << "🩺 Served Patient " << p->getId()
                << " (Final Score: " << p->getPriorityScore()
                << ", Wait Time: " << p->getTotalWaitTimeMinutes() << " min)\n";
            delete p;
        }
        else {
            cout << "❌ No more patients in queue!\n";
            break;
        }

        // Small delay between servings
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

void runHospitalSystem() {
    PriorityEngine engine;
    QueueManager queue(&engine);
    AdminConsole console(&engine, &queue);
    AdminUI adminUI(&console);
    ReportManager reportManager(&queue);

    // Initial configuration
    console.setWeights(0.5f, 0.3f, 0.2f);
    console.setServiceTypeScore("Emergency", 10);
    console.setServiceTypeScore("Critical", 8);
    console.setServiceTypeScore("Checkup", 5);
    console.setFairnessParams(25, 0.5f);

    cout << "\n🏥 Welcome to Smart Hospital Queue Management System\n";
    cout << "==================================================\n";

    while (true) {
        cout << "\n🏠 Main Menu\n"
            << "1. Add Patient\n"
            << "2. Serve Next Patient\n"
            << "3. Serve Multiple Patients\n"
            << "4. View All Queues\n"
            << "5. Admin Console\n"
            << "6. Run Simulation\n"
            << "7. Generate Reports\n"
            << "8. Simulate Time Passing\n"
            << "9. Exit\n"
            << "Choice (1-9): ";

        switch (getIntInput(1, 9)) {
        case 1:
            addPatient(queue);
            break;

        case 2: {
            Patient* p = queue.serveNextPatient();
            if (p) {
                cout << "\n🩺 Served Patient " << p->getId()
                    << " (Final Score: " << p->getPriorityScore()
                    << ", Wait Time: " << p->getTotalWaitTimeMinutes() << " min)\n";
                delete p;
            }
            else {
                cout << "\n❌ No patients in queue!\n";
            }
            break;
        }

        case 3:
            servePatients(queue);
            break;

        case 4:
            cout << "\n📋 Current Queue Status:\n";
            queue.printAllQueues();
            break;

        case 5:
            adminUI.showMainMenu();
            break;

        case 6:
            runSimulation(queue);
            break;

        case 7:
            reportManager.showReportMenu();
            break;

        case 8:
            cout << "\n⏱️ Enter minutes to simulate (1-60): ";
            simulateTimePassing(queue, getIntInput(1, 60));
            cout << "✅ Priority scores updated!\n";
            break;

        case 9:
            cout << "\n👋 System shutdown. Thank you for using Smart Hospital Queue Management!\n";
            return;
        }
    }
}

int main() {
    runHospitalSystem();
    return 0;
}