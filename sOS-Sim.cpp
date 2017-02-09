#include "sOS-Sim.h"

Simulator::Simulator(int maxMultiprogramming, int speed) {
    this->startTime = time(NULL);
    this->maxProcessMultiprogramming = maxMultiprogramming;
    this->SPEED_ = speed/5;
    this->lastPID = 0;
    this->processRunningCounter = 0;

    this->_elapsedTime = 0;
    this->_processorUse = 0;
    this->_avgWaitingTime = 0;
    this->_avgResponseTime = 0;
    this->_avgTurnaroundTime = 0;
    this->_avgServiceTime = 0;
    this->_throughput = 0;
}

/**
 * Try to create process and put it on a queue
 * @param _process
 * @return success
 */
bool Simulator::StartProcess(std::tuple<int, int, float, float, float> _process) {
    try {
        Process process(_process);
        // put process in ready queue (if submission time == 0) or incoming queue (else)
        process.getSubmissionTime() == 0? readyQueue.push_back(process): incomingQueue.push_back(process);

        this->lastPID++;
        return true;
    } catch (...) {
            return false;
    }

}

void Simulator::TerminateProcess(Process _process) {
    this->_avgWaitingTime += _process.getWaitingTime();
    this->_avgResponseTime += _process.getResponseTime();
    this->_avgServiceTime += _process.getExecutionTime();
    this->_avgTurnaroundTime += (_process.getExecutionTime() + _process.getWaitingTime() + _process.getBlockTime());
}

/***
 * Check process in running queue and move then (if executiontime <= 0 ) to block queue
 *
 */
void Simulator::CheckProcessRunning() {
    auto i = 0;
    for (auto _process: runningList) {
        if (_process.getExecutionTime() <= 0) { // move to blockedQueue
            blockedQueue.push_back(runningList[i]);
            runningList.erase(runningList.begin() + i);
            processRunningCounter--;
        }
        _process.decrementExecutionTime(); i++;
    }
}

/***
 * Check process in blocked queue and move then (if blockedtime <= 0) to ready queue
 *
 */
void Simulator::CheckBlockedQueue() {
    auto i = 0;
    for (auto _process: blockedQueue) {
        if (_process.getBlockTime() <= 0) { // move to readyQueue
            readyQueue.push_back(blockedQueue[i]);
            blockedQueue.erase(blockedQueue.begin() + i);
        }
        _process.decrementBlockTime(); i++;
    }
}

/***
 * Check process in incoming queue and move then (if submissiontime <= 0) to ready queue
 *
 */
void Simulator::CheckIncomingQueue() {
    auto i = 0;
    for (auto _process: incomingQueue) {
        if (_process.getSubmissionTime() <= 0) { // move to readyQueue
            readyQueue.push_back(incomingQueue[i]);
            incomingQueue.erase(incomingQueue.begin() + i);
        }
        _process.decrementSubmissionTime(); i++;
    }
}

/***
 * Update elapsedTime and increments i in 1 and 1 seconds
 * @param i
 */
void Simulator::UpdateTime(uint32_t &i) {
    this->_elapsedTime = this->startTime - time(NULL);
    if (_elapsedTime >= i*SPEED_) i++;
}

/***
 * Checks if has process in one of queues
 * @return incomingqueue and readyqueue and blockedqueue are empty?
 */
bool Simulator::EmptyQueue() {
    return incomingQueue.empty()
           && readyQueue.empty() && blockedQueue.empty();
}

void Simulator::StartSimulation(auto* algorithm, std::vector<std::tuple<int, int, float, float, float>> process) {

    for (std::tuple<int, int, float, float, float> _process : process)
        StartProcess(_process);

    for (uint32_t* i = new uint32_t(0); !EmptyQueue(); UpdateTime(*i)){
        if (processRunningCounter < maxProcessMultiprogramming) {
            algorithm(&readyQueue, &runningList);
            /***
             * pega processos aqui dentro e executa
             */
        }
    }
    CalcStatistics();
}

/**
 * Calc statistics and update variables
 */
void Simulator::CalcStatistics() {
    this->_elapsedTime = this->startTime - time(NULL);
    //this->_processorUse = ???
    //this->_throughput /= ??
    this->_avgWaitingTime /= this->lastPID;
    this->_avgResponseTime /= this->lastPID;
    this->_avgTurnaroundTime /= this->lastPID;
    this->_avgServiceTime /= this->lastPID;
}

/**
 * Print results from simulation
 */
void Simulator::PrintResults() {
    std::cout << "Duração da Simulação: " << this->_elapsedTime << "\nEficiência: " << this->_processorUse
              << "\nVazão: " << this->_throughput << "\nTempo médio de espera: " << this->_avgWaitingTime
              << "\nTempo médio de resposta: " << this->_avgResponseTime
              << "\nTempo médio de retorno: " << this->_avgTurnaroundTime
              << "\nTempo médio de serviço: " << this->_avgServiceTime;
}
