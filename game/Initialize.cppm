export module PotatoEngine.Initialize;

import std;

import PotatoEngine.Core;
import PotatoEngine.Core.Init.CreateObject;
using namespace PotatoEngine::Core;

namespace PotatoEngine {
	export void Initialize(Manager* manager) {
		::manager = manager;

		SafePtr<Object> controller = manager->add_object_ns();

		Time::Scheduler = controller->add_component_ns<Core::Time::TimeKeeper>();

		Init::YAML yaml = Init::parse_yaml_file("prefabs/test.yaml");
		Init::parse_objects_yaml(yaml);

		Init::initialize_object(manager, Init::prefabs["Object1"]);
	}
}