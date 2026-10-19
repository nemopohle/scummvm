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

#include "startrek/console.h"
#include "startrek/startrek.h"

namespace StarTrek {

static bool getKeyDirection(Common::KeyCode key, int16 *dx, int16 *dy);

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

			// These keys work everywhere
			if (event.kbd.flags & Common::KBD_CTRL) {
				if (event.kbd.keycode == Common::KEYCODE_s) {
					_sound->toggleSfx();
					break;
				} else if (event.kbd.keycode == Common::KEYCODE_m) {
					_sound->toggleMusic();
					break;
				} else if (event.kbd.keycode == Common::KEYCODE_p) {
					pauseUntilKeyPress();
					break;
				}
			}

			if (getKeyDirection(event.kbd.keycode, nullptr, nullptr) && event.kbd.keycode != _heldDirectionKey) {
				_heldDirectionKey = event.kbd.keycode;
				_directionKeyChanged = true;
			}

			// The keypad + and - are the mouse buttons
			if (queueEvents && !event.kbdRepeat
			        && (event.kbd.keycode == Common::KEYCODE_KP_PLUS || event.kbd.keycode == Common::KEYCODE_KP_MINUS)) {
				trekEvent.type = event.kbd.keycode == Common::KEYCODE_KP_PLUS ? TREKEVENT_LBUTTONDOWN : TREKEVENT_RBUTTONDOWN;
				trekEvent.mouse = _gfx->getMousePos();
				addEventToQueue(trekEvent);
			}

			_heldKey = event.kbd.keycode;
			if (queueEvents) {
				trekEvent.type = TREKEVENT_KEYDOWN;
				addEventToQueue(trekEvent);
			}
			break;

		case Common::EVENT_KEYUP:
			if (event.kbd.keycode == _heldKey)
				_heldKey = Common::KEYCODE_INVALID;
			if (event.kbd.keycode == _heldDirectionKey) {
				_heldDirectionKey = Common::KEYCODE_INVALID;
				_directionKeyChanged = true;
			}
			break;

		default:
			break;
		}
	}

	// WORKAROUND: this improves the responsiveness of the mouse. A fast mouse
	// sends many events, so the screen is updated once for all of them.
	if (mouseMoved) {
		_gfx->updateLockedMouse();
		_system->updateScreen();
	}

	updateKeyboardMouse(queueEvents);
}

static bool getKeyDirection(Common::KeyCode key, int16 *dx, int16 *dy) {
	int16 x, y;
	switch (key) {
	case Common::KEYCODE_HOME:
	case Common::KEYCODE_KP7:
		x = -1;
		y = -1;
		break;
	case Common::KEYCODE_UP:
	case Common::KEYCODE_KP8:
		x = 0;
		y = -1;
		break;
	case Common::KEYCODE_PAGEUP:
	case Common::KEYCODE_KP9:
		x = 1;
		y = -1;
		break;
	case Common::KEYCODE_LEFT:
	case Common::KEYCODE_KP4:
		x = -1;
		y = 0;
		break;
	case Common::KEYCODE_RIGHT:
	case Common::KEYCODE_KP6:
		x = 1;
		y = 0;
		break;
	case Common::KEYCODE_END:
	case Common::KEYCODE_KP1:
		x = -1;
		y = 1;
		break;
	case Common::KEYCODE_DOWN:
	case Common::KEYCODE_KP2:
		x = 0;
		y = 1;
		break;
	case Common::KEYCODE_PAGEDOWN:
	case Common::KEYCODE_KP3:
		x = 1;
		y = 1;
		break;
	default:
		return false;
	}
	if (dx)
		*dx = x;
	if (dy)
		*dy = y;
	return true;
}

void StarTrekEngine::updateKeyboardMouse(bool queueEvents) {
	// The recorded input of the DOS demo moves the mouse
	if (_demoEvents)
		return;

	int16 dx = 0, dy = 0;
	getKeyDirection(_heldDirectionKey, &dx, &dy);
	Common::Point pos;

	if (_gameMode == GAMEMODE_BRIDGE && _mouseControllingShip) {
		// The keys turn the ship as fast as possible, releasing them stops turning
		if (_heldDirectionKey == Common::KEYCODE_INVALID && !_directionKeyChanged)
			return;
		const Common::Rect &r = _starfieldRect;
		pos.x = (r.left + r.right - 1) / 2 + dx * 2 * ((r.right - 1 - r.left) / 4);
		pos.y = (r.top + r.bottom - 1) / 2 + dy * 2 * ((r.bottom - 1 - r.top) / 4);
		if (!_directionKeyChanged && pos == _gfx->getMousePos())
			return;
		_directionKeyChanged = false;
	} else {
		// The mouse moves once per tick, faster the longer the key is held
		if (!_keyboardControlsMouse || _heldDirectionKey == Common::KEYCODE_INVALID || _keyboardMouseTick == _clockTicks)
			return;
		_keyboardMouseTick = _clockTicks;

		int16 step;
		if (_directionKeyChanged) {
			_directionKeyChanged = false;
			_directionKeyTick = _clockTicks;
			step = 1;
		} else if (_clockTicks > _directionKeyTick + 0x40) {
			step = 0x80;
		} else {
			int16 n = (_clockTicks - _directionKeyTick) / 4;
			step = n * ((n + 1) / 2);
		}

		pos = _gfx->getMousePos();
		pos.x = CLIP<int16>(pos.x + dx * step, 0, SCREEN_WIDTH - 1);
		pos.y = CLIP<int16>(pos.y + dy * step, 0, SCREEN_HEIGHT - 1);
	}

	_gfx->warpMouse(pos.x, pos.y);
	if (queueEvents) {
		TrekEvent trekEvent;
		trekEvent.type = TREKEVENT_MOUSEMOVE;
		trekEvent.mouse = pos;
		addEventToQueue(trekEvent);
	}
}

// Like in DOS, Ctrl+P waits for a key press
void StarTrekEngine::pauseUntilKeyPress() {
	while (!shouldQuit()) {
		Common::Event event;
		while (_eventMan->pollEvent(event)) {
			if (event.type == Common::EVENT_KEYDOWN
			        && (event.kbd.keycode < Common::KEYCODE_NUMLOCK || event.kbd.keycode > Common::KEYCODE_COMPOSE))
				return;
		}
		_system->updateScreen();
		_system->delayMillis(10);
	}
}

uint32 StarTrekEngine::getNextTickMillis() const {
	// The BIOS clock ticks 1193182 / 65536 times per second
	uint64 ticks = (uint64)_frameStartMillis * 1193182 / 65536000 + 1;
	return (ticks * 65536000 + 1193181) / 1193182;
}

void StarTrekEngine::waitForNextTick(bool queueEvents) {
	// The recorded input of the DOS demo has its own ticks
	if (_demoEvents)
		queueEvents = false;
	pollEvents(queueEvents);

	uint32 nextFrame = getNextTickMillis();
	uint32 millis = _system->getMillis();

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
		uint32 nextFrame = getNextTickMillis();
		uint32 millis = _system->getMillis();

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

		updateSmoothSpace();

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

// Mouse movements from before a warp of the mouse would move it back
void StarTrekEngine::discardMouseMoveEvents() {
	pollEvents(false);
	for (Common::List<TrekEvent>::iterator i = _eventQueue.begin(); i != _eventQueue.end();) {
		if (i->type == TREKEVENT_MOUSEMOVE)
			i = _eventQueue.erase(i);
		else
			++i;
	}
	_mouseMoveEventInQueue = false;
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
