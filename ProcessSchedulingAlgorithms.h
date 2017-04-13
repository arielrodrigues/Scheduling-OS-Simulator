#ifndef SIMPLE_OS_SIMULATOR_ALGORITHMS_H
#define SIMPLE_OS_SIMULATOR_ALGORITHMS_H

#define firstTimeRunning(_process) _process.getResponseTime() == -1

#include <vector>
#include <ctime>
#include <cstdlib>
#include <limits>
#include "Process.h"
#include "sOS-Sim.h"

namespace ProcessSchedulingAlgorithms {

	static uint32_t maxProcessMultiprogramming = 0;

	/***
	 * First-Come-First-Served (FCFS) scheduling algorithm [also know as First-In-Fist-Out (FIFO)]
	 * Used to medium-term Scheduling
	 * @return (*waitingQueue)[0]
	 */
    static bool FCFS(std::vector<Process>* waitingQueue, std::vector<Process>* readyQueue, double _elapsedTime) {
        try {
            if (!waitingQueue->empty()) {
                Simulator::DebugLog(_elapsedTime,
                                    "Processo " + std::to_string((*waitingQueue)[0].getPID()) + " pronto");

                (*waitingQueue)[0].updateSubmissionTime(_elapsedTime);
                readyQueue->push_back((*waitingQueue)[0]);
                waitingQueue->erase(waitingQueue->begin());
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
                int maxpriority = (*readyQueue)[0].getPriority(), priority = 0, i = 0;
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
    static bool LOTTERY(std::vector<Process> *readyQueue, std::vector<Process> *runningList, int* _quantum, double _elapsedTime)  {
        try {
            if (!readyQueue->empty()) {
                uint32_t countTickets = 0, winner = 0, i = 0;

	            // set ticket-range for each process
	            for (; i < (*readyQueue).size(); i++) {
		            (*readyQueue)[i].firstTicket = countTickets;
		            countTickets += ((*readyQueue)[i].getPriority() + 1) * 10;
		            (*readyQueue)[i].lastTicket = countTickets;
	            } i = 0;

	            // make a random coice for ticket
                srand((unsigned)time(0));
                winner = rand()%(countTickets + 1);

	            // get the winner process
	            for (Process process: (*readyQueue))
		            if (process.firstTicket <= winner && process.lastTicket >= winner) {
			            winner = i;
			            break;
		            } else i++;

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
	            (*_quantum) = 2;
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

	/***
	 * State Dependent (SD) scheduling algorithm w/ quantum = maxProcessMultiprogramming / size(readyQueue)
	 * Used to short-term Scheduling
	 * @return (*readyQueue)[0]
	 */
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

	/***
	 * Feedback scheduling algorithm
	 * Used to short-term Scheduling
	 * @return (*readyQueue)[minpriority]
	 */
    static bool FEEDBACK(std::vector<Process> *readyQueue, std::vector<Process> *runningList, int* _quantum, double _elapsedTime) {
        try {
            if (!readyQueue->empty()) {
                for (Process process: (*readyQueue)){
                    if (process.getTimesExecuted() == 0) process.setPriority(0);
                    else if (process.getTimesExecuted() == 1) process.setPriority(1);
                    else if (process.getTimesExecuted() == 2) process.setPriority(2);
                    else if (process.getTimesExecuted() >= 3) process.setPriority(3);
                }

                int minpriority = 0, priority = (*readyQueue)[0].getPriority(), i = 0;
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
