#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "Tablero.hpp"
#include "ControladorJuego.hpp"
#include "AgenteHumanoConsola.hpp"
#include "AgenteAleatorio.hpp"
#include "AgenteEstudiante.hpp"
#include "AgenteNinja.hpp"
#include "GUI.hpp"
#include <ctime>
#include <cstdlib>

void mostrarAyuda() {
    std::cout << "Uso: ./n_en_raya [opciones]" << std::endl;
    std::cout << "Opciones:" << std::endl;
    std::cout << "  -p1 <tipo>  Tipo Jugador 1: humano, aleatorio, minimax, inteligente, status, ninja[1-4] (def: humano)" << std::endl;
    std::cout << "  -p2 <tipo>  Tipo Jugador 2: humano, aleatorio, minimax, inteligente, status, ninja[1-4] (def: humano)" << std::endl;
    std::cout << "  -f <int>    Número de filas (por defecto: 9)" << std::endl;
    std::cout << "  -c <int>    Número de columnas (por defecto: 9)" << std::endl;
    std::cout << "  -n <int>    Número de piezas en raya para ganar (por defecto: 5)" << std::endl;
    std::cout << "  -d <int>    Profundidad de búsqueda de la IA (por defecto: 4)" << std::endl;
    std::cout << "  -t <double> Tiempo límite para la IA en segundos (por defecto: 100.0)" << std::endl;
    std::cout << "  -id1 <int>  ID Heurística Jugador 1 (para inteligente, def: 1)" << std::endl;
    std::cout << "  -id2 <int>  ID Heurística Jugador 2 (para inteligente, def: 1)" << std::endl;
    std::cout << "  -nogui      Ejecuta sin interfaz gráfica" << std::endl;
    std::cout << "  -file <str> Carga el tablero desde un fichero" << std::endl;
    std::cout << "  -h          Muestra esta ayuda" << std::endl;
}

std::shared_ptr<Agente> crearAgente(const std::string& tipo, int id, int profundidad, double tiempoMax, int h = 1) {
    if (tipo == "aleatorio") return std::make_shared<AgenteAleatorio>();
    if (tipo == "minimax") return std::make_shared<AgenteEstudiante>(id, profundidad, tiempoMax, h, AgenteEstudiante::ModoJuego::MINIMAX);
    if (tipo == "inteligente") return std::make_shared<AgenteEstudiante>(id, profundidad, tiempoMax, h, AgenteEstudiante::ModoJuego::INTELIGENTE);
    if (tipo == "status") return std::make_shared<AgenteEstudiante>(id, profundidad, tiempoMax, h, AgenteEstudiante::ModoJuego::STATUS);

    if (tipo == "ninja1") return std::make_shared<AgenteNinja>(id, 1);
    if (tipo == "ninja2") return std::make_shared<AgenteNinja>(id, 2);
    if (tipo == "ninja3") return std::make_shared<AgenteNinja>(id, 3);
    if (tipo == "ninja4") return std::make_shared<AgenteNinja>(id, 4);
    return std::make_shared<AgenteHumanoConsola>(); 
}

int main(int argc, char** argv) {
    int filas = 9;
    int cols = 9;
    int n = 5;
    int profundidad = 4;
    std::string tipoP1 = "humano";
    std::string tipoP2 = "humano";
    double tiempoIA = 100.0;
    int idH1 = 1;
    int idH2 = 1;
    std::string rutaFichero = "";

    bool noGUI = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-f" && i + 1 < argc) filas = std::stoi(argv[++i]);
        else if (arg == "-c" && i + 1 < argc) cols = std::stoi(argv[++i]);
        else if (arg == "-n" && i + 1 < argc) n = std::stoi(argv[++i]);
        else if (arg == "-d" && i + 1 < argc) profundidad = std::stoi(argv[++i]);
        else if (arg == "-p1" && i + 1 < argc) tipoP1 = argv[++i];
        else if (arg == "-p2" && i + 1 < argc) tipoP2 = argv[++i];
        else if (arg == "-t" && i + 1 < argc) tiempoIA = std::stod(argv[++i]);
        else if (arg == "-id1" && i + 1 < argc) idH1 = std::stoi(argv[++i]);
        else if (arg == "-id2" && i + 1 < argc) idH2 = std::stoi(argv[++i]);
        else if (arg == "-file" && i + 1 < argc) rutaFichero = argv[++i];
        else if (arg == "-nogui") noGUI = true;
        else if (arg == "-h") {
            mostrarAyuda();
            return 0;
        }
    }

    Tablero t(filas, cols, n);
    if (!rutaFichero.empty()) {
        t = Tablero::cargarDesdeFichero(rutaFichero);
    }
    
    auto p1 = crearAgente(tipoP1, 1, profundidad, tiempoIA, idH1);
    auto p2 = crearAgente(tipoP2, 2, profundidad, tiempoIA, idH2);
    
    ControladorJuego juego(t, p1, p2, tiempoIA);
    juego.setConfig(idH1, idH2, profundidad);
    
    if (noGUI) {
        while (!juego.esFinDeJuego()) {
            juego.paso();
        }
    } else {
        GUI::inicializar(argc, argv, t.getFilas(), t.getColumnas(), &juego);
        GUI::ejecutar();
    }
    
    return 0;
}
