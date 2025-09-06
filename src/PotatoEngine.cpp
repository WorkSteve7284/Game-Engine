#include <string>
#include <format>

import PotatoEngine.Core;
import PotatoEngine.Initialize;

using namespace PotatoEngine::Core;

int main() {
	Manager manager;

	PotatoEngine::Initialize(&manager);

	manager.start();

	manager.run();

	Debug::Debug.log(std::format("Ran for {} seconds", Time::runtime));
	Debug::Debug.log(std::format("Updated {} times", Time::updates));
	Debug::Debug.log(std::format("Fixed Updated {} times", Time::fixed_updates));

}