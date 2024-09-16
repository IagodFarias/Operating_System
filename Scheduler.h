#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <string>
#include "Process.h"
#include "MemorySimulator.h"  // Incluímos o cabeçalho do MemorySimulator

struct AccessInfo {
    bool fault;
    int parcial_total_fault;
    int last_access;
    int frame;
    int page_number;
    int process_id;
    std::vector<std::string> frames_state;  // Estado dos frames após o acesso
};

struct ProcessInfo {
    int process_number;
    std::vector<AccessInfo> accesses;
};

class Scheduler {
    std::vector<Process*> processes;
    int time_quantum;
    int current_process_index;

    std::vector<ProcessInfo> process_infos;

public:
    Scheduler(int time_quantum);
    void run(const std::string& policy, const std::vector<int>& future_references, MemorySimulator& simulator);
    void add_process(Process* process);

    void output_json(const std::string& policy);
};

#endif // SCHEDULER_H
