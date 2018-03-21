#include "EventManager.h"

EventManager::EventManager() : hasFocus(true) { LoadBindings(); }        /*chiama il metodo LoadBindings che è usato per caricare
																		 le informazioni sui nostri legami da un file. */

EventManager::~EventManager() {
	for (auto &itr : bindings) {
		delete itr.second;
		itr.second = nullptr;
	}
}

bool EventManager::AddBinding(Binding *l_binding) {                       /*prende in ingresso un puntatore a Binding. Controlla se 
																		  il Binding ha già un legame con lo stesso nome: se sì, il 
																		  metodo ritorna false, altrimenti inserisce il nuovo legame 
																		  nel container.*/
	if (bindings.find(l_binding->name) != bindings.end())
		return false;

	return bindings.emplace(l_binding->name, l_binding).second;
}

bool EventManager::RemoveBinding(std::string lName) {                      /*prende come argomento una stringa e cerca in Binding un 
																		   riscontro per salvarlo in un'iteratore. Se trova un riscontro, 
																		   prima elimina il secondo elemento nella coppia key-value, che 
																		   è la memoria dinamicamente allocata per l'oggetto legato,
																		   e poi elimina l'entrata dal container per poi ritornare true.*/
	auto itr = bindings.find(lName);
	if (itr == bindings.end()) { return false; }
	delete itr->second;
	bindings.erase(itr);
	return true;
}

void EventManager::SetFocus(const bool& lFocus) { hasFocus = lFocus; }

void EventManager::HandleEvent(sf::Event& lEvent) {                      /*processa gli eventi SFML che sono trascinati in ogni iterazione
                                                                          per "guardarli" e vedere se ci fosse qualcosa in cui siamo interessati.*/
	
	for (auto &b_itr : bindings) {
		Binding* bind = b_itr.second;                                    /* Serve per iterare su tutti i Bindings e dentro ogni evento dentro il legame
																		 per controllare se il tipo dell'argomento coincide col tipo dell'evento binding 
																		 che è attualmente in atto. */
		for (auto &e_itr : bind->events) {
			EventType sfmlEvent = (EventType)lEvent.type;                /*crea la variabile che contiene il tipo dell'evento passato
																         come argomento*/
			if (e_itr.first != sfmlEvent) { continue; }
			if (sfmlEvent == EventType::KeyDown || sfmlEvent == EventType::KeyUp) {
				if (e_itr.second.code == lEvent.key.code) {
					// leghiamo l'evento alla pressione del pulsante.
					// Incremento count.
					if (bind->details.keyCode != -1) {
						bind->details.keyCode = e_itr.second.code;
					}
					++(bind->c);
					break;
				}
			}
			else {
			
				if (sfmlEvent == EventType::WindowResized) {
					bind->details.size.x = lEvent.size.width;
					bind->details.size.y = lEvent.size.height;
				}
				else if (sfmlEvent == EventType::TextEntered) {
					bind->details.textEntered = lEvent.text.unicode;
				}
				++(bind->c);
			}
		}
	}

}

void EventManager::Update() {
	if (!hasFocus) { return; }
	for (auto &b_itr : bindings) {
		Binding* bind = b_itr.second;
		for (auto &e_itr : bind->events) {
			switch (e_itr.first) {
			case(EventType::Keyboard):
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(e_itr.second.code))) {
					if (bind->details.keyCode != -1) {
						bind->details.keyCode = e_itr.second.code;
					}
					++(bind->c);
				}
				break;
			}
		}

		if (bind->events.size() == bind->c) {                           /*controlliamo se il numero di eventi nel container corrisponde col numero di 
																		eventi che sono in funzione. Se sì, localiziamo il nostro callback container
																		e invochiamo il secondo membro, ed essendo un wrapper noi implementiamo anche 
																		ufficialmente il callback del secondo membro. */
			auto callItr = callbacks.find(bind->name);
			if (callItr != callbacks.end()) {
				callItr->second(&bind->details);
			}
		}
		bind->c = 0;
		bind->details.Clear();
	}
}