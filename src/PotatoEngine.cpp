#include <chrono>

import PotatoEngine.Core;
import PotatoEngine.Initialize;

int main() {
	Manager manager;

	PotatoEngine::Initialize(&manager);

	manager.Start();

	manager.Run();

	PotatoEngine::Core::Debug::Debug.Log("Stopping after " + std::to_string(Time::runtime) + " seconds");
}