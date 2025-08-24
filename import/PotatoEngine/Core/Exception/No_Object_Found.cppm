module;

#include <exception>
#include <string>

export module PotatoEngine.Core.Exception:No_Object_Found;

namespace PotatoEngine::Core::Exception {
	export class No_Object_Found : std::exception {
	private:
		std::string message;
	public:
		No_Object_Found(std::string msg) : message(msg) {};

		const char* what() noexcept {
			return message.c_str();
		}
	};
}