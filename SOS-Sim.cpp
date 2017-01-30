#include "SOS-Sim.h"

Simulator::Simulator() {
    this->startTime = time(NULL);

    this->_elapsedTime = 0f;
    this->_processorUse = 0f;
    this->_avgWaitingTime = 0f;
    this->_avgResponseTime = 0f;
    this->_avgTurnaroungTime = 0f;
    this->_avgServiceTime = 0f;
    this->_throughput = 0f;
}

/**
 * Try to create process and put it on a queue
 * @param _process
 * @return success
 */
bool Simulator::NewProcess(std::tuple<int, int, float, float, float> _process) {
    try {
        Process process = new Process(_process);
        // put process in ready queue (if submission time == 0) or incoming queue (else)
        process.getSubmissionTime() == 0? readyQueue.push_back(process): incomingQueue.push_back(process);
        return true;
    } catch (...) {
            return false;
    }

}

/***
 * Update elapsedTime and increments i in 10 and 10 seconds
 * @param i
 */
void Simulator::UpdateTime(uint32_t &i) {
    elapsedTime = this->startTime - time(NULL);
    if (elapsedTime >= i*10) i++;
}

Simulator::StartSimulation() {
    // Checks if has process in one of queues
    auto emptyQueue = []() { return incomingQueue.empty()
               && readyQueue.empty() && blockedQueue.empty(); };
    for (uint32_t* i = new int(0); !emptyQueue(); UpdateTime(i)){
        /***
         * pega processos aqui dentro e executa
         */
    }
    CalcStatistics();
}

/**
 * Calc statistics and update variables
 */
Simulator::CalcStatistics() {

}

/**
 * Print results from simulation
 */
Simulator::PrintResults() {
    std::cout << "Duração da Simulação: " << this->_totalTime << "\nEficiência: " << this->_processorUse
              << "\nVazão: " << this->_thoughput << "\nTempo médio de espera: " << this->_avgWaitingTime
              << "\nTempo médio de resposta: " << this->_avgResponseTime
              << "\nTempo médio de retorno: " << this->_avgTurnaroundTime
              << "\nTempo médio de serviço: " << this->_avgServiceTime;
}