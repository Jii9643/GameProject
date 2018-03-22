#pragma once
#include "Window.h"

class Game {
public:
	Game();
	~Game();

	void HandleInput();
	void Update();
	void Render();

	sf::Time GetElapsed();
	void RestartClock();

	Window* GetWindow();
private:
	Window window;
	sf::Clock clock;
	float elapsed;

};