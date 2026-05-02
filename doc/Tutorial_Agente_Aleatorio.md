# Tutorial: Creación de un Agente Aleatorio (Baseline)

Este tutorial guía al estudiante en la construcción de su primer agente inteligente. Un agente aleatorio es la base perfecta para entender cómo interactuar con el motor de juego.

## 1. La Idea Fundamental
El objetivo es que el agente, en su turno, sea capaz de:
1.  Identificar todas las jugadas legales.
2.  Elegir una al azar.
3.  Devolver las coordenadas de esa jugada.

---

## 2. Herramientas Disponibles
En el esqueleto de `AgenteEstudiante`, ya dispones de métodos que facilitan esta tarea:

*   **`getSucesores(tablero)`**: Devuelve un vector con todos los estados (`Tablero`) a los que se puede llegar legalmente. Este método ya filtra por ti la Regla de la Trinidad y la Adyacencia.
*   **`SacarMovimiento(padre, hijo)`**: Una función auxiliar que compara dos tableros y te dice en qué fila y columna se puso la nueva pieza.

---

## 3. Implementación Paso a Paso

### Paso 1: Obtener los sucesores
Invocamos a `getSucesores` pasando el tablero actual.
```cpp
auto sucesores = getSucesores(tablero);
```

### Paso 2: Comprobar bloqueos
Si el vector está vacío, significa que el jugador no puede mover en ninguna casilla (bloqueo por Trinidad o Adyacencia). En este caso, debemos devolver `(-1, -1)` para pasar el turno.
```cpp
if (sucesores.empty()) return {-1, -1};
```

### Paso 3: Selección aleatoria
Elegimos un índice al azar del vector y extraemos las coordenadas del movimiento realizado.
```cpp
int elegido = rand() % sucesores.size();
std::pair<int, int> Mov = SacarMovimiento(tablero, sucesores[elegido]);
return Mov;
```

---

## 4. Ejemplo Completo en `JuegaAleatorio`

```cpp
std::pair<int, int> AgenteEstudiante::JuegaAleatorio(const Tablero& tablero) {
    // 1. Obtenemos los estados legales
    auto sucesores = getSucesores(tablero);

    // 2. Si no hay movimientos, pasamos turno
    if (sucesores.empty()) return {-1, -1};

    // 3. Elegimos uno al azar
    int elegido = rand() % sucesores.size();

    // 4. Identificamos qué casilla se ha marcado
    return SacarMovimiento(tablero, sucesores[elegido]);
}
```

---

## 5. ¿Por qué empezar por aquí?
Implementar correctamente el agente aleatorio asegura que:
1.  Entiendes la diferencia entre un objeto `Tablero` y un par de coordenadas.
2.  Sabes cómo gestionar el caso de "no hay movimientos válidos".
3.  Tienes una base sólida para el método `JuegaInteligente`, donde en lugar de elegir al azar, usarás `alfaBeta` para elegir el sucesor con mejor valoración heurística.
