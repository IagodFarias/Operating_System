#ifndef MEMORY_SIMULATOR_H
#define MEMORY_SIMULATOR_H

#include <iostream>
#include <vector>
#include <algorithm>

class MemoryPage {
public:
    int page_number;
    int frequency;
    int last_access;

    MemoryPage(int page_number, int last_access);
};

class MemorySimulator {
private:
    int memory_size;
    std::vector<MemoryPage> memory;
    int total_requests;
    int total_page_faults;

    void fifo_policy(int page_number, int current_time);
    void lfu_policy(int page_number, int current_time);
    void opt_policy(int page_number, int current_time, const std::vector<int>& future_references);

public:
    MemorySimulator(int memory_size);

    void access_page(int page_number, int current_time, const std::string& policy, const std::vector<int>& future_references = {});
    void print_statistics() const;
    void display_frames() const;
};

#endif // MEMORY_SIMULATOR_H
