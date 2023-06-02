#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

#include "world/world.h"
#include "util/texturedatabase.h"

int main(int argv, char **argc) {
	sf::View view(sf::View(sf::FloatRect(0, 0, 800, 600)));
	sf::RenderWindow window(sf::VideoMode(800, 600), "Game");
	window.setView(view);

	float coordX = 0, coordY = 0;
	sf::Vector2i cursorPos{ 0, 0 };
	bool shiftOn = false;

	World wrld(50, 50);
	wrld.Generate();

	size_t h = 64;
	sf::Font font; font.loadFromFile("data/fonts/Roboto-Regular.ttf");
	sf::Text heightTxt = sf::Text(std::string("Height: ") + std::to_string(h), font, 16);
	sf::Text posTxt = sf::Text("", font, 16);
	posTxt.setPosition(0, window.getSize().y - 20);

	const sf::Texture &cursorTexture = TextureDB::GetTexture("data/textures/tileset.png");
	sf::Sprite cursorSprite = sf::Sprite(cursorTexture, sf::IntRect(160, 0, 32, 32));

	{
		std::stringstream ss;
		ss << "[" << coordX << "][" << coordY << "]-[" <<
			(coordX + window.getSize().x / 32) << "][" << (coordY + window.getSize().y / 32) << "]";
		posTxt.setString(ss.str());
	}

	while (window.isOpen()) {
		sf::Event event;

		while(window.pollEvent(event)){
			if (event.type == sf::Event::Closed) {
				window.close();
				break;
			}
			else if (event.type == sf::Event::Resized) {
				sf::Vector2f size = static_cast<sf::Vector2f>(window.getSize());

				// Minimum size
				if (size.x < 800)
					size.x = 800;
				if (size.y < 600)
					size.y = 600;

				view.setCenter(size / 2.0f);
				view.setSize(size);

				window.setView(view);

				// Apply possible size changes
				window.setSize(static_cast<sf::Vector2u>(size));

				posTxt.setPosition(0, window.getSize().y - 20);
			}
			else if (event.type == sf::Event::KeyPressed) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) shiftOn = true;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && h < 255) {
					h++;
					heightTxt.setString(std::string("Height: ") + std::to_string(h));
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && h > 0) {
					h--;
					heightTxt.setString(std::string("Height: ") + std::to_string(h));
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
					cursorPos.x -= (shiftOn ? 16 : 1);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
					cursorPos.x += (shiftOn ? 16 : 1);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
					cursorPos.y -= (shiftOn ? 16 : 1);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
					cursorPos.y += (shiftOn ? 16 : 1);
				}

				if (((float)cursorPos.x > window.getSize().x / 32 - 1) && (coordX < wrld.GetDims().x * 16)) {
					if (shiftOn && ((coordX + 16) < (wrld.GetDims().x * 16))) coordX += 16;
					else coordX++;
				}
				if (cursorPos.x < 0 && coordX > 0) {
					if (shiftOn && ((coordX - 16) < 0)) coordX = 0;
					else coordX--;
				}

				if (((float)cursorPos.y > window.getSize().y / 32 - 1) && (coordY < wrld.GetDims().y * 16)) {
					if (shiftOn && ((coordY + 16) < (wrld.GetDims().y * 16))) coordY += 16;
					else coordY++;

					std::cout << cursorPos.y << "\n";
				}
				if (cursorPos.y < 0 && coordY > 0) {
					if (shiftOn) coordY = (((coordY - 16) > 0) ? coordY - 16 : 0);
					else coordY--;
				}

				cursorPos.x = std::clamp<int>(cursorPos.x, 0, window.getSize().x / 32 - 1);
				cursorPos.y = std::clamp<int>(cursorPos.y, 0, window.getSize().y / 32 - 1);

				std::stringstream ss;
				ss << "[" << coordX << "][" << coordY << "]-[" <<
					(coordX + window.getSize().x) << "][" << (coordY + window.getSize().y) << "]";
				if(posTxt.getString() != ss.str())
					posTxt.setString(ss.str());
			}
			else if (event.type == sf::Event::KeyReleased) {
				if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) shiftOn = false;
			}
		}

		window.clear();
		wrld.Draw(window, h, Vector2(coordX, coordY), Vector2(coordX + window.getSize().x, coordY + window.getSize().y));
		window.draw(cursorSprite);
		window.draw(heightTxt);
		window.draw(posTxt);
		window.display();
	}
	if (!window.isOpen()) window.close();

	//wrld.Save();

	return 0;
}