#ifndef SIMPLE_OS_SIMULATOR_ALGORITHMS_H
#define SIMPLE_OS_SIMULATOR_ALGORITHMS_H

#include <vector>
#include <ctime>
#include <cstdlib>
#include <limits>
#include "Process.h"
#include "sOS-Sim.h"

namespace Algorithms {

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

    static bool HRRN(std::vector<Process>* readyQueue, std::vector<Process>* runningList, int* _quantum, double _elapsedTime) {
        try {
            if (!readyQueue->empty()) {
                double responseratio = 0, maior = 0;
                int i = 0, hrr = 0;
                for (Process process: (*readyQueue)) {
                    process.setWaitingTime(_elapsedTime);
                    responseratio = 1 + process.getWaitingTime() / process.getExecutionTime();
                    if (responseratio > maior) {
                        maior = responseratio;
                        hrr = i;
                    }i++;
                }

                (*_quantum) = std::numeric_limits<int>::max();
	            if ((*readyQueue)[hrr].getResponseTime() == -1)
		            (*readyQueue)[hrr].setResponseTime(_elapsedTime);

                runningList->push_back((*readyQueue)[hrr]);
                readyQueue->erase(readyQueue->begin()+hrr);
                Simulator::DebugLog(_elapsedTime,
                                    "Processo " + std::to_string((*readyQueue)[hrr].getPID()) + " em execução");
                return true;
            } else return false;
        }
        catch (...){
            std::cout << "Erro no escalonamento. Nenhum processo escalonado\n";
            return false;
        }
    }

    static bool PRIORITY(std::vector<Process> *readyQueue, std::vector<Process> *runningList, int* _quantum, double _elapsedTime) {
        try {
            if (!readyQueue->empty()) {
                int maiorprioridade = 0, prioridade = 0, i = 0;
                for (Process process: (*readyQueue)) {
                    if (process.getPriority() < prioridade) {
                        prioridade = process.getPriority();
                        maiorprioridade = i;
                    } i++;
                }

	            (*_quantum) = std::numeric_limits<int>::max();
				if ((*readyQueue)[maiorprioridade].getResponseTime() == -1)
					(*readyQueue)[maiorprioridade].setResponseTime(_elapsedTime);
	            Simulator::DebugLog(_elapsedTime,
	                                "Processo " + std::to_string((*readyQueue)[maiorprioridade].getPID()) + " em execução");

                runningList->push_back((*readyQueue)[maiorprioridade]);
                readyQueue->erase(readyQueue->begin()+maiorprioridade);
                return true;
            } else return false;
        }
        catch (...) {
            std::cout << "Erro no escalonamento. Nenhum processo escalonado\n";
            return false;
        }
    }

    static bool LOTTERY(std::vector<Process> *readyQueue, std::vector<Process> *runningList, int* _quantum, double _elapsedTime) {
        try {
            if (!readyQueue->empty()) {
                int i = 0, initickets = 0, ticketatual = 0, winner = 0, processo = 0;
                for (Process process: (*readyQueue)) ticketatual += (process.getPriority() + 1) * 10;
                int *tickets = new int [ticketatual];
                ticketatual = 0;
                for (Process process: (*readyQueue)) {
                    initickets = ticketatual;
                    ticketatual += (process.getPriority() + 1) * 10;
                    for (int j = initickets; j < ticketatual; j++){
                        tickets[j] = i;
                    }
                    i++;
                }
                srand((unsigned)time(0));
                winner = rand()%(ticketatual + 1);
                processo = tickets[winner];

                (*_quantum) = 2;
	            if ((*readyQueue)[processo].getResponseTime() == -1)
		            (*readyQueue)[processo].setResponseTime(_elapsedTime);

	            runningList->push_back((*readyQueue)[processo]);
                readyQueue->erase(readyQueue->begin()+processo);
                Simulator::DebugLog(_elapsedTime,
                                    "Processo " + std::to_string((*readyQueue)[processo].getPID()) + " em execução");
                return true;
            } else return false;
        }
        catch (...) {
            std::cout << "Erro no escalonamento. Nenhum processo escalonado\n";
            return false;
        }
    }

    static bool RR(std::vector<Process>* readyQueue, std::vector<Process>* runningList, int* _quantum, double _elapsedTime) {
        try {
            if (!readyQueue->empty()) {

                (*_quantum) = 4;
	            if ((*readyQueue)[0].getResponseTime() == -1)
		            (*readyQueue)[0].setResponseTime(_elapsedTime);

	            runningList->push_back((*readyQueue)[0]);
                readyQueue->erase(readyQueue->begin());
                Simulator::DebugLog(_elapsedTime,
                                    "Processo " + std::to_string((*readyQueue)[0].getPID()) + " em execução");
                return true;
            } else return false;
        } catch (...) {
            std::cout << "Erro no escalonamento. Nenhum processo escalonado\n";
            return false;
        }
    }

}


#endif //SIMPLE_OS_SIMULATOR_ALGORITHMS_H
