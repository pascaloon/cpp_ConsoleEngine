#pragma once

#include <cmath>

class Matrix4;
class Vector3;
class Vector4;

Vector3 operator*(double, const Vector3&);
Vector3 operator*(const Vector3&, double);

inline Matrix4 operator*(const Matrix4&, const Matrix4&);

class Vector3
{
public:
	Vector3() : X(0.0), Y(0.0), Z(0.0) {}
	Vector3(double x, double y, double z) : X(x), Y(y), Z(z) {}

	~Vector3() {}

	double operator[](size_t i) const { return i == 0 ? X : (i == 1 ? Y : Z); }

	static Vector3 Cross(const Vector3& a, const Vector3& b)
	{
		return Vector3((a.Y * b.Z) - (a.Z * b.Y), (a.Z * b.X) - (a.X * b.Z), (a.X * b.Y) - (a.Y * b.X));
	}

	static double Norm(const Vector3& v)
	{
		return sqrt((v.X * v.X) + (v.Y * v.Y) + (v.Z * v.Z));
	}

	static Vector3 Unit(const Vector3& v)
	{
		return (1.0 / Norm(v)) * v;
	}
	

	double X;
	double Y;
	double Z;
	static const Vector3 Zero;

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

inline Vector3 operator-(const Vector3& v)
{
	return Vector3(-v.X, -v.Y, -v.Z);
}

// Dot Product
inline double operator*(const Vector3& lhs, const Vector3& rhs)
{
	return (lhs.X * rhs.X) + (lhs.Y * rhs.Y) + (lhs.Z * rhs.Z);
}

inline Vector3 operator*(const Matrix3& lhs, const Vector3& rhs)
{
	return Vector3(
		(lhs[0][0] * rhs[0]) + (lhs[0][1] * rhs[1]) + (lhs[0][2] * rhs[2]),
		(lhs[1][0] * rhs[0]) + (lhs[1][1] * rhs[1]) + (lhs[1][2] * rhs[2]),
		(lhs[2][0] * rhs[0]) + (lhs[2][1] * rhs[1]) + (lhs[2][2] * rhs[2]));
}


class Vector4
{
public:
	Vector4() : X(0.0), Y(0.0), Z(0.0), W(0.0) {}
	Vector4(const Vector3& v, double w) : X(v.X), Y(v.Y), Z(v.Z), W(w) {}
	Vector4(double x, double y, double z, double w) : X(x), Y(y), Z(z), W(w) {}

	~Vector4() {}

	double operator[](size_t i) const { return i == 0 ? X : (i == 1 ? Y : (i == 2 ? Z : W)); }
	double& operator[](size_t i) { return i == 0 ? X : (i == 1 ? Y : (i == 2 ? Z : W)); }

	Vector3 XYZ() const
	{
		return Vector3{ X, Y, Z };
	}

	double X;
	double Y;
	double Z;
	double W;

private:

};

double FromDegrees(double degrees);

class Matrix4
{
public:
	Matrix4() {}
	Matrix4(const Vector4& A, const Vector4& B, const Vector4& C, const Vector4& D)
	{
		_rows[0] = A;
		_rows[1] = B;
		_rows[2] = C;
		_rows[3] = D;
	}
	~Matrix4() {}

	const Vector4& operator[](size_t i) const { return _rows[i]; }
	Vector4& operator[](size_t i) { return _rows[i]; }


	Matrix4 Inverse()
	{
		
	}

	static Matrix4 CreateRotationMatrixX(double xRot)
	{
		double cosRotX = cos(FromDegrees(xRot));
		double sinRotX = sin(FromDegrees(xRot));
		return Matrix4 {
			Vector4{ 1,        0,	     0,    0 },
			Vector4{ 0,  cosRotX, -sinRotX,    0 },
			Vector4{ 0,  sinRotX,  cosRotX,    0 },
			Vector4{ 0,		   0,		 0,    1 }};
	}
	static Matrix4 CreateRotationMatrixY(double yRot)
	{
		double cosRotY = cos(FromDegrees(yRot));
		double sinRotY = sin(FromDegrees(yRot));
		return Matrix4{
			Vector4{ cosRotY,	      0,  sinRotY,    0 },
			Vector4{ 0,               1,        0,    0 },
			Vector4{ -sinRotY,        0,  cosRotY,    0 },
			Vector4{ 0,		          0,		0,    1 }};
	}
	static Matrix4 CreateRotationMatrixZ(double zRot)
	{
		double cosRotZ = cos(FromDegrees(zRot));
		double sinRotZ = sin(FromDegrees(zRot));
		return Matrix4{
			Vector4{  cosRotZ,   sinRotZ,        0,   0 },
			Vector4{ -sinRotZ,   cosRotZ,        0,   0 },
			Vector4{        0,         0,        1,   0 },
			Vector4{        0,		   0,		 0,   1 }};
	}
	static Matrix4 CreateScaleMatrix(const Vector3& scale)
	{
		return Matrix4{
			Vector4{ scale.X,         0,        0,   0 },
			Vector4{       0,   scale.Y,        0,   0 },
			Vector4{       0,         0,  scale.Z,   0 },
			Vector4{       0,		  0,		0,   1 }};
	}

	static Matrix4 CreateTranslationMatrix(const Vector3& translation)
	{
		return Matrix4{
			Vector4{ 1,   0,  0,   translation.X },
			Vector4{ 0,   1,  0,   translation.Y },
			Vector4{ 0,   0,  1,   translation.Z },
			Vector4{ 0,	  0,  0,			   1 }};
	}

	static Matrix4 LookAtMatrix(const Vector3& eye, const Vector3& at, const Vector3& up)
	{
		Vector3 zAxis = Vector3::Unit(at - eye);
		Vector3 xAxis = Vector3::Unit(Vector3::Cross(up, zAxis));
		Vector3 yAxis = Vector3::Cross(zAxis, xAxis);

		// s = x; u = y; f = z;
		return
		Matrix4 {
			Vector4{  xAxis.X,         xAxis.Y,          xAxis.Z,   0 },
			Vector4{  yAxis.X,         yAxis.Y,          yAxis.Z,   0 },
			Vector4{ -zAxis.X,        -zAxis.Y,         -zAxis.Z,   0 },
			Vector4{        0,               0,                0,   1 } }
		*
		Matrix4{
			Vector4{ 1,        0,       0,   -eye.X },
			Vector4{ 0,        1,       0,   -eye.Y },
			Vector4{ 0,        0,       1,   -eye.Z },
			Vector4{ 0,        0,       0,   1 } };



		/*return Matrix4{
			Vector4{        xAxis.X,          yAxis.X,          zAxis.X,   0 },
			Vector4{        xAxis.Y,          yAxis.Y,          zAxis.Y,   0 },
			Vector4{        -xAxis.Z,          -yAxis.Z,          -zAxis.Z,   0 },
			Vector4{ xAxis * (-eye),   yAxis * (-eye),   zAxis * (-eye),   1 } };*/
	}

	static Matrix4 PerspectiveMatrix(double near, double far, double top, double bottom, double left, double right)
	{
		return Matrix4{
			Vector4{ 2.0 * near / (right - left),                           0,             (right+left)/(right-left),                                 0 },
			Vector4{						   0,  2.0 * near / (top - bottom),      (top + bottom) / (top - bottom),                                 0 },
			Vector4{						   0,                           0,           (far + near) / (far - near),   -2.0 * far* near / (far - near) },
			Vector4{						   0,                           0,                                  -1.0,                                 1 } };
	}

private:
	Vector4 _rows[4];
};

inline bool operator==(const Vector4& lhs, const Vector4& rhs)
{
	return lhs.X == rhs.X
		&& lhs.Y == rhs.Y
		&& lhs.Z == rhs.Z
		&& lhs.W == rhs.W;
}

inline bool operator!=(const Vector4& lhs, const Vector4& rhs)
{
	return !(lhs == rhs);
}

inline Vector4 operator+(const Vector4& lhs, const Vector4& rhs)
{
	return Vector4(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z, lhs.W + rhs.W);
}
inline Vector4 operator-(const Vector4& lhs, const Vector4& rhs)
{
	return Vector4(lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z, lhs.W - rhs.W);
}
inline Vector4 operator*(const Vector4& lhs, double v)
{
	return Vector4(lhs.X * v, lhs.Y * v, lhs.Z * v, lhs.W * v);
}
inline Vector4 operator*(double v, const Vector4& rhs)
{
	return rhs * v;
}

// Dot Product
inline double operator*(const Vector4& lhs, const Vector4& rhs)
{
	return (lhs.X * rhs.X) + (lhs.Y * rhs.Y) + (lhs.Z * rhs.Z) + (lhs.W * rhs.W);
}

inline Vector4 operator*(const Matrix4& lhs, const Vector4& rhs)
{
	return Vector4(
		(lhs[0][0] * rhs[0]) + (lhs[0][1] * rhs[1]) + (lhs[0][2] * rhs[2]) + (lhs[0][3] * rhs[3]),
		(lhs[1][0] * rhs[0]) + (lhs[1][1] * rhs[1]) + (lhs[1][2] * rhs[2]) + (lhs[1][3] * rhs[3]),
		(lhs[2][0] * rhs[0]) + (lhs[2][1] * rhs[1]) + (lhs[2][2] * rhs[2]) + (lhs[2][3] * rhs[3]),
		(lhs[3][0] * rhs[0]) + (lhs[3][1] * rhs[1]) + (lhs[3][2] * rhs[2]) + (lhs[3][3] * rhs[3]));
}

inline Matrix4 operator*(const Matrix4& lhs, const Matrix4& rhs)
{
	Matrix4 out;

	for (size_t i = 0; i < 4; i++)
	{ 
		for (size_t j = 0; j < 4; j++)
		{
			out[i][j] = 0.0;
			for (size_t k = 0; k < 4; k++)
			{
				out[i][j] += lhs[i][k] * rhs[k][j];
			}

		}
	}

	return out;
}

inline Matrix4 operator*(double lhs, const Matrix4& rhs)
{
	Matrix4 out;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			out[i][j] = rhs[i][j] * lhs;
		}
	}

	return out;
}

#define PI 3.14159265
inline double FromDegrees(double degrees)
{
	return degrees * PI / 180.0;
}
