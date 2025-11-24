// c_to_asm.c  (traductor muy simple para asignaciones y OUTPUT)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int var_to_addr(char v) {
    if (v >= 'A' && v <= 'Z') return v - 'A';
    return 0;
}

int next_free_const = 128;

void traducir_C_a_ASM() {
    FILE *in = fopen("input.c", "r");
    if (!in) { printf("No se encontró input.c (se puede omitir)\n"); return; }
    FILE *out = fopen("program.asm", "w");
    if (!out) { fclose(in); printf("No se pudo abrir program.asm para escritura\n"); return; }

    char line[256];
    while (fgets(line, sizeof(line), in)) {
        char a,b;
        int num;
        if (sscanf(line, " %c = %d ;", &a, &num) == 2) {
            int const_addr = next_free_const++;
            // no inicializamos memoria[const_addr] desde aquí; main puede hacerlo
            fprintf(out, "LOAD %d\nSTORE %d\n", const_addr, var_to_addr(a));
        } else if (sscanf(line, " %c = %c + %d ;", &a, &b, &num) == 3) {
            int const_addr = next_free_const++;
            fprintf(out, "LOAD %d\nADD %d\nSTORE %d\n", var_to_addr(b), const_addr, var_to_addr(a));
        } else if (sscanf(line, " OUTPUT %c ;", &a) == 1) {
            fprintf(out, "LOAD %d\nOUT\n", var_to_addr(a));
        }
    }

    fprintf(out, "HALT\n");
    fclose(in);
    fclose(out);
}
