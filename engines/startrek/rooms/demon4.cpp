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

namespace StarTrek {

extern const RoomAction demon4ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::demon4Tick1 },
	{ {ACTION_FINISHED_ANIMATION, 1, 0, 0, 0}, &Room::demon4FinishedAnimation1 },
	{ {ACTION_FINISHED_ANIMATION, 2, 0, 0, 0}, &Room::demon4FinishedAnimation2 },
	{ {ACTION_FINISHED_ANIMATION, 3, 0, 0, 0}, &Room::demon4FinishedAnimation3 },
	{ {ACTION_TIMER_EXPIRED, 0, 0, 0, 0}, &Room::demon4Timer0Expired },
	{ {ACTION_TIMER_EXPIRED, 1, 0, 0, 0}, &Room::demon4Timer1Expired },
	{ {ACTION_FINISHED_WALKING, 4, 0, 0, 0}, &Room::demon4CrewmanReachedBeamoutPosition },
	{ {ACTION_TIMER_EXPIRED, 2, 0, 0, 0}, &Room::demon4Timer2Expired },
	{ {ACTION_TIMER_EXPIRED, 3, 0, 0, 0}, &Room::demon4Timer3Expired },
	{ {ACTION_TIMER_EXPIRED, 4, 0, 0, 0}, &Room::demon4Timer4Expired },
	{ {ACTION_TIMER_EXPIRED, 5, 0, 0, 0}, &Room::demon4Timer5Expired },
	{ {ACTION_TIMER_EXPIRED, 6, 0, 0, 0}, &Room::demon4Timer6Expired },

	{ {ACTION_USE, OBJECT_IPHASERK, 8, 0, 0}, &Room::demon4UsePhaserOnNauian },
	{ {ACTION_USE, OBJECT_IPHASERS, 8, 0, 0}, &Room::demon4UsePhaserOnNauian },
	{ {ACTION_USE, OBJECT_IMETAL, 0x22, 0, 0}, &Room::demon4UseMetalOnSecurityEquipment },
	{ {ACTION_FINISHED_WALKING, 5, 0, 0, 0}, &Room::demon4KirkReachedSecurityEquipment },
	{ {ACTION_FINISHED_ANIMATION, 4, 0, 0, 0}, &Room::demon4KirkFinishedUsingSecurityEquipment },
	{ {ACTION_USE, OBJECT_IMETAL, 8, 0, 0}, &Room::demon4UseMetalOnNauian },
	{ {ACTION_FINISHED_WALKING, 1, 0, 0, 0}, &Room::demon4KirkReachedNauian },
	{ {ACTION_USE, OBJECT_ISKULL, 8, 0, 0}, &Room::demon4UseSkullOnNauian },
	{ {ACTION_FINISHED_WALKING, 2, 0, 0, 0}, &Room::demon4KirkReachedNauianWithSkull },

	{ {ACTION_USE, OBJECT_IPHASERS, 0x21, 0, 0}, &Room::demon4UsePhaserOnPanel },
	{ {ACTION_USE, OBJECT_IPHASERS, 0x20, 0, 0}, &Room::demon4UsePhaserOnPattern },
	{ {ACTION_USE, OBJECT_IPHASERS, OBJECT_MCCOY, 0, 0}, &Room::demon4UsePhaserOnMccoy },

	{ {ACTION_FINISHED_ANIMATION, 5, 0, 0, 0}, &Room::demon4TalkToNauian },
	{ {ACTION_TALK, 8, 0, 0, 0}, &Room::demon4TalkToNauian },

	{ {ACTION_LOOK, 0x20, 0, 0, 0}, &Room::demon4LookAtPattern },
	{ {ACTION_LOOK, 8, 0, 0, 0}, &Room::demon4LookAtAlien },
	{ {ACTION_LOOK, 0xff, 0, 0, 0}, &Room::demon4LookAnywhere },
	{ {ACTION_LOOK, 0x22, 0, 0, 0}, &Room::demon4LookAtSecurityEquipment },
	{ {ACTION_LOOK, 0x23, 0, 0, 0}, &Room::demon4LookAtFloor },

	{ {ACTION_LOOK, OBJECT_KIRK, 0, 0, 0}, &Room::demon4LookAtKirk },
	{ {ACTION_LOOK, OBJECT_MCCOY, 0, 0, 0}, &Room::demon4LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_SPOCK, 0, 0, 0}, &Room::demon4LookAtSpock },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::demon4LookAtRedshirt },

	{ {ACTION_LOOK, 10, 0, 0, 0}, &Room::demon4LookAtChamber },
	{ {ACTION_LOOK, 0x21, 0, 0, 0}, &Room::demon4LookAtPanel },

	{ {ACTION_USE, OBJECT_KIRK, 0x21, 0, 0}, &Room::demon4UseKirkOnPanel },
	{ {ACTION_USE, OBJECT_SPOCK, 0x21, 0, 0}, &Room::demon4UseSpockOnPanel },
	{ {ACTION_USE, OBJECT_MCCOY, 0x21, 0, 0}, &Room::demon4UseMccoyOnPanel },
	{ {ACTION_USE, OBJECT_REDSHIRT, 0x21, 0, 0}, &Room::demon4UseRedshirtOnPanel },
	{ {ACTION_FINISHED_WALKING, 3, 0, 0, 0}, &Room::demon4CrewmanReachedPanel },
	{ {ACTION_TALK, OBJECT_KIRK, 0, 0, 0}, &Room::demon4TalkToKirk },
	{ {ACTION_TALK, OBJECT_MCCOY, 0, 0, 0}, &Room::demon4TalkToMccoy },
	{ {ACTION_TALK, OBJECT_SPOCK, 0, 0, 0}, &Room::demon4TalkToSpock },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::demon4TalkToRedshirt },
	{ {ACTION_USE, OBJECT_ISTRICOR, 10, 0, 0}, &Room::demon4UseSTricorderOnChamber },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x20, 0, 0}, &Room::demon4UseSTricorderOnPattern },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0x20, 0, 0}, &Room::demon4UseMTricorderOnPattern },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x21, 0, 0}, &Room::demon4UseSTricorderOnPanel },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0x21, 0, 0}, &Room::demon4UseMTricorderOnPanel },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x23, 0, 0}, &Room::demon4UseSTricorderOnAnything },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0xff, 0, 0}, &Room::demon4UseSTricorderOnAnything },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0x23, 0, 0}, &Room::demon4UseMTricorderOnAnything },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0xff, 0, 0}, &Room::demon4UseMTricorderOnAnything },
	{ {ACTION_USE, OBJECT_ISTRICOR, 8, 0, 0}, &Room::demon4UseSTricorderOnNauian },
	{ {ACTION_USE, OBJECT_IMTRICOR, 8, 0, 0}, &Room::demon4UseMTricorderOnNauian },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x22, 0, 0}, &Room::demon4UseSTricorderOnSecurityEquipment },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0x22, 0, 0}, &Room::demon4UseMTricorderOnSecurityEquipment },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum demon4TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_NAUIAN, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_EVERTS,
	TX_DEM4_001, TX_DEM4_002, TX_DEM4_003, TX_DEM4_004, TX_DEM4_005,
	TX_DEM4_006, TX_DEM4_007, TX_DEM4_008, TX_DEM4_009, TX_DEM4_010,
	TX_DEM4_011, TX_DEM4_012, TX_DEM4_013, TX_DEM4_014, TX_DEM4_015,
	TX_DEM4_017, TX_DEM4_018, TX_DEM4_019, TX_DEM4_020, TX_DEM4_021,
	TX_DEM4_022, TX_DEM4_023, TX_DEM4_024, TX_DEM4_025, TX_DEM4_026,
	TX_DEM4_027, TX_DEM4_029, TX_DEM4_030, TX_DEM4_031, TX_DEM4_032,
	TX_DEM4_033, TX_DEM4_034, TX_DEM4_035, TX_DEM4_036, TX_DEM4_037,
	TX_DEM4_038, TX_DEM4_039, TX_DEM4_040, TX_DEM4_041, TX_DEM4_042,
	TX_DEM4_043, TX_DEM4_044, TX_DEM4_045, TX_DEM4_046, TX_DEM4_047,
	TX_DEM4N000, TX_DEM4N001, TX_DEM4N002, TX_DEM4N003, TX_DEM4N004,
	TX_DEM4N005, TX_DEM4N006, TX_DEM4N007, TX_DEM4N008, TX_DEM4N009,
	TX_DEM4N010, TX_DEM4N011, TX_DEM4N012, TX_DEM4N013, TX_DEM4N014,
	TX_DIALOG_ERROR
};

extern const RoomTextOffsets demon4TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 3902, 3698, 4063, 3989, 3710, 3803, 3826 },
	{ TX_SPEAKER_NAUIAN, 3913, 3709, 4074, 4000, 3721, 3816, 3837 },
	{ TX_SPEAKER_MCCOY, 3920, 3716, 4081, 4007, 3728, 3823, 3844 },
	{ TX_SPEAKER_SPOCK, 3930, 3726, 4091, 4017, 3738, 3833, 3854 },
	{ TX_SPEAKER_EVERTS, 3940, 3736, 4101, 4027, 3748, 3843, 3864 },
	{ TX_DEM4_001, 2980, 2806, 3120, 3048, 2818, 2891, 2913 },
	{ TX_DEM4_002, 4032, 3813, 4201, 4137, 3825, 3938, 3951 },
	{ TX_DEM4_003, 9925, 9058, 10924, 10595, 9073, 9772, 10015 },
	{ TX_DEM4_004, 4097, 3884, 4289, 4241, 3896, 4032, 4040 },
	{ TX_DEM4_005, 9991, 9109, 10994, 10668, 9124, 9830, 10070 },
	{ TX_DEM4_006, 9774, 8922, 10751, 10447, 8937, 9639, 9858 },
	{ TX_DEM4_007, 5073, 4770, 5460, 5308, 4782, 5009, 5121 },
	{ TX_DEM4_008, 5030, 4742, 5418, 5267, 4754, 4982, 5094 },
	{ TX_DEM4_009, 4210, 3982, 4409, 4364, 3994, 4140, 4145 },
	{ TX_DEM4_010,  596, 570, 596, 596, 570, 570, 570 },
	{ TX_DEM4_011, 1055, 970, 1080, 1074, 982, 1005, 1007 },
	{ TX_DEM4_012, 3195, 3006, 3347, 3276, 3018, 3105, 3125 },
	{ TX_DEM4_013, 8780, 8031, 9574, 9347, 8048, 8656, 8785 },
	{ TX_DEM4_014, 7484, 6855, 8198, 7897, 6867, 7323, 7531 },
	{ TX_DEM4_015, 6822, 6309, 7422, 7189, 6321, 6717, 6875 },
	{ TX_DEM4_017, 9415, 8593, 10352, 10077, 8608, 9304, 9488 },
	{ TX_DEM4_018, 8903, 8124, 9708, 9470, 8141, 8753, 8889 },
	{ TX_DEM4_019, 7104, 6546, 7769, 7489, 6558, 6991, 7177 },
	{ TX_DEM4_020, 7376, 6762, 8094, 7790, 6774, 7232, 7442 },
	{ TX_DEM4_021, 7334, 6731, 8040, 7740, 6743, 7197, 7403 },
	{ TX_DEM4_022, 7172, 6599, 7848, 7569, 6611, 7056, 7241 },
	{ TX_DEM4_023, 7999, 7340, 8747, 8483, 7356, 7880, 8049 },
	{ TX_DEM4_024, 6882, 6354, 7488, 7260, 6366, 6773, 6926 },
	{ TX_DEM4_025, 7756, 7112, 8491, 8209, 7128, 7620, 7808 },
	{ TX_DEM4_026, 8239, 7550, 9019, 8754, 7566, 8121, 8290 },
	{ TX_DEM4_027, 8146, 7472, 8912, 8657, 7488, 8039, 8198 },
	{ TX_DEM4_029, 8429, 7725, 9213, 8978, 7741, 8330, 8469 },
	{ TX_DEM4_030,  914, 858, 936, 930, 858, 874, 880 },
	{ TX_DEM4_031, 1971, 1842, 2012, 2002, 1854, 1889, 1895 },
	{ TX_DEM4_032, 8718, 7984, 9489, 9281, 8001, 8603, 8715 },
	{ TX_DEM4_033, 8637, 7918, 9405, 9191, 7935, 8528, 8646 },
	{ TX_DEM4_034, 9051, 8244, 9870, 9658, 8259, 8892, 9021 },
	{ TX_DEM4_035,  755, 714, 776, 767, 714, 722, 735 },
	{ TX_DEM4_036, 9539, 8702, 10492, 10216, 8717, 9428, 9613 },
	{ TX_DEM4_037, 6019, 5656, 6507, 6322, 5668, 5978, 6109 },
	{ TX_DEM4_038, 5821, 5473, 6301, 6095, 5485, 5761, 5917 },
	{ TX_DEM4_039, 4845, 4572, 5213, 5054, 4584, 4782, 4904 },
	{ TX_DEM4_040, 5272, 4939, 5678, 5530, 4951, 5211, 5309 },
	{ TX_DEM4_041, 5130, 4812, 5521, 5380, 4824, 5066, 5167 },
	{ TX_DEM4_042, 4310, 4067, 4543, 4479, 4079, 4240, 4264 },
	{ TX_DEM4_043, 4476, 4218, 4746, 4641, 4230, 4386, 4452 },
	{ TX_DEM4_044, 3955, 3751, 4116, 4042, 3763, 3858, 3881 },
	{ TX_DEM4_045, 8836, 8072, 9636, 9406, 8089, 8704, 8832 },
	{ TX_DEM4_046, 7055, 6512, 7705, 7431, 6524, 6948, 7128 },
	{ TX_DEM4_047, 7305, 6717, 7997, 7709, 6729, 7181, 7375 },
	{ TX_DEM4N000, 6576, 6138, 7147, 6928, 6150, 6531, 6675 },
	{ TX_DEM4N001, 6667, 6199, 7240, 7024, 6211, 6597, 6738 },
	{ TX_DEM4N002, 6301, 5923, 6856, 6635, 5935, 6276, 6443 },
	{ TX_DEM4N003, 6765, 6267, 7365, 7135, 6279, 6678, 6833 },
	{ TX_DEM4N004, 6715, 6232, 7296, 7076, 6244, 6634, 6779 },
	{ TX_DEM4N005, 6613, 6160, 7188, 6969, 6172, 6557, 6701 },
	{ TX_DEM4N006, 10194, 9282, 11203, 10865, 9297, 10005, 10249 },
	{ TX_DEM4N007, 6491, 6068, 7058, 6821, 6080, 6432, 6600 },
	{ TX_DEM4N008, 6419, 6011, 6974, 6743, 6023, 6361, 6531 },
	{ TX_DEM4N009, 10116, 9219, 11112, 10781, 9234, 9936, 10173 },
	{ TX_DEM4N010, 6365, 5972, 6919, 6694, 5984, 6328, 6491 },
	{ TX_DEM4N011, 8924, 8130, 9730, 9493, 8147, 8762, 8896 },
	{ TX_DEM4N012, 2666, 2522, 2778, 2729, 2534, 2602, 2601 },
	{ TX_DEM4N013, 2868, 2709, 3005, 2936, 2721, 2794, 2813 },
	{ TX_DEM4N014, 1602, 1488, 1630, 1630, 1500, 1532, 1528 },
	{ TX_DIALOG_ERROR, 10252, 9325, 11268, 10920, 9340, 10045, 10299 },
	{          -1, 0,    0, 0, 0, 0, 0, 0 }
};

extern const RoomText demon4Texts[] = {
	{ -1, Common::UNK_LANG, "" }
};

void Room::demon4Tick1() {
	playVoc("DEM4LOOP");

	if (!_awayMission->demon.healedMiner)
		_awayMission->demon.minerDead = true;

	if (_awayMission->demon.solvedSunPuzzle) {
		loadActorAnim(10, "bxrise2", 0x122, 0x91, 0);
		loadActorAnim(8, "stpout2", 0x107, 0x92, 0);
		_roomVar.demon.nauianEmerged = true;

		if ((_awayMission->demon.itemsTakenFromCase & 0x10) && !_awayMission->demon.gaveSkullToNauian)
			_awayMission->timers[6] = 20;
	}

	if (!_awayMission->demon.foundAlienRoom) {
		_awayMission->demon.foundAlienRoom = true;
		playMidiMusicTracks(MIDITRACK_1);
	}
}

// Alien emerged
void Room::demon4FinishedAnimation1() {
	loadActorAnim(8, "stpout", 0x107, 0x92, 5);
	_roomVar.demon.nauianEmerged = true;
	if (_awayMission->demon.itemsTakenFromCase & 0x10)
		_awayMission->timers[6] = 45;
}

void Room::demon4FinishedAnimation2() {
	if (!_roomVar.demon.cd) {
		_roomVar.demon.cd = true;
		showDescription(TX_DEM4N011);
	}

	showGameOverMenu();

	// WORKAROUND: original game has the below line; however, it's never executed anyway
	// since the game over menu manipulates the stack. Here, the menu could delete this
	// room object, so it should be the last statement...
	//_awayMission->disableInput = false;
}

void Room::demon4FinishedAnimation3() {
	showText(TX_SPEAKER_NAUIAN, TX_DEM4_034);
	showText(TX_SPEAKER_KIRK,   TX_DEM4_010);
	showText(TX_SPEAKER_NAUIAN, TX_DEM4_035);
	showText(TX_SPEAKER_SPOCK,  TX_DEM4_030);

	_awayMission->disableInput = true;
	_awayMission->demon.missionScore += 5;
	_awayMission->timers[1] = 20;
}

// Just solved sun puzzle
void Room::demon4Timer0Expired() {
	loadActorAnim(10, "bxrise", 0x122, 0x91, 1);
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_E;
	loadActorStandAnim(OBJECT_KIRK);
	playVoc("SE2POWER");
}

// Mission end
void Room::demon4Timer1Expired() {
	showText(TX_SPEAKER_KIRK, TX_DEM4_011);

	walkCrewman(OBJECT_KIRK, 0xbe, 0xa5, 4);
	walkCrewman(OBJECT_SPOCK, 0xb4, 0x9b, 4);
	walkCrewman(OBJECT_MCCOY, 0xc8, 0x9b, 4);

	if (!_awayMission->redshirtDead)
		walkCrewman(OBJECT_REDSHIRT, 0xc1, 0x91, 4);
	else
		_roomVar.demon.crewReadyToBeamOut++;
}

void Room::demon4CrewmanReachedBeamoutPosition() {
	_roomVar.demon.crewReadyToBeamOut++;
	if (_roomVar.demon.crewReadyToBeamOut != 4)
		return;

	if (!_awayMission->demon.insultedStephen)
		_awayMission->demon.missionScore += 3;
	if (!_awayMission->redshirtDead)
		_awayMission->demon.missionScore += 2;

	endMission(_awayMission->demon.missionScore, 0x24, 0);
}

void Room::demon4Timer2Expired() {
	loadActorAnim2(OBJECT_SPOCK, "skille", -1, -1, 0);
}

void Room::demon4Timer3Expired() {
	loadActorAnim2(OBJECT_REDSHIRT, "rkille", -1, -1, 0);
}

void Room::demon4Timer4Expired() {
	loadActorAnim2(OBJECT_MCCOY, "mkille", -1, -1, 0);
}

void Room::demon4Timer5Expired() {
	loadActorAnim2(OBJECT_KIRK, "kkille", -1, -1, 2);
}

void Room::demon4Timer6Expired() {
	showText(TX_SPEAKER_MCCOY, TX_DEM4_017);
}

void Room::demon4UsePhaserOnNauian() {
	if (!_roomVar.demon.triedToShootNauian) { // Lenient the first time
		showText(TX_SPEAKER_NAUIAN, TX_DEM4_033);
		_roomVar.demon.triedToShootNauian = true;
	} else { // Murdery the second time
		showText(TX_SPEAKER_NAUIAN, TX_DEM4_032);
		loadActorAnim2(10, "bxfire", 0, 0, 2);
		playVoc("V0SPOCKT");
		_awayMission->disableInput = true;
		_awayMission->timers[2] = 7;
		_awayMission->timers[3] = 8;
		_awayMission->timers[4] = 7;
		_awayMission->timers[5] = 8;
	}
}

void Room::demon4UseMetalOnSecurityEquipment() {
	_awayMission->disableInput = true;
	walkCrewman(OBJECT_KIRK, 0xf5, 0x90, 5);
}

void Room::demon4KirkReachedSecurityEquipment() {
	loadActorAnim2(OBJECT_KIRK, "kusemn", -1, -1, 4);
}

void Room::demon4KirkFinishedUsingSecurityEquipment() {
	_awayMission->disableInput = false;
	showDescription(TX_DEM4N014);
}

void Room::demon4UseMetalOnNauian() {
	walkCrewman(OBJECT_KIRK, 0xe9, 0x90, 1);
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_E;
}

void Room::demon4KirkReachedNauian() {
	loadActorAnim2(8, "usekey", 0x107, 0x8e, 3);
	loseItem(OBJECT_IMETAL);
	_awayMission->demon.missionScore += 2;
	_awayMission->demon.itemsTakenFromCase &= ~1;
}

void Room::demon4UseSkullOnNauian() {
	walkCrewman(OBJECT_KIRK, 0xe9, 0x90, 2);
}

void Room::demon4KirkReachedNauianWithSkull() {
	showText(TX_SPEAKER_NAUIAN, TX_DEM4_036);

	const TextRef choices[] = { TX_SPEAKER_KIRK, TX_DEM4_006, TX_DEM4_003, TX_DEM4_005, TX_END };
	int choice = showMultipleTexts(choices);

	switch (choice) {
	case 0:
		_awayMission->demon.missionScore++;
		loadActorAnim2(8, "takesk", 0x107, 0x8e, 0);
		loseItem(OBJECT_ISKULL);
		_awayMission->demon.itemsTakenFromCase &= ~16; // BUG: skull reappears in case? Can abuse for infinite ponits?
		_awayMission->demon.gaveSkullToNauian = true;
		break;

	case 1:
	case 2:
		break;

	default:
		showDescription(TX_DIALOG_ERROR);
		break;
	}
}

void Room::demon4UsePhaserOnPanel() {
	showText(TX_SPEAKER_SPOCK, TX_DEM4_013);
}

void Room::demon4UsePhaserOnPattern() {
	showText(TX_SPEAKER_SPOCK, TX_DEM4_045);
}

void Room::demon4UsePhaserOnMccoy() {
	showText(TX_SPEAKER_MCCOY, TX_DEM4_018);
}

void Room::demon4TalkToNauian() {
	_awayMission->disableInput = false;

	if (!_awayMission->demon.metNauian) {
		_awayMission->demon.metNauian = true;
		showText(TX_SPEAKER_NAUIAN, TX_DEM4_044);

		const TextRef choices[] = { TX_SPEAKER_KIRK, TX_DEM4_002, TX_DEM4_004, TX_DEM4_009, TX_END };
		int choice = showMultipleTexts(choices);

		switch (choice) {
		case 0:
			_awayMission->demon.field2d = true;
			showText(TX_SPEAKER_NAUIAN, TX_DEM4_042);
			break;

		case 1:
			break;

		case 2:
			showText(TX_SPEAKER_NAUIAN, TX_DEM4_039);
			break;

		default:
			showDescription(TX_DIALOG_ERROR);
			break;
		}

		showText(TX_SPEAKER_NAUIAN, TX_DEM4_043);

		const TextRef choices2[] = { TX_SPEAKER_KIRK, TX_DEM4_008, TX_DEM4_007, TX_END };
		choice = showMultipleTexts(choices2);

		switch (choice) {
		case 0:
			_awayMission->demon.field2d = true;
			showText(TX_SPEAKER_NAUIAN, TX_DEM4_041);
			break;
		case 1:
			break;
		default:
			showDescription(TX_DIALOG_ERROR);
			break;
		}

		showText(TX_SPEAKER_NAUIAN, TX_DEM4_040);

		if (_roomVar.demon.disabledSecurity) { // Impossible condition?
			showText(TX_SPEAKER_NAUIAN, TX_DEM4_037);

			// BUGFIX: Originally all of these used no audio, despite the files existing
			// (being used elsewhere).
			showText(TX_SPEAKER_KIRK,   TX_DEM4_010);
			showText(TX_SPEAKER_NAUIAN, TX_DEM4_035);
			showText(TX_SPEAKER_SPOCK,  TX_DEM4_030);

			_awayMission->disableInput = true;
			_awayMission->demon.missionScore += 5;
			_awayMission->timers[1] = 20;
		} else {
			showText(TX_SPEAKER_NAUIAN, TX_DEM4_038);
		}
	} else {
		showText(TX_SPEAKER_NAUIAN, TX_DEM4_031);
	}
}

void Room::demon4LookAtPattern() {
	showDescription(TX_DEM4N002);
}

void Room::demon4LookAtAlien() {
	showDescription(TX_DEM4N007);
}

void Room::demon4LookAnywhere() {
	showDescription(TX_DEM4N004);
}

void Room::demon4LookAtSecurityEquipment() {
	showDescription(TX_DEM4N008);
}

void Room::demon4LookAtFloor() {
	showDescription(TX_DEM4N009);
}

void Room::demon4LookAtKirk() {
	showDescription(TX_DEM4N003);
}

void Room::demon4LookAtMccoy() {
	showDescription(TX_DEM4N000);
}

void Room::demon4LookAtSpock() {
	showDescription(TX_DEM4N005);
}

void Room::demon4LookAtRedshirt() {
	showDescription(TX_DEM4N001);
}

void Room::demon4LookAtChamber() {
	showDescription(TX_DEM4N006);
}

void Room::demon4LookAtPanel() {
	showDescription(TX_DEM4N012);
}

void Room::demon4UseKirkOnPanel() {
	_roomVar.demon.crewmanUsingPanel = OBJECT_KIRK;
	demon4UseCrewmanOnPanel();
}

void Room::demon4UseSpockOnPanel() {
	_roomVar.demon.crewmanUsingPanel = OBJECT_SPOCK;
	demon4UseCrewmanOnPanel();
}

void Room::demon4UseMccoyOnPanel() {
	_roomVar.demon.crewmanUsingPanel = OBJECT_MCCOY;
	demon4UseCrewmanOnPanel();
}

void Room::demon4UseRedshirtOnPanel() {
	_roomVar.demon.crewmanUsingPanel = OBJECT_REDSHIRT;
	demon4UseCrewmanOnPanel();
}

void Room::demon4UseCrewmanOnPanel() {
	if (_awayMission->demon.solvedSunPuzzle)
		return;
	walkCrewman(_roomVar.demon.crewmanUsingPanel, 0xda, 0x83, 3);
	_awayMission->crewDirectionsAfterWalk[_roomVar.demon.crewmanUsingPanel] = DIR_N;
	_awayMission->disableInput = true;
}

void Room::demon4CrewmanReachedPanel() {
	if (demon4ShowSunPuzzle()) {
		_awayMission->demon.solvedSunPuzzle = true;
		loadActorAnim(9, "ctrl", 0, 0, 0);
		_awayMission->demon.missionScore += 3;
		_awayMission->timers[0] = 10;
	} else
		showDescription(TX_DEM4N013);

	walkCrewman(_roomVar.demon.crewmanUsingPanel, 0xae, 0x93, 0);
	_awayMission->disableInput = false;
}

bool Room::demon4ShowSunPuzzle() {
	bool solved = false;

	int sliderY = 17;
	int sliderR = -17;
	int sliderB = 17;

	_vm->_gfx->fadeoutScreen();
	_vm->_sound->stopAllVocSounds();

	_vm->_gfx->pushSprites();
	_vm->_gfx->setBackgroundImage("machineb");
	_vm->_gfx->copyBackgroundScreen();
	_vm->_gfx->clearPri();

	Sprite sprites[21];

	for (int i = 0; i < 21; i++) {
		_vm->_gfx->addSprite(&sprites[i]);
	}

	sprites[0].setBitmap(loadBitmapFile("levery"));
	sprites[1].setBitmap(loadBitmapFile("leverr"));
	sprites[2].setBitmap(loadBitmapFile("leverb"));

	Bitmap *lightyBitmap = new Bitmap(loadBitmapFile("lighty"));
	Bitmap *lightrBitmap = new Bitmap(loadBitmapFile("lightr"));
	Bitmap *lightbBitmap = new Bitmap(loadBitmapFile("lightb"));

	for (int i = 3; i < 9; i++)
		sprites[i].setBitmap(lightyBitmap);
	for (int i = 9; i < 15; i++)
		sprites[i].setBitmap(lightrBitmap);
	for (int i = 15; i < 21; i++)
		sprites[i].setBitmap(lightbBitmap);

	Sprite doneButtonSprite;
	_vm->_gfx->addSprite(&doneButtonSprite);
	doneButtonSprite.setXYAndPriority(0x104, 0x64, 2);
	doneButtonSprite.setBitmap(loadBitmapFile("donebutt"));
	// BUGFIX: use draw mode 2 so the entire button is clickable (not just non-transparent
	// pixels)
	doneButtonSprite.drawMode = 2;

	_vm->_gfx->fadeinScreen();

	bool continueLoop = true;
	bool sliderChanged = true;

	while (continueLoop) {
		if (sliderChanged) {
			sprites[0].setXYAndPriority(0xa0, sliderY + 0x75, 14);
			sprites[1].setXYAndPriority(0xa8, sliderR + 0x75, 14);
			sprites[2].setXYAndPriority(0xb0, sliderB + 0x75, 14);

			int y = 0x82;
			int x = 0x83;
			int start = 3;
			int numSprites = abs(sliderY) / 3;
			for (int i = 0; i < 6; i++) {
				sprites[i + start].setXYAndPriority(x, y, 14);
			}
			y -= 6;
			for (int i = 0; i < numSprites; i++) {
				sprites[i + start].setXYAndPriority(x, y, 14);
				y -= 6;
			}

			y = 0x82;
			x = 0x8b;
			start = 9;
			numSprites = abs(sliderR) / 3;
			for (int i = 0; i < 6; i++) {
				sprites[i + start].setXYAndPriority(x, y, 14);
			}
			y -= 6;
			for (int i = 0; i < numSprites; i++) {
				sprites[i + start].setXYAndPriority(x, y, 14);
				y -= 6;
			}

			y = 0x82;
			x = 0x93;
			start = 15;
			numSprites = abs(sliderB) / 3;
			for (int i = 0; i < 6; i++) {
				sprites[i + start].setXYAndPriority(x, y, 14);
			}
			y -= 6;
			for (int i = 0; i < numSprites; i++) {
				sprites[i + start].setXYAndPriority(x, y, 14);
				y -= 6;
			}

			sliderChanged = false;
		}

		TrekEvent event;
		if (!_vm->popNextEvent(&event))
			continue;

		switch (event.type) {
		case TREKEVENT_LBUTTONDOWN: {
			Common::Point mousePos = _vm->_gfx->getMousePos();
			if (_vm->_gfx->getSpriteAt(mousePos) == &doneButtonSprite) {
				solved = (abs(sliderY) <= 2 && abs(sliderR) <= 2 && abs(sliderB) <= 2);
				continueLoop = false;
			} else {
				if (mousePos.y >= 0x64 && mousePos.y <= 0x86) {
					if (mousePos.x >= 0xa0 && mousePos.x <= 0xa6)
						sliderY = mousePos.y - 0x75;
					else if (mousePos.x >= 0xa8 && mousePos.x <= 0xae)
						sliderR = mousePos.y - 0x75;
					else if (mousePos.x >= 0xb0 && mousePos.x <= 0xb6)
						sliderB = mousePos.y - 0x75;
					sliderChanged = true;
				}
			}
			break;
		}

		case TREKEVENT_RBUTTONDOWN:
			solved = (abs(sliderY) <= 2 && abs(sliderR) <= 2 && abs(sliderB) <= 2);
			continueLoop = false;
			break;

		case TREKEVENT_KEYDOWN:
			switch (event.kbd.keycode) {
			case Common::KEYCODE_ESCAPE:
			case Common::KEYCODE_F2:
				solved = (abs(sliderY) <= 2 && abs(sliderR) <= 2 && abs(sliderB) <= 2);
				continueLoop = false;
				break;

			case Common::KEYCODE_RETURN:
			case Common::KEYCODE_KP_ENTER:
			case Common::KEYCODE_F1: {
				// Same as TREKEVENT_LBUTTONDOWN
				Common::Point mousePos = _vm->_gfx->getMousePos();
				if (_vm->_gfx->getSpriteAt(mousePos) == &doneButtonSprite) {
					solved = (abs(sliderY) <= 2 && abs(sliderR) <= 2 && abs(sliderB) <= 2);
					continueLoop = false;
				} else {
					if (mousePos.y >= 0x64 && mousePos.y <= 0x86) {
						if (mousePos.x >= 0xa0 && mousePos.x <= 0xa6)
							sliderY = mousePos.y - 0x75;
						else if (mousePos.x >= 0xa8 && mousePos.x <= 0xae)
							sliderR = mousePos.y - 0x75;
						else if (mousePos.x >= 0xb0 && mousePos.x <= 0xb6)
							sliderB = mousePos.y - 0x75;
						sliderChanged = true;
					}
				}
			}

			default:
				break;
			}
			break;

		default:
			break;
		}

		_vm->_gfx->drawAllSprites();
	}

	_vm->_gfx->fadeoutScreen();
	_vm->_gfx->popSprites();
	_vm->_gfx->loadPri(_vm->getScreenName());
	_vm->_gfx->setBackgroundImage(_vm->getScreenName());
	_vm->_gfx->copyBackgroundScreen();
	_vm->_gfx->forceDrawAllSprites();

	for (int i = 0; i < 21; i++) {
		sprites[i].bitmap = nullptr;
	}

	delete lightbBitmap;
	delete lightrBitmap;
	delete lightyBitmap;

	return solved;
}

void Room::demon4TalkToKirk() {
	showText(TX_SPEAKER_KIRK, TX_DEM4_001);
}

void Room::demon4TalkToMccoy() {
	if (_roomVar.demon.nauianEmerged)
		showText(TX_SPEAKER_MCCOY, TX_DEM4_019);
	else
		showText(TX_SPEAKER_MCCOY, TX_DEM4_015);
}

void Room::demon4TalkToSpock() {
	if (_roomVar.demon.nauianEmerged)
		showText(TX_SPEAKER_SPOCK, TX_DEM4_022);
	else
		showText(TX_SPEAKER_SPOCK, TX_DEM4_024);
}

void Room::demon4TalkToRedshirt() {
	if (_roomVar.demon.nauianEmerged)
		showText(TX_SPEAKER_EVERTS, TX_DEM4_047);
	else
		showText(TX_SPEAKER_EVERTS, TX_DEM4_046);
}

void Room::demon4UseSTricorderOnChamber() {
	loadActorAnim2(OBJECT_SPOCK, "sscane", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM4_012);
}

void Room::demon4UseSTricorderOnPattern() {
	loadActorAnim2(OBJECT_SPOCK, "sscane", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM4_014);
	showText(TX_SPEAKER_SPOCK, TX_DEM4_025);
}

void Room::demon4UseMTricorderOnPattern() {
	showText(TX_SPEAKER_MCCOY, TX_DEM4_021);
}

void Room::demon4UseSTricorderOnPanel() {
	loadActorAnim2(OBJECT_SPOCK, "sscane", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM4_027);
}

void Room::demon4UseMTricorderOnPanel() {
	loadActorAnim2(OBJECT_MCCOY, "mscane", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_MCCOY, TX_DEM4_027); // BUG: Uses spock's voice (though speaker says "Mccoy")
}

void Room::demon4UseSTricorderOnAnything() {
	loadActorAnim2(OBJECT_SPOCK, "sscane", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM4_023);
}

void Room::demon4UseMTricorderOnAnything() {
	loadActorAnim2(OBJECT_MCCOY, "mscane", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_MCCOY, TX_DEM4_020);
}

void Room::demon4UseSTricorderOnNauian() {
	loadActorAnim2(OBJECT_SPOCK, "sscane", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM4_029);
}

void Room::demon4UseMTricorderOnNauian() {
	loadActorAnim2(OBJECT_MCCOY, "mscane", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM4_029); // BUG: Uses spock's voice (speaker says "Spock" too)
}

void Room::demon4UseSTricorderOnSecurityEquipment() {
	loadActorAnim2(OBJECT_SPOCK, "sscane", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM4_026);
}

void Room::demon4UseMTricorderOnSecurityEquipment() {
	loadActorAnim2(OBJECT_MCCOY, "mscane", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM4_026); // BUG: Uses spock's voice (speaker says "Spock" too)
}

} // namespace StarTrek
