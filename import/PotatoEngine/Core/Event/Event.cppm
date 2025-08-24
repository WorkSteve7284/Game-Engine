module;

#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

export module PotatoEngine.Core.Event;

namespace PotatoEngine::Core {
	export class Event_Manager {
	public:
		template <typename... Args>
		void Broadcast(const std::string& event, Args... args) {
			auto& listeners = Get_Listeners<Args...>(event);

			for (auto& listener : listeners) {
				listener(args...);
			}
		}

		// Member functions
		template <typename I, typename... Args>
		void Subscribe(const std::string& event, I* instance, void(I::*target_function)(Args...)) {
			Get_Listeners<Args...>(event).push_back([instance, target_function](Args... args) { if (instance) (instance->*target_function)(args...); });
		}

		// Non member functions
		template <typename... Args>
		void Subscribe(const std::string& event, void(*target_function)(Args...)) {
			Get_Listeners<Args...>(event).push_back(target_function);
		}

	private:
		template <typename... Args>
		std::vector<std::function<void(Args...)>>& Get_Listeners(const std::string& event) {
			static std::unordered_map<std::string, std::vector<std::function<void(Args...)>>> listeners_map;
			return listeners_map[event];
		}
	};

	export Event_Manager Events;
}