#include "MemorySimulator.h"

// Construtor de MemoryPage com process_id
MemoryPage::MemoryPage(int page_number, int last_access, int process_id)
    : page_number(page_number), frequency(1), last_access(last_access), process_id(process_id) {}

// Construtor de MemorySimulator
MemorySimulator::MemorySimulator(int memory_size)
    : memory_size(memory_size), total_requests(0), total_page_faults(0) {}

// Política FIFO com process_id
bool MemorySimulator::fifo_policy(int process_id, int page_number, int current_time) {
    auto it = std::find_if(memory.begin(), memory.end(), [&](const MemoryPage& p) {
        return p.page_number == page_number && p.process_id == process_id;
    });

    if (it == memory.end()) {
        if (memory.size() >= memory_size) {
            memory.erase(memory.begin()); // Remove a primeira página (FIFO)
        }
        memory.emplace_back(page_number, current_time, process_id);  // Adiciona process_id
        total_page_faults++;
        return true;  // Falta de página ocorreu
    } else {
        return false;  // Página já está na memória
    }
}

// Política LFU com process_id
bool MemorySimulator::lfu_policy(int process_id, int page_number, int current_time) {
    auto it = std::find_if(memory.begin(), memory.end(), [&](const MemoryPage& p) {
        return p.page_number == page_number && p.process_id == process_id;
    });

    if (it != memory.end()) {
        it->frequency++; // Incrementa a frequência
        it->last_access = current_time;
        return false;  // Página já está na memória
    } else {
        if (memory.size() >= memory_size) {
            auto lfu_it = std::min_element(memory.begin(), memory.end(), [](const MemoryPage& a, const MemoryPage& b) {
                return a.frequency < b.frequency;
            });
            memory.erase(lfu_it); // Remove a página menos frequentemente usada
        }
        memory.emplace_back(page_number, current_time, process_id);  // Adiciona process_id
        total_page_faults++;
        return true;  // Falta de página ocorreu
    }
}

// Política OPT com process_id
bool MemorySimulator::opt_policy(int process_id, int page_number, int current_time, const std::vector<int>& future_references) {
    auto it = std::find_if(memory.begin(), memory.end(), [&](const MemoryPage& p) {
        return p.page_number == page_number && p.process_id == process_id;
    });

    if (it != memory.end()) {
        it->last_access = current_time; // Atualiza o último acesso
        return false;  // Página já está na memória
    } else {
        if (memory.size() >= memory_size) {
            // Encontra a página a ser removida com base nas referências futuras
            // Implementação simplificada para este exemplo
            memory.erase(memory.begin());  // Remove a primeira página
        }
        memory.emplace_back(page_number, current_time, process_id);  // Adiciona process_id
        total_page_faults++;
        return true;  // Falta de página ocorreu
    }
}

// Simula o acesso a uma página
AccessResult MemorySimulator::access_page(int process_id, int page_number, int current_time, const std::string& policy, const std::vector<int>& future_references) {
    total_requests++;
    AccessResult result;
    bool page_fault_occurred = false;
    int frame_number = -1;

    if (policy == "FIFO") {
        page_fault_occurred = fifo_policy(process_id, page_number, current_time);
    } else if (policy == "LFU") {
        page_fault_occurred = lfu_policy(process_id, page_number, current_time);
    } else if (policy == "OPT") {
        page_fault_occurred = opt_policy(process_id, page_number, current_time, future_references);
    } else {
        std::cerr << "Unknown policy: " << policy << std::endl;
        result.fault = false;
        result.frame = -1;
        return result;
    }

    // Encontra o frame onde a página está localizada
    for (int i = 0; i < memory.size(); ++i) {
        if (memory[i].page_number == page_number && memory[i].process_id == process_id) {
            frame_number = i;  // O índice no vetor memory pode ser considerado o número do quadro
            break;
        }
    }

    result.fault = page_fault_occurred;
    result.frame = frame_number;
    result.parcial_total_faults = total_page_faults;
    result.last_access = current_time;
    result.page_number = page_number;
    result.process_id = process_id;
    result.frames_state = get_frames_state();  // Obtém o estado atual dos frames

    // Exibe os frames após o acesso
    display_frames();

    return result;
}

// Exibe estatísticas
void MemorySimulator::print_statistics() const {
    std::cout << "Total de requisições: " << total_requests << std::endl;
    std::cout << "Total de faltas de página: " << total_page_faults << std::endl;
    std::cout << "Taxa de falta de página: " << (double)total_page_faults / total_requests * 100 << "%" << std::endl;
}

void MemorySimulator::display_frames() const {
    std::cout << "===== Estado Atual da Memória Física =====" << std::endl;
    std::cout << " Frame | Process ID | Page Number " << std::endl;
    std::cout << "-----------------------------------------" << std::endl;

    for (int i = 0; i < memory_size; ++i) {
        if (i < memory.size()) {
            const auto& page = memory[i];
            std::cout << "   " << i << "   |     P" << page.process_id << "     |     " << page.page_number << std::endl;
        } else {
            std::cout << "   " << i << "   |     -     |     -" << std::endl;  // Indica frames vazios
        }
    }

    std::cout << "=========================================" << std::endl;
}

// Função para obter o estado dos frames como um vetor de strings
std::vector<std::string> MemorySimulator::get_frames_state() const {
    std::vector<std::string> frames;
    for (int i = 0; i < memory_size; ++i) {
        std::string frame_info;
        if (i < memory.size()) {
            const auto& page = memory[i];
            frame_info = "Frame " + std::to_string(i) + " | P" + std::to_string(page.process_id) + " | Page " + std::to_string(page.page_number);
        } else {
            frame_info = "Frame " + std::to_string(i) + " | - | -";
        }
        frames.push_back(frame_info);
    }
    return frames;
}
