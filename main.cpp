#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>

#include "sOS-Sim.h"
#include "Algorithms.h"
#include "FileManager.h"

std::vector<std::tuple<int, double, int, double, double>> process;


/**
 * Reads the file, takes the tuples from the processes and places them in a vector;
 * Each process is a tuple with PID, Submission time, Priority, Execution time and Block time, respectively.
 * @param filename
 */

void filetoVectorofTuples(std::string filename) {
    using namespace FileManager;

    std::ifstream file(filename.c_str(), std::ifstream::in);
    double submissionTime, executionTime, blockTime;
    int PID, priority;
    char delichar;

    std::stringstream ss = readFile(filename);
    while (ss.good()) {
        ss >> PID >> submissionTime >> priority >> executionTime >> blockTime >> delichar;
	    std::cout << PID << " " << submissionTime << " " << priority << " " << executionTime << " " << blockTime << "\n";
        process.push_back(std::tuple<int, double, int, double, double>(PID, submissionTime, priority,
                                                                       executionTime, blockTime));
    }
}

int main() {

    filetoVectorofTuples("/home/ariel/ClionProjects/Simple-OS-Simulator/teste.txt");
    int alfa = 100; // max process multiprogramming
    bool step_by_step = false, debug_mode = true;
    Simulator sim(alfa, step_by_step, debug_mode);

    // Algorithms
    //bool (*FCFS)(std::vector <Process>*, std::vector<Process>*, int*, double) = Algorithms::FCFS;
    bool (*RR)(std::vector <Process>*, std::vector<Process>*, int*, double) = Algorithms::RR;
    bool (*HRRN)(std::vector <Process>*, std::vector<Process>*, int*, double) = Algorithms::HRRN;
    bool (*PRIORITY)(std::vector <Process>*, std::vector<Process>*, int*, double) = Algorithms::PRIORITY;
    bool (*LOTTERY)(std::vector <Process>*, std::vector<Process>*, int*, double) = Algorithms::LOTTERY;

    sim.StartSimulation(RR, process);
    return 0;
}
