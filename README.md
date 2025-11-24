#Sistema Integrado de CPU + Ensamblador + C a ASM (Factorial Recursivo)

1. Objetivo General
   El código desarrollado tiene como finalidad simular un sistema computacional completo para la ejecución de un **programa recursivo**, como el cálculo del factorial de un número. Este sistema demuestra cómo interactúan distintos niveles de programación y hardware de manera conceptual:

- Nivel de programación en C: Se escribe la función recursiva para calcular factorial.
- Nivel de ensamblador (ASM): Convierte las instrucciones de C a un formato cercano al hardware, representando operaciones básicas de la CPU.
- Nivel de CPU simulada: Ejecuta las instrucciones ensambladas, manipulando memoria, registros y una pila para manejar la recursión.

El sistema permite visualizar cómo una función recursiva se traduce y ejecuta paso a paso a nivel de máquina.

2. Componentes del Sistema

* CPU Simulada

  * Representa un procesador simple con:

    - AC (Acumulador): Registro para operaciones aritméticas.
    - PC (Contador de programa): Indica la dirección de la siguiente instrucción.
    - Memoria RAM: Arreglo de 256 bytes para almacenar datos, instrucciones y pila.
    - Stack (pila): Para manejar llamadas recursivas.
  * Funciones implementadas:

    - LOAD → Cargar un valor en AC.
    - ADD → Sumar un valor al AC.
    - STORE → Guardar AC en memoria.
    - PUSH/POP → Guardar y recuperar valores en la pila.
    - CALL/RET → Manejar subrutinas recursivas.
    - OUT → Mostrar el resultado en pantalla.
    - HALT → Finalizar la ejecución.

* Ensamblador (Assembler)

  - Convierte código C recursivo (factorial) en instrucciones ASM que la CPU pueda ejecutar.
  - Maneja direcciones de memoria, constantes, variables y operaciones de pila.

* Traductor C → ASM

  - Permite que funciones recursivas escritas en C se traduzcan a ASM compatible.
  - Genera el archivo `program.asm` con instrucciones listas para la CPU simulada.

* Main Integrador

  - Activa codificación UTF-8 México para mostrar correctamente resultados.
  - Traduce `input.c` a ASM.
  - Ensambla `program.asm` en memoria.
  - Ejecuta la CPU simulada mostrando paso a paso la ejecución y el resultado del factorial.

3. Propósito Educativo

* Permite visualizar cómo la **recursión** se implementa a nivel de CPU mediante pilas y subrutinas.
* Enseña la traducción de funciones de alto nivel a instrucciones de máquina.
* Facilita la comprensión de conceptos de arquitectura y ejecución secuencial en presencia de llamadas recursivas.

4. Beneficios del Sistema

* Comprender cómo una función recursiva (factorial) se ejecuta en un procesador simulado.
* Visualizar el uso de la pila para mantener valores intermedios durante la recursión.
* Entender la relación entre C, ASM y la CPU.
* Base para implementar otras funciones recursivas o algoritmos complejos en la CPU simulada.
