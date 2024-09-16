#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include "MemorySimulator.h"
#include "Scheduler.h"
#include "Process.h"

// Função que gera um vetor aleatório de páginas para acessar
void generateRandomPages(std::vector<int>& pages, int mean = 10, int stddev = 2) {
    std::default_random_engine generator(static_cast<unsigned>(std::time(0)));
    std::normal_distribution<double> distribution(mean, stddev);

    for (auto& page : pages) {
        page = distribution(generator);  // Gera um número de acordo com a distribuição normal
    }
}

int main() {
    int memory_size = 3;  // Defina o tamanho da memória
    int time_quantum = 5;
    int pagesNumber = 100;
    Scheduler scheduler(time_quantum);  // Escalona a partir do time_quantum

    // Geração de processos e suas páginas de forma aleatória
    std::vector<int> pages1(pagesNumber), pages2(pagesNumber);
    generateRandomPages(pages1, 12, 3);
    generateRandomPages(pages2);

    Process process1(1, pages1);  // Cria os processos, com seu ID, e páginas.
    Process process2(2, pages2);

    scheduler.add_process(&process1);  // Adiciona o processo ao escalonador.
    scheduler.add_process(&process2);

    std::vector<int> future_references = {5, 4, 3, 2, 1};  // Referências futuras para OPT

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

        // Salva o contexto de cada processo antes de iniciar a simulação
        process1.save_context();
        process2.save_context();

        // Carrega o contexto de cada processo antes de simular
        process1.load_context();
        process2.load_context();

        // Executa o escalonador com a política escolhida
        scheduler.run(policy, future_references);

        // Mostra o estado atual da memória física após a execução do escalonador
        std::cout << "Memory state after scheduling:" << std::endl;
        simulator.display_frames();  // Chama a função para exibir a memória

        // Pausa o sistema para a próxima execução
        std::cout << "Press any key to continue..." << std::endl;
        std::cin.ignore();
        std::cin.get();
    }

    std::cout << "Program terminated." << std::endl;
    return 0;
}
