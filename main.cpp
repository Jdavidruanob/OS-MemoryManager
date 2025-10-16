#include <iostream>
#include <string>
#include <vector>
#include <limits> // <-- LÍNEA AGREGADA PARA CORREGIR EL ERROR
#include "MemoryManager.h"

int main(int argc, char* argv[]) {
    // Verificación de los argumentos de línea de comandos
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <tamano_memoria> <politica>" << std::endl;
        std::cerr << "Politicas disponibles: ff (First Fit), bf (Best Fit), wf (Worst Fit)" << std::endl;
        return 1;
    }

    int memory_size = std::stoi(argv[1]);
    std::string policy = argv[2];

    if (policy != "ff" && policy != "bf" && policy != "wf") {
        std::cerr << "Politica invalida. Use 'ff', 'bf', or 'wf'." << std::endl;
        return 1;
    }

    // Creamos nuestro gestor de memoria
    MemoryManager manager(memory_size, policy);

    char command;
    std::string process_id;
    int size;

    // Bucle para leer comandos de la entrada estándar (consola o archivo)
    while (std::cin >> command) {
        switch (command) {
            case 'A':
                std::cin >> process_id >> size;
                manager.allocate(process_id, size);
                break;
            case 'L':
                std::cin >> process_id;
                manager.deallocate(process_id);
                break;
            case 'M':
                manager.showMemory();
                break;
            case 'F': // <-- AÑADIR ESTE NUEVO CASO
                manager.reportFragmentation();
                break;
            default:
                std::cerr << "Comando desconocido: " << command << std::endl;
                // Ignorar el resto de la línea para evitar errores en cascada
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
        }
    }

    return 0;
}