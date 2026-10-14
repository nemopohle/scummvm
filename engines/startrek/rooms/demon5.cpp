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

namespace StarTrek {

extern const RoomAction demon5ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::demon5Tick1 },
	{ {ACTION_WALK, 0x22, 0, 0, 0}, &Room::demon5WalkToDoor },
	{ {ACTION_WALK, 12, 0, 0, 0}, &Room::demon5WalkToDoor },
	{ {ACTION_TOUCHED_HOTSPOT, 0, 0, 0, 0}, &Room::demon5TouchedDoorOpenTrigger },
	{ {ACTION_FINISHED_WALKING, 1, 0, 0, 0}, &Room::demon5DoorOpenedOrReachedDoor },
	{ {ACTION_FINISHED_ANIMATION, 1, 0, 0, 0}, &Room::demon5DoorOpenedOrReachedDoor },

	{ {ACTION_USE, OBJECT_ISTRICOR, 0x21, 0, 0}, &Room::demon5UseSTricorderOnCrate },
	{ {ACTION_USE, OBJECT_IPHASERK, 0xff, 0, 0}, &Room::demon5UsePhaserOnAnything },
	{ {ACTION_USE, OBJECT_IPHASERS, 0xff, 0, 0}, &Room::demon5UsePhaserOnAnything },

	{ {ACTION_USE, OBJECT_IHAND, 11, 0, 0}, &Room::demon5UseHandOnStephen },
	{ {ACTION_USE, OBJECT_IBERRY, 11, 0, 0}, &Room::demon5UseBerryOnStephen },
	{ {ACTION_USE, OBJECT_IDETOXIN, 9, 0, 0}, &Room::demon5UseHypoDytoxinOnChub },
	{ {ACTION_FINISHED_WALKING, 2, 0, 0, 0}, &Room::demon5MccoyReachedChub },
	{ {ACTION_FINISHED_ANIMATION, 2, 0, 0, 0}, &Room::demon5MccoyHealedChub },

	{ {ACTION_USE, OBJECT_IDETOXIN, 0xff, 0, 0}, &Room::demon5UseHypoDytoxinOnAnything },
	{ {ACTION_USE, OBJECT_IBERRY, 9, 0, 0}, &Room::demon5UseBerryOnChub },

	{ {ACTION_LOOK, 8, 0, 0, 0}, &Room::demon5LookAtRoberts },
	{ {ACTION_LOOK, 10, 0, 0, 0}, &Room::demon5LookAtGrisnash },
	{ {ACTION_LOOK, 11, 0, 0, 0}, &Room::demon5LookAtStephen },

	{ {ACTION_LOOK, OBJECT_KIRK, 0, 0, 0}, &Room::demon5LookAtKirk },
	{ {ACTION_LOOK, OBJECT_SPOCK, 0, 0, 0}, &Room::demon5LookAtSpock },
	{ {ACTION_LOOK, OBJECT_MCCOY, 0, 0, 0}, &Room::demon5LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::demon5LookAtRedshirt },

	{ {ACTION_LOOK, 0x20, 0, 0, 0}, &Room::demon5LookAtMountain },
	{ {ACTION_LOOK, 0x21, 0, 0, 0}, &Room::demon5LookAtCrate },
	{ {ACTION_LOOK, 0xff, 0, 0, 0}, &Room::demon5LookAnywhere },
	{ {ACTION_LOOK, 9, 0, 0, 0}, &Room::demon5LookAtChub },

	{ {ACTION_TALK, 8, 0, 0, 0}, &Room::demon5TalkToRoberts },
	{ {ACTION_TALK, 9, 0, 0, 0}, &Room::demon5TalkToChub },
	{ {ACTION_TALK, 10, 0, 0, 0}, &Room::demon5TalkToGrisnash },
	{ {ACTION_TALK, 11, 0, 0, 0}, &Room::demon5TalkToStephen },

	{ {ACTION_TALK, OBJECT_KIRK, 0, 0, 0}, &Room::demon5TalkToKirk },
	{ {ACTION_TALK, OBJECT_SPOCK, 0, 0, 0}, &Room::demon5TalkToSpock },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::demon5TalkToRedshirt },
	{ {ACTION_TALK, OBJECT_MCCOY, 0, 0, 0}, &Room::demon5TalkToMccoy },

	{ {ACTION_USE, OBJECT_IMTRICOR, 8, 0, 0}, &Room::demon5UseMTricorderOnRoberts },
	{ {ACTION_USE, OBJECT_IMEDKIT,  9, 0, 0}, &Room::demon5UseMTricorderOnChub },
	{ {ACTION_USE, OBJECT_MCCOY,    9, 0, 0}, &Room::demon5UseMTricorderOnChub },
	{ {ACTION_USE, OBJECT_IMTRICOR, 9, 0, 0}, &Room::demon5UseMTricorderOnChub },
	{ {ACTION_USE, OBJECT_IMTRICOR, 10, 0, 0}, &Room::demon5UseMTricorderOnGrisnash },
	{ {ACTION_USE, OBJECT_IMTRICOR, 11, 0, 0}, &Room::demon5UseMTricorderOnStephen },

	{ {ACTION_GET, 0x21, 0, 0, 0}, &Room::demon5GetCrate },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum demon5TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_EVERTS,
	TX_SPEAKER_ROBERTS, TX_SPEAKER_CHUB, TX_SPEAKER_GRISNASH, TX_SPEAKER_STEPHEN,
	TX_DEM5_001, TX_DEM5_002, TX_DEM5_003, TX_DEM5_004, TX_DEM5_005,
	TX_DEM5_006, TX_DEM5_007, TX_DEM5_008, TX_DEM5_009, TX_DEM5_010,
	TX_DEM5_011, TX_DEM5_012, TX_DEM5_013, TX_DEM5_014, TX_DEM5_015,
	TX_DEM5_016, TX_DEM5_017, TX_DEM5_018, TX_DEM5_019, TX_DEM5_020,
	TX_DEM5_021, TX_DEM5_022, TX_DEM5_023, TX_DEM5_024, TX_DEM5_025,
	TX_DEM5_026, TX_DEM5_027, TX_DEM5_028, TX_DEM5_029, TX_DEM5_030,
	TX_DEM5_031, TX_DEM5_032, TX_DEM5_033, TX_DEM5_034, TX_DEM5_035,
	TX_DEM5_036, TX_DEM5_037, TX_DEM5_038, TX_DEM5_039, TX_DEM5_040,
	TX_DEM5_041, TX_DEM5_042, TX_DEM5_043, TX_DEM5_044, TX_DEM5_045,
	TX_DEM5L027, TX_DEM5L028, TX_DEM5L029, TX_DEM5L030, TX_DEM5L031,
	TX_DEM5N000, TX_DEM5N001, TX_DEM5N002, TX_DEM5N003, TX_DEM5N004,
	TX_DEM5N005, TX_DEM5N006, TX_DEM5N007, TX_DEM5N008, TX_DEM5N009,
	TX_DEM5N010, TX_DEM5N011, TX_DEM5N012, TX_DEM5N013
};

extern const RoomTextOffsets demon5TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 3530, 3234, 3676, 3710, 3235, 3413, 3379, 3235, 3520 },
	{ TX_SPEAKER_MCCOY, 3541, 3245, 3687, 3721, 3246, 3427, 3391, 3246, 3531 },
	{ TX_SPEAKER_SPOCK, 3551, 3255, 3697, 3731, 3256, 3437, 3401, 3256, 3541 },
	{ TX_SPEAKER_EVERTS, 3561, 3265, 3707, 3741, 3266, 3447, 3411, 3266, 3551 },
	{ TX_SPEAKER_ROBERTS, 3575, 3279, 3721, 3755, 3280, 3461, 3427, 3280, 3565 },
	{ TX_SPEAKER_CHUB, 3591, 3295, 3737, 3770, 3296, 3476, 3441, 3296, 3581 },
	{ TX_SPEAKER_GRISNASH, 3604, 3308, 3750, 3782, 3309, 3488, 3452, 3309, 3594 },
	{ TX_SPEAKER_STEPHEN, 3621, 3325, 3767, 3798, 3326, 3504, 3467, 3326, 3611 },
	{ TX_DEM5_001, 7000, 6337, 7719, 7488, 6338, 6813, 7023, 6338, 6990 },
	{ TX_DEM5_002, 4928, 4461, 5233, 5173, 4461, 4682, 4734, 4461, 4918 },
	{ TX_DEM5_003, 7506, 6768, 8279, 7993, 6769, 7243, 7508, 6769, 7496 },
	{ TX_DEM5_004, 7798, 7014, 8607, 8297, 7016, 7502, 7792, 7016, 7788 },
	{ TX_DEM5_005, 5344, 4847, 5733, 5652, 4847, 5133, 5202, 4847, 5334 },
	{ TX_DEM5_006,  636, 610, 636, 636, 610, 610, 610, 610, 626 },
	{ TX_DEM5_007,  743, 701, 760, 766, 701, 725, 719, 701, 733 },
	{ TX_DEM5_008, 1506, 1373, 1596, 1580, 1374, 1449, 1465, 1374, 1496 },
	{ TX_DEM5_009, 1567, 1419, 1665, 1645, 1420, 1499, 1519, 1420, 1557 },
	{ TX_DEM5_010, 2942, 2671, 3067, 3102, 2672, 2836, 2801, 2672, 2932 },
	{ TX_DEM5_011, 9224, 8269, 10150, 9793, 8272, 8841, 9170, 8272, 9214 },
	{ TX_DEM5_012, 8086, 7227, 8942, 8599, 7229, 7738, 8052, 7229, 8076 },
	{ TX_DEM5_013, 8998, 8078, 9896, 9582, 8081, 8660, 8945, 8081, 8988 },
	{ TX_DEM5_014, 9101, 8166, 10031, 9693, 8169, 8756, 9066, 8169, 9091 },
	{ TX_DEM5_015, 8705, 7815, 9587, 9248, 7818, 8359, 8666, 7818, 8695 },
	{ TX_DEM5_016, 8808, 7903, 9698, 9363, 7906, 8458, 8762, 7906, 8798 },
	{ TX_DEM5_017, 7564, 6811, 8335, 8051, 6812, 7286, 7549, 6812, 7554 },
	{ TX_DEM5_018, 7136, 6443, 7861, 7614, 6444, 6909, 7135, 6444, 7126 },
	{ TX_DEM5_019, 8035, 7191, 8872, 8546, 7193, 7691, 7997, 7193, 8025 },
	{ TX_DEM5_020, 7941, 7127, 8771, 8445, 7129, 7620, 7926, 7129, 7931 },
	{ TX_DEM5_021, 7423, 6700, 8189, 7906, 6701, 7171, 7433, 6701, 7413 },
	{ TX_DEM5_022, 6203, 5600, 6759, 6604, 5601, 5980, 6124, 5601, 6193 },
	{ TX_DEM5_023, 7653, 6884, 8437, 8138, 6886, 7358, 7636, 6886, 7643 },
	{ TX_DEM5_024, 5638, 5095, 6111, 5965, 5096, 5401, 5536, 5096, 5628 },
	{ TX_DEM5_025, 5838, 5265, 6336, 6187, 5266, 5593, 5731, 5266, 5828 },
	{ TX_DEM5_026, 7991, 7162, 8826, 8500, 7164, 7660, 7966, 7164, 7981 },
	{ TX_DEM5_027, 7055, 6377, 7776, 7529, 6378, 6839, 7065, 6378, 7045 },
	{ TX_DEM5_028, 5408, 4881, 5803, 5726, 4881, 5177, 5242, 4881, 5398 },
	{ TX_DEM5_029, 5816, 5258, 6309, 6157, 5259, 5578, 5719, 5259, 5806 },
	{ TX_DEM5_030, 4526, 4066, 4731, 4745, 4066, 4285, 4265, 4066, 4516 },
	{ TX_DEM5_031, 6611, 5978, 7265, 7074, 5979, 6428, 6599, 5979, 6601 },
	{ TX_DEM5_032, 2308, 2085, 2403, 2438, 2086, 2217, 2182, 2086, 2298 },
	{ TX_DEM5_033, 1155, 1067, 1218, 1231, 1068, 1145, 1132, 1068, 1145 },
	{ TX_DEM5_034, 2488, 2235, 2619, 2643, 2236, 2392, 2368, 2236, 2478 },
	{ TX_DEM5_035, 2223, 2015, 2314, 2335, 2016, 2129, 2108, 2016, 2213 },
	{ TX_DEM5_036, 1080, 1007, 1126, 1151, 1008, 1080, 1055, 1008, 1070 },
	{ TX_DEM5_037, 1246, 1143, 1314, 1324, 1144, 1223, 1213, 1144, 1236 },
	{ TX_DEM5_038, 2398, 2160, 2508, 2543, 2161, 2307, 2272, 2161, 2388 },
	{ TX_DEM5_039,  903, 845, 941, 951, 846, 895, 885, 846, 893 },
	{ TX_DEM5_040, 8411, 7536, 9293, 8941, 7539, 8065, 8387, 7539, 8401 },
	{ TX_DEM5_041, 5897, 5309, 6399, 6247, 5310, 5638, 5779, 5310, 5887 },
	{ TX_DEM5_042, 6680, 6032, 7346, 7144, 6033, 6484, 6666, 6033, 6670 },
	{ TX_DEM5_043, 6286, 5668, 6864, 6701, 5669, 6062, 6214, 5669, 6276 },
	{ TX_DEM5_044, 7849, 7050, 8666, 8349, 7052, 7539, 7836, 7052, 7839 },
	{ TX_DEM5_045, 7268, 6560, 8010, 7757, 6561, 7037, 7269, 6561, 7258 },
	{ TX_DEM5L027, 1423, 1305, 1513, 1500, 1306, 1384, 1397, 1306, 1413 },
	{ TX_DEM5L028, 3700, 3389, 3866, 3870, 3390, 3561, 3549, 3390, 3690 },
	{ TX_DEM5L029, 4772, 4320, 5048, 5016, 4320, 4539, 4564, 4320, 4762 },
	{ TX_DEM5L030, 4982, 4500, 5296, 5235, 4500, 4730, 4782, 4500, 4972 },
	{ TX_DEM5L031, 5389, 4877, 5783, 5705, 4877, 5171, 5237, 4877, 5379 },
	{ TX_DEM5N000, 4183, 3798, 4375, 4386, 3798, 4003, 3983, 3798, 4173 },
	{ TX_DEM5N001, 3637, 3341, 3783, 3813, 3342, 3519, 3481, 3342, 3627 },
	{ TX_DEM5N002, 4033, 3678, 4221, 4209, 3678, 3855, 3859, 3678, 4023 },
	{ TX_DEM5N003, 3918, 3577, 4095, 4099, 3578, 3760, 3748, 3578, 3908 },
	{ TX_DEM5N004, 4111, 3741, 4290, 4302, 3741, 3934, 3913, 3741, 4101 },
	{ TX_DEM5N005, 4298, 3883, 4494, 4511, 3883, 4098, 4072, 3883, 4288 },
	{ TX_DEM5N006, 4440, 3995, 4639, 4647, 3995, 4203, 4188, 3995, 4430 },
	{ TX_DEM5N007, 4245, 3845, 4434, 4452, 3845, 4054, 4027, 3845, 4235 },
	{ TX_DEM5N008, 4378, 3948, 4569, 4589, 3948, 4160, 4133, 3948, 4368 },
	{ TX_DEM5N009, 3777, 3451, 3966, 3950, 3452, 3626, 3634, 3452, 3767 },
	{ TX_DEM5N010, 1811, 1648, 1892, 1888, 1649, 1727, 1731, 1649, 1801 },
	{ TX_DEM5N011, 2066, 1873, 2153, 2159, 1874, 1968, 1962, 1874, 2056 },
	{ TX_DEM5N012, 3276, 2995, 3414, 3453, 2996, 3172, 3133, 2996, 3266 },
	{ TX_DEM5N013, 1923, 1745, 2008, 2011, 1746, 1835, 1832, 1746, 1913 },
	{          -1, 0,    0, 0, 0, 0, 0, 0, 0, 0 }
};

extern const RoomText demon5Texts[] = {
	{ -1, Common::UNK_LANG, "" }
};

void Room::demon5Tick1() {
	playVoc("DEM5LOOP");

	if (!_awayMission->demon.curedChub || _awayMission->demon.field37 != 1)
		loadActorAnim(11, "oldman", 0x7a, 0xb0, 0);
	loadActorAnim(8, "brorob", 0xc9, 0xa8, 0);
	loadActorAnim(9, "crip", 0xe3, 0xa8, 0);
	loadActorAnim(10, "brotel", 0xf9, 0xbc, 0);
	loadActorAnim(12, "s0r6d2", 0xa0, 0x92, 0);
}

void Room::demon5WalkToDoor() {
	_awayMission->disableInput = true;
	_roomVar.demon.movingToDoor = true;
	walkCrewman(OBJECT_KIRK, 0xa0, 0x94, 1);
}

void Room::demon5TouchedDoorOpenTrigger() {
	if (!_roomVar.demon.movingToDoor)
		return;
	loadActorAnim(12, "s0r6d1", 0xa0, 0x92, 1);
	playSoundEffectIndex(kSfxDoor);
}

void Room::demon5DoorOpenedOrReachedDoor() {
	// This is invoked when the door opens and when Kirk reaches the door.
	// Must wait for both to occur.
	_roomVar.demon.doorCounter++;
	if (_roomVar.demon.doorCounter == 2)
		loadRoomIndex(0, 1);
}

void Room::demon5UseSTricorderOnCrate() {
	loadActorAnim(OBJECT_SPOCK, "sscans", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM5_006);
}

// Like in the original, the engine handles phasers used on crewmen itself
void Room::demon5UsePhaserOnAnything() {
	showText(TX_SPEAKER_SPOCK, TX_DEM5_007);
}

void Room::demon5UseHandOnStephen() {
	if (_awayMission->demon.repairedHand)
		showText(TX_SPEAKER_STEPHEN, TX_DEM5_036);
	else
		showText(TX_SPEAKER_STEPHEN, TX_DEM5_039);
}

void Room::demon5UseBerryOnStephen() {
	if (_awayMission->demon.knowAboutHypoDytoxin)
		showText(TX_SPEAKER_STEPHEN, TX_DEM5_033);
	else
		showText(TX_SPEAKER_STEPHEN, TX_DEM5_037);
}

void Room::demon5UseHypoDytoxinOnChub() {
	walkCrewman(OBJECT_MCCOY, 0xe1, 0xb0, 2);
}

void Room::demon5MccoyReachedChub() {
	loadActorAnim2(OBJECT_MCCOY, "musemn", -1, -1, 2);
}

void Room::demon5MccoyHealedChub() {
	walkCrewman(OBJECT_MCCOY, 0xc8, 0xba, 0);
	_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_W;
	showText(TX_SPEAKER_CHUB, TX_DEM5L027);
	loseItem(OBJECT_IDETOXIN);
	_awayMission->demon.curedChub = true;
	_awayMission->demon.missionScore += 2;
}

void Room::demon5UseHypoDytoxinOnAnything() {
	showText(TX_SPEAKER_MCCOY, TX_DEM5_008);
}

void Room::demon5UseBerryOnChub() {
	showText(TX_SPEAKER_MCCOY, TX_DEM5_009);
}

void Room::demon5LookAtRoberts() {
	showDescription(TX_DEM5N001);
}

void Room::demon5LookAtGrisnash() {
	showDescription(TX_DEM5N009);
}

void Room::demon5LookAtStephen() {
	showDescription(TX_DEM5N003);
}

void Room::demon5LookAtKirk() {
	showDescription(TX_DEM5N006);
}

void Room::demon5LookAtSpock() {
	showDescription(TX_DEM5N008);
}

void Room::demon5LookAtMccoy() {
	showDescription(TX_DEM5N007);
}

void Room::demon5LookAtRedshirt() {
	showDescription(TX_DEM5N005);
}

void Room::demon5LookAtMountain() {
	showDescription(TX_DEM5N002);
}

void Room::demon5LookAtCrate() {
	showDescription(TX_DEM5N004);
}

void Room::demon5LookAnywhere() {
	showDescription(TX_DEM5N000);
}

void Room::demon5LookAtChub() {
	showText(TX_SPEAKER_CHUB, TX_DEM5L028);
}

void Room::demon5TalkToRoberts() {
	if (_awayMission->demon.curedChub) {
		showText(TX_SPEAKER_ROBERTS, TX_DEM5_030);
		if (!_roomVar.demon.talkedToRoberts) {
			_roomVar.demon.talkedToRoberts = true;
			_roomVar.demon.numTalkedTo++;
			demon5CheckCompletedStudy();
		}
	} else {
		showDescription(TX_DEM5N010);
	}
}

void Room::demon5TalkToChub() {
	if (_awayMission->demon.curedChub) {
		showText(TX_SPEAKER_CHUB, TX_DEM5L029);
		showText(TX_SPEAKER_KIRK, TX_DEM5_002);
		showText(TX_SPEAKER_CHUB, TX_DEM5L030);
		showText(TX_SPEAKER_KIRK, TX_DEM5_005);
		showText(TX_SPEAKER_CHUB, TX_DEM5L031);

		if (!_roomVar.demon.talkedToChub) {
			_roomVar.demon.talkedToChub = true;
			_roomVar.demon.numTalkedTo++;
			demon5CheckCompletedStudy();
		}
	} else {
		showDescription(TX_DEM5N013);
	}
}

void Room::demon5TalkToGrisnash() {
	if (_awayMission->demon.curedChub) {
		showText(TX_SPEAKER_GRISNASH, TX_DEM5_028);
		showText(TX_SPEAKER_SPOCK, TX_DEM5_024);
		showText(TX_SPEAKER_GRISNASH, TX_DEM5_029);
		showText(TX_SPEAKER_SPOCK, TX_DEM5_025);

		if (!_roomVar.demon.talkedToGrisnash) {
			_roomVar.demon.talkedToGrisnash = true;
			_roomVar.demon.numTalkedTo++;
			demon5CheckCompletedStudy();
		}
	} else {
		showDescription(TX_DEM5N011);
	}
}

void Room::demon5TalkToStephen() {
	if (_awayMission->demon.curedChub) {
		showText(TX_SPEAKER_STEPHEN, TX_DEM5_041);
		showText(TX_SPEAKER_MCCOY, TX_DEM5_022);
		showText(TX_SPEAKER_STEPHEN, TX_DEM5_043);
		showText(TX_SPEAKER_ROBERTS, TX_DEM5_031);
		showText(TX_SPEAKER_STEPHEN, TX_DEM5_042);

		if (!_roomVar.demon.talkedToStephen) {
			_roomVar.demon.talkedToStephen = true;
			_roomVar.demon.numTalkedTo++;
			demon5CheckCompletedStudy();
		}
	} else if (!_awayMission->demon.knowAboutHypoDytoxin) {
		showText(TX_SPEAKER_ROBERTS, TX_DEM5_034);
	} else if (_awayMission->demon.madeHypoDytoxin) {
		showText(TX_SPEAKER_ROBERTS, TX_DEM5_035);
	} else if (_awayMission->demon.gotBerries) {
		showText(TX_SPEAKER_ROBERTS, TX_DEM5_032);
		_awayMission->demon.field3e = false;
	} else if (_awayMission->demon.field3e) {
		showText(TX_SPEAKER_ROBERTS, TX_DEM5_038);
	} else {
		showText(TX_SPEAKER_ROBERTS, TX_DEM5_034);
	}
}

void Room::demon5TalkToKirk() {
	showText(TX_SPEAKER_KIRK, TX_DEM5_001);
}

void Room::demon5TalkToSpock() {
	showText(TX_SPEAKER_SPOCK, TX_DEM5_027);
	showText(TX_SPEAKER_MCCOY, TX_DEM5_018);
}

void Room::demon5TalkToRedshirt() {
	showText(TX_SPEAKER_EVERTS, TX_DEM5_045);
	showText(TX_SPEAKER_MCCOY,  TX_DEM5_021);
	showText(TX_SPEAKER_KIRK,   TX_DEM5_003);
}

void Room::demon5TalkToMccoy() {
	if (_awayMission->demon.curedChub) {
		showText(TX_SPEAKER_MCCOY, TX_DEM5_023);
		showText(TX_SPEAKER_KIRK,  TX_DEM5_004);
		if (!_awayMission->redshirtDead) {
			showText(TX_SPEAKER_EVERTS, TX_DEM5_044);
			showText(TX_SPEAKER_MCCOY, TX_DEM5_020);
			showText(TX_SPEAKER_SPOCK, TX_DEM5_026);
			showText(TX_SPEAKER_MCCOY, TX_DEM5_019);
		}
	} else {
		showText(TX_SPEAKER_MCCOY, TX_DEM5_017);
	}
}

void Room::demon5UseMTricorderOnRoberts() {
	if (_roomVar.demon.scannedRoberts)
		return;
	loadActorAnim2(OBJECT_MCCOY, "mscane", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_MCCOY, TX_DEM5_015);

	_roomVar.demon.scannedRoberts = true;
	_roomVar.demon.numScanned++;
	demon5CheckCompletedStudy();
}

void Room::demon5UseMTricorderOnChub() {
	loadActorAnim2(OBJECT_MCCOY, "mscane", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);

	if (_awayMission->demon.curedChub) {
		if (_roomVar.demon.scannedChub)
			return;
		showText(TX_SPEAKER_MCCOY, TX_DEM5_016);
		_roomVar.demon.scannedChub = true;
		_roomVar.demon.numScanned++;
		demon5CheckCompletedStudy();
	} else {
		if (_awayMission->demon.field3e)
			showText(TX_SPEAKER_MCCOY, TX_DEM5_010);
		else {
			showText(TX_SPEAKER_MCCOY, TX_DEM5_012);
			showText(TX_SPEAKER_STEPHEN, TX_DEM5_040);
			_awayMission->demon.knowAboutHypoDytoxin = true;
			_awayMission->demon.field3e = true;
		}
	}
}


void Room::demon5UseMTricorderOnGrisnash() {
	if (_roomVar.demon.scannedGrisnash)
		return;
	loadActorAnim2(OBJECT_MCCOY, "mscane", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_MCCOY, TX_DEM5_013);

	_roomVar.demon.scannedGrisnash = true;
	_roomVar.demon.numScanned++;
	demon5CheckCompletedStudy();
}

void Room::demon5UseMTricorderOnStephen() {
	if (_roomVar.demon.scannedStephen)
		return;
	loadActorAnim2(OBJECT_MCCOY, "mscanw", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_MCCOY, TX_DEM5_014);

	_roomVar.demon.scannedStephen = true;
	_roomVar.demon.numScanned++;
	demon5CheckCompletedStudy();
}

void Room::demon5CheckCompletedStudy() {
	if (_roomVar.demon.numScanned == 4 && _roomVar.demon.numTalkedTo == 4) {
		showText(TX_SPEAKER_MCCOY, TX_DEM5_011);
		_roomVar.demon.numTalkedTo = 5;
	}
}

void Room::demon5GetCrate() {
	showDescription(TX_DEM5N012);
}

}
