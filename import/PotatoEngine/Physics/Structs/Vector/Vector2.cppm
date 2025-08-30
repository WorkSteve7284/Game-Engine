module;

#include <cmath>
#include <array>
#include <string>

export module PotatoEngine.Physics.Structs.Vector:Vector2;

namespace PotatoEngine::Physics::Structs {

	export template <typename P> struct Vector2 {
	public:
		P x = 0, y = 0;

	public: // Constructors
		Vector2() : x(0), y(0) {};
		// Two numbers
		Vector2(const P new_x, const P new_y) : x(new_x), y(new_y) {}
		// Another Vector2
		template <typename Other_P, typename = std::enable_if_t<std::is_convertible_v<Other_P, P>>>
		Vector2(const Vector2<Other_P>& original) : x(original.x), y(original.y) {}
		// An Array
		Vector2(const std::array<P, 2>& original) : x(original.at(0)), y(original.at(1)) {}

	public: // Member Functions
		P Sqr_Magnitude() const;
		P Magnitude() const;
		Vector2<P> Normalized() const;
		Vector2<P>& Normalize();
		Vector2<P> Conjugate() const;
		std::string String() const;
	public: // Operators
		Vector2<P> operator+ (const Vector2<P>&) const;
		Vector2<P> operator- (const Vector2<P>&) const;
		Vector2<P> operator* (const Vector2<P>&) const;
		Vector2<P> operator/ (const Vector2<P>&) const;
		Vector2<P> operator* (const P&)	const;
		Vector2<P> operator/ (const P&)	const;
		Vector2<P>& operator+= (const Vector2<P>&);
		Vector2<P>& operator-= (const Vector2<P>&);
		Vector2<P>& operator*= (const Vector2<P>&);
		Vector2<P>& operator/= (const Vector2<P>&);
		Vector2<P>& operator*= (const P&);
		Vector2<P>& operator/= (const P&);
	public: // Static Functions
		static P Dot(const Vector2<P>&, const Vector2<P>&);
		static P Angle(const Vector2<P>&, const Vector2<P>&);
		static Vector2<P> Reflect(const Vector2<P>&, const Vector2<P>&);
		static Vector2<P> Project(const Vector2<P>&, const Vector2<P>&);
	};

	export template <typename P>
	inline Vector2<P> operator*(const P&, const Vector2<P>&);

	template <typename P>
	P Vector2<P>::Sqr_Magnitude() const {
		return (x * x) + (y * y);
	}

	template <typename P>
	P Vector2<P>::Magnitude() const {
		return std::sqrt((x * x) + (y * y));
	}

	template <typename P>
	Vector2<P> Vector2<P>::Normalized() const {
		return (*this) / this->Magnitude();
	}

	template <typename P>
	Vector2<P>& Vector2<P>::Normalize() {
		*this /= this->Magnitude();
		return *this;
	}

	template <typename P>
	Vector2<P> Vector2<P>::Conjugate() const {
		return Vector2<P>(x, -y);
	}

	template <typename P>
	std::string Vector2<P>::String() const {
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	}

	template <typename P>
	Vector2<P> Vector2<P>::operator +	 (const Vector2<P>& other) const {
		return Vector2<P>(x + other.x, y + other.y);
	}

	template <typename P>
	Vector2<P> Vector2<P>::operator- (const Vector2<P>& other) const {
		return Vector2<P>(x - other.x, y - other.y);
	}

	template <typename P>
	Vector2<P> Vector2<P>::operator* (const Vector2<P>& other) const {
		return Vector2<P>(x * other.x - y * other.y, x * other.y + y * other.x);
	}

	template <typename P>
	Vector2<P> Vector2<P>::operator/ (const Vector2<P>& other) const {
		return (*this * other.Conjugate()) / other.Sqr_Magnitude();
	}

	template <typename P>
	Vector2<P> Vector2<P>::operator* (const P& scalar) const {
		return Vector2<P>(x * scalar, y * scalar);
	}

	template <typename P>
	Vector2<P> Vector2<P>::operator/ (const P& scalar) const {
		return Vector2<P>(x / scalar, y / scalar);
	}

	template <typename P>
	Vector2<P>& Vector2<P>::operator+= (const Vector2<P>& other) {
		*this = *this + other;
		return *this;
	}

	template <typename P>
	Vector2<P>& Vector2<P>::operator-= (const Vector2<P>& other) {
		*this = *this - other;
		return *this;
	}
	template <typename P>
	Vector2<P>& Vector2<P>::operator*= (const Vector2<P>& other) {
		*this = *this * other;
		return *this;
	}

	template <typename P>
	Vector2<P>& Vector2<P>::operator/= (const Vector2<P>& other) {
		*this = *this / other;
		return *this;
	}

	template <typename P>
	Vector2<P>& Vector2<P>::operator*= (const P& scalar) {
		*this = *this * scalar;
		return *this;
	}

	template <typename P>
	Vector2<P>& Vector2<P>::operator/= (const P& scalar) {
		*this = *this / scalar;
		return *this;
	}

	template <typename P>
	inline Vector2<P> operator*(const P& scalar, const Vector2<P>& vec) {
		return vec * scalar;
	}

	template <typename P>
	static P Vector2<P>::Dot(const Vector2<P>& a, const Vector2<P>& b) {
		return (a.x * b.x) + (a.y + b.y);
	}

	template <typename P>
	static P Vector2<P>::Angle(const Vector2<P>& a, const Vector2<P>& b) {
		return std::acos(Dot(a, b) / (a.Magnitude() * b.Magnitude()));
	}

	template <typename P>
	static Vector2<P> Vector2<P>::Reflect(const Vector2<P>& ray, const Vector2<P>& normal) {
		return ray - 2 * (Dot(ray, normal.Normalized())) * normal.Normalized();
	}

	template <typename P>
	static Vector2<P> Vector2<P>::Project(const Vector2<P>& from, const Vector2<P>& to) {
		return to * (Dot(from, to) / to.Sqr_Magnitude());
	}
}