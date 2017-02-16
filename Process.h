#include <iostream>
#include <string>
#include <tuple>

#ifndef SIMPLE_OS_SIMULATOR_PROCESS_H
#define SIMPLE_OS_SIMULATOR_PROCESS_H

// Define functions to get tuple itens
#define _getPID(x) std::get<0>(x)
#define _getPriority(x) std::get<1>(x)
#define _getSubmissionTime(x) std::get<2>(x)
#define _getExecutionTime(x) std::get<3>(x)
#define _getBlockedTime(x) std::get<4>(x)

/***
 * Data structure of the process
 */

class Process {
public:
    Process();
    Process(std::tuple<int, int, double, double, double>);
    int getPID();
    int getPriority();
    int getQuantum();
    double getLastTimeRunning();
    double getSubmissionTime();
    double getExecutionTime();
    double getBlockTime();
    double getResponseTime();
    double getWaitingTime();
    void setResponseTime(double responseTime);
    void setWaitingTime(double waitingTime);
    void setLasTimeRunning(double lastTimeRunning);
    void setQuantum(int quantum);
    void decrementQuantum();
    void updateWaitingTime(double waitingTime);

private:
    int PID, priority, quantum;
    double submissionTime, executionTime, blockTime, waitingTime, responseTime, lastTimeRunning;
};


#endif //SIMPLE_OS_SIMULATOR_PROCESS_H
