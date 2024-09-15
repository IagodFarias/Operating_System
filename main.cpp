#include "MemorySimulator.h"
#include <iostream>
#include <vector>
#include <string>
#include <random>  // Para std::default_random_engine e std::normal_distribution
#include <ctime>   // Para std::time

void generateRadomVector(std::vector<int>& vector){
    int mean = 10 , stddev = 2;  // Média e desvio padrão

    std::default_random_engine generator(static_cast<unsigned>(std::time(0)));

    std::normal_distribution<double> distribution(mean, stddev);

    for (auto& num: vector) {
        num = distribution(generator);  // Gera um número de acordo com a distribuição normal
    }
 }

int main() {
    int pagesNumber= 100;
    int memory_size = 3;  // Defina o tamanho da memória
    std::vector<int> pagesToAccess(pagesNumber); //= {1, 2, 3, 3, 1, 4, 5, 1, 2, 5, 4, 5, 3, 1, 2, 4, 5, 1, 3, 4, 2, 5, 1, 3,1, 2, 3, 3, 1, 4, 5, 1, 2, 5, 4, 5, 3, 1, 2, 4, 5, 1, 3, 4, 2, 5, 1, 3, 21, 2, 3, 3, 1, 4, 5, 1, 2, 5, 4, 5, 3, 1, 2, 4, 5, 1, 3, 4, 2, 5, 1, 3, 2 ,3, 1, 2, 4, 5, 1, 3, 4, 2, 5, 1, 3, 21, 2, 3, 3, 1, 4, 5, 1, 2, 5, 4, 5, 3, 1, 2, 4, 5, 1, 3, 4, 2, 5, 1, 3, 2,  3, 1, 2, 4, 5, 1, 3, 4, 2, 5, 1, 3, 21, 2, 3, 3, 1, 4, 5, 1, 2, 5, 4, 5, 3, 1, 2, 4, 5, 1, 3, 4, 2, 5, 1, 3, 2};  // Páginas predefinidas
    std::vector<int> future_references = {5, 4 ,3 ,2 ,1};  // Referências futuras para OPT
    generateRadomVector(pagesToAccess);

    for (auto& num: pagesToAccess) {
        std::cout << num << std::endl;
    }

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
