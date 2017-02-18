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

#define noRemainingTime(_process) _process.getExecutionTime() < 1 && _process.getBlockTime() < 1
#define remainingSubmissionTime(_process) _process.getSubmissionTime() - _elapsedTime
#define decrementQuantum() _quantum--
#define noProcessRunning() runningProcess.size() < 1


class Simulator {

public:
	Simulator(int, bool, bool);
    void StartSimulation(bool (*shortTermSchedulingAlgorithm)(std::vector <Process>*, std::vector<Process>*, int*, double),
						 std::vector<std::tuple<int, double, int, double, double>>);
    std::string getResults();
	static void DebugLog(std::string happen);
	static void DebugLog(double instantTime, std::string happen);

private:
	bool EmptyQueue();
	void TerminateProcess(Process);
	bool StartProcess(std::tuple<int, double, int, double, double>);
	void CheckRunningProcess();
	void CheckBlockedQueue();
	void CheckIncomingQueue();
	void CheckQueues();
	void CalcStatistics();

	static bool debugmode;
	static int _quantum;
	bool _cpuIdle;
    int _elapsedTime, _cpuIdleTime, maxProcessMultiprogramming, countProcess;
    // Scheduling process' queues
    std::vector<Process> blockedQueue, readyQueue, incomingQueue, runningProcess, readysuspendQueue;
	// Statistics
	double _processorUse, _throughput, _avgWaitingTime, lastUpdate, SPEED_,
		  _avgResponseTime, _avgTurnaroundTime, _avgServiceTime;
};


#endif //SIMPLE_OS_SIMULATOR_SIMULATOR_H
