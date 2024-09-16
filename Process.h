#ifndef PROCESS_H
#define PROCESS_H

#include <vector>

class Process {
private:
    int process_id;
    std::vector<int> pages_to_access;

public:
    Process(int id, const std::vector<int>& pages);

    int get_id() const { return process_id; }
    std::vector<int>& get_pages() { return pages_to_access; }

    // Removemos as funções de salvar e carregar contexto, pois não são mais necessárias
};

#endif // PROCESS_H
