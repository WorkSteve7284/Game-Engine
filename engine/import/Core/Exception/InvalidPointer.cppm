export module PotatoEngine.Core.Exception:InvalidPointer;

import std;

namespace PotatoEngine::Core::Exception {
	export class InvalidPointer : std::exception {
	private:
		std::string message;
	public:
		InvalidPointer(std::string msg) : message(msg) {};

		const char* what() const noexcept override {
			return message.c_str();
		}
	};
}