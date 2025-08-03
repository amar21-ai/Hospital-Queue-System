#pragma once
#include "AdminConsole.h"

class AdminUI {
private:
    AdminConsole* console;

    int getIntInput(int min, int max);
    void showServiceTypes();
    void handleInput(int choice); 

public:
    AdminUI(AdminConsole* console);
    void showMainMenu();
    void showWeightsMenu();
    void showServiceTypeMenu();
};