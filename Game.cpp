#include "Game.h"
Game::Game() : m_window("Game Project", sf::Vector2u(800, 600)), m_stateManager(&m_context) { //il costruttore prende "m_window" e "m_stateManager"
	m_clock.restart(); //metodo di m_clock
	srand(time(nullptr)); //inizializza un numero randomico passato come seed, è inizializzato con il valore di ritorno della funzione time 

	m_context.m_wind = &m_window; //m_wind è un attributo di m_context 
	m_context.m_eventManager = m_window.GetEventManager(); //chiama GetEventManager di window

	m_stateManager.SwitchTo(StateType::Intro);//metodo di m_stateManager a cui è passato "StateType::Intro"
}

Game::~Game() { }

sf::Time Game::GetElapsed() {
	return m_clock.getElapsedTime();
}
void Game::RestartClock() { 
	m_elapsed = m_clock.restart(); 
}
Window* Game::GetWindow() {
	return &m_window;
}

void Game::Update() {
	m_window.Update();
	m_stateManager.Update(m_elapsed);
}

void Game::Render() {
	m_window.BeginDraw(); //inizia a disegnare
	m_stateManager.Draw();
	m_window.EndDraw();
}

void Game::LateUpdate() {
	m_stateManager.ProcessRequests(); //tiene traccia degli StateType che vogliamo rimuovere e li rimuove quando non vengono più usati
	RestartClock();
}