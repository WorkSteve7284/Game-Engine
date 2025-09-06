export module PotatoEngine.Initialize;

import PotatoEngine.Core;
//import PotatoEngine.Physics;

import Stop_After_Time;
//import Event_Broadcaster;
//import Event_Reciever;
//import Move_At_Speed;

namespace PotatoEngine {
	export void Initialize(Manager* manager) {
		PotatoEngine::Core::manager = manager;

		Object* controller = manager->add_object_ns();
		
		Core::Time::Scheduler = controller->add_component_ns<Core::Time::TimeKeeper>();

		manager->add_object_with_component_ns<Stop_After_Time>(0.1);

	}
}