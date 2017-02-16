#include "Process.h"

Process::Process() {
    this->PID = 0;
    this->priority = 0;
    this->submissionTime = 0;
    this->executionTime = 0;
    this->blockTime = 0;

    this->waitingTime = 0;
    this->responseTime = 0;
    this->quantum = 0;
    this->lastTimeRunning = 0;
}

Process::Process(std::tuple<int, int, double, double, double> _process) {
    this->PID = _getPID(_process);
    this->priority = _getPriority(_process);
    this->submissionTime = _getSubmissionTime(_process);
    this->executionTime = _getExecutionTime(_process);
    this->blockTime = _getBlockedTime(_process);
    this->waitingTime = 0;
    this->responseTime = 0;
    this->quantum = 0;
    this->lastTimeRunning = 0;
}

int Process::getPID() {
    return PID;
}

int Process::getPriority() {
    return priority;
}

int Process::getQuantum() {
    return quantum;
}

double Process::getLastTimeRunning() {
    return lastTimeRunning;
}

double Process::getSubmissionTime() {
    return submissionTime;
}

double Process::getExecutionTime() {
    return executionTime;
}

double Process::getBlockTime() {
    return blockTime;
}

double Process::getResponseTime() {
    return responseTime;
}

double Process::getWaitingTime() {
    return waitingTime;
}

void Process::setResponseTime(double responseTime) {
    this->responseTime = responseTime - this->submissionTime;
}

void Process::setWaitingTime(double waitingTime) {
    this->waitingTime = waitingTime - this->submissionTime;
}

void Process::setQuantum(int quantum) {
    this->quantum = quantum;
}

void Process::setLasTimeRunning(double lastTimeRunning) {
    this->lastTimeRunning = lastTimeRunning;
}

void Process::decrementQuantum() {
    this->quantum--;
}

void Process::updateWaitingTime(double waitingTime) {
    this->waitingTime += waitingTime;
}