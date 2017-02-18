#include <iostream>
#include <string>
#include <tuple>

#ifndef SIMPLE_OS_SIMULATOR_PROCESS_H
#define SIMPLE_OS_SIMULATOR_PROCESS_H

// Define functions to get tuple itens
#define _getPID(x) std::get<0>(x)
#define _getSubmissionTime(x) std::get<1>(x)
#define _getPriority(x) std::get<2>(x)
#define _getExecutionTime(x) std::get<3>(x)
#define _getBlockedTime(x) std::get<4>(x)

/***
 * Data structure of the process
 */

class Process {
public:
    Process();
    Process(std::tuple<int, double, int, double, double>);
    int getPID();
    int getPriority();
    double getSubmissionTime();
    double getExecutionTime();
    double getBlockTime();
    double getResponseTime();
    double getWaitingTime();
	double getTurnaroundTime();
    void setResponseTime(double responseTime);
    void setWaitingTime(double _elapsedTime);
	void setTurnaroundTime(double _elapsedTime);
	void decrementExecutionTime() {if (executionTime > 0) --executionTime;};
	void decrementBlockTime() {if (blockTime > 0) --blockTime;};

	double executionTime__;
private:
    int PID, priority;
    double submissionTime, executionTime, blockTime, waitingTime, responseTime, turnaroundTime;
};


#endif //SIMPLE_OS_SIMULATOR_PROCESS_H
