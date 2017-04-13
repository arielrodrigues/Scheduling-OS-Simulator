#include <iostream>
#include <fstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>

#include "sOS-Sim.h"
#include "ProcessSchedulingAlgorithms.h"
#include "FileManager.h"
#include "PageReplacementAlgorithms.h"

std::vector<std::tuple<int, double, int, double, double, std::vector<Page>>> process;
std::vector<Page> getPages(std::string _pages);
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
    std::string _pages;
	int PID, priority;

    std::istringstream pagesFile(readFile(filename+"_pages.txt").str());
    std::stringstream processFile = readFile(filename+"_process.txt");

	for (auto i = 0; i < FileManager::numberofLines; i++) {
		processFile >> PID >> submissionTime >> priority >> executionTime >> blockTime;
		executionTime -= blockTime;
		if (debug_mode)
			std::cout << PID << " " << submissionTime << " " << priority << " " << executionTime << " " << blockTime << "\n";
        std::getline(pagesFile, _pages);
		process.push_back(std::tuple<int, double, int, double, double, std::vector<Page>>(PID, submissionTime, priority,
		                                                               executionTime, blockTime,getPages(_pages)));
	}
}

std::vector<Page> getPages(std::string _pages) {
    std::vector<Page> pages;
    std::stringstream aux_pages(_pages);
    int pid, time, value;
    char colon;

    aux_pages >> pid;
    while (aux_pages >> time) {
        aux_pages >> colon >> value;
        pages.push_back(Page(time, 0, value, pid));
    }

    // sort pages by time
    std::sort(pages.begin(), pages.end());
    // Update life time in all pages
    for (int i = 0; i < pages.size(); i++)
        pages[i].setLifeTime(pages[i+1].getFirstUse() - pages[i].getFirstUse());
    return pages;
}

int main() {
	// Short Time Scheduling Algorithms
	bool (*RR)(std::vector <Process>*, std::vector<Process>*, int*, double) = ProcessSchedulingAlgorithms::RR;
	bool (*HRRN)(std::vector <Process>*, std::vector<Process>*, int*, double) = ProcessSchedulingAlgorithms::HRRN;
	bool (*PRIORITY)(std::vector <Process>*, std::vector<Process>*, int*, double) = ProcessSchedulingAlgorithms::PRIORITY;
	bool (*LOTTERY)(std::vector <Process>*, std::vector<Process>*, int*, double) = ProcessSchedulingAlgorithms::LOTTERY;
	bool (*SD) (std::vector <Process>*, std::vector<Process>*, int*, double) = ProcessSchedulingAlgorithms::SD;
	bool (*FEEDBACK)(std::vector <Process>*, std::vector<Process>*, int*, double) = ProcessSchedulingAlgorithms::FEEDBACK;

    // Page Replacement Algorithms
    bool (*FIFO)(std::vector<Page>*, std::vector<Page>*, Page, double) = PageReplacementAlgorithms::FIFO;
    bool (*LRU)(std::vector<Page>*, std::vector<Page>*, Page, double) = PageReplacementAlgorithms::LRU;

    std::stringstream out;
	Simulator sim(50, step_by_step, debug_mode);
    ProcessSchedulingAlgorithms::maxProcessMultiprogramming = 50;

    // get process from file
    process.clear();
    filetoVectorofTuples("/home/ariel/ClionProjects/sOS-Sim/Files4Test/cenario1");

    out << "Cenário 1:\n" << "\tShort Time Scheduling Algorithm: Round Robbin - Quantum = 2\n";
    std::cout << "Cenário 1:\n" << "\tShort Time Scheduling Algorithm: Round Robbin - Quantum = 2\n";
    out << "\rPage replacement Algorithm: FIFO \n";
    std::cout << "\tPage replacement Algorithm: FIFO \n";
    sim.StartSimulation(RR,FIFO, process);
    out << sim.getResults() << "\n\n";

	FileManager::writeFile("/home/ariel/ClionProjects/sOS-Sim/Files4Test/simulation.out", out.str());
	return 0;
}