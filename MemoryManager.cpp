#include "MemoryManager.h"
#include <iostream>
#include <limits> // Para std::numeric_limits

MemoryManager::MemoryManager(int total_size, const std::string& policy) {
    allocation_policy = policy;
    // Inicializa la memoria con un único bloque grande y libre
    head = new MemoryBlock{total_size, true, "Libre", nullptr, nullptr};
}

MemoryManager::~MemoryManager() {
    // Libera todos los nodos de la lista para evitar fugas de memoria
    MemoryBlock* current = head;
    while (current != nullptr) {
        MemoryBlock* next = current->next;
        delete current;
        current = next;
    }
}

void MemoryManager::showMemory() const {
    MemoryBlock* current = head;
    while (current != nullptr) {
        std::cout << "[" << current->process_id << ":" << current->size << "]";
        current = current->next;
    }
    std::cout << std::endl;
}

void MemoryManager::allocate(const std::string& p_id, int size) {
    MemoryBlock* best_block = nullptr;

    if (allocation_policy == "ff") { // First Fit
        MemoryBlock* current = head;
        while (current != nullptr) {
            if (current->is_free && current->size >= size) {
                best_block = current;
                break; // Encontramos el primero, salimos
            }
            current = current->next;
        }
    } else if (allocation_policy == "bf") { // Best Fit
        int min_waste = std::numeric_limits<int>::max();
        MemoryBlock* current = head;
        while (current != nullptr) {
            if (current->is_free && current->size >= size) {
                if (current->size - size < min_waste) {
                    min_waste = current->size - size;
                    best_block = current;
                }
            }
            current = current->next;
        }
    } else if (allocation_policy == "wf") { // Worst Fit
        int max_size = -1;
        MemoryBlock* current = head;
        while (current != nullptr) {
            if (current->is_free && current->size >= size) {
                if (current->size > max_size) {
                    max_size = current->size;
                    best_block = current;
                }
            }
            current = current->next;
        }
    }

    if (best_block) {
        splitBlock(best_block, size, p_id);
    } else {
        std::cout << "Error: No hay espacio suficiente para asignar el proceso " << p_id << std::endl;
    }
}

void MemoryManager::splitBlock(MemoryBlock* block, int required_size, const std::string& p_id) {
    // Si el bloque es mucho más grande, lo dividimos
    if (block->size > required_size) {
        MemoryBlock* new_free_block = new MemoryBlock{
            block->size - required_size,
            true,
            "Libre",
            block->next,
            block
        };

        if (block->next != nullptr) {
            block->next->prev = new_free_block;
        }
        block->next = new_free_block;
        
        // Actualizamos el bloque original
        block->size = required_size;
    }
    
    // Asignamos el proceso al bloque (ya sea el original o el recién ajustado)
    block->is_free = false;
    block->process_id = p_id;
}


void MemoryManager::deallocate(const std::string& p_id) {
    MemoryBlock* current = head;
    // Buscamos el proceso a liberar
    while (current != nullptr) {
        if (current->process_id == p_id) {
            if(current->is_free) {
                std::cout << "Error: El proceso " << p_id << " ya esta libre." << std::endl;
                return;
            }
            current->is_free = true;
            current->process_id = "Libre";
            // La magia: Intentamos fusionar con los vecinos
            mergeFreeBlocks(current);
            return;
        }
        current = current->next;
    }
    std::cout << "Error: Proceso " << p_id << " no encontrado." << std::endl;
}

void MemoryManager::mergeFreeBlocks(MemoryBlock* block) {
    // 1. Intentar fusionar con el bloque SIGUIENTE
    if (block->next && block->next->is_free) {
        MemoryBlock* next_block = block->next;
        block->size += next_block->size; // Aumentamos el tamaño
        block->next = next_block->next;   // Nos saltamos el bloque fusionado
        if (next_block->next) {
            next_block->next->prev = block;
        }
        delete next_block; // Liberamos la memoria del nodo ya inútil
    }

    // 2. Intentar fusionar con el bloque ANTERIOR
    if (block->prev && block->prev->is_free) {
        MemoryBlock* prev_block = block->prev;
        prev_block->size += block->size; // El anterior absorbe al actual
        prev_block->next = block->next;
        if (block->next) {
            block->next->prev = prev_block;
        }
        delete block; // Liberamos la memoria del nodo actual
    }
}


void MemoryManager::reportFragmentation() const {
    int total_free_space = 0;
    int free_blocks_count = 0;
    int largest_free_block = 0;

    MemoryBlock* current = head;
    while (current != nullptr) {
        if (current->is_free) {
            total_free_space += current->size;
            free_blocks_count++;
            if (current->size > largest_free_block) {
                largest_free_block = current->size;
            }
        }
        current = current->next;
    }

    std::cout << "--- Reporte de Fragmentacion ---" << std::endl;
    std::cout << "Fragmentacion Interna: 0 (debido a la asignacion dinamica)" << std::endl;
    std::cout << "Fragmentacion Externa:" << std::endl;
    std::cout << "  - Espacio total libre: " << total_free_space << std::endl;
    std::cout << "  - Numero de huecos libres: " << free_blocks_count << std::endl;
    std::cout << "  - Hueco libre mas grande: " << largest_free_block << std::endl;
    std::cout << "--------------------------------" << std::endl;
}