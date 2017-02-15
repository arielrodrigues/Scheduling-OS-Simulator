#ifndef SIMPLE_OS_SIMULATOR_ALGORITHMS_H
#define SIMPLE_OS_SIMULATOR_ALGORITHMS_H
#include "sOS-Sim.h"


#include <vector>
#include <ctime>
#include <cstdlib>
#include "Process.h"

namespace Algorithms {

    static bool FCFS(std::vector<Process>* readyQueue, std::vector<Process>* runningList, double _elapsedTime) {
        try {
            if (!readyQueue->empty()) {
                Simulator::DebugLog(_elapsedTime,
                                    "Processo " + std::to_string((*readyQueue)[0].getPID()) + " em execução");
                runningList->push_back((*readyQueue)[0]);
                readyQueue->erase(readyQueue->begin());
            } return true;
        }
        catch (...) {
            return false;
        }
    }

    static bool HRRN(std::vector<Process>* readyQueue, std::vector<Process>* runningList, double _elapsedTime) {
        try {
            if (!readyQueue->empty()) {
                double responseratio = 0, maior = 0;
                int i = 0, hrr = 0;
                for (Process process: (*readyQueue)) {
                    process.setWaitingTime(_elapsedTime);
                    responseratio =
                            (process.getExecutionTime() + process.getWaitingTime()) / process.getExecutionTime();
                    if (responseratio > maior) {
                        maior = responseratio;
                        hrr = i;
                    }
                    i++;
                }
                Simulator::DebugLog(_elapsedTime,
                                    "Processo " + std::to_string((*readyQueue)[hrr].getPID()) + " em execução");
                runningList->push_back((*readyQueue)[hrr]);
                readyQueue->erase(readyQueue->begin()+hrr);
            } return true;
        }
        catch (...){
            return false;
        }
    }

    static bool Priority(std::vector<Process>* readyQueue, std::vector<Process>* runningList, double _elapsedTime) {
        try {
            if (!readyQueue->empty()) {
                int maiorprioridade = 0, prioridade = 0, i = 0;
                for (Process process: (*readyQueue)) {
                    if (process.getPriority() > prioridade) {
                        prioridade = process.getPriority();
                        maiorprioridade = i;
                    }
                    i++;
                }
                Simulator::DebugLog(_elapsedTime,
                                    "Processo " + std::to_string((*readyQueue)[maiorprioridade].getPID()) + " em execução");
                runningList->push_back((*readyQueue)[maiorprioridade]);
                readyQueue->erase(readyQueue->begin()+maiorprioridade);
            } return true;
        }
        catch (...) {
            return false;
        }
    }

    static bool Lottery(std::vector<Process>* readyQueue, std::vector<Process>* runningList, double _elapsedTime) {
        try {
            if (!readyQueue->empty()) {
                int i = 0, initickets = 0, ticketatual = 0, winner = 0, processo = 0;
                for (Process process: (*readyQueue)) {
                    process.setQuantum(2);
                    ticketatual += (process.getPriority() + 1) * 10;
                }
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
                Simulator::DebugLog(_elapsedTime,
                                    "Processo " + std::to_string((*readyQueue)[processo].getPID()) + " em execução");
                runningList->push_back((*readyQueue)[processo]);
                readyQueue->erase(readyQueue->begin()+processo);
            } return true;
        }
        catch (...) {
            return false;
        }
    }

    static bool RR(std::vector<Process>* readyQueue, std::vector<Process>* runningList, double _elapsedTime) {
        try {
            if (!readyQueue->empty()){
                for (Process process: (*readyQueue)) {
                    process.setQuantum(4);
                }
                Simulator::DebugLog(_elapsedTime,
                                    "Processo " + std::to_string((*readyQueue)[0].getPID()) + " em execução");
                runningList->push_back((*readyQueue)[0]);
                readyQueue->erase(readyQueue->begin());
            } return true;
        }
        catch (...) {
            return false;
        }
    }

}


#endif //SIMPLE_OS_SIMULATOR_ALGORITHMS_H
