#include "Algorithms.h"

bool Algorithms::FCFS(std::vector <Process>* readyQueue, std::vector<Process>* runningList) {
    try {
        runningList->push_back((*readyQueue)[0]);
        readyQueue->erase(readyQueue->begin());
        return true;
    }
    catch (...) {
        return false;
    }
}