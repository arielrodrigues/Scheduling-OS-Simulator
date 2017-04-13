#ifndef SIMPLE_OS_SIMULATOR_PAGEREPLACEMENTALGORITHMS_H
#define SIMPLE_OS_SIMULATOR_PAGEREPLACEMENTALGORITHMS_H

#include <cstdint>
#include <vector>
#include <iostream>
#include "Page.h"
#include "sOS-Sim.h"

namespace PageReplacementAlgorithms {

    uint32_t maxFrames = Simulator::numberOfMemoryFrames;


    static bool FIFO(std::vector<Page>* pagesInMemory, std::vector<Page>* pagesInDisk, Page page, double _elapsedTime) {
        try {
            if (!pagesInDisk->empty() && page.getLifeTime() > 0) {
                int i = 0;
                // search page on disk
                for (; i < (*pagesInDisk).size() && !((*pagesInDisk)[i].getValue() == page.getValue() &&
                                                     (*pagesInDisk)[i].getPID() == page.getPID()); i++);
                if (i > (*pagesInDisk).size()) { Simulator::DebugLog("ERRO. PÁGINA NÃO ENCONTRADA NO DISCO"); exit(1);}
                // if memory is full, move first page in memory to disk
                if ((*pagesInMemory).size() >= maxFrames) {
                    (*pagesInDisk).push_back((*pagesInMemory)[0]);
                    (*pagesInMemory).erase((*pagesInMemory).begin());
                    Simulator::DebugLog(_elapsedTime,
                                        "Page " + std::to_string((*pagesInMemory)[0].getValue()) + " of pid " +
                                        std::to_string((*pagesInMemory)[0].getPID()) + " in pos 0 on memory " +
                                        "was moved to disk");
                }
                // move page from disk to memory
                (*pagesInMemory).push_back((*pagesInDisk)[i]);
                (*pagesInDisk).erase((*pagesInDisk).begin() + i);
                Simulator::DebugLog(_elapsedTime, "Page "+std::to_string(page.getValue())+" of pid "+
                            std::to_string(page.getPID())+" moved from disk to memory");
                return true;
            } return false;
        } catch (...) {
            std::cout << "Erro ao substituir página." << std::endl;
            return false;
        }
    }

    static bool LRU(std::vector<Page>* pagesInMemory, std::vector<Page>* pagesInDisk, Page page, double _elapsedTime) {
        try {
            if (!(*pagesInDisk).empty() && page.getLifeTime() > 0) {
                int lru = 0, i = 0;
                // search page on disk
                for (; i < (*pagesInDisk).size() && !((*pagesInDisk)[i].getValue() == page.getValue() &&
                                                     (*pagesInDisk)[i].getPID() == page.getPID()); i++);
                if (i > (*pagesInDisk).size()) { Simulator::DebugLog("ERRO. PÁGINA NÃO ENCONTRADA NO DISCO"); exit(1);}
                if (!(*pagesInMemory).empty()) {
                    // finds the less recently used page in memory
                    for (auto j = (*pagesInMemory).size(); j-- > 0; )
                        if ((*pagesInMemory)[j].getLastTimeUsed() < (*pagesInMemory)[lru].getLastTimeUsed()) lru = j;
                    // move the page memory[lru] in disk
                    (*pagesInDisk).push_back((*pagesInMemory)[lru]);
                    // then send this page disk[i] to memory[lru]
                    (*pagesInMemory)[lru] = (*pagesInDisk)[i];
                    (*pagesInDisk).erase((*pagesInDisk).begin()+i);
                    Simulator::DebugLog(_elapsedTime, "Page "+std::to_string(page.getValue())+" of pid "+
                            std::to_string(page.getPID())+" moved from disk to pos "+std::to_string(lru)+" on memory");
                } else {
                    // then send this page disk[i] to memory
                    (*pagesInMemory).push_back((*pagesInDisk)[i]);
                    (*pagesInDisk).erase((*pagesInDisk).begin()+i);
                    Simulator::DebugLog(_elapsedTime, "Page "+std::to_string(page.getValue())+" of pid "+
                              std::to_string(page.getPID())+" moved from disk to pos 0 on memory");
                }
                return true;
            }
            return false;
        } catch(...) {
            std::cout << "Erro ao substituir página." << std::endl;
            return false;
        }
    }

    static bool CLOCK(std::vector<Page>* pagesInMemory, std::vector<Page>* pagesInDisk, Page page, double _elapsedTime) {
        try {
            //TO DO
            return true;
        } catch(...) {
            std::cout << "Erro ao substituir página." << std::endl;
            return false;
        }
    }

    static bool OTIMO(std::vector<Page>* pagesInMemory, std::vector<Page>* pagesInDisk, Page page, double _elapsedTime) {
        try {
            //TO DO
            return true;
        } catch(...) {
            std::cout << "Erro ao substituir página." << std::endl;
            return false;
        }
    }
}

#endif //SIMPLE_OS_SIMULATOR_PAGEREPLACEMENTALGORITHMS_H
