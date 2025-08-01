#include "AdminConsole.h"
#include<iostream>
AdminConsole::AdminConsole(PriorityEngine* e, QueueManager* qm)
    : engine(e), queueManager(qm) {
}

void AdminConsole::setWeights(float u, float w, float s) {
    if (u + w + s != 1.0f) {
        std::cerr << "Error: Weights must sum to 1.0\n";
        return;
    }
    engine->setWeights(u, w, s);
    std::cout << "Weights updated successfully.\n";
}

void AdminConsole::setServiceTypeScore(std::string type, float score) {
    engine->setServiceTypeScore(type, score);
    std::cout << type << " score set to " << score << ".\n";
}

void AdminConsole::setFairnessParams(int maxWait, float boost) {
    if (maxWait <= 0 || boost <= 0) {
        std::cerr << "Error: Values must be positive\n";
        return;
    }
    queueManager->setFairnessParams(maxWait, boost);
    std::cout << "Fairness rules updated (Max wait: "
        << maxWait << " mins, Boost: " << boost << ").\n";
}