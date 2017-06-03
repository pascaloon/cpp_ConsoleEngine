#pragma once

class Vector3
{
public:
	Vector3() : X(0.0), Y(0.0), Z(0.0) {}
	Vector3(double x, double y, double z) : X(x), Y(y), Z(z) {}

	~Vector3() {}

	double operator[](size_t i) const { return i == 0 ? X : (i == 1 ? Y : Z); }


	double X;
	double Y;
	double Z;

private:

};


class Matrix3
{
public:
	Matrix3() {}
	Matrix3(const Vector3& A, const Vector3& B, const Vector3& C)
	{
		_rows[0] = A;
		_rows[1] = B;
		_rows[2] = C;
	}
	~Matrix3(){}

	const Vector3& operator[](size_t i) const { return _rows[i]; }
	Vector3& operator[](size_t i) { return _rows[i]; }

private:
	Vector3 _rows[3];
};

inline bool operator==(const Vector3& lhs, const Vector3& rhs)
{
	return lhs.X == rhs.X
		&& lhs.Y == rhs.Y
		&& lhs.Z == rhs.Z;
}

inline bool operator!=(const Vector3& lhs, const Vector3& rhs)
{
	return !(lhs == rhs);
}

inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z);
}
inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z);
}
inline Vector3 operator*(const Vector3& lhs, double v)
{
	return Vector3(lhs.X * v, lhs.Y * v, lhs.Z * v);
}
inline Vector3 operator*(double v, const Vector3& rhs)
{
	return Vector3(rhs.X * v, rhs.Y * v, rhs.Z * v);
}
inline Vector3 operator*(const Matrix3& lhs, const Vector3& rhs)
{
	return Vector3(
		(lhs[0][0] * rhs[0]) + (lhs[0][1] * rhs[1]) + (lhs[0][2] * rhs[2]),
		(lhs[1][0] * rhs[0]) + (lhs[1][1] * rhs[1]) + (lhs[1][2] * rhs[2]),
		(lhs[2][0] * rhs[0]) + (lhs[2][1] * rhs[1]) + (lhs[2][2] * rhs[2]));
}

#define PI 3.14159265
inline double FromDegrees(double degrees)
{
	return degrees * PI / 180.0;
}
