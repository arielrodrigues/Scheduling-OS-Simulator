#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>
#include "Process.h"

#ifndef SIMPLE_OS_SIMULATOR_SIMULATOR_H
#define SIMPLE_OS_SIMULATOR_SIMULATOR_H


class Simulator {

public:
	Simulator();
    bool CreateProcess(std::tuple<int, int, float, float, float>);
    void StartSimulation();
    void CalcStatistics();
    void PrintResults();

private:
	float _totalTime, _processorUse, _thoughput, _avgWaitingTime,
          _avgResponseTime, _avgTurnaroungTime, _avgServiceTime;
    int processCounter, maxProcessMultiprogramming;
    // Scheduling process' queues
    std::vector<Process> blockedQueue, readyQueue, incomingQueue;
};


#endif //SIMPLE_OS_SIMULATOR_SIMULATOR_H
