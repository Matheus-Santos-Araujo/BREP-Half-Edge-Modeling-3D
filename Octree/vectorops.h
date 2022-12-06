#pragma once
#include <iostream>
#include "math.h"
#include <immintrin.h>
#define M_PI 3.14159265
#include <cmath>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
using namespace std;

struct point {
	union {
		struct {
			float x, y, z;
		};
	};

	float length() const {
		return sqrtf(x * x + y * y + z * z);
	}
	void norm() {
		x = x / length();
		y = y / length();
		z = z / length();
	};

	point unit() {
		x = x / length();
		y = y / length();
		z = z / length();

		return point{ x, y, z };
	};

	point operator*(float a) const {
		return point{ x * a, y * a, z * a };
	}
	point operator+(const point& v) const {
		return point{ x + v.x, y + v.y, z + v.z };
	}
	point operator-(const point& v) const {
		return point{ x - v.x, y - v.y, z - v.z };
	}

	void operator+=(const point& v) {
		x += v.x; y += v.y; z += v.z;
	}
	void operator-=(const point& v) {
		x -= v.x; y -= v.y; z -= v.z;
	}

	bool operator==(const point& v) const {
		return fabsf(x - v.x) < 0.000001 &&
			fabsf(y - v.y) < 0.000001 &&
			fabsf(z - v.z) < 0.000001;
	}
};

inline float dot(const point& v1, const point& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline point cross(const point& v1, const point& v2) {
	point p = { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
	return p;
}

class Vec3 {
public:
	_inline Vec3() {

	}

	_inline Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {

	}

	_inline Vec3 operator+(const Vec3& v) const {
		return Vec3(x + v.x, y + v.y, z + v.z);
	}

	_inline Vec3& operator+=(const Vec3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	_inline Vec3 operator-(const Vec3& v) const {
		return Vec3(x - v.x, y - v.y, z - v.z);
	}

	_inline Vec3& operator-=(const Vec3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	_inline Vec3 operator*(float s) const {
		return Vec3(x * s, y * s, z * s);
	}

	_inline Vec3& operator*=(float s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	_inline Vec3 operator/(float s) const {
		return Vec3(x / s, y / s, z / s);
	}

	_inline Vec3& operator/=(float s) {
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	_inline bool operator==(const Vec3& v) const {
		if ((x == v.x) && (y == v.y) && (z == v.z))
			return true;
		else
			return false;
	}

	_inline bool operator!=(const Vec3& v) const {
		if (!(*this == v))
			return true;
		else
			return false;
	}

	_inline float length() const {
		return sqrt(x * x + y * y + z * z);
	}

	float x, y, z;
};

_inline float dot(const Vec3& v1, const Vec3& v2) {
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

_inline Vec3 cross(const Vec3& v1, const Vec3& v2) {
	return Vec3(v1.y * v2.z - v2.y * v1.z,
		v1.z * v2.x - v2.z * v1.x,
		v1.x * v2.y - v2.x * v1.y);
}

_inline Vec3 normalize(const Vec3& v) {
	float length = v.length();
	return Vec3(v.x / length, v.y / length, v.z / length);
}

_inline float angle(const Vec3& v1, const Vec3& v2) {
	return acos(dot(v1, v2) / (v1.length() * v2.length()));
}

class Quat {
public:
	_inline Quat() {}

	_inline Quat(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

	_inline Quat(const Vec3& v, float s) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = s;
	}

	_inline void fromAxisAngle(const Vec3& v, float theta) {
		float angle = theta * 0.5;
		w = std::cos(angle);
		x = v.x * std::sin(angle);
		y = v.y * std::sin(angle);
		z = v.z * std::sin(angle);
		*this = Quat(x / this->length(), y / this->length(), z / this->length(), w / this->length());
	}

	_inline void toMatrix(float* m) {
		float xx, yy, zz, wx, wy, wz, xy, xz, yz;
		xx = 2 * x * x;		yy = 2 * y * y;		zz = 2 * z * z;
		wx = 2 * w * x;		wy = 2 * w * y;		wz = 2 * w * z;
		xy = 2 * x * y;		xz = 2 * x * z;		yz = 2 * y * z;

		m[0] = 1.0 - yy - zz;
		m[1] = xy - wz;
		m[2] = xz + wy;

		m[4] = xy + wz;
		m[5] = 1.0 - xx - zz;
		m[6] = yz - wx;

		m[8] = xz - wy;
		m[9] = yz + wx;
		m[10] = 1.0 - xx - yy;

		m[3] = m[7] = m[11] = m[12] = m[13] = m[14] = 0.0;
		m[15] = 1.0;
	}

	_inline float length() const { return sqrt(x * x + y * y + z * z + w * w); }

	float x, y, z, w;
};

_inline Quat operator*(const Quat& q2, const Quat& q1) {
	float _x, _y, _z, _w;

	_x = q2.y * q1.z - q2.z * q1.y + q2.w * q1.x + q2.x * q1.w;
	_y = q2.z * q1.x - q2.x * q1.z + q2.w * q1.y + q2.y * q1.w;
	_z = q2.x * q1.y - q2.y * q1.x + q2.w * q1.z + q2.z * q1.w;
	_w = q2.w * q1.w - q2.x * q1.x - q2.y * q1.y - q2.z * q1.z;

	return Quat(_x, _y, _z, _w);
}

_inline Quat normalize(const Quat& q) {
	float length = q.length();

	return Quat(q.x / length, q.y / length, q.z / length, q.w / length);
}

_inline Quat conjugate(const Quat& q) { return Quat(-q.x, -q.y, -q.z, q.w); }

_inline Vec3 rotateVec(Vec3& v, const Quat& q) {
	Quat q1(v.x, v.y, v.z, 0);
	Quat result = (q * q1) * conjugate(q);
	Vec3 resultVec(result.x, result.y, result.z);
	return resultVec;
}

class Point {
public:
	Point() {}
	Point(long int id) {
		this->id = id;
		size = 6;
	}

	Point(std::vector<double> coords, long int id) {
		this->setCoords(coords);
		size = 6;
	}

	Point(const Point* p) {
		this->setCoords(p->getCoords());
		size = 6;
	}

	~Point() {
		this->coords.clear();
	}

	unsigned int dimension() const {
		return this->coords.size();
	}

	void setCoords(std::vector<double> coords) { this->coords = coords; }

	std::vector<double> getCoords() const {
		return this->coords;
	}

	void setCoord(unsigned int i, double c) {
		if (i < this->dimension()) {
			this->coords[i] = c;
		} else {
			std::cout << "debug i (" << i << ") >= dimension(" << this->dimension() << ")" << std::endl;
		}
	}

	double getCoord(unsigned int i) const {
		if (i < this->dimension())
			return (this->coords[i]);
		else {
			std::cout << "debug i (" << i << ") >= dimension(" << this->dimension() << ")" << std::endl;
			return 0.0;
		}
	}

	double distance(const Point& p) const {
		double norm = 0.0;

		for (unsigned int i = 0; i < 2; i++) {
			norm += pow(this->coords[i] - p.coords[i], 2.0);
		}

		return sqrt(norm);
	}

	bool equal(const Point& p) const {
		unsigned int dim = max(this->dimension(), p.dimension());

		for (unsigned int i = 0; i < dim; i++) {
			if (fabs(this->getCoord(i) - p.getCoord(i)) > 0.0001) {
				return false;
			}
		}
		return true;
	}

	bool match(const Point& p) const {
		unsigned int dim = max(this->dimension(), p.dimension());

		for (unsigned int i = 0; i < dim; i++) {
			if (this->getCoord(i) != p.getCoord(i)) {
				return false;
			}
		}
		return true;
	}

	bool match(const Point* p) const { return this->match(*p); }
	bool equal(const Point* p) const { return this->equal(*p); }
	void sum(const Point& p) { this->sum(&p); }
	void sum(const Point* p) {
		unsigned int dim = max(p->dimension(), this->dimension());

		for (unsigned int i = 0; i < dim; i++) {
			this->setCoord(i, this->getCoord(i) + p->getCoord(i));
		}
	}

	void esc(const double k) {
		unsigned int dim = this->dimension();

		for (unsigned int i = 0; i < dim; i++) {
			this->setCoord(i, this->getCoord(i) * k);
		}
	}

	void draw() const {}

protected:
	int id;
	int size;
	std::vector<double> coords;
};

class Point3D : public Point {
public:
	Point3D() {}
	Point3D(double x, double y, double z) : Point(3) {
		this->coords.resize(3, 0.0);

		this->setPosition(x, y, z);
	}

	Point3D(const Point& p) : Point(3) {
		this->coords.resize(3, 0.0);

		this->setPosition(p.getCoord(0), p.getCoord(1), p.getCoord(2));
	}

	Point3D(const Point* p) : Point(3) {
		this->coords.resize(3, 0.0);

		this->setPosition(p->getCoord(0), p->getCoord(1), p->getCoord(2));
	}

	~Point3D() {}

	void setPosition(double x, double y, double z) {
		this->setX(x);
		this->setY(y);
		this->setZ(z);
	}

	void setX(double x) { this->coords[0] = x; }
    double getX() const { return this->coords[0]; }
    void setY(double y) { this->coords[1] = y; }
    double getY() const { return this->coords[1]; }
    void setZ(double z) { this->coords[2] = z; }
    double getZ() const { return this->coords[2]; }
    unsigned int dimension() const { return 3; }
    bool match(double x, double y, double z) { return Point::match(Point3D(x, y, z)); }
    bool equal(double x, double y, double z) { return Point::equal(Point3D(x, y, z)); }

#if USE_ARRAY
	using Point::operator=;
#endif 

	void draw() const {
		glPointSize(size);

		glColor3f(0.9, 0.0, 0.0);

		bool lighting = false;

		if (glIsEnabled(GL_LIGHTING))
		{
			glDisable(GL_LIGHTING);

			lighting = true;
		}

		glBegin(GL_POINTS);
		glVertex3f(this->coords[0], this->coords[1], this->coords[2]);
		glEnd();

		if (lighting)
		{
			glEnable(GL_LIGHTING);
		}
	}

};
