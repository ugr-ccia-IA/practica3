# Guía de Parámetros de Línea de Comandos - Ninja 9x9

El binario principal `./n_en_raya` permite una configuración total de la partida mediante parámetros. Esto es especialmente útil para realizar experimentos de rendimiento, comparar heurísticas o ejecutar pruebas automáticas sin interfaz gráfica.

## 1. Definición de Jugadores (`-p1` y `-p2`)
Define el tipo de agente que controlará a cada jugador.
*   **`humano`** (por defecto): El jugador es controlado por una persona a través de la interfaz gráfica o la consola.
*   **`aleatorio`**: El agente elige cualquier casilla válida de forma totalmente al azar. Útil como oponente básico para pruebas iniciales.
*   **`minimax`**: Activa el agente con búsqueda Minimax básica (sin poda).
*   **`inteligente`**: Activa el agente con búsqueda Minimax y Poda Alfa-Beta (implementado por el estudiante).

*   **`status`**: Activa la búsqueda exhaustiva completa para encontrar el veredicto teórico (Gana/Pierde/Empata). **Atención**: Ignora el límite de tiempo (`-t`) y solo es viable en tableros pequeños.
*   **`ninja1`, `ninja2`, `ninja3`, `ninja4`**: Agentes pre-entrenados con distintos niveles de dificultad que actúan como "jefes" a batir.

---

## 2. Configuración del Tablero (`-f`, `-c`, `-n`)
Permite cambiar las dimensiones y las reglas de victoria.
*   **`-f <int>`**: Número de filas. (Defecto: 9).
*   **`-c <int>`**: Número de columnas. (Defecto: 9).
*   **`-n <int>`**: Número de piezas consecutivas necesarias para ganar. (Defecto: 5).

> [!IMPORTANT]
> Cambiar estas dimensiones afecta directamente a la **Regla de la Trinidad**, ya que el residuo válido depende de la posición `(f+c)%3`. Estas reglas avanzadas (Trinidad y Adyacencia) solo se activan en el **Modo Competición** (9x9 con n=5).

---

## 3. Parámetros de la IA (`-d`, `-t`, `-id1`, `-id2`)
Controlan el comportamiento del algoritmo Minimax.
*   **`-d <int>`**: Profundidad máxima de búsqueda. Un valor más alto hace a la IA más "sabia" pero consume mucho más tiempo. (Defecto: 5).
*   **`-t <double>`**: Límite de tiempo en segundos por movimiento. La IA utiliza este valor para abortar su búsqueda si se está excediendo. (Defecto: 5.0).
*   **`-id1 <int>` / `-id2 <int>`**: Selector de heurística. Permite elegir qué función de evaluación usar para el J1 o J2 respectivamente.
    *   `1`: Heurística avanzada (Ninja).
    *   `2`: Heurística básica (Baseline).

---

## 4. Otros Parámetros
*   **`-nogui`**: Desactiva la interfaz gráfica. La partida se ejecuta a máxima velocidad y el tablero se imprime por consola en cada turno. Imprescindible para realizar experimentos masivos (benchmarking).
*   **`-file <nombreFichero>`**: Carga un estado de tablero desde un fichero de texto. Esto anula los parámetros `-f`, `-c` y `-n`.
*   **`-h`**: Muestra el mensaje de ayuda con un resumen de todas las opciones.

### Formato del Fichero de Tablero (`-file`)
El fichero debe ser un texto plano con la siguiente estructura:
```text
filas columnas objetivo
fase jugadorTurno movs_rest turno_actual
[rejilla de 0, 1, 2]
```
*   **fase**: Valor que determina el residuo `(f+c)%3` permitido (avanza cuando cambia el jugador).
*   **jugadorTurno**: 1 o 2.
*   **movs_rest**: Número de piezas que le quedan por colocar al jugador en este turno (secuencia 1-2-2-2).
*   **turno_actual**: Número total de piezas que ya han sido colocadas en el tablero (plies).
*   **rejilla**: Matriz de enteros separados por espacios (0: vacío, 1: J1, 2: J2).

---

## 5. Ejemplos Avanzados de Ejecución

### Ejemplo A: Entrenamiento contra Ninja de nivel 2
```bash
./n_en_raya -p1 humano -p2 ninja2 -f 9 -c 9 -n 5
```
*Ideal para practicar estrategias contra un oponente de nivel medio antes de programar tu propia IA.*

### Ejemplo B: Experimento de Profundidad (Modo Texto)
```bash
./n_en_raya -p1 inteligente -id1 1 -p2 inteligente -id2 1 -d 7 -t 10.0 -nogui
```
*Enfrenta a la misma IA contra sí misma a profundidad 7. Al no haber interfaz, la partida se resolverá rápidamente y verás el log de nodos visitados en la terminal.*

### Ejemplo C: Comparativa de Heurísticas
```bash
./n_en_raya -p1 inteligente -id1 1 -p2 inteligente -id2 2 -d 4 -nogui
```
*Duelo entre tu mejor heurística (ID 1) y la heurística básica (ID 2). Es la mejor forma de validar científicamente si tus mejoras en la función de evaluación son realmente efectivas.*

### Ejemplo E: Resolución Completa (Tablero 3x3)
```bash
./n_en_raya -p1 status -f 3 -c 3 -n 3 -nogui
```
*Intenta resolver el 3-en-raya clásico. Verás por consola si el primer movimiento lleva a una victoria forzada o a un empate.*

### Ejemplo D: Partida Rápida (Blitz)
```bash
./n_en_raya -p1 inteligente -p2 inteligente -t 0.5 -d 10
```
*Obliga a la IA a jugar muy rápido (medio segundo). La IA usará "Iterative Deepening" para devolver la mejor jugada que haya encontrado antes de que se agote el tiempo, probablemente sin llegar a la profundidad 10.*
