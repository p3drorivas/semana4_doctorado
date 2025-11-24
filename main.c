// main.c
#include <stdio.h>    // Para printf, scanf, FILE, fopen, fclose
#include <stdlib.h>   // Para exit, malloc, free
#include <string.h>   // Para strcmp, strlen, sscanf
#include <locale.h>   // Para setlocale
#ifdef _WIN32
#include <windows.h>  // Para SetConsoleCP y SetConsoleOutputCP (UTF-8 en Windows)
#endif


extern unsigned char memoria[]; // desde assembler.c/cpu_simulator.c
extern int PC, AC, SP;         // no es estrictamente necesario, pero pueden usarse
void ejecutar(unsigned char opcode, unsigned char operand);
int ensamblar_y_cargar(const char *file, int start_addr);
void traducir_C_a_ASM();

int main() {
    setlocale(LC_ALL, "es_MX.UTF-8");
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

   // limpiar memoria
    memset(memoria, 0, 256);

    // inicializar constantes y entrada
    memoria[200] = 5;   // n inicial -> cambia aquí para probar otras n
    memoria[201] = 1;   // constante 1
    // memoria[202] libre (si necesitas)

    // opcional: si tienes input.c y quieres traducirlo
    // traducir_C_a_ASM();

    // ensamblar y cargar program.asm en memoria desde 0
    if (!ensamblar_y_cargar("program.asm", 0)) {
        fprintf(stderr, "Error al ensamblar program.asm\n");
        return 1;
    }

    // ejecutar ciclo fetch-decode-execute
    PC = 0;
    while (1) {
        unsigned char opcode = memoria[PC++];
        unsigned char operand = memoria[PC++];
        ejecutar(opcode, operand);
    }

    return 0;
}
