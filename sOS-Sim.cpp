#include "sOS-Sim.h"

Simulator::Simulator(int maxMultiprogramming, int speed) {
    this->startTime = time(NULL);
    this->maxProcessMultiprogramming = maxMultiprogramming;
    this->SPEED_ = speed/5;

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
bool Simulator::NewProcess(std::tuple<int, int, float, float, float> _process) {
    try {
        Process process(_process);
        // put process in ready queue (if submission time == 0) or incoming queue (else)
        process.getSubmissionTime() == 0? readyQueue.push_back(process): incomingQueue.push_back(process);

        this->processCounter++;
        return true;
    } catch (...) {
            return false;
    }

}


/***
 * Check process in read queue and move then (if executiontime <= 0 ) to block queue
 *
 */
void Simulator::CheckReadyQueue() {
    auto i = 0;
    for (auto _process: readyQueue) {
        if (_process.getExecutionTime() <= 0) { // move to blockedQueue
            blockedQueue.push_back(readyQueue[i]);
            readyQueue.erase(readyQueue.begin() + i);
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
 * Update elapsedTime and increments i in 10 and 10 seconds
 * @param i
 */
void Simulator::UpdateTime(uint32_t &i) {
    _elapsedTime = this->startTime - time(NULL);
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

void Simulator::StartSimulation() {
    for (uint32_t* i = new uint32_t(0); !EmptyQueue(); UpdateTime(*i)){
        /***
         * pega processos aqui dentro e executa
         */
    }
    CalcStatistics();
}

/**
 * Calc statistics and update variables
 */
void Simulator::CalcStatistics() {

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
