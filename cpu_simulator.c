// cpu_simulator.c
#include <stdio.h>
#include <stdlib.h>

unsigned char memoria[256]; // RAM (programa + datos)
int PC = 0;    // program counter (byte index)
int AC = 0;    // acumulador (int para cálculos)
int SP = 255;  // stack pointer (stack crece hacia abajo)

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
#define OP_PUSH  10   // push AC
#define OP_POP   11   // pop -> AC
#define OP_MUL   12

// Helpers de pila (trabajan sobre memoria[])
void push_byte(unsigned char v) {
    if (SP <= 0) { printf("STACK OVERFLOW\n"); exit(1); }
    memoria[SP--] = v;
}
unsigned char pop_byte() {
    if (SP >= 255) { printf("STACK UNDERFLOW\n"); exit(1); }
    return memoria[++SP];
}

// Ejecutar una instrucción (opcode, operand)
void ejecutar(unsigned char opcode, unsigned char operand) {
    switch (opcode) {
        case OP_LOAD:
            AC = memoria[operand];
            break;
        case OP_ADD:
            AC = AC + memoria[operand];
            break;
        case OP_SUB:
            AC = AC - memoria[operand];
            break;
        case OP_STORE:
            memoria[operand] = (unsigned char)(AC & 0xFF);
            break;
        case OP_OUT:
            printf("OUTPUT → %d\n", AC);
            break;
        case OP_HALT:
            printf("FIN\n");
            exit(0);
            break;
        case OP_CALL: {
            unsigned char ret = (unsigned char)(PC & 0xFF);
            push_byte(ret);     // push return address
            PC = operand;
            break;
        }
        case OP_RET: {
            unsigned char ret = pop_byte();
            PC = (int)ret;
            break;
        }
        case OP_JMP:
            PC = operand;
            break;
        case OP_JZ:
            if (AC == 0) PC = operand;
            break;
        case OP_PUSH:
            push_byte((unsigned char)(AC & 0xFF));
            break;
        case OP_POP:
            AC = (int)pop_byte();
            break;
        case OP_MUL:
            AC = AC * memoria[operand];
            break;
        default:
            printf("Instr desconocida: %d (PC-2=%d)\n", opcode, PC-2);
            exit(1);
    }
}
