#ifndef SIMPLE_OS_SIMULATOR_PAGE_H
#define SIMPLE_OS_SIMULATOR_PAGE_H

#include <cstdint>

class Page {
public:
    Page(uint32_t firstUse, uint32_t lifeTime, uint32_t value, uint32_t pid) { this->firstUse = firstUse;
        this->lastTimeUsed = 0; this->pid = pid; this->lifeTime = lifeTime; this->value = value; }

    uint32_t getPID()        { return this->pid; }
    uint32_t getFirstUse()   { return this->firstUse; }
    uint32_t getLifeTime()   { return this->lifeTime; }
    uint32_t getValue()      { lastTimeUsed = 0; return this->value; }
    double getLastTimeUsed() { return this->lastTimeUsed; }

    void setLifeTime(uint32_t lifeTime) { this->lifeTime = lifeTime; }
    void setUsed(double _elapsedTime)  { this->lastTimeUsed = _elapsedTime; }

    void decrementLifeTime()      { this->lifeTime--; }
    bool operator<(Page &a) const { return firstUse < a.getFirstUse(); }
private:
    uint32_t firstUse, lifeTime, value, pid;
    double lastTimeUsed;
};

#endif //SIMPLE_OS_SIMULATOR_PAGE_H
