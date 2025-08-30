module;

#include <array>
#include <tuple>
#include <utility>
#include <string>
#include <cmath>

export module PotatoEngine.Physics.Structs.Quaternion;

import PotatoEngine.Physics.Structs.Vector;
import PotatoEngine.Physics.Structs.Matrix;

namespace PotatoEngine::Physics::Structs {
	export template <typename P> class Quaternion {
	public: 
		P w = 0, x = 0, y = 0, z = 0;

	public: // Constructors
		// Default (Identity Quaternion)
		Quaternion() : w(1), x(0), y(0), z(0) {}
		// 4 variables
		Quaternion(P new_w, P new_x, P new_y, P new_z) : w(new_w), x(new_x), y(new_y), z(new_y) {}
		// Another Quaternion
		template <typename Other_P, typename = std::enable_if_t<std::is_convertible_v<Other_P, P>>>
		Quaternion(const Quaternion<Other_P>& original) : w(original.at(w)), x(original.x), y(original.y), z(original.z) {}
		// An Array
		Quaternion(const std::array<P, 4>& array) : w(array.at(0)), x(array.at(1)), y(array.at(2)), z(array.at(3)) {}
		// A Matrix
		Quaternion(const Matrix3x3<P>&);
		// A Vector
		Quaternion(const Vector3<P>& vector) : w(0), x(vector.x), y(vector.y), z(vector.z) {};
		// Euler Angles
		Quaternion(const P&, const P&, const P&);
		// Axis-Angle
		Quaternion(const Vector3<P>&, const P&);
	
	public: // Member Functions
		P Sqr_Magnitude() const;
		P Magnitude() const;
		Quaternion Normalized() const;
		Quaternion& Normalize();
		Quaternion Conjugate() const;
		Quaternion Inverse() const;
		std::tuple<P, P, P> To_Euler_Angles() const;
		std::pair<Vector3<P>, P> To_Axis_Angle() const;
		Matrix3x3<P> To_Matrix() const;
		std::string String() const;
	public: // Operators
		Quaternion operator+(const Quaternion&) const;
		Quaternion operator-(const Quaternion&) const;
		Quaternion operator*(const Quaternion&) const;
		Quaternion operator/(const Quaternion&) const;
		Quaternion operator*(const P&) const;
		Quaternion operator/(const P&) const;
		Quaternion& operator+=(const Quaternion&);
		Quaternion& operator-=(const Quaternion&);
		Quaternion& operator*=(const Quaternion&);
		Quaternion& operator/=(const Quaternion&);
		Quaternion& operator*=(const P&);
		Quaternion& operator/=(const P&);
	public: // Static Functions
		static Quaternion Dot(const Quaternion&, const Quaternion&);		
	};

	template <typename P>
	Quaternion<P>::Quaternion(const Matrix3x3<P>& m) {
		P trace = m[0][0] + m[1][1] + m[2][2];
		P max_element = std::max(m[0][0], m[1][1], m[2][2]);
		
		if (trace > 0) {
			P s = std::sqrt(trace + 1) * 2;
			w = s / 4;
			x = (m[2][1] - m[1][2]) * s;
			y = (m[0][2] - m[2][0]) * s;
			z = (m[1][0] - m[0][1]) * s;
		}
		else {
			switch (max_element) {
			case m[0][0]:
				P s = std::sqrt(1 + m[0][0] - m[1][1] - m[2][2]) * 2;
				w = (m[2][1] - m[1][2]) / s;
				x = s / 4;
				y = (m[0][1] + m[1][0]) / s;
				z = (m[0][2] + m[2][0]) / s;
				break;
			case m[1][1]:
				P s = std::sqrt(1.0 + m[1][1] - m[0][0] - m[2][2]) * 2;
				w = (m[0][2] - m[2][0]) / s;
				x = (m[0][1] + m[1][0]) / s;
				y = s / 4;
				z = (m[1][2] + m[2][1]) / s;
				break;
			case m[2][2]:
				P s = std::sqrt(1.0 + m[2][2] - m[0][0] - m[1][1]) * 2;
				w = (m[1][0] - m[0][1]) / s;
				x = (m[0][2] + m[2][0]) / s;
				y = (m[1][2] + m[2][1]) / s;
			    z = s / 4;
			}
		}
		if (w < 0) {
			*this *= -1;
		}
	}

	template <typename P>
	Quaternion<P>::Quaternion(const P& roll, const P& pitch, const P& yaw) {
		P cos_roll = std::cos(roll * 0.5);
		P cos_pitch = std::cos(pitch * 0.5);
		P cos_yaw = std::cos(yaw * 0.5);
		P sin_roll = std::sin(roll * 0.5);
		P sin_pitch = std::sin(pitch * 0.5);
		P sin_yaw = std::sin(yaw * 0.5);

		w = cos_roll * cos_pitch * cos_yaw + sin_roll * sin_pitch * sin_yaw;
		x = sin_roll * cos_pitch * cos_yaw - cos_roll * sin_pitch * sin_yaw;
		y = cos_roll * sin_pitch * cos_yaw + sin_roll * cos_pitch * sin_yaw;
		z = cos_roll * cos_pitch * sin_yaw - sin_roll * sin_pitch * cos_yaw;
	}

	template <typename P>
	Quaternion<P>::Quaternion(const Vector3<P>& axis, const P& angle) {
		w = std::cos(angle / 2);
		P sin_a = std::sin(angle / 2);
		x = sin_a * std::cos(axis.x);
		y = sin_a * std::cos(axis.y);
		z = sin_a * std::cos(axis.z);
	}

	template <typename P>
	P Quaternion<P>::Sqr_Magnitude() const {
		return (w * w) + (x * x) + (y * y) + (z * z);
	}

	template <typename P>
	P Quaternion<P>::Magnitude() const {
		return std::sqrt((w * w) + (x * x) + (y * y) + (z * z));
	}
	
	template <typename P>
	Quaternion<P> Quaternion<P>::Normalized() const {
		return *this / Magnitude();
	}

	template <typename P>
	Quaternion<P>& Quaternion<P>::Normalize() {
		*this = Normalized();
		return *this;
	}
	
	template <typename P>
	Quaternion<P> Quaternion<P>::Conjugate() const {
		return { w, -x, -y, -z };
	}
	
	template <typename P>
	Quaternion<P> Quaternion<P>::Inverse() const {
		return (Conjugate()) / (Sqr_Magnitude());
	}
	template <typename P>
	std::tuple<P, P, P> Quaternion<P>::To_Euler_Angles() const {
		P sin_roll_cos_pitch = 2 * (w * x + y * z);
		P cos_roll_cos_pitch = 1 - 2 * (x * x + y * y);
		P roll = std::atan2(sin_roll_cos_pitch, cos_roll_cos_pitch);

		P sin_pitch = std::sqrt(1 + 2 * (w * y - x * z));
		P cos_pitch = std::sqrt(1 - 2 * (w * y - x * z));
		P pitch = 2 * std::atan2(sin_pitch, cos_pitch);

		P sin_yaw_cos_pitch = 2 * (w * z + x * y);
		P cos_yaw_cos_pitch = 1 - 2 * (y * y + z * z);
		P yaw = std::atan2(sin_yaw_cos_pitch, cos_yaw_cos_pitch);

		return { roll, pitch, yaw };
	}

	template <typename P>
	std::pair<Vector3<P>, P> Quaternion<P>::To_Axis_Angle() const {
		P angle = 2 * std::acos(w);
		Vector3<P> axis;
		axis.x = x / std::sqrt(1 - (w * w));
		axis.y = y / std::sqrt(1 - (w * w));
		axis.z = z / std::sqrt(1 - (w * w));

		return { axis, angle };
	}

	template <typename P>
	Matrix3x3<P> Quaternion<P>::To_Matrix() const {
		return { 
			{1 - 2 * (y * y) - 2 * (z * z), 2 * x * y - 2 * z * w, 2 * x * z + 2 * y * w},
			{2 * x * y + 2 * z * w, 1 - 2 * (x * x) - 2 * (z * z), 2 * y * z - 2 * x * w},
			{2 * x * z - 2 * y * w, 2 * y * z + 2 * x * w, 1 - 2 * (x * x) - 2 * (y * y)}
		};
	}

	template <typename P>
	std::string Quaternion<P>::String() const {
		return "(" + std::to_string(w) + ", " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
	}

	template <typename P>
	Quaternion<P> Quaternion<P>::operator+(const Quaternion& other) const {
		return { w + other.w, x + other.x, y + other.y, z + other.z };
	}

	template <typename P>
	Quaternion<P> Quaternion<P>::operator-(const Quaternion& other) const {
		return { w - other.w, x - other.x, y - other.y, z - other.z };
	}

	template <typename P>
	Quaternion<P> Quaternion<P>::operator*(const Quaternion& other) const {
		return {
			w * other.w - x * other.x - y * other.y - z * other.z,
			w * other.x + x * other.w + y * other.z - z * other.y,
			w * other.y - x * other.z + y * other.w + z * other.x,
			w * other.z + x * other.y - y * other.x + z * other.w
		};
	}

	template <typename P>
	Quaternion<P> Quaternion<P>::operator/(const Quaternion& other) const {
		return *this * other.Inverse();
	}

	template <typename P>
	Quaternion<P> Quaternion<P>::operator*(const P& scalar) const {
		return { w * scalar, x * scalar, y * scalar, z * scalar };
	}

	template <typename P>
	Quaternion<P> Quaternion<P>::operator/(const P& scalar) const {
		return { w / scalar, x / scalar, y / scalar, z / scalar };
	}
	
	template <typename P>
	Quaternion<P>& Quaternion<P>::operator+=(const Quaternion& other) {
		*this = *this + other;
		return *this;
	}

	template <typename P>
	Quaternion<P>& Quaternion<P>::operator-=(const Quaternion& other) {
		*this = *this - other;
		return *this;
	}
	
	template <typename P>
	Quaternion<P>& Quaternion<P>::operator*=(const Quaternion& other) {
		*this = *this * other;
		return *this;
	}

	template <typename P>
	Quaternion<P>& Quaternion<P>::operator/=(const Quaternion& other) {
		*this = *this * other;
		return *this;
	}

	template <typename P>
	Quaternion<P>& Quaternion<P>::operator*=(const P& scalar) {
		*this = *this * scalar;
		return *this;
	}

	template <typename P>
	Quaternion<P>& Quaternion<P>::operator/=(const P& scalar) {
		*this = *this / scalar;
		return *this;
	}

	template <typename P>
	static Quaternion<P> Quaternion<P>::Dot(const Quaternion<P>& a, const Quaternion<P>& b) {
		return (a.w * b.w) + (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	}
}