#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <string>
#include <tuple>
#include <vector>
#include <iomanip>
#include "Process.h"

#ifndef SIMPLE_OS_SIMULATOR_SIMULATOR_H
#define SIMPLE_OS_SIMULATOR_SIMULATOR_H
#define remainingExecutionTime(_process) _process.getExecutionTime() + _process.getSubmissionTime() - _elapsedTime
#define remainingBlockTime(_process) _process.getExecutionTime() + _process.getSubmissionTime() + _process.getBlockTime() - _elapsedTime
#define remaningSubmissionTime(_process) _process.getSubmissionTime() - _elapsedTime


class Simulator {

public:
	Simulator(int, int, bool);
    void StartSimulation(bool (*algorithm)(std::vector <Process>*, std::vector<Process>*, double),
						 std::vector<std::tuple<int, int, double, double, double>>);
    std::string getResults();
	static void DebugLog(std::string happen);
	static void DebugLog(double instantTime, std::string happen);

private:
	bool EmptyQueue();
	void TerminateProcess(Process);
	bool StartProcess(std::tuple<int, int, double, double, double>);
	void UpdateTime();
	void CheckProcessRunning();
	void CheckBlockedQueue();
	void CheckIncomingQueue();
	void CalcStatistics();

	static bool debugmode;
    int _elapsedTime, processRunningCounter, maxProcessMultiprogramming, countProcess, _cpuIdleTime;
    // Scheduling process' queues
    std::vector<Process> blockedQueue, readyQueue, incomingQueue, runningList;
	// Statistics
	double _processorUse, _throughput, _avgWaitingTime, lastUpdate, SPEED_,
		  _avgResponseTime, _avgTurnaroundTime, _avgServiceTime;

};


#endif //SIMPLE_OS_SIMULATOR_SIMULATOR_H
