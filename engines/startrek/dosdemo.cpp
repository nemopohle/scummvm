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

static void moveDemoR3(R3 &r3) {
	r3.pos.x += r3.speed.x;
	r3.pos.y += r3.speed.y;
	r3.pos.z += r3.speed.z;
}

// Where an object moved by initDemoR3ObjectToMove() arrives
static Point3 getDemoR3Destination(const R3 &r3, int ticks) {
	Point3 dest = r3.pos;
	dest.x += r3.speed.x * ticks;
	dest.y += r3.speed.y * ticks;
	dest.z += r3.speed.z * ticks;
	return dest;
}

// The DOS demo of 1991 shows its parts in a loop: an intro, an away mission, a
// montage, a space battle and an ending.
void StarTrekEngine::runDosDemo() {
	_sound->startDemoMusicSetup();
	initDemoEnterprise();

	while (!shouldQuit()) {
		playDemoIntro();
		playDemoMontage();
		playDemoEnding();
	}
}

void StarTrekEngine::initDemoEnterprise() {
	_enterpriseR3 = R3();
	_enterpriseR3.matrix = initMatrix();
	_enterpriseR3.field1e = 2;
	_enterpriseR3.field20 = 0;
	_enterpriseR3.field22 = 200;
	_enterpriseR3.field24 = 3;
	addR3(&_enterpriseR3);
}

void StarTrekEngine::initDemoStarfield() {
	initStarfieldPosition();
	initStarfield(10, 16, 309, 199, 128);
	initStarfieldSprite(&_starfieldSprite, new StubBitmap(0, 0), _starfieldRect);
}

void StarTrekEngine::clearDemoScreen() {
	_gfx->clearBackgroundImage();
	_gfx->clearScreenAndPriBuffer();
}

void StarTrekEngine::waitDemoTicks(int ticks) {
	for (int i = 0; i < ticks && !shouldQuit(); i++)
		waitForNextTick();
}

// One frame of the demo: the camera moves through the starfield
void StarTrekEngine::runDemoFrame(bool fadeIn) {
	waitForNextTick();
	updateStarfieldAndShips(false);
	_gfx->drawAllSprites();
	_starfieldPosition.z += 60;
	if (fadeIn)
		_gfx->incPaletteFadeLevel();
}

void StarTrekEngine::playDemoIntro() {
	_gfx->loadPalette("bridge");
	showDemoLogo();

	initDemoStarfield();
	_sound->playDemoMusic("sta1v1");
	showDemoSubtitles("kirkspch");
	moveDemoEnterprise();
	showDemoTitle();
	_gfx->delSprite(&_starfieldSprite);
	_gfx->fadeoutScreen();

	while (_sound->isDemoMusicSetupPlaying() && !shouldQuit())
		waitForNextTick();
	_sound->playDemoMusic("sta8v1");
	showDemoLog();
	while (_sound->isDemoMusicSetupPlaying() && !shouldQuit())
		waitForNextTick();

	_enterpriseR3.pos = Point3();
	_sound->stopDemoMusic();
}

// The Interplay logo, with a highlight moving over it
void StarTrekEngine::showDemoLogo() {
	_gfx->setBackgroundImage("iplogo");
	_gfx->loadPri("iplogo");
	_gfx->copyBackgroundScreen();

	Sprite highlight;
	_gfx->addSprite(&highlight);
	highlight.setBitmap(new Bitmap(_resource->loadBitmapFile("hilite")));
	for (int x = 0; x < 300 && !shouldQuit(); x += 2) {
		_gfx->incPaletteFadeLevel();
		highlight.setXYAndPriority(x, 10, 8);
		_gfx->drawAllSprites();
		waitForNextTick();
	}
	_gfx->fadeoutScreen();
	_gfx->delSprite(&highlight);

	clearDemoScreen();
}

// Texts in the format "<duration>#<line>\n<line>*", ended by "#"
void StarTrekEngine::showDemoSubtitles(const Common::String &name) {
	Common::String text = loadDemoText(name);

	Bitmap *bitmap = new Bitmap(SCREEN_WIDTH, 18);
	Sprite sprite;
	_gfx->addSprite(&sprite);
	sprite.setBitmap(bitmap);
	sprite.setXYAndPriority(0, 100, 2);

	const char *pos = text.c_str();
	while (*pos && *pos != '#' && !shouldQuit()) {
		int duration = atoi(pos);
		while (*pos && *pos++ != '#')
			;

		memset(bitmap->pixels, 0, bitmap->width * bitmap->height);
		int y = 0;
		bool pageEnd = false;
		while (!pageEnd && *pos) {
			Common::String line;
			pos = readDemoTextLine(pos, line, 36);
			pageEnd = line.hasSuffix("*");
			if (pageEnd)
				line.deleteLastChar();
			drawTextLineToBitmap(line.c_str(), line.size(), (40 - (int)line.size()) * 8 / 2, y, bitmap);
			y += 9;
		}
		sprite.bitmapChanged = true;

		for (int i = 0; i < duration * 10 && !shouldQuit(); i++)
			runDemoFrame(true);
	}

	_gfx->delSprite(&sprite);
}

Common::String StarTrekEngine::loadDemoText(const Common::String &name) {
	Common::MemoryReadStreamEndian *stream = _resource->loadFile(name + ".txt");
	Common::String text;
	while (!stream->eos()) {
		char c = stream->readByte();
		if (stream->eos())
			break;
		text += c;
	}
	delete stream;
	return text;
}

// Reads a line without its line break, and wraps it at a space when it is too long
const char *StarTrekEngine::readDemoTextLine(const char *pos, Common::String &line, uint maxLength) {
	line.clear();
	while (*pos && *pos != '\n') {
		if (line.size() == maxLength) {
			int space = line.findLastOf(' ');
			if (space > 0) {
				pos -= line.size() - space - 1;
				line = Common::String(line.c_str(), space);
			}
			return pos;
		}
		if (*pos != '\r')
			line += *pos;
		pos++;
	}
	return *pos ? pos + 1 : pos;
}

// Moves an object of the starfield from one place to another in front of the camera,
// like initIntroR3ObjectToMove(), which has the parameters in another order
void StarTrekEngine::initDemoR3ObjectToMove(R3 *r3, int16 srcDepth, int16 srcAngle, int16 destDepth, int16 destAngle, int16 ticks) {
	initIntroR3ObjectToMove(r3, srcAngle, srcDepth, destAngle, destDepth, ticks);
}

// The Enterprise passes the camera
void StarTrekEngine::moveDemoEnterprise() {
	initDemoR3ObjectToMove(&_enterpriseR3, 6000, 330, 400, 90, 30);
	Point3 dest = getDemoR3Destination(_enterpriseR3, 30);

	for (int frame = 0; frame < 40 && !shouldQuit(); frame++) {
		runDemoFrame(false);
		if (frame < 29) {
			moveDemoR3(_enterpriseR3);
		} else if (frame == 29) {
			_enterpriseR3.pos = dest;
		}
		_enterpriseR3.pos.z += 60;
		dest.z += 60;
	}
}

// The parts of the title fly together
void StarTrekEngine::showDemoTitle() {
	static const struct {
		int16 srcDepth;
		int16 srcAngle;
	} kParts[] = {
		{ 30000, 0 }, { 6708, 300 }, { 6708, 60 }, { 0, 0 }
	};

	R3 parts[4];
	Point3 dest[4];
	for (int i = 0; i < 4; i++) {
		parts[i] = R3();
		parts[i].matrix = initMatrix();
		parts[i].bitmap = new Bitmap(_resource->loadBitmapFile(Common::String::format("logo%02d", i)));
		parts[i].field1e = 3;
		parts[i].field22 = 2000;
		parts[i].field24 = 3000 / _starfieldPointDivisor;
		initDemoR3ObjectToMove(&parts[i], kParts[i].srcDepth, kParts[i].srcAngle, 3000, 0, 20);
		dest[i] = getDemoR3Destination(parts[i], 20);
		addR3(&parts[i]);
	}

	for (int frame = 0; frame < 90 && !shouldQuit(); frame++) {
		runDemoFrame(false);
		for (int i = 0; i < 4; i++) {
			if (frame < 19)
				moveDemoR3(parts[i]);
			else if (frame == 19)
				parts[i].pos = dest[i];
			parts[i].pos.z += 60;
			dest[i].z += 60;
		}
	}

	for (int i = 0; i < 4; i++)
		delR3(&parts[i]);
}

// The captain's log, on a black screen
void StarTrekEngine::showDemoLog() {
	clearDemoScreen();

	Common::String text = loadDemoText("demolog");
	Bitmap *bitmap = new Bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
	memset(bitmap->pixels, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
	int y = 8;
	const char *pos = text.c_str();
	while (*pos) {
		Common::String line;
		pos = readDemoTextLine(pos, line, 36);
		drawTextLineToBitmap(line.c_str(), line.size(), 8, y, bitmap);
		y += 9;
	}

	Sprite sprite;
	_gfx->addSprite(&sprite);
	sprite.setBitmap(bitmap);
	sprite.setXYAndPriority(0, 0, 15);
	_gfx->drawAllSprites();

	_gfx->fadeinScreen();
	waitDemoTicks(100);
	_gfx->fadeoutScreen();

	_gfx->delSprite(&sprite);
}

// Pictures of the planets of the game
void StarTrekEngine::playDemoMontage() {
	clearDemoScreen();
	_sound->playDemoMusic("sta1v1");
	_gfx->loadPalette("palette");
	_gfx->fadeinScreen();
	showDemoSubtitles("montage");

	for (int i = 0; i < 14 && !shouldQuit(); i++) {
		_gfx->setBackgroundImage(Common::String::format("montage%x", i));
		_gfx->copyBackgroundScreen();
		_system->updateScreen();
		waitDemoTicks(50);
	}

	_gfx->fadeoutScreen();
	clearDemoScreen();
	while (_sound->isDemoMusicSetupPlaying() && !shouldQuit())
		waitForNextTick();
}

void StarTrekEngine::playDemoEnding() {
	_gfx->loadPalette("bridge");
	initDemoStarfield();
	clearDemoScreen();
	_gfx->fadeinScreen();
	showDemoTitle();
	_gfx->fadeoutScreen();
	updateStarfieldAndShips(false);
	_gfx->drawAllSprites();
	showDemoSubtitles("endseq");
	_gfx->delSprite(&_starfieldSprite);
	_gfx->fadeoutScreen();
	_enterpriseR3.pos = Point3();
}

} // End of namespace StarTrek
