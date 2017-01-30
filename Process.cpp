#include "Process.h"

Process::Process() {
    this->PID = 0;
    this->priority = 0;
    this->submissionTime = 0;
    this->executionTime = 0;
    this->blockTime = 0;
}

Process::Process(std::tuple<int, int, float, float, float> _process) {
    this.PID = _getPID(_process);
    this.priority = _getPriority(_process);
    this.submissionTime = _getSubmissionTime(_process);
    this.executionTime = _getExecutionTime(_process);
    this.blockTime = _getBlocketTime(_process);
}

Process::decrementBlockTime() {
    this->blockTime--;
}

Process::decrementExecutionTime() {
    this->executionTime--;
}

Process::decrementSubmissionTime() {
    this->submissionTime--;
}

float Process::getPID() {
    return PID;
}

float Process::getPriority() {
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