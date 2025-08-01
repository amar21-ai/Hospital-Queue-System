#pragma once
#include "PriorityEngine.h"
#include "QueueManager.h"

class AdminConsole {
private:
    PriorityEngine* engine;
    QueueManager* queueManager;

public:
    AdminConsole(PriorityEngine* e, QueueManager* qm);
    void setWeights(float u, float w, float s);
    void setServiceTypeScore(std::string type, float score);
    void setFairnessParams(int maxWait, float boost);
};