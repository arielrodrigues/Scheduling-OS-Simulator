#ifndef SIMPLE_OS_SIMULATOR_ALGORITHMS_H
#define SIMPLE_OS_SIMULATOR_ALGORITHMS_H


#include <vector>
#include "Process.h"

namespace Algorithms {

    static bool FCFS(std::vector<Process>* readyQueue, std::vector<Process>* runningList) {
        try {
            if (!readyQueue->empty()) {
                runningList->push_back((*readyQueue)[0]);
                readyQueue->erase(readyQueue->begin());
                return true;
            }
        }
        catch (...) {
            return false;
        }
    }

}


#endif //SIMPLE_OS_SIMULATOR_ALGORITHMS_H
