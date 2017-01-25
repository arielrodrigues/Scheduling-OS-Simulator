#include "Process.h"

Process::Process() {

}

Process::Process(int PID, int priority, float submissionTime, float executionTime, float blockTime) {
    this.PID = PID;
    this.priority = priority;
    this.submissionTime = submissionTime;
    this.executionTime = executionTime;
    this.blockTime = blockTime;
}

float Process::_getblocktime() {
    return blockTime;
}