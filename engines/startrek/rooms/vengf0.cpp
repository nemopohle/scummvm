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

// Bridge of the Republic in the floppy version (VENG0.RDF)

#include "startrek/room.h"
#include "startrek/startrek.h"

#define HOTSPOT_DEAD_MAN 0x20
#define HOTSPOT_CHAIR 0x21
#define HOTSPOT_DEAD_CREWMAN 0x22
#define HOTSPOT_COMPUTER_BANK 0x23
#define HOTSPOT_PATTERSON 0x24

namespace StarTrek {

extern const RoomAction vengf0ActionList[] = {
	{ {ACTION_TICK, 1,  0, 0, 0}, &Room::vengf0Tick1 },
	{ {ACTION_TICK, 10, 0, 0, 0}, &Room::vengf0Tick10 },
	{ {ACTION_TICK, 50, 0, 0, 0}, &Room::vengf0Tick50 },

	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_COMPUTER_BANK, 0, 0}, &Room::vengf0UseSTricorderOnComputerBank },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_CHAIR,         0, 0}, &Room::vengf0UseSTricorderOnChair },

	{ {ACTION_USE, OBJECT_MCCOY,    HOTSPOT_PATTERSON,     0, 0}, &Room::vengf0UseMccoyOnDeadCrew },
	{ {ACTION_USE, OBJECT_IMTRICOR, HOTSPOT_PATTERSON,     0, 0}, &Room::vengf0UseMccoyOnDeadCrew },
	{ {ACTION_USE, OBJECT_IMEDKIT,  HOTSPOT_PATTERSON,     0, 0}, &Room::vengf0UseMccoyOnDeadCrew },
	{ {ACTION_USE, OBJECT_MCCOY,    HOTSPOT_DEAD_CREWMAN,  0, 0}, &Room::vengf0UseMccoyOnDeadCrew },
	{ {ACTION_USE, OBJECT_IMTRICOR, HOTSPOT_DEAD_CREWMAN,  0, 0}, &Room::vengf0UseMccoyOnDeadCrew },
	{ {ACTION_USE, OBJECT_IMEDKIT,  HOTSPOT_DEAD_CREWMAN,  0, 0}, &Room::vengf0UseMccoyOnDeadCrew },

	{ {ACTION_TALK, OBJECT_KIRK,     0, 0, 0}, &Room::vengf0TalkToKirk },
	{ {ACTION_TALK, OBJECT_MCCOY,    0, 0, 0}, &Room::vengf0TalkToMccoy },
	{ {ACTION_TALK, OBJECT_SPOCK,    0, 0, 0}, &Room::vengf0TalkToSpock },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::vengf0TalkToRedshirt },

	{ {ACTION_USE, OBJECT_ICOMM, 0xff, 0, 0}, &Room::vengf0UseCommunicator },
	{ {ACTION_TOUCHED_HOTSPOT, 0, 0, 0, 0}, &Room::vengf0TouchedHotspot0 },

	{ {ACTION_LOOK, HOTSPOT_PATTERSON,     0, 0, 0}, &Room::vengf0LookAtPatterson },
	{ {ACTION_LOOK, HOTSPOT_DEAD_CREWMAN,  0, 0, 0}, &Room::vengf0LookAtDeadCrewman },
	{ {ACTION_LOOK, HOTSPOT_COMPUTER_BANK, 0, 0, 0}, &Room::vengf0LookAtComputerBank },
	{ {ACTION_LOOK, HOTSPOT_CHAIR,         0, 0, 0}, &Room::vengf0LookAtChair },
	{ {ACTION_LOOK, OBJECT_KIRK,           0, 0, 0}, &Room::vengf0LookAtKirk },
	{ {ACTION_LOOK, OBJECT_SPOCK,          0, 0, 0}, &Room::vengf0LookAtSpock },
	{ {ACTION_LOOK, OBJECT_MCCOY,          0, 0, 0}, &Room::vengf0LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_REDSHIRT,       0, 0, 0}, &Room::vengf0LookAtRedshirt },
	{ {ACTION_LOOK, HOTSPOT_DEAD_MAN,      0, 0, 0}, &Room::vengf0LookAtDeadMan },

	{ {ACTION_USE, OBJECT_IMEDKIT,  HOTSPOT_DEAD_MAN, 0, 0}, &Room::vengf0UseMccoyOnDeadMan },
	{ {ACTION_USE, OBJECT_IMTRICOR, HOTSPOT_DEAD_MAN, 0, 0}, &Room::vengf0UseMccoyOnDeadMan },
	{ {ACTION_USE, OBJECT_MCCOY,    HOTSPOT_DEAD_MAN, 0, 0}, &Room::vengf0UseMccoyOnDeadMan },
	{ {ACTION_DONE_WALK, 1,                        0, 0, 0}, &Room::vengf0MccoyReachedDeadMan },
	{ {ACTION_DONE_ANIM, 1,                        0, 0, 0}, &Room::vengf0MccoyExaminedDeadMan },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum vengf0TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_KIJE, TX_SPEAKER_PATTERSON,
	TX_VENF0_001, TX_VENF0_002, TX_VENF0_003, TX_VENF0_004, TX_VENF0_005,
	TX_VENF0_006, TX_VENF0_007, TX_VENF0_008, TX_VENF0_009, TX_VENF0_010,
	TX_VENF0_011, TX_VENF0_012, TX_VENF0_013, TX_VENF0_014, TX_VENF0_015,
	TX_VENF0_016, TX_VENF0_017, TX_VENF0_018, TX_VENF0_019, TX_VENF0_020,
	TX_VENF0_021, TX_VENF0_022, TX_VENF0N001, TX_VENF0N002, TX_VENF0N003,
	TX_VENF0N004, TX_VENF0N005, TX_VENF0N006, TX_VENF0N007, TX_VENF0N008,
	TX_VENF0N009
};

extern const RoomTextOffsets vengf0TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 0, 1317, 0, 0, 1314, 1314, 1323 },
	{ TX_SPEAKER_MCCOY, 0, 1328, 0, 0, 1325, 1327, 1334 },
	{ TX_SPEAKER_SPOCK, 0, 1338, 0, 0, 1335, 1337, 1344 },
	{ TX_SPEAKER_KIJE, 0, 1348, 0, 0, 1345, 1347, 1354 },
	{ TX_SPEAKER_PATTERSON, 0, 1358, 0, 0, 1357, 1359, 1344 },
	{ TX_VENF0_001, 0, 1395, 0, 0, 1394, 1401, 1403 },
	{ TX_VENF0_002, 0, 1459, 0, 0, 1458, 1468, 1486 },
	{ TX_VENF0_003, 0, 1483, 0, 0, 1482, 1495, 1510 },
	{ TX_VENF0_004, 0, 1848, 0, 0, 1847, 1848, 1925 },
	{ TX_VENF0_005, 0, 2208, 0, 0, 2198, 2213, 2340 },
	{ TX_VENF0_006, 0, 2266, 0, 0, 2256, 2281, 2403 },
	{ TX_VENF0_007, 0, 2339, 0, 0, 2329, 2364, 2496 },
	{ TX_VENF0_008, 0, 2403, 0, 0, 2393, 2421, 2563 },
	{ TX_VENF0_009, 0, 2416, 0, 0, 2406, 2447, 2587 },
	{ TX_VENF0_010, 0, 2504, 0, 0, 2494, 2560, 2749 },
	{ TX_VENF0_011, 0, 2534, 0, 0, 2524, 2593, 2861 },
	{ TX_VENF0_012, 0, 2623, 0, 0, 2613, 2690, 2919 },
	{ TX_VENF0_013, 0, 2687, 0, 0, 2667, 2741, 3458 },
	{ TX_VENF0_014, 0, 2717, 0, 0, 2697, 2780, 2953 },
	{ TX_VENF0_015, 0, 2777, 0, 0, 2757, 2855, 3208 },
	{ TX_VENF0_016, 0, 2937, 0, 0, 2920, 3046, 3244 },
	{ TX_VENF0_017, 0, 2967, 0, 0, 2950, 3094, 3458 },
	{ TX_VENF0_018, 0, 3022, 0, 0, 3005, 3146, 3300 },
	{ TX_VENF0_019, 0, 3059, 0, 0, 3038, 3176, 3327 },
	{ TX_VENF0_020, 0, 3085, 0, 0, 3064, 3211, 3356 },
	{ TX_VENF0_021, 0, 3174, 0, 0, 3153, 3308, 3458 },
	{ TX_VENF0_022, 0, 709, 0, 0, 706, 702, 702 },
	{ TX_VENF0N001, 0, 1377, 0, 0, 1376, 1380, 1385 },
	{ TX_VENF0N002, 0, 1622, 0, 0, 1621, 1629, 1661 },
	{ TX_VENF0N003, 0, 1708, 0, 0, 1707, 1715, 1746 },
	{ TX_VENF0N004, 0, 1729, 0, 0, 1728, 1737, 1772 },
	{ TX_VENF0N005, 0, 1784, 0, 0, 1783, 1781, 1845 },
	{ TX_VENF0N006, 0, 1966, 0, 0, 1956, 1982, 2065 },
	{ TX_VENF0N007, 0, 2035, 0, 0, 2025, 2049, 2140 },
	{ TX_VENF0N008, 0, 2063, 0, 0, 2053, 2079, 2169 },
	{ TX_VENF0N009, 0, 2131, 0, 0, 2121, 2143, 2250 },
	{          -1, 0,    0, 0, 0, 0, 0, 0 }
};

extern const RoomText vengf0Texts[] = {
	{ -1, Common::UNK_LANG, "" }
};

void Room::vengf0Tick1() {
	playMidiMusicTracks(MIDITRACK_14);
}

void Room::vengf0Tick10() {
	_vm->_sound->playMidiTrack(MIDITRACK_14);
}

void Room::vengf0Tick50() {
	if (!_awayMission->veng.enteredRoom0FirstTime) {
		playMidiMusicTracks(MIDITRACK_0);
		_awayMission->veng.enteredRoom0FirstTime = true;
	}
}

void Room::vengf0UseSTricorderOnComputerBank() {
	if (!_awayMission->veng.scannedComputerBank) {
		_awayMission->veng.scannedComputerBank = true;
		_awayMission->veng.missionScore += 2;
		spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_VENF0_009);
		showText(TX_SPEAKER_KIRK,  TX_VENF0_010);
		showText(TX_SPEAKER_SPOCK, TX_VENF0_011);
		showText(TX_SPEAKER_MCCOY, TX_VENF0_012);
		showText(TX_SPEAKER_SPOCK, TX_VENF0_013);
	} else
		showText(TX_SPEAKER_SPOCK, TX_VENF0_021);
}

void Room::vengf0UseSTricorderOnChair() {
	if (!_awayMission->veng.scannedCaptainsChair) {
		_awayMission->veng.scannedCaptainsChair = true;
		_awayMission->veng.missionScore += 2;
		spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_VENF0_014);
		showText(TX_SPEAKER_PATTERSON, TX_VENF0_015);
		showText(TX_SPEAKER_SPOCK,     TX_VENF0_016);
		showText(TX_SPEAKER_KIRK,      TX_VENF0_017);
	} else
		showText(TX_SPEAKER_SPOCK, TX_VENF0_021);
}

void Room::vengf0UseMccoyOnDeadCrew() {
	mccoyScan(DIR_S, TX_SPEAKER_MCCOY, TX_VENF0_018);
}

void Room::vengf0TalkToKirk() {
	showText(TX_SPEAKER_KIRK, TX_VENF0_005);
}

void Room::vengf0TalkToMccoy() {
	showText(TX_SPEAKER_MCCOY, TX_VENF0_007);
}

void Room::vengf0TalkToSpock() {
	showText(TX_SPEAKER_SPOCK, TX_VENF0_006);
}

void Room::vengf0TalkToRedshirt() {
	showText(TX_SPEAKER_KIJE, TX_VENF0_008);
}

void Room::vengf0UseCommunicator() {
	const TextRef choices[] = {
		TX_SPEAKER_KIRK,
		TX_VENF0_019, TX_VENF0_020,
		TX_END
	};

	if (showMultipleTexts(choices) == 0) {
		if (_awayMission->veng.usedMedkitOnBrittany)
			showText(TX_SPEAKER_KIRK, TX_VENF0_022);
		endMission(_awayMission->veng.missionScore, 12, 0);
	}
}

void Room::vengf0TouchedHotspot0() {
	playSoundEffectIndex(kSfxDoor);
	showText(TX_SPEAKER_KIJE, TX_VENF0_004);
	loadRoomIndex(1, 5);
}

void Room::vengf0LookAtPatterson() {
	showDescription(TX_VENF0N002);
}

void Room::vengf0LookAtDeadCrewman() {
	showDescription(TX_VENF0N003);
}

void Room::vengf0LookAtComputerBank() {
	showDescription(TX_VENF0N004);
}

void Room::vengf0LookAtChair() {
	showDescription(TX_VENF0N005);
}

void Room::vengf0LookAtKirk() {
	showDescription(TX_VENF0N006);
}

void Room::vengf0LookAtSpock() {
	showDescription(TX_VENF0N007);
}

void Room::vengf0LookAtMccoy() {
	showDescription(TX_VENF0N008);
}

void Room::vengf0LookAtRedshirt() {
	showDescription(TX_VENF0N009);
}

void Room::vengf0LookAtDeadMan() {
	showDescription(TX_VENF0N001);
	vengf0UseMccoyOnDeadMan();
}

void Room::vengf0UseMccoyOnDeadMan() {
	if (!_awayMission->veng.bridgeCrewmanDead) {
		_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_E;
		walkCrewmanC(OBJECT_MCCOY, 0xdc, 0xa0, &Room::vengf0MccoyReachedDeadMan);
	}
}

void Room::vengf0MccoyReachedDeadMan() {
	_awayMission->veng.bridgeCrewmanDead = true;
	_awayMission->veng.missionScore += 2;
	mccoyScan(DIR_E, TX_SPEAKER_MCCOY, TX_VENF0_001);
	showText(TX_SPEAKER_KIRK, TX_VENF0_002);
	loadActorAnimC(OBJECT_MCCOY, "museme", -1, -1, &Room::vengf0MccoyExaminedDeadMan);
}

void Room::vengf0MccoyExaminedDeadMan() {
	showText(TX_SPEAKER_MCCOY, TX_VENF0_003);
}

}
