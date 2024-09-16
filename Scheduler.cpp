#include "Scheduler.h"
#include "MemorySimulator.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include "json.hpp"
using json = nlohmann::json;

Scheduler::Scheduler(int time_quantum)
    : time_quantum(time_quantum), current_process_index(0) {}

void Scheduler::run(const std::string& policy, const std::vector<int>& future_references, MemorySimulator& simulator) {
    if (processes.empty()) {
        std::cout << "No processes to schedule." << std::endl;
        return;
    }

    std::cout << "Running processes with " << policy << " replacement policy." << std::endl;

    // Limpa informações anteriores
    process_infos.clear();

    // Inicializa as informações dos processos
    for (Process* process : processes) {
        ProcessInfo p_info;
        p_info.process_number = process->get_id();
        process_infos.push_back(p_info);
    }

    int current_time = 0;

    while (true) {
        bool all_done = true; // Verifica se todos os processos terminaram

        for (int i = 0; i < processes.size(); ++i) {
            Process* process = processes[i];

            if (process->get_pages().empty()) {
                continue;  // Se não houver páginas, pula para o próximo processo
            }

            all_done = false;  // Ainda há processos para serem executados

            int pages_to_run = std::min(time_quantum, (int)process->get_pages().size());

            for (int j = 0; j < pages_to_run; ++j) {
                int page_number = process->get_pages()[j];

                std::cout << "Process " << process->get_id() << " accessing page " << page_number << std::endl;

                // Simula o acesso à memória e coleta as informações
                AccessResult result = simulator.access_page(process->get_id(), page_number, current_time, policy, future_references);

                AccessInfo access_info;
                access_info.process_id = result.process_id;
                access_info.page_number = result.page_number;
                access_info.frame = result.frame;
                access_info.fault = result.fault;
                access_info.parcial_total_fault = result.parcial_total_faults;
                access_info.last_access = result.last_access;
                access_info.frames_state = result.frames_state;

                // Adiciona as informações de acesso ao processo correspondente
                process_infos[i].accesses.push_back(access_info);

                current_time++;
            }

            // Remove as páginas já acessadas
            process->get_pages().erase(process->get_pages().begin(), process->get_pages().begin() + pages_to_run);
        }

        if (all_done) {
            break;
        }
    }

    // Mostra as estatísticas do simulador
    simulator.print_statistics();

    // Gera a saída em formato JSON
    output_json(policy);
}

void Scheduler::add_process(Process* process) {
    processes.push_back(process);
}

void Scheduler::output_json(const std::string& policy) {
    json output = json::array();

    for (const auto& p_info : process_infos) {
        json process_json;
        process_json["Process_number"] = p_info.process_number;
        process_json["times"] = json::array();

        for (const auto& access : p_info.accesses) {
            json access_json;
            access_json["fault"] = access.fault;
            access_json["parcial_total_fault"] = access.parcial_total_fault;
            access_json["last_access"] = access.last_access;
            access_json["frame"] = access.frame;
            access_json["page_number"] = access.page_number;
            access_json["process_id"] = access.process_id;

            // Adiciona o estado dos frames
            access_json["frames_state"] = access.frames_state;

            process_json["times"].push_back(access_json);
        }

        output.push_back(process_json);
    }

    // Cria o diretório 'output' se não existir
    std::filesystem::create_directory("output");

    // Escreve o JSON em um arquivo dentro do diretório 'output'
    std::ofstream outfile("output/output_" + policy + ".json");
    if (outfile.is_open()) {
        outfile << output.dump(4);  // Indenta com 4 espaços
        outfile.close();
        std::cout << "JSON output saved to output/output_" << policy << ".json" << std::endl;
    } else {
        std::cerr << "Failed to open output file." << std::endl;
    }
}
