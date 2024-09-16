#ifndef MEMORY_SIMULATOR_H
#define MEMORY_SIMULATOR_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "Process.h"

// Estrutura para armazenar os resultados do acesso à memória
struct AccessResult {
    int frame;
    bool fault;
    int parcial_total_faults;
    int last_access;
    int page_number;
    int process_id;
    std::vector<std::string> frames_state;  // Estado atual dos frames após o acesso
};

class MemoryPage {
public:
    int page_number;
    int frequency;
    int last_access;
    int process_id;  // Adiciona o campo process_id

    // Construtor atualizado
    MemoryPage(int page_number, int last_access, int process_id);
};

class MemorySimulator {
private:
    int memory_size;
    std::vector<MemoryPage> memory;
    int total_requests;
    int total_page_faults;

    bool fifo_policy(int process_id, int page_number, int current_time);
    bool lfu_policy(int process_id, int page_number, int current_time);
    bool opt_policy(int process_id, int page_number, int current_time, const std::vector<int>& future_references);

public:
    MemorySimulator(int memory_size);

    AccessResult access_page(int process_id, int page_number, int current_time, const std::string& policy, const std::vector<int>& future_references = {});

    void print_statistics() const;
    void display_frames() const;

    // Nova função para obter o estado dos frames como strings
    std::vector<std::string> get_frames_state() const;

    int get_total_page_faults() const { return total_page_faults; }
};

#endif // MEMORY_SIMULATOR_H
