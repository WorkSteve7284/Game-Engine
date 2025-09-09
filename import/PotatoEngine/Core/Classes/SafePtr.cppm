export module PotatoEngine.Core.Classes.SafePtr;

import PotatoEngine.Core.Event;
import PotatoEngine.Core.Exception;

export template <typename T> class SafePtr {
private:
	T* raw_ptr;

	bool valid = false;
public:

	SafePtr() : raw_ptr(nullptr), valid(false) {}

	SafePtr(T* new_ptr) : raw_ptr(new_ptr), valid(true) {
		PotatoEngine::Core::Events.subscribe("Object Destroyed", this, &SafePtr::object_destroyed);
	};

	SafePtr(const SafePtr& other) : raw_ptr(other.raw_ptr), valid(true) {
		PotatoEngine::Core::Events.subscribe("Object Destroyed", this, &SafePtr::object_destroyed);
	}

	SafePtr& operator=(const SafePtr& other) {
		if (this != &other) {
			if (other) {
				valid = true;
				raw_ptr = other.raw_ptr;
				PotatoEngine::Core::Events.subscribe("Object Destroyed", this, &SafePtr::object_destroyed);
			}
			else {
				valid = false;
				raw_ptr = nullptr;
			}
		}
		return *this;
	}

public:
	void object_destroyed(T* other_ptr) {
		if (other_ptr == raw_ptr) {
			valid = false;
			raw_ptr = nullptr;
		}
	}

public:
	T* operator->() const {
		if (!valid) {
			throw PotatoEngine::Core::Exception::InvalidPointer("Pointer invalid!");
		}
		else {
			return raw_ptr;
		}
	}

	T& operator*() const {
		if (!valid) {
			throw PotatoEngine::Core::Exception::InvalidPointer("Pointer invalid!");
		}
		else {
			return *raw_ptr;
		}
	}

	operator bool() const { return valid; }

	friend class SafePtr;
};