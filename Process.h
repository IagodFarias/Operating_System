#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include <string>

class Process {
private:
    int process_id;
    std::vector<int> pages_to_access;

public:
    // Construtor que recebe o ID do processo e as páginas que ele acessará
    Process(int id, const std::vector<int>& pages);
    
    // Métodos para salvar e carregar o contexto do processo
    void save_context() const;
    void load_context();

    // Retorna o ID do processo
    int get_id() const { return process_id; }

    // Retorna as páginas que o processo precisa acessar
    // Adicionar uma versão não constante de get_pages()
std::vector<int>& get_pages() { return pages_to_access; }

};

#endif // PROCESS_H
