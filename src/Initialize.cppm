export module PotatoEngine.Initialize;

import PotatoEngine.Core;

import Stop_After_Time;
import Event_Broadcaster;
import Event_Reciever;
import Move_At_Speed;

namespace PotatoEngine {
	export void Initialize(Manager* manager) {
		PotatoEngine::Get_Manager = [manager]() { return manager; };

		manager->Add_Object_With_Component_No_Start<Stop_After_Time>(0.5);

		//Object& event_object = manager->Add_Object_With_Component_No_Start<Event_Broadcaster>(15);
		//event_object.Add_Component_With_Type_No_Start<Event_Reciever>();

		manager->Add_Object_With_Component_No_Start<Move_At_Speed>(1);
	}
}