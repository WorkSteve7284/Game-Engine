export module PotatoEngine.Core.Classes.SafePtr;

import std;

import PotatoEngine.Core.Event;
import PotatoEngine.Core.Exception;
import PotatoEngine.Core.Debug;
import PotatoEngine.Core.Classes.ObjectMap;

namespace PotatoEngine::Core::Classes {
	export template <typename T> class SafePtr {
	private:
		T* raw_ptr;
		std::uint_fast64_t uid;
	public:

		SafePtr() : raw_ptr(nullptr), uid(0) {}

		SafePtr(T* new_ptr) : raw_ptr(new_ptr) { if (new_ptr) uid = new_ptr->uid; exists(); }

		SafePtr(std::unique_ptr<T>& new_ptr) : raw_ptr(new_ptr.get()) { if (new_ptr) uid = new_ptr->uid; exists(); }

		SafePtr(const SafePtr& other) : raw_ptr(other.raw_ptr), uid(other.uid) { exists(); }

		SafePtr& operator=(const SafePtr& other) {
			if (this != &other) {
				
				raw_ptr = other.raw_ptr;
				uid = other.uid;
			}
			exists();
			return *this;
		}

		operator T*() {
			exists();
			return raw_ptr;
		}

	public:
		T* get() {
			exists();
			return raw_ptr;
		}

		T* operator->() {
			if (!exists()) {
				throw PotatoEngine::Core::Exception::InvalidPointer("Pointer invalid!");
			}
			else {
				return raw_ptr;
			}
		}

		T& operator*() const {
			if (!exists()) {
				throw PotatoEngine::Core::Exception::InvalidPointer("Pointer invalid!");
			}
			else {
				return *raw_ptr;
			}
		}

		bool exists() {
			if (object_map.exists(uid)) {
				return true;
			}
			else {
				uid = 0;
				raw_ptr = nullptr;
				return false;
			}
		}

		operator bool() { return exists(); }

		friend class SafePtr;
	};
}