#include "MemorySimulator.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    int memory_size = 3;  // Defina o tamanho da memória
    std::vector<int> pagesToAccess = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};  // Páginas predefinidas
    std::vector<int> future_references = {1, 2, 3, 4, 5};  // Referências futuras para OPT

    while (true) {
        MemorySimulator simulator(memory_size);  // Reinicializa o simulador a cada iteração

        // Solicita ao usuário que escolha a política de substituição
        std::string policy;
        std::cout << "Choose a replacement policy (FIFO, LFU, OPT) or type 'exit' to quit: ";
        std::cin >> policy;

        // Permite ao usuário sair do loop digitando "exit"
        if (policy == "exit") {
            break;
        }

        // Verifica se a política escolhida é válida
        if (policy != "FIFO" && policy != "LFU" && policy != "OPT") {
            std::cout << "Invalid policy. Please choose between FIFO, LFU, or OPT." << std::endl;
            continue;  // Volta para o início do loop
        }

        // Simulação do acesso a páginas
        int current_time = 0;
        for (int page : pagesToAccess) {
            std::cout << "Time " << current_time << ": Accessing page " << page << std::endl;
            simulator.access_page(page, current_time, policy, future_references);
            simulator.display_frames();
            current_time++;
        }

        simulator.print_statistics();
        
        // Pausa o sistema
        system("pause");
    }

    std::cout << "Program terminated." << std::endl;
    return 0;
}
