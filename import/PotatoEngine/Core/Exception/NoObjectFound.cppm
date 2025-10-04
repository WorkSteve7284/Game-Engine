export module PotatoEngine.Core.Exception:NoObjectFound;

import std;

namespace PotatoEngine::Core::Exception {
	export class NoObjectFound : std::exception {
	private:
		std::string message;
	public:
		NoObjectFound(std::string msg) : message(msg) {};

		const char* what() noexcept {
			return message.c_str();
		}
	};
}