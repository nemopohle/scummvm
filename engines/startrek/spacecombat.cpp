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
#include "startrek/sound.h"
#include "startrek/startrek.h"

namespace StarTrek {

// Ship systems hit on each side, by a random percentage
struct HitArea {
	byte low;
	byte high;
	byte system;
};

static const HitArea kHitAreasStarboard[] = {
	{ 0, 19, 15 }, { 20, 24, 14 }, { 25, 34, 13 }, { 35, 39, 7 }, { 40, 49, 12 },
	{ 50, 51, 10 }, { 52, 54, 11 }, { 55, 56, 8 }, { 57, 59, 9 }, { 60, 99, 6 }
};

static const HitArea kHitAreasAft[] = {
	{ 0, 24, 15 }, { 25, 49, 14 }, { 50, 64, 13 }, { 65, 99, 6 }
};

static const HitArea kHitAreasPort[] = {
	{ 0, 19, 14 }, { 20, 24, 15 }, { 25, 34, 13 }, { 35, 39, 7 }, { 40, 49, 12 },
	{ 50, 51, 11 }, { 52, 54, 10 }, { 55, 56, 9 }, { 57, 59, 8 }, { 60, 99, 6 }
};

static const HitArea kHitAreasForward[] = {
	{ 0, 9, 14 }, { 10, 19, 15 }, { 20, 24, 7 }, { 25, 39, 12 }, { 40, 44, 11 },
	{ 45, 49, 10 }, { 50, 54, 9 }, { 55, 59, 8 }, { 60, 99, 6 }
};

static const HitArea kHitAreasUpper[] = {
	{ 0, 19, 14 }, { 20, 39, 15 }, { 40, 49, 13 }, { 50, 54, 7 }, { 55, 56, 9 },
	{ 57, 59, 8 }, { 60, 99, 6 }
};

static const HitArea kHitAreasLower[] = {
	{ 0, 14, 14 }, { 15, 29, 15 }, { 30, 34, 13 }, { 35, 44, 12 }, { 45, 49, 11 },
	{ 50, 54, 10 }, { 55, 57, 9 }, { 58, 59, 8 }, { 60, 99, 6 }
};

static const struct {
	const HitArea *areas;
	int count;
} kHitAreas[] = {
	{ kHitAreasStarboard, ARRAYSIZE(kHitAreasStarboard) },
	{ kHitAreasAft, ARRAYSIZE(kHitAreasAft) },
	{ kHitAreasPort, ARRAYSIZE(kHitAreasPort) },
	{ kHitAreasForward, ARRAYSIZE(kHitAreasForward) },
	{ kHitAreasUpper, ARRAYSIZE(kHitAreasUpper) },
	{ kHitAreasLower, ARRAYSIZE(kHitAreasLower) }
};

enum ShipSide {
	kSideStarboard,
	kSideAft,
	kSidePort,
	kSideForward,
	kSideUpper,
	kSideLower
};

enum Maneuver {
	kManeuverLoop,
	kManeuverEvade,
	kManeuverJink
};

// Only the low words of the coordinates are used, like in the original
static int32 approximateDistance(int32 x, int32 y, int32 z) {
	int16 values[3] = { (int16)x, (int16)y, (int16)z };
	for (int i = 0; i < 3; i++) {
		if (values[i] < 0)
			values[i] = -values[i];
	}
	Common::sort(values, values + 3);
	uint16 middle = ((uint32)(uint16)values[1] * 0x5555) >> 16;
	return (int16)(values[2] + middle + ((uint16)values[0] >> 2));
}

static int16 safeFixedDiv(int32 a, int32 b) {
	if ((int16)b == 0)
		return 0;
	return fixedDiv((int16)a, (int16)b);
}

static ShipSide findSide(const Point3 &v) {
	int32 x = ABS(v.x);
	int32 y = ABS(v.y);
	int32 z = ABS(v.z);
	if (y <= x && z <= x)
		return v.x < 0 ? kSidePort : kSideStarboard;
	if (x <= y && z <= y)
		return v.y < 0 ? kSideLower : kSideUpper;
	return v.z < 0 ? kSideAft : kSideForward;
}

void StarTrekEngine::spawnEnemyShip(const Common::String &name) {
	for (int i = 0; i < NUM_ENEMY_SHIPS; i++) {
		R3 *ship = &_enemyShips[i];
		if (ship->field1e != 0)
			continue;

		*ship = R3();
		ship->pos = _enterpriseR3.pos;
		for (int j = 0; j < 3; j++)
			ship->pos[j] += (int16)((0x40 - (msRand() & 0x7f)) << 4);
		loadShipDescriptor(ship, name);
		ship->matrix = initMatrix();
		ship->shieldsOn = true;
		ship->weaponsOn = true;
		ship->cloakLevel = 1;
		addR3(ship);
		return;
	}
}

bool StarTrekEngine::hasEnemyShips() {
	for (int i = 0; i < NUM_ENEMY_SHIPS; i++) {
		if (_enemyShips[i].field1e != 0)
			return true;
	}
	return false;
}

void StarTrekEngine::updateEnemyShips() {
	for (int i = 0; i < NUM_ENEMY_SHIPS; i++) {
		if (_enemyShips[i].field1e != 0)
			updateEnemyShip(&_enemyShips[i]);
	}
	_enterpriseState.underAttack = hasEnemyShips();
}

void StarTrekEngine::updateEnemyShip(R3 *ship) {
	if (ship->exploding) {
		moveR3(ship);
		return;
	}

	distributeEnergy(ship);
	if ((_frameIndex & 0x1f) == 7)
		repairShipSystems(ship);
	if (ship->canCloak)
		ship->cloakRequested = true;

	// Damaged sensors confuse the ship
	const ShipSystem &sensors = ship->systems[12];
	int16 confusion = (int16)(sensors.maxStrength * 3) >> 2;
	if (confusion > 0 && (int16)(msRand() % confusion) > sensors.strength) {
		moveR3(ship);
		return;
	}

	Point3 direction = _enterpriseR3.pos - ship->pos;
	Point3 local = multPointMatrix(direction, ship->matrix.invert());
	int32 distance = approximateDistance(direction.x, direction.y, direction.z);
	ship->enterpriseDistanceChange = distance - ship->enterpriseDistance;
	ship->enterpriseDistance = distance;

	if (!ship->canCloak) {
		fireEnemyWeapons(ship, local, direction);
	} else {
		// Uncloak when facing the Enterprise
		if ((MAX(ABS(local.x), ABS(local.y)) << 2) < local.z)
			ship->cloakRequested = false;
		if (ship->cloakLevel == 0)
			fireEnemyWeapons(ship, local, direction);
	}

	if (ship->retreating) {
		if (distance > 12000) {
			delR3(ship);
			return;
		}
		int16 pitch = angleAsin(safeFixedDiv(local.y, distance));
		int16 yaw = -angleAtan2((int16)local.x, (int16)local.z);
		ship->commandedSpeed = ship->maxSpeed;
		ship->targetTurnRate = Point3((int32)pitch << 16, (int32)yaw << 16, 0);
		moveR3(ship);
		return;
	}

	int32 speed;
	if (ship->targetSpeed < (ship->commandedSpeed >> 2)) {
		speed = 0;
		ship->maneuverActive = false;
	} else {
		if (ship->maneuverActive) {
			updateManeuver(ship);
			return;
		}

		int32 range = ship->engageRange;
		if (distance <= range) {
			ship->maneuverActive = true;
			ship->maneuver = msRand() % 3;
			ship->maneuverState = 0;
			return;
		}

		if (distance > range * 3)
			speed = ship->maxSpeed;
		else if (distance > range * 2)
			speed = ship->maxSpeed >> 1;
		else
			speed = ship->maxSpeed >> 2;
	}

	// Turn towards the Enterprise
	int16 pitch = -angleAsin(safeFixedDiv(local.y, distance));
	int16 yaw = angleAtan2((int16)local.x, (int16)local.z);
	ship->commandedSpeed = speed;
	ship->targetTurnRate = Point3((int32)pitch << 16, (int32)yaw << 16, 0);
	moveR3(ship);
}

int32 StarTrekEngine::randomTurnRate(R3 *ship) {
	return (msRand() & 1) ? -ship->maxTurnRate : ship->maxTurnRate;
}

void StarTrekEngine::updateManeuver(R3 *ship) {
	const int32 maxTurnRate = ship->maxTurnRate;
	const Point3 &angle = ship->turnAngle;
	// Leaving: the Enterprise moves away and is out of range
	const bool leaving = ship->enterpriseDistanceChange > 0 && ship->enterpriseDistance > ship->engageRange;

	switch (ship->maneuver) {
	case kManeuverLoop:
		switch (ship->maneuverState) {
		case 0:
			ship->commandedSpeed = ship->maxSpeed;
			ship->targetTurnRate = Point3(randomTurnRate(ship), 0, 0);
			ship->turnAngle.x = 0;
			ship->maneuverState++;
			break;
		case 1:
			if (angle.x >= (0x200 << 16)) {
				if (ship->enterpriseDistanceChange > 0)
					ship->maneuverActive = false;
				ship->maneuverState++;
				return;
			}
			break;
		case 2:
			if (angle.x >= (0x400 << 16)) {
				ship->maneuverActive = false;
				return;
			}
			break;
		default:
			break;
		}
		break;

	case kManeuverEvade:
		switch (ship->maneuverState) {
		case 0:
			ship->commandedSpeed = ship->maxSpeed;
			ship->targetTurnRate = Point3(0, 0, 0);
			ship->field140 = 0x20;
			ship->maneuverState++;
			break;
		case 1:
		case 5:
		case 7:
			if (ship->field140 < 0) {
				ship->targetTurnRate.x = -maxTurnRate;
				ship->turnAngle.x = 0;
				ship->maneuverState++;
			}
			break;
		case 2:
		case 6:
			if (angle.x >= (0x100 << 16)) {
				ship->targetTurnRate.x = 0;
				ship->field140 = 0x30;
				ship->maneuverState++;
			}
			break;
		case 3:
			if (ship->field140 < 0) {
				ship->targetTurnRate.y = randomTurnRate(ship);
				ship->turnAngle.y = 0;
				ship->maneuverState++;
			}
			break;
		case 4:
			if (angle.y >= (0x200 << 16)) {
				ship->targetTurnRate.y = 0;
				ship->field140 = 0x30;
				ship->maneuverState++;
			}
			break;
		case 8:
			ship->maneuverActive = false;
			return;
		default:
			break;
		}
		break;

	case kManeuverJink:
		switch (ship->maneuverState) {
		case 0:
			if (leaving) {
				ship->maneuverActive = false;
			} else {
				ship->commandedSpeed = ship->maxSpeed;
				ship->targetTurnRate = Point3(0, 0, 0);
				ship->field140 = msRand() & 0x3f;
				ship->maneuverState++;
			}
			break;
		case 2:
			if (leaving) {
				ship->maneuverActive = false;
			} else {
				ship->commandedSpeed = ship->maxSpeed;
				ship->targetTurnRate = Point3(randomTurnRate(ship), 0, 0);
				ship->turnAngle.x = 0;
				ship->maneuverState++;
			}
			break;
		case 4:
			if (leaving) {
				ship->maneuverActive = false;
			} else {
				ship->commandedSpeed = ship->maxSpeed;
				ship->targetTurnRate = Point3(0, randomTurnRate(ship), 0);
				ship->turnAngle.y = 0;
				ship->maneuverState++;
			}
			break;
		case 6:
			if (leaving) {
				ship->maneuverActive = false;
			} else {
				ship->commandedSpeed = ship->maxSpeed;
				ship->targetTurnRate.y = 0;
				ship->targetTurnRate.z = randomTurnRate(ship);
				ship->turnAngle.z = 0;
				ship->maneuverState++;
			}
			break;
		case 1:
		case 3:
		case 5:
		case 7: {
			bool done;
			if (ship->maneuverState == 1)
				done = ship->field140 < 0;
			else
				done = angle[(ship->maneuverState - 3) / 2] >= (0x100 << 16);
			if (done)
				ship->maneuverState = (msRand() & 3) * 2;
			break;
		}
		default:
			break;
		}
		break;

	default:
		break;
	}

	moveR3(ship);
}

void StarTrekEngine::fireEnemyWeapons(R3 *ship, const Point3 &local, const Point3 &direction) {
	// Only if the Enterprise is in front of the ship
	if ((MAX(ABS(local.x), ABS(local.y)) << 2) >= local.z)
		return;

	bool fired = false;
	for (int i = 0; i < ARRAYSIZE(ship->weapons); i++)
		fired |= fireWeapon(ship, i, &direction);
	if (fired)
		_sound->playSoundEffectIndex(kSfxPhotonTorpedoes);
}

bool StarTrekEngine::fireWeapon(R3 *ship, int weaponIndex, const Point3 *direction) {
	ShipWeapon &weapon = ship->weapons[weaponIndex];
	const ShipSystem &system = ship->systems[8 + weaponIndex];
	if (system.strength < (system.maxStrength >> 1) || weapon.nextFireTime > _frameIndex)
		return false;
	if (!system.present || !ship->weaponsActive)
		return false;

	R3 *projectile = nullptr;
	for (int i = 0; i < NUM_PROJECTILES; i++) {
		if (_projectiles[i].field1e == 0) {
			projectile = &_projectiles[i];
			break;
		}
	}
	if (!projectile)
		return false;

	*projectile = R3();
	projectile->pos = ship->pos + multPointMatrix(weapon.mount, ship->matrix);
	if (!direction) {
		projectile->matrix = ship->matrix;
	} else {
		int16 length = vectorLength((int16)direction->x, (int16)direction->y, (int16)direction->z);
		if (length == 0)
			length = 1;
		for (int i = 0; i < 3; i++)
			projectile->matrix[2][i] = Fixed14::fromRaw(fixedDiv((int16)(*direction)[i], length));
	}

	projectile->field1e = 3;
	projectile->field20 = weapon.shape;
	projectile->field22 = 60;
	projectile->field24 = 3;
	updateShipSpeed(projectile, ((int32)weapon.speed << 16) + ship->currentSpeed);
	projectile->projectileOwner = ship;
	projectile->projectileLife = weapon.lifetime;
	projectile->projectileDamage = system.maxStrength;
	switch (ship->field20) {
	case 0:
		projectile->projectileColor = 0xa0;
		break;
	case 1:
		projectile->projectileColor = 0xb8;
		break;
	case 2:
		projectile->projectileColor = 0xea;
		break;
	default:
		break;
	}

	weapon.nextFireTime = _frameIndex + weapon.cooldown;
	addR3(projectile);
	return true;
}

void StarTrekEngine::fireEnterpriseWeapons(bool torpedoes) {
	bool weaponReady = false;
	for (int i = 0; i < ARRAYSIZE(_enterpriseR3.weapons); i++) {
		if (_enterpriseR3.weapons[i].nextFireTime < _frameIndex)
			weaponReady = true;
	}
	if (!weaponReady || !_enterpriseR3.weaponsActive)
		return;

	// Aim from the center of the viewscreen through the mouse cursor
	Common::Point mouse = _gfx->getMousePos();
	Point3 aim(mouse.x - _starfieldXVar1, _starfieldYVar1 - mouse.y, _starfieldPointDivisor);
	Point3 direction = multPointMatrix(aim, _enterpriseR3.matrix);

	int first = torpedoes ? 0 : 2;
	bool fired = fireWeapon(&_enterpriseR3, first, &direction);
	fired |= fireWeapon(&_enterpriseR3, first + 1, &direction);
	if (fired)
		_sound->playSoundEffectIndex(torpedoes ? kSfxPhotonTorpedoes : kSfxPhaser2);
}

void StarTrekEngine::updateProjectiles() {
	for (int i = 0; i < NUM_PROJECTILES; i++) {
		R3 *projectile = &_projectiles[i];
		if (projectile->field1e != 3)
			continue;
		if (projectile->projectileLife-- <= 0) {
			delR3(projectile);
			continue;
		}

		// Hit flashes don't collide
		if (projectile->field20 != 3) {
			for (int j = 0; j < NUM_SPACE_OBJECTS; j++) {
				R3 *target = _r3List[j];
				if (!target || target->field1e != 2 || target == projectile->projectileOwner)
					continue;

				Point3 offset = projectile->pos - target->pos;
				int32 radius = target->field22;
				if (ABS(offset.x) > radius || ABS(offset.y) > radius || ABS(offset.z) > radius)
					continue;

				// Only shots between the Enterprise and its enemies hit
				if ((projectile->projectileOwner == &_enterpriseR3) != (target == &_enterpriseR3)) {
					projectile->field20 = 3;
					projectile->projectileLife = 8;
					projectile->field22 = 60;
					projectile->field24 = 6;
					projectile->matrix = target->matrix;
					projectile->speed = target->speed;
					applyHit(target, projectile);
					if (target != &_enterpriseR3)
						_targetShip = target;
				}
				break;
			}
		}

		moveR3(projectile);
	}
}

void StarTrekEngine::applyHit(R3 *ship, R3 *projectile) {
	Point3 local = multPointMatrix(projectile->pos - ship->pos, ship->matrix.invert());
	ShipSide side = findSide(local);
	int16 &damage = projectile->projectileDamage;

	if (ship->shieldsActive) {
		ShipSystem &shield = ship->systems[side];
		int16 strength = shield.strength;
		shield.strength -= MIN(damage, strength);
		if (shield.maxStrength != 0)
			damage -= (int16)(damage * strength) / shield.maxStrength;
	}

	if (damageShipSystem(ship, side, damage) && !ship->exploding) {
		// Track 4 of the bridge music is not a sound effect
		if (ship == &_enterpriseR3)
			_sound->playMidiMusicTracks((MidiTracks)4);
		else
			_sound->playMidiMusicTracks(MIDITRACK_3, kLoopTypeLast);
		ship->exploding = true;
		ship->explosionFrame = -1;
		ship->cloakRequested = false;
		ship->cloakActive = false;
	}
}

// Returns true if the ship is destroyed
bool StarTrekEngine::damageShipSystem(R3 *ship, int side, int16 damage) {
	int16 percentage = msRand() % 100;
	const HitArea *area = nullptr;
	for (int i = 0; i < kHitAreas[side].count; i++) {
		area = &kHitAreas[side].areas[i];
		if (area->low <= percentage && percentage <= area->high)
			break;
	}

	ShipSystem &system = ship->systems[area->system];
	int16 absorbed = MIN(damage, system.strength);
	system.strength -= absorbed;
	int16 hullDamage = damage - absorbed;
	if (hullDamage != 0) {
		ship->systems[6].strength -= hullDamage;
		if (ship->systems[6].strength < 1)
			return true;
	}
	return false;
}

void StarTrekEngine::loadProjectileView(R3 *projectile) {
	Common::String name;
	switch (projectile->field20) {
	case 0:
		name = Common::String::format("shot%d", 1 + (projectile->projectileLife & 1));
		break;
	case 2:
		name = Common::String::format("plasm%d", _frameIndex & 3);
		if ((_frameIndex & 3) == 0)
			projectile->field24++;
		break;
	case 3:
		name = Common::String::format("expl0%d", projectile->projectileLife & 3);
		if (projectile->projectileLife == 5)
			_sound->playSoundEffectIndex(kSfxShieldHit);
		break;
	default:
		return;
	}
	projectile->bitmap = new Bitmap(_resource->loadFile(name + ".shp"));
}

// Updates the leftmost and rightmost pixel of a row of a phaser beam
static void addBeamPixels(int16 *spans, const Common::Rect &rect, int16 y, int16 left, int16 right) {
	if (y < rect.top || y >= rect.bottom)
		return;
	int16 *span = spans + (y - rect.top) * 2;
	span[0] = MAX(span[0], right);
	span[1] = MIN(span[1], left);
}

static void addBeamDisc(int16 *spans, const Common::Rect &rect, int16 cx, int16 cy, int16 radius) {
	int16 y = radius;
	int16 d = 3 - 2 * radius;
	for (int16 x = 0; x < y; x++) {
		addBeamPixels(spans, rect, cy + y, cx - x, cx + x);
		addBeamPixels(spans, rect, cy - y, cx - x, cx + x);
		addBeamPixels(spans, rect, cy + x, cx - y, cx + y);
		addBeamPixels(spans, rect, cy - x, cx - y, cx + y);
		if (d < 0) {
			d += 4 * x + 6;
		} else {
			d += 4 * (x - y) + 10;
			y--;
		}
	}
}

static void addBeamEdge(int16 *spans, const Common::Rect &rect, int16 x1, int16 y1, int16 x2, int16 y2) {
	if (y1 > y2) {
		SWAP(x1, x2);
		SWAP(y1, y2);
	}
	for (int16 y = y1; y <= y2; y++) {
		int16 x = (y1 == y2) ? x1 : x1 + (int32)(x2 - x1) * (y - y1) / (y2 - y1);
		addBeamPixels(spans, rect, y, x, x);
	}
	addBeamPixels(spans, rect, y2, x2, x2);
}

void StarTrekEngine::drawPhaserBeam(R3 *projectile) {
	// The beam is 400 units long
	const Point_Fixed14 &forward = projectile->matrix2[2];
	Point3 tail = projectile->field36;
	Point3 head = projectile->field36;
	for (int i = 0; i < 3; i++) {
		int32 half = (int32)forward[i].raw() * 200 / 0x4000;
		tail[i] -= half;
		head[i] += half;
	}
	if (tail.z < 50 || head.z < 50)
		return;

	int16 radius = projectile->field22 >> 3;
	int16 tailWidth = CLIP<int16>((int32)radius * _starfieldPointDivisor / (int16)tail.z, 1, 99);
	int16 headWidth = CLIP<int16>((int32)radius * _starfieldPointDivisor / (int16)head.z, 1, 99);
	int16 tailX = _starfieldXVar1 + (int32)(int16)tail.x * _starfieldPointDivisor / (int16)tail.z;
	int16 tailY = _starfieldYVar1 - (int32)(int16)tail.y * _starfieldPointDivisor / (int16)tail.z;
	int16 headX = _starfieldXVar1 + (int32)(int16)head.x * _starfieldPointDivisor / (int16)head.z;
	int16 headY = _starfieldYVar1 - (int32)(int16)head.y * _starfieldPointDivisor / (int16)head.z;

	const Common::Rect &rect = _starfieldRect;
	int rows = rect.height();
	int16 *spans = new int16[rows * 2];
	for (int i = 0; i < rows; i++) {
		spans[i * 2] = -0x7fff;
		spans[i * 2 + 1] = 0x7fff;
	}

	addBeamDisc(spans, rect, headX, headY, headWidth);
	addBeamDisc(spans, rect, tailX, tailY, tailWidth);

	int16 angle = angleAtan2(headY - tailY, headX - tailX);
	int16 tailOffsets[2][2];
	int16 headOffsets[2][2];
	for (int i = 0; i < 2; i++) {
		int16 sideAngle = angle + (i == 0 ? 0x100 : -0x100);
		tailOffsets[i][0] = (int32)(tailWidth + 1) * angleCos(sideAngle) / 0x4000;
		tailOffsets[i][1] = (int32)(tailWidth + 1) * angleSin(sideAngle) / 0x4000;
		headOffsets[i][0] = (int32)(headWidth + 1) * angleCos(sideAngle) / 0x4000;
		headOffsets[i][1] = (int32)(headWidth + 1) * angleSin(sideAngle) / 0x4000;
	}
	addBeamEdge(spans, rect, headX + headOffsets[1][0], headY + headOffsets[1][1], tailX + tailOffsets[1][0], tailY + tailOffsets[1][1]);
	addBeamEdge(spans, rect, tailX + tailOffsets[0][0], tailY + tailOffsets[0][1], headX + headOffsets[0][0], headY + headOffsets[0][1]);

	byte *pixels = _gfx->getBackgroundPixels();
	byte color = projectile->projectileColor;

	// Fill, center line and edges
	for (int i = 0; i < rows; i++) {
		int16 right = spans[i * 2];
		int16 left = spans[i * 2 + 1];
		if (left > right || left >= rect.right || right < rect.left)
			continue;
		left = MAX<int16>(left, rect.left);
		right = MIN<int16>(right, rect.right - 1);
		memset(pixels + (rect.top + i) * SCREEN_WIDTH + left, color + 2, right - left + 1);
	}

	int16 dx = ABS(headX - tailX);
	int16 dy = -ABS(headY - tailY);
	int16 stepX = tailX < headX ? 1 : -1;
	int16 stepY = tailY < headY ? 1 : -1;
	int16 error = dx + dy;
	int16 x = tailX;
	int16 y = tailY;
	while (true) {
		if (rect.contains(x, y))
			pixels[y * SCREEN_WIDTH + x] = color;
		if (x == headX && y == headY)
			break;
		int16 error2 = error * 2;
		if (error2 >= dy) {
			error += dy;
			x += stepX;
		}
		if (error2 <= dx) {
			error += dx;
			y += stepY;
		}
	}

	for (int i = 0; i < rows; i++) {
		int16 right = spans[i * 2];
		int16 left = spans[i * 2 + 1];
		if (left > right || left >= rect.right || right < rect.left)
			continue;
		left = MAX<int16>(left, rect.left);
		right = MIN<int16>(right, rect.right - 1);
		pixels[(rect.top + i) * SCREEN_WIDTH + left] = color + 4;
		pixels[(rect.top + i) * SCREEN_WIDTH + right] = color + 4;
	}

	delete[] spans;
}

} // End of namespace StarTrek
