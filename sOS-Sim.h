#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <tuple>
#include <vector>
#include "Process.h"

#ifndef SIMPLE_OS_SIMULATOR_SIMULATOR_H
#define SIMPLE_OS_SIMULATOR_SIMULATOR_H


class Simulator {

public:
	Simulator(int, int);
    bool StartProcess(std::tuple<int, int, float, float, float>);
	void UpdateTime(uint32_t &);
    void CheckProcessRunning();
	void CheckBlockedQueue();
    void CheckIncomingQueue();
    void TerminateProcess(Process);
    void StartSimulation(auto*, std::vector<std::tuple<int, int, float, float, float>>);
    void CalcStatistics();
    void PrintResults();

private:
	float startTime;
    int processRunningCounter, maxProcessMultiprogramming, SPEED_, lastPID;
    // Scheduling process' queues
    std::vector<Process> blockedQueue, readyQueue, incomingQueue, runningList;
	// Statistics
	float _elapsedTime, _processorUse, _throughput, _avgWaitingTime,
		  _avgResponseTime, _avgTurnaroundTime, _avgServiceTime;

    bool EmptyQueue();
};


#endif //SIMPLE_OS_SIMULATOR_SIMULATOR_H
