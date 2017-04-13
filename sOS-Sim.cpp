#include <cmath>
#include "sOS-Sim.h"
#include "ProcessSchedulingAlgorithms.h"

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

    // _pageStatistics
    this->_elapsedTime = 0;
    this->_processorUse = 0;
    this->_avgWaitingTime = 0;
    this->_avgResponseTime = 0;
    this->_avgTurnaroundTime = 0;
    this->_avgServiceTime = 0;
    this->_throughput = 0;
    this->_cpuIdleTime = 0;
    // from pages
    this->_pageStatistics.hits = 0;
    this->_pageStatistics.miss = 0;
}

/**
 * @return the sys is full? return true if the number of active process is equal the mutliprogramming level
 */
bool Simulator::isSysFull() {
    return (maxProcessMultiprogramming -
            (this->runningProcess.size() + this->readyQueue.size() + this->blockedQueue.size())) <= 0;
}

/**
 * Try to create process and put it on a queue
 * (if submission time == 0) create process in readyqueue (if readyQueue.size < 100) or create on readysuspendedqueue
 * (else) create in incomingqueue
 * @param _process
 * @return success
 */
bool Simulator::StartProcess(std::tuple<int, double, int, double, double, std::vector<Page>> _process) {
    try {
        Process process(_process);
        if (process.getSubmissionTime() == 0) {
            !isSysFull()? readyQueue.push_back(process) : waitingQueue.push_back(process);
            for (Page page : process.getAllPages()) Disk.push_back(page);
        }
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
    _avgServiceTime += (_elapsedTime - _process.getSubmissionTime() - _process.getWaitingTime());
    _avgTurnaroundTime += _process.getTurnaroundTime();
    DebugLog(_elapsedTime, ("Processo " + std::to_string(_process.getPID()) + " finalizado"));
}

/***
 * Check process in incoming queue and move then (if submissiontime <= 0) to ready queue
 */
void Simulator::CheckIncomingQueue() {
    for (auto i = incomingQueue.size(); i-- > 0;) {
        if (remainingSubmissionTime(incomingQueue[i]) <= 0) { // move to waitingQueue
            waitingQueue.push_back(incomingQueue[i]);
            for (Page page : incomingQueue[i].getAllPages()) Disk.push_back(page);
            incomingQueue.erase(incomingQueue.begin() + i);
            DebugLog(_elapsedTime, "Processo " + std::to_string(incomingQueue[i].getPID()) + " submetido");
        }
    }
}

/***
 * Check if the page that process need to execute righ now is in memory
 * @return page is in memory?
 */

bool Simulator::PageInMemory() {
    if (runningProcess[0].getPage().getLifeTime() == 0) return true; // if is an empty page, process not need a page in memory now
    for (int i = 0; i < memoryFrames.size(); i++)
        if (memoryFrames[i].getValue() == runningProcess[0].getPage().getValue()
            && memoryFrames[i].getPID() == runningProcess[0].getPage().getPID()) {
            memoryFrames[i].setUsed(_elapsedTime);
            _pageStatistics.hits++;
            return true;
        }
    _pageStatistics.miss++;
    return false;
}


/***
 * Remove all pages linked w/ the process with pid = PID from memory
 */
void Simulator::RemovePages(uint32_t PID) {
    for (int i = 0; i < memoryFrames.size(); i++)
        if (memoryFrames[i].getPID() == PID) memoryFrames.erase(memoryFrames.begin()+i);
}


/***
 * Check if the process running has quantum time and if it has remaing time to execute, else send to block process
 */
void Simulator::CheckRunningProcess() {
    if (!runningProcess.empty()) {
        runningProcess[0].decrementExecutionTime();
        runningProcess[0].decrementPageLifeTime();
        if (runningProcess[0].getExecutionTime() < 1) {
            if (runningProcess[0].getBlockTime() > 0) { // block process
                if (!waitingQueue.empty()) { // if are process waiting, block and suspense process
                    blockedSuspensedQueue.push_back(runningProcess[0]);
                    RemovePages(runningProcess[0].getPID());
                    DebugLog(_elapsedTime,
                             ("Processo " + std::to_string(runningProcess[0].getPID()) + " bloqueado e suspenso"));
                } else { // just block processs
                    blockedQueue.push_back(runningProcess[0]);
                    DebugLog(_elapsedTime,
                             ("Processo " + std::to_string(runningProcess[0].getPID()) + " bloqueado e pronto"));
                }
                runningProcess.erase(runningProcess.begin());
            } else { // terminate process
                runningProcess[0].setTurnaroundTime(_elapsedTime);
                runningProcess[0].setWaitingTime(_elapsedTime);
                TerminateProcess(runningProcess[0]);
                runningProcess.erase(runningProcess.begin());
            }
        } else if (this->_quantum < 1) { // send process back to ready queue
            readyQueue.push_back(runningProcess[0]);
            DebugLog(_elapsedTime,
                     ("Processo " + std::to_string(runningProcess[0].getPID()) + " pronto"));
            runningProcess.erase(runningProcess.begin());
        }
    }
}


/***
 * Check process in blockedqueue and blockedSuspensedQueue and decrement block time from this process if blocktime
 * from a process is over, then send it back to readyqueue. If has process waiting, it has priority to come to readyQueue
 * The process will be finnished when it come back to runningProcess
 */
void Simulator::CheckBlockedQueue() {
    for (auto i = blockedQueue.size(); i-- > 0; ) { // checks blocked queue
        if (blockedQueue[i].getBlockTime() > 0) blockedQueue[i].decrementBlockTime();
        if (blockedQueue[i].getBlockTime() <= 0 && waitingQueue.empty()) { // if no block time remains and waiting queue is empty, send it to ready queue
            readyQueue.push_back(blockedQueue[i]);
            blockedQueue.erase(blockedQueue.begin() + i);
        }
    }
    for (auto i = blockedSuspensedQueue.size(); i-- > 0; ) { // checks blocked suspensed queue
        if (blockedSuspensedQueue[i].getBlockTime() > 0) blockedSuspensedQueue[i].decrementBlockTime();
        if (blockedSuspensedQueue[i].getBlockTime() <= 0) {// if no block time remains, send it to ready suspensed queue
            readySuspensedQueue.push_back(blockedSuspensedQueue[i]);
            blockedSuspensedQueue.erase(blockedSuspensedQueue.begin() + i);
        }
    }
}


/***
 * Check each suspensed process and, if has space on memory, put it in non active (non suspensed) mode
 */
void Simulator::CheckReadySuspensedQueue() {
    if (waitingQueue.empty() && !readySuspensedQueue.empty() && !isSysFull()) // if has no process waiting to born
        ProcessSchedulingAlgorithms::FCFS(&readySuspensedQueue, &readyQueue, _elapsedTime);
}


/***
 * Call function to check all queues
 */
void Simulator::CheckQueues() {
    CheckIncomingQueue();
    CheckBlockedQueue();
    CheckReadySuspensedQueue();
    CheckRunningProcess();
}


/***
 * Checks if has process in one of queues
 * @return incomingqueue and readyqueue and blockedqueue are empty?
 */
bool Simulator::EmptyQueue() {
    return incomingQueue.empty() && waitingQueue.empty() &&readySuspensedQueue.empty()
           && readyQueue.empty() && blockedQueue.empty() &&blockedSuspensedQueue.empty() && _cpuIdle;
}


/***
 * Start the simulation using the algorithm parameter-passed to schedule process
 * with the end of simulation statistics are calculated
 * @param shortTermSchedulingAlgorithm - pointer to the short time scheduling algorithm
 * @param process - a vector of tuples with information of process
 */
void Simulator::StartSimulation(
        bool (*shortTermSchedulingAlgorithm)(std::vector<Process>*, std::vector<Process>*, int*, double),
        bool (*pageReplacementAlgorithm)(std::vector<Page>*, std::vector<Page>*, Page, double),
        std::vector<std::tuple<int, double, int, double, double,std::vector<Page>>> process) {

    DebugLog("Inicio da simulação:");

    // For each process, create and put it on a queue
    for (std::tuple<int, double, int, double, double, std::vector<Page>> _process : process)
        StartProcess(_process);

    // Clear vector of tuples
    process.clear();

    for (;!EmptyQueue(); lastUpdate = time(NULL))
        if (time(NULL) - lastUpdate >= SPEED_) {
            // Check queues
            CheckQueues();

            // Medium-term scheduling
            while (!isSysFull()) {
                if (!waitingQueue.empty()) {
                    if (ProcessSchedulingAlgorithms::FCFS(&waitingQueue, &readyQueue, _elapsedTime))
                        pageReplacementAlgorithm(&memoryFrames, &Disk, readyQueue.back().getPage(), _elapsedTime);
                } else break;
            }

            // Short-term scheduling
            if (noProcessRunning())
                _cpuIdle = !shortTermSchedulingAlgorithm(&readyQueue, &runningProcess, &_quantum, _elapsedTime);

            // Check if page of running process is in memory
            if (!_cpuIdle && !PageInMemory())
                pageReplacementAlgorithm(&memoryFrames, &Disk, runningProcess[0].getPage(), _elapsedTime);

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
    _processorUse = (static_cast<double>(_elapsedTime) - _cpuIdleTime) / _elapsedTime * 100;
    _throughput = static_cast<double>(countProcess) / _elapsedTime * 100;
    _avgWaitingTime /= countProcess;
    _avgResponseTime /= countProcess;
    _avgTurnaroundTime /= countProcess;
    _avgServiceTime /= countProcess;
    _pageStatistics.hitRate = (static_cast<double>(_pageStatistics.hits) / (_pageStatistics.miss + _pageStatistics.hits)) * 100;
}

/**
 * Print results from simulation
 */
std::string Simulator::getResults() {
    std::stringstream out;
    out << "*******************************\n";
    out << "Duração da Simulação (Simulation duration): " << _elapsedTime
        << "\nUtilização do processador (CPU efficiency): " << _processorUse
        << "%\nVazão (Throughput): " << _throughput
        << "%\nTempo médio de espera (Average waiting time): " << _avgWaitingTime
        << "\nTempo médio de resposta (Average response time): "
        << _avgResponseTime << "\nTempo médio de retorno (Average turnaround time): " << _avgTurnaroundTime
        << "\nTempo médio de serviço (Average service time): " << _avgServiceTime
        << "\nEstatísticas da paginação (Paging Statistics):" << "\n\tHits: " << _pageStatistics.hits
        << "\n\tMiss: " << _pageStatistics.miss
        << "\n\tHit Rate: " << std::setprecision(3) << _pageStatistics.hitRate << "%";
    return out.str();
}

/***
 * If in debugmode print debug msg
 */
void Simulator::DebugLog(std::string happen) {
    if (Simulator::debugmode) std::cout << happen << std::endl;
}

/***
 * If in debugmode print debug msg w/ instantTime
 */
void Simulator::DebugLog(double instantTime, std::string happen) {
    if (Simulator::debugmode) std::cout << "\tT = " << instantTime << ": " << happen << std::endl;
}

/***
 * Clear the system status, important if want to execute another algorithm in same set of process and pages
 */
void Simulator::Clear(int maxMultiprogramming, bool step_by_step, bool debugmode) {

	this->_cpuIdle = true;
	this->_quantum = 0;
	this->debugmode = debugmode;
	this->SPEED_ = step_by_step? 1/2.0 : 0;
	this->maxProcessMultiprogramming = maxMultiprogramming;

	this->lastUpdate = 0;
	this->countProcess = 0;
	// _pageStatistics
	this->_elapsedTime = 0;
	this->_processorUse = 0;
	this->_avgWaitingTime = 0;
	this->_avgResponseTime = 0;
	this->_avgTurnaroundTime = 0;
	this->_avgServiceTime = 0;
	this->_throughput = 0;
	this->_cpuIdleTime = 0;

	//queues
	this->incomingQueue.clear();
	this->waitingQueue.clear();
	this->readyQueue.clear();
    this->readySuspensedQueue.clear();
	this->runningProcess.clear();
	this->blockedQueue.clear();
    this->blockedSuspensedQueue.clear();

	this->out.clear();
}