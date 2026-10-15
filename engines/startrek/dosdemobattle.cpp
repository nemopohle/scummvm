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


#include "startrek/graphics.h"
#include "startrek/resource.h"
#include "startrek/sound.h"
#include "startrek/startrek.h"

namespace StarTrek {

static const int kDemoBattleFrames = 900;

// Shields, hull, bridge, torpedoes, phasers, sensors, impulse and warp engines
static const int16 kDemoEnterpriseSystems[16] = { 99, 99, 99, 99, 99, 99, 300, 99, 15, 15, 20, 20, 99, 20, 40, 40 };
static const int16 kDemoKlingonSystems[16] = { 99, 99, 99, 99, 99, 99, 250, 99, 20, 20, 20, 20, 99, 20, 40, 40 };

// The demo presses some keys by itself
static const struct {
	int16 frame;
	Common::KeyCode key;
} kDemoBattleKeys[] = {
	{ 190, Common::KEYCODE_s }, { 220, Common::KEYCODE_w }, { 281, Common::KEYCODE_SPACE },
	{ 320, Common::KEYCODE_SPACE }, { 426, Common::KEYCODE_SPACE }, { 450, Common::KEYCODE_SPACE },
	{ 506, Common::KEYCODE_SPACE }, { 634, Common::KEYCODE_SPACE }, { 695, Common::KEYCODE_SPACE },
	{ 756, Common::KEYCODE_SPACE }
};

// The Enterprise fights two Klingon ships, which fly along fixed paths
void StarTrekEngine::playDemoBattle() {
	initDemoBattle();

	_demoBattleFrame = 0;
	while (_demoBattleFrame < kDemoBattleFrames && !shouldQuit()) {
		TrekEvent event;
		if (!popNextEvent(&event))
			continue;

		if (event.type == TREKEVENT_TICK) {
			updateDemoBattle();
			_demoBattleFrame++;
		} else {
			handleDemoBattleEvent(event);
		}
	}

	cleanupDemoBattle();
}

void StarTrekEngine::initDemoBattle() {
	static const struct {
		int actor;
		const char *anim;
	} kCrew[] = {
		{ 0, "kirk" }, { 1, "spock" }, { 5, "uhura" }, { 4, "chek" }, { 3, "sulu" }, { 6, "scott" }
	};

	_gfx->loadPalette("bridge");
	_gfx->setBackgroundImage("bridge");
	_gfx->loadPri("bridge");
	_gfx->copyBackgroundScreen();
	_gfx->setMouseBitmap("cursor");
	_gfx->toggleMouse(true);
	_sound->playDemoMusic("sta8v1");

	initStarfieldPosition();
	initStarfield(72, 30, 247, 102, 0x40);
	initStarfieldSprite(&_starfieldSprite, new StubBitmap(0, 0), _starfieldRect);

	for (int i = 0; i < ARRAYSIZE(kCrew); i++)
		loadActorAnim(kCrew[i].actor, kCrew[i].anim, 0, 0, 1.0);

	// The ship panels at the sides are drawn into the background
	initStarfieldSprite(&_bridgeDisplaySprites[0], new StubBitmap(0, 0), Common::Rect(0, 66, 62, 108));
	initStarfieldSprite(&_bridgeDisplaySprites[1], new StubBitmap(0, 0), Common::Rect(257, 68, 320, 109));

	// A light runs along the bottom of the viewscreen
	Bitmap *light = new Bitmap(7, 2);
	memset(light->pixels, 0xda, 7 * 2);
	_gfx->addSprite(&_bridgeLightSprite);
	_bridgeLightSprite.setBitmap(light);
	_bridgeLightSprite.setXYAndPriority(73, 107, 2);
	_bridgeLightPosition = 0;

	initDemoShip(&_enterpriseR3, 0, 450, kDemoEnterpriseSystems);
	_enterpriseR3.speedAcceleration = 4 << 16;
	_enterpriseR3.weapons[2].mount = Point3(-20, -20, 50);
	_enterpriseR3.weapons[3].mount = Point3(20, -20, 50);

	_enterpriseState.shields = false;
	_enterpriseState.weapons = false;
	_demoFireTime = 0;
	_demoShieldMessageTime = 0;
	_demoMessages.clear();
	_demoMessageEnd = -1;
}

void StarTrekEngine::cleanupDemoBattle() {
	if (_demoMessageEnd != -1)
		_gfx->delSprite(&_demoMessageSprite);
	_demoMessages.clear();

	removeDrawnActorsFromScreen();
	removeProjectiles();
	for (int i = 0; i < 2; i++) {
		if (_enemyShips[i].field1e != 0)
			delR3(&_enemyShips[i]);
		_enemyShips[i] = R3();
	}
	for (int i = 0; i < 2; i++)
		_gfx->delSprite(&_bridgeDisplaySprites[i]);
	_gfx->delSprite(&_bridgeLightSprite);
	if (_gfx->hasSprite(&_scannerSprite))
		_gfx->delSprite(&_scannerSprite);
	_gfx->delSprite(&_starfieldSprite);

	_gfx->fadeoutScreen();
	_gfx->toggleMouse(false);
	_sound->stopDemoMusic();
	clearDemoScreen();

	delR3(&_enterpriseR3);
	initDemoEnterprise();
}

void StarTrekEngine::initDemoShip(R3 *ship, int16 shape, int16 size, const int16 *strengths) {
	ship->field1e = 2;
	ship->field20 = shape;
	ship->field22 = 200;
	ship->field24 = size;
	ship->pos = Point3();
	ship->matrix = initMatrix();
	ship->speed = Point3_Short();
	for (int i = 0; i < 16; i++) {
		ship->systems[i].present = 1;
		ship->systems[i].maxStrength = strengths[i];
		ship->systems[i].strength = strengths[i];
	}

	ship->maxSpeed = 30 << 16;
	ship->commandedSpeed = 0;
	ship->targetSpeed = 0;
	ship->currentSpeed = 0;
	ship->turnRate = Point3();

	// Only the phasers are used
	for (int i = 2; i < 4; i++) {
		ShipWeapon &weapon = ship->weapons[i];
		weapon.nextFireTime = 0;
		weapon.lifetime = 50;
		weapon.speed = 160;
		weapon.cooldown = 0;
		weapon.shape = 0;
	}
}

void StarTrekEngine::updateDemoBattle() {
	_enterpriseR3.shieldsOn = _enterpriseState.shields;
	_enterpriseR3.weaponsOn = _enterpriseState.weapons;
	updateCamera();

	// The viewscreen fails with damage to the sensors
	const ShipSystem &sensors = _enterpriseR3.systems[12];
	if (msRand() % sensors.maxStrength < sensors.strength) {
		updateStarfieldAndShips(false);
		updateScanner();
	} else {
		clearStarfieldPixels();
		_starfieldSprite.bitmapChanged = true;
	}

	if ((_demoBattleFrame & 0xf) == 3)
		drawDemoShipPanels();
	if ((_demoBattleFrame & 3) == 0) {
		_bridgeLightSprite.setXYAndPriority(73 + 8 * _bridgeLightPosition, 107, 2);
		_bridgeLightPosition = (_bridgeLightPosition + 1) % 22;
	}
	_gfx->drawAllSprites();
	updateDemoBridgeMessages();

	updateDemoShipEnergy(&_enterpriseR3);
	moveDemoShip(&_enterpriseR3);
	if ((_demoBattleFrame & 0x1f) == 7)
		repairDemoShip(&_enterpriseR3);
	for (int i = 0; i < 2; i++) {
		if (_enemyShips[i].field1e != 0)
			moveDemoKlingons();
	}
	updateProjectiles();
	updateActorAnimations();

	runDemoBattleScript();
	_sound->restartDemoMusic();
	if ((_demoBattleFrame & 1) == 0)
		_gfx->cyclePalette(0xd0, 8, (_demoBattleFrame >> 1) & 7);
	_gfx->incPaletteFadeLevel();

	// The Enterprise turns towards the first Klingon ship
	if (_enemyShips[0].field1e != 0) {
		Point3 local = multPointMatrix(_enemyShips[0].pos - _enterpriseR3.pos, _enterpriseR3.matrix.invert());
		if (local.x * local.x + local.y * local.y + local.z * local.z > 99 * 99)
			_enterpriseR3.turnRate.y = angleAtan2(local.x, local.z) >> 1;
	}

	for (int i = 0; i < ARRAYSIZE(kDemoBattleKeys); i++) {
		if (kDemoBattleKeys[i].frame == _demoBattleFrame)
			handleDemoBattleKey(kDemoBattleKeys[i].key);
	}
	_frameIndex++;
}

void StarTrekEngine::runDemoBattleScript() {
	switch (_demoBattleFrame) {
	case 10:
		addDemoBridgeMessage("Uhura", "Captain, I'm picking up coded Klingon messages.", 200, 150);
		break;
	case 20:
		addDemoBridgeMessage("Chekov", "Captain, there are Klingon wessels approaching.", 180, 120);
		break;
	case 120:
		_sound->playDemoJingle("sta15v2");
		waitDemoTicks(60);
		loadActorAnim(7, "klghd", 160, 102, 1.0);
		addDemoBridgeMessage("Klingon", "Prepare to die humans.", 160, 120);
		break;
	case 150:
		for (int i = 0; i < 2; i++) {
			R3 *ship = &_enemyShips[i];
			*ship = R3();
			initDemoShip(ship, 1, 381, kDemoKlingonSystems);
			ship->pos = i == 0 ? Point3(1000, 0, 3750) : Point3(-1500, 100, 2875);
			ship->shieldsOn = true;
			ship->weaponsOn = true;
			ship->weaponsActive = true;
			addR3(ship);
		}
		break;
	default:
		break;
	}
}

// The original runs this once for each Klingon ship, even when the first one is destroyed
void StarTrekEngine::moveDemoKlingons() {
	const double kAngleStep = 0.026179166666666667;
	int32 t = _demoBattleFrame - 150;
	R3 *first = &_enemyShips[0];
	R3 *second = &_enemyShips[1];

	// The first ship flies in a figure eight
	if (t > 0 && t != 540) {
		double angle = (t < 540 ? -kAngleStep : kAngleStep) * (t % 240);
		double s = sin(angle);
		double c = cos(angle);
		int16 heading;
		if (t < 540) {
			first->pos = Point3((int32)((s + 1.0) * 1000.0), 0, (int32)(c * 1750.0 + 2000.0));
			heading = angleAtan2((int16)(c * -1000.0), (int16)(s * 1750.0));
		} else {
			first->pos = Point3((int32)((s - 1.0) * 1000.0), 0, (int32)(c * 1750.0 + 2000.0));
			heading = angleAtan2((int16)(c * 1000.0), (int16)(s * -1750.0));
		}
		first->matrix = initMatrix();
		rotateR3(first, 1, heading);
	}

	if (_demoBattleFrame == 205 || _demoBattleFrame == 445)
		fireDemoKlingon(first);

	// The second one waits, and then passes the Enterprise
	if (t < 660) {
		second->pos = Point3(-1500, 100, 2875);
	} else if (t > 660) {
		int32 time = t - 660;
		second->pos = Point3((int32)(time * 13.333 - 1500.0), 100, (int32)(time * -31.9444 + 2875.0));
		second->matrix = initMatrix();
		rotateR3(second, 1, angleAtan2(13, -31));

		if (_demoBattleFrame < 885 && t % 25 == 0)
			fireDemoKlingon(second);
	}
}

void StarTrekEngine::fireDemoKlingon(R3 *ship) {
	Point3 direction = _enterpriseR3.pos - ship->pos;
	ship->weapons[2].mount = Point3(-80, -20, -50);
	ship->weapons[3].mount = Point3(80, -20, -50);
	fireWeapon(ship, 2, &direction);
	fireWeapon(ship, 3, &direction);
	_sound->playDemoJingle("sta9v1");
}

void StarTrekEngine::fireDemoEnterprise() {
	if (_demoFireTime >= _demoBattleFrame || !_enterpriseR3.weaponsActive)
		return;

	_demoFireTime = _demoBattleFrame + 20;
	fireWeapon(&_enterpriseR3, 2, nullptr);
	fireWeapon(&_enterpriseR3, 3, nullptr);
	_sound->playDemoJingle("sta10v1");
}

// The rates of turning are in steps per frame, which the arrow keys change
void StarTrekEngine::moveDemoShip(R3 *ship) {
	const ShipSystem &bridge = ship->systems[7];
	int32 maxRate = 11 * bridge.strength / bridge.maxStrength;
	for (int axis = 0; axis < 3; axis++) {
		ship->turnRate[axis] = CLIP<int32>(ship->turnRate[axis], -maxRate, maxRate);
		if (ship->turnRate[axis] != 0)
			rotateR3(ship, axis, ship->turnRate[axis]);
	}

	updateShipSpeed(ship, ship->targetSpeed);
	ship->pos.x += ship->speed.x;
	ship->pos.y += ship->speed.y;
	ship->pos.z += ship->speed.z;
}

void StarTrekEngine::updateDemoShipEnergy(R3 *ship) {
	int16 energy = ship->systems[13].strength + ship->systems[14].strength + ship->systems[15].strength;

	ship->shieldsActive = ship->shieldsOn && energy >= 10;
	if (ship->shieldsActive)
		energy -= 10;

	// Weapons that were fired recently need more energy
	int16 weaponsEnergy = _demoBattleFrame >= _demoFireTime ? 5 : 10;
	ship->weaponsActive = ship->weaponsOn && energy >= weaponsEnergy;
	if (ship->weaponsActive)
		energy -= weaponsEnergy;

	int16 required = (ship->commandedSpeed >> 16) * 70 / (ship->maxSpeed >> 16);
	if (energy < required)
		ship->targetSpeed = ((ship->maxSpeed >> 16) * energy / 70) << 16;
	else
		ship->targetSpeed = ship->commandedSpeed;
}

void StarTrekEngine::repairDemoShip(R3 *ship) {
	for (int i = 0; i < 16; i++) {
		ShipSystem &system = ship->systems[i];
		system.strength = MIN<int16>(system.strength + 1, system.maxStrength);
	}
}

void StarTrekEngine::handleDemoBattleEvent(const TrekEvent &event) {
	switch (event.type) {
	case TREKEVENT_LBUTTONDOWN:
	case TREKEVENT_RBUTTONDOWN: {
		Sprite *sprite = _gfx->getSpriteAt(_gfx->getMousePos());
		if (sprite == nullptr) {
			fireDemoEnterprise();
			break;
		}
		for (int i = 0; i < 7; i++) {
			if (sprite == &_actorList[i].sprite)
				showDemoCrewMenu(i);
		}
		break;
	}

	case TREKEVENT_KEYDOWN:
		handleDemoBattleKey(event.kbd.keycode);
		break;

	default:
		break;
	}
}

void StarTrekEngine::handleDemoBattleKey(Common::KeyCode key) {
	Point3 &rate = _enterpriseR3.turnRate;

	switch (key) {
	case Common::KEYCODE_1:
	case Common::KEYCODE_2:
	case Common::KEYCODE_3:
	case Common::KEYCODE_4:
	case Common::KEYCODE_5:
	case Common::KEYCODE_6:
	case Common::KEYCODE_7:
	case Common::KEYCODE_8:
	case Common::KEYCODE_9:
		_enterpriseR3.commandedSpeed = ((_enterpriseR3.maxSpeed >> 16) * (key - Common::KEYCODE_1) / 9) << 16;
		break;
	case Common::KEYCODE_0:
		_enterpriseR3.commandedSpeed = _enterpriseR3.maxSpeed;
		break;
	case Common::KEYCODE_UP:
	case Common::KEYCODE_KP8:
		rate.x++;
		break;
	case Common::KEYCODE_DOWN:
	case Common::KEYCODE_KP2:
		rate.x--;
		break;
	case Common::KEYCODE_LEFT:
	case Common::KEYCODE_KP4:
		rate.y--;
		break;
	case Common::KEYCODE_RIGHT:
	case Common::KEYCODE_KP6:
		rate.y++;
		break;
	case Common::KEYCODE_INSERT:
	case Common::KEYCODE_KP0:
		rate.z++;
		break;
	case Common::KEYCODE_DELETE:
	case Common::KEYCODE_KP_PERIOD:
		rate.z--;
		break;
	case Common::KEYCODE_KP5:
		rate = Point3();
		_enterpriseR3.commandedSpeed = 0;
		break;
	case Common::KEYCODE_s:
		toggleDemoShields();
		break;
	case Common::KEYCODE_w:
		toggleDemoWeapons();
		break;
	case Common::KEYCODE_SPACE:
	case Common::KEYCODE_SLASH:
		fireDemoEnterprise();
		break;
	default:
		break;
	}
}

void StarTrekEngine::toggleDemoShields() {
	_enterpriseState.shields = !_enterpriseState.shields;
	addDemoBridgeMessage("Sulu", _enterpriseState.shields ? "Raising shields." : "Lowering shields.", 100, 120);
}

void StarTrekEngine::toggleDemoWeapons() {
	_enterpriseState.weapons = !_enterpriseState.weapons;
	addDemoBridgeMessage("Chekov", _enterpriseState.weapons ? "Arming weapons." : "Disarming weapons.", 180, 120);
}

void StarTrekEngine::showDemoCrewMenu(int actor) {
	static const struct {
		const char *menu;
		int16 x;
		int16 y;
	} kMenus[] = {
		{ "command", 60, 100 }, // Kirk
		{ "comm", 200, 30 },    // Spock
		{ nullptr, 0, 0 },
		{ "nav", 60, 70 },      // Sulu
		{ "helm", 60, 70 },     // Chekov
		{ "comm", 30, 30 },     // Uhura
		{ "eng", 30, 30 }       // Scotty
	};

	if (!kMenus[actor].menu)
		return;

	loadMenuButtons(kMenus[actor].menu, kMenus[actor].x, kMenus[actor].y);
	int choice = handleMenuEvents(0, false);
	unloadMenuButtons();

	if (choice == 1 && actor == 3)
		toggleDemoShields();
	else if (choice == 1 && actor == 4)
		toggleDemoWeapons();
}

// The messages of the bridge are shown one after the other, while the battle goes on
void StarTrekEngine::addDemoBridgeMessage(const Common::String &speaker, const Common::String &text, int16 x, int16 y) {
	DemoMessage message;
	message.speaker = speaker;
	message.text = text;
	message.x = x;
	message.y = y;
	_demoMessages.push_back(message);
}

void StarTrekEngine::updateDemoBridgeMessages() {
	if (_demoMessages.empty())
		return;

	if (_demoMessageEnd == -1) {
		const DemoMessage &message = _demoMessages.front();
		_gfx->addSprite(&_demoMessageSprite);
		drawDosDemoTextbox(&_demoMessageSprite, message.speaker, message.text.c_str(), message.x, message.y);
		int lines = _demoMessageSprite.bitmap->height / 8 - 1;
		_demoMessageEnd = _demoBattleFrame + lines * 12;
	} else if (_demoBattleFrame >= _demoMessageEnd) {
		_demoMessageSprite.dontDrawNextFrame();
		_gfx->drawAllSprites();
		_gfx->delSprite(&_demoMessageSprite);
		_demoMessages.pop_front();
		_demoMessageEnd = -1;
	}
}

// Damaged systems are shown in other colors on the pictures of the Enterprise
void StarTrekEngine::drawDemoShipPanels() {
	static const struct {
		int16 topX;
		int16 topY;
		int16 sideX;
		int16 sideY;
		const char *top;
		const char *side;
	} kSystemPictures[10] = {
		{ 54, 84, 286, 91, "tent", "sent" },
		{ 41, 84, 276, 80, "tent1", "sent1" },
		{ 52, 81, 262, 83, "tent2", "sent2" },
		{ 52, 81, 262, 83, "tent2", "sent2" },
		{ 0, 0, 276, 86, nullptr, "sent3" },
		{ 0, 0, 276, 86, nullptr, "sent3" },
		{ 0, 0, 286, 89, nullptr, "sent4" },
		{ 30, 82, 295, 82, "tent5", "sent5" },
		{ 12, 74, 0, 0, "tent6", nullptr },
		{ 12, 92, 0, 0, "tent7", nullptr }
	};
	static const int16 kShieldPositions[6][2] = {
		{ 0, 99 }, { 0, 79 }, { 0, 72 }, { 42, 70 }, { 260, 83 }, { 261, 84 }
	};

	for (int i = 0; i < 10; i++) {
		const ShipSystem &system = _enterpriseR3.systems[6 + i];
		if (i != 0 && system.strength == system.maxStrength)
			continue;
		if (kSystemPictures[i].top)
			drawDemoPanelBitmap(new Bitmap(_resource->loadBitmapFile(kSystemPictures[i].top)), kSystemPictures[i].topX, kSystemPictures[i].topY);
		if (kSystemPictures[i].side)
			drawDemoPanelBitmap(new Bitmap(_resource->loadBitmapFile(kSystemPictures[i].side)), kSystemPictures[i].sideX, kSystemPictures[i].sideY);
	}

	// The shields get darker when they are weakened
	for (int i = 0; i < 6; i++) {
		const ShipSystem &shield = _enterpriseR3.systems[i];
		byte color = 0xb7;
		if (_enterpriseR3.shieldsActive)
			color = 0xb0 + (shield.maxStrength - shield.strength) * 8 / (shield.maxStrength + 1);

		Bitmap *bitmap = new Bitmap(_resource->loadBitmapFile(Common::String::format("shield%d", i)));
		for (int p = 0; p < bitmap->width * bitmap->height; p++) {
			if (bitmap->pixels[p] != 0)
				bitmap->pixels[p] = color;
		}
		drawDemoPanelBitmap(bitmap, kShieldPositions[i][0], kShieldPositions[i][1]);
	}

	for (int i = 0; i < 2; i++)
		_bridgeDisplaySprites[i].bitmapChanged = true;
}

void StarTrekEngine::drawDemoPanelBitmap(Bitmap *bitmap, int16 x, int16 y) {
	Common::Rect rect(x - bitmap->xoffset, y - bitmap->yoffset, x - bitmap->xoffset + bitmap->width, y - bitmap->yoffset + bitmap->height);
	_gfx->drawBitmapToBackground(rect, rect, bitmap);
	delete bitmap;
}

} // End of namespace StarTrek
