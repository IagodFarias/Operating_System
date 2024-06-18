#ifndef MEMORY_SIMULATOR_H
#define MEMORY_SIMULATOR_H

#include <vector>
#include <queue>
#include <unordered_map>
#include <iostream>

class MemoryPage {
public:
    int pageId;
    int frameId;
    MemoryPage(int pid, int fid) : pageId(pid), frameId(fid) {}
};

class MemorySimulator {
private:
    int numFrames;
    std::vector<int> frames;
    std::queue<int> pageQueue;
    std::unordered_map<int, MemoryPage> pageTable;

    void removeOldestPage();
    void addNewPage(int pageId);

public:
    MemorySimulator(int numFrames);
    void accessPage(int pageId);
    void displayFrames();
};

#endif // MEMORY_SIMULATOR_H
