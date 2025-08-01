#include "AdminUI.h"
#include <limits>
#include<iostream>

AdminUI::AdminUI(AdminConsole* console) : console(console) {}

int AdminUI::getIntInput(int min, int max) {
    int choice;
    while (true) {
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number ("
                << min << "-" << max << "): ";
            continue;
        }
        if (choice < min || choice > max) {
            std::cout << "Input out of range. Please enter ("
                << min << "-" << max << "): ";
            continue;
        }
        break;
    }
    return choice;
}

void AdminUI::showServiceTypes() {
    std::cout << "\n=== Service Types ===\n";
    std::cout << "1. Emergency\n";
    std::cout << "2. Critical\n";
    std::cout << "3. Checkup\n";
}

void AdminUI::showMainMenu() {
    while (true) {
        std::cout << "\n=== Admin Menu ===\n";
        std::cout << "1. Change Priority Weights\n";
        std::cout << "2. Manage Service Types\n";
        std::cout << "3. Set Fairness Rules\n";
        std::cout << "4. Return to Main Menu\n";
        std::cout << "Choice (1-4): ";

        int choice = getIntInput(1, 4);
        if (choice == 4) break;
        handleInput(choice);

    }
}

void AdminUI::showWeightsMenu() {
    std::cout << "\n=== Priority Weights ===\n";
    std::cout << "Current weights must sum to 1.0\n";

    float remaining = 1.0f;

    std::cout << "Enter urgency weight (0-100%): ";
    float u = getIntInput(0, 100) / 100.0f;
    remaining -= u;

    std::cout << "Enter wait time weight (0-" << remaining * 100 << "%): ";
    float w = getIntInput(0, remaining * 100) / 100.0f;
    remaining -= w;

    float s = remaining;
    console->setWeights(u, w, s);
    std::cout << "Weights set: Urgency=" << u
        << ", Wait=" << w
        << ", Service=" << s << "\n";
}

void AdminUI::showServiceTypeMenu() {
    showServiceTypes();
    std::cout << "Select type to modify (1-3): ";
    int typeChoice = getIntInput(1, 3);

    std::string types[] = { "Emergency", "Critical", "Checkup" };
    std::cout << "Enter new score for " << types[typeChoice - 1] << " (1-100): ";
    float score = getIntInput(1, 100);

    console->setServiceTypeScore(types[typeChoice - 1], score);
    std::cout << types[typeChoice - 1] << " score updated to " << score << "\n";
}

void AdminUI::handleInput(int choice) {
    switch (choice) {
    case 1:
        showWeightsMenu();
        break;
    case 2:
        showServiceTypeMenu();
        break;
    case 3: {
        std::cout << "\n=== Fairness Rules ===\n";
        std::cout << "Enter max wait time (1-120 minutes): ";
        int wait = getIntInput(1, 120);

        std::cout << "Enter boost multiplier (1-50%): ";
        float boost = getIntInput(1, 50) / 100.0f;

        console->setFairnessParams(wait, boost);
        std::cout << "Fairness rules updated (Max wait: " << wait
            << " mins, Boost: " << boost << ")\n";
        break;
    }
    case 4:
        return;
    }
}