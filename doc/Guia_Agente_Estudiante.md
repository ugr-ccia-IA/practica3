# Guía de Implementación: Clase AgenteEstudiante

Esta clase es el núcleo de la práctica. Aquí es donde los estudiantes deben implementar la lógica de inteligencia artificial utilizando el algoritmo **MiniMax con Poda Alfa-Beta**.

## 1. Estructura de la Clase (`AgenteEstudiante.hpp`)

La clase hereda de `Agente` y define los componentes necesarios para la búsqueda y evaluación de estados.

### Constantes de Evaluación:
*   **`GANAR` (1e12)**: Valor asignado a un estado de victoria.
*   **`PERDER` (-1e12)**: Valor asignado a un estado de derrota.
*   **`MasInfinito` / `MenosInfinito`**: Valores para inicializar los límites de la poda Alfa-Beta.

### Tipos de Datos:
*   **`ModoJuego`**: Define si el agente juega en modo `ALEATORIO`, `MINIMAX`, `INTELIGENTE` (alfa-beta) o `STATUS` (resolución completa).

*   **`Resultado`**: Utilizado por el algoritmo Status. Tiene tres valores: `VICTORIA`, `DERROTA` y `EMPATE`.

---

## 2. Métodos Principales (`AgenteEstudiante.cpp`)

### `think(const Tablero& tablero)`
Es el punto de entrada de la IA. Dependiendo del `modo`, llama a `JuegaAleatorio`, `JuegaInteligente` o `Status`.

### `JuegaInteligente(const Tablero& tablero)`
Gestiona el inicio de la búsqueda. Por defecto llama a `minimax`, pero el estudiante debe cambiarlo a `alfaBeta` una vez implementado. Aquí se puede implementar **Profundización Iterativa** si se desea.

### `alfaBeta(const Tablero &tablero, int profundidad, int prof_Max, double alfa, double beta, std::pair<int,int> &Mov)`
Es el corazón del algoritmo de búsqueda.
*   **Parámetros**:
    *   `tablero`: Estado actual a explorar.
    *   `profundidad`: Nivel actual en el árbol.
    *   `prof_Max`: Límite máximo de profundidad.
    *   `alfa` / `beta`: Límites de la poda.
    *   `Mov`: Parámetro de **salida** que devolverá la mejor jugada encontrada en la raíz.

### `heuristica(const Tablero& tablero)`
La función de evaluación. El sistema permite tener varias heurísticas (`heuristica1`, `heuristica2`) seleccionables mediante el parámetro de consola `-id1` o `-id2`.

### `getSucesores(const Tablero& t)`
Genera todos los estados posibles (`std::vector<Tablero>`).
*   **Importante**: Ya viene implementado en el esqueleto. Respeta automáticamente la **Regla de la Trinidad** y las adyacencias del Modo Competición.
*   **⚠️ ADVERTENCIA (Turnos Múltiples)**: El juego utiliza una secuencia 1-2-2-2 (un jugador puede poner varias fichas seguidas). Esto significa que un "nivel de profundidad" en tu árbol Minimax no siempre se corresponde con un cambio de jugador. ¡No asumas que el nivel `d+1` siempre pertenece al oponente! Usa `tablero.getJugadorTurno()` para saber exactamente a quién le toca jugar en cada nodo.

### `Status(const Tablero &tablero, std::pair<int,int> &Mov)`
Implementa la resolución completa del juego utilizando recursividad. No usa heurísticas, busca hasta el final del juego.

---

## 3. Consejos para el Estudiante

1.  **Abortar la Búsqueda**: Dentro de `alfaBeta` y `minimax` existe un bloque de código obligatorio que comprueba `abortarBanda` y el tiempo transcurrido. **No elimines este código**, es lo que permite que la IA se detenga a tiempo.
2.  **Uso de `mejorMovimientoH`**: Si implementas Profundización Iterativa, guarda la mejor jugada de cada nivel completado en esta variable para asegurar que siempre devuelves un movimiento válido si el tiempo se agota en medio de una búsqueda más profunda.
3.  **Depuración**: Usa el modo `-nogui` para imprimir valores por consola y ver cómo evolucionan `alfa` y `beta` en el árbol.
