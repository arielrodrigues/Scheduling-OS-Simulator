#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <string>
#include <tuple>
#include <vector>
#include <iomanip>
#include "Process.h"
#include "Page.h"

#ifndef SIMPLE_OS_SIMULATOR_SIMULATOR_H
#define SIMPLE_OS_SIMULATOR_SIMULATOR_H

#define remainingSubmissionTime(_process) _process.getSubmissionTime() - _elapsedTime
#define decrementQuantum() _quantum--
#define noProcessRunning() runningProcess.size() < 1


class Simulator {

public:
	Simulator(int, bool, bool);
    void StartSimulation(bool (*shortTermSchedulingAlgorithm)(std::vector <Process>*, std::vector<Process>*, int*, double),
						 bool (*pageReplacementAlgorithm)(std::vector<Page>*, std::vector<Page>*, Page, double),
						 std::vector<std::tuple<int, double, int, double, double, std::vector<Page>>>);
    std::string getResults();
	static void DebugLog(std::string happen);
	static void DebugLog(double instantTime, std::string happen);
	void Clear(int maxMultiprogramming, bool step_by_step, bool debugmode);

	static const int numberOfMemoryFrames = 3;

private:
    bool isSysFull();
	bool EmptyQueue();
	void TerminateProcess(Process);
	bool StartProcess(std::tuple<int, double, int, double, double,std::vector<Page>>);
	void CheckRunningProcess();
	void CheckBlockedQueue();
	void CheckIncomingQueue();
    void CheckReadySuspensedQueue();
	void CheckQueues();
	void CalcStatistics();
	bool PageInMemory();
    void RemovePages(uint32_t PID);

	static bool debugmode;
	static int _quantum;
	bool _cpuIdle;
    int _elapsedTime, _cpuIdleTime, maxProcessMultiprogramming, countProcess;
    // Scheduling process' queues
    std::vector<Process> incomingQueue, waitingQueue, readySuspensedQueue, readyQueue,
                         runningProcess, blockedQueue, blockedSuspensedQueue;
    // Memory frames and disk
    std::vector<Page> Disk, memoryFrames;
	// Statistics
	double _processorUse, _throughput, _avgWaitingTime, lastUpdate, SPEED_,
		  _avgResponseTime, _avgTurnaroundTime, _avgServiceTime;
	std::stringstream out;

	struct Statistics {
		uint32_t hits;
		uint32_t miss;
        double hitRate;
	};

    Statistics _pageStatistics;
};


#endif //SIMPLE_OS_SIMULATOR_SIMULATOR_H
