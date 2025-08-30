#include <chrono>

import PotatoEngine.Core;
import PotatoEngine.Initialize;

using namespace PotatoEngine::Core;

int main() {
	Manager manager;

	PotatoEngine::Initialize(&manager);

	manager.Start();

	manager.Run();

	Debug::Debug.Log("Stopping after " + std::to_string(Time::runtime) + " seconds");
	Debug::Debug.Log("Updated " + std::to_string(Time::updates) + " times");
	Debug::Debug.Log("Fixed Updated " + std::to_string(Time::fixed_updates) + " times");

}