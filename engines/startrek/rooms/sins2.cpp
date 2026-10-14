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

#define OBJECT_DOOR 8

#define HOTSPOT_TERMINAL 0x20
#define HOTSPOT_DOOR 0x21
#define HOTSPOT_KEYPAD 0x22

namespace StarTrek {

extern const RoomAction sins2ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::sins2Tick1 },
	{ {ACTION_USE, OBJECT_IPHASERK, HOTSPOT_DOOR, 0, 0}, &Room::sins2UsePhaserOnDoor },
	{ {ACTION_USE, OBJECT_IPHASERS, HOTSPOT_DOOR, 0, 0}, &Room::sins2UsePhaserOnDoor },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0xff,         0, 0}, &Room::sins2UseSTricorderAnywhere },

	{ {ACTION_USE, OBJECT_KIRK,  HOTSPOT_TERMINAL,    0, 0}, &Room::sins2UseKirkOnTerminal },
	{ {ACTION_USE, OBJECT_SPOCK, HOTSPOT_TERMINAL,    0, 0}, &Room::sins2UseSpockOnTerminal },
	{ {ACTION_DONE_WALK, 5,                        0, 0, 0}, &Room::sins2SpockReachedTerminal },
	{ {ACTION_DONE_ANIM, 6,                        0, 0, 0}, &Room::sins2SpockUsedTerminal },
	{ {ACTION_USE, OBJECT_MCCOY,    HOTSPOT_TERMINAL, 0, 0}, &Room::sins2UseMccoyOnTerminal },
	{ {ACTION_USE, OBJECT_REDSHIRT, HOTSPOT_TERMINAL, 0, 0}, &Room::sins2UseRedshirtOnTerminal },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_TERMINAL, 0, 0}, &Room::sins2UseSTricorderOnTerminal },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_KEYPAD,   0, 0}, &Room::sins2UseSTricorderOnKeypad },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_DOOR,     0, 0}, &Room::sins2UseSTricorderOnDoor },
	{ {ACTION_USE, OBJECT_KIRK,     HOTSPOT_KEYPAD,   0, 0}, &Room::sins2UseKirkOnKeypad },
	{ {ACTION_USE, OBJECT_SPOCK,    HOTSPOT_KEYPAD,   0, 0}, &Room::sins2UseSpockOnKeypad },
	{ {ACTION_DONE_WALK, 2,                        0, 0, 0}, &Room::sins2SpockReachedKeypad },
	{ {ACTION_DONE_ANIM, 3,                        0, 0, 0}, &Room::sins2EnteredCorrectCode },
	{ {ACTION_DONE_ANIM, 4,                        0, 0, 0}, &Room::sins2EnteredIncorrectCode },
	{ {ACTION_DONE_ANIM, 1,                        0, 0, 0}, &Room::sins2DoorFinishedOpening },

	{ {ACTION_TICK, 40,               0, 0, 0}, &Room::sins2Tick40 },
	{ {ACTION_LOOK, 0xff,             0, 0, 0}, &Room::sins2LookAnywhere },
	{ {ACTION_LOOK, OBJECT_DOOR,      0, 0, 0}, &Room::sins2LookAtOpenDoor },
	{ {ACTION_LOOK, HOTSPOT_TERMINAL, 0, 0, 0}, &Room::sins2LookAtTerminal },
	{ {ACTION_LOOK, HOTSPOT_KEYPAD,   0, 0, 0}, &Room::sins2LookAtKeypad },
	{ {ACTION_LOOK, OBJECT_KIRK,      0, 0, 0}, &Room::sins2LookAtKirk },
	{ {ACTION_LOOK, OBJECT_SPOCK,     0, 0, 0}, &Room::sins2LookAtSpock },
	{ {ACTION_LOOK, OBJECT_MCCOY,     0, 0, 0}, &Room::sins2LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_REDSHIRT,  0, 0, 0}, &Room::sins2LookAtRedshirt },
	{ {ACTION_TALK, OBJECT_KIRK,      0, 0, 0}, &Room::sins2TalkToKirk },
	{ {ACTION_TALK, OBJECT_SPOCK,     0, 0, 0}, &Room::sins2TalkToSpock },
	{ {ACTION_TALK, OBJECT_MCCOY,     0, 0, 0}, &Room::sins2TalkToMccoy },
	{ {ACTION_TALK, OBJECT_REDSHIRT,  0, 0, 0}, &Room::sins2TalkToRedshirt },
	{ {ACTION_LOOK, HOTSPOT_DOOR,     0, 0, 0}, &Room::sins2LookAtDoor },

	{ {ACTION_USE, OBJECT_SPOCK,    HOTSPOT_DOOR,    0, 0}, &Room::sins2UseSpockOnDoor },
	{ {ACTION_USE, OBJECT_ICOMM,    0xff,            0, 0}, &Room::sins2UseCommunicator },
	{ {ACTION_USE, OBJECT_IMEDKIT,  OBJECT_KIRK,     0, 0}, &Room::sins2UseMedkitOnCrewman },
	{ {ACTION_USE, OBJECT_IMEDKIT,  OBJECT_SPOCK,    0, 0}, &Room::sins2UseMedkitOnCrewman },
	{ {ACTION_USE, OBJECT_IMEDKIT,  OBJECT_MCCOY,    0, 0}, &Room::sins2UseMedkitOnCrewman },
	{ {ACTION_USE, OBJECT_IMEDKIT,  OBJECT_REDSHIRT, 0, 0}, &Room::sins2UseMedkitOnCrewman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_KIRK,     0, 0}, &Room::sins2UseMTricorderOnCrewman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_SPOCK,    0, 0}, &Room::sins2UseMTricorderOnCrewman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_MCCOY,    0, 0}, &Room::sins2UseMTricorderOnCrewman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_REDSHIRT, 0, 0}, &Room::sins2UseMTricorderOnCrewman },
	{ {ACTION_USE, OBJECT_MCCOY,    HOTSPOT_KEYPAD,  0, 0}, &Room::sins2UseMccoyOnKeypad },
	{ {ACTION_USE, OBJECT_REDSHIRT, HOTSPOT_KEYPAD,  0, 0}, &Room::sins2UseRedshirtOnKeypad },

	{ {ACTION_WALK, HOTSPOT_DOOR, 0, 0, 0}, &Room::sins2WalkToDoor },
	{ {ACTION_WALK, OBJECT_DOOR,  0, 0, 0}, &Room::sins2WalkToDoor },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum sins2TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_MOSHER,
	TX_SPEAKER_SCOTT, TX_SPEAKER_UHURA,
	TX_SIN2_001, TX_SIN2_002, TX_SIN2_003, TX_SIN2_004, TX_SIN2_005,
	TX_SIN2_006, TX_SIN2_007, TX_SIN2_008, TX_SIN2_009, TX_SIN2_010,
	TX_SIN2_012, TX_SIN2_013, TX_SIN2_014, TX_SIN2_015, TX_SIN2_016,
	TX_SIN2_017, TX_SIN2_019, TX_SIN2_020, TX_SIN2_021, TX_SIN2_022,
	TX_SIN2_023, TX_SIN2_024, TX_SIN2_025, TX_SIN2_026, TX_SIN2_027,
	TX_SIN2_028, TX_SIN2_029, TX_SIN2_030, TX_SIN2_031, TX_SIN2_032,
	TX_SIN2_033, TX_SIN2_034, TX_SIN2_035, TX_SIN2_037, TX_SIN2_038,
	TX_SIN2_039, TX_SIN2_040, TX_SIN2_041, TX_SIN2_042, TX_SIN2_043,
	TX_SIN2_044, TX_SIN2_045, TX_SIN2_046, TX_SIN2_047, TX_SIN2_048,
	TX_SIN2_049, TX_SIN2_050, TX_SIN2_F12, TX_SIN2_F26, TX_SIN2_S05,
	TX_SIN2_S16, TX_SIN3_008, TX_SIN1U070, TX_SIN2N000, TX_SIN2N001,
	TX_SIN2N002, TX_SIN2N003, TX_SIN2N004, TX_SIN2N005, TX_SIN2N006,
	TX_SIN2N007, TX_SIN2N008, TX_SIN2U085, TX_SIN2U096, TX_SIN2U097
};

extern const RoomTextOffsets sins2TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 301, 301, 301, 301, 301, 301, 301, 301, 301 },
	{ TX_SPEAKER_MCCOY, 312, 312, 312, 312, 312, 314, 312, 312, 312 },
	{ TX_SPEAKER_SPOCK, 322, 322, 322, 322, 322, 324, 322, 322, 322 },
	{ TX_SPEAKER_MOSHER, 332, 332, 332, 332, 332, 334, 332, 332, 332 },
	{ TX_SPEAKER_SCOTT, 356, 356, 356, 356, 356, 358, 358, 356, 356 },
	{ TX_SPEAKER_UHURA, 346, 346, 346, 346, 346, 348, 348, 346, 346 },
	{ TX_SIN2_001, 4324, 4214, 4152, 4274, 4215, 4208, 4346, 4215, 4320 },
	{ TX_SIN2_002, 892, 836, 899, 873, 836, 819, 845, 836, 888 },
	{ TX_SIN2_003, 6723, 6278, 6771, 6511, 6290, 6323, 6630, 6290, 6719 },
	{ TX_SIN2_004, 5869, 5559, 5854, 5645, 5571, 5591, 5847, 5571, 5865 },
	{ TX_SIN2_005, 5052, 4850, 4924, 4801, 4863, 4879, 5037, 4863, 5048 },
	{ TX_SIN2_006, 8116, 7516, 8325, 7951, 7550, 7630, 8043, 7550, 8109 },
	{ TX_SIN2_007, 3070, 3097, 3037, 2960, 3096, 3020, 3131, 3096, 3066 },
	{ TX_SIN2_008, 5400, 5152, 5303, 5156, 5164, 5177, 5371, 5164, 5396 },
	{ TX_SIN2_009, 3770, 3705, 3560, 3688, 3706, 3657, 3799, 3706, 3766 },
	{ TX_SIN2_010, 4811, 4624, 4676, 9370, 4625, 4637, 4792, 4625, 4807 },
	{ TX_SIN2_012, 3514, 3479, 3291, 3438, 3478, 3437, 3560, 3478, 3510 },
	{ TX_SIN2_013, 8717, 8024, 8990, 8564, 8060, 8132, 8617, 8060, 8710 },
	{ TX_SIN2_014, 8458, 7795, 8716, 8312, 7831, 7914, 8373, 7831, 8451 },
	{ TX_SIN2_015, 1353, 1228, 1372, 1345, 1228, 1222, 1248, 1228, 1349 },
	{ TX_SIN2_016, 6980, 6500, 7052, 6779, 6517, 6561, 6881, 6517, 6976 },
	{ TX_SIN2_017, 8296, 7663, 8536, 8146, 7698, 7778, 8223, 7698, 8289 },
	{ TX_SIN2_019, 5806, 5511, 5777, 5586, 5523, 5547, 5785, 5523, 5802 },
	{ TX_SIN2_020, 2835, 2893, 2774, 2709, 2892, 2791, 2899, 2892, 2831 },
	{ TX_SIN2_021, 2303, 2421, 2406, 2348, 2421, 2476, 2576, 2421, 2299 },
	{ TX_SIN2_022, 3268, 3264, 3242, 3180, 3263, 3210, 3306, 3263, 3264 },
	{ TX_SIN2_023, 7064, 6569, 7144, 6875, 6586, 6642, 6958, 6586, 7060 },
	{ TX_SIN2_024, 4683, 4497, 4523, 4643, 4498, 4508, 4640, 4498, 4679 },
	{ TX_SIN2_025, 670, 644, 670, 670, 644, 646, 646, 644, 666 },
	{ TX_SIN2_026, 789, 748, 793, 783, 748, 744, 754, 748, 785 },
	{ TX_SIN2_027, 4101, 4006, 3919, 4047, 4007, 3986, 4128, 4007, 4097 },
	{ TX_SIN2_028, 3933, 3853, 3740, 3866, 3854, 3820, 3964, 3854, 3929 },
	{ TX_SIN2_029, 3302, 3283, 9879, 3214, 3282, 3229, 3325, 3282, 3298 },
	{ TX_SIN2_030, 7914, 7329, 8109, 7743, 7351, 7423, 7830, 7351, 7907 },
	{ TX_SIN2_031, 2586, 2674, 2497, 2431, 2674, 2544, 2652, 2674, 2582 },
	{ TX_SIN2_032, 1436, 1296, 1463, 1425, 1295, 1286, 1323, 1295, 1432 },
	{ TX_SIN2_033, 2936, 2979, 2886, 2815, 2978, 2890, 2995, 2978, 2932 },
	{ TX_SIN2_034, 2372, 2475, 9616, 9133, 2475, 8324, 8828, 2475, 2368 },
	{ TX_SIN2_035, 8952, 1591, 9235, 8790, 1591, 1605, 1662, 1591, 8945 },
	{ TX_SIN2_037, 3219, 3230, 3192, 3122, 3229, 3167, 3271, 3229, 3215 },
	{ TX_SIN2_038, 2687, 2759, 2608, 2542, 2759, 2640, 2748, 2759, 2683 },
	{ TX_SIN2_039, 6813, 6353, 6870, 6602, 6365, 6399, 6714, 6365, 6809 },
	{ TX_SIN2_040, 2170, 2303, 2260, 2186, 2303, 2324, 2445, 2303, 2166 },
	{ TX_SIN2_041, 1974, 2122, 2037, 1983, 2122, 2137, 2238, 2122, 1970 },
	{ TX_SIN2_042, 1064, 969, 1080, 1045, 969, 952, 987, 969, 1060 },
	{ TX_SIN2_043, 1770, 1933, 1842, 1785, 1933, 1911, 2058, 1933, 1766 },
	{ TX_SIN2_044, 1127, 1017, 1145, 1106, 1017, 998, 1036, 1017, 1123 },
	{ TX_SIN2_045, 3615, 3565, 3402, 3543, 3566, 3527, 3656, 3566, 3611 },
	{ TX_SIN2_046, 8820, 8112, 9083, 8650, 8148, 8203, 8695, 8148, 8813 },
	{ TX_SIN2_047, 7328, 6803, 7449, 7136, 6825, 6871, 7233, 6825, 7324 },
	{ TX_SIN2_048, 7795, 7225, 7971, 7605, 7247, 7298, 7707, 7247, 7788 },
	{ TX_SIN2_049, 7155, 6645, 7241, 6954, 6662, 6706, 7040, 6662, 7151 },
	{ TX_SIN2_050, 1639, 1484, 1691, 1640, 1484, 1485, 1535, 1484, 1635 },
	{ TX_SIN2_F12, 7608, 7053, 7772, 7415, 7075, 7124, 7526, 7075, 7604 },
	{ TX_SIN2_F26, 5591, 5312, 5512, 5354, 5324, 5345, 5550, 5324, 5587 },
	{ TX_SIN2_S05, 5116, 4899, 5005, 4869, 4912, 4933, 5103, 4912, 5112 },
	{ TX_SIN2_S16, 8179, 7564, 8393, 8015, 7598, 7679, 8095, 7598, 8172 },
	{ TX_SIN3_008, 8591, 7913, 8858, 8449, 7949, 8036, 8500, 7949, 8584 },
	{ TX_SIN1U070, 8370, 7722, 8620, 8219, 7757, 7836, 8292, 7757, 8363 },
	{ TX_SIN2N000, 6119, 5764, 6127, 5903, 5776, 5804, 6075, 5776, 6115 },
	{ TX_SIN2N001, 6301, 5931, 6305, 6077, 5943, 5963, 6238, 5943, 6297 },
	{ TX_SIN2N002, 6620, 6190, 6647, 6409, 6202, 6236, 6521, 6202, 6616 },
	{ TX_SIN2N003, 6359, 5974, 6365, 6139, 5986, 6010, 6283, 5986, 6355 },
	{ TX_SIN2N004, 6536, 6121, 6549, 6323, 6133, 6164, 6437, 6133, 6532 },
	{ TX_SIN2N005, 6451, 6051, 6465, 6240, 6063, 6096, 6368, 6063, 6447 },
	{ TX_SIN2N006, 5948, 5623, 5944, 5732, 5635, 5663, 5922, 5635, 5944 },
	{ TX_SIN2N007, 6060, 5720, 6063, 5838, 5732, 5754, 6026, 5732, 6056 },
	{ TX_SIN2N008, 7524, 6984, 7681, 7335, 7006, 7059, 7450, 7006, 7520 },
	{ TX_SIN2U085, 5252, 5009, 5117, 4986, 5022, 5022, 5200, 5022, 5248 },
	{ TX_SIN2U096, 5671, 5377, 5603, 5448, 5389, 5424, 5626, 5389, 5667 },
	{ TX_SIN2U097, 5445, 5181, 5351, 5200, 5193, 5206, 5404, 5193, 5441 },
	{          -1, 0,    0, 0, 0, 0, 0, 0, 0, 0 }
};

extern const RoomText sins2Texts[] = {
	  { -1, Common::UNK_LANG, "" }
};

void Room::sins2Tick1() {
	playVoc("SIN2LOOP");
	playMidiMusicTracks(MIDITRACK_27, kLoopTypeRepeat);

	if (!_awayMission->sins.enteredRoom2FirstTime)
		_awayMission->disableInput = 2;

	if (_awayMission->sins.openedInnerDoor) {
		// NOTE: callback is never called, since animation never finishes?
		loadActorAnimC(OBJECT_DOOR, "s2dro", 0, 0, &Room::sins2DoorFinishedOpening);
		loadMapFile("sins22");
	} else {
		loadMapFile("sins2");
	}
}

void Room::sins2UsePhaserOnDoor() {
	showText(TX_SPEAKER_SPOCK, TX_SIN2_025);
}

void Room::sins2UseSTricorderAnywhere() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_SIN2_026);
}

void Room::sins2UseKirkOnTerminal() {
	showText(TX_SPEAKER_KIRK, TX_SIN2_002);
	sins2UseSpockOnTerminal();
}

void Room::sins2UseSpockOnTerminal() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_W;
	walkCrewmanC(OBJECT_SPOCK, 0x5f, 0xa7, &Room::sins2SpockReachedTerminal);
}

void Room::sins2SpockReachedTerminal() {
	playVoc("EFX14S");
	loadActorAnimC(OBJECT_SPOCK, "susehw", -1, -1, &Room::sins2SpockUsedTerminal);
}

void Room::sins2SpockUsedTerminal() {
	showText(TX_SPEAKER_SPOCK, TX_SIN2_042);
	showText(TX_SPEAKER_SPOCK, TX_SIN2_044);

	if (!_awayMission->sins.gotPointsForAccessingTerminal) {
		_awayMission->sins.missionScore += 2;
		_awayMission->sins.gotPointsForAccessingTerminal = true;
	}

	showText(TX_SPEAKER_MCCOY,  TX_SIN2_015);
	showText(TX_SPEAKER_SPOCK,  TX_SIN2_032);
	showText(TX_SPEAKER_MOSHER, TX_SIN2_050);
	showText(TX_SPEAKER_SPOCK,  TX_SIN2_035);
	showText(TX_SPEAKER_SPOCK,  TX_SIN2_043);
	showText(TX_SPEAKER_SPOCK,  TX_SIN2_041);
	showText(TX_SPEAKER_SPOCK,  TX_SIN2_040);
	showText(TX_SPEAKER_MCCOY,  TX_SIN2_021);
	showText(TX_SPEAKER_SPOCK,  TX_SIN2_034);
	showText(TX_SPEAKER_SPOCK,  TX_SIN2_031);
	showText(TX_SPEAKER_SPOCK,  TX_SIN2_038);
	showText(TX_SPEAKER_MCCOY,  TX_SIN2_020);
	showText(TX_SPEAKER_SPOCK,  TX_SIN2_033);
	showText(TX_SPEAKER_KIRK,   TX_SIN2_007);
	showText(TX_SPEAKER_SPOCK,  TX_SIN2_037);
	showText(TX_SPEAKER_MCCOY,  TX_SIN2_022);
	showText(TX_SPEAKER_SPOCK,  TX_SIN2_029);
}

void Room::sins2UseMccoyOnTerminal() {
	showText(TX_SPEAKER_MCCOY, TX_SIN2_012);
}

void Room::sins2UseRedshirtOnTerminal() {
	showText(TX_SPEAKER_MOSHER, TX_SIN2_045);
}

void Room::sins2UseSTricorderOnTerminal() {
	spockScan(DIR_W, TX_SPEAKER_SPOCK, TX_SIN2_009);
}

void Room::sins2UseSTricorderOnKeypad() {
	spockScan(DIR_N, TX_SPEAKER_SPOCK, TX_SIN2_028);
}

void Room::sins2UseSTricorderOnDoor() {
	spockScan(DIR_N, TX_SPEAKER_SPOCK, TX_SIN2_027);
}

void Room::sins2UseKirkOnKeypad() {
	if (!_awayMission->sins.openedInnerDoor) {
		showText(TX_SPEAKER_KIRK, TX_SIN2_001);
		sins2UseSpockOnKeypad();
	}
}

void Room::sins2UseSpockOnKeypad() {
	if (!_awayMission->sins.openedInnerDoor) {
		walkCrewmanC(OBJECT_SPOCK, 0xc3, 0x9b, &Room::sins2SpockReachedKeypad);
	}
}

void Room::sins2SpockReachedKeypad() {
	const char *codes[] = {
		"122", nullptr
	};

	int ans = showCodeInputBox(codes);

	if (ans == -1) { // ENHANCEMENT: if nothing was entered, do nothing.
	} else if (ans == 1) {
		playVoc("EFX28S");
		loadActorAnimC(OBJECT_SPOCK, "susehe", -1, -1, &Room::sins2EnteredCorrectCode);
	} else { // Incorrect answer
		playVoc("EFX28S");
		loadActorAnimC(OBJECT_SPOCK, "susehe", -1, -1, &Room::sins2EnteredIncorrectCode);
	}
}

void Room::sins2EnteredCorrectCode() {
	_awayMission->sins.openedInnerDoor = true;
	loadMapFile("sins22");
	playVoc("BIGDOOR3");
	loadActorAnimC(OBJECT_DOOR, "s2door", 0, 0, &Room::sins2DoorFinishedOpening);
}

void Room::sins2EnteredIncorrectCode() {
	showText(TX_SPEAKER_SPOCK, TX_SIN2_024); // BUGFIX: Speaker is spock, not "none"
}

void Room::sins2DoorFinishedOpening() {
	showText(TX_SPEAKER_SPOCK, TX_SIN2_010);
}

void Room::sins2Tick40() {
	if (!_awayMission->sins.enteredRoom2FirstTime) {
		_awayMission->disableInput = false;

		showText(TX_SPEAKER_KIRK,  TX_SIN2_005);
		showText(TX_SPEAKER_SCOTT, TX_SIN2_S05);
		showText(TX_SPEAKER_UHURA, TX_SIN2U085);
		showText(TX_SPEAKER_KIRK,  TX_SIN2_008);
		showText(TX_SPEAKER_UHURA, TX_SIN2U097);
		showText(TX_SPEAKER_MCCOY, TX_SIN2_F26);
		showText(TX_SPEAKER_UHURA, TX_SIN2U096);
		showText(TX_SPEAKER_MCCOY, TX_SIN2_019);
		showText(TX_SPEAKER_KIRK,  TX_SIN2_004);

		_awayMission->sins.enteredRoom2FirstTime = true;
	}
}

void Room::sins2LookAnywhere() {
	showDescription(TX_SIN2N006);
}

void Room::sins2LookAtOpenDoor() {
	showDescription(TX_SIN2N007);
}

void Room::sins2LookAtTerminal() {
	showDescription(TX_SIN2N000);
}

void Room::sins2LookAtKeypad() {
	showDescription(TX_SIN2N001);
}

void Room::sins2LookAtKirk() {
	showDescription(TX_SIN2N003);
}

void Room::sins2LookAtSpock() {
	showDescription(TX_SIN2N005);
}

void Room::sins2LookAtMccoy() {
	showDescription(TX_SIN2N004);
}

void Room::sins2LookAtRedshirt() {
	showDescription(TX_SIN2N002);
}

void Room::sins2TalkToKirk() {
	showText(TX_SPEAKER_KIRK, TX_SIN2_003);
}

void Room::sins2TalkToSpock() {
	showText(TX_SPEAKER_SPOCK, TX_SIN2_039);
	showText(TX_SPEAKER_MCCOY, TX_SIN2_016);
}

void Room::sins2TalkToMccoy() {
	showText(TX_SPEAKER_MCCOY,  TX_SIN2_023);
	showText(TX_SPEAKER_MOSHER, TX_SIN2_049);
}

void Room::sins2TalkToRedshirt() {
	showText(TX_SPEAKER_MOSHER, TX_SIN2_047);
}

void Room::sins2LookAtDoor() {
	if (!_awayMission->sins.openedInnerDoor)
		showDescription(TX_SIN2N008);
}

void Room::sins2UseSpockOnDoor() {
	if (!_awayMission->sins.openedInnerDoor) {
		showText(TX_SPEAKER_SPOCK,  TX_SIN2_F12);
		showText(TX_SPEAKER_MOSHER, TX_SIN2_048);
		showText(TX_SPEAKER_SPOCK,  TX_SIN2_030);
	}
}

void Room::sins2UseCommunicator() {
	if (!_awayMission->sins.enteredRoom4FirstTime) {
		showText(TX_SPEAKER_KIRK,  TX_SIN2_006);
		showText(TX_SPEAKER_SCOTT, TX_SIN2_S16);
		showText(TX_SPEAKER_MCCOY, TX_SIN2_017);
	} else
		showText(TX_SPEAKER_UHURA, TX_SIN1U070);
}

void Room::sins2UseMedkitOnCrewman() {
	showText(TX_SPEAKER_MCCOY, TX_SIN2_014);
}

void Room::sins2UseMTricorderOnCrewman() {
	mccoyScan(DIR_S, TX_SPEAKER_MCCOY, TX_SIN3_008); // BUGFIX: original game had wrong audio file path
}

void Room::sins2UseMccoyOnKeypad() {
	if (!_awayMission->sins.openedInnerDoor)
		showText(TX_SPEAKER_MCCOY, TX_SIN2_013);
}

void Room::sins2UseRedshirtOnKeypad() {
	if (!_awayMission->sins.openedInnerDoor)
		showText(TX_SPEAKER_MOSHER, TX_SIN2_046);
}

void Room::sins2WalkToDoor() {
	if (_awayMission->sins.openedInnerDoor)
		walkCrewman(OBJECT_KIRK, 0x9c, 0x8b);
}
}
