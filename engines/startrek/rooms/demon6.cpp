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

extern const RoomAction demon6ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::demon6Tick1 },
	{ {ACTION_TICK, 30, 0, 0, 0}, &Room::demon6Tick30 },
	{ {ACTION_FINISHED_WALKING, 3, 0, 0, 0}, &Room::demon6SpockReachedComputer },

	{ {ACTION_WALK, 0x26, 0, 0, 0}, &Room::demon6WalkToDoor },
	{ {ACTION_WALK, 10, 0, 0, 0}, &Room::demon6WalkToDoor },
	{ {ACTION_TOUCHED_HOTSPOT, 0, 0, 0, 0}, &Room::demon6TouchedDoorOpenTrigger },
	{ {ACTION_FINISHED_WALKING, 4, 0, 0, 0}, &Room::demon6DoorOpenedOrReachedDoor },
	{ {ACTION_FINISHED_ANIMATION, 2, 0, 0, 0}, &Room::demon6DoorOpenedOrReachedDoor },
	{ {ACTION_USE, OBJECT_IPHASERK, 8, 0, 0}, &Room::demon6UsePhaserOnStephen },
	{ {ACTION_USE, OBJECT_IPHASERS, 8, 0, 0}, &Room::demon6UsePhaserOnStephen },
	{ {ACTION_USE, OBJECT_IPHASERK, 0x20, 0, 0}, &Room::demon6UsePhaserOnCase },
	{ {ACTION_USE, OBJECT_IPHASERS, 0x20, 0, 0}, &Room::demon6UsePhaserOnCase },
	{ {ACTION_LOOK, 0x22, 0, 0, 0}, &Room::demon6LookAtWorkspace },
	{ {ACTION_LOOK, 0x20, 0, 0, 0}, &Room::demon6LookAtCase },
	{ {ACTION_LOOK, 0x25, 0, 0, 0}, &Room::demon6LookAtTable },
	{ {ACTION_LOOK, OBJECT_IMINERAL, 0, 0, 0}, &Room::demon6LookAtMineral },
	{ {ACTION_LOOK, OBJECT_ISHELLS, 0, 0, 0}, &Room::demon6LookAtShells },
	{ {ACTION_LOOK, OBJECT_ISKULL, 0, 0, 0}, &Room::demon6LookAtSkull },
	{ {ACTION_LOOK, OBJECT_IMETAL, 0, 0, 0}, &Room::demon6LookAtMetal },
	{ {ACTION_LOOK, OBJECT_IMETEOR, 0, 0, 0}, &Room::demon6LookAtMeteor },
	{ {ACTION_LOOK, 0x24, 0, 0, 0}, &Room::demon6LookAtMountain },
	{ {ACTION_LOOK, 0x23, 0, 0, 0}, &Room::demon6LookAtSynthesizer },
	{ {ACTION_LOOK, OBJECT_KIRK, 0, 0, 0}, &Room::demon6LookAtKirk },
	{ {ACTION_LOOK, OBJECT_SPOCK, 0, 0, 0}, &Room::demon6LookAtSpock },
	{ {ACTION_LOOK, OBJECT_MCCOY, 0, 0, 0}, &Room::demon6LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::demon6LookAtRedshirt },
	{ {ACTION_LOOK, 0x21, 0, 0, 0}, &Room::demon6LookAtComputer },
	{ {ACTION_LOOK, 9, 0, 0, 0}, &Room::demon6LookAtComputer },
	{ {ACTION_LOOK, 0xff, 0, 0, 0}, &Room::demon6LookAnywhere },
	{ {ACTION_LOOK, 8, 0, 0, 0}, &Room::demon6LookAtStephen },

	{ {ACTION_TALK, OBJECT_MCCOY, 0, 0, 0}, &Room::demon6TalkToMccoy },
	{ {ACTION_TALK, OBJECT_SPOCK, 0, 0, 0}, &Room::demon6TalkToSpock },
	{ {ACTION_TALK, OBJECT_KIRK, 0, 0, 0}, &Room::demon6TalkToKirk },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::demon6TalkToRedshirt },
	{ {ACTION_TALK, 8, 0, 0, 0}, &Room::demon6TalkToStephen },
	{ {ACTION_USE, OBJECT_IBERRY, 8, 0, 0}, &Room::demon6UseBerryOnStephen },
	{ {ACTION_USE, OBJECT_IBERRY, 0x23, 0, 0}, &Room::demon6UseBerryOnSynthesizer },
	{ {ACTION_FINISHED_WALKING, 1, 0, 0, 0}, &Room::demon6MccoyReachedSynthesizer },
	{ {ACTION_FINISHED_ANIMATION, 1, 0, 0, 0}, &Room::demon6FinishedMakingHypoDytoxin },
	{ {ACTION_USE, OBJECT_IHAND, 0x22, 0, 0}, &Room::demon6UseHandOnWorkspace },
	{ {ACTION_FINISHED_WALKING, 2, 0, 0, 0}, &Room::demon6SpockReachedWorkspace },
	{ {ACTION_FINISHED_ANIMATION, 3, 0, 0, 0}, &Room::demon6SpockFinishedRepairingHand },
	{ {ACTION_USE, 0xff, 0x22, 0, 0}, &Room::demon6UseAnythingOnWorkspace },
	{ {ACTION_USE, OBJECT_KIRK, 0x20, 0, 0}, &Room::demon6UseCrewmanOnCase },
	{ {ACTION_USE, OBJECT_MCCOY, 0x20, 0, 0}, &Room::demon6UseCrewmanOnCase },
	{ {ACTION_USE, OBJECT_SPOCK, 0x20, 0, 0}, &Room::demon6UseCrewmanOnCase },
	{ {ACTION_USE, OBJECT_REDSHIRT, 0x20, 0, 0}, &Room::demon6UseCrewmanOnCase },

	{ {ACTION_USE, OBJECT_KIRK, 0x21, 0, 0}, &Room::demon6UseKirkOnComputer },
	{ {ACTION_USE, OBJECT_KIRK, 9, 0, 0}, &Room::demon6UseKirkOnComputer },
	{ {ACTION_USE, OBJECT_MCCOY, 0x21, 0, 0}, &Room::demon6UseMccoyOnComputer },
	{ {ACTION_USE, OBJECT_MCCOY, 9, 0, 0}, &Room::demon6UseMccoyOnComputer },
	{ {ACTION_USE, OBJECT_REDSHIRT, 0x21, 0, 0}, &Room::demon6UseRedshirtOnComputer },
	{ {ACTION_USE, OBJECT_REDSHIRT, 9, 0, 0}, &Room::demon6UseRedshirtOnComputer },
	{ {ACTION_USE, OBJECT_SPOCK, 0x21, 0, 0}, &Room::demon6UseSpockOnComputer },
	{ {ACTION_USE, OBJECT_SPOCK, 9, 0, 0}, &Room::demon6UseSpockOnComputer },
	{ {ACTION_USE, OBJECT_IMINERAL, 8, 0, 0}, &Room::demon6UseMineralOnStephen },
	{ {ACTION_USE, OBJECT_ISHELLS, 8, 0, 0}, &Room::demon6UseShellsOnStephen },
	{ {ACTION_USE, OBJECT_IMETEOR, 8, 0, 0}, &Room::demon6UseMeteorOnStephen },
	{ {ACTION_USE, OBJECT_ISKULL, 8, 0, 0}, &Room::demon6UseSkullOnStephen },
	{ {ACTION_USE, OBJECT_IMETAL, 8, 0, 0}, &Room::demon6UseMetalOnStephen },
	{ {ACTION_USE, OBJECT_IHAND, 8, 0, 0}, &Room::demon6UseHandOnStephen },
	{ {ACTION_USE, OBJECT_IMTRICOR, 8, 0, 0}, &Room::demon6UseMTricoderOnStephen },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x25, 0, 0}, &Room::demon6UseSTricoderOnTable },
	{ {ACTION_USE, OBJECT_ISTRICOR, 9, 0, 0}, &Room::demon6UseSTricoderOnComputer },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x21, 0, 0}, &Room::demon6UseSTricoderOnComputer },
	{ {ACTION_USE, OBJECT_ISTRICOR, OBJECT_IMINERAL, 0, 0}, &Room::demon6UseSTricoderOnMineral },
	{ {ACTION_USE, OBJECT_ISTRICOR, OBJECT_ISHELLS, 0, 0}, &Room::demon6UseSTricoderOnShells },
	{ {ACTION_USE, OBJECT_ISTRICOR, OBJECT_ISKULL, 0, 0}, &Room::demon6UseSTricoderOnSkull },
	{ {ACTION_USE, OBJECT_ISTRICOR, OBJECT_IMETAL, 0, 0}, &Room::demon6UseSTricoderOnMetal },
	{ {ACTION_USE, OBJECT_ISTRICOR, OBJECT_IMETEOR, 0, 0}, &Room::demon6UseSTricoderOnMeteor },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x20, 0, 0}, &Room::demon6UseSTricoderOnCase },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x23, 0, 0}, &Room::demon6UseSTricoderOnSynthesizer },
	{ {ACTION_GET, 0x20, 0, 0, 0}, &Room::demon6GetCase },
	{ {ACTION_FINISHED_WALKING, 5, 0, 0, 0}, &Room::demon6KirkReachedCase },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum demon6TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_EVERTS,
	TX_SPEAKER_STEPHEN,
	TX_DEM6_001, TX_DEM6_002, TX_DEM6_003, TX_DEM6_004, TX_DEM6_005,
	TX_DEM6_006, TX_DEM6_007, TX_DEM6_008, TX_DEM6_009, TX_DEM6_010,
	TX_DEM6_011, TX_DEM6_012, TX_DEM6_013, TX_DEM6_014, TX_DEM6_015,
	TX_DEM6_016, TX_DEM6_017, TX_DEM6_018, TX_DEM6_019, TX_DEM6_020,
	TX_DEM6_021, TX_DEM6_022, TX_DEM6_023, TX_DEM6_024, TX_DEM6_025,
	TX_DEM6_026, TX_DEM6_027, TX_DEM6_028, TX_DEM6_029, TX_DEM6_030,
	TX_DEM6_031, TX_DEM6_032, TX_DEM6_033, TX_DEM6_034, TX_DEM6_035,
	TX_DEM6_036, TX_DEM6_037, TX_DEM6_038, TX_DEM6_039, TX_DEM6_040,
	TX_DEM6_041, TX_DEM6_042, TX_DEM6_043, TX_DEM6_044, TX_DEM6_045,
	TX_DEM6_046, TX_DEM6_047, TX_DEM6_048, TX_DEM6_049, TX_DEM6_050,
	TX_DEM6_051, TX_DEM6_052, TX_DEM6_053, TX_DEM6N000, TX_DEM6N001,
	TX_DEM6N002, TX_DEM6N003, TX_DEM6N004, TX_DEM6N005, TX_DEM6N006,
	TX_DEM6N007, TX_DEM6N008, TX_DEM6N009, TX_DEM6N010, TX_DEM6N011,
	TX_DEM6N012, TX_DEM6N013, TX_DEM6N014, TX_DEM6N015, TX_DEM6N016,
	TX_DEM6N017, TX_DEM6N018, TX_DEM6N019, TX_DEM6N020, TX_DEM6N021,
	TX_DEM6N022, TX_DEM6N023, TX_DEM6N024, TX_DIALOG_ERROR
};

extern const RoomTextOffsets demon6TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 4150, 3852, 4281, 4242, 3857, 3948, 3983 },
	{ TX_SPEAKER_MCCOY, 4161, 3863, 4292, 4253, 3868, 3961, 3994 },
	{ TX_SPEAKER_SPOCK, 4171, 3873, 4302, 4263, 3878, 3971, 4004 },
	{ TX_SPEAKER_EVERTS, 4181, 3883, 4312, 4273, 3888, 3981, 4014 },
	{ TX_SPEAKER_STEPHEN, 4195, 3897, 4326, 4287, 3902, 3995, 4030 },
	{ TX_DEM6_001, 6283, 5625, 6560, 6463, 5629, 5821, 5898 },
	{ TX_DEM6_002, 9373, 8310, 10069, 9712, 8318, 8685, 8980 },
	{ TX_DEM6_003, 6387, 5699, 6681, 6565, 5703, 5893, 5989 },
	{ TX_DEM6_004, 2427, 2254, 2495, 2460, 2254, 2286, 2322 },
	{ TX_DEM6_005, 1927, 1769, 1985, 1944, 1769, 1785, 1827 },
	{ TX_DEM6_006, 6339, 5666, 6634, 6509, 5670, 5852, 5957 },
	{ TX_DEM6_007, 3239, 2991, 3340, 3320, 2991, 3071, 3090 },
	{ TX_DEM6_008, 3668, 3405, 3784, 3755, 3405, 3491, 3518 },
	{ TX_DEM6_009, 3057, 2824, 3150, 3117, 2824, 2883, 2916 },
	{ TX_DEM6_010, 10137, 8969, 10951, 10502, 8977, 9374, 9756 },
	{ TX_DEM6_011, 10040, 8887, 10851, 10401, 8895, 9288, 9671 },
	{ TX_DEM6_012, 9812, 8674, 10586, 10209, 8682, 9111, 9422 },
	{ TX_DEM6_013, 9601, 8493, 10347, 9973, 8501, 8905, 9213 },
	{ TX_DEM6_014, 8244, 7301, 8758, 8486, 7309, 7576, 7799 },
	{ TX_DEM6_015, 2549, 2361, 2623, 2599, 2361, 2410, 2435 },
	{ TX_DEM6_016, 2942, 2724, 3031, 3003, 2724, 2784, 2813 },
	{ TX_DEM6_017, 715, 674, 746, 723, 674, 682, 705 },
	{ TX_DEM6_018, 1765, 1622, 1817, 1780, 1622, 1636, 1674 },
	{ TX_DEM6_019, 6103, 5490, 6369, 6273, 5494, 5677, 5752 },
	{ TX_DEM6_020, 8973, 7940, 9631, 9299, 7948, 8301, 8576 },
	{ TX_DEM6_021, 9718, 8595, 10482, 10112, 8603, 9029, 9333 },
	{ TX_DEM6_022, 5898, 5330, 6168, 6044, 5334, 5493, 5596 },
	{ TX_DEM6_023, 10197, 9014, 11017, 10561, 9022, 9418, 9807 },
	{ TX_DEM6_024, 5812, 5259, 6085, 5939, 5263, 5403, 5528 },
	{ TX_DEM6_025, 9188, 8140, 9874, 9511, 8148, 8498, 8802 },
	{ TX_DEM6_026, 1485, 1384, 1528, 1484, 1384, 1382, 1427 },
	{ TX_DEM6_027, 1322, 1236, 1372, 1334, 1236, 1247, 1286 },
	{ TX_DEM6_028, 613, 587, 613, 613, 587, 587, 587 },
	{ TX_DEM6_029, 8443, 7470, 8986, 8715, 7478, 7775, 7997 },
	{ TX_DEM6_030, 6431, 5728, 6725, 6608, 5736, 5921, 6018 },
	{ TX_DEM6_031, 6524, 5806, 6837, 6727, 5814, 6025, 6115 },
	{ TX_DEM6_032, 8881, 7863, 9514, 9201, 7871, 8217, 8475 },
	{ TX_DEM6_033, 7758, 6830, 8236, 7957, 6838, 7081, 7293 },
	{ TX_DEM6_034, 6613, 5880, 6935, 6837, 5888, 6120, 6197 },
	{ TX_DEM6_035, 9475, 8382, 10206, 9834, 8390, 8777, 9087 },
	{ TX_DEM6_036, 7210, 6327, 7575, 7435, 6335, 6604, 6679 },
	{ TX_DEM6_037, 8785, 7782, 9398, 9102, 7790, 8133, 8375 },
	{ TX_DEM6_038, 7315, 6417, 7691, 7544, 6425, 6698, 6780 },
	{ TX_DEM6_039, 6930, 6077, 7278, 7151, 6085, 6349, 6415 },
	{ TX_DEM6_040, 7122, 6254, 7476, 7357, 6262, 6541, 6596 },
	{ TX_DEM6_041, 7566, 6653, 8005, 7755, 6661, 6894, 7079 },
	{ TX_DEM6_042, 4216, 3918, 4347, 4307, 3923, 4015, 4049 },
	{ TX_DEM6_043, 8493, 7505, 9048, 8786, 7513, 7831, 8044 },
	{ TX_DEM6_044, 6213, 5570, 6497, 6385, 5574, 5759, 5850 },
	{ TX_DEM6_045, 6701, 5938, 7032, 6931, 5946, 6184, 6264 },
	{ TX_DEM6_046, 6735, 5957, 7072, 6958, 5965, 6200, 6288 },
	{ TX_DEM6_047, 6761, 5968, 7102, 6983, 5976, 6216, 6302 },
	{ TX_DEM6_048, 6791, 5983, 7131, 7016, 5991, 6238, 6315 },
	{ TX_DEM6_049, 6837, 6014, 7179, 7061, 6022, 6284, 6347 },
	{ TX_DEM6_050, 6868, 6030, 7216, 7101, 6038, 6314, 6368 },
	{ TX_DEM6_051, 6667, 5919, 6989, 6888, 5927, 6156, 6236 },
	{ TX_DEM6_052, 2655, 2452, 2751, 2716, 2452, 2512, 2548 },
	{ TX_DEM6_053, 9413, 8335, 10122, 9754, 8343, 8712, 9018 },
	{ TX_DEM6N000, 5105, 4687, 5366, 5234, 4692, 4831, 4944 },
	{ TX_DEM6N001, 4691, 4348, 4910, 4784, 4353, 4448, 4563 },
	{ TX_DEM6N002, 5174, 4741, 5436, 5303, 4746, 4889, 4999 },
	{ TX_DEM6N003, 4592, 4264, 4762, 4708, 4269, 4387, 4430 },
	{ TX_DEM6N004, 5252, 4804, 5499, 5391, 4809, 4961, 5047 },
	{ TX_DEM6N005, 5058, 4655, 5316, 5182, 4660, 4794, 4909 },
	{ TX_DEM6N006, 4498, 4185, 4677, 4613, 4190, 4307, 4361 },
	{ TX_DEM6N007, 5323, 4860, 5572, 5462, 4865, 5017, 5105 },
	{ TX_DEM6N008, 5424, 4946, 5675, 5532, 4950, 5071, 5193 },
	{ TX_DEM6N009, 5594, 5086, 5865, 5701, 5090, 5210, 5353 },
	{ TX_DEM6N010, 5662, 5139, 5937, 5786, 5143, 5280, 5410 },
	{ TX_DEM6N011, 4909, 4536, 5143, 5031, 4541, 4673, 4766 },
	{ TX_DEM6N012, 6074, 5476, 6341, 6236, 5480, 5655, 5739 },
	{ TX_DEM6N013, 6188, 5560, 6470, 6354, 5564, 5743, 5838 },
	{ TX_DEM6N014, 10359, 9161, 11172, 10740, 9169, 9576, 9947 },
	{ TX_DEM6N015, 5501, 5008, 5768, 5616, 5012, 5140, 5271 },
	{ TX_DEM6N016, 4871, 4513, 5097, 5002, 4518, 4659, 4735 },
	{ TX_DEM6N017, 4960, 4572, 5208, 5088, 4577, 4715, 4816 },
	{ TX_DEM6N018, 6006, 5423, 6281, 6168, 5427, 5602, 5694 },
	{ TX_DEM6N019, 8372, 7414, 8908, 8624, 7422, 7699, 7934 },
	{ TX_DEM6N020, 5750, 5212, 6016, 5868, 5216, 5347, 5474 },
	{ TX_DEM6N021, 1139, 1068, 1178, 1144, 1068, 1073, 1107 },
	{ TX_DEM6N022, 3812, 3529, 3946, 3897, 3534, 3618, 3663 },
	{ TX_DEM6N023, 1687, 1559, 1739, 1698, 1559, 1569, 1611 },
	{ TX_DEM6N024, 1036, 980, 1060, 1045, 980, 989, 1004 },
	{ TX_DIALOG_ERROR, 8214, 7271, 8728, 0, 7279, 7546, 7769 },
	{          -1, 0,    0, 0, 0, 0, 0, 0 }
};

extern const RoomText demon6Texts[] = {
	{ -1, Common::UNK_LANG, "" }
};

void Room::demon6Tick1() {
	playVoc("DEM6LOOP");
	if (_awayMission->demon.gotBerries || (!_awayMission->demon.field3e && _awayMission->demon.field37)) {
		loadActorAnim(8, "oldman", 0x5f, 0xb1, 0);
		_roomVar.demon.stephenInRoom = true;
	}
	loadActorAnim(10, "s0r6d2", 0xa0, 0x92, 0);
	loadActorAnim(9, "scrnan", 0, 0xc7, 0);
}

void Room::demon6Tick30() {
	if (!_awayMission->demon.gotBerries)
		return;
	if (_awayMission->demon.field3e)
		return;
	if (!_awayMission->demon.field37)
		return;
	if (_awayMission->demon.stephenWelcomedToStudy)
		return;

	showText(TX_SPEAKER_STEPHEN, TX_DEM6_042);
	_awayMission->demon.stephenWelcomedToStudy = true;
}

void Room::demon6SpockReachedComputer() {
	showText(TX_SPEAKER_SPOCK, TX_DEM6_014);
}

void Room::demon6WalkToDoor() {
	_awayMission->disableInput = true;
	_roomVar.demon.movingToDoor = true;
	walkCrewman(OBJECT_KIRK, 0xa0, 0x94, 4);
}

void Room::demon6TouchedDoorOpenTrigger() {
	if (!_roomVar.demon.movingToDoor)
		return;
	loadActorAnim(10, "s0r6d1", 0xa0, 0x92, 2);
	playSoundEffectIndex(kSfxDoor);
}

void Room::demon6DoorOpenedOrReachedDoor() {
	_roomVar.demon.doorCounter++;
	if (_roomVar.demon.doorCounter == 2)
		loadRoomIndex(0, 2);
}

void Room::demon6UsePhaserOnStephen() {
	showText(TX_SPEAKER_STEPHEN, TX_DEM6_028);
}

void Room::demon6UsePhaserOnCase() {
	showText(TX_SPEAKER_MCCOY, TX_DEM6_017);
}

void Room::demon6LookAtWorkspace() {
	showDescription(TX_DEM6N003);
}

void Room::demon6LookAtCase() {
	showDescription(TX_DEM6N001);
}

void Room::demon6LookAtTable() {
	showDescription(TX_DEM6N007);
}

void Room::demon6LookAtMineral() {
	showDescription(TX_DEM6N016);
}

void Room::demon6LookAtShells() {
	showDescription(TX_DEM6N011);
}

void Room::demon6LookAtSkull() {
	showDescription(TX_DEM6N017);
}

void Room::demon6LookAtMetal() {
	showDescription(TX_DEM6N005);
}

void Room::demon6LookAtMeteor() {
	showDescription(TX_DEM6N000);
}

void Room::demon6LookAtMountain() {
	showDescription(TX_DEM6N002);
}

void Room::demon6LookAtSynthesizer() {
	showDescription(TX_DEM6N004);
}

void Room::demon6LookAtKirk() {
	showDescription(TX_DEM6N008);
}

void Room::demon6LookAtSpock() {
	showDescription(TX_DEM6N015);
}

void Room::demon6LookAtMccoy() {
	showDescription(TX_DEM6N009);
}

void Room::demon6LookAtRedshirt() {
	showDescription(TX_DEM6N010);
}

void Room::demon6LookAtComputer() {
	showDescription(TX_DEM6N006);
	if (!_awayMission->demon.lookedAtComputer) {
		_awayMission->demon.lookedAtComputer = true;
		_awayMission->demon.missionScore++;
	}
}

void Room::demon6LookAnywhere() {
	showDescription(TX_DEM6N024);
}

void Room::demon6LookAtStephen() {
	showDescription(TX_DEM6N021);
}

void Room::demon6TalkToMccoy() {
	showText(TX_SPEAKER_MCCOY, TX_DEM6_020);
}

void Room::demon6TalkToSpock() {
	showText(TX_SPEAKER_SPOCK, TX_DEM6_025);
}

void Room::demon6TalkToKirk() {
	showText(TX_SPEAKER_KIRK, TX_DEM6_002);
}

void Room::demon6TalkToRedshirt() {
	showText(TX_SPEAKER_EVERTS, TX_DEM6_053);
}

void Room::demon6TalkToStephen() {
	showText(TX_SPEAKER_STEPHEN, TX_DEM6_035);
}

void Room::demon6UseBerryOnStephen() {
	if (_awayMission->demon.knowAboutHypoDytoxin) {
		showText(TX_SPEAKER_STEPHEN, TX_DEM6_027);
	} else {
		showText(TX_SPEAKER_STEPHEN, TX_DEM6_026);
	}
}

void Room::demon6UseBerryOnSynthesizer() {
	if (!_awayMission->demon.knowAboutHypoDytoxin)
		return;
	walkCrewman(OBJECT_MCCOY, 0x104, 0xa5, 1);
}

void Room::demon6MccoyReachedSynthesizer() {
	loadActorAnim2(OBJECT_MCCOY, "musemn", -1, -1, 1);
	playVoc("LD1REPLI");
}

void Room::demon6FinishedMakingHypoDytoxin() {
	playMidiMusicTracks(MIDITRACK_NONE);
	showDescription(TX_DEM6N023);
	showText(TX_SPEAKER_MCCOY, TX_DEM6_018);

	giveItem(OBJECT_IDETOXIN);
	_awayMission->demon.madeHypoDytoxin = true;
	loseItem(OBJECT_IBERRY);

	if (!_awayMission->demon.gavePointsForDytoxin) {
		_awayMission->demon.gavePointsForDytoxin = true;
		_awayMission->demon.missionScore++;
	}
}

void Room::demon6UseHandOnWorkspace() {
	if (_awayMission->demon.repairedHand) {
		showText(TX_SPEAKER_SPOCK, TX_DEM6N018); // Spock speaks a narration text, as in the original
	} else {
		_awayMission->disableInput = true;
		showText(TX_SPEAKER_KIRK, TX_DEM6_005);
		walkCrewman(OBJECT_SPOCK, 0xb3, 0xbb, 2);
	}
}

void Room::demon6SpockReachedWorkspace() {
	loadActorAnim(OBJECT_SPOCK, "spokop", 0xb3, 0xbb, 3);
}

void Room::demon6SpockFinishedRepairingHand() {
	showText(TX_SPEAKER_SPOCK, TX_DEM6_024);
	_awayMission->demon.repairedHand = true;
	_awayMission->demon.missionScore += 2;
	loadActorStandAnim(OBJECT_SPOCK);
	_awayMission->disableInput = false;
}

// Like in the original, this isn't reached for crewmen and for items with a default
// behavior (tricorders, phasers, medkit, communicator)
void Room::demon6UseAnythingOnWorkspace() {
	showDescription(TX_DEM6N020);
}

void Room::demon6StephenIsInsulted() {
	showText(TX_SPEAKER_STEPHEN, TX_DEM6_030);
	_roomVar.demon.insultedStephenRecently = true;
	_awayMission->demon.insultedStephen = true;
}

void Room::demon6StephenDescribesItemsInCase() {
	while (true) {
		showText(TX_SPEAKER_STEPHEN, TX_DEM6_051);
		TextRef choices2[] = { TX_SPEAKER_STEPHEN, TX_DEM6_045, TX_DEM6_046, TX_DEM6_047, TX_DEM6_048, TX_DEM6_049, TX_DEM6_050, TX_END };
		TextRef choice = showMultipleTexts(choices2);

		switch (choice) {
		case 0:
			showText(TX_SPEAKER_STEPHEN, TX_DEM6_040);
			break;
		case 1:
			showText(TX_SPEAKER_STEPHEN, TX_DEM6_033);
			break;
		case 2:
			showText(TX_SPEAKER_STEPHEN, TX_DEM6_036);
			break;
		case 3:
			showText(TX_SPEAKER_STEPHEN, TX_DEM6_038);
			break;
		case 4:
			showText(TX_SPEAKER_STEPHEN, TX_DEM6_039);
			break;
		case 5:
			showText(TX_SPEAKER_STEPHEN, TX_DEM6_041);
			_roomVar.demon.caseOpened = true;
			return;
		default:
			showDescription(TX_DIALOG_ERROR);
		}
	}
}

void Room::demon6UseCrewmanOnCase() {
	if (_roomVar.demon.stephenInRoom) {
		if (_roomVar.demon.insultedStephenRecently)
			return;
		showText(TX_SPEAKER_STEPHEN, TX_DEM6_044);
		TextRef choices1[] = {TX_SPEAKER_KIRK, TX_DEM6_001, TX_DEM6_006, TX_DEM6_003, TX_END};
		TextRef choice = showMultipleTexts(choices1);

		if (choice == 0) {
			demon6StephenIsInsulted();
		} else if (choice == 1) {
			showText(TX_SPEAKER_STEPHEN, TX_DEM6_034);
			demon6StephenDescribesItemsInCase();
		} else if (choice == 2) {
			showText(TX_SPEAKER_STEPHEN, TX_DEM6_031);
			TextRef choices3[] = {TX_SPEAKER_KIRK, TX_DEM6_001, TX_DEM6_006, TX_END};
			choice = showMultipleTexts(choices3);

			if (choice == 0)
				demon6StephenIsInsulted();
			else if (choice == 1)
				demon6StephenDescribesItemsInCase();
			else
				showDescription(TX_DIALOG_ERROR);
		} else {
			showDescription(TX_DIALOG_ERROR);
		}
	} else {
		showDescription(TX_DEM6N012);
		showText(TX_SPEAKER_MCCOY, TX_DEM6_019);
	}
}

void Room::demon6UseKirkOnComputer() {
	showText(TX_SPEAKER_KIRK, TX_DEM6_004);
	demon6UseSpockOnComputer();
}

void Room::demon6UseMccoyOnComputer() {
	showText(TX_SPEAKER_MCCOY, TX_DEM6_015);
}

void Room::demon6UseRedshirtOnComputer() {
	showText(TX_SPEAKER_EVERTS, TX_DEM6_052);
}

void Room::demon6UseSpockOnComputer() {
	walkCrewman(OBJECT_SPOCK, 0x3d, 0xc5, 3);
}

void Room::demon6UseMineralOnStephen() {
	_awayMission->demon.itemsTakenFromCase &= ~8;
	demon6ReturnItemToStephen(OBJECT_IMINERAL);
}

void Room::demon6UseShellsOnStephen() {
	_awayMission->demon.itemsTakenFromCase &= ~2;
	demon6ReturnItemToStephen(OBJECT_ISHELLS);
}

void Room::demon6UseMeteorOnStephen() {
	_awayMission->demon.itemsTakenFromCase &= ~4;
	demon6ReturnItemToStephen(OBJECT_IMETEOR);
}

void Room::demon6UseSkullOnStephen() {
	_awayMission->demon.itemsTakenFromCase &= ~16;
	demon6ReturnItemToStephen(OBJECT_ISKULL);
}

void Room::demon6UseMetalOnStephen() {
	_awayMission->demon.itemsTakenFromCase &= ~1;
	demon6ReturnItemToStephen(OBJECT_IMETAL);
}

void Room::demon6ReturnItemToStephen(int item) {
	loseItem(item);
	showDescription(TX_DEM6N019);
	showText(TX_SPEAKER_STEPHEN, TX_DEM6_029);
}

void Room::demon6UseHandOnStephen() {
	if (_awayMission->demon.repairedHand)
		showText(TX_SPEAKER_STEPHEN, TX_DEM6_037);
	else
		showText(TX_SPEAKER_STEPHEN, TX_DEM6_043);
}

void Room::demon6UseMTricoderOnStephen() {
	loadActorAnim2(OBJECT_MCCOY, "mscans", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_MCCOY, TX_DEM6_016);
}

void Room::demon6UseSTricoderOnTable() {
	loadActorAnim2(OBJECT_SPOCK, "sscans", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM6_009);
}

void Room::demon6UseSTricoderOnComputer() {
	loadActorAnim2(OBJECT_SPOCK, "sscans", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM6_007);
}

void Room::demon6UseSTricoderOnMineral() {
	loadActorAnim2(OBJECT_SPOCK, "sscans", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM6_013);
}

void Room::demon6UseSTricoderOnShells() {
	loadActorAnim2(OBJECT_SPOCK, "sscans", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM6_021);
}

void Room::demon6UseSTricoderOnSkull() {
	loadActorAnim2(OBJECT_SPOCK, "sscans", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM6_012);
}

void Room::demon6UseSTricoderOnMetal() {
	loadActorAnim2(OBJECT_SPOCK, "sscans", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM6_011);
}

void Room::demon6UseSTricoderOnMeteor() {
	loadActorAnim2(OBJECT_SPOCK, "sscans", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM6_010);
}

void Room::demon6UseSTricoderOnCase() {
	loadActorAnim2(OBJECT_SPOCK, "sscans", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM6_023);
}

void Room::demon6UseSTricoderOnSynthesizer() {
	loadActorAnim2(OBJECT_SPOCK, "sscans", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM6_008);
}

void Room::demon6GetCase() {
	if (!_roomVar.demon.caseOpened)
		showDescription(TX_DEM6N022);
	else
		walkCrewman(OBJECT_KIRK, 0xff, 0xba, 5);
}

void Room::demon6KirkReachedCase() {
	const int items[] = { OBJECT_IMETAL, OBJECT_ISHELLS, OBJECT_IMETEOR, OBJECT_IMINERAL, OBJECT_ISKULL };
	byte ret = 0x1f ^ demon6ShowCase(_awayMission->demon.itemsTakenFromCase ^ 0x1f);
	_awayMission->demon.itemsTakenFromCase = ret;

	const int *i = items;
	while (ret != 0) {
		if (ret & 1)
			giveItem(*i);
		i++;
		ret >>= 1;
	}
}

int Room::demon6ShowCase(int visible) {
	const Common::Point spritePositions[] = {
		Common::Point(0x29, 0x80),
		Common::Point(0x58, 0x7d),
		Common::Point(0x82, 0x7d),
		Common::Point(0xae, 0x79),
		Common::Point(0xe1, 0x7d)
	};

	_vm->_gfx->fadeoutScreen();
	_vm->_gfx->pushSprites();

	_vm->_gfx->setBackgroundImage("stlcase");
	_vm->_gfx->copyBackgroundScreen();
	_vm->_gfx->clearPri();

	Sprite sprites[5];

	for (int i = 0; i < 5; i++) {
		if (!(visible & (0x10 >> i)))
			continue;
		_vm->_gfx->addSprite(&sprites[i]);
		sprites[i].pos.x = spritePositions[i].x;
		sprites[i].pos.y = spritePositions[i].y;
		sprites[i].drawPriority = 2;
		sprites[i].bitmapChanged = true;
		sprites[i].setBitmap(loadBitmapFile(Common::String::format("stlitem%d", i)));
	}

	Sprite buttonSprite;

	// BUGFIX: use draw mode 2 so the entire button is clickable (not just non-transparent parts)
	buttonSprite.drawMode = 2;

	buttonSprite.pos.x = 0xe1;
	buttonSprite.pos.y = 0x19;
	buttonSprite.drawPriority = 2;
	buttonSprite.bitmapChanged = true;
	buttonSprite.setBitmap(loadBitmapFile("donebutt"));

	_vm->_gfx->addSprite(&buttonSprite);
	_vm->_gfx->forceDrawAllSprites();
	_vm->_gfx->fadeinScreen();

	visible = demon6ShowCaseProcessInput(sprites, &buttonSprite, visible);

	_vm->_gfx->fadeoutScreen();
	_vm->_gfx->popSprites();
	_vm->_gfx->loadPri(_vm->getScreenName());
	_vm->_gfx->setBackgroundImage(_vm->getScreenName());
	_vm->_gfx->copyBackgroundScreen();
	_vm->_gfx->forceDrawAllSprites();

	return visible;
}

int Room::demon6ShowCaseProcessInput(Sprite *sprites, Sprite *buttonSprite, int visible) {
	bool exitLoop = false;

	while (!exitLoop) {
		TrekEvent event;
		if (!_vm->popNextEvent(&event))
			continue;

		switch (event.type) {
		case TREKEVENT_TICK:
			_vm->_gfx->drawAllSprites();
			break;

		case TREKEVENT_LBUTTONDOWN: {
			Sprite *clickedSprite = _vm->_gfx->getSpriteAt(_vm->_gfx->getMousePos());
			if (clickedSprite == buttonSprite)
				exitLoop = true;
			else if (clickedSprite != nullptr)
				visible = demon6ShowCaseProcessSelection(sprites, clickedSprite, visible);
			break;
		}

		case TREKEVENT_RBUTTONDOWN:
			exitLoop = true;
			break;

		case TREKEVENT_KEYDOWN:
			switch (event.kbd.keycode) {
			case Common::KEYCODE_ESCAPE:
				exitLoop = true;
				break;

			case Common::KEYCODE_RETURN:
			case Common::KEYCODE_KP_ENTER:
			case Common::KEYCODE_F1: {
				Sprite *clickedSprite = _vm->_gfx->getSpriteAt(_vm->_gfx->getMousePos());
				if (clickedSprite == buttonSprite)
					exitLoop = true;
				else if (clickedSprite != nullptr)
					visible = demon6ShowCaseProcessSelection(sprites, clickedSprite, visible);
				break;
			}

			case Common::KEYCODE_F2:
				exitLoop = true;
				break;

			default:
				break;
			}
			break;

		default:
			break;
		}
	}

	return visible;
}

int Room::demon6ShowCaseProcessSelection(Sprite *sprites, Sprite *clickedSprite, int visible) {
	while (clickedSprite->pos.y < SCREEN_HEIGHT) { // Move the selected item down and off the screen.
		clickedSprite->drawPriority = 8;
		clickedSprite->bitmapChanged = true;
		_vm->_gfx->drawAllSprites();

		// WORKAROUND: original game had no bound on how fast the items move
		// off the screen. Here I bind it to the tick event.
		// (This was probably the intended behaviour since the original game
		// does check the clock cycle, but doesn't do a proper comparison with
		// it.)
		TrekEvent event;
		do {
			if (!_vm->popNextEvent(&event))
				continue;
		} while (event.type != TREKEVENT_TICK);
		clickedSprite->pos.y++;
	}

	clickedSprite->dontDrawNextFrame();
	_vm->_gfx->drawAllSprites();
	_vm->_gfx->delSprite(clickedSprite);
	delete clickedSprite->bitmap;
	clickedSprite->bitmap = nullptr;
	visible ^= (0x10 >> (clickedSprite - sprites));

	return visible;
}

}
