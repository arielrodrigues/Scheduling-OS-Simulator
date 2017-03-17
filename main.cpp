#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>

#include "sOS-Sim.h"
#include "Algorithms.h"
#include "FileManager.h"

std::vector<std::tuple<int, double, int, double, double>> process;
bool step_by_step = false, debug_mode = false;

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

	std::stringstream ss = readFile(filename);
	for (auto i = 0; i < FileManager::numberofLines; i++) {
		ss >> PID >> submissionTime >> priority >> executionTime >> blockTime;
		executionTime -= blockTime;
		if (debug_mode)
			std::cout << PID << " " << submissionTime << " " << priority << " " << executionTime << " " << blockTime << "\n";
		process.push_back(std::tuple<int, double, int, double, double>(PID, submissionTime, priority,
		                                                               executionTime, blockTime));
	}
}

int main() {
	// Algorithms
	//bool (*FCFS)(std::vector <Process>*, std::vector<Process>*, int*, double) = Algorithms::FCFS;
	bool (*RR)(std::vector <Process>*, std::vector<Process>*, int*, double) = Algorithms::RR;
	bool (*HRRN)(std::vector <Process>*, std::vector<Process>*, int*, double) = Algorithms::HRRN;
	bool (*PRIORITY)(std::vector <Process>*, std::vector<Process>*, int*, double) = Algorithms::PRIORITY;
	bool (*LOTTERY)(std::vector <Process>*, std::vector<Process>*, int*, double) = Algorithms::LOTTERY;
	bool (*SD) (std::vector <Process>*, std::vector<Process>*, int*, double) = Algorithms::SD;
	bool (*FEEDBACK)(std::vector <Process>*, std::vector<Process>*, int*, double) = Algorithms::FEEDBACK;

	// vector of algorithms
	std::vector<bool (*)(std::vector <Process>*, std::vector<Process>*, int*, double)> algorithms;
	algorithms.push_back(RR);
	algorithms.push_back(HRRN);
	algorithms.push_back(PRIORITY);
	algorithms.push_back(LOTTERY);
	algorithms.push_back(SD);
	algorithms.push_back(FEEDBACK);

	Simulator sim(0, step_by_step, debug_mode);
	std::stringstream out;

	for (uint32_t alfa = 100; alfa < 300; alfa*=2) {
		out << "**** ALFA " << alfa << " ****\n";
		std::cout << "**** ALFA " << alfa << " ****\n";
		Algorithms::maxProcessMultiprogramming = alfa;
		for (auto i = 1, j = 0; i < 6; i++, j = 0) {
			out << "Cenário " << i << ": \n";
			std::cout << "Cenário " << i << ": \n";
			process.clear();
			filetoVectorofTuples("/home/ariel/ClionProjects/sOS-Sim/Files4test/cenario"+std::to_string(i)+".txt");
			for (auto algorithm: algorithms) {
				sim.Clear(alfa, step_by_step, debug_mode);
				out << "Algoritmo: " << ++j << "\n";
				std::cout << "Algoritmo: " << j << "\n";
				sim.StartSimulation(algorithm, process);
				out << sim.getResults() << "\n\n";
			}
		}
	}

	FileManager::writeFile("/home/ariel/ClionProjects/sOS-Sim/simulation.out", out.str());
	return 0;
}