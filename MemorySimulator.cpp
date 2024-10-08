#include "MemorySimulator.h"

// Construtor de MemoryPage
MemoryPage::MemoryPage(int page_number, int last_access)
    : page_number(page_number), frequency(1), last_access(last_access) {}

// Construtor de MemorySimulator
MemorySimulator::MemorySimulator(int memory_size)
    : memory_size(memory_size), total_requests(0), total_page_faults(0) {}

// Política FIFO
void MemorySimulator::fifo_policy(int page_number, int current_time) {
    auto it = std::find_if(memory.begin(), memory.end(), [&](const MemoryPage &p) {
        return p.page_number == page_number;
    });

    if (it == memory.end()) {
        if (memory.size() >= memory_size) {
            memory.erase(memory.begin()); // Remove a primeira página (FIFO)
        }
        memory.emplace_back(page_number, current_time);
        total_page_faults++;
    }
}

// Política LFU
void MemorySimulator::lfu_policy(int page_number, int current_time) {
    auto it = std::find_if(memory.begin(), memory.end(), [&](const MemoryPage &p) {
        return p.page_number == page_number;
    });

    if (it != memory.end()) {
        it->frequency++; // Incrementa a frequência
        it->last_access = current_time;
    } else {
        if (memory.size() >= memory_size) {
            auto lfu_it = std::min_element(memory.begin(), memory.end(), [](const MemoryPage &a, const MemoryPage &b) {
                return a.frequency < b.frequency;
            });
            memory.erase(lfu_it); // Remove a página menos frequentemente usada
        }
        memory.emplace_back(page_number, current_time);
        total_page_faults++;
    }
}

// Política OPT
void MemorySimulator::opt_policy(int page_number, int current_time, const std::vector<int>& future_references) {
    auto it = std::find_if(memory.begin(), memory.end(), [&](const MemoryPage &p) {
        return p.page_number == page_number;
    });

    if (it != memory.end()) {
        it->last_access = current_time; // Atualiza o último acesso
    } else {
        if (memory.size() >= memory_size) {
            // Encontra a página a ser removida com base nas referências futuras
            int page_to_remove = -1;
            int farthest_use = -1;
            for (int i = 0; i < memory.size(); ++i) {
                auto future_it = std::find(future_references.begin(), future_references.end(), memory[i].page_number);
                if (future_it == future_references.end()) {
                    page_to_remove = i;
                    break;
                } else {
                    int distance = std::distance(future_references.begin(), future_it);
                    if (distance > farthest_use) {
                        farthest_use = distance;
                        page_to_remove = i;
                    }
                }
            }
            memory.erase(memory.begin() + page_to_remove);
        }
        memory.emplace_back(page_number, current_time);
        total_page_faults++;
    }
}

// Método que simula o acesso de uma página com a política escolhida
void MemorySimulator::access_page(int page_number, int current_time, const std::string& policy, const std::vector<int>& future_references) {
    total_requests++;

    if (policy == "FIFO") {
        fifo_policy(page_number, current_time);
    } else if (policy == "LFU") {
        lfu_policy(page_number, current_time);
    } else if (policy == "OPT") {
        opt_policy(page_number, current_time, future_references);
    } else {
        std::cerr << "Unknown policy: " << policy << std::endl;
        return;
    }
}

// Método para exibir estatísticas
void MemorySimulator::print_statistics() const {
    std::cout << "Total requests: " << total_requests << std::endl;
    std::cout << "Total page faults: " << total_page_faults << std::endl;
    std::cout << "Page fault rate: " << (double)total_page_faults / total_requests * 100 << "%" << std::endl;
}

// Método para exibir os frames atuais da memória
void MemorySimulator::display_frames() const {
    std::cout << "Current Frames: ";
    for (const auto& page : memory) {
        std::cout << page.page_number << " ";
    }
    std::cout << std::endl;
}
