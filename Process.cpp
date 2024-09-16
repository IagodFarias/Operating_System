#include "Process.h"
#include <fstream>
#include <iostream>

// Construtor que inicializa o processo com um ID e uma lista de páginas
Process::Process(int id, const std::vector<int>& pages)
    : process_id(id), pages_to_access(pages) {}

// Salva o contexto do processo em um arquivo
void Process::save_context() const {
    std::ofstream file("process_" + std::to_string(process_id) + "_context.txt");
    if (file.is_open()) {
        file << process_id << std::endl;
        for (int page : pages_to_access) {
            file << page << " ";
        }
        file << std::endl;
        file.close();
    } else {
        std::cerr << "Failed to save context for process " << process_id << std::endl;
    }
}

// Carrega o contexto do processo a partir de um arquivo
void Process::load_context() {
    std::ifstream file("process_" + std::to_string(process_id) + "_context.txt");
    if (file.is_open()) {
        pages_to_access.clear();
        file >> process_id;  // Carrega o ID do processo
        int page;
        while (file >> page) {
            pages_to_access.push_back(page);
        }
        file.close();
    } else {
        std::cerr << "Failed to load context for process " << process_id << std::endl;
    }
}
