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
        page = static_cast<int>(distribution(generator));  // Gera um número de acordo com a distribuição normal
        if (page < 0) page = 0; // Garantir que não haja números negativos
    }
}

int main() {
    int memory_size = 10;  // Defina o tamanho da memória
    int time_quantum = 5;
    int pagesNumber = 20;  // Reduzi o número de páginas para facilitar o teste
    Scheduler scheduler(time_quantum);  // Escalona a partir do time_quantum

    // Geração de processos e suas páginas de forma aleatória
    std::vector<int> pages1(pagesNumber), pages2(pagesNumber), pages3(pagesNumber), pages4(pagesNumber);
    generateRandomPages(pages1, 12, 3);
    generateRandomPages(pages2);
    generateRandomPages(pages3);
    generateRandomPages(pages4);

    Process process1(1, pages1);  // Cria os processos, com seu ID, e páginas.
    Process process2(2, pages2);
    Process process3(3, pages3);
    Process process4(4, pages4);

    scheduler.add_process(&process1);  // Adiciona o processo ao escalonador.
    scheduler.add_process(&process2);
    scheduler.add_process(&process3);
    scheduler.add_process(&process4);

    std::vector<int> future_references = {};  // Referências futuras para OPT (pode ser preenchido conforme necessário)

    while (true) {
        MemorySimulator simulator(memory_size);  // Reinicializa o simulador a cada iteração

        // Solicita ao usuário que escolha a política de substituição
        std::string policy;
        std::cout << "Escolha uma política de substituição (FIFO, LFU, OPT) ou digite 'exit' para sair: ";
        std::cin >> policy;

        // Permite ao usuário sair do loop digitando "exit"
        if (policy == "exit") {
            break;
        }

        // Verifica se a política escolhida é válida
        if (policy != "FIFO" && policy != "LFU" && policy != "OPT") {
            std::cout << "Política inválida. Por favor, escolha entre FIFO, LFU ou OPT." << std::endl;
            continue;  // Volta para o início do loop
        }

        // Executa o escalonador com a política escolhida, passando o simulador para exibir o estado da memória
        scheduler.run(policy, future_references, simulator);

        // Pausa o sistema para a próxima execução
        std::cout << "Pressione Enter para continuar..." << std::endl;
        std::cin.ignore();
        std::cin.get();
    }

    std::cout << "Programa encerrado." << std::endl;
    return 0;
}
