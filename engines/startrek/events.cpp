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
 */

// ST_DEBUG_HARNESS BEGIN (temporary, do not commit)
#define FORBIDDEN_SYMBOL_EXCEPTION_getenv
#include <stdlib.h>
#include "common/file.h"
#include "graphics/paletteman.h"
#include "graphics/surface.h"
#include "image/png.h"
// ST_DEBUG_HARNESS END
#include "startrek/console.h"
#include "startrek/startrek.h"

namespace StarTrek {

// ST_DEBUG_HARNESS BEGIN (temporary, do not commit)
void stDebugHarness(OSystem *sys, Common::EventManager *em) {
	static uint32 lastDump = 0;
	static int scriptPos = 0;
	uint32 now = sys->getMillis();
	const char *dir = getenv("ST_DUMP_DIR");
	const char *dumpMsStr = getenv("ST_DUMP_MS");
	uint32 dumpMs = dumpMsStr ? atoi(dumpMsStr) : 2000;
	if (dir && now - lastDump >= dumpMs) {
		lastDump = now;
		::Graphics::Surface *s = sys->lockScreen();
		if (s) {
			byte pal[256 * 3];
			if (s->format.bytesPerPixel == 1)
				sys->getPaletteManager()->grabPalette(pal, 0, 256);
			Common::DumpFile f;
			if (f.open(Common::Path(Common::String::format("%s/f%06u.png", dir, now))))
				Image::writePNG(f, *s, s->format.bytesPerPixel == 1 ? pal : nullptr);
			sys->unlockScreen();
		}
	}
	// ST_SCRIPT: comma separated list of <ms>:<action>, action = k<ascii> | c<x>x<y> | r<x>x<y>
	const char *script = getenv("ST_SCRIPT");
	if (!script)
		return;
	Common::String sc(script);
	int idx = 0;
	uint start = 0;
	while (start < sc.size()) {
		int end = sc.findFirstOf(',', start);
		if (end < 0) end = sc.size();
		Common::String item = sc.substr(start, end - start);
		start = end + 1;
		if (idx++ < scriptPos)
			continue;
		int colon = item.findFirstOf(':');
		uint32 t = atoi(item.substr(0, colon).c_str());
		if (now < t)
			return;
		scriptPos++;
		Common::String act = item.substr(colon + 1);
		Common::Event ev;
		if (act[0] == 'k') {
			int code = atoi(act.c_str() + 1);
			ev.type = Common::EVENT_KEYDOWN;
			ev.kbd = Common::KeyState((Common::KeyCode)code, code);
			em->pushEvent(ev);
			debug("ST_SCRIPT %u: key %d", now, code);
		} else if (act[0] == 'c' || act[0] == 'r' || act[0] == 'm') {
			int x = atoi(act.c_str() + 1);
			int y = atoi(act.c_str() + act.findFirstOf('x') + 1);
			sys->warpMouse(x, y);
			ev.mouse = Common::Point(x, y);
			ev.type = Common::EVENT_MOUSEMOVE;
			em->pushEvent(ev);
			if (act[0] != 'm') {
				ev.type = act[0] == 'c' ? Common::EVENT_LBUTTONDOWN : Common::EVENT_RBUTTONDOWN;
				em->pushEvent(ev);
				ev.type = act[0] == 'c' ? Common::EVENT_LBUTTONUP : Common::EVENT_RBUTTONUP;
				em->pushEvent(ev);
			}
			debug("ST_SCRIPT %u: %c %d,%d", now, act[0], x, y);
		}
		return;
	}
}
// ST_DEBUG_HARNESS END

void StarTrekEngine::pollEvents(bool queueEvents) {
	Common::Event event;
	TrekEvent trekEvent;
	bool mouseMoved = false;
	while (_eventMan->pollEvent(event)) {
		trekEvent.mouse = event.mouse;
		trekEvent.kbd = event.kbd;

		switch (event.type) {
		case Common::EVENT_QUIT:
			_system->quit();
			break;

		case Common::EVENT_MOUSEMOVE:
			// The mouse stays inside of the viewscreen while steering the ship
			if (_gameMode == GAMEMODE_BRIDGE && _mouseControllingShip) {
				restrictBridgeMouse();
				trekEvent.mouse = _gfx->getMousePos();
			}
			if (queueEvents) {
				trekEvent.type = TREKEVENT_MOUSEMOVE;
				addEventToQueue(trekEvent);
			}
			mouseMoved = true;
			break;

		case Common::EVENT_LBUTTONDOWN:
			if (queueEvents) {
				trekEvent.type = TREKEVENT_LBUTTONDOWN;
				addEventToQueue(trekEvent);
			}
			break;

		case Common::EVENT_RBUTTONDOWN:
			if (queueEvents) {
				trekEvent.type = TREKEVENT_RBUTTONDOWN;
				addEventToQueue(trekEvent);
			}
			break;

		case Common::EVENT_KEYDOWN:
			// Like in DOS, the modifier keys alone are no key presses
			if (event.kbd.keycode >= Common::KEYCODE_NUMLOCK && event.kbd.keycode <= Common::KEYCODE_COMPOSE)
				break;
			_heldKey = event.kbd.keycode;
			if (queueEvents) {
				trekEvent.type = TREKEVENT_KEYDOWN;
				addEventToQueue(trekEvent);
			}
			break;

		case Common::EVENT_KEYUP:
			if (event.kbd.keycode == _heldKey)
				_heldKey = Common::KEYCODE_INVALID;
			break;

		default:
			break;
		}
	}

	// WORKAROUND: this improves the responsiveness of the mouse. A fast mouse
	// sends many events, so the screen is updated once for all of them.
	if (mouseMoved)
		_system->updateScreen();
}

void StarTrekEngine::waitForNextTick(bool queueEvents) {
	// The recorded input of the DOS demo has its own ticks
	if (_demoEvents)
		queueEvents = false;
	pollEvents(queueEvents);

	uint nextFrame = _frameStartMillis + 1000 / 18.206;
	uint millis = _system->getMillis();

	if (millis < nextFrame)
		_system->delayMillis(nextFrame - millis);

	_clockTicks++;
	_frameStartMillis = nextFrame;

	if (queueEvents) {
		TrekEvent tickEvent;
		tickEvent.type = TREKEVENT_TICK;
		tickEvent.tick = _clockTicks;
		addEventToQueue(tickEvent);
	}
}

void StarTrekEngine::initializeEventsAndMouse() {
	_mouseMoveEventInQueue = false;
	_tickEventInQueue = false;
	_frameStartMillis = _system->getMillis();

	// TODO: mouse
}

bool StarTrekEngine::getNextEvent(TrekEvent *e, bool poll) {
	while (poll && _eventQueue.empty()) {
		if (_demoEvents) {
			pollEvents(false);
			if (shouldQuit())
				return false;
			if (!readDemoEvent())
				_system->delayMillis(5);
			continue;
		}

		pollEvents(true);

		// Check for tick event
		uint nextFrame = _frameStartMillis + 1000 / 18.206;
		uint millis = _system->getMillis();

		if (millis >= nextFrame) {
			_clockTicks++;
			_frameStartMillis = millis;

			TrekEvent tickEvent;
			tickEvent.type = TREKEVENT_TICK;
			tickEvent.tick = _clockTicks;
			addEventToQueue(tickEvent);
		}

		if (!_eventQueue.empty())
			break;

		// Still no events; wait a 60th of a second before checking for events again
		int delay = 1000 / 60;
		millis = _system->getMillis();
		if (millis + delay > nextFrame)
			delay = nextFrame - millis;
		if (delay > 0)
			_system->delayMillis(delay);
	}

	if (_eventQueue.empty())
		return false;
	*e = _eventQueue.front();
	return true;
}

void StarTrekEngine::removeNextEvent() {
	if (_eventQueue.empty())
		return;

	const TrekEvent &e = _eventQueue.front();

	if (e.type == TREKEVENT_MOUSEMOVE)
		_mouseMoveEventInQueue = false;
	if (e.type == TREKEVENT_TICK)
		_tickEventInQueue = false;

	_eventQueue.pop_front();
}

bool StarTrekEngine::popNextEvent(TrekEvent *e, bool poll) {
	if (!getNextEvent(e, poll))
		return false;

	removeNextEvent();
	return true;
}

void StarTrekEngine::addEventToQueue(const TrekEvent &e) {
	if (e.type == TREKEVENT_MOUSEMOVE && _mouseMoveEventInQueue) {
		// Only allow one mouse move event at once
		for (Common::List<TrekEvent>::iterator i = _eventQueue.begin(); i != _eventQueue.end(); ++i) {
			if (i->type == TREKEVENT_MOUSEMOVE) {
				*i = e;
				return;
			}
		}

		error("Couldn't find mouse move event in eventQueue");
	}

	if (e.type == TREKEVENT_TICK) {
		// Only allow one tick event at once
		if (_tickEventInQueue)
			return;
		_tickEventInQueue = true;
	}

	if (e.type == TREKEVENT_MOUSEMOVE)
		_mouseMoveEventInQueue = true;

	assert(_eventQueue.size() < 0x40);
	_eventQueue.push_back(e);
}

} // End of namespace StarTrek
