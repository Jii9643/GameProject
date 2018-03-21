#pragma once
#include "Window.h"
#include "EventManager.h"
#include "StateManager.h"
#include "SharedContext.h"
#include <iostream>

class Game {
public:
	Game();
	~Game();

	void Update();
	void Render();
	void LateUpdate();

	sf::Time GetElapsed(); //misura il tempo di esecuzione dell'ultima istanza, reset del clock o avvio

	Window* GetWindow(); //prende window
private:
	SharedContext m_context;
	Window m_window;
	StateManager m_stateManager;  //notazione ungara, "m_" sta per membro di una struttura/classe
	sf::Clock m_clock;
	sf::Time m_elapsed;
	void RestartClock();
};