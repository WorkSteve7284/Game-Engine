module;

#include <exception>
#include <string>

export module PotatoEngine.Core.Exception:NoComponentFound;

namespace PotatoEngine::Core::Exception {
	export class No_Component_Found : std::exception {
	private:
		std::string message;
	public:
		No_Component_Found(std::string msg) : message(msg) {};

		const char* what() noexcept {
			return message.c_str();
		}
	};
}