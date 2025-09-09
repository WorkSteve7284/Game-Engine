module;

#include <exception>
#include <string>

export module PotatoEngine.Core.Exception:NoFileFound;

namespace PotatoEngine::Core::Exception {
	export class NoFileFound : std::exception {
	private:
		std::string message;
	public:
		NoFileFound(std::string msg) : message(msg) {};

		const char* what() noexcept {
			return message.c_str();
		}
	};
}