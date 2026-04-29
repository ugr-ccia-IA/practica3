#ifndef AGENTE_ESTUDIANTE_HPP
#define AGENTE_ESTUDIANTE_HPP

#include "Agente.hpp"
#include <atomic>
#include <ctime>
#include <chrono>
#include <vector>

/**
 * @brief Agente que será implementado por los estudiantes usando MiniMax y Poda Alfa-Beta.
 */
class AgenteEstudiante : public Agente {
public:
    static constexpr double GANAR = 1e12;
    static constexpr double PERDER = -1e12;
    static constexpr double MasInfinito = 1e15;
    static constexpr double MenosInfinito = -1e15;

    enum class ModoJuego { ALEATORIO, MINIMAX, INTELIGENTE, STATUS };

    /**
     * Constructor 
     */
    AgenteEstudiante(int id, int profundidadMax, double tiempoMax = 5.0, int numHeuristica = 1, ModoJuego modo = ModoJuego::INTELIGENTE);

    /**
     * @brief Decide el movimiento usando el algoritmo MiniMax con Poda Alfa-Beta.
     */
    virtual std::pair<int, int> think(const Tablero& tablero) override;


    /**
     * @brief Genera todos los estados posibles para el jugador que tiene el turno.
     */
    std::vector<Tablero> getSucesores(const Tablero& t) const;


    // --- Métodos de Juego  ---
    std::pair<int, int> JuegaAleatorio(const Tablero& tablero);
    std::pair<int, int> JuegaInteligente(const Tablero& tablero);
    

    
    // --- Algoritmos de Búsqueda y Resolución ---
    enum class Resultado { VICTORIA, EMPATE, DERROTA };

    /**
     * @brief Implementación del algoritmo Status para resolución completa.
     */
    Resultado Status(const Tablero &tablero, std::pair<int,int> &Mov);

    /**
     * @brief Implementación del algoritmo MiniMax clásico.
     */
    double minimax(const Tablero &tablero, int profundidad, int prof_Max, std::pair<int,int> &Mov);

    /**
     * @brief Implementación del algoritmo MiniMax con Poda Alfa-Beta.
     */
    double alfaBeta(const Tablero &tablero, int profundidad, int prof_Max, double alfa, double beta, std::pair<int,int> &Mov);

    virtual void interrumpir() override { abortarBanda = true; }
    virtual bool tieneLimiteDeTiempo() const override;

private:
    int id;                // ID del jugador (1 o 2)
    int profundidadMax;    // Profundidad máxima de búsqueda
    double tiempoMaxSegundos; // Límite de tiempo por movimiento
    int numHeuristica;     // Selector de la heurística a usar
    ModoJuego modo;        // Modo de operación del agente

    std::atomic<bool> abortarBanda; // Flag para interrumpir la búsqueda
    std::pair<int, int> mejorMovimientoH; // Mejor movimiento encontrado en la última profundidad completada
    
    std::chrono::time_point<std::chrono::steady_clock> inicioBusqueda;
    long long nodosVisitados; // Contador para benchmarking
    

    /**
     * @brief Funciones heurísticas para evaluar un estado del tablero. 
     * La que se llama "heuristica" llama al resto. 
     * En el valor 0 esta heuristicaPrueba que no se puede modificar
     * El usuario puede definir tantas como quiera y asignarle un número
     * IMPORTANTE: en la evaluación final se probarará con las que esté bajo -id 1
     * @param tablero Estado a evaluar.
     * @return Valor numérico que representa la ventaja para este agente.
     */
    double heuristica(const Tablero& tablero);

    double heuristicaPrueba(const Tablero& tablero);
    double heuristica1(const Tablero& tablero);
    double heuristica2(const Tablero& tablero);


};

#endif // AGENTE_ESTUDIANTE_HPP
