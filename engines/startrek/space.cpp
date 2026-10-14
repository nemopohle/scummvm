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

#include "startrek/resource.h"
#include "startrek/startrek.h"

namespace StarTrek {

// Multiplies a row vector with a matrix
static Point3 multPointMatrix(const Point3 &p, const Matrix &m) {
	Point3 result;
	for (int i = 0; i < 3; i++) {
		int32 sum = (int32)(int16)p.x * m[0][i].raw() + (int32)(int16)p.y * m[1][i].raw() + (int32)(int16)p.z * m[2][i].raw();
		result[i] = (int16)(sum >> 14);
	}
	return result;
}

static Matrix multMatrix(const Matrix &a, const Matrix &b) {
	Matrix result;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int32 sum = (int32)a[i][0].raw() * b[0][j].raw() + (int32)a[i][1].raw() * b[1][j].raw() + (int32)a[i][2].raw() * b[2][j].raw();
			result[i][j] = Fixed14::fromRaw((int16)(sum >> 14));
		}
	}
	return result;
}

// Matrix rows and columns changed by rotations around the X, Y and Z axis
static const int kRotationAxes[3][2] = { { 1, 2 }, { 2, 0 }, { 0, 1 } };

static Matrix axisRotation(int axis, int16 sinValue, int16 cosValue) {
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

static void rotateMatrix(Matrix &matrix, int axis, int16 sinValue, int16 cosValue) {
	matrix = multMatrix(axisRotation(axis, sinValue, cosValue), matrix);
}

static Point_Fixed14 crossProduct(const Point_Fixed14 &a, const Point_Fixed14 &b) {
	Point_Fixed14 result;
	result.x = Fixed14::fromRaw(fixedMult2(a.y.raw(), b.z.raw(), a.z.raw(), -b.y.raw()));
	result.y = Fixed14::fromRaw(fixedMult2(a.z.raw(), b.x.raw(), a.x.raw(), -b.z.raw()));
	result.z = Fixed14::fromRaw(fixedMult2(a.x.raw(), b.y.raw(), a.y.raw(), -b.x.raw()));
	return result;
}

static int16 vectorLength(int16 x, int16 y, int16 z) {
	uint32 squared = (uint32)((int32)x * x + (int32)y * y + (int32)z * z);
	uint32 root = (uint32)sqrt((double)squared);
	while (root * root > squared)
		root--;
	while ((root + 1) * (root + 1) <= squared)
		root++;
	return (int16)root;
}

static Point_Fixed14 normalizeVector(const Point_Fixed14 &v) {
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
static void normalizeMatrix(Matrix &matrix) {
	matrix[0] = crossProduct(matrix[1], matrix[2]);
	matrix[1] = crossProduct(matrix[2], matrix[0]);
	for (int i = 0; i < 3; i++)
		matrix[i] = normalizeVector(matrix[i]);
}

void StarTrekEngine::initTrigTables() {
	// Same steps and truncation as the original
	const double step = 0.00613592315234375;

	double angle = 0.0;
	for (int i = 0; i < ARRAYSIZE(_sinTable); i++) {
		_sinTable[i] = (int16)(sin(angle) * 16384.0);
		_cosTable[i] = (int16)(cos(angle) * 16384.0);
		angle += step;
	}

	angle = 0.0;
	for (int i = 0; i < ARRAYSIZE(_tanTable); i++) {
		_tanTable[i] = (int16)(tan(angle) * 16384.0);
		angle += step;
	}

	for (int i = 0; i < ARRAYSIZE(_turnSinTable); i++) {
		angle = (16.0 * i) / 16384.0;
		_turnSinTable[i] = (int16)(sin(angle) * 16384.0);
		_turnCosTable[i] = (int16)(cos(angle) * 16384.0);
		_turnAngleTable[i] = (int32)(512.0 * angle / 3.141592654 * 65536.0);
	}
}

int16 StarTrekEngine::angleSin(int16 angle) {
	int16 a = angle & 0x3ff;
	int16 index = a & 0xff;

	switch (a >> 8) {
	case 0:
		return _sinTable[index];
	case 1:
		return _sinTable[0x100 - index];
	case 2:
		return -_sinTable[index];
	default:
		return -_sinTable[0x100 - index];
	}
}

int16 StarTrekEngine::angleCos(int16 angle) {
	return angleSin(angle + 0x100);
}

int16 StarTrekEngine::angleAtan2(int16 x, int16 z) {
	if (x == 0 && z == 0)
		return 0;

	bool xNegative = x <= 0;
	bool zNegative = z <= 0;
	int16 absX = ABS(x);
	int16 absZ = ABS(z);
	bool zSmaller = absZ <= absX;
	int16 ratio = zSmaller ? fixedDiv(absZ, absX) : fixedDiv(absX, absZ);

	int16 low = 0;
	int16 high = 0x80;
	int16 angle = 0;
	while (low <= high) {
		angle = (low + high) >> 1;
		if (ratio < _tanTable[angle])
			high = angle - 1;
		else if (ratio <= _tanTable[angle])
			break;
		else
			low = angle + 1;
	}

	if (zSmaller) {
		if (!zNegative && !xNegative)
			return 0x100 - angle;
		else if (zNegative && !xNegative)
			return angle + 0x100;
		else if (zNegative && xNegative)
			return -0x100 - angle;
		else
			return angle - 0x100;
	} else {
		if (!zNegative && !xNegative)
			return angle;
		else if (!zNegative && xNegative)
			return -angle;
		else if (zNegative && !xNegative)
			return 0x200 - angle;
		else
			return angle - 0x200;
	}
}

int16 StarTrekEngine::angleAcos(int16 value) {
	bool negative = value < 0;
	if (negative)
		value = -value;

	int16 low = 0;
	int16 high = 0x100;
	int16 angle = 0;
	while (low <= high) {
		angle = (low + high) >> 1;
		if (value < _cosTable[angle])
			low = angle + 1;
		else if (value <= _cosTable[angle])
			break;
		else
			high = angle - 1;
	}

	return negative ? 0x200 - angle : angle;
}

int16 StarTrekEngine::angleAsin(int16 value) {
	bool negative = value < 0;
	if (negative)
		value = -value;

	int16 low = 0;
	int16 high = 0x100;
	int16 angle = 0;
	while (low <= high) {
		angle = (low + high) >> 1;
		if (value < _sinTable[angle])
			high = angle - 1;
		else if (value <= _sinTable[angle])
			break;
		else
			low = angle + 1;
	}

	return negative ? -angle : angle;
}

void StarTrekEngine::loadShipDescriptor(R3 *r3, const Common::String &name) {
	Common::MemoryReadStreamEndian *file = _resource->loadFile(name + ".dsc", 0, false);
	if (!file) {
		warning("Ship descriptor %s.dsc not found", name.c_str());
		return;
	}

	r3->field1e = 2;
	r3->field20 = file->readSint16(); // Shape
	r3->field22 = file->readSint16(); // Radius
	r3->field24 = file->readSint16(); // Size

	for (int i = 0; i < ARRAYSIZE(r3->systems); i++) {
		r3->systems[i].present = file->readSint16();
		r3->systems[i].maxStrength = file->readSint16();
		r3->systems[i].strength = file->readSint16();
	}

	r3->maxTurnRate = file->readSint32();
	r3->turnAcceleration = file->readSint32();
	r3->maxSpeed = file->readSint32();
	r3->speedAcceleration = file->readSint32();
	r3->commandedSpeed = file->readSint32();
	for (int i = 0; i < 3; i++)
		r3->targetTurnRate[i] = file->readSint32();
	r3->targetSpeed = file->readSint32();
	r3->currentSpeed = file->readSint32();
	for (int i = 0; i < 3; i++)
		r3->turnRate[i] = file->readSint32();
	for (int i = 0; i < 3; i++)
		r3->turnAngle[i] = file->readSint32();
	r3->field140 = file->readSint16();
	// TODO: The rest of the file describes weapons etc.
	delete file;
}

// Indices into the 25 heights of a planet, for each color scheme
static const byte kPlanetColors[][25] = {
	{ 0x73, 0x6e, 0x69, 0x64, 0xa2, 0xa3, 0xa4, 0xa5, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xcb, 0xca, 0xc9, 0xc8, 0xc7, 0xc6, 0xc5, 0xc4, 0xc3 },
	{ 0x63, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xc8, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0xa7, 0x9d, 0x9e, 0xaf, 0xae, 0xad },
	{ 0x8c, 0x8b, 0x8a, 0x89, 0x88, 0x87, 0x86, 0x85, 0x84, 0x83, 0x82, 0x81, 0x80, 0x7f, 0x80, 0x81, 0x82, 0x1d, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae },
	{ 0x78, 0xd8, 0xd9, 0xda, 0xdb, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9c, 0x9d, 0x9e, 0x9f, 0x22, 0xa9, 0xab, 0xac, 0xad },
	{ 0x8b, 0x89, 0x87, 0x86, 0x85, 0x84, 0x83, 0x07, 0x23, 0x0a, 0x0b, 0x28, 0x2e, 0x5f, 0x5a, 0xb0, 0xb1, 0x55, 0xb4, 0x38, 0x25, 0x22, 0xa9, 0xab, 0xac },
	{ 0x8a, 0x88, 0x86, 0x4a, 0x48, 0x49, 0x46, 0x45, 0x41, 0x3f, 0x05, 0x24, 0x0b, 0x28, 0x2e, 0x5f, 0x5a, 0x42, 0xc6, 0x25, 0x22, 0xa9, 0xab, 0xac, 0xad },
	{ 0x8c, 0x8b, 0x8a, 0x89, 0x88, 0x87, 0x86, 0x85, 0x84, 0x83, 0x82, 0x81, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x88, 0x89, 0x8a, 0x8b },
	{ 0xa5, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xbf, 0xbe, 0xbd, 0xbc, 0xbb, 0xba, 0xb9, 0xb8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xaf },
	{ 0xa4, 0xa3, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa6, 0xa5, 0xa4, 0xa3, 0xa2, 0xa3, 0xa4, 0xa3, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa6, 0xa5, 0xa4 },
	{ 0x89, 0x8a, 0x8b, 0x8c, 0x63, 0x31, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0xbf, 0xbe, 0xbd, 0xbc, 0xbb, 0xba, 0xb9, 0xb8, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xae },
	{ 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0x9e, 0x9d, 0x9c, 0x9b, 0x9a, 0x99, 0x98, 0x97, 0x96 },
	{ 0xaf, 0x01, 0x01, 0x80, 0x80, 0x01, 0x01, 0xa7, 0x01, 0x01, 0x80, 0x80, 0x01, 0x01, 0xaf, 0x01, 0x01, 0x80, 0x80, 0x01, 0x01, 0xb7, 0x01, 0x01, 0x80 },
	{ 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xce, 0xcd, 0xcc, 0xcb, 0xca, 0xc9, 0xc8, 0xc7, 0xc6 },
	{ 0xac, 0xab, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xae, 0xad, 0xac, 0xab, 0xaa, 0xab, 0xac, 0xab, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xae, 0xad, 0xac },
	{ 0xb4, 0xb3, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb6, 0xb5, 0xb4, 0xb3, 0xb2, 0xb3, 0xb4, 0xb3, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb6, 0xb5, 0xb4 },
	{ 0x79, 0x78, 0xd8, 0xd9, 0xda, 0xdb, 0xb0, 0xdb, 0xda, 0xd9, 0xd8, 0x78, 0x79, 0x78, 0xd8, 0xd9, 0xda, 0xdb, 0xb0, 0xdb, 0xda, 0xd9, 0xd8, 0x78, 0x79 },
	{ 0x8c, 0x8c, 0x8c, 0x8c, 0x63, 0x63, 0x63, 0x31, 0x31, 0x63, 0x63, 0x63, 0x8c, 0x8c, 0x8c, 0x8c, 0x63, 0x63, 0x63, 0x31, 0x31, 0x63, 0x63, 0x63, 0x8c },
	{ 0x6e, 0x69, 0x69, 0xa1, 0xa1, 0xa1, 0xa2, 0xa1, 0xa1, 0xa1, 0x69, 0x69, 0x6e, 0x69, 0x69, 0xa1, 0xa1, 0xa1, 0xa2, 0xa1, 0xa1, 0xa1, 0x69, 0x69, 0x6e },
	{ 0xbc, 0xbb, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xbe, 0xbd, 0xbc, 0xbb, 0xba, 0xbb, 0xbc, 0xbb, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xbe, 0xbd, 0xbc },
	{ 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd0 },
	{ 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe0 },
	{ 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 0xe8 }
};

// Name, random seed and color scheme of the planet of each star system
static const struct {
	const char *name;
	uint16 seed;
	int16 colors;
} kSystemPlanets[] = {
	{ "cent",    0x1c5,  6 }, { "cam",    0x125, 10 }, { "ark7",   0x205, 11 },
	{ "harl",    0x13e, 12 }, { "harrapa", 0x39c, 18 }, { "elasi",  0x155,  7 },
	{ "digifal", 0x358,  8 }, { "strah",   0x87,  0 }, { "hrak",   0x188, 14 },
	{ "trirho",  0x283, 13 }, { "shiva",  0x340, 12 }, { "alpha",  0x155, 11 },
	{ "omega",   0x347, 10 }, { "argos",  0x3a4,  7 }, { "beta",   0x285, 14 },
	{ "sirius",  0xea,  15 }, { "sigma",  0x155, 16 }, { "castor", 0x373, 17 },
	{ "pollux",  0x2f0,  9 }, { "christ", 0xea,   9 }
};

// The random number generator of the Microsoft C library
static uint16 msRand(uint32 &state) {
	state = state * 0x343fd + 0x269ec3;
	return (state >> 16) & 0x7fff;
}

// Fractal height map of a planet, created by midpoint displacement
class PlanetHeightMap {
public:
	PlanetHeightMap(int16 size, uint16 seed) : _size(size), _state(seed) {
		_heights = new int16[size * size]();
	}

	~PlanetHeightMap() {
		delete[] _heights;
	}

	void generate() {
		subdivideLine(0, 0, _size, 0);
		subdivideLine(0, 0, 0, _size);
		subdivideSquare(0, 0, _size, _size);
	}

	int16 at(int i) const {
		return _heights[i];
	}

private:
	void subdivideLine(int16 x1, int16 y1, int16 x2, int16 y2) {
		int16 halfX = (x2 - x1) >> 1;
		int16 halfY = (y2 - y1) >> 1;
		int16 h1 = _heights[x1 * _size + y1];
		int16 h2 = _heights[(x2 & (_size - 1)) * _size + (y2 & (_size - 1))];
		int16 offset = (int16)(msRand(_state) - 0x4000) % (halfX + halfY);
		_heights[(x1 + halfX) * _size + y1 + halfY] = (h1 + h2) / 2 + offset;

		if (halfX + halfY > 1) {
			subdivideLine(x1, y1, x1 + halfX, y1 + halfY);
			subdivideLine(x1 + halfX, y1 + halfY, x2, y2);
		}
	}

	void subdivideSquare(int16 x1, int16 y1, int16 x2, int16 y2) {
		int16 halfX = (x2 - x1) >> 1;
		int16 halfY = (y2 - y1) >> 1;
		subdivideLine(x1, y1 + halfY, x2, y1 + halfY);
		if (halfX + halfY > 2) {
			subdivideLine(x1 + halfX, y1, x1 + halfX, y1 + halfY);
			subdivideLine(x1 + halfX, y1 + halfY, x1 + halfX, y2);
		}
		if (halfX + halfY > 3) {
			subdivideSquare(x1, y1, x1 + halfX, y1 + halfY);
			subdivideSquare(x1 + halfX, y1, x2, y1 + halfY);
			subdivideSquare(x1, y1 + halfY, x1 + halfX, y2);
			subdivideSquare(x1 + halfX, y1 + halfY, x2, y2);
		}
	}

	int16 _size;
	uint32 _state;
	int16 *_heights;
};

void StarTrekEngine::generatePlanetTexture(uint16 seed, int16 colors) {
	PlanetHeightMap heightMap(PLANET_TEXTURE_SIZE, seed);
	heightMap.generate();

	const int count = PLANET_TEXTURE_SIZE * PLANET_TEXTURE_SIZE;
	int16 maxHeight = -0x8000;
	int16 minHeight = 0x7fff;
	for (int i = 0; i < count; i++) {
		maxHeight = MAX(maxHeight, heightMap.at(i));
		minHeight = MIN(minHeight, heightMap.at(i));
	}

	const byte *colorTable = kPlanetColors[MIN<int16>(colors, ARRAYSIZE(kPlanetColors) - 1)];
	int16 range = maxHeight - minHeight + 1;
	for (int i = 0; i < count; i++)
		_planetTexture[i] = colorTable[(int16)((heightMap.at(i) - minHeight) * 25) / range];
}

void StarTrekEngine::initPlanet(R3 *r3, const Common::String &name, uint16 seed, int16 colors) {
	r3->field1e = 1;
	r3->planetSize = 500;
	r3->planetName = name;
	r3->planetSeed = seed;
	r3->planetColors = colors;
	r3->field22 = r3->planetSize * 11;
}

void StarTrekEngine::initSystemPlanet(int system) {
	if (system >= 0 && system < ARRAYSIZE(kSystemPlanets)) {
		initPlanet(&_planetR3, kSystemPlanets[system].name, kSystemPlanets[system].seed, kSystemPlanets[system].colors);
	} else {
		uint16 seed = _randomSource.getRandomNumber(999);
		int16 colors = _randomSource.getRandomNumber(15) + 6;
		initPlanet(&_planetR3, Common::String::format("r%dc%d", seed, colors), seed, colors);
	}
}

// Creates the image of a planet as seen from the camera
void StarTrekEngine::loadPlanetView(R3 *r3) {
	if (!_planetSphereMap) {
		Common::MemoryReadStreamEndian *file = _resource->loadFile("planet.dat");
		_planetSphereMap = new byte[file->size()];
		file->read(_planetSphereMap, file->size());
		delete file;
	}

	if (_planetTextureName != r3->planetName) {
		generatePlanetTexture(r3->planetSeed, r3->planetColors);
		_planetTextureName = r3->planetName;
	}

	// Planets that are closer are drawn with more detail
	int32 threshold = (int32)_starfieldPointDivisor * r3->planetSize;
	int32 z = r3->field36.z;
	int16 size;
	uint16 tableOffset;
	if (z >= threshold) {
		size = 11;
		tableOffset = 0x4000;
		r3->field24 = r3->planetSize;
	} else if (z >= threshold / 2) {
		size = 21;
		tableOffset = 0x40f2;
		r3->field24 = r3->planetSize / 2;
	} else if (z >= threshold / 3) {
		size = 31;
		tableOffset = 0x4464;
		r3->field24 = r3->planetSize / 3;
	} else if (z >= threshold / 4) {
		size = 41;
		tableOffset = 0x4be6;
		r3->field24 = r3->planetSize / 4;
	} else {
		size = 61;
		tableOffset = 0x5908;
		r3->field24 = r3->planetSize / 6;
	}

	// Direction to the camera, relative to the planet
	Point3 relative = multPointMatrix(r3->field36, r3->matrix2.invert());
	int16 azimuth = angleAtan2((int16)relative.x, (int16)relative.z);
	int16 elevation = angleAsin(fixedDiv((int16)relative.y, (int16)r3->field54));
	if (elevation < -0x80 || elevation > 0x80) {
		int16 horizontal = vectorLength((int16)relative.x, (int16)relative.z, 0);
		elevation = angleAcos(fixedDiv(horizontal, (int16)r3->field54));
		if (relative.y < 0)
			elevation = -elevation;
	}

	int16 yaw = 0x200 - azimuth;
	r3->shipViewMatrix = initMatrix();
	r3->shipViewMatrix = multMatrix(r3->shipViewMatrix, axisRotation(1, angleSin(yaw), angleCos(yaw)));
	r3->shipViewMatrix = multMatrix(r3->shipViewMatrix, axisRotation(0, angleSin(elevation), angleCos(elevation)));

	// Offsets into the sphere map and the texture
	uint16 latitude = ((((int32)elevation << 7) + 0x200) / 0x400 & 0x7f) << 7;
	uint16 longitude = ((((int32)yaw << 7) + 0x200) / 0x400 & 0x7f) << 7;

	r3->bitmap = new Bitmap(size, size);
	r3->bitmap->xoffset = (size - 1) / 2;
	r3->bitmap->yoffset = (size - 1) / 2;

	const byte *table = _planetSphereMap + tableOffset;
	const int count = size * size;
	int i = 0;
	while (i < count) {
		uint16 entry = READ_LE_UINT16(table + i * 2) + latitude;
		if (entry & 0x8000) {
			// Pixels outside of the planet
			r3->bitmap->pixels[i++] = 0;
			while (i < count && (READ_LE_UINT16(table + i * 2) & 0x8000))
				r3->bitmap->pixels[i++] = 0;
			continue;
		}

		uint16 texel = (READ_LE_UINT16(_planetSphereMap + (entry & 0xbfff)) + longitude) & 0xbfff;
		r3->bitmap->pixels[i++] = _planetTexture[texel & (PLANET_TEXTURE_SIZE * PLANET_TEXTURE_SIZE - 1)];
	}
}

void StarTrekEngine::freePlanetView(R3 *r3) {
	delete r3->bitmap;
	r3->bitmap = nullptr;
}

// Scales the turn rate of a ship by the strength of this system
static const int kSteeringSystem = 7;

void StarTrekEngine::moveR3(R3 *r3) {
	if (r3->field1e != 3) {
		const ShipSystem &steering = r3->systems[kSteeringSystem];
		int32 factor = 0;
		if (steering.maxStrength != 0)
			factor = ((int32)steering.strength << 16) / steering.maxStrength;

		for (int i = 0; i < 3; i++) {
			int32 delta = r3->targetTurnRate[i] - r3->turnRate[i];
			if (delta == 0)
				continue;
			delta = CLIP(delta, -r3->turnAcceleration, r3->turnAcceleration);
			r3->turnRate[i] = CLIP(r3->turnRate[i] + (delta >> 16) * factor, -r3->maxTurnRate, r3->maxTurnRate);
		}

		for (int i = 0; i < 3; i++) {
			int32 steps = r3->turnRate[i] >> 16;
			if (steps == 0)
				continue;
			int16 index = ABS(steps);
			int16 sinValue = steps < 0 ? -_turnSinTable[index] : _turnSinTable[index];
			rotateMatrix(r3->matrix, i, sinValue, _turnCosTable[index]);
			r3->turnAngle[i] += _turnAngleTable[index];
		}

		r3->field140--;
		updateShipSpeed(r3, r3->targetSpeed);
	}

	r3->pos.x += r3->speed.x;
	r3->pos.y += r3->speed.y;
	r3->pos.z += r3->speed.z;
}

void StarTrekEngine::updateShipSpeed(R3 *r3, int32 targetSpeed) {
	int32 speed = targetSpeed;
	if (r3->field1e == 2) {
		r3->currentSpeed += CLIP(targetSpeed - r3->currentSpeed, -r3->speedAcceleration, r3->speedAcceleration);
		speed = r3->currentSpeed;
	}

	int16 speedInt = speed >> 16;
	r3->speed.x = fixedMult(speedInt, r3->matrix[2].x.raw());
	r3->speed.y = fixedMult(speedInt, r3->matrix[2].y.raw());
	r3->speed.z = fixedMult(speedInt, r3->matrix[2].z.raw());
}

void StarTrekEngine::distributeEnergy(R3 *r3) {
	r3->energy = r3->systems[13].strength + r3->systems[14].strength + r3->systems[15].strength;
	// TODO: Emergency power, shields, weapons and cloaking device

	int16 available = MIN<int16>(r3->energy, 70);
	int16 required = 0;
	if (r3->maxSpeed != 0)
		required = (int16)(r3->commandedSpeed * 70 / r3->maxSpeed);

	if (available < required) {
		r3->targetSpeed = r3->maxSpeed * available / 70;
		r3->energy -= available;
	} else {
		r3->targetSpeed = r3->commandedSpeed;
		r3->energy -= required;
	}
}

void StarTrekEngine::rotateR3(R3 *r3, int axis, int16 angle) {
	rotateMatrix(r3->matrix, axis, angleSin(angle), angleCos(angle));
}

void StarTrekEngine::resetShipControls(R3 *r3) {
	r3->commandedSpeed = 0;
	r3->targetTurnRate = Point3(0, 0, 0);
	r3->currentSpeed = 0;
	r3->turnRate = Point3(0, 0, 0);
}

void StarTrekEngine::repairShipSystems(R3 *r3) {
	for (int i = 0; i < ARRAYSIZE(r3->systems); i++) {
		ShipSystem &system = r3->systems[i];
		system.strength = MIN<int16>(system.strength + 1, system.maxStrength);
	}
	// TODO: Damage control of the Enterprise
}

void StarTrekEngine::normalizeShipMatrices() {
	for (int i = 0; i < NUM_SPACE_OBJECTS; i++) {
		R3 *r3 = _r3List[i];
		if (r3 != nullptr && (r3->field1e == 1 || r3->field1e == 2))
			normalizeMatrix(r3->matrix);
	}
}

void StarTrekEngine::updateCamera() {
	if (_viewDirection == -1)
		return;

	_starfieldPosition = _enterpriseR3.pos;
	_someMatrix = _enterpriseR3.matrix;

	Point_Fixed14 right = _someMatrix[0];
	Point_Fixed14 up = _someMatrix[1];
	Point_Fixed14 forward = _someMatrix[2];
	const Point_Fixed14 zero;

	switch (_viewDirection) {
	case 1:
		_someMatrix[0] = zero - right;
		_someMatrix[2] = zero - forward;
		break;
	case 2:
		_someMatrix[0] = forward;
		_someMatrix[2] = zero - right;
		break;
	case 3:
		_someMatrix[0] = zero - forward;
		_someMatrix[2] = right;
		break;
	case 4:
		_someMatrix[1] = zero - forward;
		_someMatrix[2] = up;
		break;
	case 5:
		_someMatrix[1] = forward;
		_someMatrix[2] = zero - up;
		break;
	default:
		break;
	}
}

void StarTrekEngine::initStarfieldPosition() {
	_starfieldPosition = Point3(0, 0, 0);
	_someMatrix = initMatrix();
}

void StarTrekEngine::initStarfield(int16 left, int16 top, int16 right, int16 bottom, int16 viewAngle) {
	// The given bounds are inclusive
	_starfieldXVar1 = (left + right) / 2;
	_starfieldYVar1 = (top + bottom) / 2;
	_starfieldXVar2 = (right - left + 1) / 2;
	_starfieldYVar2 = (bottom - top + 1) / 2;

	_starfieldRect.left = _starfieldXVar1 - _starfieldXVar2;
	_starfieldRect.right = _starfieldXVar1 + _starfieldXVar2 + 1;
	_starfieldRect.top = _starfieldYVar1 - _starfieldYVar2;
	_starfieldRect.bottom = _starfieldYVar1 + _starfieldYVar2 + 1;

	for (uint i = 0; i < ARRAYSIZE(_starList); i++) {
		_starList[i].active = false;
		_starList[i].pos = Point3(0, 0, 0);
	}

	// viewAngle is half of the horizontal field of view
	_starfieldPointDivisor = fixedDiv(_starfieldXVar2, _tanTable[viewAngle]);
	_flt_50898 = 50.0;
}

void StarTrekEngine::addR3(R3 *r3) {
	for (int i = 0; i < NUM_SPACE_OBJECTS; i++) {
		if (_r3List[i] == nullptr) {
			_r3List[i] = r3;
			return;
		}
	}

	error("addR3: out of shapes.");
}

void StarTrekEngine::delR3(R3 *r3) {
	delete r3->bitmap;
	r3->bitmap = nullptr;

	for (int i = 0; i < NUM_SPACE_OBJECTS; i++) {
		if (_r3List[i] == r3) {
			_r3List[i] = nullptr;
			r3->field1e = 0;
			return;
		}
	}

	error("delR3: shape not found.");
}

void StarTrekEngine::clearStarfieldPixels() {
	_gfx->fillBackgroundRect(_starfieldRect, 0);
}

void StarTrekEngine::drawStarfield() {
	int16 var28 = ((_starfieldXVar2 * 3) >> 1);
	int16 xvar = var28 / 2;
	int16 var2a = ((_starfieldYVar2 * 3) >> 1);
	int16 yvar = var2a / 2;
	int16 var8 = _starfieldPointDivisor << 3;

	Common::MemoryReadStreamEndian *file = _resource->loadFile("stars.shp");

	for (int i = 0; i < NUM_STARS; i++) {
		Star *star = &_starList[i];
		if ((i & 0xf) == 0) {
			file->seek(0, SEEK_SET);
		}

		if (!star->active) {
			int16 var4 = getRandomWord() / var28 - xvar;
			int16 var6 = getRandomWord() / var2a - yvar;
			Point3 point = constructPoint3ForStarfield(var4, var6, var8);
			star->pos = matrixMult(_starPositionMatrix, point) + _starfieldPosition;
			star->active = true;
		}

		Point3 p = star->pos - _starfieldPosition;
		Point3 point2 = matrixMult(p, _starPositionMatrix);

		if (point2.z > _flt_50898 && point2.z < 0x3fff
		        && abs(point2.x) < point2.z && abs(point2.y) < point2.z) {

			int16 x = _starfieldXVar1 + scaleSpacePosition(point2.x, point2.z);
			int16 y = _starfieldYVar1 - scaleSpacePosition(point2.y, point2.z);

			int fileOffset = file->pos();
			file->readUint32();
			int16 width = file->readUint16();
			int16 height = file->readUint16();

			Common::Rect starRect(x, y, x + width, y + height);
			Common::Rect drawRect = _starfieldRect.findIntersectingRect(starRect);

			file->seek(fileOffset, SEEK_SET);

			Bitmap *bitmap = new Bitmap(file, false);
			if (!drawRect.isEmpty())
				_gfx->drawBitmapToBackground(starRect, drawRect, bitmap);
			delete bitmap;
		} else {
			star->active = false;

			file->readUint32();
			int16 offset2 = file->readUint16() * file->readUint16();
			file->seek(file->pos() + offset2, SEEK_SET);
		}
	}

	delete file;
}

// Objects farther away are drawn first
bool compareR3Objects(R3 *obj1, R3 *obj2) {
	return obj1->field54 > obj2->field54;
}

void StarTrekEngine::updateStarfieldAndShips(bool arg0) {
	bool enterpriseDestroyed = false;

	_starfieldSprite.bitmapChanged = true;
	_starPositionMatrix = _someMatrix.invert();
	clearStarfieldPixels();
	drawStarfield();

	int numObjects = 0;
	for (int i = 0; i < NUM_SPACE_OBJECTS; i++) {
		R3 *r3 = _r3List[i];
		if (r3 == nullptr)
			continue;
		r3->field34 = 0;

		switch (r3->field1e) { // TODO
		case 1:
			r3 = sub_19f24(r3);
			break;
		case 2:
			// TODO: exploding ships
			r3 = sub_19f24(r3);
			break;
		case 3:
			r3 = sub_19f24(r3);
			break;
		default:
			break;
		}

		if (r3 != nullptr)
			_orderedR3List[numObjects++] = r3;
	}

	if (numObjects != 0) {
		Common::sort(_orderedR3List, _orderedR3List + numObjects, &compareR3Objects);

		for (int i = 0; i < numObjects; i++) {
			R3 *r3 = _orderedR3List[i];
			r3->field34 = 1;

			switch (r3->field1e) { // TODO
			case 1:
			case 2:
				drawR3Shape(r3);
				break;
			case 3:
				if (r3->field20 == 1) {
					// TODO
				} else
					drawR3Shape(r3);
				break;
			default:
				break;
			}
		}
	}

	if (enterpriseDestroyed) {
		showTextbox("", "#GENE\\SPACE000#The Enterprise has been destroyed!", 20, 20, TEXTCOLOR_YELLOW, 0);
		showGameOverMenu();
		// FIXME: original game manipulates stack to jump somewhere...
		return;
	}

	// TODO: static on enterprise viewscreen?
}

R3 *StarTrekEngine::sub_19f24(R3 *r3) {
	r3->matrix2 = r3->matrix;
	r3->field36 = r3->pos;
	r3->field36 -= _starfieldPosition;
	r3->field54 = vectorLength(r3->field36.x, r3->field36.y, r3->field36.z);
	r3->field36 = multPointMatrix(r3->field36, _starPositionMatrix);

	if (sub_1c022(r3)) {
		if (r3->field1e >= 1 && r3->field1e <= 3)
			r3->matrix2 = multMatrix(r3->matrix2, _starPositionMatrix);
		r3->field58 = _starfieldXVar1 + scaleSpacePosition(r3->field36.x, r3->field36.z);
		r3->field5a = _starfieldYVar1 - scaleSpacePosition(r3->field36.y, r3->field36.z);
		return r3;
	} else
		return nullptr;
}

// Returns false if the ship can't be drawn
bool StarTrekEngine::calcShipViewTransform(R3 *r3, double scale, double forward[4], double inverse[4]) {
	const Matrix &v = r3->shipViewMatrix;
	const Matrix &m = r3->matrix2;
	const Point3 &p = r3->field36;

	// Avoids an overflow in the divisions below
	int16 limit = (int16)(ABS(p.z) << 1);
	if (ABS(p.x) >= limit || ABS(p.y) >= limit)
		return false;

	int16 rows01 = ABS(fixedMult2(v[0][0].raw(), v[1][1].raw(), -v[1][0].raw(), v[0][1].raw())) / 4;
	int16 rows12 = ABS(fixedMult2(v[1][0].raw(), v[2][1].raw(), -v[2][0].raw(), v[1][1].raw())) / 2;
	int16 rows02 = ABS(fixedMult2(v[0][0].raw(), v[2][1].raw(), -v[2][0].raw(), v[0][1].raw()));
	int i = 0;
	int j = 2;
	if (rows01 > rows12) {
		if (rows01 > rows02)
			j = 1;
	} else if (rows12 > rows02) {
		i = 1;
	}

	int16 vi0 = v[i][0].raw();
	int16 vi1 = v[i][1].raw();
	int16 vj0 = v[j][0].raw();
	int16 vj1 = v[j][1].raw();
	int16 a1 = fixedMult2(vj1, m[i][0].raw(), -vi1, m[j][0].raw());
	int16 a2 = fixedMult2(vj1, m[i][1].raw(), -vi1, m[j][1].raw());
	int16 a3 = fixedMult2(vj1, m[i][2].raw(), -vi1, m[j][2].raw());
	int16 b1 = fixedMult2(-vj0, m[i][0].raw(), vi0, m[j][0].raw());
	int16 b2 = fixedMult2(-vj0, m[i][1].raw(), vi0, m[j][1].raw());
	int16 b3 = fixedMult2(-vj0, m[i][2].raw(), vi0, m[j][2].raw());

	int16 projX = fixedDiv((int16)p.x, (int16)p.z);
	int16 projY = fixedDiv((int16)p.y, (int16)p.z);
	double a = (int16)(a1 - fixedMult(a3, projX)) / 16384.0;
	double b = (int16)(a2 - fixedMult(a3, projY)) / 16384.0;
	double c = (int16)(b1 - fixedMult(b3, projX)) / 16384.0;
	double d = (int16)(b2 - fixedMult(b3, projY)) / 16384.0;

	scale /= fixedMult2(vi0, vj1, -vj0, vi1) / 16384.0;

	b = -b;
	c = -c;
	if (r3->shipViewMirrored) {
		a = -a;
		b = -b;
	}

	double inverseScale = (1.0 / scale) / (a * d - b * c);

	forward[0] = a * scale;
	forward[1] = b * scale;
	forward[2] = c * scale;
	forward[3] = d * scale;
	inverse[0] = d * inverseScale;
	inverse[1] = -b * inverseScale;
	inverse[2] = -c * inverseScale;
	inverse[3] = a * inverseScale;
	return true;
}

void StarTrekEngine::drawR3Shape(R3 *r3) {
	if (r3->field1e == 4) // TODO: explosions
		return;

	if (r3->field1e == 1)
		loadPlanetView(r3);
	else if (r3->field1e == 2)
		loadShipView(r3);

	if (r3->bitmap != nullptr) {
		double dbl68 = ((double)r3->field24 * _starfieldPointDivisor) / r3->field36.z;
		double dbl70 = 1.0 / dbl68;

		double dbl30 = 1.0;
		double dbl28 = 0.0;
		double dbl20 = 0.0;
		double dbl18 = 1.0;
		double dbl60 = 1.0;
		double dbl58 = 0.0;
		double dbl50 = 0.0;
		double dbl48 = 0.0;

		switch (r3->field1e) {
		case 1: {
			// The image is rotated around the view axis
			Matrix m = multMatrix(r3->shipViewMatrix.invert(), r3->matrix2);
			int16 roll = angleAtan2(m[0][1].raw(), m[0][0].raw());
			double cosValue = angleCos(roll) / 16384.0;
			double sinValue = angleSin(roll) / 16384.0;
			double scale = CLIP(dbl68, 0.01, 10.0);
			dbl30 = cosValue * scale;
			dbl28 = -sinValue * scale;
			dbl20 = sinValue * scale;
			dbl18 = cosValue * scale;
			dbl60 = cosValue / scale;
			dbl58 = sinValue / scale;
			dbl50 = -sinValue / scale;
			dbl48 = cosValue / scale;
			break;
		}
		case 3:
			dbl30 = dbl68;
			dbl28 = 0.0;
			dbl20 = 0.0;
			dbl18 = dbl68;
			dbl60 = dbl70;
			dbl58 = 0.0;
			dbl50 = 0.0;
			dbl48 = dbl70;
			break;
		case 4: // TODO
			break;
		case 2: {
			double forward[4], inverse[4];
			if (!calcShipViewTransform(r3, dbl68, forward, inverse)) {
				freeShipView(r3);
				return;
			}
			dbl30 = forward[0];
			dbl28 = forward[1];
			dbl20 = forward[2];
			dbl18 = forward[3];
			dbl60 = inverse[0];
			dbl58 = inverse[1];
			dbl50 = inverse[2];
			dbl48 = inverse[3];
			break;
		}
		default:
			break;
		}

		r3->field80 = dbl60;
		r3->field88 = dbl58;
		r3->field90 = dbl50;
		r3->field98 = dbl48;

		// dbl30, (bitmap->xoffset + 2), r3->field58,
		double tmp = r3->field58 - ((double)r3->bitmap->xoffset + 2) * dbl30;
		// dbl20, (bitmap->yoffset + 2), tmp
		double dbl10 = tmp - ((double)r3->bitmap->yoffset + 2) * dbl20;

		// dbl28, (bitmap->xoffset + 2), r3->field5a
		tmp = r3->field5a - ((double)r3->bitmap->xoffset + 2) * dbl28;
		// dbl18, (bitmap->yoffset + 2), tmp
		double dbl8 = tmp - ((double)r3->bitmap->yoffset + 2) * dbl18;

		// dbl60, r3->field58, bitmap->xoffset + 2
		tmp = ((double)r3->bitmap->xoffset + 2) - dbl60 * r3->field58;
		double dbl40 = tmp - dbl50 * r3->field5a;

		tmp = ((double)r3->bitmap->yoffset + 2) - dbl58 * r3->field58;
		double dbl38 = tmp - dbl48 * r3->field5a;

		double dbl3e4 = (double)r3->bitmap->width + 2;
		double dbl3ec = (double)r3->bitmap->height + 2;

		double thing[8];
		tmp = 1.0 * dbl30;
		thing[0] = 1.0 * dbl20 + tmp + dbl10;

		tmp = 1.0 * dbl28;
		thing[1] = 1.0 * dbl18 + tmp + dbl8;

		tmp = 1.0 * dbl30;
		thing[2] = dbl3ec * dbl20 + tmp + dbl10;

		tmp = 1.0 * dbl28;
		thing[3] = dbl3ec * dbl18 + tmp + dbl8;

		thing[4] = dbl3ec * dbl20 + dbl3e4 * dbl30 + dbl10;
		thing[5] = dbl3ec * dbl18 + dbl3e4 * dbl28 + dbl8;

		thing[6] = 1.0 * dbl20 + dbl3e4 * dbl30 + dbl10;
		thing[7] = 1.0 * dbl18 + dbl3e4 * dbl28 + dbl8;

		int16 *rightBounds = new int16[SCREEN_HEIGHT];
		int16 *leftBounds = new int16[SCREEN_HEIGHT];

		for (int y = _starfieldRect.top; y < _starfieldRect.bottom; y++) {
			leftBounds[y] = _starfieldRect.right;
			rightBounds[y] = _starfieldRect.left - 1;
		}

		int16 shpImageTop = _starfieldRect.bottom - 1;
		int16 shpImageBottom = _starfieldRect.top;

		for (int i = 0; i < 4; i++) {
			int16 index1 = i;
			int16 index2 = (i + 1) & 3;

			if (thing[index1 * 2 + 1] > thing[index2 * 2 + 1]) {
				index1 = index2;
				index2 = i;
			}

			int16 top = ceil(thing[index1 * 2 + 1]);
			int16 bottom = floor(thing[index2 * 2 + 1]);

			if (top > bottom)
				continue;
			if (top > _starfieldRect.bottom - 1)
				continue;
			if (bottom < _starfieldRect.top)
				continue;
			if (top < _starfieldRect.top)
				top = _starfieldRect.top;
			if (bottom > _starfieldRect.bottom - 1)
				bottom = _starfieldRect.bottom - 1;

			if (top < shpImageTop)
				shpImageTop = top;
			if (bottom > shpImageBottom)
				shpImageBottom = bottom;

			double dbl3f4;
			if (thing[index2 * 2 + 1] == thing[index1 * 2 + 1])
				dbl3f4 = 0.0;
			else
				dbl3f4 = (thing[index2 * 2] - thing[index1 * 2]) / (thing[index2 * 2 + 1] - thing[index1 * 2 + 1]);

			int32 boundDiff = (int32)(0x10000 * dbl3f4); // var3ec
			int32 boundBase = (int32)(((top - thing[index1 * 2 + 1]) * dbl3f4 + thing[index1 * 2]) * 0x10000); // var3e8

			for (int y = top; y <= bottom; y++) {
				int16 rightBound = boundBase >> 16; // var3f6
				int16 leftBound = (boundBase + 0xffff) >> 16; // var3f8

				if (leftBound < _starfieldRect.left)
					leftBound = _starfieldRect.left;
				if (leftBound < leftBounds[y])
					leftBounds[y] = leftBound;

				if (rightBound > _starfieldRect.right - 1)
					rightBound = _starfieldRect.right - 1;
				if (rightBound > rightBounds[y])
					rightBounds[y] = rightBound;

				boundBase += boundDiff;
			}
		}

		while (shpImageTop <= shpImageBottom) {
			if (leftBounds[shpImageTop] > rightBounds[shpImageTop])
				shpImageTop++;
			else
				break;
		}
		while (shpImageTop <= shpImageBottom) {
			if (leftBounds[shpImageBottom] > rightBounds[shpImageBottom])
				shpImageBottom--;
			else
				break;
		}

		if (shpImageTop <= shpImageBottom) {
			// Objects that are not close are drawn without dithering
			bool var3fa;
			if (r3->field1e == 2)
				var3fa = !_shipDitheringEnabled || ((r3->field24 * _starfieldPointDivisor) << 1) / 5 <= r3->field36.z;
			else
				var3fa = ((r3->field24 * _starfieldPointDivisor) << 1) / 3 <= r3->field36.z;

			uint16 ditherSeed = 0;
			if (!var3fa) {
				if (r3->field1e == 3) {
					ditherSeed = (_frameIndex << 3) ^ _frameIndex;
				} else {
					int16 sum = 0;
					for (int i = 0; i < 3; i++)
						sum += r3->shipViewMatrix[i].x.raw() + r3->shipViewMatrix[i].y.raw() + r3->shipViewMatrix[i].z.raw();
					ditherSeed = ((sum >> 3) + (sum >> 6)) ^ sum;
				}
			}

			// Amount added to X/Y positions after each pixel is drawn
			int16 xDiff = (int16)(dbl60 * 256);
			int16 yDiff = (int16)(dbl58 * 256);

			int16 var3f2 = (int16)(dbl50 * 256);
			int16 var3f4 = (int16)(dbl48 * 256);

			int16 var3f6 = var3f2 * shpImageTop + (int16)(dbl40 * 256);
			int16 var3f8 = var3f4 * shpImageTop + (int16)(dbl38 * 256);

			// The shape gets a transparent border of 2 pixels
			Bitmap tmpBitmap(256, 256);
			memset(tmpBitmap.pixels, 0, 256 * 256);

			int16 bitmapWidth = r3->bitmap->width;
			int16 bitmapHeight = r3->bitmap->height;

			if (bitmapHeight > 245)
				error("Shape height too big in drawR3Shape!");
			if (bitmapWidth > 252)
				error("Shape width too big in drawR3Shape!");

			// TODO: Ships that are hit are drawn differently (R3 fields 0x1c0-0x1c6)
			_gfx->copyRectBetweenBitmaps(&tmpBitmap, 2, 2, r3->bitmap, 0, 0, bitmapWidth, bitmapHeight);

			byte *bgPixels = _gfx->getBackgroundPixels() + shpImageTop * SCREEN_WIDTH;

			for (int y = shpImageTop; y <= shpImageBottom; y++) {
				int16 leftBound = leftBounds[y];
				int16 rowWidth = rightBounds[y] - leftBound + 1;

				int16 srcX = leftBound * xDiff + var3f6;
				int16 srcY = leftBound * yDiff + var3f8;
				var3f6 += var3f2;
				var3f8 += var3f4;

				byte *bgPixels2 = bgPixels + leftBound;
				bgPixels += SCREEN_WIDTH;

				if (rowWidth == 0)
					continue;

				// Positions in the shape are 8.8 fixed-point values
				uint16 cx = srcX;
				uint16 bx = srcY;
				byte *di = bgPixels2;

				if (var3fa) {
					cx += 0x80;
					bx += 0x80;

					for (int a = 0; a < rowWidth; a++) {
						byte b = tmpBitmap.pixels[(bx & 0xff00) + (cx >> 8)];
						cx += xDiff;
						bx += yDiff;
						if (b == 0)
							di++;
						else
							*(di++) = b;
					}
				} else {
					// Ordered dithering of the position in the shape
					static const byte kDitherOffsets[] = { 0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0 };
					int phase = (leftBound + y + ditherSeed) & 7;
					cx += kDitherOffsets[phase];
					bx += kDitherOffsets[phase];

					for (int a = 0; a < rowWidth; a++) {
						byte b = tmpBitmap.pixels[(bx & 0xff00) + (cx >> 8)];
						int nextPhase = (phase + 1) & 7;
						int16 offsetDiff = kDitherOffsets[nextPhase] - kDitherOffsets[phase];
						cx += xDiff + offsetDiff;
						bx += yDiff + offsetDiff;
						phase = nextPhase;
						if (b == 0)
							di++;
						else
							*(di++) = b;
					}
				}
			}

		}

		delete[] rightBounds;
		delete[] leftBounds;
	}

	if (r3->field1e == 1)
		freePlanetView(r3);
	else if (r3->field1e == 2)
		freeShipView(r3);
}

// Checks if an object is in the view frustum. field22 is its radius.
bool StarTrekEngine::sub_1c022(R3 *r3) {
	const Point3 &p = r3->field36;
	if (p.z < _flt_50898)
		return false;

	// The original uses 32-bit products, which may overflow
	uint32 divisor = (uint32)_starfieldPointDivisor;
	uint32 radius = (uint32)r3->field22;
	int32 horizontal, vertical;

	if (p.x <= 0)
		horizontal = (int32)(divisor * ((uint32)p.x + radius) + (uint32)_starfieldXVar2 * (uint32)p.z);
	else
		horizontal = (int32)((uint32)p.z * (uint32)_starfieldXVar2 - ((uint32)p.x - radius) * divisor);

	if (p.y <= 0)
		vertical = (int32)(((uint32)p.y + radius) * divisor + (uint32)p.z * (uint32)_starfieldYVar2);
	else
		vertical = (int32)((uint32)p.z * (uint32)_starfieldYVar2 - ((uint32)p.y - radius) * divisor);

	return horizontal > 0 && vertical > 0;
}

// Indexed by R3::field20
static const char *const kShipShapeNames[] = { "ENT", "KLING", "ORION", "ROM" };

// Indexed by azimuth and elevation. Bit 15 marks mirrored views.
static const uint16 kShipViews[12][7] = {
	{ 0, 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c },
	{ 0, 0x0022, 0x0024, 0x0026, 0x0028, 0x002a, 0x000c },
	{ 0, 0x0042, 0x0044, 0x0046, 0x0048, 0x004a, 0x000c },
	{ 0, 0x0062, 0x0064, 0x0066, 0x0068, 0x006a, 0x000c },
	{ 0, 0x0082, 0x0084, 0x0086, 0x0088, 0x008a, 0x000c },
	{ 0, 0x00a2, 0x00a4, 0x00a6, 0x00a8, 0x00aa, 0x000c },
	{ 0, 0x00c2, 0x00c4, 0x00c6, 0x00c8, 0x00ca, 0x000c },
	{ 0, 0x80a2, 0x80a4, 0x80a6, 0x80a8, 0x80aa, 0x000c },
	{ 0, 0x8082, 0x8084, 0x8086, 0x8088, 0x808a, 0x000c },
	{ 0, 0x8062, 0x8064, 0x8066, 0x8068, 0x806a, 0x000c },
	{ 0, 0x8042, 0x8044, 0x8046, 0x8048, 0x804a, 0x000c },
	{ 0, 0x8022, 0x8024, 0x8026, 0x8028, 0x802a, 0x000c }
};

// Loads the view of a ship closest to the direction it is seen from
void StarTrekEngine::loadShipView(R3 *r3) {
	if (r3->field20 == 4) {
		// TODO: Explosion (BigExp%02d.shp)
		return;
	}

	if (r3->field54 < _flt_50898)
		error("object too close.");

	Point3 toViewer = Point3() - r3->field36;
	toViewer = multPointMatrix(toViewer, r3->matrix2.invert());

	int32 azimuth = angleAtan2(toViewer.x, toViewer.z);
	if (azimuth < 0)
		azimuth += 0x400;

	int32 elevation = angleAcos(fixedDiv(toViewer.y, r3->field54));
	if (elevation < 0x80 || elevation > 0x180) {
		// More precise near the poles
		int16 horizontalDistance = vectorLength(toViewer.x, toViewer.z, 0);
		elevation = angleAsin(fixedDiv(horizontalDistance, r3->field54));
		if (toViewer.y < 0)
			elevation = 0x200 - elevation;
	}

	int row = MIN<int32>((elevation * 6 + 0x100) / 0x200, 6);
	azimuth += 0x2a;
	if (azimuth >= 0x400)
		azimuth -= 0x400;
	int column = (azimuth * 12) / 0x400;

	uint16 view = kShipViews[column][row];

	freeShipView(r3);
	Common::String filename = Common::String::format("%s%02x.r3s", kShipShapeNames[r3->field20], (view >> 1) & 0x77);
	debugC(6, kDebugSpace, "loadShipView: azimuth %d, elevation %d -> %s", azimuth, elevation, filename.c_str());
	Common::MemoryReadStreamEndian *file = _resource->loadFile(filename);
	for (int i = 0; i < 3; i++) {
		r3->shipViewMatrix[i].x = Fixed14::fromRaw(file->readSint16());
		r3->shipViewMatrix[i].y = Fixed14::fromRaw(file->readSint16());
		r3->shipViewMatrix[i].z = Fixed14::fromRaw(file->readSint16());
	}
	file->skip(18); // Unused second matrix
	r3->bitmap = new Bitmap(file);

	r3->shipViewMirrored = (view & 0x8000) != 0;
	if (r3->shipViewMirrored) {
		r3->shipViewMatrix[0].y = -r3->shipViewMatrix[0].y;
		r3->shipViewMatrix[0].z = -r3->shipViewMatrix[0].z;
		r3->shipViewMatrix[1].x = -r3->shipViewMatrix[1].x;
		r3->shipViewMatrix[2].x = -r3->shipViewMatrix[2].x;
	}

	r3->shipViewCode = view;
}

void StarTrekEngine::freeShipView(R3 *r3) {
	delete r3->bitmap;
	r3->bitmap = nullptr;
}

Point3 StarTrekEngine::constructPoint3ForStarfield(int16 x, int16 y, int16 z) {
	Point3 point;
	point.z = z;
	point.y = y * z / _starfieldPointDivisor;
	point.x = x * z / _starfieldPointDivisor;

	return point;
}

Point3 StarTrekEngine::matrixMult(const Matrix &weight, const Point3 &point) {
	Point3 p;
	for (int i = 0; i < 3; i++) {
		p[i] = 0;
		for (int j = 0; j < 3; j++)
			p[i] += (int16)weight[i][j].multToInt(point[j]);
	}
	return p;
}

Point3 StarTrekEngine::matrixMult(const Point3 &point, const Matrix &weight) {
	Point3 p = Point3();
	for (int i = 0; i < 3; i++) {
		p[i] = 0;
		for (int j = 0; j < 3; j++)
			p[i] += (int16)weight[j][i].multToInt(point[j]);
	}
	return p;
}

int32 StarTrekEngine::scaleSpacePosition(int32 x, int32 z) {
	if (x == 0 || z == 0)
		return 0;
	return (x * _starfieldPointDivisor) / z;
}

Matrix StarTrekEngine::initMatrix() {
	Matrix mat;
	mat[0][0] = 1.0;
	mat[1][1] = 1.0;
	mat[2][2] = 1.0;
	return mat;
}

Matrix StarTrekEngine::initSpeedMatrixForXZMovement(int16 angle, const Matrix &matrix) {
	Matrix rotation = initMatrix();
	rotation[0].x = Fixed14::fromRaw(angleCos(angle));
	rotation[0].z = Fixed14::fromRaw(-angleSin(angle));
	rotation[2].x = Fixed14::fromRaw(angleSin(angle));
	rotation[2].z = Fixed14::fromRaw(angleCos(angle));

	return multMatrix(matrix, rotation);
}

} // End of namespace StarTrek
