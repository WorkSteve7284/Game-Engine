module;

#include <string>

export module Event_Broadcaster;

import PotatoEngine.Core;

export class Event_Broadcaster : public Component {
public:

	int value = 0;

	Event_Broadcaster(int val) : value(val) {}

	void Fixed_Update(double delta_time) override {
		PotatoEngine::Core::Events.Broadcast("My Event", value);
		PotatoEngine::Core::Debug::Debug.Log("Broadcasted Event with value " + std::to_string(value));

		Get_Object()->Events.Broadcast("Another Event!", value + 2);
		PotatoEngine::Core::Debug::Debug.Log("Broadcasted Local Event with value " + std::to_string(value + 2));
	}
};