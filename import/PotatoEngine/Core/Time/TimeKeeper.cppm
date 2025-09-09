module;

#include <map>
#include <string>
#include <vector>
#include <functional>

export module PotatoEngine.Core.Time.TimeKeeper;

import PotatoEngine.Core.Classes;
import PotatoEngine.Core.Time.TimeVariables;

namespace PotatoEngine::Core::Time {
	
	export class TimeKeeper : public PotatoEngine::Core::Classes::Component<PotatoEngine::Core::Classes::Object> {
	private:
		std::map<double, std::vector<std::function<void(void)>>> listeners_map;
	public:
		template <typename I>
		void schedule_event(double, I*, void(I::*)(void));
	public:
		void update(double delta_time) override {
			for (auto event = listeners_map.begin(); event != listeners_map.end(); ) {
				if (event->first <= runtime) {
					for (auto& listener : event->second) {
						listener();
					}
					event = listeners_map.erase(event);
				}
				else {
					break;
				}
			}
		}
	};

	template <typename I>
	void TimeKeeper::schedule_event(double delay, I* instance, void(I::* target_func)(void)) {
		listeners_map[runtime + delay].emplace_back(
			[instance, target_func]() { if (instance) (instance->*target_func)(); });
	}

	export SafePtr<TimeKeeper> Scheduler;
}