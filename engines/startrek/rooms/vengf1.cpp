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

// Sickbay of the Republic in the floppy version (VENG1.RDF)

#include "startrek/room.h"

#define OBJECT_BRITTANY 8
#define OBJECT_DOOR 9

#define HOTSPOT_DOOR 0x20
#define HOTSPOT_LEFT_BED 0x21
#define HOTSPOT_RIGHT_BED 0x22
#define HOTSPOT_LEFT_READINGS 0x23
#define HOTSPOT_RIGHT_READINGS 0x24

namespace StarTrek {

extern const RoomAction vengf1ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::vengf1Tick1 },

	{ {ACTION_TALK, OBJECT_KIRK,     0, 0, 0}, &Room::vengf1TalkToKirk },
	{ {ACTION_TALK, OBJECT_MCCOY,    0, 0, 0}, &Room::vengf1TalkToMccoy },
	{ {ACTION_TALK, OBJECT_SPOCK,    0, 0, 0}, &Room::vengf1TalkToSpock },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::vengf1TalkToRedshirt },

	{ {ACTION_USE, OBJECT_IMEDKIT,  HOTSPOT_RIGHT_BED, 0, 0}, &Room::vengf1UseMccoyOnDeadGuy },
	{ {ACTION_USE, OBJECT_IMTRICOR, HOTSPOT_RIGHT_BED, 0, 0}, &Room::vengf1UseMccoyOnDeadGuy },
	{ {ACTION_USE, OBJECT_MCCOY,    HOTSPOT_RIGHT_BED, 0, 0}, &Room::vengf1UseMccoyOnDeadGuy },

	{ {ACTION_USE, OBJECT_IMEDKIT,  OBJECT_BRITTANY,   0, 0}, &Room::vengf1UseMedkitOnBrittany },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_BRITTANY,   0, 0}, &Room::vengf1UseMTricorderOnBrittany },
	{ {ACTION_USE, OBJECT_MCCOY,    OBJECT_BRITTANY,   0, 0}, &Room::vengf1UseMTricorderOnBrittany },
	{ {ACTION_DONE_WALK, 1,                         0, 0, 0}, &Room::vengf1MccoyReachedBrittany },
	{ {ACTION_DONE_ANIM, 1,                         0, 0, 0}, &Room::vengf1UsedMedkitOnBrittany },
	{ {ACTION_DONE_ANIM, 2,                         0, 0, 0}, &Room::vengf1MccoyScannedBrittany },

	{ {ACTION_TALK, OBJECT_BRITTANY,                0, 0, 0}, &Room::vengf1TalkToBrittany },
	{ {ACTION_USE, OBJECT_ICOMM, 0xff,              0, 0}, &Room::vengf1UseCommunicator },

	{ {ACTION_LOOK, HOTSPOT_LEFT_BED,               0, 0, 0}, &Room::vengf1LookAtLeftBed },
	{ {ACTION_LOOK, HOTSPOT_RIGHT_BED,              0, 0, 0}, &Room::vengf1LookAtRightBed },
	{ {ACTION_LOOK, HOTSPOT_LEFT_READINGS,          0, 0, 0}, &Room::vengf1LookAtLeftBedReadings },
	{ {ACTION_LOOK, HOTSPOT_RIGHT_READINGS,         0, 0, 0}, &Room::vengf1LookAtRightBedReadings },
	{ {ACTION_LOOK, HOTSPOT_DOOR,                   0, 0, 0}, &Room::vengf1LookAtDoor },
	{ {ACTION_LOOK, OBJECT_KIRK,                    0, 0, 0}, &Room::vengf1LookAtKirk },
	{ {ACTION_LOOK, OBJECT_SPOCK,                   0, 0, 0}, &Room::vengf1LookAtSpock },
	{ {ACTION_LOOK, OBJECT_MCCOY,                   0, 0, 0}, &Room::vengf1LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_REDSHIRT,                0, 0, 0}, &Room::vengf1LookAtRedshirt },
	{ {ACTION_LOOK, OBJECT_BRITTANY,                0, 0, 0}, &Room::vengf1LookAtBrittany },

	{ {ACTION_TOUCHED_HOTSPOT, 0, 0, 0, 0}, &Room::vengf1TouchedHotspot0 },
	{ {ACTION_WALK, HOTSPOT_DOOR, 0, 0, 0}, &Room::vengf1WalkToDoor },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum vengf1TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_KIJE, TX_SPEAKER_BRITTANY_MARATA,
	TX_VENF1_001, TX_VENF1_002, TX_VENF1_003, TX_VENF1_004, TX_VENF1_005,
	TX_VENF1_006, TX_VENF1_007, TX_VENF1_008, TX_VENF1_009, TX_VENF1_010,
	TX_VENF1_011, TX_VENF1_012, TX_VENF1_013, TX_VENF1_014, TX_VENF1_015,
	TX_VENF1_016, TX_VENF1_017, TX_VENF1_018, TX_VENF1_019, TX_VENF1_020,
	TX_VENF1_021, TX_VENF1_022, TX_VENF1N001, TX_VENF1N002, TX_VENF1N003,
	TX_VENF1N004, TX_VENF1N005, TX_VENF1N006, TX_VENF1N007, TX_VENF1N008,
	TX_VENF1N009
};

extern const RoomTextOffsets vengf1TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 0, 1500, 0, 0, 1515, 1564, 1561, 1515, 0 },
	{ TX_SPEAKER_MCCOY, 0, 1511, 0, 0, 1526, 1577, 1572, 1526, 0 },
	{ TX_SPEAKER_SPOCK, 0, 1521, 0, 0, 1536, 1587, 1582, 1536, 0 },
	{ TX_SPEAKER_KIJE, 0, 1531, 0, 0, 1546, 1597, 1592, 1546, 0 },
	{ TX_SPEAKER_BRITTANY_MARATA, 0, 1541, 0, 0, 1558, 1609, 1561, 1558, 0 },
	{ TX_VENF1_001, 0, 466, 0, 0, 468, 468, 468, 468, 0 },
	{ TX_VENF1_002, 0, 727, 0, 0, 730, 733, 727, 730, 0 },
	{ TX_VENF1_003, 0, 812, 0, 0, 815, 842, 835, 815, 0 },
	{ TX_VENF1_004, 0, 1025, 0, 0, 1040, 1072, 1073, 1040, 0 },
	{ TX_VENF1_005, 0, 1719, 0, 0, 1736, 1805, 1803, 1736, 0 },
	{ TX_VENF1_006, 0, 1816, 0, 0, 1833, 1911, 1906, 1833, 0 },
	{ TX_VENF1_007, 0, 2099, 0, 0, 2123, 2170, 2212, 2123, 0 },
	{ TX_VENF1_008, 0, 2205, 0, 0, 2229, 2270, 2354, 2229, 0 },
	{ TX_VENF1_009, 0, 2250, 0, 0, 2274, 2325, 2399, 2274, 0 },
	{ TX_VENF1_010, 0, 2482, 0, 0, 2506, 2557, 2661, 2506, 0 },
	{ TX_VENF1_011, 0, 2537, 0, 0, 2561, 2618, 2740, 2561, 0 },
	{ TX_VENF1_012, 0, 2563, 0, 0, 2587, 2658, 2801, 2587, 0 },
	{ TX_VENF1_013, 0, 2586, 0, 0, 2610, 2684, 2877, 2610, 0 },
	{ TX_VENF1_014, 0, 2652, 0, 0, 2677, 2753, 2891, 2677, 0 },
	{ TX_VENF1_015, 0, 2679, 0, 0, 2704, 2781, 2978, 2704, 0 },
	{ TX_VENF1_016, 0, 2737, 0, 0, 2762, 2833, 3016, 2762, 0 },
	{ TX_VENF1_017, 0, 2771, 0, 0, 2796, 2871, 3016, 2796, 0 },
	{ TX_VENF1_018, 0, 2833, 0, 0, 2858, 2961, 3096, 2858, 0 },
	{ TX_VENF1_019, 0, 2902, 0, 0, 2927, 3031, 3178, 2927, 0 },
	{ TX_VENF1_020, 0, 3028, 0, 0, 3051, 3189, 3334, 3051, 0 },
	{ TX_VENF1_021, 0, 3072, 0, 0, 3095, 3229, 3380, 3095, 0 },
	{ TX_VENF1_022, 0, 3098, 0, 0, 3121, 3264, 3416, 3121, 0 },
	{ TX_VENF1N001, 0, 1558, 0, 0, 1575, 1626, 1623, 1575, 0 },
	{ TX_VENF1N002, 0, 1607, 0, 0, 1624, 1667, 1678, 1624, 0 },
	{ TX_VENF1N003, 0, 1646, 0, 0, 1663, 1711, 1717, 1663, 0 },
	{ TX_VENF1N004, 0, 1683, 0, 0, 1700, 1769, 1758, 1700, 0 },
	{ TX_VENF1N005, 0, 1866, 0, 0, 1883, 1936, 1953, 1883, 0 },
	{ TX_VENF1N006, 0, 1910, 0, 0, 1927, 1983, 2007, 1927, 0 },
	{ TX_VENF1N007, 0, 1938, 0, 0, 1955, 2007, 2040, 1955, 0 },
	{ TX_VENF1N008, 0, 1980, 0, 0, 2005, 2054, 2081, 2005, 0 },
	{ TX_VENF1N009, 0, 2030, 0, 0, 2055, 2111, 2126, 2055, 0 },
	{          -1, 0,    0, 0, 0, 0, 0, 0, 0, 0 }
};

extern const RoomText vengf1Texts[] = {
	{ -1, Common::UNK_LANG, "" }
};

#define DOOR_X 0x13f
#define DOOR_Y 0xab

#define BRITTANY_X 0x5a
#define BRITTANY_Y 0xa5

void Room::vengf1Tick1() {
	playMidiMusicTracks(MIDITRACK_31);

	if (!_awayMission->veng.usedMedkitOnBrittany)
		loadActorAnim(OBJECT_BRITTANY, "s9r1bm", BRITTANY_X, BRITTANY_Y);
	else
		loadActorAnim(OBJECT_BRITTANY, "s9r1bh", BRITTANY_X, BRITTANY_Y);

	walkCrewman(OBJECT_KIRK,  0xe6,  0xb9);
	walkCrewman(OBJECT_SPOCK, 0x105, 0xc2);
	walkCrewman(OBJECT_MCCOY, 0xd2,  0xaa);
}

void Room::vengf1TalkToKirk() {
	showText(TX_SPEAKER_KIRK, TX_VENF1_007);
}

void Room::vengf1TalkToMccoy() {
	showText(TX_SPEAKER_MCCOY, TX_VENF1_009);
}

void Room::vengf1TalkToSpock() {
	showText(TX_SPEAKER_SPOCK, TX_VENF1_008);
}

void Room::vengf1TalkToRedshirt() {
	showText(TX_SPEAKER_KIJE, TX_VENF1_010);
}

void Room::vengf1UseMccoyOnDeadGuy() {
	showText(TX_SPEAKER_MCCOY, TX_VENF1_001);
}

void Room::vengf1UseMedkitOnBrittany() {
	_roomVar.veng.usingMedkitOnBrittany = true;
	vengf1UseMccoyOnBrittany();
}

void Room::vengf1UseMTricorderOnBrittany() {
	_roomVar.veng.usingMedkitOnBrittany = false;
	vengf1UseMccoyOnBrittany();
}

void Room::vengf1UseMccoyOnBrittany() {
	if (_awayMission->veng.usedMedkitOnBrittany)
		showText(TX_SPEAKER_MCCOY, TX_VENF1_020);
	else
		vengf1WalkMccoyToBrittany();
}

void Room::vengf1WalkMccoyToBrittany() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_E;
	walkCrewmanC(OBJECT_MCCOY, 0x3c, 0xa0, &Room::vengf1MccoyReachedBrittany);
}

void Room::vengf1MccoyReachedBrittany() {
	if (!_roomVar.veng.usingMedkitOnBrittany) {
		loadActorAnimC(OBJECT_MCCOY, "mscane", -1, -1, &Room::vengf1MccoyScannedBrittany);
		playSoundEffectIndex(kSfxTricorder);
	} else {
		loadActorAnimC(OBJECT_MCCOY, "museme", -1, -1, &Room::vengf1UsedMedkitOnBrittany);
	}
}

void Room::vengf1UsedMedkitOnBrittany() {
	loadActorAnim(OBJECT_BRITTANY, "s9r1bh");
	_awayMission->veng.missionScore += 4;
	_awayMission->veng.usedMedkitOnBrittany = true;
	showText(TX_SPEAKER_MCCOY, TX_VENF1_019);
}

void Room::vengf1MccoyScannedBrittany() {
	showText(TX_SPEAKER_MCCOY, TX_VENF1_002);
	showText(TX_SPEAKER_KIRK,  TX_VENF1_003);

	_roomVar.veng.usingMedkitOnBrittany = true;
	vengf1WalkMccoyToBrittany();
}

void Room::vengf1TalkToBrittany() {
	if (_awayMission->veng.talkedToBrittany)
		showText(TX_SPEAKER_MCCOY, TX_VENF1_018);
	else if (_awayMission->veng.usedMedkitOnBrittany) {
		_awayMission->veng.talkedToBrittany = true;
		_awayMission->veng.missionScore += 2;
		showText(TX_SPEAKER_BRITTANY_MARATA, TX_VENF1_011);
		showText(TX_SPEAKER_KIRK,            TX_VENF1_012);
		showText(TX_SPEAKER_BRITTANY_MARATA, TX_VENF1_013);
		showText(TX_SPEAKER_KIRK,            TX_VENF1_014);
		showText(TX_SPEAKER_BRITTANY_MARATA, TX_VENF1_015);
		showText(TX_SPEAKER_KIRK,            TX_VENF1_016);
		showText(TX_SPEAKER_MCCOY,           TX_VENF1_017);
	}
}

void Room::vengf1UseCommunicator() {
	const TextRef choices[] = {
		TX_SPEAKER_KIRK,
		TX_VENF1_021, TX_VENF1_022,
		TX_END
	};

	if (showMultipleTexts(choices) == 0) {
		if (_awayMission->veng.usedMedkitOnBrittany)
			showText(TX_SPEAKER_KIRK, TX_VENF1_004);
		endMission(_awayMission->veng.missionScore, 12, 0);
	}
}

void Room::vengf1LookAtLeftBed() {
	showDescription(TX_VENF1N005);
}

void Room::vengf1LookAtRightBed() {
	showDescription(TX_VENF1N006);
}

void Room::vengf1LookAtLeftBedReadings() {
	showDescription(TX_VENF1N007);
}

void Room::vengf1LookAtRightBedReadings() {
	showDescription(TX_VENF1N008);
}

void Room::vengf1LookAtDoor() {
	showDescription(TX_VENF1N009);
}

void Room::vengf1LookAtKirk() {
	showDescription(TX_VENF1N001);
}

void Room::vengf1LookAtSpock() {
	showDescription(TX_VENF1N003);
}

void Room::vengf1LookAtMccoy() {
	showDescription(TX_VENF1N002);
}

void Room::vengf1LookAtRedshirt() {
	showDescription(TX_VENF1N004);
}

void Room::vengf1LookAtBrittany() {
	if (!_awayMission->veng.lookedAtBrittany) {
		_awayMission->veng.lookedAtBrittany = true;
		_awayMission->veng.missionScore += 1;
		showText(TX_SPEAKER_KIRK,  TX_VENF1_005);
		showText(TX_SPEAKER_MCCOY, TX_VENF1_006);
	}
}

void Room::vengf1TouchedHotspot0() { // Trigger door opening
	if (_roomVar.veng.walkingToDoor) {
		playSoundEffectIndex(kSfxDoor);
		loadActorAnim(OBJECT_DOOR, "s9r1do", DOOR_X, DOOR_Y);
	}
}

void Room::vengf1WalkToDoor() {
	_roomVar.veng.walkingToDoor = true;
	_awayMission->disableInput = true;
	walkCrewman(OBJECT_KIRK, 0x12e, 0xab);
}

}
