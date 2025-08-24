module;

#include <functional>

export module PotatoEngine.Core.Classes.Get_Manager;

import PotatoEngine.Core.Classes.Manager;

namespace PotatoEngine {
	export std::function<PotatoEngine::Core::Classes::Manager* (void)> Get_Manager = []() { return nullptr; };
}