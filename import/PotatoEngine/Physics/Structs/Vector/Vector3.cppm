module;

#include <cmath>
#include <array>
#include <string>
#include <cstdint>
#include <sstream>

export module PotatoEngine.Physics.Structs.Vector:Vector3;

namespace PotatoEngine::Physics::Structs {

	export template <typename P> struct Vector3 {
	public:
		P x = 0, y = 0, z = 0;

	public: // Constructors
		Vector3() : x(0), y(0), z(0) {};
		// Three numbers
		Vector3(const P new_x, const P new_y, const P new_z) : x(new_x), y(new_y), z(new_z) {}
		// Another Vector3
		template <typename Other_P, typename = std::enable_if_t<std::is_convertible_v<Other_P, P>>>
		Vector3(const Vector3<Other_P>& original) : x(original.x), y(original.y), z(original.z) {}
		// An Array
		Vector3(const std::array<P, 3>& original) : x(original.at(0)), y(original.at(1)), z(original.at(2)) {}

	public: // Member Functions
		P Sqr_Magnitude() const;
		P Magnitude() const;
		Vector3<P> Normalized() const;
		Vector3<P>& Normalize();
		Vector3<P> Conjugate() const;
		std::string String() const;
	public: // Operators
		Vector3<P> operator+ (const Vector3<P>&) const;
		Vector3<P> operator- (const Vector3<P>&) const;
		Vector3<P> operator* (const P&)	const;
		Vector3<P> operator/ (const P&) const;
		Vector3<P>& operator+= (const Vector3<P>&);
		Vector3<P>& operator-= (const Vector3<P>&);
		Vector3<P>& operator*= (const P&);
		Vector3<P>& operator/= (const P&);
		inline P& operator[](std::uint_fast8_t);
	public: // Static Functions
		static P Dot(const Vector3<P>&, const Vector3<P>&);
		static Vector3<P> Cross(const Vector3<P>&, const Vector3<P>&);
		static P Angle(const Vector3<P>&, const Vector3<P>&);
		static Vector3<P> Reflect(const Vector3<P>&, const Vector3<P>&);
		static Vector3<P> Project(const Vector3<P>&, const Vector3<P>&);
	};

	export template <typename P>
	inline Vector3<P> operator*(const P&, const Vector3<P>& );

	template <typename P>
	P Vector3<P>::Sqr_Magnitude() const {
		return (x * x) + (y * y) + (z * z);
	}

	template <typename P>
	P Vector3<P>::Magnitude() const {
		return std::sqrt((x * x) + (y * y) + (z * z));
	}

	template <typename P>
	Vector3<P> Vector3<P>::Normalized() const {
		return (*this) / this->Magnitude();
	}

	template <typename P>
	Vector3<P>& Vector3<P>::Normalize() {
		*this /= this->Magnitude();
		return *this;
	}

	template <typename P>
	Vector3<P> Vector3<P>::Conjugate() const {
		return Vector3<P>(-x, -y, -z);
	}

	template <typename P>
	std::string Vector3<P>::String() const {
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) +")";
	}

	template <typename P>
	Vector3<P> Vector3<P>::operator + (const Vector3<P>& other) const {
		return Vector3<P>(x + other.x, y + other.y, z + other.z);
	}

	template <typename P>
	Vector3<P> Vector3<P>::operator- (const Vector3<P>& other) const {
		return Vector3<P>(x - other.x, y - other.y, z - other.z);
	}

	template <typename P>
	Vector3<P> Vector3<P>::operator* (const P& scalar) const {
		return Vector3<P>(x * scalar, y * scalar, z * scalar);
	}

	template <typename P>
	Vector3<P> Vector3<P>::operator/ (const P& scalar) const {
		return Vector3<P>(x / scalar, y / scalar, z / scalar);
	}

	template <typename P>
	Vector3<P>& Vector3<P>::operator+= (const Vector3<P>& other) {
		*this = *this + other;
		return *this;
	}

	template <typename P>
	Vector3<P>& Vector3<P>::operator-= (const Vector3<P>& other) {
		*this = *this - other;
		return *this;
	}

	template <typename P>
	Vector3<P>& Vector3<P>::operator*= (const P& scalar) {
		*this = *this * scalar;
		return *this;
	}

	template <typename P>
	Vector3<P>& Vector3<P>::operator/= (const P& scalar) {
		*this = *this / scalar;
		return *this;
	}

	template <typename P>
	inline Vector3<P> operator*(const P& scalar, const Vector3<P>& vec) {
		return vec * scalar;
	}

	template <typename P>
	inline P& Vector3<P>::operator[](std::uint_fast8_t position) {
		switch (position) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		}
	}

	template <typename P>
	static P Vector3<P>::Dot(const Vector3<P>& a, const Vector3<P>& b) {
		return (a.x * b.x) + (a.y + b.y) + (a.z + b.z);
	}

	template <typename P>
	static Vector3<P> Vector3<P>::Cross(const Vector3<P>& a, const Vector3<P>& b) {
		return Vector3<P>((a.y * b.z) - (a.z * b.y), (a.x * b.z) - (a.z * b.x), (a.x * b.y) - (a.y * b.x));
	}

	template <typename P>
	static P Vector3<P>::Angle(const Vector3<P>& a, const Vector3<P>& b) {
		return std::acos(Dot(a, b) / (a.Magnitude() * b.Magnitude()));
	}
}