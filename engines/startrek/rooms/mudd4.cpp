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

#define OBJECT_VIEWSCREEN 8
#define OBJECT_ALIENDV 9
#define OBJECT_REPAIR_TOOL 10
#define OBJECT_11 11

#define HOTSPOT_SCREEN 0x20
#define HOTSPOT_CONSOLE 0x21
#define HOTSPOT_LEFT_CONSOLE 0x22
#define HOTSPOT_RIGHT_CONSOLE 0x23
#define HOTSPOT_EAST_DOOR 0x24
#define HOTSPOT_WEST_DOOR 0x25

namespace StarTrek {

extern const RoomAction mudd4ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::mudd4Tick1 },
	{ {ACTION_USE, OBJECT_ICOMM, 0xff, 0, 0},    &Room::mudd4UseCommunicator },
	{ {ACTION_TIMER_EXPIRED, 2, 0, 0, 0},        &Room::mudd4Timer2Expired },
	{ {ACTION_TIMER_EXPIRED, 3, 0, 0, 0},        &Room::mudd4Timer3Expired },
	{ {ACTION_USE, OBJECT_SPOCK, 0x21, 0, 0},    &Room::mudd4UseSpockOnLeftConsole },
	{ {ACTION_USE, OBJECT_SPOCK, 0x22, 0, 0},    &Room::mudd4UseSpockOnLeftConsole },
	{ {ACTION_USE, OBJECT_SPOCK, 0x23, 0, 0},    &Room::mudd4UseSpockOnRightConsole },
	{ {ACTION_FINISHED_WALKING, 1, 0, 0, 0},     &Room::mudd4SpockReachedChair },
	{ {ACTION_FINISHED_ANIMATION, 1, 0, 0, 0},   &Room::mudd4SpockSatInChair },
	{ {ACTION_FINISHED_ANIMATION, 2, 0, 0, 0},   &Room::mudd4SpockUsedSensors },
	{ {ACTION_FINISHED_ANIMATION, 3, 0, 0, 0},   &Room::mudd4SpockUsedEngineering },
	{ {ACTION_FINISHED_ANIMATION, 4, 0, 0, 0},   &Room::mudd4SpockUsedNavigation },
	{ {ACTION_FINISHED_WALKING, 3, 0, 0, 0},     &Room::mudd4FinishedWalking3 },
	{ {ACTION_USE, OBJECT_KIRK, 0x21, 0, 0},     &Room::mudd4UseKirkOnRightConsole },
	{ {ACTION_USE, OBJECT_KIRK, 0x23, 0, 0},     &Room::mudd4UseKirkOnRightConsole },
	{ {ACTION_USE, OBJECT_KIRK, 0x22, 0, 0},     &Room::mudd4UseKirkOnLeftConsole },
	{ {ACTION_FINISHED_WALKING, 2, 0, 0, 0},     &Room::mudd4KirkReachedRightConsole },
	{ {ACTION_FINISHED_ANIMATION, 5, 0, 0, 0},   &Room::mudd4KirkSatInChair },
	{ {ACTION_FINISHED_ANIMATION, 6, 0, 0, 0},   &Room::mudd4KirkUsedViewScreen },
	{ {ACTION_TIMER_EXPIRED, 1, 0, 0, 0},        &Room::mudd4Timer1Expired },
	{ {ACTION_FINISHED_ANIMATION, 7, 0, 0, 0},   &Room::mudd4KirkUsedCommunications },
	{ {ACTION_FINISHED_WALKING, 4, 0, 0, 0},     &Room::mudd4KirkReachedPositionToTalkToMudd },
	{ {ACTION_USE, OBJECT_MCCOY, 0x21, 0, 0},    &Room::mudd4UseMccoyOnConsole },
	{ {ACTION_USE, OBJECT_MCCOY, 0x22, 0, 0},    &Room::mudd4UseMccoyOnConsole },
	{ {ACTION_USE, OBJECT_MCCOY, 0x23, 0, 0},    &Room::mudd4UseMccoyOnConsole },
	{ {ACTION_USE, OBJECT_REDSHIRT, 0x21, 0, 0}, &Room::mudd4UseRedshirtOnConsole },
	{ {ACTION_USE, OBJECT_REDSHIRT, 0x22, 0, 0}, &Room::mudd4UseRedshirtOnConsole },
	{ {ACTION_USE, OBJECT_REDSHIRT, 0x23, 0, 0}, &Room::mudd4UseRedshirtOnConsole },
	{ {ACTION_USE, OBJECT_ISTRICOR, 10, 0, 0},   &Room::mudd4UseSTricorderOnRepairTool },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x21, 0, 0}, &Room::mudd4UseSTricorderOnConsole },

	// ENHANCEMENT: Allow scanning the console to work when scanning the specific stations
	// as well
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x22, 0, 0}, &Room::mudd4UseSTricorderOnConsole },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x23, 0, 0}, &Room::mudd4UseSTricorderOnConsole },

	{ {ACTION_USE, OBJECT_ISTRICOR, 0x20, 0, 0}, &Room::mudd4UseSTricorderOnViewscreen },

	// ENHANCEMENT: Allow scanning the viewscreen when it's on, not just when off
	{ {ACTION_USE, OBJECT_ISTRICOR, 8, 0, 0},    &Room::mudd4UseSTricorderOnViewscreen },

	// Common code (next 4 lines)
	{ {ACTION_USE, OBJECT_ILENSES, OBJECT_IDEGRIME, 0, 0}, &Room::muddaUseLenseOnDegrimer },
	{ {ACTION_USE, OBJECT_IALIENDV, 0xff, 0, 0}, &Room::muddaUseAlienDevice },
	{ {ACTION_FINISHED_ANIMATION, 9, 0, 0, 0}, &Room::muddaFiredAlienDevice },
	{ {ACTION_USE, OBJECT_IDEGRIME, 0xff, 0, 0}, &Room::muddaUseDegrimer },

	{ {ACTION_USE, OBJECT_IMEDKIT, 0xff, 0, 0},   &Room::mudd4UseMedkit },
	{ {ACTION_GET, 10, 0, 0, 0},                &Room::mudd4GetRepairTool },
	{ {ACTION_FINISHED_WALKING, 5, 0, 0, 0},    &Room::mudd4ReachedRepairTool },
	{ {ACTION_FINISHED_ANIMATION, 10, 0, 0, 0}, &Room::mudd4PickedUpRepairTool },

	{ {ACTION_LOOK, 0x21, 0, 0, 0}, &Room::mudd4LookAtConsole },
	// ENHANCEMENT: Allow look action work with the specific stations as well
	{ {ACTION_LOOK, 0x22, 0, 0, 0}, &Room::mudd4LookAtConsole },
	{ {ACTION_LOOK, 0x23, 0, 0, 0}, &Room::mudd4LookAtConsole },

	{ {ACTION_LOOK, 0x20, 0, 0, 0}, &Room::mudd4LookAtViewscreen },
	// ENHANCEMENT: Allow look action work when viewscreen is on, not just when off
	{ {ACTION_LOOK, 8, 0, 0, 0},    &Room::mudd4LookAtViewscreen },

	{ {ACTION_LOOK, OBJECT_KIRK, 0, 0, 0},     &Room::mudd4LookAtKirk },
	{ {ACTION_LOOK, OBJECT_SPOCK, 0, 0, 0},    &Room::mudd4LookAtSpock },
	{ {ACTION_LOOK, OBJECT_MCCOY, 0, 0, 0},    &Room::mudd4LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::mudd4LookAtRedshirt },
	{ {ACTION_LOOK, 10, 0, 0, 0},              &Room::mudd4LookAtRepairTool },
	{ {ACTION_TALK, OBJECT_KIRK, 0, 0, 0},     &Room::mudd4TalkToKirk },
	{ {ACTION_TALK, OBJECT_SPOCK, 0, 0, 0},    &Room::mudd4TalkToSpock },
	{ {ACTION_TALK, OBJECT_MCCOY, 0, 0, 0},    &Room::mudd4TalkToMccoy },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::mudd4TalkToRedshirt },
	{ {ACTION_WALK, 0x24, 0, 0, 0},            &Room::mudd4WalkToEastDoor },
	{ {ACTION_WALK, 0x25, 0, 0, 0},            &Room::mudd4WalkToWestDoor },
	{ {ACTION_TOUCHED_HOTSPOT, 0, 0, 0, 0},    &Room::mudd4TouchedHotspot0 },

	// Common code (countdown for losing atmosphere when life support malfunctioning)
	{ {ACTION_TICK, 0xff, 0xff, 0xff, 0},           &Room::muddaTick },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum mudd4TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_UHURA, TX_SPEAKER_BUCHERT,
	TX_SPEAKER_SCOTT, TX_SPEAKER_MUDD,
	TX_MUD4_001, TX_MUD4_002, TX_MUD4_004, TX_MUD4_005, TX_MUD4_006,
	TX_MUD4_007, TX_MUD4_008, TX_MUD4_009, TX_MUD4_010, TX_MUD4_011,
	TX_MUD4_012, TX_MUD4_014, TX_MUD4_015, TX_MUD4_021, TX_MUD4_026,
	TX_MUD4_029, TX_MUD4_030, TX_MUD4_034, TX_MUD4_035, TX_MUD4_036,
	TX_MUD4_037, TX_MUD4_046, TX_MUD4_047, TX_MUD4_050, TX_MUD4_051,
	TX_MUD4_063, TX_MUD4_064, TX_MUD4_065, TX_MUD4_067, TX_MUD4_069,
	TX_MUD4_070, TX_MUD4_071, TX_MUD4_073, TX_MUD4_076, TX_MUD4_013,
	TX_MUD4_016, TX_MUD4_017, TX_MUD4_018, TX_MUD4_019, TX_MUD4_020,
	TX_MUD4_023, TX_MUD4_024, TX_MUD4_025, TX_MUD4_027, TX_MUD4_028,
	TX_MUD4_031, TX_MUD4_032, TX_MUD4_033, TX_MUD4_038, TX_MUD4_039,
	TX_MUD4_040, TX_MUD4_041, TX_MUD4_042, TX_MUD4_043, TX_MUD4_045,
	TX_MUD4_048, TX_MUD4_049, TX_MUD4_052, TX_MUD4_053, TX_MUD4_054,
	TX_MUD4_055, TX_MUD4_056, TX_MUD4_057, TX_MUD4_058, TX_MUD4_059,
	TX_MUD4_060, TX_MUD4_061, TX_MUD4_062, TX_MUD4_066, TX_MUD4_068,
	TX_MUD4_072, TX_MUD4_074, TX_MUD4_075, TX_MUD4_A29, TX_MUD4_S02,
	TX_MUD4_S03, TX_MUD4_S04, TX_MUD4N000, TX_MUD4N001, TX_MUD4N003,
	TX_MUD4N004, TX_MUD4N005, TX_MUD4N006, TX_MUD4N007, TX_MUD4N008,
	TX_MUD4N011, TX_MUD4N012, TX_MUD4N013, TX_MUD4_003, TX_STATICU1
};

extern const RoomTextOffsets mudd4TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 4048, 3916, 4109, 4066, 3932, 3935, 4024 },
	{ TX_SPEAKER_MCCOY, 4059, 3927, 4120, 4077, 3943, 3949, 4036 },
	{ TX_SPEAKER_SPOCK, 4069, 3937, 4130, 4087, 3953, 3959, 4046 },
	{ TX_SPEAKER_UHURA, 491, 470, 490, 511, 482, 482, 480 },
	{ TX_SPEAKER_BUCHERT, 4079, 3947, 4140, 4097, 3963, 3969, 4056 },
	{ TX_SPEAKER_SCOTT, 4091, 3959, 4152, 4109, 3975, 3981, 4068 },
	{ TX_SPEAKER_MUDD, 4101, 3969, 4162, 4119, 3985, 3991, 4078 },
	{ TX_MUD4_001, 12511, 11096, 13694, 12551, 11107, 11307, 12332 },
	{ TX_MUD4_002, 6069, 5652, 6301, 5318, 5667, 5678, 5947 },
	{ TX_MUD4_004, 6134, 5687, 6369, 6098, 5702, 5711, 5993 },
	{ TX_MUD4_005, 11984, 10674, 13100, 11987, 10685, 10856, 11842 },
	{ TX_MUD4_006, 5958, 5556, 6187, 5979, 5571, 5574, 5848 },
	{ TX_MUD4_007, 6103, 5671, 6331, 5347, 5686, 5696, 5966 },
	{ TX_MUD4_008, 9263, 8405, 10020, 9192, 8415, 8470, 9211 },
	{ TX_MUD4_009, 9687, 8784, 10525, 9640, 8794, 8890, 9671 },
	{ TX_MUD4_010, 10886, 9802, 11897, 10857, 9812, 9923, 10864 },
	{ TX_MUD4_011, 10945, 9831, 11961, 10919, 9840, 9955, 10898 },
	{ TX_MUD4_012, 10793, 9739, 11783, 10765, 9750, 9861, 10780 },
	{ TX_MUD4_013, 4255, 4108, 4342, 4278, 4124, 4135, 4243 },
	{ TX_MUD4_014, 8571, 7783, 9198, 8525, 7793, 7831, 8477 },
	{ TX_MUD4_015, 12129, 10789, 13284, 12146, 10800, 10983, 11996 },
	{ TX_MUD4_016, 10051, 9088, 10961, 10029, 9098, 9220, 10047 },
	{ TX_MUD4_017, 8939, 8106, 9642, 8919, 8116, 8181, 8876 },
	{ TX_MUD4_018, 454, 428, 454, 454, 440, 440, 440 },
	{ TX_MUD4_019, 7470, 6847, 7912, 7419, 6857, 6869, 7356 },
	{ TX_MUD4_020, 7279, 6686, 7724, 7224, 6696, 6704, 7198 },
	{ TX_MUD4_021, 8726, 7923, 9387, 8707, 7933, 7998, 8651 },
	{ TX_MUD4_026, 9960, 9012, 10861, 9942, 9022, 9153, 9962 },
	{ TX_MUD4_029, 10391, 9383, 11351, 10359, 9393, 9500, 10392 },
	{ TX_MUD4_030, 10461, 9438, 11415, 10427, 9448, 9553, 10441 },
	{ TX_MUD4_034, 9772, 8854, 10624, 9740, 8864, 8975, 9755 },
	{ TX_MUD4_035, 11155, 10011, 12227, 11145, 10019, 10151, 11134 },
	{ TX_MUD4_036, 10357, 9364, 11305, 10310, 9374, 9466, 10361 },
	{ TX_MUD4_037, 9869, 8936, 10757, 9851, 8946, 9071, 9873 },
	{ TX_MUD4_046, 12235, 10865, 13388, 12254, 10876, 11058, 12070 },
	{ TX_MUD4_047, 12171, 10816, 13320, 12182, 10827, 11003, 12017 },
	{ TX_MUD4_050, 8178, 7435, 8751, 8128, 7445, 7466, 8075 },
	{ TX_MUD4_051, 12411, 11011, 13570, 12434, 11022, 11206, 12223 },
	{ TX_MUD4_063, 12072, 10747, 13207, 12084, 10758, 10937, 11934 },
	{ TX_MUD4_064, 10677, 9638, 11662, 10648, 9649, 9759, 10674 },
	{ TX_MUD4_065, 8267, 7509, 8849, 8215, 7519, 7538, 8158 },
	{ TX_MUD4_067, 10915, 9816, 11928, 10888, 9826, 9939, 10880 },
	{ TX_MUD4_069, 11221, 10062, 12301, 11210, 10070, 10201, 11193 },
	{ TX_MUD4_070, 10818, 9749, 11812, 10789, 9760, 9870, 10794 },
	{ TX_MUD4_071, 10145, 9167, 11061, 10111, 9177, 9282, 10132 },
	{ TX_MUD4_073, 11092, 9963, 12159, 11079, 9971, 10100, 11081 },
	{ TX_MUD4_076, 12314, 10929, 13471, 12328, 10940, 11116, 12139 },
	{ TX_MUD4_023, 7208, 6630, 7652, 7156, 6640, 6647, 7141 },
	{ TX_MUD4_024, 7106, 6543, 7529, 7050, 6553, 6555, 7033 },
	{ TX_MUD4_025, 6345, 5868, 6601, 6298, 5883, 5872, 6195 },
	{ TX_MUD4_027, 6588, 6081, 6905, 6546, 6095, 6104, 6469 },
	{ TX_MUD4_028, 6812, 6290, 7187, 6753, 6304, 6304, 6736 },
	{ TX_MUD4_031, 4607, 4400, 4745, 4634, 4415, 4430, 4585 },
	{ TX_MUD4_032, 9626, 8738, 10442, 9559, 8748, 8824, 9603 },
	{ TX_MUD4_033, 7060, 6512, 7473, 6998, 6522, 6518, 6992 },
	{ TX_MUD4_038, 5197, 4930, 5347, 5219, 4945, 4982, 5127 },
	{ TX_MUD4_039, 5224, 4942, 5371, 5243, 4957, 4995, 5140 },
	{ TX_MUD4_040, 5254, 4957, 5397, 5269, 4972, 5010, 5155 },
	{ TX_MUD4_041, 5285, 4973, 5423, 5298, 4988, 5028, 5170 },
	{ TX_MUD4_042, 7869, 7171, 8357, 7819, 7181, 7194, 7726 },
	{ TX_MUD4_043, 2510, 2449, 2509, 2530, 2465, 2465, 2463 },
	{ TX_MUD4_045, 7555, 6902, 8005, 7514, 6912, 6934, 7419 },
	{ TX_MUD4_048, 4377, 4200, 4490, 4400, 4216, 4227, 4361 },
	{ TX_MUD4_049, 5786, 5414, 5988, 5834, 5429, 5451, 5679 },
	{ TX_MUD4_052, 5840, 5453, 6045, 5882, 5468, 5484, 5721 },
	{ TX_MUD4_053, 5309, 4982, 5443, 5373, 4997, 5037, 5179 },
	{ TX_MUD4_054, 6219, 5757, 6462, 6179, 5772, 5777, 6071 },
	{ TX_MUD4_055, 5603, 5246, 5774, 5671, 5261, 5303, 5480 },
	{ TX_MUD4_056, 5523, 5181, 5664, 5610, 5196, 5257, 5385 },
	{ TX_MUD4_057, 4895, 4658, 5039, 4929, 4673, 4709, 4849 },
	{ TX_MUD4_058, 5023, 4771, 5170, 5055, 4786, 4820, 4965 },
	{ TX_MUD4_059, 4459, 4267, 4566, 4483, 4282, 4294, 4421 },
	{ TX_MUD4_060, 4676, 4454, 4806, 4713, 4469, 4494, 4631 },
	{ TX_MUD4_061, 4119, 3987, 4180, 4137, 4003, 4009, 4096 },
	{ TX_MUD4_062, 4288, 4126, 4378, 4323, 4142, 4165, 4264 },
	{ TX_MUD4_066, 8396, 7623, 8996, 8344, 7633, 7654, 8290 },
	{ TX_MUD4_068, 9395, 8522, 10164, 9322, 8532, 8585, 9340 },
	{ TX_MUD4_072, 9205, 8362, 9951, 9140, 8372, 8432, 9157 },
	{ TX_MUD4_074, 8881, 8063, 9566, 8866, 8074, 8142, 8815 },
	{ TX_MUD4_075, 7611, 6943, 8062, 7570, 6953, 6975, 7461 },
	{ TX_MUD4_A29, 7503, 6865, 7948, 7453, 6875, 6888, 7377 },
	{ TX_MUD4_S02, 6377, 5885, 6649, 6331, 5899, 5892, 6228 },
	{ TX_MUD4_S03, 7323, 6715, 7766, 7270, 6725, 6735, 7225 },
	{ TX_MUD4_S04, 6912, 6375, 7288, 6840, 6389, 6375, 6822 },
	{ TX_MUD4N000, 7801, 7118, 8265, 7751, 7128, 7141, 7649 },
	{ TX_MUD4N001, 11747, 10483, 12874, 11747, 10494, 10660, 11661 },
	{ TX_MUD4N003, 11615, 10381, 12731, 11623, 10389, 10551, 11548 },
	{ TX_MUD4N004, 11792, 10513, 12923, 11806, 10524, 10703, 11695 },
	{ TX_MUD4N005, 3106, 3018, 3111, 3119, 3034, 3034, 3071 },
	{ TX_MUD4N006, 11677, 10428, 12795, 11682, 10439, 10594, 11597 },
	{ TX_MUD4N007, 3298, 3195, 3332, 3318, 3211, 3218, 3277 },
	{ TX_MUD4N008, 8029, 7301, 8564, 7985, 7311, 7338, 7903 },
	{ TX_MUD4N011, 11912, 10617, 13038, 11923, 10628, 10808, 11795 },
	{ TX_MUD4N012, 11278, 10104, 12367, 11270, 10112, 10256, 11244 },
	{ TX_MUD4N013, 7985, 7272, 8518, 7935, 7282, 7303, 7872 },
	{          -1, 0,    0, 0, 0, 0, 0, 0 }
};

extern const RoomText mudd4Texts[] = {
	{ TX_MUD4_003, Common::EN_ANY, "#MUD4\\MUD4_003#<<Done>>" },
	{ -1, Common::UNK_LANG, "" }
};

void Room::mudd4Tick1() {
	playVoc("MUD4LOOP");

	if (!_awayMission->mudd.tookRepairTool)
		loadActorAnim(OBJECT_REPAIR_TOOL, "s4crdv", 0xc9, 0x8e);

	if (_awayMission->mudd.viewScreenEnabled)
		loadActorAnim(OBJECT_VIEWSCREEN, "s4crvo", 0xa5, 0x76);
}

void Room::mudd4UseCommunicator() {
	showText(TX_SPEAKER_KIRK,  TX_MUD4_018);
	showText(TX_SPEAKER_UHURA, TX_STATICU1);
}

void Room::mudd4Timer2Expired() {
	playSoundEffectIndex(kSfxButton);
}

void Room::mudd4Timer3Expired() {
	playSoundEffectIndex(kSfxTransporterEnergize);
}

void Room::mudd4UseSpockOnLeftConsole() {
	_roomVar.mudd.usingLeftConsole = true;
	mudd4UseSpockOnConsole();
}

void Room::mudd4UseSpockOnRightConsole() {
	_roomVar.mudd.usingLeftConsole = false;
	mudd4UseSpockOnConsole();
}

void Room::mudd4UseSpockOnConsole() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_N;
	loadActorStandAnim(OBJECT_SPOCK);

	if (_awayMission->mudd.translatedAlienLanguage) {
		if (!_roomVar.mudd.usingLeftConsole)
			showText(TX_SPEAKER_SPOCK, TX_MUD4_057);
	} else if (_awayMission->mudd.discoveredBase3System) {
		showText(TX_SPEAKER_SPOCK, TX_MUD4_062);
	} else {
		showText(TX_SPEAKER_SPOCK, TX_MUD4_061);
		showText(TX_SPEAKER_KIRK,  TX_MUD4_013);
		showText(TX_SPEAKER_SPOCK, TX_MUD4_062);
		showText(TX_SPEAKER_MCCOY, TX_MUD4_048);
		showText(TX_SPEAKER_SPOCK, TX_MUD4_059);
		showText(TX_SPEAKER_KIRK,  TX_MUD4_031);
		showText(TX_SPEAKER_SPOCK, TX_MUD4_060);

		if (!_awayMission->mudd.discoveredBase3System) {
			_awayMission->mudd.discoveredBase3System = true;
			_awayMission->mudd.missionScore++;
		}
	}

	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_N;
	walkCrewmanC(OBJECT_SPOCK, 0x6d, 0x9a, &Room::mudd4SpockReachedChair);
	_awayMission->disableInput = true;
}

void Room::mudd4SpockReachedChair() {
	if (_roomVar.mudd.usingLeftConsole && _awayMission->mudd.translatedAlienLanguage)
		loadActorAnimC(OBJECT_SPOCK, "s4crss", -1, -1, &Room::mudd4SpockSatInChair);
	else
		_awayMission->disableInput = false;
}

void Room::mudd4SpockSatInChair() {
	loadActorAnim2(OBJECT_SPOCK, "s4crsr", 0x55, 0x98);

	showText(TX_SPEAKER_SPOCK, TX_MUD4_058);
	mudd4ShowLeftConsoleMenu();
}

void Room::mudd4ShowLeftConsoleMenu() {
	const TextRef choices[] = {
		TX_SPEAKER_SPOCK,
		TX_MUD4_038,
		TX_MUD4_039,
		TX_MUD4_040,
		TX_MUD4_041,
		TX_END
	};

	int choice = showMultipleTexts(choices);

	switch (choice) {
	case 0: // Sensors
		loadActorAnimC(OBJECT_SPOCK, "s4crsr", 0x55, 0x98, &Room::mudd4SpockUsedSensors);
		_awayMission->timers[2] = 5;
		break;

	case 1: // Navigation
		loadActorAnimC(OBJECT_SPOCK, "s4crsl", 0x55, 0x98, &Room::mudd4SpockUsedNavigation);
		_awayMission->timers[2] = 5;
		break;

	case 2: // Engineering
		loadActorAnimC(OBJECT_SPOCK, "s4crsr", 0x55, 0x98, &Room::mudd4SpockUsedEngineering);
		_awayMission->timers[2] = 5;
		break;

	case 3: // Done
		_awayMission->disableInput = false;
		_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_N;
		walkCrewman(OBJECT_SPOCK, 0x69, 0xb7);
		break;
	}
}

void Room::mudd4SpockUsedSensors() {
	_awayMission->disableInput = false;
	showText(TX_SPEAKER_SPOCK, TX_MUD4_053);
	mudd4ShowLeftConsoleMenu();
}

void Room::mudd4SpockUsedEngineering() {
	_awayMission->disableInput = false;
	showText(TX_SPEAKER_SPOCK, TX_MUD4_056);
	mudd4ShowLeftConsoleMenu();
}

void Room::mudd4SpockUsedNavigation() {
	_awayMission->disableInput = false;
	showText(TX_SPEAKER_SPOCK, TX_MUD4_055);
	mudd4ShowLeftConsoleMenu();
}

void Room::mudd4FinishedWalking3() { // Spock reaches console after attempting to use Kirk on one
	_awayMission->disableInput = false;
	mudd4UseSpockOnLeftConsole();
}

void Room::mudd4UseKirkOnRightConsole() {
	_roomVar.mudd.kirkUsingRightConsole = true;
	mudd4UseKirkOnConsole();
}

void Room::mudd4UseKirkOnLeftConsole() {
	_roomVar.mudd.kirkUsingRightConsole = false;
	mudd4UseKirkOnConsole();
}

void Room::mudd4UseKirkOnConsole() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_N;
	loadActorStandAnim(OBJECT_KIRK);

	if (!_awayMission->mudd.translatedAlienLanguage) {
		showText(TX_SPEAKER_KIRK, TX_MUD4_006);
		_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_N;
		walkCrewmanC(OBJECT_SPOCK, 0x6d, 0x9a, &Room::mudd4FinishedWalking3);
		_awayMission->disableInput = true;
	} else {
		if (!_roomVar.mudd.kirkUsingRightConsole)
			showText(TX_SPEAKER_KIRK, TX_MUD4_049);
		_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_N;
		walkCrewmanC(OBJECT_KIRK, 0x7e, 0x98, &Room::mudd4KirkReachedRightConsole);
		_awayMission->disableInput = true;
	}
}

void Room::mudd4KirkReachedRightConsole() {
	if (_roomVar.mudd.kirkUsingRightConsole && _awayMission->mudd.translatedAlienLanguage)
		loadActorAnimC(OBJECT_KIRK, "s4crks", -1, -1, &Room::mudd4KirkSatInChair);
	else
		_awayMission->disableInput = false;
}

void Room::mudd4KirkSatInChair() {
	loadActorAnim2(OBJECT_KIRK, "s4crkl", 0x94, 0x98);
	_awayMission->timers[2] = 5;
	_awayMission->disableInput = false;

	showText(TX_SPEAKER_SPOCK, TX_MUD4_052); // NOTE: why is Spock talking here?

	mudd4ShowRightConsoleMenu();
}

void Room::mudd4ShowRightConsoleMenu() {
	const TextRef choices[] = {
		TX_SPEAKER_KIRK,
		TX_MUD4_002,
		TX_MUD4_007,
		TX_MUD4_003, // BUGFIX: this used Spock's voice instead of Kirk's in original
		TX_END
	};

	int choice = showMultipleTexts(choices);

	switch (choice) {
	case 0: // Communications
		_awayMission->disableInput = true;
		loadActorAnimC(OBJECT_KIRK, "s4crkr", 0x94, 0x98, &Room::mudd4KirkUsedCommunications);
		break;

	case 1: // View Screen
		_awayMission->disableInput = true;
		loadActorAnimC(OBJECT_KIRK, "s4crkl", 0x94, 0x98, &Room::mudd4KirkUsedViewScreen);
		break;

	case 2: // Done
		_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_N;
		walkCrewman(OBJECT_KIRK, 0x83, 0xb6);
		break;
	}
}

void Room::mudd4KirkUsedViewScreen() {
	if (!_awayMission->mudd.viewScreenEnabled) {
		_awayMission->mudd.viewScreenEnabled = true;
		playVoc("SE2BIGAS");
		loadActorAnim(OBJECT_VIEWSCREEN, "s4crvs", 0xa5, 0x76);
		_awayMission->timers[3] = 5;
		_awayMission->timers[1] = 50;
		if (!_awayMission->mudd.gotPointsForEnablingViewscreen) {
			_awayMission->mudd.gotPointsForEnablingViewscreen = true;
			_awayMission->mudd.missionScore++;
		}
	} else {
		_awayMission->mudd.viewScreenEnabled = false;
		loadActorAnim(OBJECT_VIEWSCREEN, "s4crvf", 0xa5, 0x76);
		_awayMission->timers[3] = 5;
		_awayMission->timers[1] = 30;
	}
}

void Room::mudd4Timer1Expired() {
	_awayMission->disableInput = false;
	if (_awayMission->mudd.viewScreenEnabled)
		showText(TX_SPEAKER_KIRK, TX_MUD4_004);
	mudd4ShowRightConsoleMenu();
}

void Room::mudd4KirkUsedCommunications() {
	_awayMission->disableInput = false;
	if (!_awayMission->mudd.knowAboutTorpedo) {
		showText(TX_SPEAKER_SPOCK, TX_MUD4_054);
		mudd4ShowRightConsoleMenu();
	} else {
		if (!_awayMission->mudd.contactedEnterpriseFirstTime) {
			_awayMission->mudd.missionScore++;
			showText(TX_SPEAKER_KIRK,  TX_MUD4_025);
			showText(TX_SPEAKER_SCOTT, TX_MUD4_S02);

			if (_awayMission->mudd.torpedoStatus == 1)
				showText(TX_SPEAKER_KIRK, TX_MUD4_027);
			else
				showText(TX_SPEAKER_KIRK, TX_MUD4_028);

			showText(TX_SPEAKER_SCOTT, TX_MUD4_S04);

			const TextRef choices[] = {
				TX_SPEAKER_KIRK,
				TX_MUD4_024,
				TX_MUD4_033,
				TX_MUD4_023,
				TX_END
			};

			int choice = showMultipleTexts(choices);

			if (choice == 1) {
				endMission(_awayMission->mudd.missionScore, 0x1b, _awayMission->mudd.torpedoStatus);
			} else if (choice == 0) {
				_awayMission->mudd.contactedEnterpriseFirstTime = true;
			} else { // choice == 2
				mudd4TalkWithMuddAtMissionEnd();
			}
		} else {
			const TextRef choices[] = {
				TX_SPEAKER_KIRK,
				TX_MUD4_019,
				TX_MUD4_A29,
				TX_MUD4_023,
				TX_END
			};

			showText(TX_SPEAKER_KIRK,  TX_MUD4_020);
			showText(TX_SPEAKER_SCOTT, TX_MUD4_S03);
			int choice = showMultipleTexts(choices);

			if (choice == 1) {
				endMission(_awayMission->mudd.missionScore, 0x1b, _awayMission->mudd.torpedoStatus);
			} else if (choice == 2) {
				mudd4TalkWithMuddAtMissionEnd();
			}
		}
	}
}

void Room::mudd4TalkWithMuddAtMissionEnd() {
	if (_awayMission->mudd.muddUnavailable)
		return;
	else {
		_awayMission->disableInput = true;
		_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_E;
		walkCrewmanC(OBJECT_KIRK, 0x83, 0xb4, &Room::mudd4KirkReachedPositionToTalkToMudd);

		_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_E;
		walkCrewman(OBJECT_SPOCK, 0x69, 0xb7);

		_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_N;
		loadActorStandAnim(OBJECT_MCCOY);

		_awayMission->crewDirectionsAfterWalk[OBJECT_REDSHIRT] = DIR_E;
		loadActorStandAnim(OBJECT_REDSHIRT);

		playMidiMusicTracks(MIDITRACK_3);
		loadActorAnim(OBJECT_11, "s4crhw", 0x13f, 0xc7);
	}
}

void Room::mudd4KirkReachedPositionToTalkToMudd() {
	_awayMission->disableInput = false;

	if (_awayMission->mudd.discoveredLenseAndDegrimerFunction
	        || _awayMission->mudd.muddErasedDatabase
	        || _awayMission->mudd.databaseDestroyed
	        || !_awayMission->mudd.accessedAlienDatabase) { // NOTE: why this last line? Test this...
		const TextRef choices[] = {
			TX_SPEAKER_KIRK,
			TX_MUD4_009,
			TX_MUD4_016,
			TX_END
		};

		showText(TX_SPEAKER_MUDD, TX_MUD4_066);
		int choice = showMultipleTexts(choices);

		if (choice == 1) {
			// Copy of code at very bottom of function
			showText(TX_SPEAKER_MUDD, TX_MUD4_074);
			showText(TX_SPEAKER_KIRK, TX_MUD4_017);
			showText(TX_SPEAKER_MUDD, TX_MUD4_072);
			showText(TX_SPEAKER_KIRK, TX_MUD4_008);
			showText(TX_SPEAKER_MUDD, TX_MUD4_068);
			showText(TX_SPEAKER_KIRK, TX_MUD4_032);

			endMission(_awayMission->mudd.missionScore, 0x1b, _awayMission->mudd.torpedoStatus);
		} else {
			// Threaten to arrest Mudd.
			if (_awayMission->mudd.muddErasedDatabase || _awayMission->mudd.databaseDestroyed)
				showText(TX_SPEAKER_KIRK, TX_MUD4_034);
			if (_awayMission->mudd.discoveredLenseAndDegrimerFunction)
				showText(TX_SPEAKER_KIRK, TX_MUD4_037);

			showText(TX_SPEAKER_KIRK, TX_MUD4_026);

			if (_awayMission->mudd.muddErasedDatabase && _awayMission->mudd.databaseDestroyed) {
				// Mudd offers to copy the lost data to the enterprise computer. You don't
				// get any points for this outcome.

				// NOTE: This combination is probably impossible, making this unused?
				// (Either you give Mudd access to the database, or he destroys it.)

				showText(TX_SPEAKER_MUDD,  TX_MUD4_071);
				showText(TX_SPEAKER_KIRK,  TX_MUD4_036);
				showText(TX_SPEAKER_SPOCK, TX_MUD4_029);
				showText(TX_SPEAKER_KIRK,  TX_MUD4_030);
			} else {
				// Mudd is forced to agree to give samples to a university.
				showText(TX_SPEAKER_MUDD, TX_MUD4_064);
				showText(TX_SPEAKER_KIRK, TX_MUD4_012);
				showText(TX_SPEAKER_MUDD, TX_MUD4_070);
				showText(TX_SPEAKER_KIRK, TX_MUD4_010);
				showText(TX_SPEAKER_MUDD, TX_MUD4_067);
				showText(TX_SPEAKER_KIRK, TX_MUD4_011);

				_awayMission->mudd.missionScore += 4;
				playMidiMusicTracks(MIDITRACK_30);

				showText(TX_SPEAKER_MUDD, TX_MUD4_073);
				showText(TX_SPEAKER_KIRK, TX_MUD4_035);
			}
			showText(TX_SPEAKER_MUDD, TX_MUD4_069);
			showText(TX_SPEAKER_KIRK, TX_MUD4_001);

			endMission(_awayMission->mudd.missionScore, 0x1b, _awayMission->mudd.torpedoStatus);
		}
	} else {
		const TextRef choices[] = {
			TX_SPEAKER_KIRK,
			TX_MUD4_014,
			TX_MUD4_021,
			TX_END
		};

		showText(TX_SPEAKER_MUDD, TX_MUD4_065);
		int choice = showMultipleTexts(choices);

		if (choice == 1) {
			showText(TX_SPEAKER_MUDD, TX_MUD4_074);
			showText(TX_SPEAKER_KIRK, TX_MUD4_017);
		}
		showText(TX_SPEAKER_MUDD, TX_MUD4_072);
		showText(TX_SPEAKER_KIRK, TX_MUD4_008);
		showText(TX_SPEAKER_MUDD, TX_MUD4_068);
		showText(TX_SPEAKER_KIRK, TX_MUD4_032);

		endMission(_awayMission->mudd.missionScore, 0x1b, _awayMission->mudd.torpedoStatus);
	}
}

void Room::mudd4UseMccoyOnConsole() {
	// NOTE: This audio was recorded twice (TX_MUD4_045, and TX_MUD4_044 which is unused)
	showText(TX_SPEAKER_MCCOY, TX_MUD4_045);

	_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_N;
	walkCrewman(OBJECT_MCCOY, 0x7d, 0xc3);
}

void Room::mudd4UseRedshirtOnConsole() {
	// Floppy version shows two different pieces of text here, but in the CD version,
	// there's no difference
	//if (!_awayMission->mudd.translatedAlienLanguage)
	//showText(TX_SPEAKER_BUCHERT, TX_MUD4_075);
	//else
	showText(TX_SPEAKER_BUCHERT, TX_MUD4_075);

	_awayMission->crewDirectionsAfterWalk[OBJECT_REDSHIRT] = DIR_N;
	walkCrewman(OBJECT_REDSHIRT, 0x2a, 0xbb);
}

void Room::mudd4UseSTricorderOnRepairTool() {
	spockScan(DIR_E, TX_SPEAKER_SPOCK, TX_MUD4_051, false);
}

void Room::mudd4UseSTricorderOnConsole() {
	spockScan(DIR_N, TX_SPEAKER_SPOCK, TX_MUD4_042, false);
}

void Room::mudd4UseSTricorderOnViewscreen() {
	spockScan(DIR_N, TX_SPEAKER_SPOCK, TX_MUD4_050, false);
}

void Room::mudd4UseMedkit() {
	showText(TX_SPEAKER_MCCOY, TX_MUD4_043);
}

void Room::mudd4GetRepairTool() {
	if (_awayMission->mudd.tookRepairTool)
		showDescription(TX_MUD4N012); // NOTE: unused, since the object disappears, can't be selected again
	else {
		_awayMission->disableInput = true;
		_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_N;
		walkCrewmanC(OBJECT_KIRK, 0xcb, 0x9e, &Room::mudd4ReachedRepairTool);
	}
}

void Room::mudd4ReachedRepairTool() {
	loadActorAnimC(OBJECT_KIRK, "kuseln", -1, -1, &Room::mudd4PickedUpRepairTool);
}

void Room::mudd4PickedUpRepairTool() {
	_awayMission->disableInput = false;
	loadActorStandAnim(OBJECT_REPAIR_TOOL);
	_awayMission->mudd.tookRepairTool = true;
	_awayMission->mudd.missionScore++;
	giveItem(OBJECT_IDOOVER);
}

void Room::mudd4LookAtConsole() {
	showDescription(TX_MUD4N000);
}

void Room::mudd4LookAtViewscreen() {
	if (_awayMission->mudd.viewScreenEnabled)
		showDescription(TX_MUD4N008);
	else
		showDescription(TX_MUD4N013);
}

void Room::mudd4LookAtKirk() {
	showDescription(TX_MUD4N003);
}

void Room::mudd4LookAtSpock() {
	showDescription(TX_MUD4N006);
}

void Room::mudd4LookAtMccoy() {
	showDescription(TX_MUD4N001);
}

void Room::mudd4LookAtRedshirt() {
	showDescription(TX_MUD4N004);
}

void Room::mudd4LookAtRepairTool() {
	showDescription(TX_MUD4N011);
}

void Room::mudd4TalkToKirk() {
	showText(TX_SPEAKER_KIRK, TX_MUD4_005);
}

void Room::mudd4TalkToSpock() {
	showText(TX_SPEAKER_SPOCK, TX_MUD4_063);
	showText(TX_SPEAKER_KIRK,  TX_MUD4_015);
	showText(TX_SPEAKER_MCCOY, TX_MUD4_047);
}

void Room::mudd4TalkToMccoy() {
	showText(TX_SPEAKER_MCCOY, TX_MUD4_046);
}

void Room::mudd4TalkToRedshirt() {
	showText(TX_SPEAKER_BUCHERT, TX_MUD4_076);
}

void Room::mudd4WalkToEastDoor() {
	_roomVar.mudd.walkingToDoor = true;
	_awayMission->disableInput = true;
	walkCrewman(OBJECT_KIRK, 0xf0, 0xc7);
}

void Room::mudd4WalkToWestDoor() {
	_roomVar.mudd.walkingToDoor = true;
	_awayMission->disableInput = true;
	walkCrewman(OBJECT_KIRK, 0x50, 0xc7);
}

void Room::mudd4TouchedHotspot0() { // Trigger door sound
	if (_roomVar.mudd.walkingToDoor)
		playVoc("SMADOOR3");
}

}
