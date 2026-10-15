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

// Areas of the bridge background that the displays draw into
static const int16 kBridgeDisplayRects[NUM_BRIDGE_DISPLAYS][4] = {
	{  90, 15, 127,  22 }, // Phaser bars
	{ 140, 13, 179,  23 }, // Warp and speed bars
	{ 194, 15, 231,  22 }, // Torpedo bars
	{   0, 66,  62, 108 }, // Ship seen from above
	{ 257, 68, 320, 109 }  // Ship seen from the side
};

static const char *const kShipPanelNames[] = { "ent", "klg", "ori", "rom" };

// Colors of the enemy ships on the scanner
static const byte kScannerColors[] = { 0x2e, 0x78, 0x64, 0x56 };

void StarTrekEngine::initBridgeDisplays() {
	for (int i = 0; i < NUM_BRIDGE_DISPLAYS; i++) {
		const int16 *r = kBridgeDisplayRects[i];
		initStarfieldSprite(&_bridgeDisplaySprites[i], new StubBitmap(0, 0), Common::Rect(r[0], r[1], r[2], r[3]));
	}

	// A light runs along the bottom of the viewscreen
	Bitmap *light = new Bitmap(7, 2);
	memset(light->pixels, 0xda, 7 * 2);
	_gfx->addSprite(&_bridgeLightSprite);
	_bridgeLightSprite.setBitmap(light);
	_bridgeLightSprite.setXYAndPriority(73, 107, 2);
}

void StarTrekEngine::removeBridgeDisplays() {
	for (int i = 0; i < NUM_BRIDGE_DISPLAYS; i++) {
		if (_gfx->hasSprite(&_bridgeDisplaySprites[i]))
			_gfx->delSprite(&_bridgeDisplaySprites[i]);
	}
	if (_gfx->hasSprite(&_bridgeLightSprite))
		_gfx->delSprite(&_bridgeLightSprite);
	if (_gfx->hasSprite(&_scannerSprite))
		_gfx->delSprite(&_scannerSprite);
}

void StarTrekEngine::updateBridgeDisplays() {
	updateScanner();

	R3 *ship = &_enterpriseR3;
	if (_enterpriseState.targetAnalysis && _targetShip) {
		if (_targetShip->field1e == 0)
			_targetShip = nullptr;
		else
			ship = _targetShip;
	}
	drawConsoleBars();
	if ((_frameIndex & 3) == 0)
		drawShipPanels(ship);

	if ((_frameIndex & 3) == 0) {
		_bridgeLightSprite.setXYAndPriority(73 + 8 * _bridgeLightPosition, 107, 2);
		_bridgeLightPosition = (_bridgeLightPosition + 1) % 22;
	}
}

void StarTrekEngine::cycleBridgeColors() {
	if (_gfx->getPaletteFadeLevel() != 100 || (_frameIndex & 1) != 0)
		return;

	static const int kRanges[][2] = { { 0xd0, 8 }, { 0xe0, 8 }, { 0xe8, 8 }, { 0xf2, 11 } };
	for (int i = 0; i < ARRAYSIZE(kRanges); i++)
		_gfx->cyclePalette(kRanges[i][0], kRanges[i][1], (_frameIndex >> 1) % kRanges[i][1]);
}

void StarTrekEngine::updateScanner() {
	if (!_gfx->hasSprite(&_scannerSprite)) {
		Bitmap *bitmap = new Bitmap(17, 17);
		bitmap->xoffset = 8;
		bitmap->yoffset = 8;
		_gfx->addSprite(&_scannerSprite);
		_scannerSprite.setBitmap(bitmap);
		if (getFeatures() & GF_DEMO)
			_scannerSprite.setXYAndPriority(160, 141, 2);
		else
			_scannerSprite.setXYAndPriority(159, 140, 2);
	}

	// The Enterprise is in the center, ships behind it are on the outer ring
	byte *pixels = _scannerSprite.bitmap->pixels;
	memset(pixels, 0, 17 * 17);
	pixels[8 * 17 + 8] = 0x8b;
	for (int i = 0; i < NUM_ENEMY_SHIPS; i++) {
		const R3 &ship = _enemyShips[i];
		if (ship.field1e != 2 || ship.field54 < 50)
			continue;
		if (ship.cloakActive && ship.cloakLevel >= 20)
			continue;

		int16 x = ship.field36.x;
		int16 y = ship.field36.y;
		int16 angle = angleAtan2(y, x);
		int16 offAxis = 0x100 - angleAtan2((int16)ship.field36.z, vectorLength(x, y, 0));
		int16 radius = fixedMult(16, angleSin(offAxis >> 1));
		int16 px = (fixedMult(radius, angleCos(angle)) + 1) / 2 + 8;
		int16 py = (fixedMult(radius, -angleSin(angle)) + 1) / 2 + 8;
		if (px >= 0 && px < 17 && py >= 0 && py < 17)
			pixels[py * 17 + px] = kScannerColors[i % 4];
	}
	_scannerSprite.bitmapChanged = true;
}

void StarTrekEngine::drawBridgeLine(int16 x, int16 y, int16 length, byte color) {
	if (length > 0)
		memset(_gfx->getBackgroundPixels() + y * SCREEN_WIDTH + x, color, length);
}

void StarTrekEngine::drawConsoleBars() {
	static const int16 kWeaponBarPositions[][2] = { { 196, 16 }, { 194, 20 }, { 90, 16 }, { 92, 20 } };

	Bitmap *indicators = new Bitmap(_resource->loadBitmapFile("phasind"));
	byte *background = _gfx->getBackgroundPixels();

	for (int i = 0; i < 4; i++) {
		const ShipSystem &system = _enterpriseR3.systems[8 + i];
		const ShipWeapon &weapon = _enterpriseR3.weapons[i];
		int16 x = kWeaponBarPositions[i][0];
		int16 y = kWeaponBarPositions[i][1];

		// Off, unavailable, charging or ready
		int16 length = 24;
		int16 frame = 0;
		byte color = 0x81;
		if (_enterpriseR3.weaponsOn) {
			if (!_enterpriseR3.weaponsActive || system.strength < (system.maxStrength >> 1)) {
				frame = 24;
			} else {
				color = 0xb1 + (i & 1);
				if (weapon.nextFireTime <= _frameIndex) {
					frame = 16;
				} else {
					length = 24 - (int16)((int32)(weapon.nextFireTime - _frameIndex) * 24 / weapon.cooldown);
					frame = 8;
				}
			}
		}
		drawBridgeLine(x, y, length, color);
		drawBridgeLine(x + length, y, 24 - length, 0x81);

		int16 frameY = x > 160 ? 3 : 0;
		for (int row = 0; row < 3; row++) {
			memcpy(background + (y - 1 + row) * SCREEN_WIDTH + x + 27,
			       indicators->pixels + (frameY + row) * indicators->width + frame, 8);
		}
	}
	delete indicators;

	// Warp engines, with the light for emergency power
	bool emergencyPower = _emergencyPowerTicks != 0;
	for (int i = 0; i < 2; i++) {
		const ShipSystem &system = _enterpriseR3.systems[14 + i];
		int16 value = system.strength;
		if (emergencyPower)
			value += _emergencyPowerRate / 2;
		value = MIN(value, system.maxStrength);
		int16 length = system.maxStrength != 0 ? (int32)value * 34 / system.maxStrength : 0;
		for (int16 y = 13 + 3 * i; y <= 14 + 3 * i; y++) {
			drawBridgeLine(140, y, length, 0xb1);
			drawBridgeLine(140 + length, y, 34 - length, 0x81);
			drawBridgeLine(175, y, 4, emergencyPower ? 0x69 : 0x81);
		}
	}

	// Commanded and current speed
	int32 maxSpeed = _enterpriseR3.maxSpeed != 0 ? _enterpriseR3.maxSpeed : 1;
	int16 commanded = MAX<int32>(_enterpriseR3.commandedSpeed * 25 / maxSpeed, 0);
	drawBridgeLine(143, 20, commanded, 0x69);
	drawBridgeLine(143 + commanded, 20, 25 - commanded, 0);
	int16 current = MAX<int32>(_enterpriseR3.currentSpeed * 24 / maxSpeed, 0);
	drawBridgeLine(144, 22, current, 0xba);
	drawBridgeLine(144 + current, 22, 24 - current, 0);
}

// Draws a bitmap at the position in its header
void StarTrekEngine::drawBridgeBitmap(Bitmap *bitmap) {
	Common::Rect rect(-bitmap->xoffset, -bitmap->yoffset, -bitmap->xoffset + bitmap->width, -bitmap->yoffset + bitmap->height);
	_gfx->drawBitmapToBackground(rect, rect, bitmap);
}

void StarTrekEngine::drawShipPanels(R3 *ship) {
	for (int i = 0; i < NUM_BRIDGE_DISPLAYS; i++)
		_bridgeDisplaySprites[i].bitmapChanged = true;

	if (ship->field20 < 0 || ship->field20 >= ARRAYSIZE(kShipPanelNames))
		return;
	const char *name = kShipPanelNames[ship->field20];

	const char *const views[] = { "t", "s" };
	for (int view = 0; view < 2; view++) {
		Bitmap *bitmap = new Bitmap(_resource->loadBitmapFile(Common::String::format("%s%s", views[view], name)));
		drawBridgeBitmap(bitmap);
		delete bitmap;
	}

	// Systems below 75% are shown in red
	for (int i = 0; i < 10; i++) {
		const ShipSystem &system = ship->systems[6 + i];
		if (system.strength * 4 >= system.maxStrength * 3)
			continue;
		for (int view = 0; view < 2; view++) {
			Bitmap *bitmap = new Bitmap(_resource->loadBitmapFile(Common::String::format("%s%s%02d", views[view], name, i)));
			drawBridgeBitmap(bitmap);
			delete bitmap;
		}
	}

	// The shields get darker when they are weakened
	for (int i = 0; i < 6; i++) {
		const ShipSystem &shield = ship->systems[i];
		byte color = 0xb7;
		if (ship->shieldsActive)
			color = 0xb0 + (int16)((shield.maxStrength - shield.strength) * 8) / (shield.maxStrength + 1);

		Bitmap *bitmap = new Bitmap(_resource->loadBitmapFile(Common::String::format("%sshd%02d", name, i)));
		for (int p = 0; p < bitmap->width * bitmap->height; p++) {
			if (bitmap->pixels[p] != 0)
				bitmap->pixels[p] = color;
		}
		drawBridgeBitmap(bitmap);
		delete bitmap;
	}
}

} // End of namespace StarTrek
