#include "Game.h"

void Game::MoveSprite(EventDetails* lDetails) {
	sf::Vector2i mousepos = window.GetEventManager()->GetMousePos(window.GetRenderWindow());
	sprite.setPosition(mousepos.x, mousepos.y);
	std::cout << "Moving sprite to: " << mousepos.x << ":" << mousepos.y << std::endl;
}

Game::Game() : window("Chapter 4", sf::Vector2u(800, 600)) {
	Clock.restart();
	srand(time(nullptr));

	texture.loadFromFile("Mushroom.png");
	sprite.setTexture(texture);
	sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
	sprite.setPosition(0, 0);

	window.GetEventManager()->AddCallback("Move", &Game::MoveSprite, this);
}

Game::~Game() {}

sf::Time Game::GetElapsed() { return Clock.getElapsedTime(); }
void Game::RestartClock() { Clock.restart(); }
Window* Game::GetWindow() { return &window; }

void Game::Update() {
	window.Update();
}

void Game::Render() {
	window.BeginDraw();
	// Render here.
	window.GetRenderWindow()->draw(sprite);
	window.EndDraw();
}