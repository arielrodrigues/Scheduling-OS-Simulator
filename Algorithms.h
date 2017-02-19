#ifndef SIMPLE_OS_SIMULATOR_ALGORITHMS_H
#define SIMPLE_OS_SIMULATOR_ALGORITHMS_H

#define firstTimeRunning(_process) _process.getResponseTime() == -1

#include <vector>
#include <ctime>
#include <cstdlib>
#include <limits>
#include "Process.h"
#include "sOS-Sim.h"

namespace Algorithms {

	/***
	 * First-Come-First-Served (FCFS) scheduling algorithm [also know as First-In-Fist-Out (FIFO)]
	 * Used to medium-term Scheduling
	 * @return (*readysuspendedQueue)[0]
	 */
    static bool FCFS(std::vector<Process>* readysuspendedQueue, std::vector<Process>* readyQueue, double _elapsedTime) {
        try {
            if (!readysuspendedQueue->empty()) {
                Simulator::DebugLog(_elapsedTime,
                                    "Processo " + std::to_string((*readysuspendedQueue)[0].getPID()) + " pronto");

	            readyQueue->push_back((*readysuspendedQueue)[0]);
                readysuspendedQueue->erase(readysuspendedQueue->begin());
                return true;
            } else return false;
        }
        catch (...) {
            std::cout << "Erro no escalonamento. Nenhum processo escalonado\n";
            return false;
        }
    }

	/***
	 * High-Response-Ration-Next (HRRN) scheduling algorithm
	 * Used to short-term Scheduling
	 * @return process with max((executionTime + waitingTime)/executionTime) in readyQueue
	 */
    static bool HRRN(std::vector<Process>* readyQueue, std::vector<Process>* runningList, int* _quantum, double _elapsedTime) {
        try {
            if (!readyQueue->empty()) {
                int i = 0, hrr = 0;
                for (auto responseratio = 0.0, maior = 0.0; i < (*readyQueue).size(); i++) {
	                (*readyQueue)[i].setWaitingTime(_elapsedTime);
                    responseratio = 1 + (*readyQueue)[i].getWaitingTime() / (*readyQueue)[i].getExecutionTime();
                    if (responseratio > maior) {
                        maior = responseratio;
                        hrr = i;
                    }
                }

	            // set Quantum as limitless
                (*_quantum) = std::numeric_limits<int>::max();
	            // set response time
	            if (firstTimeRunning((*readyQueue)[hrr])) (*readyQueue)[hrr].setResponseTime(_elapsedTime);


	            Simulator::DebugLog(_elapsedTime,
	                                "Processo " + std::to_string((*readyQueue)[hrr].getPID()) + " em execução");

	            // set as running process
                runningList->push_back((*readyQueue)[hrr]);
                readyQueue->erase(readyQueue->begin()+hrr);
                return true;
            } else return false;
        }
        catch (...){
            std::cout << "Erro no escalonamento. Nenhum processo escalonado\n";
            return false;
        }
    }

	/***
	 * Priority scheduling algorithm
	 * Used to short-term Scheduling
	 * @return process with max(priority) in readyQueue
	 */
    static bool PRIORITY(std::vector<Process> *readyQueue, std::vector<Process> *runningList, int* _quantum, double _elapsedTime) {
        try {
            if (!readyQueue->empty()) {
                int maxpriority = 0, priority = 0, i = 0;
                for (Process process: (*readyQueue)) {
                    if (process.getPriority() > priority) {
                        priority = process.getPriority();
                        maxpriority = i;
                    } i++;
                }

	            // set Quantum as limitless
	            (*_quantum) = std::numeric_limits<int>::max();
	            // set response time
	            if (firstTimeRunning((*readyQueue)[maxpriority])) (*readyQueue)[maxpriority].setResponseTime(_elapsedTime);
	            Simulator::DebugLog(_elapsedTime,
	                                "Processo " + std::to_string((*readyQueue)[maxpriority].getPID()) + " em execução");

	            // set as running process
                runningList->push_back((*readyQueue)[maxpriority]);
                readyQueue->erase(readyQueue->begin()+maxpriority);
                return true;
            } else return false;
        }
        catch (...) {
            std::cout << "Erro no escalonamento. Nenhum processo escalonado\n";
            return false;
        }
    }

	/***
	 * Lottery scheduling algorithm w/ quantum = 2
	 * Used to short-term Scheduling
	 * @return a semi-randomly chosen process, each process has (process_priority+1)*10 tickets
	 */
    static bool LOTTERY(std::vector<Process> *readyQueue, std::vector<Process> *runningList, int* _quantum, double _elapsedTime) {
        try {
            if (!readyQueue->empty()) {
                int i = 0, initickets = 0, countTickets = 0, winner = 0;

	            // initialize vector of tickets
                for (Process process: (*readyQueue)) countTickets += (process.getPriority() + 1) * 10;
                int *tickets = new int [countTickets]; countTickets = 0;

	            // make semi-random choice
                for (Process process: (*readyQueue)) {
                    initickets = countTickets;
                    countTickets += (process.getPriority() + 1) * 10;
                    for (int j = initickets; j < countTickets; j++){
                        tickets[j] = i;
                    } i++;
                }
                srand((unsigned)time(0));
                winner = tickets[rand()%(countTickets + 1)];

	            // set Quantum as 2
                (*_quantum) = 2;
	            // set response time
	            if (firstTimeRunning((*readyQueue)[winner])) (*readyQueue)[winner].setResponseTime(_elapsedTime);

	            Simulator::DebugLog(_elapsedTime,
	                                "Processo " + std::to_string((*readyQueue)[winner].getPID()) + " em execução");

	            // set as running process
	            runningList->push_back((*readyQueue)[winner]);
                readyQueue->erase(readyQueue->begin()+winner);
                return true;
            } else return false;
        }
        catch (...) {
            std::cout << "Erro no escalonamento. Nenhum processo escalonado\n";
            return false;
        }
    }

	/***
	 * Round Robin (RR) scheduling algorithm w/ quantum = 4
	 * Used to short-term Scheduling
	 * @return (*readyQueue)[0]
	 */
    static bool RR(std::vector<Process>* readyQueue, std::vector<Process>* runningList, int* _quantum, double _elapsedTime) {
        try {
            if (!readyQueue->empty()) {

	            // set Quantum as 4
	            (*_quantum) = 4;
				// set response time
	            if (firstTimeRunning((*readyQueue)[0])) (*readyQueue)[0].setResponseTime(_elapsedTime);

	            Simulator::DebugLog(_elapsedTime,
	                                "Processo " + std::to_string((*readyQueue)[0].getPID()) + " em execução");

	            // set process as running
	            runningList->push_back((*readyQueue)[0]);
                readyQueue->erase(readyQueue->begin());
                return true;
            } else return false;
        } catch (...) {
            std::cout << "Erro no escalonamento. Nenhum processo escalonado\n";
            return false;
        }
    }

    static bool SD(std::vector<Process>* readyQueue, std::vector<Process>* runningList, int* _quantum, double _elapsedTime) {
        try {
            if (!readyQueue->empty()) {

	            // set Quantum
	            (*_quantum) = maxProcessMultiprogramming / (*readyQueue).size();
				// set response time
	            if (firstTimeRunning((*readyQueue)[0])) (*readyQueue)[0].setResponseTime(_elapsedTime);

	            Simulator::DebugLog(_elapsedTime,
	                                "Processo " + std::to_string((*readyQueue)[0].getPID()) + " em execução");

	            // set process as running
	            runningList->push_back((*readyQueue)[0]);
                readyQueue->erase(readyQueue->begin());
                return true;
            } else return false;
        } catch (...) {
            std::cout << "Erro no escalonamento. Nenhum processo escalonado\n";
            return false;
        }
    }

    static bool FEEDBACK(std::vector<Process> *readyQueue, std::vector<Process> *runningList, int* _quantum, double _elapsedTime) {
        try {
            if (!readyQueue->empty()) {
                for (Process process: (*readyQueue)){
                    if (process.getTimesExecuted() == 0) process.setPriority(0);
                    else if (process.getTimesExecuted() == 1) process.setPriority(1);
                    else if (process.getTimesExecuted() == 2) process.setPriority(2);
                    else if (process.getTimesExecuted() >= 3) process.setPriority(3);
                }

                int minpriority = 0, priority = 0, i = 0;
                for (Process process: (*readyQueue)) {
                    if (process.getPriority() < priority) {
                        priority = process.getPriority();
                        minpriority = i;
                    } i++;
                }

                (*readyQueue)[minpriority].incrementTimesExecuted();

                // set Quantum according to priority
                if (priority == 0) (*_quantum) = 1;
                else if (priority == 1) (*_quantum) = 2;
                else if (priority == 2) (*_quantum) = 4;
                else if (priority == 3) (*_quantum) = 8;
                // set response time
                if (firstTimeRunning((*readyQueue)[minpriority])) (*readyQueue)[minpriority].setResponseTime(_elapsedTime);
                Simulator::DebugLog(_elapsedTime,
                                    "Processo " + std::to_string((*readyQueue)[minpriority].getPID()) + " em execução");

                // set as running process
                runningList->push_back((*readyQueue)[minpriority]);
                readyQueue->erase(readyQueue->begin()+minpriority);
                return true;
            } else return false;
        }
        catch (...) {
            std::cout << "Erro no escalonamento. Nenhum processo escalonado\n";
            return false;
        }
    }
    
}


#endif //SIMPLE_OS_SIMULATOR_ALGORITHMS_H
