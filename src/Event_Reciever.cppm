module;

#include <string>

export module Event_Reciever;

import PotatoEngine.Core;

export class Event_Reciever : public Component {
public:

	int value = 0;

	Event_Reciever() = default;

	void Start() override {
		PotatoEngine::Core::Events.Subscribe("My Event", this, &Event_Reciever::My_Event);

		Get_Object()->Events.Subscribe("Another Event!", this, &Event_Reciever::Local_Event);
	}

	void Local_Event(int value) {
		PotatoEngine::Core::Debug::Debug.Log("Recieved local event with value " + std::to_string(value));
	}

	void My_Event(int value) {
		PotatoEngine::Core::Debug::Debug.Log("Recieved Event with value " + std::to_string(value));
	}
	
};