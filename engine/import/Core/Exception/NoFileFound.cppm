export module PotatoEngine.Core.Exception:NoFileFound;

import std;

namespace PotatoEngine::Core::Exception {
	export class NoFileFound : std::exception {
	private:
		std::string message;
	public:
		NoFileFound(std::string msg) : message(msg) {};

		const char* what() const noexcept override {
			return message.c_str();
		}
	};
}