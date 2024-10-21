#include "Scheduler.h"
#include "MemorySimulator.h"
#include <iostream>

// Construtor que inicializa o quantum e o índice do processo atual
Scheduler::Scheduler(int time_quantum)
    : time_quantum(time_quantum), current_process_index(0) {}

// Agora a função run aceita o simulador de memória como argumento
void Scheduler::run(const std::string& policy, const std::vector<int>& future_references, MemorySimulator& simulator) {
    if (processes.empty()) {
        std::cout << "No processes to schedule." << std::endl;
        return;
    }

    std::cout << "Running processes with " << policy << " replacement policy." << std::endl;

    // Agora, em vez de criar um novo simulador, utilizamos o simulador passado por referência
    while (true) {
        bool all_done = true; // Verifica se todos os processos terminaram

        for (int i = 0; i < processes.size(); ++i) {
            Process* process = processes[i];

            // Verifica se o processo ainda tem páginas a serem acessadas
            if (process->get_pages().empty()) {
                continue;  // Se não houver páginas, pula para o próximo processo
            }

            all_done = false;  // Ainda há processos para serem executados

            // Exibe o processo que está sendo executado
            std::cout << "Running process: " << process->get_id() << std::endl;

            // Calcula quantas páginas o processo pode acessar durante este quantum
            int pages_to_run = std::min(time_quantum, (int)process->get_pages().size());

            // Simula o acesso às páginas durante o quantum
            for (int j = 0; j < pages_to_run; ++j) {
                int page_number = process->get_pages()[j];
                std::cout << "Process " << process->get_id() << " accessing page " << page_number << std::endl;

                // Aqui chamamos o simulador de memória com a política escolhida
                simulator.access_page(process->get_id(), page_number, j, policy, future_references);
            }

            // Remove as páginas já acessadas do vetor
            process->get_pages().erase(process->get_pages().begin(), process->get_pages().begin() + pages_to_run);

            // Verifica se o processo terminou a execução de todas as suas páginas
            if (process->get_pages().empty()) {
                std::cout << "Process " << process->get_id() << " finished execution." << std::endl;
            }
        }

        // Se todos os processos estiverem concluídos, saia do loop
        if (all_done) {
            std::cout << "All processes finished execution." << std::endl;
            break;
        }
    }

    // Mostra as estatísticas do simulador
    simulator.print_statistics();
}

void Scheduler::add_process(Process* process) {
    processes.push_back(process);
}