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


// Room of the Nauian (DEMO1.RDF of the DOS demo)

#include "startrek/room.h"

#define OBJECT_ALIEN 8

namespace StarTrek {

extern const RoomAction demo1ActionList[] = {
	{ {ACTION_USE, OBJECT_ISTRICOR, OBJECT_ALIEN, 0, 0}, &Room::demo1UseSTricorderOnAlien },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_ALIEN, 0, 0}, &Room::demo1UseMTricorderOnAlien },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum demo1TextIds {
	TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_DEMO1_001
};

extern const RoomTextOffsets demo1TextOffsets[] = {
	{ TX_SPEAKER_MCCOY, 0, 267, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_SPEAKER_SPOCK, 0, 277, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO1_001, 0, 431, 0, 0, 0, 0, 0, 0, 0 },
	{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

extern const RoomText demo1Texts[] = {
	{ -1, Common::UNK_LANG, "" }
};

void Room::demo1UseSTricorderOnAlien() {
	showText(TX_SPEAKER_SPOCK, TX_DEMO1_001);
}

void Room::demo1UseMTricorderOnAlien() {
	showText(TX_SPEAKER_MCCOY, TX_DEMO1_001);
	endDosDemoMission();
}

}
