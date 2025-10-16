#ifndef MEMORY_BLOCK_H
#define MEMORY_BLOCK_H

#include <string>

// Representa un único bloque de memoria (ocupado o libre)
struct MemoryBlock {
    int size;                 // Tamaño del bloque en unidades
    bool is_free;             // Estado: true si está libre, false si está ocupado
    std::string process_id;   // "Libre" o el ID del proceso ("P1", "P2", etc.)
    
    // Punteros para la lista doblemente enlazada
    MemoryBlock* next;        // Puntero al siguiente bloque en la memoria
    MemoryBlock* prev;        // Puntero al bloque anterior en la memoria
};

#endif //MEMORY_BLOCK_H