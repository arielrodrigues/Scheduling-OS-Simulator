#include "Simulator.h"

Simulator::Simulator() {
#pragma region inicializações_de_variaveis
    this->duracaoSimulacao = 0f;
    this->utilizacaoProcessador = 0f;
    this->tempoEsperaMedio = 0f;
    this->tempoRespostaMedio = 0f;
    this->tempoRetornoMedio = 0f;
    this->tempoServicoMedio = 0f;
    this->throughput = 0f;
#pragma endregion
}

/**
 * Try to create process and put it on a queue
 * @param _process
 * @return success
 */
bool Simulator::CreateProcess(std::tuple _process) {
    try {
        Process process = new Process(_process);
        // put process in ready queue (if submission time == 0) or incoming queue (else)
        process.getSubmissionTime() == 0? readyQueue.push_back(process): incomingQueue.push_back(process);
        return true;
    } catch (...) {
            return false;
    }

}

Simulator::StartSimulation() {

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