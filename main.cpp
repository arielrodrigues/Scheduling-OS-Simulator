#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>

#include "sOS-Sim.h"
#include "Algorithms.h"
#include "FileManager.h"

std::vector<std::tuple<int, int, double, double, double>> process;

void filetoTuple(std::string filename) {
    using namespace FileManager;

    std::ifstream file(filename.c_str(), std::ifstream::in);
    double submissionTime, executionTime, blockTime;
    int PID, priority;
    std::string buffer, PIDbuff, prioritybuff;

    std::stringstream ss = readFile(filename);
    //for (auto i = 0; ss.peek() == (decltype(ss))::traits_type::eof(); i++) {
    ss.seekp(0, std::ios::end);
    for (auto i = 0; i < ss.tellp(); i++) {
        ss >> PID >> priority >> submissionTime >> executionTime >> blockTime;
        std::cout << PID << " " << priority << " " << submissionTime << " " << executionTime << " " << blockTime << "\n";
        process.push_back(std::tuple<int, int, double, double, double>(PID, priority, submissionTime,
                                                                    executionTime, blockTime));
    }
}

int main() {
    Simulator sim(1, 1, true);
    bool (*FCFS)(std::vector <Process>*, std::vector<Process>*, double) = Algorithms::FCFS;
    bool (*RR)(std::vector <Process>*, std::vector<Process>*, double) = Algorithms::RR;
    bool (*HRRN)(std::vector <Process>*, std::vector<Process>*, double) = Algorithms::HRRN;
    bool (*PRIORITY)(std::vector <Process>*, std::vector<Process>*, double) = Algorithms::PRIORITY;
    bool (*LOTTERY)(std::vector <Process>*, std::vector<Process>*, double) = Algorithms::LOTTERY;
    filetoTuple("/home/ariel/ClionProjects/Simple-OS-Simulator/teste.txt");

    sim.StartSimulation(RR, process);
    std::cout << sim.getResults() << std::endl;
    return 0;
}
