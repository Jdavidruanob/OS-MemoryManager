#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "MemoryBlock.h"
#include <string>
#include <vector>

class MemoryManager {
public:
    // El constructor inicializa la memoria con un solo bloque libre.
    MemoryManager(int total_size, const std::string& policy);
    
    // El destructor se encarga de liberar toda la memoria de la lista enlazada.
    ~MemoryManager();

    // Métodos públicos para interactuar con el gestor.
    void allocate(const std::string& p_id, int size);
    void deallocate(const std::string& p_id);
    void showMemory() const;
    void reportFragmentation() const;

private:
    MemoryBlock* head; // Puntero al inicio de nuestra memoria simulada
    std::string allocation_policy; // "ff" (First Fit), "bf" (Best Fit), o "wf" (Worst Fit)

    // Métodos privados para la lógica interna.
    void splitBlock(MemoryBlock* block, int required_size, const std::string& p_id);
    void mergeFreeBlocks(MemoryBlock* block);
   
};

#endif //MEMORY_MANAGER_H