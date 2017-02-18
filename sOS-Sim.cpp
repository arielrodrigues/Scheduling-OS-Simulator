#include <cmath>
#include "sOS-Sim.h"
#include "Algorithms.h"

bool Simulator::debugmode;
int Simulator::_quantum;

/***
 * Constructor to the simulator
 * @param maxMultiprogramming - number of max. process running at same time (multiprogramming)
 * @param speed - speed of simulator
 * @param debugmode - active debuglog
 */

Simulator::Simulator(int maxMultiprogramming, bool step_by_step, bool debugmode) {
    this->_cpuIdle = true;
    this->_quantum = 0;
    this->debugmode = debugmode;
    this->SPEED_ = step_by_step? 1/2.0 : 0;
    this->maxProcessMultiprogramming = maxMultiprogramming;

    this->lastUpdate = 0;
    this->countProcess = 0;
    // statistics
    this->_elapsedTime = 0;
    this->_processorUse = 0;
    this->_avgWaitingTime = 0;
    this->_avgResponseTime = 0;
    this->_avgTurnaroundTime = 0;
    this->_avgServiceTime = 0;
    this->_throughput = 0;
    this->_cpuIdleTime = 0;
}

/**
 * Try to create process and put it on a queue
 * (if submission time == 0) create process in readyqueue (if readyQueue.size < 100) or create on readysuspendedqueue
 * (else) create in incomingqueue
 * @param _process
 * @return success
 */
bool Simulator::StartProcess(std::tuple<int, double, int, double, double> _process) {
    try {
        Process process(_process);
        if (process.getSubmissionTime() == 0)
            readyQueue.size() >= 100 ? readysuspendQueue.push_back(process) : readyQueue.push_back(process);
        else incomingQueue.push_back(process);
        countProcess++;
        return true;
    } catch (...) {
            return false;
    }
}

/***
 * Terminate a process and update simulator statistics
 * @param _process
 */
void Simulator::TerminateProcess(Process _process) {
    _avgWaitingTime += _process.getWaitingTime();
    _avgResponseTime += _process.getResponseTime();
    _avgServiceTime += _process.executionTime__;
    _avgTurnaroundTime += _process.getTurnaroundTime();
    DebugLog(_elapsedTime, ("Processo " + std::to_string(_process.getPID()) + " finalizado"));
}

/***
 * Check process in incoming queue and move then (if submissiontime <= 0) to ready queue
 */
void Simulator::CheckIncomingQueue() {
    for (auto i = incomingQueue.size(); i-- > 0;) {
        if (remainingSubmissionTime(incomingQueue[i]) <= 0) { // move to readyQueue
            readysuspendQueue.push_back(incomingQueue[i]);
            incomingQueue.erase(incomingQueue.begin() + i);
            DebugLog(_elapsedTime, "Processo " + std::to_string(incomingQueue[i].getPID()) + " submetido");
        }
    }
}

/***
 * Check if the process running has quantum time and if it has remaing time to execute, else send to block process
 */
void Simulator::CheckRunningProcess() {
    if (runningProcess.size() >= 1) {
        runningProcess[0].decrementExecutionTime();
        if (runningProcess[0].getExecutionTime() < 1) { // block process
            blockedQueue.push_back(runningProcess[0]);
            runningProcess.erase(runningProcess.begin());
            DebugLog(_elapsedTime, ("Processo " + std::to_string(runningProcess[0].getPID()) + " suspenso"));
        } else if (this->_quantum < 1) { // suspend process
            readysuspendQueue.push_back(runningProcess[0]);
            runningProcess.erase(runningProcess.begin());
            DebugLog(_elapsedTime, ("Processo " + std::to_string(runningProcess[0].getPID()) + " bloqueado"));
        }
    }
}

/***
 * Check process in blockedqueue and terminate it (if blockedtime <= 0)
 */
void Simulator::CheckBlockedQueue() {
    for (auto i = blockedQueue.size(); i-- > 0;) {
        blockedQueue[i].decrementBlockTime();
        if (noRemainingTime(blockedQueue[i])) { // terminate process
            blockedQueue[i].setTurnaroundTime(_elapsedTime);
            blockedQueue[i].setWaitingTime(_elapsedTime);
            TerminateProcess(blockedQueue[i]);
            blockedQueue.erase(blockedQueue.begin() + i);
        }
    }
}

void Simulator::CheckQueues() {
	CheckIncomingQueue();
	CheckBlockedQueue();
	CheckRunningProcess();
}


/***
 * Checks if has process in one of queues
 * @return incomingqueue and readyqueue and blockedqueue are empty?
 */
bool Simulator::EmptyQueue() {
    return incomingQueue.empty() && readysuspendQueue.empty()
           && readyQueue.empty() && blockedQueue.empty() && _cpuIdle;
}

/***
 * Start the simulation using the algorithm parameter-passed to schedule process
 * with the end of simulation statistics are calculated
 * @param shortTermSchedulingAlgorithm - pointer to the short time scheduling algorithm
 * @param process - a vector of tuples with information of process
 */
void Simulator::StartSimulation(
        bool (*shortTermSchedulingAlgorithm)(std::vector<Process>*, std::vector<Process>*, int*, double),
        std::vector<std::tuple<int, double, int, double, double>> process) {

    DebugLog("Inicio da simulação:");

    // For each process, create and put it on a queue
    for (std::tuple<int, double, int, double, double> _process : process)
        StartProcess(_process);

	// Clear vector of tuples
	process.clear();

    for (;!EmptyQueue(); lastUpdate = time(NULL))
        if (time(NULL) - lastUpdate >= SPEED_) {
            // Check queues
            CheckQueues();

            // Medium-term scheduling
            if (readyQueue.size() < maxProcessMultiprogramming)
                Algorithms::FCFS(&readysuspendQueue, &readyQueue, _elapsedTime);

            // Short-term scheduling
            if (noProcessRunning())
                _cpuIdle = !shortTermSchedulingAlgorithm(&readyQueue, &runningProcess, &_quantum, _elapsedTime);

            // Update counters
            _elapsedTime++;
	        decrementQuantum();
            if (_cpuIdle && noProcessRunning()) _cpuIdleTime++; // Is cpu idle?
        }

    CalcStatistics();
    if (debugmode) std::cout << getResults() << std::endl;
}

/**
 * Calc statistics and update variables
 */
void Simulator::CalcStatistics() {
    --_elapsedTime; --_cpuIdleTime;
    DebugLog("\nFim da simulação\n*******************************\nCalculando estatísticas");
    _processorUse = std::round((static_cast<double>(_elapsedTime) - _cpuIdleTime) / _elapsedTime * 100);
    _throughput = std::round(static_cast<double>(countProcess) / _elapsedTime * 100);
    _avgWaitingTime /= countProcess;
    _avgResponseTime /= countProcess;
    _avgTurnaroundTime /= countProcess;
    _avgServiceTime /= countProcess;
}

/**
 * Print results from simulation
 */
std::string Simulator::getResults() {
    std::stringstream out;
    out << "*******************************\n";
    out << "Duração da Simulação: " << _elapsedTime << "\nEficiência: " << _processorUse
        << "%\nVazão: " << _throughput << "%\nTempo médio de espera: " << _avgWaitingTime
        << "\nTempo médio de resposta: " << _avgResponseTime << "\nTempo médio de retorno: " << _avgTurnaroundTime
        << "\nTempo médio de serviço: " << _avgServiceTime;
    return out.str();
}

/***
 * If debugmode print debug
 * @param happen
 */
void Simulator::DebugLog(std::string happen) {
    if (Simulator::debugmode) std::cout << happen << std::endl;
}

void Simulator::DebugLog(double instantTime, std::string happen) {
    if (Simulator::debugmode) std::cout << "T = " << instantTime << ": " << happen << std::endl;
}
