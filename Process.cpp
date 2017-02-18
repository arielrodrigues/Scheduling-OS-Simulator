#include "Process.h"

Process::Process() {
    this->PID = 0;
    this->submissionTime = 0;
    this->priority = 0;
    this->executionTime = 0;
    this->blockTime = 0;

    this->waitingTime = 0;
    this->responseTime = 0;

	executionTime__ = executionTime;
}

Process::Process(std::tuple<int, double, int, double, double> _process) {
    this->PID = _getPID(_process);
    this->submissionTime = _getSubmissionTime(_process);
    this->priority = _getPriority(_process);
    this->executionTime = _getExecutionTime(_process);
    this->blockTime = _getBlockedTime(_process);
    this->waitingTime = -1;
    this->responseTime = -1;

	executionTime__ = executionTime;
}

int Process::getPID() {
    return PID;
}

double Process::getSubmissionTime() {
    return submissionTime;
}

int Process::getPriority() {
    return priority;
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

double Process::getTurnaroundTime() {
	return turnaroundTime;
}

void Process::setResponseTime(double responseTime) {
    this->responseTime = responseTime - this->submissionTime;
}

void Process::setWaitingTime(double _elapsedTime) {
    this->waitingTime = _elapsedTime - (this->submissionTime + this->executionTime__);
}

void Process::setTurnaroundTime(double _elapsedTime) {
	this->turnaroundTime = _elapsedTime - submissionTime;
}