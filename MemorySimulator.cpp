#include "MemorySimulator.h"

MemorySimulator::MemorySimulator(int numFrames) : numFrames(numFrames) {
    frames.resize(numFrames, -1);
}

void MemorySimulator::removeOldestPage() {
    if (!pageQueue.empty()) {
        int oldestPageId = pageQueue.front();
        pageQueue.pop();
        pageTable.erase(oldestPageId);

        for (int &frame : frames) {
            if (frame == oldestPageId) {
                frame = -1;
                break;
            }
        }
    }
}

void MemorySimulator::addNewPage(int pageId) {
    int emptyFrameIndex = -1;

    for (int i = 0; i < numFrames; ++i) {
        if (frames[i] == -1) {
            emptyFrameIndex = i;
            break;
        }
    }

    if (emptyFrameIndex != -1) {
        frames[emptyFrameIndex] = pageId;
        pageTable[pageId] = MemoryPage(pageId, emptyFrameIndex);
    } else {
        removeOldestPage();
        addNewPage(pageId);
    }

    pageQueue.push(pageId);
}

void MemorySimulator::accessPage(int pageId) {
    if (pageTable.find(pageId) == pageTable.end()) {
        addNewPage(pageId);
    }
}

void MemorySimulator::displayFrames() {
    std::cout << "Current Frames: ";
    for (int frame : frames) {
        if (frame != -1) {
            std::cout << frame << " ";
        } else {
            std::cout << ". ";
        }
    }
    std::cout << std::endl;
}
