#ifndef SIMPLE_OS_SIMULATOR_PAGE_H
#define SIMPLE_OS_SIMULATOR_PAGE_H

#include <cstdint>

class Page {
public:
    Page(uint32_t firstUse, uint32_t lifeTime, uint32_t value, uint32_t pid) { this->firstUse = firstUse;
        this->pid = pid; this->lifeTime = lifeTime; this->value = value; }

    uint32_t getPID()        { return this->pid; }
    uint32_t getFirstUse()   { return this->firstUse; }
    uint32_t getLifeTime()   { return this->lifeTime; }
    uint32_t getValue()      { return this->value; }
    double getLastTimeUsed() { return this->lastTimeUsed; }

    void setLifeTime(uint32_t lifeTime) { this->lifeTime = lifeTime; }
    void setLastTimeUsed(double _elapsedTime)  { this->lastTimeUsed = _elapsedTime; }

    // adjust references, used in clock algorithm
    bool isreferenced() { return this->referenced; }
    void setreference() { this->referenced = true; }
    void resetreference() { this->referenced = false; }

    void decrementLifeTime()      { this->lifeTime--; }
    bool operator<(Page &a) const { return firstUse < a.getFirstUse(); }
private:
    uint32_t firstUse, lifeTime, value, pid;
    double lastTimeUsed = 0;

    bool referenced = false; // just used in clock replacement algorithm
};

#endif //SIMPLE_OS_SIMULATOR_PAGE_H
