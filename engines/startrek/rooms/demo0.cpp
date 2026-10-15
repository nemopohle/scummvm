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


// Square in front of the church of the colony (DEMO0.RDF of the DOS demo)

#include "startrek/room.h"

#define OBJECT_PRELATE 8

#define HOTSPOT_COLONISTS 0x20
#define HOTSPOT_SIGN 0x21
#define HOTSPOT_MOUNTAIN_1 0x24
#define HOTSPOT_MOUNTAIN_2 0x25

namespace StarTrek {

extern const RoomAction demo0ActionList[] = {
	{ {ACTION_TICK, 100, 0, 0, 0}, &Room::demo0Tick100 },

	{ {ACTION_TALK, OBJECT_PRELATE, 0, 0, 0}, &Room::demo0TalkToPrelate },
	{ {ACTION_LOOK, OBJECT_PRELATE, 0, 0, 0}, &Room::demo0LookAtPrelate },
	{ {ACTION_USE, OBJECT_IPHASERS, OBJECT_PRELATE, 0, 0}, &Room::demo0UsePhaserOnPrelate },
	{ {ACTION_LOOK, HOTSPOT_SIGN, 0, 0, 0}, &Room::demo0LookAtSign },
	{ {ACTION_LOOK, HOTSPOT_COLONISTS, 0, 0, 0}, &Room::demo0LookAtColonists },
	{ {ACTION_LOOK, HOTSPOT_MOUNTAIN_1, 0, 0, 0}, &Room::demo0LookAtMountain },
	{ {ACTION_LOOK, HOTSPOT_MOUNTAIN_2, 0, 0, 0}, &Room::demo0LookAtMountain },
	{ {ACTION_LOOK, OBJECT_SPOCK, 0, 0, 0}, &Room::demo0LookAtSpock },
	{ {ACTION_TALK, OBJECT_SPOCK, 0, 0, 0}, &Room::demo0TalkToSpock },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::demo0TalkToRedshirt },
	{ {ACTION_TALK, OBJECT_MCCOY, 0, 0, 0}, &Room::demo0TalkToMccoy },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0xff, 0, 0}, &Room::demo0UseSTricorderAnywhere },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_PRELATE, 0, 0}, &Room::demo0UseMTricorderOnPrelate },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum demo0TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_ANGIVEN, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_SMITH,
	TX_DEMO0_001, TX_DEMO0_002, TX_DEMO0_003, TX_DEMO0_004, TX_DEMO0_005,
	TX_DEMO0_006, TX_DEMO0_007, TX_DEMO0_008, TX_DEMO0_009, TX_DEMO0_010,
	TX_DEMO0_011, TX_DEMO0_012, TX_DEMO0_013, TX_DEMO0_014, TX_DEMO0_015,
	TX_DEMO0_016, TX_DEMO0_017, TX_DEMO0_018, TX_DEMO0_019, TX_DEMO0_020,
	TX_DEMO0_021, TX_DEMO0_022, TX_DEMO0N001, TX_DEMO0N002, TX_DEMO0N003,
	TX_DEMO0N004, TX_DEMO0N005
};

extern const RoomTextOffsets demo0TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 0, 773, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_SPEAKER_ANGIVEN, 0, 784, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_SPEAKER_MCCOY, 0, 798, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_SPEAKER_SPOCK, 0, 808, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_SPEAKER_SMITH, 0, 818, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_001, 0, 945, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_002, 0, 1062, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_003, 0, 1207, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_004, 0, 1281, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_005, 0, 1472, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_006, 0, 1636, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_007, 0, 1838, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_008, 0, 1905, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_009, 0, 1969, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_010, 0, 2162, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_011, 0, 2206, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_012, 0, 2360, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_013, 0, 2564, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_014, 0, 2678, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_015, 0, 3040, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_016, 0, 3102, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_017, 0, 3253, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_018, 0, 3604, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_019, 0, 3633, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_020, 0, 3704, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_021, 0, 3754, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0_022, 0, 3910, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0N001, 0, 303, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0N002, 0, 2806, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0N003, 0, 2888, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0N004, 0, 2904, 0, 0, 0, 0, 0, 0, 0 },
	{ TX_DEMO0N005, 0, 3015, 0, 0, 0, 0, 0, 0, 0 },
	{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

extern const RoomText demo0Texts[] = {
	{ -1, Common::UNK_LANG, "" }
};

void Room::demo0Tick100() {
	loadActorAnim(OBJECT_PRELATE, "prelclap", 0x78, 0xbe);
}

void Room::demo0TalkToPrelate() {
	const TextRef greetings[] = {
		TX_SPEAKER_KIRK,
		TX_DEMO0_001, TX_DEMO0_002, TX_DEMO0_003,
		TX_END
	};
	const TextRef questions[] = {
		TX_SPEAKER_KIRK,
		TX_DEMO0_007, TX_DEMO0_008,
		TX_END
	};
	const TextRef aboutMine[] = {
		TX_SPEAKER_KIRK,
		TX_DEMO0_010, TX_DEMO0_011,
		TX_END
	};

	if (_awayMission->demon.talkedToPrelate)
		return;
	_awayMission->demon.talkedToPrelate = true;

	int choice = showMultipleTexts(greetings);
	if (choice == 0)
		showText(TX_SPEAKER_ANGIVEN, TX_DEMO0_004);
	else if (choice == 1)
		showText(TX_SPEAKER_ANGIVEN, TX_DEMO0_005);
	else
		demo0OffendPrelate();

	if (showMultipleTexts(questions) != 0)
		demo0OffendPrelate();
	showText(TX_SPEAKER_ANGIVEN, TX_DEMO0_009);

	if (showMultipleTexts(aboutMine) != 0)
		demo0OffendPrelate();
	showText(TX_SPEAKER_ANGIVEN, TX_DEMO0_012);

	if (_awayMission->demon.wasRudeToPrelate)
		showText(TX_SPEAKER_ANGIVEN, TX_DEMO0_013);
	else
		showText(TX_SPEAKER_ANGIVEN, TX_DEMO0_014);
}

void Room::demo0OffendPrelate() {
	if (!_awayMission->demon.wasRudeToPrelate) {
		_awayMission->demon.wasRudeToPrelate = true;
		showText(TX_SPEAKER_ANGIVEN, TX_DEMO0_006);
	}
}

void Room::demo0LookAtPrelate() {
	showDescription(TX_DEMO0N002);
}

void Room::demo0UsePhaserOnPrelate() {
	showText(TX_SPEAKER_MCCOY, TX_DEMO0_022);
}

void Room::demo0LookAtSign() {
	showDescription(TX_DEMO0N001);
}

void Room::demo0LookAtColonists() {
	showDescription(TX_DEMO0N003);
}

void Room::demo0LookAtMountain() {
	showDescription(TX_DEMO0N004);
}

void Room::demo0LookAtSpock() {
	showDescription(TX_DEMO0N005);
}

void Room::demo0TalkToSpock() {
	showText(TX_SPEAKER_SPOCK, TX_DEMO0_017);
}

void Room::demo0TalkToRedshirt() {
	showText(TX_SPEAKER_SMITH, TX_DEMO0_015);
}

void Room::demo0TalkToMccoy() {
	showText(TX_SPEAKER_MCCOY, TX_DEMO0_016);
}

void Room::demo0UseSTricorderAnywhere() {
	if (_awayMission->demon.talkedToPrelate)
		showText(TX_SPEAKER_SPOCK, TX_DEMO0_021);
	else
		showText(TX_SPEAKER_SPOCK, TX_DEMO0_020);
}

void Room::demo0UseMTricorderOnPrelate() {
	if (_awayMission->demon.talkedToPrelate)
		showText(TX_SPEAKER_MCCOY, TX_DEMO0_019);
	else
		showText(TX_SPEAKER_MCCOY, TX_DEMO0_018);
}

}
