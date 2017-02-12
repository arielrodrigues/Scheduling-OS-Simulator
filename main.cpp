#include <iostream>
#include <fstream>
#include "sOS-Sim.h"
#include "Algorithms.h"

std::vector<std::tuple<int, int, float, float, float>> process;

void filetoTuple(std::string filename) {
    using namespace std;

    ifstream file(filename.c_str(), ifstream::in);
    float submissionTime, executionTime, blockTime;
    int PID, priority;
    string buffer;

    if (!file.is_open()) {
        cout << "Unable to open file." << endl;
        exit(EXIT_FAILURE);
    } else {
        while(file.good()) {
            getline(file, buffer, ','); PID = stoi(buffer);
            getline(file, buffer, ','); priority = stoi(buffer);
            getline(file, buffer, ','); submissionTime = stoi(buffer);
            getline(file, buffer, ','); executionTime = stoi(buffer);
            getline(file, buffer); blockTime = stoi(buffer);
            process.push_back(std::tuple<int, int, float, float, float>(PID, priority, submissionTime,
            executionTime, blockTime));
        }
    }
}

int main() {
    Simulator sim(2, 1, true);
    bool (*FCFS)(std::vector <Process>*, std::vector<Process>*, double) = Algorithms::FCFS;
    filetoTuple("/home/ariel/ClionProjects/Simple-OS-Simulator/teste.txt");

    sim.StartSimulation(FCFS, process);
    std::cout << sim.getResults() << std::endl;
    return 0;
}
