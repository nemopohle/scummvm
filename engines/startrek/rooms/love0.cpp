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

#include "startrek/room.h"
#include "startrek/startrek.h"

#define OBJECT_DOOR1 8
#define OBJECT_DOOR2 9

#define HOTSPOT_CONSOLE 0x20
#define HOTSPOT_DOOR2 0x21
#define HOTSPOT_DOOR1 0x22
#define HOTSPOT_VIEWSCREEN 0x23

namespace StarTrek {

extern const RoomAction love0ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::love0Tick1 },
	{ {ACTION_TICK, 10, 0, 0, 0}, &Room::love0Tick10 },
	{ {ACTION_WALK, 0x21, 0, 0, 0}, &Room::love0WalkToDoor2 },
	{ {ACTION_WALK, 9, 0, 0, 0}, &Room::love0WalkToDoor2 },
	{ {ACTION_TOUCHED_HOTSPOT, 0, 0, 0, 0}, &Room::love0OpenDoor2 },
	{ {ACTION_FINISHED_WALKING, 4, 0, 0, 0}, &Room::love0ReachedDoor2 },
	{ {ACTION_FINISHED_ANIMATION, 3, 0, 0, 0}, &Room::love0ReachedDoor2 },
	{ {ACTION_WALK, 0x22, 0, 0, 0}, &Room::love0WalkToDoor1 },
	{ {ACTION_WALK, 8, 0, 0, 0}, &Room::love0WalkToDoor1 },
	{ {ACTION_TOUCHED_HOTSPOT, 1, 0, 0, 0}, &Room::love0OpenDoor1 },
	{ {ACTION_FINISHED_WALKING, 5, 0, 0, 0}, &Room::love0ReachedDoor1 },
	{ {ACTION_FINISHED_ANIMATION, 4, 0, 0, 0}, &Room::love0ReachedDoor1 },
	{ {ACTION_LOOK, 0x20, 0, 0, 0}, &Room::love0LookAtConsole },
	{ {ACTION_LOOK, 0x23, 0, 0, 0}, &Room::love0LookAtViewscreen },
	{ {ACTION_LOOK, 0xff, 0, 0, 0}, &Room::love0LookAnywhere },
	{ {ACTION_LOOK, 8, 0, 0, 0}, &Room::love0LookAtDoor1 },
	{ {ACTION_LOOK, 9, 0, 0, 0}, &Room::love0LookAtDoor2 },
	{ {ACTION_LOOK, OBJECT_KIRK, 0, 0, 0}, &Room::love0LookAtKirk },
	{ {ACTION_LOOK, OBJECT_MCCOY, 0, 0, 0}, &Room::love0LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_SPOCK, 0, 0, 0}, &Room::love0LookAtSpock },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::love0LookAtRedshirt },
	{ {ACTION_TALK, OBJECT_KIRK, 0, 0, 0}, &Room::love0TalkToKirk },
	{ {ACTION_TALK, OBJECT_MCCOY, 0, 0, 0}, &Room::love0TalkToMccoy },
	{ {ACTION_TALK, OBJECT_SPOCK, 0, 0, 0}, &Room::love0TalkToSpock },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::love0TalkToRedshirt },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0xff, 0, 0}, &Room::love0UseMTricorderAnywhere },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x20, 0, 0}, &Room::love0UseSTricorderOnConsole },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0xff, 0, 0}, &Room::love0UseSTricorderAnywhere },
	{ {ACTION_USE, OBJECT_KIRK, 0x20, 0, 0}, &Room::love0UseKirkOnConsole },
	{ {ACTION_USE, OBJECT_REDSHIRT, 0x20, 0, 0}, &Room::love0UseRedshirtOnConsole },
	{ {ACTION_USE, OBJECT_SPOCK, 0x20, 0, 0}, &Room::love0UseSpockOnConsole },
	{ {ACTION_FINISHED_WALKING, 2, 0, 0, 0}, &Room::love0SpockReachedConsole },
	{ {ACTION_FINISHED_ANIMATION, 5, 0, 0, 0}, &Room::love0SpockAccessedConsole },
	{ {ACTION_USE, OBJECT_MCCOY, 0x20, 0, 0}, &Room::love0UseMccoyOnConsole },
	{ {ACTION_FINISHED_WALKING, 3, 0, 0, 0}, &Room::love0MccoyReachedConsole },
	{ {ACTION_FINISHED_ANIMATION, 6, 0, 0, 0}, &Room::love0MccoyAccessedConsole },
	{ {ACTION_GET,    8, 0, 0, 0}, &Room::love0GetDoorOrConsole },
	{ {ACTION_GET,    9, 0, 0, 0}, &Room::love0GetDoorOrConsole },
	{ {ACTION_GET, 0x20, 0, 0, 0}, &Room::love0GetDoorOrConsole },

	// Common code
	{ {ACTION_TIMER_EXPIRED, 0, 0, 0, 0}, &Room::loveaTimer0Expired },
	{ {ACTION_TIMER_EXPIRED, 1, 0, 0, 0}, &Room::loveaTimer1Expired },
	{ {ACTION_USE, OBJECT_IMEDKIT, OBJECT_SPOCK, 0, 0}, &Room::loveaUseMedkitOnSpock },
	{ {ACTION_USE, OBJECT_ISAMPLE, OBJECT_SPOCK, 0, 0}, &Room::loveaUseCureSampleOnSpock },
	{ {ACTION_USE, OBJECT_ICURE, OBJECT_SPOCK, 0, 0}, &Room::loveaUseCureOnSpock },
	{ {ACTION_FINISHED_WALKING,   99, 0, 0, 0}, &Room::loveaSpockOrMccoyInPositionToUseCure },
	{ {ACTION_FINISHED_ANIMATION, 99, 0, 0, 0}, &Room::loveaFinishedCuringSpock },
	{ {ACTION_TIMER_EXPIRED, 2, 0, 0, 0}, &Room::loveaTimer2Expired },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_SPOCK, 0, 0}, &Room::loveaUseMTricorderOnSpock },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_KIRK, 0, 0}, &Room::loveaUseMTricorderOnHuman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_MCCOY, 0, 0}, &Room::loveaUseMTricorderOnHuman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_REDSHIRT, 0, 0}, &Room::loveaUseMTricorderOnHuman },
	{ {ACTION_USE, OBJECT_IRLG, 0xff, 0, 0}, &Room::loveaUseRomulanLaughingGas },
	{ {ACTION_USE, OBJECT_IN2O, 0xff, 0, 0}, &Room::loveaUseHumanLaughingGas },
	{ {ACTION_USE, OBJECT_INH3, 0xff, 0, 0}, &Room::loveaUseAmmonia },
	{ {ACTION_USE, OBJECT_ICOMM, 0xff, 0, 0}, &Room::loveaUseCommunicator },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum love0TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_FERRIS, TX_SPEAKER_COMPUTER,
	TX_LOV0_001, TX_LOV0_002, TX_LOV0_003, TX_LOV0_004, TX_LOV0_005,
	TX_LOV0_006, TX_LOV0_007, TX_LOV0_008, TX_LOV0_009, TX_LOV0_010,
	TX_LOV0_023, TX_LOV0_024, TX_LOV0_027, TX_LOV0_028, TX_LOV0_030,
	TX_LOV0_031, TX_LOV0_035, TX_LOV0_036, TX_LOV0_037, TX_LOV0_038,
	TX_LOV0_049, TX_LOV0N000, TX_LOV0N001, TX_LOV0N002,	TX_LOV0N003,
	TX_LOV0N004, TX_LOV0N005, TX_LOV0N006, TX_LOV0N007, TX_LOV0N008,
	TX_LOV0N009, TX_COMPA186, TX_COMPA187, TX_COMPA189, TX_COMPA190,
	TX_COMPA191, TX_COMPA193, TX_COMPU186, TX_COMPU187, TX_COMPU188,
	TX_COMPU189, TX_COMPU190, TX_COMPU192, TX_COMPU193, TX_DIALOG_ERROR
};

extern const RoomTextOffsets love0TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 5915, 6466, 6024, 6270, 6469, 7096, 7069 },
	{ TX_SPEAKER_MCCOY, 5926, 6477, 6035, 6281, 6480, 7109, 7080 },
	{ TX_SPEAKER_SPOCK, 5936, 6487, 6045, 6291, 6490, 7119, 7090 },
	{ TX_SPEAKER_FERRIS, 5946, 6497, 6055, 6301, 6500, 7129, 7100 },
	{ TX_SPEAKER_COMPUTER, 5957, 6508, 6066, 6312, 6511, 7140, 7111 },
	{ TX_LOV0_001, 10075, 10144, 10684, 10653, 10146, 10880, 11117 },
	{ TX_LOV0_002,  1488, 1444, 1557, 1555, 1443, 1600, 1642 },
	{ TX_LOV0_003,   959, 960, 959, 959, 960, 1084, 1092 },
	{ TX_LOV0_004,  7933, 8212, 8268, 8396, 8213, 8938, 9041 },
	{ TX_LOV0_005, 10149, 10203, 10769, 10722, 10203, 10933, 11187 },
	{ TX_LOV0_006,  1256, 1227, 1308, 1299, 1227, 1361, 1408 },
	{ TX_LOV0_007,  1105, 1091, 1144, 1136, 1091, 1217, 1262 },
	{ TX_LOV0_008,  6848, 7234, 7049, 7271, 7237, 7923, 7929 },
	{ TX_LOV0_009,  7999, 8263, 8346, 8459, 8264, 8986, 9104 },
	{ TX_LOV0_010, 10477, 10471, 11147, 11083, 10463, 11225, 11493 },
	{ TX_LOV0_023,  7881, 8175, 8230, 8342, 8176, 8899, 9018 },
	{ TX_LOV0_024,  7536, 7862, 7826, 7990, 7864, 8583, 8645 },
	{ TX_LOV0_027,  6650, 7066, 6809, 7042, 7069, 7735, 7719 },
	{ TX_LOV0_028,  7062, 7418, 7299, 7479, 7421, 8102, 8149 },
	{ TX_LOV0_030, 10335, 10359, 10997, 10919, 10358, 11100, 11385 },
	{ TX_LOV0_031,  6931, 7302, 7130, 7357, 7305, 7995, 7995 },
	{ TX_LOV0_035,  7805, 8114, 8156, 8266, 8115, 8838, 8959 },
	{ TX_LOV0_036,  1591, 1532, 1680, 1679, 1531, 1705, 1750 },
	{ TX_LOV0_037, 10240, 10279, 10878, 10833, 10279, 11029, 11281 },
	{ TX_LOV0_038,  6760, 7161, 6931, 7160, 7164, 7838, 7826 },
	{ TX_LOV0_049, 11461, 10717, 12232, 12067, 10709, 11469, 11781 },
	{ TX_LOV0N000,  6449, 6895, 6591, 6812, 6898, 7535, 7531 },
	{ TX_LOV0N001, 10380, 10389, 11045, 10966, 10388, 11132, 11418 },
	{ TX_LOV0N002,  6227, 6733, 6362, 6597, 6736, 7380, 7362 },
	{ TX_LOV0N003,  6344, 6820, 6478, 6713, 6823, 7466, 7448 },
	{ TX_LOV0N004,  6185, 6706, 6329, 6556, 6709, 7354, 7344 },
	{ TX_LOV0N005,  6292, 6783, 6435, 6667, 6786, 7435, 7420 },
	{ TX_LOV0N006,  6049, 6585, 6158, 6418, 6588, 7231, 7188 },
	{ TX_LOV0N007,  6525, 6956, 6683, 6906, 6959, 7614, 7608 },
	{ TX_LOV0N008,  5971, 6522, 6080, 6326, 6525, 7154, 7125 },
	{ TX_LOV0N009,  6399, 6860, 6537, 6763, 6863, 7501, 7492 },
	{ TX_COMPA186,  8349, 8538, 8720, 8807, 8539, 435, 430 },
	{ TX_COMPA187,  8372, 8546, 8746, 8831, 8547, 444, 441 },
	{ TX_COMPA189,  8320, 8524, 8693, 8776, 8525, 415, 418 },
	{ TX_COMPA190,  8265, 8499, 8638, 8722, 8500, 390, 393 },
	{ TX_COMPA191,  8405, 8564, 8783, 8863, 8565, 460, 463 },
	{ TX_COMPA193,  8295, 8514, 8668, 8751, 8515, 405, 408 },
	{ TX_COMPU186,  9642, 9741, 10183, 10154, 9743, 10414, 10646 },
	{ TX_COMPU187,  9864, 9948, 10438, 10415, 9950, 10659, 10886 },
	{ TX_COMPU188,  7106, 7447, 7347, 7530, 7450, 8138, 8182 },
	{ TX_COMPU189,  9381, 9495, 9900, 9883, 9497, 10145, 10378 },
	{ TX_COMPU190,  8434, 8578, 8822, 8898, 8579, 9184, 9329 },
	{ TX_COMPU192,  8220, 8469, 8586, 8673, 8470, 356, 356 },
	{ TX_COMPU193,  8858, 8987, 9314, 9390, 8988, 9662, 9807 },
	{ TX_DIALOG_ERROR, 12536, 11410, 13405, 13277, 11403, 12290, 12616 },
	{          -1,  0,    0, 0, 0, 0, 0, 0 }
};

extern const RoomText love0Texts[] = {
	{ -1, Common::UNK_LANG, "" }
};

void Room::love0Tick1() {
	// BUGFIX: moved this out of the if statement below.
	playVoc("LOV0LOOP");

	if (!_awayMission->love.alreadyStartedMission) {
		_awayMission->love.canister1 = CANTYPE_O2;
		_awayMission->love.canister2 = CANTYPE_H2;
		_awayMission->love.alreadyStartedMission = true;
	}

	if (_awayMission->love.releasedHumanLaughingGas)
		_awayMission->timers[0] = getRandomWordInRange(200, 400);
	if (_awayMission->love.releasedRomulanLaughingGas)
		_awayMission->timers[1] = getRandomWordInRange(200, 400);
	_awayMission->timers[2] = 200;

	loadActorAnim(OBJECT_DOOR2, "s3r0d2a", 0xe6, 0x80, 0);
	loadActorAnim(OBJECT_DOOR1, "s3r0d1a", 0x123, 0x8d, 0);

	_roomVar.love.cmnXPosToCureSpock = 0xf4;
	_roomVar.love.cmnYPosToCureSpock = 0x8f;

	if (_vm->isFloppyVersion())
		playMidiMusicTracks(MIDITRACK_21, kLoopTypeRepeat);
}

void Room::love0Tick10() {
	if (_vm->isFloppyVersion())
		playMidiMusicTracks(MIDITRACK_21, kLoopTypeRepeat);
}

void Room::love0WalkToDoor2() {
	_awayMission->disableInput = true;
	_roomVar.love.walkingToDoor = 2;
	walkCrewman(OBJECT_KIRK, 0xe6, 0x81, 4);
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_N;
}

void Room::love0OpenDoor2() {
	if (_roomVar.love.walkingToDoor == 2) {
		loadActorAnim(OBJECT_DOOR2, "s3r0d2", 0xe6, 0x80, 3);
		playSoundEffectIndex(kSfxDoor);
	}
}

void Room::love0ReachedDoor2() {
	_roomVar.love.doorOpenCounter++;
	if (_roomVar.love.doorOpenCounter == 2)
		loadRoomIndex(2, 1);
}

void Room::love0WalkToDoor1() {
	_awayMission->disableInput = true;
	_roomVar.love.walkingToDoor = 1;
	walkCrewman(OBJECT_KIRK, 0x125, 0x8d, 5);
}

void Room::love0OpenDoor1() {
	if (_roomVar.love.walkingToDoor == 1) {
		loadActorAnim(OBJECT_DOOR1, "s3r0d1", 0x123, 0x8d, 4);
		playSoundEffectIndex(kSfxDoor);
	}
}

void Room::love0ReachedDoor1() {
	_roomVar.love.doorOpenCounter++;
	if (_roomVar.love.doorOpenCounter == 2)
		loadRoomIndex(1, 3);
}

void Room::love0LookAtConsole() {
	showDescription(TX_LOV0N006);
}

void Room::love0LookAtViewscreen() {
	showDescription(TX_LOV0N000);
}

void Room::love0LookAnywhere() {
	showDescription(TX_LOV0N009);
}

void Room::love0LookAtDoor1() {
	showDescription(TX_LOV0N008);
}

void Room::love0LookAtDoor2() {
	showDescription(TX_LOV0N007);
}

void Room::love0LookAtKirk() {
	showDescription(TX_LOV0N002);
}

void Room::love0LookAtMccoy() {
	showDescription(TX_LOV0N004);
}

void Room::love0LookAtSpock() {
	showDescription(TX_LOV0N005);
}

void Room::love0LookAtRedshirt() {
	showDescription(TX_LOV0N003);
}

void Room::love0TalkToKirk() {
	showText(TX_SPEAKER_KIRK, TX_LOV0_003);
}

void Room::love0TalkToMccoy() {
	showText(TX_SPEAKER_MCCOY, TX_LOV0_007);
}

void Room::love0TalkToSpock() {
	showText(TX_SPEAKER_SPOCK, TX_LOV0_027);
}

void Room::love0TalkToRedshirt() {
	showText(TX_SPEAKER_FERRIS, TX_LOV0_038);
}

void Room::love0UseMTricorderAnywhere() {
	if (_awayMission->love.knowAboutVirus)
		mccoyScan(DIR_N, TX_SPEAKER_MCCOY, TX_LOV0_008, false);
	else
		mccoyScan(DIR_N, TX_SPEAKER_MCCOY, TX_LOV0_006, false);
}

void Room::love0UseSTricorderOnConsole() {
	spockScan(DIR_N, TX_SPEAKER_SPOCK,TX_LOV0_031, false);
}

void Room::love0UseSTricorderAnywhere() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK,TX_LOV0_028, false);
}

void Room::love0UseKirkOnConsole() {
	showText(TX_SPEAKER_KIRK, TX_LOV0_002);
	love0UseSpockOnConsole();
}

void Room::love0UseRedshirtOnConsole() {
	showText(TX_SPEAKER_FERRIS, TX_LOV0_036);
}

void Room::love0UseSpockOnConsole() {
	walkCrewman(OBJECT_SPOCK, 0x9a, 0x9a, 2);
	if (!_awayMission->love.spockAccessedConsole) {
		_awayMission->love.spockAccessedConsole = true;
		_awayMission->love.missionScore += 4;
	}
}

void Room::love0SpockReachedConsole() {
	loadActorAnim2(OBJECT_SPOCK, "susemn", -1, -1, 5);
}

void Room::love0SpockAccessedConsole() {
	playVoc("V6KIRKTY");
	if (_awayMission->love.knowAboutVirus)
		love0InteractWithConsole();
	else {
		showText(TX_SPEAKER_COMPUTER, TX_COMPU188);
		showText(TX_SPEAKER_SPOCK, TX_LOV0_005);
		_roomVar.love.heardSummaryOfVirus = true;
	}
}

void Room::love0UseMccoyOnConsole() {
	walkCrewman(OBJECT_MCCOY, 0x78, 0x98, 3);
}

void Room::love0MccoyReachedConsole() {
	loadActorAnim2(OBJECT_MCCOY, "musemw", -1, -1, 6);
}

void Room::love0MccoyAccessedConsole() {
	playVoc("V6KIRKTY");
	if (!_awayMission->love.mccoyAccessedConsole) {
		_awayMission->love.mccoyAccessedConsole = true;
		_awayMission->love.missionScore += 2;
	}

	if (_awayMission->love.knowAboutVirus)
		love0InteractWithConsole();
	else {
		if (!_roomVar.love.heardSummaryOfVirus) {
			showText(TX_SPEAKER_COMPUTER, TX_COMPU188);
			_roomVar.love.heardSummaryOfVirus = true;
		}
		showText(TX_SPEAKER_MCCOY, TX_LOV0_024);
		showText(TX_SPEAKER_SPOCK, TX_LOV0_035);
		showText(TX_SPEAKER_MCCOY, TX_LOV0_023);
		showText(TX_SPEAKER_KIRK,  TX_LOV0_004);
		showText(TX_SPEAKER_MCCOY, TX_LOV0_009);
		_awayMission->love.knowAboutVirus = true;
	}
}

// Interact with computer console, by selecting topics for the computer to talk about
void Room::love0InteractWithConsole() {
	const TextRef choices[] = {
		TX_SPEAKER_COMPUTER,
		TX_COMPA190,
		TX_COMPA193,
		TX_COMPA189,
		TX_COMPA186,
		TX_COMPA187,
		TX_COMPA191,
		TX_END
	};

	while (true) {
		showText(TX_SPEAKER_COMPUTER, TX_COMPU192);
		int choice = showMultipleTexts(choices);

		switch (choice) {
		case 0:
			showText(TX_SPEAKER_COMPUTER, TX_COMPU190);
			break;
		case 1:
			showText(TX_SPEAKER_COMPUTER, TX_COMPU193);
			break;
		case 2:
			showText(TX_SPEAKER_COMPUTER, TX_COMPU189);
			break;
		case 3:
			showText(TX_SPEAKER_COMPUTER, TX_COMPU186);
			break;
		case 4:
			showText(TX_SPEAKER_COMPUTER, TX_COMPU187);
			break;
		case 5:
			return;
		default:
			showDescription(TX_DIALOG_ERROR);
			break;
		}
	}
}

void Room::love0GetDoorOrConsole() {
	// BUGFIX: There was a problem with "get door 1" where it would execute a bit of
	// non-code before reaching where it was supposed to be. Not sure if it had any actual
	// effect.
	showDescription(TX_LOV0N001);

	// NOTE: There is an unused block of code that's jumped over in the "get door 2"
	// function. (Spock says "that's not logical".)
	//showText(TX_SPEAKER_SPOCK, TX_LOV0_030);
}

}
