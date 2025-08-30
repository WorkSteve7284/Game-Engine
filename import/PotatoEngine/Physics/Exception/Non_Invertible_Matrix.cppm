module;

#include <exception>
#include <string>

export module PotatoEngine.Physics.Exception:Non_Invertible_Matrix;

namespace PotatoEngine::Physics::Exception {
	export class Non_Invertible_Matrix : std::exception {
	private:
		std::string message;
	public:
		Non_Invertible_Matrix(std::string msg) : message(msg) {};

		const char* what() noexcept {
			return message.c_str();
		}
	};
}