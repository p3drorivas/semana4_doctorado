// assembler.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern unsigned char memoria[]; // viene de cpu_simulator.c

// Opcodes (deben coincidir con cpu_simulator.c)
#define OP_HALT  0
#define OP_LOAD  1
#define OP_ADD   2
#define OP_SUB   3
#define OP_STORE 4
#define OP_OUT   5
#define OP_CALL  6
#define OP_RET   7
#define OP_JMP   8
#define OP_JZ    9
#define OP_PUSH  10
#define OP_POP   11
#define OP_MUL   12

// Trim helpers (compatibles con Dev-C++)
static char *ltrim(char *s) {
    while (*s && isspace((unsigned char)*s)) s++;
    return s;
}
static void rtrim_inplace(char *s) {
    int len = (int)strlen(s);
    while (len > 0 && isspace((unsigned char)s[len-1])) { s[len-1] = '\0'; len--; }
}
static char *trim(char *s) {
    rtrim_inplace(s);
    return ltrim(s);
}

// Label table
typedef struct { char name[64]; int addr; } Label;

int get_opcode(const char *w) {
    if (!strcmp(w,"LOAD")) return OP_LOAD;
    if (!strcmp(w,"ADD")) return OP_ADD;
    if (!strcmp(w,"SUB")) return OP_SUB;
    if (!strcmp(w,"STORE")) return OP_STORE;
    if (!strcmp(w,"OUT")) return OP_OUT;
    if (!strcmp(w,"CALL")) return OP_CALL;
    if (!strcmp(w,"RET")) return OP_RET;
    if (!strcmp(w,"JMP")) return OP_JMP;
    if (!strcmp(w,"JZ")) return OP_JZ;
    if (!strcmp(w,"PUSH")) return OP_PUSH;
    if (!strcmp(w,"POP")) return OP_POP;
    if (!strcmp(w,"MUL")) return OP_MUL;
    if (!strcmp(w,"HALT")) return OP_HALT;
    return -1;
}

// Ensamblador de dos pasadas: resuelve etiquetas y carga bytes en memoria[]
int ensamblar_y_cargar(const char *file, int start_addr) {
    FILE *in = fopen(file, "r");
    if (!in) { printf("No se encontró %s\n", file); return 0; }

    char line[512];
    Label labels[256];
    int label_count = 0;
    int ptr = start_addr;

    // Primera pasada: registrar etiquetas (direcciones en bytes)
    while (fgets(line, sizeof(line), in)) {
        char *s = trim(line);
        if (*s == ';' || *s == '\0') continue;
        // detectar etiqueta "NAME:"
        char *colon = strchr(s, ':');
        if (colon) {
            *colon = '\0';
            char *lbl = trim(s);
            if (strlen(lbl) > 0) {
                strcpy(labels[label_count].name, lbl);
                labels[label_count].addr = ptr;
                label_count++;
            }
            // mover s al texto después de ':'
            s = trim(colon + 1);
            if (*s == '\0') continue;
        }
        // contar instrucción (2 bytes)
        ptr += 2;
    }

    // Segunda pasada: generar bytes
    rewind(in);
    ptr = start_addr;
    while (fgets(line, sizeof(line), in)) {
        char *s = trim(line);
        if (*s == ';' || *s == '\0') continue;
        // si hay etiqueta al inicio
        char *colon = strchr(s, ':');
        if (colon) {
            s = trim(colon + 1);
            if (*s == '\0') continue;
        }

        // parse instruction and operand (operand puede ser label o número)
        char instr[64] = {0};
        char operand_str[128] = {0};
        int fields = sscanf(s, "%63s %127s", instr, operand_str);
        if (fields < 1) continue;

        // uppercase instr
        for (char *p = instr; *p; ++p) *p = (char)toupper((unsigned char)*p);

        int opcode = get_opcode(instr);
        if (opcode == -1) {
            printf("Instrucción inválida: %s\n", instr);
            fclose(in);
            return 0;
        }

        // instrucciones sin operandos
        if (opcode == OP_RET || opcode == OP_POP || opcode == OP_PUSH || opcode == OP_OUT || opcode == OP_HALT) {
            memoria[ptr++] = (unsigned char)opcode;
            memoria[ptr++] = (unsigned char)0;
            continue;
        }

        if (fields < 2) {
            printf("Falta operando para %s\n", instr);
            fclose(in);
            return 0;
        }

        int operand = 0;
        // si operand_str es número decimal (o negativo)
        if ( (operand_str[0] == '-') || isdigit((unsigned char)operand_str[0]) ) {
            operand = atoi(operand_str) & 0xFF;
        } else {
            // buscar etiqueta
            int found = 0;
            for (int i = 0; i < label_count; ++i) {
                if (!strcmp(labels[i].name, operand_str)) { operand = labels[i].addr; found = 1; break; }
            }
            if (!found) {
                printf("Operando/etiqueta no encontrada: %s\n", operand_str);
                fclose(in);
                return 0;
            }
        }

        memoria[ptr++] = (unsigned char)opcode;
        memoria[ptr++] = (unsigned char)(operand & 0xFF);
    }

    fclose(in);
    return 1;
}
