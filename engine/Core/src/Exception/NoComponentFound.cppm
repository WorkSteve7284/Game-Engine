export module PotatoEngine.Core.Exception:NoComponentFound;

import std;

namespace PotatoEngine::Core::Exception {
	export class NoComponentFound : std::exception {
	private:
		std::string message;
	public:
		NoComponentFound(std::string msg) : message(msg) {};

		const char* what() const noexcept override {
			return message.c_str();
		}
	};
}