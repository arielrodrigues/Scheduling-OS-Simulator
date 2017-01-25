#ifndef SIMPLE_OS_SIMULATOR_PROCESS_H
#define SIMPLE_OS_SIMULATOR_PROCESS_H


class Process {
public:
    Process();
    Process(int PID, int priority, float submissionTime, float executionTime, float blockTime);
    int _getpid();
    int _getprioriy();
    float _getexecutiontime();
    float _getblocktime();
    float _getsubmissiontime();

private:
    // tudo isso pode ser uma tupla Process(pid, priority, submissiontime, executiontime, blocktime);
    int PID, priority;
    float submissionTime, executionTime, blockTime;
    //usar ou não usar camelCase??
};


#endif //SIMPLE_OS_SIMULATOR_PROCESS_H
