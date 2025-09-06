module;

#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

export module PotatoEngine.Core.Event;

namespace PotatoEngine::Core::Event {
	export class EventManager {
	public:
		template <typename... Args>
		void broadcast(const std::string& event, Args... args) {
			auto& listeners = get_listeners<Args...>(event);

			for (auto& listener : listeners) {
				listener(args...);
			}
		}

		// Member functions
		template <typename I, typename... Args>
		void subscribe(const std::string& event, I* instance, void(I::* target_function)(Args...)) {
			get_listeners<Args...>(event).push_back([instance, target_function](Args... args) { if (instance) (instance->*target_function)(args...); });
		}

	private:
		template <typename... Args>
		std::vector<std::function<void(Args...)>>& get_listeners(const std::string& event) {
			static std::unordered_map<std::string, std::vector<std::function<void(Args...)>>> listeners_map;
			return listeners_map[event];
		}
	};
}
namespace PotatoEngine::Core {
	export Event::EventManager Events;
}