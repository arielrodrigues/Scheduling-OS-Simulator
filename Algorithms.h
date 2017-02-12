#ifndef SIMPLE_OS_SIMULATOR_ALGORITHMS_H
#define SIMPLE_OS_SIMULATOR_ALGORITHMS_H
#include "sOS-Sim.h"


#include <vector>
#include "Process.h"

namespace Algorithms {

    static bool FCFS(std::vector<Process>* readyQueue, std::vector<Process>* runningList, double _elapsedTime) {
        try {
            if (!readyQueue->empty()) {
                Simulator::DebugLog(_elapsedTime,
                                    "Processo " + std::to_string((*readyQueue)[0].getPID()) + " em execução");
                runningList->push_back((*readyQueue)[0]);
                readyQueue->erase(readyQueue->begin());
            } return true;
        }
        catch (...) {
            return false;
        }
    }

}


#endif //SIMPLE_OS_SIMULATOR_ALGORITHMS_H
