#include "Process.h"
#include <fstream>
#include <iostream>

// Construtor que inicializa o processo com um ID e uma lista de páginas
Process::Process(int id, const std::vector<int>& pages)
    : process_id(id), pages_to_access(pages) {}


