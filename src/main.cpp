#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "lib/queue.h"
#include "lib/stack.h"

// Estructura para representar un bloque visual en la cola
struct GameBlock {
    int id;
    sf::Color color;
};

int main() {
    std::cout << "==========================================" << std::endl;
    std::cout << "     MedievalBlock - Demo SFML 3          " << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Controles:" << std::endl;
    std::cout << "  [ESPACIO / Click]: Encolar nuevo bloque" << std::endl;
    std::cout << "  [D / RETROCESO]  : Desencolar (quitar frente)" << std::endl;
    std::cout << "  [P]              : Inspeccionar frente (peek)" << std::endl;
    std::cout << "  [ESC]            : Salir" << std::endl;
    std::cout << "==========================================" << std::endl;

    // Crear ventana con la nueva API de SFML 3 (VideoMode con Vector2u)
    sf::RenderWindow window(sf::VideoMode({800, 600}), "MedievalBlock - SFML 3 Demo");
    window.setFramerateLimit(60);

    // Bloque central animado (Medieval Block)
    sf::RectangleShape centerBlock({140.f, 90.f});
    centerBlock.setOrigin({70.f, 45.f});
    centerBlock.setPosition({400.f, 250.f});
    centerBlock.setFillColor(sf::Color(70, 130, 180)); // Steel Blue
    centerBlock.setOutlineThickness(3.f);
    centerBlock.setOutlineColor(sf::Color::White);

    // Cola dinámica usando nuestra biblioteca Queue
    Queue<GameBlock> blockQueue;
    int nextId = 1;

    const std::vector<sf::Color> palette = {
        sf::Color(220, 20, 60),   // Carmesí
        sf::Color(46, 139, 87),   // Verde mar
        sf::Color(255, 165, 0),   // Naranja
        sf::Color(147, 112, 219), // Púrpura
        sf::Color(255, 215, 0),   // Dorado
        sf::Color(30, 144, 255)   // Azul brillante
    };

    // Encolar bloques iniciales para visualizar la fila (FIFO)
    for (int i = 0; i < 4; ++i) {
        enqueue(blockQueue, GameBlock{nextId, palette[(nextId - 1) % palette.size()]});
        std::cout << "[Inicial]: Bloque #" << nextId << " encolado." << std::endl;
        nextId++;
    }

    // Bucle principal del juego
    while (window.isOpen()) {
        // En SFML 3 pollEvent() retorna un std::optional<sf::Event>
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    window.close();
                } else if (keyPressed->code == sf::Keyboard::Key::Space) {
                    // Encolar un nuevo bloque
                    sf::Color color = palette[(nextId - 1) % palette.size()];
                    enqueue(blockQueue, GameBlock{nextId, color});
                    std::cout << "-> [ENQUEUE]: Bloque #" << nextId << " agregado a la cola." << std::endl;
                    nextId++;
                } else if (keyPressed->code == sf::Keyboard::Key::D ||
                           keyPressed->code == sf::Keyboard::Key::Backspace) {
                    // Desencolar bloque del frente
                    GameBlock removed;
                    if (dequeue(blockQueue, removed)) {
                        std::cout << "<- [DEQUEUE]: Bloque #" << removed.id << " salio de la cola." << std::endl;
                    } else {
                        std::cout << "<- [DEQUEUE]: La cola esta vacia!" << std::endl;
                    }
                } else if (keyPressed->code == sf::Keyboard::Key::P) {
                    // Consultar frente sin eliminar
                    GameBlock frontBlock;
                    if (peek(blockQueue, frontBlock)) {
                        std::cout << "[PEEK]: El frente es el bloque #" << frontBlock.id << std::endl;
                    } else {
                        std::cout << "[PEEK]: La cola esta vacia." << std::endl;
                    }
                }
            }

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    sf::Color color = palette[(nextId - 1) % palette.size()];
                    enqueue(blockQueue, GameBlock{nextId, color});
                    std::cout << "-> [CLICK ENQUEUE]: Bloque #" << nextId << " agregado a la cola." << std::endl;
                    nextId++;
                }
            }
        }

        // Rotación continua con sf::degrees() de SFML 3
        centerBlock.rotate(sf::degrees(1.0f));

        // Limpieza de pantalla (fondo oscuro)
        window.clear(sf::Color(20, 24, 30));

        // Dibujar el bloque central rotando
        window.draw(centerBlock);

        // Representación visual de la cola FIFO en la parte inferior
        float startX = 60.f;
        float posY = 480.f;
        float blockWidth = 60.f;
        float blockHeight = 40.f;
        float spacing = 15.f;

        node<GameBlock>* curr = blockQueue.front;
        int index = 0;
        while (curr != nullptr && startX + index * (blockWidth + spacing) < 760.f) {
            sf::RectangleShape shape({blockWidth, blockHeight});
            shape.setPosition({startX + index * (blockWidth + spacing), posY});
            shape.setFillColor(curr->data.color);
            // El frente de la cola se resalta con borde dorado/amarillo
            shape.setOutlineThickness(index == 0 ? 3.f : 1.f);
            shape.setOutlineColor(index == 0 ? sf::Color::Yellow : sf::Color::White);
            window.draw(shape);

            curr = curr->next;
            index++;
        }

        // Mostrar fotograma renderizado
        window.display();
    }

    return 0;
}
