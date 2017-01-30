#include <iostream>
#include <string>
#include <tuple>

#ifndef SIMPLE_OS_SIMULATOR_PROCESS_H
#define SIMPLE_OS_SIMULATOR_PROCESS_H

// Define functions to get tuple itens
#define _getPID(x) std::get<0>(x)
#define _getPriority(x) std::get<1>(x)
#define _getExecutionTime(x) std::get<2>(x)
#define _getBlocketTime(x) std::get<3>(x)
#define _getSubmissionTime(x) std::get<4>(x)

/***
 * Data structure of the process
 */

class Process {
public:
    Process();
    Process(std::tuple<int, int, float, float, float>);
    void decrementExecutionTime();
    void decrementBlockTime();
    void decrementSubmissionTime();
    int getPID();
    int getPriority();
    float getExecutionTime();
    float getBlockTime();
    float getSubmissionTime();

private:
    int PID, priority;
    float submissionTime, executionTime, blockTime;
};


#endif //SIMPLE_OS_SIMULATOR_PROCESS_H
