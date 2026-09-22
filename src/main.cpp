#include <SFML/Graphics.hpp>
#include <iostream>
#include "lib/queue.h"
#include "core/tetromino.h"
#include "core/bag.h"


void drawCell(sf::RenderWindow& window, float pixelX, float pixelY, float size, sf::Color color, sf::Color outlineColor = sf::Color(40, 40, 40)) {
    sf::RectangleShape cell({size - 2.f, size - 2.f});
    cell.setPosition({pixelX + 1.f, pixelY + 1.f});
    cell.setFillColor(color);
    cell.setOutlineThickness(1.f);
    cell.setOutlineColor(outlineColor);
    window.draw(cell);
}


void drawPiecePreview(sf::RenderWindow& window, PieceType pType, float startX, float startY, float cellSize) {
    if (pType == PieceType::NONE) return;

    BlockPos blocks[4];
    Tetromino::getBlocksFor(pType, 0, blocks);
    sf::Color color = Tetromino::getColorFor(pType);

    for (int i = 0; i < 4; ++i) {
        float px = startX + blocks[i].col * cellSize;
        float py = startY + blocks[i].row * cellSize;
        drawCell(window, px, py, cellSize, color, sf::Color::White);
    }
}

int main() {
    std::cout << "===============================================" << std::endl;
    std::cout << "   TETRIS (EIF207) - FASE 1: PIEZAS Y COLA     " << std::endl;
    std::cout << "===============================================" << std::endl;
    std::cout << "Mecánicas evaluadas activas:" << std::endl;
    std::cout << "  - Cola propia (FIFO): Generador 7-Bag" << std::endl;
    std::cout << "  - 7 Tetrominos con sus 4 orientaciones fijas" << std::endl;
    std::cout << "  - Panel con las siguientes 3 piezas en espera" << std::endl;
    std::cout << "Controles:" << std::endl;
    std::cout << "  [FLECHAS / A-D]: Mover izquierda / derecha" << std::endl;
    std::cout << "  [ARRIBA / W]   : Rotar (orientaciones 0 a 3)" << std::endl;
    std::cout << "  [ABAJO / S]    : Bajar rapido" << std::endl;
    std::cout << "  [ESPACIO]      : Fijar y pedir siguiente de la cola" << std::endl;
    std::cout << "  [ESC]          : Salir" << std::endl;
    std::cout << "===============================================" << std::endl;

    // Ventana principal en SFML 3 (800x650)
    sf::RenderWindow window(sf::VideoMode({800, 650}), "Tetris - Fase 1: Cola 7-Bag y Piezas");
    window.setFramerateLimit(60);

    // 1. Inicializar la Cola de Piezas Futuras (Sección 3.1 del PDF)
    Queue<PieceType> pieceQueue;
    PieceBag::refillBag(pieceQueue); // Primera bolsa de 7
    PieceBag::refillBag(pieceQueue); // Segunda bolsa de respaldo

    // 2. Extraer la primera pieza activa
    Tetromino currentPiece = PieceBag::getNext(pieceQueue);
    std::cout << "[INICIO]: Pieza activa generada. Piezas en cola: " 
              << PieceBag::countQueue(pieceQueue) << std::endl;

    // Configuración del tablero visual (10 columnas x 20 filas)
    const int COLS = 10;
    const int ROWS = 20;
    const float CELL_SIZE = 26.f;
    const float BOARD_X = 220.f;
    const float BOARD_Y = 50.f;

    // Reloj para caída automática (gravedad básica de prueba)
    sf::Clock dropClock;
    float dropInterval = 0.6f; // Cae cada 0.6 segundos

    while (window.isOpen()) {
        // Manejo de eventos con la API de SFML 3
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    window.close();
                } else if (keyPressed->code == sf::Keyboard::Key::Left || keyPressed->code == sf::Keyboard::Key::A) {
                    // Mover a la izquierda (con límite básico de pantalla)
                    if (currentPiece.col > 0) currentPiece.col--;
                } else if (keyPressed->code == sf::Keyboard::Key::Right || keyPressed->code == sf::Keyboard::Key::D) {
                    // Mover a la derecha (con límite básico)
                    if (currentPiece.col < COLS - 3) currentPiece.col++;
                } else if (keyPressed->code == sf::Keyboard::Key::Up || keyPressed->code == sf::Keyboard::Key::W) {
                    // Rotar pieza (cicla 0 -> 1 -> 2 -> 3 -> 0)
                    currentPiece.rotation = (currentPiece.rotation + 1) % 4;
                    std::cout << "-> Rotacion: " << currentPiece.rotation << std::endl;
                } else if (keyPressed->code == sf::Keyboard::Key::Down || keyPressed->code == sf::Keyboard::Key::S) {
                    // Caída suave acelerada
                    if (currentPiece.row < ROWS - 4) currentPiece.row++;
                } else if (keyPressed->code == sf::Keyboard::Key::Space) {
                    // Simular fijar y pedir la siguiente pieza de la cola (FIFO)
                    currentPiece = PieceBag::getNext(pieceQueue);
                    std::cout << "-> [ESPACIO]: Siguiente pieza extraida de la cola. Restantes: " 
                              << PieceBag::countQueue(pieceQueue) << std::endl;
                }
            }
        }

        // Caída por gravedad automática de prueba
        if (dropClock.getElapsedTime().asSeconds() >= dropInterval) {
            dropClock.restart();
            if (currentPiece.row < ROWS - 4) {
                currentPiece.row++;
            } else {
                // Al llegar al fondo, pide la siguiente pieza de la cola
                currentPiece = PieceBag::getNext(pieceQueue);
                std::cout << "-> [FONDO]: Siguiente pieza de la cola. Restantes: " 
                          << PieceBag::countQueue(pieceQueue) << std::endl;
            }
        }

        // ================= RENDERIZADO =================
        window.clear(sf::Color(18, 20, 24)); // Fondo oscuro

        // 1. Dibujar fondo y cuadrícula del tablero (10x20)
        sf::RectangleShape boardBg({COLS * CELL_SIZE, ROWS * CELL_SIZE});
        boardBg.setPosition({BOARD_X, BOARD_Y});
        boardBg.setFillColor(sf::Color(10, 12, 16));
        boardBg.setOutlineThickness(3.f);
        boardBg.setOutlineColor(sf::Color(80, 85, 100));
        window.draw(boardBg);

        // Cuadrícula sutil
        for (int r = 0; r < ROWS; ++r) {
            for (int c = 0; c < COLS; ++c) {
                drawCell(window, BOARD_X + c * CELL_SIZE, BOARD_Y + r * CELL_SIZE, CELL_SIZE, sf::Color(15, 18, 22), sf::Color(30, 34, 42));
            }
        }

        // 2. Dibujar la pieza activa actual
        BlockPos blocks[4];
        currentPiece.getBlocks(blocks);
        for (int i = 0; i < 4; ++i) {
            float px = BOARD_X + (currentPiece.col + blocks[i].col) * CELL_SIZE;
            float py = BOARD_Y + (currentPiece.row + blocks[i].row) * CELL_SIZE;
            drawCell(window, px, py, CELL_SIZE, currentPiece.color, sf::Color::White);
        }

        // 3. Dibujar el panel lateral "SIGUIENTES PIEZAS (COLA FIFO)"
        const float PANEL_X = 530.f;
        const float PANEL_Y = BOARD_Y;
        const float PANEL_W = 180.f;
        const float PANEL_H = 340.f;

        sf::RectangleShape nextPanel({PANEL_W, PANEL_H});
        nextPanel.setPosition({PANEL_X, PANEL_Y});
        nextPanel.setFillColor(sf::Color(25, 28, 36));
        nextPanel.setOutlineThickness(2.f);
        nextPanel.setOutlineColor(sf::Color(100, 110, 130));
        window.draw(nextPanel);

        // Obtener las 3 siguientes de la cola sin eliminarlas (PEEK)
        PieceType nextThree[3];
        PieceBag::peekNextThree(pieceQueue, nextThree);

        // Dibujar las 3 piezas en el panel
        for (int i = 0; i < 3; ++i) {
            float previewX = PANEL_X + 45.f;
            float previewY = PANEL_Y + 30.f + i * 95.f;

            // Recuadro para cada pieza
            sf::RectangleShape slot({130.f, 75.f});
            slot.setPosition({PANEL_X + 25.f, PANEL_Y + 20.f + i * 95.f});
            slot.setFillColor(sf::Color(18, 20, 26));
            slot.setOutlineThickness(1.f);
            slot.setOutlineColor(sf::Color(60, 65, 80));
            window.draw(slot);

            drawPiecePreview(window, nextThree[i], previewX, previewY, 20.f);
        }

        // 4. Dibujar panel de estado informativo (HOLD previsto)
        const float HOLD_X = 40.f;
        const float HOLD_Y = BOARD_Y;
        sf::RectangleShape holdPanel({140.f, 150.f});
        holdPanel.setPosition({HOLD_X, HOLD_Y});
        holdPanel.setFillColor(sf::Color(25, 28, 36));
        holdPanel.setOutlineThickness(2.f);
        holdPanel.setOutlineColor(sf::Color(100, 110, 130));
        window.draw(holdPanel);

        window.display();
    }

    return 0;
}
