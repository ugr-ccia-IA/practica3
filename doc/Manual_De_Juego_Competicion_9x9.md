# Manual del Juego: De las Tres en Raya al Modo Competición 9x9

## 1. Introducción Histórica
El juego de las "Tres en Raya" (conocido en inglés como *Tic-Tac-Toe*) es uno de los juegos de estrategia más antiguos de la humanidad. Se han encontrado grabados de tableros similares en templos del Antiguo Egipto que datan del 1300 a.C. En el Imperio Romano, una variante conocida como *Terni Lapilli* era extremadamente popular, jugándose en las calles y plazas de la ciudad.

A pesar de su sencillez, el juego ha perdurado durante milenios como una herramienta fundamental para enseñar conceptos básicos de lógica y teoría de juegos. Sin embargo, su limitación principal es que, con juego perfecto, siempre acaba en empate. Por ello, a lo largo de la historia han surgido variantes complejas para aumentar el "espacio de estados" y el desafío intelectual.

---

## 2. La Versión Básica (3-en-Raya)
En su forma más elemental, el juego se desarrolla en un tablero de **3x3**. Dos jugadores (X y O) se turnan para colocar su marca en una casilla vacía. El primer jugador en conseguir tres de sus marcas en una línea horizontal, vertical o diagonal gana la partida.

En esta versión básica:
*   El tablero es estático y pequeño (9 casillas).
*   No hay restricciones de colocación más allá de que la casilla esté vacía.
*   El factor de ramificación es muy pequeño (9!).

---

## 3. El Modo Competición: La Variante 9x9
La versión de **Competición 9x9** que utilizamos en esta práctica eleva el juego original a una nueva dimensión estratégica, introduciendo reglas restrictivas y elementos dinámicos que impiden la resolución trivial.

### El Tablero y el Objetivo
Aunque el tamaño por defecto es **9x9**, el software permite dimensiones variables. El objetivo estándar es conseguir **5 piezas en raya** (configurable con el parámetro `-n`).

### Reglas de Colocación
A diferencia del juego básico, no puedes poner tu pieza donde quieras. Debes cumplir dos leyes sagradas:

1.  **Secuencia de Turnos (1-2-2-2)**: El juego no es estrictamente uno-a-uno. En su primer turno de la partida, el Jugador 1 coloca **1** pieza. A partir de ese momento, los jugadores se alternan colocando **hasta 2 piezas consecutivas** por turno, permitiendo construir combinaciones complejas.
2.  **La Regla de la Trinidad**: El tablero está dividido en tres "fases" invisibles (residuo 0, 1 y 2). En cada turno de jugador, solo puedes colocar piezas en aquellas casillas `(fila, columna)` cuyo sumatorio dividido por tres coincida con el resto de la fase actual:
    ` (fila + columna) % 3 == faseActual % 3 `
    *(Nota: La `faseActual` solo avanza cuando un jugador agota sus movimientos y cede el turno al rival).*
3.  **La Regla de Adyacencia**: Para fomentar el conflicto directo, solo puedes colocar una pieza en una casilla que esté tocando (en cualquiera de las 8 direcciones) a una pieza ya existente en el tablero. (Excepción: El primer movimiento de la partida es libre).

---

## 4. Casillas Especiales (Efectos Dinámicos)
El tablero de Competición contiene casillas con poderes especiales que pueden cambiar el rumbo de la partida en un solo movimiento:

*   **🟢 Casillas Místicas (Verdes)**: Al colocar una pieza aquí, el jugador obtiene **+1 movimiento extra inmediato** en su turno actual. Es una ventaja táctica enorme para colocar 3 piezas seguidas, cerrar líneas o bloquear al rival.
*   **🔴 Casillas de Sabotaje (Rojas)**: Representan una trampa. Si colocas tu pieza aquí, el sistema la transformará automáticamente en una pieza del **oponente**. Debes evitarlas a toda costa, o usarlas para bloquear sutilmente.
*   **🟡 Casillas Bomba (Amarillas)**: Al activarse, la casilla detona y **limpia por completo** su fila y su columna. Es la herramienta definitiva para destruir las construcciones del rival y reabrir el tablero.

---

## 5. El Sistema de Puntuación (Tiebreaker)
Si el tablero se llena y ningún jugador ha conseguido el objetivo de piezas en raya, se activa el sistema de puntos para determinar al ganador:
1.  Se cuentan las líneas de **N** piezas (victoria directa durante el juego).
2.  Si el tablero se llena, el ganador se decide exclusivamente por el número de **líneas de N-1** (4 piezas).
3.  En caso de persistir el empate en el número de líneas de 4, el resultado final es un empate técnico.

---

> [!TIP]
> Recuerda que el **Algoritmo Status** de la interfaz te permite ver si, bajo estas complejas reglas, el estado actual del tablero tiene un ganador matemático garantizado. ¡Úsalo para estudiar cómo afectan las casillas rojas a la victoria forzada!
