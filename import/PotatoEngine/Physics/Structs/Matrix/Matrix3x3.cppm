module;

#include <array>
#include <cstdint>
#include <vector>

export module PotatoEngine.Physics.Structs.Matrix:Matrix3x3;

import PotatoEngine.Physics.Structs.Vector;
import PotatoEngine.Physics.Exception;

namespace PotatoEngine::Physics::Structs {

	export template <typename P> struct Matrix3x3 {
	private:
		std::array<Vector3<P>, 3> contents;
	public:
		// Nothing
		Matrix3x3() : contents({ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } }) {}
		// 9 values
		Matrix3x3(P m00, P m01, P m02, P m10, P m11, P m12, P m20, P m21, P m22) : contents({ { m00, m01, m02 }, { m10, m11, m12 }, { m20, m21, m22 } }) {}
		// An array
		Matrix3x3(const std::array<P, 9>& arr) : contents({ {arr[0], arr[1], arr[2] }, { arr[3], arr[4], arr[5] },{ arr[6], arr[7], arr[8] } }) {}
		// A Vector Array
		Matrix3x3(const std::array<Vector3<P>, 3>& arr) : contents(arr) {};
		// An Array Array
		Matrix3x3(const std::array<std::array<P, 3>, 3> arr) : contents({ arr[0], arr[1], arr[2] }) {}
		// Another Matrix
		template <typename Other_P, typename = std::enable_if_t<std::is_convertible_v<Other_P, P>>>
		Matrix3x3(const Matrix3x3<Other_P>& original) : contents(original.contents) {}
	public:
		Matrix3x3 Transpose() const;
		Matrix3x3 Inverse() const;
		Matrix3x3 Orthogonal() const;
		Matrix3x3& Orthogonalize();
		P Determinant() const;
		P Cofactor(const P&, const P&) const;
		P Minor(const P&, const P&) const;
		Matrix3x3 Adjugate() const;
		Vector3<P> Get_Col(std::uint_fast8_t) const;
	public:
		Matrix3x3 operator+(const Matrix3x3&) const;
		Matrix3x3 operator-(const Matrix3x3&) const;
		Matrix3x3 operator*(const Matrix3x3&) const;
		Vector3<P> operator*(const Vector3<P>&) const;
		Matrix3x3 operator*(const P&) const;
		Matrix3x3& operator+=(const Matrix3x3&);
		Matrix3x3& operator-=(const Matrix3x3&);
		Matrix3x3& operator*=(const Matrix3x3&);
		Matrix3x3& operator*=(const P&);
		Vector3<P>& operator[](std::uint_fast8_t);
	};

	template <typename P>
	Matrix3x3<P> Matrix3x3<P>::Transpose() const {
		return {
			{*this[0][0], *this[1][0], *this[2][0]},
			{*this[0][1], *this[1][1], *this[2][1]},
			{*this[0][2], *this[1][2], *this[2][2]}
		};
	}

	template <typename P>
	Matrix3x3<P> Matrix3x3<P>::Inverse() const {
		if (Determinant() == 0) throw PotatoEngine::Physics::Exception::Non_Invertible_Matrix("This Matrix3x3 is not invertible!");

		Matrix3x3<P> cofactor_matrix;

		for (std::uint_fast8_t i = 0; i < 3; i++) {
			for (std::uint_fast8_t j = 0; j < 3; j++) {
				cofactor_matrix[i][j] = Cofactor(i, j);
			}
		}

		return cofactor_matrix / cofactor_matrix.Determinant();
	}
	
	template <typename P>
	Matrix3x3<P> Matrix3x3<P>::Orthogonal() const {
		Vector3<P> c1 = Get_Col(0);
		c1.Normalize();
		Vector3<P> v2 = Get_Col(1);
		Vector3<P> c2 = v2 - Vector3<P>::Dot(c1, v2) * c1;
		c2.Normalize();
		Vector3<P> c3 = Vector3<P>::Cross(c1, c2);

		return { {c1[0], c2[0], c3[0]}, {c1[1], c2[1], c3[1]}, {c1[2], c2[2], c3[2]} };


	}

	template <typename P>
	Matrix3x3<P>& Matrix3x3<P>::Orthogonalize() {
		*this = Orthogonal();
		return *this;
	}

	template <typename P>
	P Matrix3x3<P>::Determinant() const {
		return (
			*this[0][0] * *this[1][1] * *this[2][2] +
			*this[0][1] * *this[1][2] * *this[2][0] +
			*this[0][2] * *this[1][0] * *this[2][1] -
			*this[0][2] * *this[1][1] * *this[2][0] -
			*this[0][1] * *this[1][0] * *this[2][2] -
			*this[0][0] * *this[1][2] * *this[2][1]
			);
	}

	template <typename P>
	P Matrix3x3<P>::Cofactor(const P& i, const P& j) const {
		return Minor(i, j) * ((i + j) % 2 == 0) ? 1 : -1;
	}
	
	template <typename P>
	P Matrix3x3<P>::Minor(const P& i, const P& j) const {
		std::vector<P&> m2x2(4);
		for (std::uint_fast8_t k = 0; k < 3; k++) {
			if (k == i) continue;
			for (std::uint_fast8_t l = 0; l < 3; l++) {
				if (l == j) continue;
				m2x2.push_back(*this[k][l]);
			}
		}
		return m2x2.at(0) * m2x2.at(3) - m2x2.at(1) * m2x2.at(2);
	}
	
	template <typename P>
	Matrix3x3<P> Matrix3x3<P>::Adjugate() const {
		Matrix3x3<P> cofactor_matrix;

		for (std::uint_fast8_t i = 0; i < 3; i++) {
			for (std::uint_fast8_t j = 0; j < 3; j++) {
				cofactor_matrix[i][j] = Cofactor(i, j);
			}
		}

		return cofactor_matrix.Transpose();
	}

	template <typename P>
	Vector3<P> Matrix3x3<P>::Get_Col(std::uint_fast8_t col) const {
		return { *this[0][col], *this[1][col], *this[2][col] };
	}

	template <typename P>
	Matrix3x3<P> Matrix3x3<P>::operator+(const Matrix3x3<P>& other) const {
		return { *this[0] + other[0], *this[1] + other[1], *this[2] + other[2] };
	}

	template <typename P>
	Matrix3x3<P> Matrix3x3<P>::operator-(const Matrix3x3<P>& other) const {
		return { *this[0] - other[0], *this[1] - other[1], *this[2] - other[2] };
	}

	template <typename P>
	Matrix3x3<P> Matrix3x3<P>::operator*(const Matrix3x3<P>& other) const {
		Matrix3x3<P> new_matrix;
		for (std::uint_fast8_t i = 0; i < 3; i++) {
			for (std::uint_fast8_t j = 0; j < 3; j++) {
				new_matrix[i][j] = Vector3<P>::Dot(*this[i], other.Get_Col(j));
			}
		}
		return new_matrix;
	}

	template <typename P>
	Vector3<P> Matrix3x3<P>::operator*(const Vector3<P>& vec) const {
		return { Vector3<P>::Dot(*this[0], vec), Vector3<P>::Dot(*this[1], vec), Vector3<P>::Dot(*this[2], vec) };
	}

	template <typename P>
	Matrix3x3<P> Matrix3x3<P>::operator*(const P& scalar) const {
		Matrix3x3<P> new_matrix;
		for (std::uint_fast8_t i = 0; i < 3; i++) {
			for (std::uint_fast8_t j = 0; j < 3; j++) {
				new_matrix[i][j] = *this[i][j] * scalar;
			}
		}
		return new_matrix;
	}

	template <typename P>
	Matrix3x3<P>& Matrix3x3<P>::operator+=(const Matrix3x3<P>& other) {
		*this = *this + other;
		return *this;
	}

	template <typename P>
	Matrix3x3<P>& Matrix3x3<P>::operator-=(const Matrix3x3<P>& other) {
		*this = *this - other;
		return *this;
	}

	template <typename P>
	Matrix3x3<P>& Matrix3x3<P>::operator*=(const Matrix3x3<P>& other) {
		*this = *this * other;
	}

	template <typename P>
	Matrix3x3<P>& Matrix3x3<P>::operator*=(const P& scalar) {
		*this = *this * scalar;
		return *this;
	}

	template <typename P>
	Vector3<P>& Matrix3x3<P>::operator[](std::uint_fast8_t row) {
		return *this[row];
	}
}