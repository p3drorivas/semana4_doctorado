; program.asm - factorial recursivo usando stack y CALL/RET
; entrada: n en memoria[200]
; salida: AC contiene factorial(n) y se imprime

        LOAD 200      ; AC = n
        STORE 0       ; memoria[0] = n (variable N)

        CALL FACT     ; llamar factorial (etiqueta FACT)
        OUT           ; imprimir AC (resultado)
        HALT

; -------------------------
; FACT subrutina (recursiva)
; -------------------------
FACT:
        LOAD 0        ; AC = N
        JZ FACT_BASE  ; si N == 0 -> caso base

        ; guardar N original en stack
        LOAD 0
        PUSH          ; push AC (N original)

        ; N = N - 1
        LOAD 0
        SUB 201       ; N - 1  (memoria[201] = 1)
        STORE 0

        ; llamada recursiva factorial(N-1)
        CALL FACT

        ; al volver, AC = factorial(N-1)
        STORE 2       ; guardar resultado parcial en memoria[2]

        POP           ; recuperar N_original -> AC
        STORE 3       ; guardar N_original en memoria[3]

        LOAD 2        ; AC = factorial(N-1)
        MUL 3         ; AC = AC * memoria[3] (factorial = fact(n-1) * n)
        RET

; -------------------------
; Caso base factorial(0) = 1
; -------------------------
FACT_BASE:
        LOAD 201      ; AC = 1
        RET
