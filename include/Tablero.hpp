#ifndef TABLERO_HPP
#define TABLERO_HPP

#include <vector>
#include <string>

/**
 * @brief Clase que representa el tablero de juego para N-en-raya.
 * Permite dimensiones configurables y define el número de piezas necesarias para ganar.
 */
class Tablero {
public:
    enum class TipoCelda { NORMAL, ROJO, VERDE, AMARILLO };

    /**
     * @brief Constructor del tablero.
     * @param filas Número de filas del tablero.
     * @param cols Número de columnas del tablero.
     * @param nParaGanar Número de piezas en línea necesarias para ganar.
     */
    Tablero(int filas, int cols, int nParaGanar);
    static Tablero cargarDesdeFichero(const std::string& ruta);

    /**
     * @brief Intenta colocar una pieza en el tablero.
     * @param f Fila donde colocar la pieza.
     * @param c Columna donde colocar la pieza.
     * @param jugador ID del jugador (1 o 2).
     * @return true si el movimiento fue válido y se realizó con éxito.
     */
    bool ponerPieza(int f, int c, int jugador);
    std::vector<Tablero> getSucesores() const; // Devuelve los posibles tableros hijos
    // Devuelve los posibles tableros hijos con el último movimiento realizado
    std::vector<std::pair<Tablero, std::pair<int,int>>> getSucesoresConMovimientos() const; 
    bool tieneMovimientosValidos() const; // Comprueba si el jugador actual puede mover
    void pasarTurno() { turnoActual++; }  // Incrementa el turno sin poner pieza

    /**
     * @brief Comprueba si hay un ganador en el estado actual.
     * @return 0 si no hay ganador, 1 o 2 para el jugador correspondiente, -1 si es empate.
     */
    int comprobarGanador() const;

    /**
     * @brief Indica si el tablero está completamente lleno.
     */
    bool estaLleno() const;

    /**
     * @brief Indica si el tablero está completamente vacío.
     */
    bool esVacio() const;

    /**
     * @brief Comprueba si una celda tiene alguna pieza en su vecindad de 8.
     */
    bool tieneAdyacente(int f, int c) const;

    // Getters básicos
    int getFilas() const { return filas; }
    int getColumnas() const { return columnas; }
    int getNParaGanar() const { return nParaGanar; }
    int getCelda(int f, int c) const { return rejilla[f][c]; }
    const std::vector<std::pair<int, int>>& getLineaGanadora() const { return lineaGanadora; }

    /**
     * @brief Cuenta cuántas combinaciones de una longitud dada tiene un jugador.
     */
    int contarCombinaciones(int longitud, int jugador) const;

    /**
     * @brief Busca y devuelve todas las líneas de una longitud dada para un jugador.
     */
    std::vector<std::vector<std::pair<int, int>>> buscarTodasLasLineas(int longitud, int jugador) const;

    /**
     * @brief Determina el ganador por puntos (5-en-raya, luego 4-en-raya) cuando el tablero está lleno.
     * @return 1 o 2 para el ganador, -1 para empate real.
     */
    int getGanadorDesempate() const;

    /**
     * @brief Retorna el tipo de especialidad de una celda.
     */
    TipoCelda getTipoCelda(int f, int c) const { return especialidad[f][c]; }

    int getTurnoActual() const { return turnoActual; }
    int getJugadorTurno() const { return jugadorTurno; }
    int getMovimientosRestantes() const { return movimientosRestantes; }
    int getFaseActual() const { return faseActual; }

private:
    int jugadorTurno; // Jugador que debe mover ahora (1 o 2)
    int movimientosRestantes; // Cuántas piezas le quedan por poner en este turno
    int faseActual; // Fase de la trinidad (se incrementa al cambiar de jugador)
    int filas;
    int columnas;
    int nParaGanar;
    int turnoActual; // Contador global de movimientos realizados (fichas en tablero)
    std::vector<std::vector<int>> rejilla; // 0: Vacío, 1: Jugador 1, 2: Jugador 2
    std::vector<std::vector<TipoCelda>> especialidad; // Mapa de tipos de celda
    mutable std::vector<std::pair<int, int>> lineaGanadora; // Coordenadas de la línea que ha ganado
};

#endif // TABLERO_HPP
