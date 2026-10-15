/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef STARTREK_SPACE_H
#define STARTREK_SPACE_H

#include "common/str.h"

#include "fixedint.h"

namespace StarTrek {

template<typename T>
struct TPoint {
	T x;
	T y;
	T z;

	TPoint() : x(0), y(0), z(0) {}
	TPoint(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

	int32 getDiagonal() {
		return (int32)sqrt((double)x * x + y * y + z * z);
	}

	TPoint<T> operator+(const TPoint<T> &p) const {
		TPoint<T> p2;
		p2.x = x + p.x;
		p2.y = y + p.y;
		p2.z = z + p.z;
		return p2;
	}
	TPoint<T> operator-(const TPoint<T> &p) const {
		TPoint<T> p2;
		p2.x = x - p.x;
		p2.y = y - p.y;
		p2.z = z - p.z;
		return p2;
	}
	void operator+=(const TPoint &p) {
		x += p.x;
		y += p.y;
		z += p.z;
	}
	void operator-=(const TPoint &p) {
		x -= p.x;
		y -= p.y;
		z -= p.z;
	}
	T &operator[](int i) {
		if (i == 0)
			return x;
		else if (i == 1)
			return y;
		else if (i == 2)
			return z;

		// Unknown Out of Range
		assert(false);
		return x;
	}
	T operator[](int i) const {
		if (i == 0)
			return x;
		else if (i == 1)
			return y;
		else if (i == 2)
			return z;

		// Unknown Out of Range
		assert(false);
		return x;
	}
};

typedef TPoint<int32> Point3;
typedef TPoint<int16> Point3_Short;
typedef TPoint<Fixed14> Point_Fixed14;


template<typename T>
struct TMatrix {
private:
	T m[3];

public:
	TMatrix() {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				m[i][j] = 0;
	}
	T &operator[](int i) {
		return m[i];
	};
	T operator[](int i) const {
		return m[i];
	};

	TMatrix operator*(const TMatrix &m2) const {
		TMatrix ret;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				ret[i][j] = 0;
				for (int a = 0; a < 3; a++)
					ret[i][j] += m[i][a] * m2[a][j];
			}
		}
		return ret;
	}

	void operator*=(const TMatrix &m2) {
		*this = *this * m2;
	}

	TMatrix<T> invert() const {
		TMatrix<T> ret;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				ret[i][j] = m[j][i];
			}
		}
		return ret;
	}
};

typedef TMatrix<Point_Fixed14> Matrix;

struct ShipSystem {
	int16 present;
	int16 maxStrength;
	int16 strength;
};

struct ShipWeapon {
	uint32 nextFireTime;
	int16 lifetime;
	int16 speed;
	int16 cooldown;
	Point3 mount; // Relative to the ship
	int16 shape;
};

struct Star {
	bool active;
	Point3 pos;
};

// Struct for objects in space.
// TODO: what does this stand for? Maybe rename it.
struct R3 {
	Point3 pos; // 0x0
	Matrix matrix; // 0xc
	int16 field1e; // 0x1e
	int16 field20; // 0x20
	int16 field22; // 0x22
	int16 field24; // 0x24
	Point3_Short speed; // 0x26
	int32 funcPtr1; // 0x2c
	int32 funcPtr2; // 0x30
	int16 field34; // 0x34
	Point3 field36; // 0x36
	Matrix matrix2; // 0x42
	int32 field54; // 0x54 (used for sorting by draw priority?)
	int16 field58; // 0x58
	int16 field5a; // 0x5a
	int16 shipViewCode = 0; // 0x66
	Bitmap *bitmap; // 0x68 (was: shpFile)
	// 0x6a (was: bitmapOffset)
	Matrix shipViewMatrix; // 0x6c
	bool shipViewMirrored = false; // 0x7e
	double field80; // 0x80
	double field88; // 0x88
	double field90; // 0x90
	double field98; // 0x98
	ShipSystem systems[16]; // 0xa0
	// 16.16 fixed-point rates and speeds
	int32 maxTurnRate; // 0x100
	int32 turnAcceleration; // 0x104
	int32 maxSpeed; // 0x108
	int32 speedAcceleration; // 0x10c
	int32 commandedSpeed; // 0x110
	Point3 targetTurnRate; // 0x114
	int32 targetSpeed; // 0x120
	int32 currentSpeed; // 0x124
	Point3 turnRate; // 0x128
	Point3 turnAngle; // 0x134
	int16 field140; // 0x140
	int16 energy; // 0x142
	bool shieldsOn; // 0x144
	bool shieldsActive; // 0x146
	bool weaponsOn; // 0x148
	bool weaponsActive; // 0x14a
	int32 enterpriseDistance; // 0x14c
	int32 enterpriseDistanceChange; // 0x150
	bool maneuverActive; // 0x154
	int16 maneuver; // 0x156
	int16 maneuverState; // 0x15a
	ShipWeapon weapons[4]; // 0x15c
	int32 engageRange; // 0x1bc
	bool canCloak; // 0x1c0
	bool cloakRequested; // 0x1c2
	bool cloakActive; // 0x1c4
	int16 cloakLevel; // 0x1c6, 0 is fully visible
	bool exploding; // 0x1c8
	int16 explosionFrame; // 0x1ca
	bool retreating; // 0x1cc
	// Projectiles use the offsets of the ship systems
	R3 *projectileOwner; // 0xa0
	int16 projectileLife; // 0xa2
	int16 projectileDamage; // 0xa4
	int16 projectileColor; // 0xa6
	// Planets use the offsets of the ship systems
	int16 planetSize; // 0xa0
	Common::String planetName; // 0xa4
	uint16 planetSeed; // 0xb0
	int16 planetColors; // 0xb2
};

// 2.14 fixed-point multiplications, with the products summed before shifting
inline int16 fixedMult(int16 a, int16 b) {
	return (int16)(((int32)a * b) >> 14);
}

inline int16 fixedMult2(int16 a, int16 b, int16 c, int16 d) {
	return (int16)(((int32)a * b + (int32)c * d) >> 14);
}

inline int16 fixedDiv(int16 a, int16 b) {
	return (int16)(((int32)a << 14) / b);
}

// Multiplies a row vector with a matrix
inline Point3 multPointMatrix(const Point3 &p, const Matrix &m) {
	Point3 result;
	for (int i = 0; i < 3; i++) {
		int32 sum = (int32)(int16)p.x * m[0][i].raw() + (int32)(int16)p.y * m[1][i].raw() + (int32)(int16)p.z * m[2][i].raw();
		result[i] = (int16)(sum >> 14);
	}
	return result;
}

inline Matrix multMatrix(const Matrix &a, const Matrix &b) {
	Matrix result;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int32 sum = (int32)a[i][0].raw() * b[0][j].raw() + (int32)a[i][1].raw() * b[1][j].raw() + (int32)a[i][2].raw() * b[2][j].raw();
			result[i][j] = Fixed14::fromRaw((int16)(sum >> 14));
		}
	}
	return result;
}

inline Matrix axisRotation(int axis, int16 sinValue, int16 cosValue) {
	// Matrix rows and columns changed by rotations around the X, Y and Z axis
	static const int kRotationAxes[3][2] = { { 1, 2 }, { 2, 0 }, { 0, 1 } };
	int a = kRotationAxes[axis][0];
	int b = kRotationAxes[axis][1];

	Matrix rotation;
	for (int i = 0; i < 3; i++)
		rotation[i][i] = Fixed14::fromRaw(0x4000);
	rotation[a][a] = Fixed14::fromRaw(cosValue);
	rotation[a][b] = Fixed14::fromRaw(sinValue);
	rotation[b][a] = Fixed14::fromRaw(-sinValue);
	rotation[b][b] = Fixed14::fromRaw(cosValue);
	return rotation;
}

inline void rotateMatrix(Matrix &matrix, int axis, int16 sinValue, int16 cosValue) {
	matrix = multMatrix(axisRotation(axis, sinValue, cosValue), matrix);
}

inline Point_Fixed14 crossProduct(const Point_Fixed14 &a, const Point_Fixed14 &b) {
	Point_Fixed14 result;
	result.x = Fixed14::fromRaw(fixedMult2(a.y.raw(), b.z.raw(), a.z.raw(), -b.y.raw()));
	result.y = Fixed14::fromRaw(fixedMult2(a.z.raw(), b.x.raw(), a.x.raw(), -b.z.raw()));
	result.z = Fixed14::fromRaw(fixedMult2(a.x.raw(), b.y.raw(), a.y.raw(), -b.x.raw()));
	return result;
}

// For the distances to the camera, which may be longer than 16 bits
inline int32 vectorLength32(int32 x, int32 y, int32 z) {
	uint64 squared = (uint64)((int64)x * x + (int64)y * y + (int64)z * z);
	uint64 root = (uint64)sqrt((double)squared);
	while (root * root > squared)
		root--;
	while ((root + 1) * (root + 1) <= squared)
		root++;
	return (int32)root;
}

inline int16 vectorLength(int16 x, int16 y, int16 z) {
	uint32 squared = (uint32)((int32)x * x + (int32)y * y + (int32)z * z);
	uint32 root = (uint32)sqrt((double)squared);
	while (root * root > squared)
		root--;
	while ((root + 1) * (root + 1) <= squared)
		root++;
	return (int16)root;
}

inline Point_Fixed14 normalizeVector(const Point_Fixed14 &v) {
	Point_Fixed14 result;
	int16 length = vectorLength(v.x.raw(), v.y.raw(), v.z.raw());
	if (length != 0) {
		result.x = Fixed14::fromRaw(fixedDiv(v.x.raw(), length));
		result.y = Fixed14::fromRaw(fixedDiv(v.y.raw(), length));
		result.z = Fixed14::fromRaw(fixedDiv(v.z.raw(), length));
	}
	return result;
}

// Removes the rounding errors accumulated by repeated rotations
inline void normalizeMatrix(Matrix &matrix) {
	matrix[0] = crossProduct(matrix[1], matrix[2]);
	matrix[1] = crossProduct(matrix[2], matrix[0]);
	for (int i = 0; i < 3; i++)
		matrix[i] = normalizeVector(matrix[i]);
}

// Returns false if the direction is zero
inline bool lookAt(const Point3 &direction, const Point_Fixed14 &up, Matrix &matrix) {
	Point_Fixed14 forward;
	for (int i = 0; i < 3; i++)
		forward[i] = Fixed14::fromRaw((int16)direction[i]);
	if (vectorLength(forward.x.raw(), forward.y.raw(), forward.z.raw()) == 0)
		return false;

	matrix[2] = normalizeVector(forward);
	matrix[0] = normalizeVector(crossProduct(up, matrix[2]));
	matrix[1] = crossProduct(matrix[2], matrix[0]);
	return true;
}

// The random number generator of the Microsoft C library
inline uint16 msRandNext(uint32 &state) {
	state = state * 0x343fd + 0x269ec3;
	return (state >> 16) & 0x7fff;
}

#define PLANET_TEXTURE_SIZE 128

// Maximum number of stars visible at once in the starfields
#define NUM_STARS 16

#define NUM_ENEMY_SHIPS 4
#define NUM_PROJECTILES 32

// Maximum number of R3 objects in space at once
#define NUM_SPACE_OBJECTS 0x30

} // End of namespace StarTrek

#endif
