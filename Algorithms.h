#ifndef SIMPLE_OS_SIMULATOR_ALGORITHMS_H
#define SIMPLE_OS_SIMULATOR_ALGORITHMS_H


#include <vector>
#include "Process.h"

namespace Algorithms {

    static bool FCFS(std::vector<Process>* readyQueue, std::vector<Process>* runningList);

};


#endif //SIMPLE_OS_SIMULATOR_ALGORITHMS_H
