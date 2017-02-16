#include "sOS-Sim.h"

bool Simulator::debugmode;

/***
 * Constructor to the simulator
 * @param maxMultiprogramming - number of max. process running at same time (multiprogramming)
 * @param speed - speed of simulator
 * @param debugmode - active debuglog
 */

Simulator::Simulator(int maxMultiprogramming, int speed, bool debugmode) {
    Simulator::debugmode = debugmode;
    this->SPEED_ = speed/2.0;
    this->maxProcessMultiprogramming = maxMultiprogramming;

    this->lastUpdate = 0;
    this->countProcess = 0;
    this->processRunningCounter = 0;
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
 * @param _process
 * @return success
 */
bool Simulator::StartProcess(std::tuple<int, int, double, double, double> _process) {
    try {
        Process process(_process);
        // create process in readyqueue (if submission time == 0) or send for incomingqueue (else)
        (process.getSubmissionTime() == 0)? readyQueue.push_back(process), DebugLog(_elapsedTime,
                              "Processo "+std::to_string(process.getPID())+" pronto"): incomingQueue.push_back(process);
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
    _avgServiceTime += _process.getExecutionTime();
    _avgTurnaroundTime += (_process.getExecutionTime() + _process.getWaitingTime() + _process.getBlockTime());
    DebugLog(_elapsedTime, ("Processo " + std::to_string(_process.getPID()) + " finalizado"));
}

/***
 * Check process in incoming queue and move then (if submissiontime <= 0) to ready queue
 */
void Simulator::CheckIncomingQueue() {
    for (auto i = incomingQueue.size(); i-- > 0;) {
        if (remaningSubmissionTime(incomingQueue[i]) <= 0) { // move to readyQueue
            readyQueue.push_back(incomingQueue[i]);
            incomingQueue.erase(incomingQueue.begin() + i);
            DebugLog(_elapsedTime, "Processo " + std::to_string(incomingQueue[i].getPID()) + " pronto");
        }
    }
}

/***
 * Check process in running queue and move then (if executiontime <= 0 ) to block queue
 */
void Simulator::CheckProcessRunning() {
    for (auto i = runningList.size(); i-- > 0;) {
        if (runningList[i].getWaitingTime() == 0) runningList[i].setWaitingTime(_elapsedTime);
        if (remainingExecutionTime(runningList[i]) <= 0 || runningList[i].getQuantum() <= 0) { // move to blockedQueue
            blockedQueue.push_back(runningList[i]);
            runningList.erase(runningList.begin() + i); processRunningCounter--;
            DebugLog(_elapsedTime, ("Processo " + std::to_string(runningList[i].getPID()) + " bloqueado"));
            runningList[i].setLasTimeRunning(_elapsedTime);
        } else if (runningList[i].getQuantum() >= 0) runningList[i].decrementQuantum();
    }
}

/***
 * Check process in blockedqueue and terminate it (if blockedtime <= 0)
 */
void Simulator::CheckBlockedQueue() {
    for (auto i = blockedQueue.size(); i-- > 0;) {
        if (remainingBlockTime(blockedQueue[i]) <= 0) { // terminate process
            TerminateProcess(blockedQueue[i]); blockedQueue.erase(blockedQueue.begin() + i);
            if (blockedQueue[i].getResponseTime() == 0) blockedQueue[i].setResponseTime(_elapsedTime);
        }
    }
}

/***
 * Update elapsedTime and increments i in 1 and 1 seconds
 * @param i
 */
void Simulator::UpdateTime() {
    if (time(NULL) - lastUpdate >= SPEED_) {
        lastUpdate = time(NULL);
        CheckIncomingQueue();
        CheckProcessRunning();
        CheckBlockedQueue();
        _elapsedTime++;
    }
}

/***
 * Checks if has process in one of queues
 * @return incomingqueue and readyqueue and blockedqueue are empty?
 */
bool Simulator::EmptyQueue() {
    return incomingQueue.empty()
           && readyQueue.empty() && blockedQueue.empty() && runningList.empty();
}

/***
 * Start the simulation using the algorithm parameter-passed to schedule process
 * with the end of simulation statistics are calculated
 * @param algorithm - pointer to scheduling algorithm
 * @param process - a vector of tuples with information of process
 */
void Simulator::StartSimulation(bool (*algorithm)(std::vector<Process>*, std::vector<Process>*, double),
        std::vector<std::tuple<int, int, double, double, double>> process) {

    DebugLog("Inicio da simulação:");
    for (std::tuple<int, int, double, double, double> _process : process)
        StartProcess(_process);

    for (; !EmptyQueue(); UpdateTime())
        if (processRunningCounter < maxProcessMultiprogramming)
            if (algorithm(&readyQueue, &runningList, _elapsedTime))
                processRunningCounter = (int) (runningList.size() - 1);
            else _cpuIdleTime++;

    CalcStatistics();
}

/**
 * Calc statistics and update variables
 */
void Simulator::CalcStatistics() {
    DebugLog(_elapsedTime, "Fim da simulação\n*******************************\nCalculando estatísticas");
    _processorUse = (_elapsedTime - _cpuIdleTime) / _elapsedTime * 100;
    _throughput = countProcess / _elapsedTime;
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
        << "\nVazão: " << _throughput << "\nTempo médio de espera: " << _avgWaitingTime
        << "\nTempo médio de resposta: " << _avgResponseTime << "\nTempo médio de retorno: " << _avgTurnaroundTime
        << "\nTempo médio de serviço: " << _avgServiceTime;
    return out.str();
}

void Simulator::DebugLog(std::string happen) {
    if (Simulator::debugmode) std::cout << happen << std::endl;
}

void Simulator::DebugLog(double instantTime, std::string happen) {
    if (Simulator::debugmode) std::cout << "T = " << instantTime << ": " << happen << std::endl;
}
