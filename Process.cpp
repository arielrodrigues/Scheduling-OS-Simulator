#include "Process.h"

Process::Process() {
    this->PID = 0;
    this->priority = 0;
    this->submissionTime = 0;
    this->executionTime = 0;
    this->blockTime = 0;
}

Process::Process(std::tuple<int, int, float, float, float> _process) {
    this->PID = _getPID(_process);
    this->priority = _getPriority(_process);
    this->submissionTime = _getSubmissionTime(_process);
    this->executionTime = _getExecutionTime(_process);
    this->blockTime = _getBlocketTime(_process);
}

void Process::decrementBlockTime() {
    this->blockTime--;
}

void Process::decrementExecutionTime() {
    this->executionTime--;
}

void Process::decrementSubmissionTime() {
    this->submissionTime--;
}

void Process::decrementWaitingTime() {
    this->waitingTime--;
}

int Process::getPID() {
    return PID;
}

int Process::getPriority() {
    return priority;
}

float Process::getSubmissionTime() {
    return submissionTime;
}

float Process::getExecutionTime() {
    return executionTime;
}

float Process::getBlockTime() {
    return blockTime;
}

float Process::getResponseTime() {
    return responseTime;
}

float Process::getWaitingTime() {
    return waitingTime;
}

