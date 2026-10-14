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

extern const RoomAction demon1ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::demon1Tick1 },

	{ {ACTION_WALK, 0x25, 0, 0, 0}, &Room::demon1WalkToCave },
	{ {ACTION_FINISHED_WALKING, 2, 0, 0, 0}, &Room::demon1TouchedTopWarp },
	{ {ACTION_TOUCHED_WARP, 0, 0, 0, 0}, &Room::demon1TouchedTopWarp },
	{ {ACTION_TOUCHED_WARP, 1, 0, 0, 0}, &Room::demon1TouchedBottomWarp },

	{ {ACTION_TIMER_EXPIRED, 2, 0, 0, 0}, &Room::demon1Timer2Expired },
	{ {ACTION_TIMER_EXPIRED, 0, 0, 0, 0}, &Room::demon1Timer0Expired },
	{ {ACTION_TIMER_EXPIRED, 3, 0, 0, 0}, &Room::demon1Timer3Expired },
	{ {ACTION_TIMER_EXPIRED, 1, 0, 0, 0}, &Room::demon1Timer1Expired },

	{ {ACTION_FINISHED_ANIMATION, 2, 0, 0, 0}, &Room::demon1KlingonFinishedAimingWeapon },
	{ {ACTION_FINISHED_ANIMATION, 1, 0, 0, 0}, &Room::demon1KirkShot },

	{ {ACTION_USE, OBJECT_IPHASERK, 0xff, 0, 0}, &Room::demon1UsePhaserOnAnything },
	{ {ACTION_USE, OBJECT_IPHASERS, 0xff, 0, 0}, &Room::demon1UsePhaserOnAnything },

	{ {ACTION_USE, OBJECT_IPHASERK, 8, 0, 0}, &Room::demon1UsePhaserOnKlingon1 },
	{ {ACTION_USE, OBJECT_IPHASERS, 8, 0, 0}, &Room::demon1UsePhaserOnKlingon1 },
	{ {ACTION_FINISHED_ANIMATION, 3, 0, 0, 0}, &Room::demon1ShootKlingon1 },
	{ {ACTION_FINISHED_ANIMATION, 7, 0, 0, 0}, &Room::demon1KlingonDropsHand },

	{ {ACTION_USE, OBJECT_IPHASERK, 9, 0, 0}, &Room::demon1UsePhaserOnKlingon2 },
	{ {ACTION_USE, OBJECT_IPHASERS, 9, 0, 0}, &Room::demon1UsePhaserOnKlingon2 },
	{ {ACTION_FINISHED_ANIMATION, 4, 0, 0, 0}, &Room::demon1ShootKlingon2 },

	{ {ACTION_USE, OBJECT_IPHASERK, 10, 0, 0}, &Room::demon1UsePhaserOnKlingon3 },
	{ {ACTION_USE, OBJECT_IPHASERS, 10, 0, 0}, &Room::demon1UsePhaserOnKlingon3 },
	{ {ACTION_FINISHED_ANIMATION, 5, 0, 0, 0}, &Room::demon1ShootKlingon3 },

	{ {ACTION_TIMER_EXPIRED, 6, 0, 0, 0}, &Room::demon1AllKlingonsDead },
	{ {ACTION_TIMER_EXPIRED, 5, 0, 0, 0}, &Room::demon1Timer5Expired },

	{ {ACTION_USE, OBJECT_IMTRICOR, 13, 0, 0}, &Room::demon1UseMTricorderOnKlingon },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x21, 0, 0}, &Room::demon1UseSTricorderOnTulips },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x22, 0, 0}, &Room::demon1UseSTricorderOnPods },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x20, 0, 0}, &Room::demon1UseSTricorderOnCattails },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x23, 0, 0}, &Room::demon1UseSTricorderOnFerns },
	{ {ACTION_USE, OBJECT_ISTRICOR, 13, 0, 0}, &Room::demon1UseSTricorderOnHand },
	{ {ACTION_USE, OBJECT_ISTRICOR, 8, 0, 0}, &Room::demon1UseSTricorderOnKlingon1 },
	{ {ACTION_USE, OBJECT_ISTRICOR, 10, 0, 0}, &Room::demon1UseSTricorderOnKlingon2Or3 },
	{ {ACTION_USE, OBJECT_ISTRICOR, 9, 0, 0}, &Room::demon1UseSTricorderOnKlingon2Or3 },
	{ {ACTION_USE, OBJECT_IMEDKIT,  8, 0, 0}, &Room::demon1UseMTricorderOnKlingon },
	{ {ACTION_USE, OBJECT_IMTRICOR, 8, 0, 0}, &Room::demon1UseMTricorderOnKlingon },
	{ {ACTION_USE, OBJECT_IMEDKIT, 10, 0, 0}, &Room::demon1UseMTricorderOnKlingon },
	{ {ACTION_USE, OBJECT_IMEDKIT,  9, 0, 0}, &Room::demon1UseMTricorderOnKlingon },
	{ {ACTION_USE, OBJECT_IMTRICOR, 10, 0, 0}, &Room::demon1UseMTricorderOnKlingon },
	{ {ACTION_USE, OBJECT_IMTRICOR,  9, 0, 0}, &Room::demon1UseMTricorderOnKlingon },
	{ {ACTION_USE, OBJECT_ISTRICOR, 8, 0, 0}, &Room::demon1UseSTricorderOnKlingon2Or3 }, // This is redundant

	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_KIRK, 0, 0}, &Room::demon1UseMTricorderOnKirk },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_SPOCK, 0, 0}, &Room::demon1UseMTricorderOnSpock },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_REDSHIRT, 0, 0}, &Room::demon1UseMTricorderOnRedshirt },

	{ {ACTION_GET, 13, 0, 0, 0}, &Room::demon1GetHand },
	{ {ACTION_FINISHED_WALKING, 1, 0, 0, 0}, &Room::demon1ReachedHand },
	{ {ACTION_FINISHED_ANIMATION, 6, 0, 0, 0}, &Room::demon1PickedUpHand },
	{ {ACTION_TIMER_EXPIRED, 4, 0, 0, 0}, &Room::demon1FinishedGettingHand },

	{ {ACTION_LOOK, 8, 0, 0, 0}, &Room::demon1LookAtKlingon },
	{ {ACTION_LOOK, 9, 0, 0, 0}, &Room::demon1LookAtKlingon },
	{ {ACTION_LOOK, 10, 0, 0, 0}, &Room::demon1LookAtKlingon },

	{ {ACTION_LOOK, 0x20, 0, 0, 0}, &Room::demon1LookAtCattails },
	{ {ACTION_LOOK, 0x21, 0, 0, 0}, &Room::demon1LookAtTulips },
	{ {ACTION_LOOK, 0x22, 0, 0, 0}, &Room::demon1LookAtPods },
	{ {ACTION_LOOK, 0x23, 0, 0, 0}, &Room::demon1LookAtFerns },
	{ {ACTION_LOOK, 0x24, 0, 0, 0}, &Room::demon1LookAtStream },
	{ {ACTION_LOOK, 0x25, 0, 0, 0}, &Room::demon1LookAtMine },
	{ {ACTION_LOOK, 0x26, 0, 0, 0}, &Room::demon1LookAtMountain },
	{ {ACTION_LOOK, 13, 0, 0, 0}, &Room::demon1LookAtHand },
	{ {ACTION_LOOK, 0xff, 0, 0, 0}, &Room::demon1LookAnywhere },
	{ {ACTION_LOOK, OBJECT_KIRK, 0, 0, 0}, &Room::demon1LookAtKirk },
	{ {ACTION_LOOK, OBJECT_SPOCK, 0, 0, 0}, &Room::demon1LookAtSpock },
	{ {ACTION_LOOK, OBJECT_MCCOY, 0, 0, 0}, &Room::demon1LookAtMcCoy },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::demon1LookAtRedshirt },

	{ {ACTION_TALK, OBJECT_KIRK, 0, 0, 0}, &Room::demon1TalkToKirk },
	{ {ACTION_TALK, OBJECT_SPOCK, 0, 0, 0}, &Room::demon1TalkToSpock },
	{ {ACTION_TALK, OBJECT_MCCOY, 0, 0, 0}, &Room::demon1TalkToMcCoy },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::demon1TalkToRedshirt },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum demon1TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_SPOCK, TX_SPEAKER_MCCOY, TX_SPEAKER_EVERTS, TX_SPEAKER_UHURA,
	TX_SPEAKER_KLINGON,
	TX_DEM1_001, TX_DEM1_002, TX_DEM1_003, TX_DEM1_004, TX_DEM1_005,
	TX_DEM1_006, TX_DEM1_007, TX_DEM1_008, TX_DEM1_009, TX_DEM1_011,
	TX_DEM1_012, TX_DEM1_013, TX_DEM1_014, TX_DEM1_015, TX_DEM1_016,
	TX_DEM1_017, TX_DEM1_018, TX_DEM1_019, TX_DEM1_020, TX_DEM1_021,
	TX_DEM1_022, TX_DEM1_023, TX_DEM1_024, TX_DEM1_025, TX_DEM1_026,
	TX_DEM1_F23, TX_DEM1N000, TX_DEM1N001, TX_DEM1N002, TX_DEM1N003,
	TX_DEM1N004, TX_DEM1N005, TX_DEM1N006, TX_DEM1N007, TX_DEM1N008,
	TX_DEM1N009, TX_DEM1N010, TX_DEM1N011, TX_DEM1N012, TX_DEM1N013,
	TX_DEM1N014, TX_DEM1N015, TX_DEM1N016, TX_DEM1N017, TX_DEM1N018,
	TX_DEM1N019, TX_DEM1N020, TX_DEM1N021, TX_DEM1N023, TX_DEM1U077,
	TX_DEM1U078
};

extern const RoomTextOffsets demon1TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 5869, 5380, 6162, 5980, 5397, 5512, 5694, 5397, 5872 },
	{ TX_SPEAKER_SPOCK, 5880, 5391, 6173, 5991, 5408, 5525, 5705, 5408, 5883 },
	{ TX_SPEAKER_MCCOY, 5890, 5401, 6183, 6001, 5418, 5535, 5715, 5418, 5893 },
	{ TX_SPEAKER_EVERTS, 5900, 5411, 6193, 6011, 5428, 5545, 5725, 5428, 5903 },
	{ TX_SPEAKER_UHURA, 5914, 5425, 6207, 6025, 5442, 5559, 5741, 5442, 5917 },
	{ TX_SPEAKER_KLINGON, 5924, 5435, 6217, 6035, 5452, 5569, 5751, 5452, 5927 },
	{ TX_DEM1_001, 7374, 6679, 7761, 7589, 6691, 6912, 7086, 6691, 7378 },
	{ TX_DEM1_002, 2710, 2547, 2790, 2774, 2562, 2631, 2648, 2562, 2712 },
	{ TX_DEM1_003, 2566, 2441, 2633, 2618, 2452, 2505, 2521, 2452, 2562 },
	{ TX_DEM1_004, 2906, 2712, 2999, 2952, 2728, 2780, 2826, 2728, 2908 },
	{ TX_DEM1_005, 3458, 3203, 3627, 3538, 3220, 3306, 3393, 3220, 3461 },
	{ TX_DEM1_006, 3669, 3399, 3866, 3744, 3416, 3497, 3617, 3416, 3672 },
	{ TX_DEM1_007, 3142, 2918, 3276, 3200, 2934, 2998, 3073, 2934, 3144 },
	{ TX_DEM1_008, 3285, 3045, 3440, 3358, 3062, 3140, 3221, 3062, 3287 },
	{ TX_DEM1_009, 4222, 3943, 4454, 4293, 3960, 4037, 4196, 3960, 4225 },
	{ TX_DEM1_011, 1492, 1466, 1492, 1492, 1466, 1466, 1466, 1466, 1488 },
	{ TX_DEM1_012, 6779, 6189, 7135, 6939, 6201, 6368, 6566, 6201, 6782 },
	{ TX_DEM1_013, 7645, 6919, 8036, 7877, 6932, 7169, 7331, 6932, 7649 },
	{ TX_DEM1_014, 7779, 7023, 8168, 8025, 7036, 7287, 7433, 7036, 7783 },
	{ TX_DEM1_015, 7904, 7133, 8299, 8163, 7146, 7408, 7549, 7146, 7908 },
	{ TX_DEM1_016, 7111, 6476, 7490, 7306, 6488, 6689, 6875, 6488, 7115 },
	{ TX_DEM1_017, 6837, 6232, 7196, 6996, 6245, 6410, 6612, 6245, 6841 },
	{ TX_DEM1_018, 6358, 5798, 6689, 6481, 5811, 5938, 6149, 5811, 6361 },
	{ TX_DEM1_019, 6926, 6306, 7301, 7092, 6319, 6491, 6702, 6319, 6930 },
	{ TX_DEM1_020, 2764, 2585, 2853, 2829, 2601, 2671, 2695, 2601, 2766 },
	{ TX_DEM1_021, 2951, 2742, 3053, 2997, 2758, 2810, 2865, 2758, 2953 },
	{ TX_DEM1_022, 7478, 6768, 7859, 7709, 6780, 7017, 7169, 6780, 7482 },
	{ TX_DEM1_023, 7983, 7197, 8412, 8254, 7210, 7484, 7647, 7210, 7987 },
	{ TX_DEM1_024, 7735, 6994, 8122, 7983, 7007, 7260, 7402, 7007, 7739 },
	{ TX_DEM1_025, 1937, 1855, 1949, 1969, 1855, 1887, 1870, 1855, 1933 },
	{ TX_DEM1_026, 8115, 7314, 8558, 8407, 7327, 7630, 7778, 7327, 8119 },
	{ TX_DEM1_F23, 2028, 1931, 2062, 2063, 1931, 1966, 1968, 1931, 2024 },
	{ TX_DEM1N000, 6289, 5744, 6603, 6417, 5757, 5889, 6077, 5757, 6292 },
	{ TX_DEM1N001, 6015, 5515, 6304, 6138, 5528, 5656, 5823, 5528, 6018 },
	{ TX_DEM1N002, 6209, 5679, 6519, 6341, 5692, 5828, 6008, 5692, 6212 },
	{ TX_DEM1N003, 5933, 5444, 6226, 6045, 5461, 5578, 5760, 5461, 5936 },
	{ TX_DEM1N004, 7334, 6654, 7714, 7545, 6666, 6883, 7054, 6666, 7338 },
	{ TX_DEM1N005, 7222, 6572, 7603, 7428, 6584, 6796, 6973, 6584, 7226 },
	{ TX_DEM1N006, 6742, 6167, 7085, 6896, 6179, 6340, 6531, 6179, 6745 },
	{ TX_DEM1N007, 7281, 6616, 7658, 7483, 6628, 6836, 7013, 6628, 7285 },
	{ TX_DEM1N008, 6118, 5603, 6416, 6241, 5616, 5743, 5920, 5616, 6121 },
	{ TX_DEM1N009, 4721, 4382, 4988, 4816, 4399, 4501, 4670, 4399, 4724 },
	{ TX_DEM1N010, 4561, 4252, 4811, 4649, 4269, 4363, 4523, 4269, 4564 },
	{ TX_DEM1N011, 4772, 4418, 5046, 4869, 4435, 4539, 4713, 4435, 4775 },
	{ TX_DEM1N012, 5145, 4716, 5423, 5260, 4733, 4852, 5015, 4733, 5148 },
	{ TX_DEM1N013, 5298, 4839, 5582, 5411, 4856, 4973, 5144, 4856, 5301 },
	{ TX_DEM1N014, 5373, 4899, 5661, 5485, 4916, 5032, 5208, 4916, 5376 },
	{ TX_DEM1N015, 5223, 4779, 5507, 5337, 4796, 4914, 5084, 4796, 5226 },
	{ TX_DEM1N016, 4917, 4533, 5195, 5013, 4550, 4653, 4832, 4550, 4920 },
	{ TX_DEM1N017, 5044, 4630, 5330, 5162, 4647, 4769, 4937, 4647, 5047 },
	{ TX_DEM1N018, 4486, 4192, 4738, 4579, 4209, 4308, 4465, 4209, 4489 },
	{ TX_DEM1N019, 4630, 4306, 4888, 4720, 4323, 4419, 4585, 4323, 4633 },
	{ TX_DEM1N020, 1814, 1750, 1825, 1824, 1750, 1760, 1761, 1750, 1810 },
	{ TX_DEM1N021, 4851, 4482, 5118, 4943, 4499, 4598, 4770, 4499, 4854 },
	{ TX_DEM1N023, 4977, 4578, 5262, 5086, 4595, 4708, 4884, 4595, 4980 },
	{ TX_DEM1U077, 2462, 2350, 2504, 2505, 2362, 2405, 2407, 2362, 2458 },
	{ TX_DEM1U078, 2641, 2498, 2708, 2701, 2508, 2573, 2581, 2508, 2637 },
	{          -1, 0,    0, 0, 0, 0, 0, 0, 0, 0 }
};

extern const RoomText demon1Texts[] = {
	{ -1, Common::UNK_LANG, "" }
};

// BUG: under certain circumstances, the klingons just stop firing?
void Room::demon1Tick1() {
	playVoc("DEM1LOOP");

	if (!_awayMission->demon.beatKlingons)
		playMidiMusicTracks(MIDITRACK_1);

	if (_awayMission->demon.enteredFrom == 1) { // Entered from south
		_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_N;
		_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_N;
		_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_N;
		_awayMission->crewDirectionsAfterWalk[OBJECT_REDSHIRT] = DIR_N;
	} else if (_awayMission->demon.enteredFrom == 2) { // Entered from north?
		_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_S;
		_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_S;
		_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_S;
		_awayMission->crewDirectionsAfterWalk[OBJECT_REDSHIRT] = DIR_S;
	}

	if (!_awayMission->demon.beatKlingons) {
		_awayMission->disableWalking = true;
		_awayMission->timers[2] = 50;
	} else {
		loadActorAnim(8, "klg1d2", 0x120, 0x82, 0);
		if (!_awayMission->demon.tookKlingonHand)
			loadActorAnim(13, "klghnd", 0x10b, 0x8e, 0);
		loadActorAnim(9, "klg2d2", 0xaa, 0x7c, 0);
		loadActorAnim(10, "klg3d2", 0, 0, 0);

		_roomVar.demon.klingonShot[0] = true;
		_roomVar.demon.klingonShot[1] = true;
		_roomVar.demon.klingonShot[2] = true;
		_roomVar.demon.numKlingonsKilled = 3;
	}
}

void Room::demon1WalkToCave() {
	if (_roomVar.demon.numKlingonsKilled != 3)
		return;
	walkCrewman(OBJECT_KIRK, 0xa3, 0x56, 2);
}

void Room::demon1TouchedTopWarp() {
	if (_awayMission->demon.warpsDisabled)
		return;
	loadRoomIndex(2, 1);
}

void Room::demon1TouchedBottomWarp() {
	if (_awayMission->demon.warpsDisabled)
		return;
	loadRoomIndex(0, 0);
}

void Room::demon1Timer2Expired() {
	if (_awayMission->demon.beatKlingons)
		return;
	_awayMission->demon.field37 = 1;
	_awayMission->demon.beatKlingons = true;
	_awayMission->demon.warpsDisabled = true;
	_awayMission->timers[0] = 5;
	_awayMission->timers[3] = 2;
	_awayMission->timers[1] = 100;
	loadActorAnim(8, "klg1u", 0x120, 0x82, 0);
}

void Room::demon1Timer0Expired() {
	loadActorAnim(9, "klg2u", 0xaa, 0x7c, 0);
}

void Room::demon1Timer3Expired() {
	loadActorAnim(10, "klg3u", 0xdb, 0x6d, 0);
}

void Room::demon1Timer1Expired() {
	int shooter;
	Common::String anim;

	if (!_roomVar.demon.klingonShot[0]) {
		anim = "klg1f";
		shooter = 8;

		switch (_roomVar.demon.attackIndex) {
		case 0:
			Common::strcpy_s(_roomVar.demon.d6, "klg1fr");
			break;
		case 1:
			Common::strcpy_s(_roomVar.demon.d6, "klg1fm");
			break;
		case 2:
			Common::strcpy_s(_roomVar.demon.d6, "klg1fs");
			break;
		case 3:
			Common::strcpy_s(_roomVar.demon.d6, "klg1fk");
			break;
		default:
			return;
		}
	} else if (!_roomVar.demon.klingonShot[1]) {
		anim = "klg2f";
		shooter = 9;
		switch (_roomVar.demon.attackIndex) {
		case 0:
			Common::strcpy_s(_roomVar.demon.d6, "klg2fr");
			break;
		case 1:
			Common::strcpy_s(_roomVar.demon.d6, "klg2fm");
			break;
		case 2:
			Common::strcpy_s(_roomVar.demon.d6, "klg2fs");
			break;
		case 3:
			Common::strcpy_s(_roomVar.demon.d6, "klg2fk");
			break;
		default:
			return;
		}
	} else {
		anim = "klg3f";
		shooter = 10;
		switch (_roomVar.demon.attackIndex) {
		case 0:
			Common::strcpy_s(_roomVar.demon.d6, "klg3fr");
			break;
		case 1:
			Common::strcpy_s(_roomVar.demon.d6, "klg3fm");
			break;
		case 2:
			Common::strcpy_s(_roomVar.demon.d6, "klg3fs");
			break;
		case 3:
			Common::strcpy_s(_roomVar.demon.d6, "klg3fk");
			break;
		default:
			return;
		}
	}

	loadActorAnim2(shooter, anim, -1, -1, 2);
}

void Room::demon1KlingonFinishedAimingWeapon() {
	loadActorAnim(11, _roomVar.demon.d6, 0, 0, 0);
	playSoundEffectIndex(kSfxPhaser);

	_roomVar.demon.attackIndex++;
	int crewman = OBJECT_REDSHIRT;
	if (_roomVar.demon.attackIndex == 1) {
		loadActorAnim2(OBJECT_REDSHIRT, "rstunn", -1, -1, 0);
		crewman = OBJECT_REDSHIRT;
	} else if (_roomVar.demon.attackIndex == 2) {
		loadActorAnim2(OBJECT_SPOCK, "sstunn", -1, -1, 0);
		crewman = OBJECT_SPOCK;
	} else if (_roomVar.demon.attackIndex == 3) {
		loadActorAnim2(OBJECT_MCCOY, "mstune", -1, -1, 0);
		crewman = OBJECT_MCCOY;
	} else if (_roomVar.demon.attackIndex == 4) {
		loadActorAnim2(OBJECT_KIRK, "kstunn", -1, -1, 1);
		crewman = OBJECT_KIRK;
	}

	_awayMission->crewDirectionsAfterWalk[crewman] = DIR_N;
	_awayMission->crewGetupTimers[crewman] = 650;
	_awayMission->crewDownBitset |= (1 << crewman);

	_awayMission->timers[1] = 100;
}

void Room::demon1KirkShot() {
	showDescription(TX_DEM1N000);
	showGameOverMenu();
}

void Room::demon1UsePhaserOnAnything() {
	if (_roomVar.demon.numKlingonsKilled == 3)
		showText(TX_SPEAKER_MCCOY, TX_DEM1_011);
}

void Room::demon1UsePhaserOnKlingon1() {
	if (_roomVar.demon.klingonShot[0])
		return;
	_awayMission->disableInput = 1;
	_roomVar.demon.klingonShot[0] = true;

	if (_roomVar.demon.kirkShooting) {
		demon1ShootKlingon1();
	} else {
		loadActorAnim2(OBJECT_KIRK, "kfiren", -1, -1, 3);
		_roomVar.demon.kirkShooting = true;
	}
}

void Room::demon1ShootKlingon1() {
	_roomVar.demon.numKlingonsKilled++;
	if (_roomVar.demon.numKlingonsKilled == 3) {
		_awayMission->timers[1] = 0;
		_awayMission->timers[5] = 180;
		_awayMission->timers[6] = 1;
		_awayMission->demon.warpsDisabled = false;
		_awayMission->disableWalking = false;
	}
	loadActorAnim(12, "s0ks1", 0, 0, 0);
	playSoundEffectIndex(kSfxPhaser);
	playVoc("EFX19");
	loadActorAnim2(8, "klg1d", 0x120, 0x84, 7);
}

void Room::demon1KlingonDropsHand() {
	loadActorAnim(13, "klghnd", 0x10b, 0x8e, 0);
	_awayMission->disableInput = 0;
	_awayMission->timers[1] = 0;
	showDescription(TX_DEM1N020);

	if (_awayMission->crewDownBitset & (1 << OBJECT_REDSHIRT))
		return;

	showText(TX_SPEAKER_EVERTS, TX_DEM1_025);

	if (_roomVar.demon.numKlingonsKilled == 3)
		return;

	showText(TX_SPEAKER_KLINGON, TX_DEM1_F23);

	_awayMission->timers[1] = 1;
}

void Room::demon1UsePhaserOnKlingon2() {
	if (_roomVar.demon.klingonShot[1])
		return;
	_awayMission->disableInput = 1;
	_roomVar.demon.klingonShot[1] = true;

	if (_roomVar.demon.kirkShooting) {
		demon1ShootKlingon2();
	} else {
		loadActorAnim2(OBJECT_KIRK, "kfiren", -1, -1, 4);
		_roomVar.demon.kirkShooting = true;
	}
}

void Room::demon1ShootKlingon2() {
	_roomVar.demon.numKlingonsKilled++;
	if (_roomVar.demon.numKlingonsKilled == 3) {
		_awayMission->timers[1] = 0;
		_awayMission->timers[5] = 180;
		_awayMission->timers[6] = 1;
		_awayMission->demon.warpsDisabled = false;
		_awayMission->disableWalking = false;
	}
	loadActorAnim(12, "s0ks2", 0, 0, 0);
	playSoundEffectIndex(kSfxPhaser);
	loadActorAnim2(9, "klg2d", 0xaa, 0x7c, 0);
	_awayMission->disableInput = 0;
}

void Room::demon1UsePhaserOnKlingon3() {
	if (_roomVar.demon.klingonShot[2])
		return;
	_awayMission->disableInput = 1;
	_roomVar.demon.klingonShot[2] = true;

	if (_roomVar.demon.kirkShooting) {
		demon1ShootKlingon3();
	} else {
		loadActorAnim2(OBJECT_KIRK, "kfiren", -1, -1, 5);
		_roomVar.demon.kirkShooting = true;
	}
}

void Room::demon1ShootKlingon3() {
	_roomVar.demon.numKlingonsKilled++;
	if (_roomVar.demon.numKlingonsKilled == 3) {
		_awayMission->timers[1] = 0;
		_awayMission->timers[5] = 180;
		_awayMission->timers[6] = 1;
		_awayMission->demon.warpsDisabled = false;
		_awayMission->disableWalking = false;
	}
	loadActorAnim(12, "s0ks3", 0, 0, 0);
	playSoundEffectIndex(kSfxPhaser);
	loadActorAnim2(10, "klg3d", 0, 0, 0);
	_awayMission->disableInput = 0;
}

// Timer 6 expired
void Room::demon1AllKlingonsDead() {
	_awayMission->crewGetupTimers[OBJECT_KIRK] = 45;
	_awayMission->crewGetupTimers[OBJECT_SPOCK] = 45;
	_awayMission->crewGetupTimers[OBJECT_MCCOY] = 45;
	_awayMission->crewGetupTimers[OBJECT_REDSHIRT] = 45;
}

void Room::demon1Timer5Expired() {
	if (_awayMission->crewDownBitset != 0)
		return;

	showText(TX_SPEAKER_UHURA, TX_DEM1U077);
	showText(TX_SPEAKER_KIRK, TX_DEM1_003);
	showText(TX_SPEAKER_UHURA, TX_DEM1U078);
	showText(TX_SPEAKER_KIRK, TX_DEM1_002);
	showText(TX_SPEAKER_SPOCK, TX_DEM1_020);
	showText(TX_SPEAKER_KIRK, TX_DEM1_004);
	showText(TX_SPEAKER_SPOCK, TX_DEM1_021);
}

void Room::demon1UseMTricorderOnKlingon() {
	loadActorAnim2(OBJECT_MCCOY, "mscann", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_MCCOY, TX_DEM1_012);
}

void Room::demon1UseSTricorderOnTulips() {
	loadActorAnim2(OBJECT_SPOCK, "sscann", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM1_007);
}

void Room::demon1UseSTricorderOnPods() {
	loadActorAnim2(OBJECT_SPOCK, "sscann", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM1_008);
}

void Room::demon1UseSTricorderOnCattails() {
	loadActorAnim2(OBJECT_SPOCK, "sscann", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM1_005);
}

void Room::demon1UseSTricorderOnFerns() {
	loadActorAnim2(OBJECT_SPOCK, "sscann", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM1_006);
}

void Room::demon1UseSTricorderOnHand() {
	loadActorAnim2(OBJECT_SPOCK, "sscann", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM1_017);
}

void Room::demon1UseSTricorderOnKlingon1() {
	// BUGFIX: Original game sets up the stack for this function call, but doesn't
	// actually call the function...
	loadActorAnim2(OBJECT_SPOCK, "sscann", -1, -1, 0);

	playSoundEffectIndex(kSfxTricorder);

	if (_roomVar.demon.numKlingonsKilled == 3 && !_awayMission->demon.tookKlingonHand && _rdfData[0xcf] != 1) {
		showText(TX_SPEAKER_SPOCK, TX_DEM1_018);
		_rdfData[0xcf] = 1;
	} else {
		showText(TX_SPEAKER_SPOCK, TX_DEM1_019);
	}
}

void Room::demon1UseSTricorderOnKlingon2Or3() {
	loadActorAnim2(OBJECT_SPOCK, "sscann", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM1_019);
}

void Room::demon1UseMTricorderOnKirk() {
	if (_awayMission->crewDownBitset & (1 << OBJECT_KIRK))
		demon1UseMTricorderOnCrewman();
}

void Room::demon1UseMTricorderOnSpock() {
	if (_awayMission->crewDownBitset & (1 << OBJECT_SPOCK))
		demon1UseMTricorderOnCrewman();
}

void Room::demon1UseMTricorderOnRedshirt() {
	if (_awayMission->crewDownBitset & (1 << OBJECT_REDSHIRT))
		demon1UseMTricorderOnCrewman();
}

void Room::demon1UseMTricorderOnCrewman() {
	loadActorAnim2(OBJECT_MCCOY, "mscann", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_MCCOY, TX_DEM1_009);
}

void Room::demon1GetHand() {
	if (_roomVar.demon.numKlingonsKilled != 3)
		return;
	walkCrewman(OBJECT_KIRK, 0x104, 0x8e, 1);
}

void Room::demon1ReachedHand() {
	loadActorAnim2(OBJECT_KIRK, "s5r1kg", -1, -1, 6);
}

void Room::demon1PickedUpHand() {
	loadActorStandAnim(13);
	_awayMission->demon.missionScore += 3;
	loadActorStandAnim(OBJECT_KIRK);
	_awayMission->timers[4] = 2;
}

// Timer 4 expired
void Room::demon1FinishedGettingHand() {

	if (_awayMission->demon.tookKlingonHand)
		showDescription(TX_DEM1N005);
	else {
		_awayMission->demon.tookKlingonHand = true;
		giveItem(OBJECT_IHAND);
		showDescription(TX_DEM1N007);
	}
}

void Room::demon1LookAtKlingon() {
	showDescription(TX_DEM1N004);
}

void Room::demon1LookAtCattails() {
	showDescription(TX_DEM1N018);
}

void Room::demon1LookAtTulips() {
	showDescription(TX_DEM1N010);
}

void Room::demon1LookAtPods() {
	showDescription(TX_DEM1N019);
}

void Room::demon1LookAtFerns() {
	showDescription(TX_DEM1N009);
}

void Room::demon1LookAtStream() {
	showDescription(TX_DEM1N011);
}

void Room::demon1LookAtMine() {
	showDescription(TX_DEM1N021);
}

void Room::demon1LookAtMountain() {
	showDescription(TX_DEM1N016);
}

void Room::demon1LookAtHand() {
	showDescription(TX_DEM1N023);
}
void Room::demon1LookAnywhere() {
	showDescription(TX_DEM1N017);
}

void Room::demon1LookAtKirk() {
	if (_awayMission->crewDownBitset & (1 << OBJECT_KIRK))
		showDescription(TX_DEM1N012);
	else
		showDescription(TX_DEM1N003);
}

void Room::demon1LookAtSpock() {
	if (_awayMission->crewDownBitset & (1 << OBJECT_SPOCK))
		showDescription(TX_DEM1N015);
	else
		showDescription(TX_DEM1N008);
}

void Room::demon1LookAtMcCoy() {
	if (_awayMission->crewDownBitset & (1 << OBJECT_MCCOY))
		showDescription(TX_DEM1N013);
	else
		showDescription(TX_DEM1N001);
}

void Room::demon1LookAtRedshirt() {
	if (_awayMission->crewDownBitset & (1 << OBJECT_REDSHIRT))
		showDescription(TX_DEM1N014);
	else
		showDescription(TX_DEM1N002);
}

void Room::demon1TalkToKirk() {
	if (_awayMission->crewDownBitset & (1 << OBJECT_KIRK))
		demon1TalkToUnconsciousCrewman();
	else if (_roomVar.demon.numKlingonsKilled == 3)
		showText(TX_SPEAKER_KIRK, TX_DEM1_001);
}

void Room::demon1TalkToSpock() {
	if (_awayMission->crewDownBitset & (1 << OBJECT_SPOCK))
		demon1TalkToUnconsciousCrewman();
	else {
		showText(TX_SPEAKER_SPOCK, TX_DEM1_022);
		showText(TX_SPEAKER_MCCOY, TX_DEM1_013);
		showText(TX_SPEAKER_SPOCK, TX_DEM1_024);
		showText(TX_SPEAKER_MCCOY, TX_DEM1_014);
	}
}

void Room::demon1TalkToMcCoy() {
	if (_awayMission->crewDownBitset & (1 << OBJECT_MCCOY))
		demon1TalkToUnconsciousCrewman();
	else {
		showText(TX_SPEAKER_MCCOY, TX_DEM1_015);
		showText(TX_SPEAKER_SPOCK, TX_DEM1_023);
	}
}

void Room::demon1TalkToRedshirt() {
	if (_awayMission->crewDownBitset & (1 << OBJECT_REDSHIRT))
		demon1TalkToUnconsciousCrewman();
	else
		showText(TX_SPEAKER_EVERTS, TX_DEM1_026);
}

// Same as in the original, but probably unreachable, since the engine doesn't allow
// talking to downed crewmen (see isObjectUnusable())
void Room::demon1TalkToUnconsciousCrewman() {
	const char *text[] = {
	    nullptr,
	    "Zzzzz....",
	    ""};
	showRoomSpecificText(text);
}

}
