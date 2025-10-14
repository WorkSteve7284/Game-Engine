export module PotatoEngine.Rendering.Exception:WindowError;

import std;

namespace PotatoEngine::Rendering::Exception {
	export class WindowError : std::exception {
	private:
		std::string message;
	public:
		WindowError(std::string msg) : message(msg) {};

		const char* what() const noexcept override {
			return message.c_str();
		}
	};
}