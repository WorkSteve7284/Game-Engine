export module PotatoEngine.Core.Classes.ObjectMap;

import std;

namespace PotatoEngine::Core::Classes {
	export class ObjectMap {
	private:
		std::unordered_map<std::uint_fast64_t, bool> objects;

	public:
	
		bool exists(std::uint_fast64_t target) {
			return objects.count(target);
		}

		template <typename T>
		void add_object(T* new_object) {
			objects.insert_or_assign(new_object->uid, true);
		}

		template <typename T>
		void remove_object(T* old_object) {
			objects.erase(old_object->uid);
		}
	};
}
namespace PotatoEngine::Core {
	export Classes::ObjectMap object_map;
}