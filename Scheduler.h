#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <string>
#include "Process.h"
#include "MemorySimulator.h"  // Incluímos o cabeçalho do MemorySimulator

class Scheduler {
private:
    int time_quantum;                         // Quantum de tempo para a execução dos processos
    std::vector<Process*> processes;          // Lista de processos a serem escalonados
    int current_process_index;                // Índice do processo atual sendo executado

public:
    Scheduler(int time_quantum);              // Construtor que inicializa o quantum
    void add_process(Process* process);       // Adiciona um processo à lista
    // A função run agora aceita um MemorySimulator como terceiro parâmetro
    void run(const std::string& scheduling_policy, const std::vector<int>& future_references, MemorySimulator& simulator);
};

#endif // SCHEDULER_H