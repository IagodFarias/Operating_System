#ifndef MEMORY_SIMULATOR_H
#define MEMORY_SIMULATOR_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "Process.h"

class MemoryPage {
public:
    int page_number;
    int frequency;
    int last_access;
    int process_id;  // Adiciona o campo process_id

    MemoryPage(int page_number, int last_access, int process_id);  // Construtor atualizado
};

class MemorySimulator {
private:
    int memory_size;
    std::vector<MemoryPage> memory;
    int total_requests;
    int total_page_faults;

    void fifo_policy(int process_id, int page_number, int current_time);  // Atualizado
    void lfu_policy(int process_id, int page_number, int current_time);   // Atualizado
    void opt_policy(int process_id, int page_number, int current_time, const std::vector<int>& future_references);  // Atualizado

public:
    MemorySimulator(int memory_size);

    void add_process(const Process& process);  // Função adicionada
    void access_page(int process_id, int page_number, int current_time, const std::string& policy, const std::vector<int>& future_references = {});  // Atualizado
    void print_statistics() const;
    void display_frames() const;
};

#endif // MEMORY_SIMULATOR_H