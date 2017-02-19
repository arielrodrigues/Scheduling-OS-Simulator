#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>

#include "sOS-Sim.h"
#include "Algorithms.h"
#include "FileManager.h"

std::vector<std::tuple<int, double, int, double, double>> process;
bool step_by_step = false, debug_mode = true;

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

	std::stringstream out;
	/*for (uint32_t alfa = 100; alfa < 300; alfa*=2) {
		out << "**** ALFA " << alfa << " ****\n";
		std::cout << "**** ALFA " << alfa << " ****\n";
		Simulator sim(100, step_by_step, debug_mode);
		Algorithms::maxProcessMultiprogramming = alfa;
		for (auto i = 1, j = 0; i < 5; i++, j = 0) {
			out << "Cenário " << i << ": \n";
			std::cout << "Cenário " << i << ": \n";
			filetoVectorofTuples("/home/ariel/ClionProjects/Simple-OS-Simulator/Cenarios/cenario"+std::to_string(i)+".txt");
			for (auto algorithm: algorithms){
				out << "Algoritmo: " << j++ << "\n";
				std::cout << "Algoritmo: " << j++ << "\n";
				sim.StartSimulation(algorithm, process);
				out << sim.getResults() << "\n\n";
			} process.clear();
		}
	}*/

	filetoVectorofTuples("/home/ariel/ClionProjects/Simple-OS-Simulator/teste.txt");
	/*auto i = 0;
	for (auto algorithm: algorithms) {
		Simulator sim(100, step_by_step, debug_mode);
		std::cout << "\n\nAlgoritmo: " << i++ << "\n";
		sim.StartSimulation(algorithm, process);
		out << "Algoritmo: " << i++ << "\n";
		out << sim.getResults() << "\n\n";
	} process.clear();*/

	Simulator sim(100, step_by_step, debug_mode);
	sim.StartSimulation(HRRN, process);
	out << "Algoritmo: " << "HRRN" << "\n";
	out << sim.getResults() << "\n\n";

	FileManager::writeFile("/home/ariel/ClionProjects/Simple-OS-Simulator/simulationteste.out", out.str());
	return 0;
}