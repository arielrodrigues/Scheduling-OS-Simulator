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
	Simulator();
    bool NewProcess(std::tuple<int, int, float, float, float>);
	void UpdateTime(uint32_t &);
    void StartSimulation();
    void CalcStatistics();
    void PrintResults();

private:
	float startTime;
    int processCounter, maxProcessMultiprogramming;
    // Scheduling process' queues
    std::vector<Process> blockedQueue, readyQueue, incomingQueue;
	// Statistics
	float _elapsedTime, _processorUse, _thoughput, _avgWaitingTime,
		  _avgResponseTime, _avgTurnaroungTime, _avgServiceTime;
};


#endif //SIMPLE_OS_SIMULATOR_SIMULATOR_H
