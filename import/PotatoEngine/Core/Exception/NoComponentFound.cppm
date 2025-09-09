module;

#include <exception>
#include <string>

export module PotatoEngine.Core.Exception:NoComponentFound;

namespace PotatoEngine::Core::Exception {
	export class NoComponentFound : std::exception {
	private:
		std::string message;
	public:
		NoComponentFound(std::string msg) : message(msg) {};

		const char* what() noexcept {
			return message.c_str();
		}
	};
}