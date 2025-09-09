module;

#include <exception>
#include <string>

export module PotatoEngine.Core.Exception:InvalidPointer;

namespace PotatoEngine::Core::Exception {
	export class InvalidPointer : std::exception {
	private:
		std::string message;
	public:
		InvalidPointer(std::string msg) : message(msg) {};

		const char* what() noexcept {
			return message.c_str();
		}
	};
}