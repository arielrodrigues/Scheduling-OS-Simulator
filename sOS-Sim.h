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
    bool NewProcess(std::tuple<int, int, float, float, float>);
	void UpdateTime(uint32_t &);
    void CheckReadyQueue();
	void CheckBlockedQueue();
    void CheckIncomingQueue();
    void StartSimulation();
    void CalcStatistics();
    void PrintResults();

private:
	float startTime;
    int processCounter, maxProcessMultiprogramming, SPEED_;
    // Scheduling process' queues
    std::vector<Process> blockedQueue, readyQueue, incomingQueue;
	// Statistics
	float _elapsedTime, _processorUse, _throughput, _avgWaitingTime,
		  _avgResponseTime, _avgTurnaroundTime, _avgServiceTime;

    bool EmptyQueue();
};


#endif //SIMPLE_OS_SIMULATOR_SIMULATOR_H
