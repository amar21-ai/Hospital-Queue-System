#include "Patient.h"
#include "PriorityEngine.h"
#include "QueueManager.h"
#include "AdminConsole.h"
#include "AdminUI.h"
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

void runHospitalSystem() {
    PriorityEngine engine;
    QueueManager queue(&engine);
    AdminConsole console(&engine, &queue);
    AdminUI adminUI(&console);

    // Initial configuration
    console.setWeights(0.5f, 0.3f, 0.2f);
    console.setServiceTypeScore("Emergency", 10);
    console.setServiceTypeScore("Critical", 8);
    console.setServiceTypeScore("Checkup", 5);
    console.setFairnessParams(25, 0.5f);

    while (true) {
        cout << "\n🏠 Smart Hospital Queue System\n"
            << "1. Add Patient\n2. Serve Next\n3. View Queue\n"
            << "4. Admin Console\n5. Simulate Time\n6. Exit\n"
            << "Choice (1-6): ";

        switch (getIntInput(1, 6)) {
        case 1:
            addPatient(queue);
            break;

        case 2: {
            Patient* p = queue.serveNextPatient();
            if (p) {
                cout << "\n🩺 Served Patient " << p->getId()
                    << " (Final Score: " << p->getPriorityScore() << ")\n";
                delete p;
            }
            else {
                cout << "\n❌ Queue is empty!\n";
            }
            break;
        }

        case 3:
            cout << "\n📋 Current Queue:\n";
            queue.printQueue();
            break;

        case 4:
            adminUI.showMainMenu();
            break;

        case 5:
            cout << "\n⏱️ Enter minutes to simulate (1-60): ";
            simulateTimePassing(queue, getIntInput(1, 60));
            cout << "Priority scores updated!\n";
            break;

        case 6:
            cout << "\n👋 System shutdown.\n";
            return;
        }
    }
}

int main() {
    runHospitalSystem();
    return 0;
}