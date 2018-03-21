#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <functional>
#include <fstream>
#include <sstream>
#include <iostream>


enum class EventType {                                                                  //enumeriamo i tipi di eventi possibili

	KeyDown = sf::Event::KeyPressed,
	KeyUp = sf::Event::KeyPressed,
	WindowResized = sf::Event::Resized,
	GainedFocus = sf::Event::GainedFocus,
	LostFocus = sf::Event::LostFocus,
	Closed = sf::Event::Closed,
	TextEntered = sf::Event::TextEntered,
	Keyboard = sf::Event::Count + 1                                                    //essendo gli enumeratori valori interi, l'ulitma riga previene 
										                                               //ogni possibile "scontro" tra interi e impedisce che sia aggiunto 
										                                               //qualcosa di più grande di valore rispetto a EventType
};

struct EventInfo {                                                                     //struttura che associa un valore ad un tasto ed alla sua tipologia
	 
	EventInfo() {                                                                      //associa ad ogni tipologia di evento il numero corrispettivo a code, 0 se default
		code = 0; 
	}
	EventInfo(int lEvent) {
		code = lEvent;
	}
	union {                                                                            //usiamo union per salvare il codice dell'evento 
		int code;
	};

};

struct EventDetails {                                                                  //ci aiuta a condividere le informazioni dell'evento con il codice che è usato in questa classe
	EventDetails(const std::string& lBindName)
		: name(lBindName) {
		Clear();
	}
	std::string name;
	sf::Vector2i size;
	sf::Uint32 textEntered;
	int keyCode;

	void Clear() {                                                                     //pulisce i valori di size, testo e tasto assegnati agli eventi
		size = sf::Vector2i(0, 0);
		textEntered = 0;
		keyCode = -1;
	}
};

using Events = std::vector<std::pair<EventType, EventInfo>>;                           //è un nuovo data type che ci permette di legare le informazioni degli eventi 

                                                                                      
struct Binding {
	Binding(const std::string& lName) : name(lName), details(lName), c(0) {}           //il costruttore prende il nome dell'azione che vogliamo legare all'evento 
																					   //e usa la lista di inizializzazione per impostare i membri della classe
	~Binding() {}

	void BindEvent(EventType lType, EventInfo lInfo = EventInfo()) {                   //metodo che unisce un tipo di evento a una struttura con l'info dell'evento
																					   //per aggiungerlo al vettore degli eventi
		events.emplace_back(lType, lInfo);
	}
	Events events;
	std::string name;
	int c;                                                                             //contatore degli eventi che stanno accadendo

	EventDetails details;
};

using Bindings = std::unordered_map<std::string, Binding*>;                            //garantisce che ci sia soltanto un binding per azione, in quanto abbiamo 
                                                                                       //un contenitore associativo con i nomi delle azioni legate al tasto che li coinvolge. 
                                                                                       //Usiamo unordered_map per associare ad ogni azione un solo tasto.
using CallbackContainer = std::unordered_map < std::string, std::function<void(EventDetails*) >>;   /*std::function è un "involucro" di funzioni polimorfico a generico scopo, 
																									può involucrare ogni tipo di elemento "chiamabile"*/
using Callbacks = std::unordered_map <std::string, std::function<void(EventDetails*)>>;

class EventManager {
public:                                                                                
	EventManager();
	~EventManager();

	bool AddBinding(Binding* lBinding);                                              
	bool RemoveBinding(std::string lName);                                           

	void SetFocus(const bool& lFocus);

	template <class T>                                                       
	bool AddCallback(const std::string lName, void(T::*lFunc)(EventDetails*), T* lIstance) 
	{

		auto temp = std::bind(lFunc, lIstance, std::placeholders::_1);       /*il bind, con l'aiuto del placeholder, aiuta a monitorare la 
																			 posizione e il numero di valori che sono usati dalla funzione.
																			 I placeholder sono uno spazio di nomi che direziona la posizione
																			 di un valore in una funzione. */
		return callbacks.emplace(lName, temp).second;
	}

	void RemoveCallback(const std::string& lName) {
		callbacks.erase(lName);
	}

	void HandleEvent(sf::Event& lEvent);
	void Update();

	
private:
	void LoadBindings();

	Bindings bindings;
	Callbacks callbacks;
	bool hasFocus;
};








