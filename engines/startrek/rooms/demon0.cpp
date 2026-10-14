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

extern const RoomAction demon0ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::demon0Tick1 },
	{ {ACTION_TICK, 2, 0, 0, 0}, &Room::demon0Tick2 },
	{ {ACTION_TICK, 60, 0, 0, 0}, &Room::demon0Tick60 },
	{ {ACTION_TICK, 100, 0, 0, 0}, &Room::demon0Tick100 },
	{ {ACTION_TICK, 140, 0, 0, 0}, &Room::demon0Tick140 },

	{ {ACTION_TOUCHED_WARP, 0, 0, 0, 0}, &Room::demon0TouchedWarp0 },

	{ {ACTION_WALK, 0x27, 0, 0, 0}, &Room::demon0WalkToBottomDoor },
	{ {ACTION_WALK, 9, 0, 0, 0}, &Room::demon0WalkToBottomDoor },
	{ {ACTION_TOUCHED_HOTSPOT, 1, 0, 0, 0}, &Room::demon0TouchedHotspot1 },
	{ {ACTION_FINISHED_WALKING, 1, 0, 0, 0}, &Room::demon0ReachedBottomDoor },
	{ {ACTION_FINISHED_ANIMATION, 1, 0, 0, 0}, &Room::demon0ReachedBottomDoor },

	{ {ACTION_WALK, 0x26, 0, 0, 0}, &Room::demon0WalkToTopDoor },
	{ {ACTION_WALK, 10, 0, 0, 0}, &Room::demon0WalkToTopDoor },
	{ {ACTION_TOUCHED_HOTSPOT, 0, 0, 0, 0}, &Room::demon0TouchedHotspot0 },
	{ {ACTION_FINISHED_WALKING, 2, 0, 0, 0}, &Room::demon0ReachedTopDoor },
	{ {ACTION_FINISHED_ANIMATION, 2, 0, 0, 0}, &Room::demon0ReachedTopDoor },

	{ {ACTION_TALK, 8, 0, 0, 0}, &Room::demon0TalkToPrelate },
	{ {ACTION_LOOK, 8, 0, 0, 0}, &Room::demon0LookAtPrelate },

	{ {ACTION_USE, OBJECT_IPHASERS, 0x23, 0, 0}, &Room::demon0UsePhaserOnSnow },
	{ {ACTION_USE, OBJECT_IPHASERK, 0x23, 0, 0}, &Room::demon0UsePhaserOnSnow },
	{ {ACTION_USE, OBJECT_IPHASERS, 0x25, 0, 0}, &Room::demon0UsePhaserOnSign },
	{ {ACTION_USE, OBJECT_IPHASERK, 0x25, 0, 0}, &Room::demon0UsePhaserOnSign },
	{ {ACTION_USE, OBJECT_IPHASERK, 0x20, 0, 0}, &Room::demon0UsePhaserOnShelter },
	{ {ACTION_USE, OBJECT_IPHASERK, 8, 0, 0}, &Room::demon0UsePhaserOnPrelate },

	{ {ACTION_LOOK, 0x25, 0, 0, 0}, &Room::demon0LookAtSign },
	{ {ACTION_LOOK, 0x21, 0, 0, 0}, &Room::demon0LookAtTrees },
	{ {ACTION_LOOK, 0x22, 0, 0, 0}, &Room::demon0LookAtTrees },
	{ {ACTION_LOOK, 0x23, 0, 0, 0}, &Room::demon0LookAtSnow },
	{ {ACTION_LOOK, 0xff, 0, 0, 0}, &Room::demon0LookAnywhere },
	{ {ACTION_LOOK, 0x24, 0, 0, 0}, &Room::demon0LookAtBushes },

	{ {ACTION_LOOK, OBJECT_KIRK,     0, 0, 0}, &Room::demon0LookAtKirk },
	{ {ACTION_LOOK, OBJECT_MCCOY,    0, 0, 0}, &Room::demon0LookAtMcCoy },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::demon0LookAtRedShirt },
	{ {ACTION_LOOK, OBJECT_SPOCK,    0, 0, 0}, &Room::demon0LookAtSpock },

	{ {ACTION_LOOK, 9, 0, 0, 0}, &Room::demon0LookAtShelter },    // Door 1
	{ {ACTION_LOOK, 10, 0, 0, 0}, &Room::demon0LookAtShelter },   // Door 2
	{ {ACTION_LOOK, 0x20, 0, 0, 0}, &Room::demon0LookAtShelter }, // Shelter itself

	{ {ACTION_TALK, OBJECT_KIRK,     0, 0, 0}, &Room::demon0TalkToKirk },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::demon0TalkToRedshirt },
	{ {ACTION_TALK, OBJECT_MCCOY,    0, 0, 0}, &Room::demon0TalkToMcCoy },
	{ {ACTION_TALK, OBJECT_SPOCK,    0, 0, 0}, &Room::demon0TalkToSpock },

	{ {ACTION_USE, OBJECT_ISTRICOR, 0xff, 0, 0}, &Room::demon0UseSTricorderAnywhere },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0xff, 0, 0}, &Room::demon0UseMTricorderAnywhere },
	{ {ACTION_USE, OBJECT_IMTRICOR, 8, 0, 0}, &Room::demon0UseMTricorderOnPrelate },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum demon0TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_ANGIVEN, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_EVERTS,
	TX_SPEAKER_SIGN,
	TX_DEM0_001,	TX_DEM0_002,	TX_DEM0_003,	TX_DEM0_004,	TX_DEM0_005,
	TX_DEM0_006,	TX_DEM0_007,	TX_DEM0_008,	TX_DEM0_009,	TX_DEM0_010,
	TX_DEM0_011,	TX_DEM0_012,	TX_DEM0_013,	TX_DEM0_014,	TX_DEM0_015,
	TX_DEM0_016,	TX_DEM0_017,	TX_DEM0_018,	TX_DEM0_019,	TX_DEM0_020,
	TX_DEM0_021,	TX_DEM0_022,	TX_DEM0_023,	TX_DEM0_024,	TX_DEM0_025,
	TX_DEM0_026,	TX_DEM0_027,	TX_DEM0_028,	TX_DEM0_029,	TX_DEM0_030,
	TX_DEM0_031,	TX_DEM0_032,	TX_DEM0_033,	TX_DEM0_034,	TX_DEM0_035,
	TX_DEM0_036,	TX_DEM0_037,	TX_DEM0_038,	TX_DEM0_039,	TX_DEM0_040,
	TX_DEM0_041,	TX_DEM0_042,	TX_DEM0_043,	TX_DEM0_044,	TX_DEM0N000,
	TX_DEM0N001,	TX_DEM0N002,	TX_DEM0N003,	TX_DEM0N004,	TX_DEM0N005,
	TX_DEM0N006,	TX_DEM0N007,	TX_DEM0N008,	TX_DEM0N009,	TX_DEM0N010,
	TX_DEM0C001
};

extern const RoomTextOffsets demon0TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 2597, 2478, 2676, 2679, 2492, 2568, 2570, 2492, 2587 },
	{ TX_SPEAKER_ANGIVEN, 2608, 2489, 2687, 2690, 2503, 2582, 2582, 2503, 2598 },
	{ TX_SPEAKER_MCCOY, 2622, 2503, 2701, 2704, 2517, 2597, 2596, 2517, 2612 },
	{ TX_SPEAKER_SPOCK, 2632, 2513, 2711, 2714, 2527, 2607, 2606, 2527, 2622 },
	{ TX_SPEAKER_EVERTS, 2642, 2523, 2721, 2724, 2537, 2617, 2616, 2537, 2632 },
	{ TX_SPEAKER_SIGN, 2661, 2542, 2740, 2743, 2556, 2636, 2637, 2556, 2651 },
	{ TX_DEM0_001, 7829, 7082, 8665, 8597, 7097, 7862, 7918, 7097, 7819 },
	{ TX_DEM0_002, 3960, 3737, 4251, 4192, 3752, 3983, 4042, 3752, 3950 },
	{ TX_DEM0_003, 3148, 2985, 3294, 3303, 2999, 3153, 3146, 2999, 3138 },
	{ TX_DEM0_004, 7588, 6886, 8404, 8335, 6901, 7624, 7702, 6901, 7578 },
	{ TX_DEM0_005, 4061, 3823, 4368, 4323, 3838, 4099, 4144, 3838, 4051 },
	{ TX_DEM0_006, 2797, 2663, 2884, 2901, 2677, 2779, 2766, 2677, 2787 },
	{ TX_DEM0_007, 7380, 6738, 8160, 8101, 6753, 7449, 7518, 6753, 7370 },
	{ TX_DEM0_008, 2988, 2840, 3110, 3097, 2854, 2967, 2977, 2854, 2978 },
	{ TX_DEM0_009, 7460, 6788, 8269, 8199, 6803, 7518, 7597, 6803, 7450 },
	{ TX_DEM0_010, 4303, 4050, 4660, 4642, 4065, 4388, 4407, 4065, 4293 },
	{ TX_DEM0_011, 6827, 6301, 7524, 7474, 6316, 6940, 7002, 6316, 6817 },
	{ TX_DEM0_012, 4362, 4094, 4727, 4705, 4109, 4436, 4459, 4109, 4352 },
	{ TX_DEM0_013, 1334, 1245, 1387, 1384, 1259, 1304, 1312, 1259, 1324 },
	{ TX_DEM0_014, 6593, 6082, 7251, 7205, 6097, 6686, 6744, 6097, 6583 },
	{ TX_DEM0_015, 1233, 1157, 1281, 1270, 1171, 1205, 1221, 1171, 1223 },
	{ TX_DEM0_016,  473, 444, 474, 474, 444, 447, 448, 444, 463 },
	{ TX_DEM0_017, 1167, 1106, 1211, 1193, 1119, 1143, 1166, 1119, 1157 },
	{ TX_DEM0_018, 8094, 7317, 8963, 8881, 7332, 8117, 8186, 7332, 8084 },
	{ TX_DEM0_019, 6427, 5931, 7069, 7006, 5946, 6501, 6577, 5946, 6417 },
	{ TX_DEM0_020, 8648, 7826, 9546, 9498, 7841, 8689, 8724, 7841, 8638 },
	{ TX_DEM0_021, 8180, 7388, 9050, 8958, 7403, 8179, 8258, 7403, 8170 },
	{ TX_DEM0_022, 8050, 7288, 8915, 8825, 7303, 8076, 8153, 7303, 8040 },
	{ TX_DEM0_023, 6871, 6330, 7572, 7516, 6345, 6967, 7035, 6345, 6861 },
	{ TX_DEM0_024, 7656, 6939, 8479, 8401, 6954, 7679, 7762, 6954, 7646 },
	{ TX_DEM0_025, 7357, 6726, 8132, 8075, 6741, 7441, 7505, 6741, 7347 },
	{ TX_DEM0_026, 7213, 6612, 7950, 7898, 6627, 7293, 7353, 6627, 7203 },
	{ TX_DEM0_027, 7526, 6839, 8343, 8274, 6854, 7578, 7656, 6854, 7516 },
	{ TX_DEM0_028, 8405, 7598, 9279, 9217, 7613, 8423, 8472, 7613, 8395 },
	{ TX_DEM0_029, 6995, 6439, 7705, 7650, 6454, 7088, 7153, 6454, 6985 },
	{ TX_DEM0_030, 7709, 6977, 8540, 8458, 6992, 7736, 7808, 6992, 7699 },
	{ TX_DEM0_031, 4125, 3887, 4463, 4428, 3902, 4188, 4225, 3902, 4115 },
	{ TX_DEM0_032, 3563, 3370, 3813, 3766, 3385, 3585, 3634, 3385, 3553 },
	{ TX_DEM0_033, 3743, 3535, 4020, 3966, 3550, 3772, 3826, 3550, 3733 },
	{ TX_DEM0_034, 5325, 5010, 5884, 5805, 5025, 5494, 5571, 5025, 5315 },
	{ TX_DEM0_035, 4533, 4248, 4938, 4928, 4263, 4644, 4655, 4263, 4523 },
	{ TX_DEM0_036, 2666, 2547, 2745, 2750, 2561, 2643, 2642, 2561, 2656 },
	{ TX_DEM0_037, 5196, 4896, 5733, 5645, 4911, 5349, 5435, 4911, 5186 },
	{ TX_DEM0_038, 3237, 3059, 3403, 3419, 3074, 3254, 3240, 3074, 3227 },
	{ TX_DEM0_039, 1058, 1014, 1078, 1064, 1020, 1028, 1043, 1020, 1048 },
	{ TX_DEM0_040, 1400, 1296, 1454, 1456, 1310, 1361, 1364, 1310, 1390 },
	{ TX_DEM0_041, 7080, 6509, 7791, 7750, 6524, 7173, 7224, 6524, 7070 },
	{ TX_DEM0_042, 7275, 6659, 8047, 7976, 6674, 7357, 7435, 6674, 7265 },
	{ TX_DEM0_043, 7144, 6558, 7869, 7816, 6573, 7225, 7287, 6573, 7134 },
	{ TX_DEM0_044, 7429, 6772, 8229, 8168, 6787, 7502, 7572, 6787, 7419 },
	{ TX_DEM0N000, 5591, 5231, 6161, 6095, 5246, 5730, 5804, 5246, 5581 },
	{ TX_DEM0N001, 5805, 5415, 6397, 6331, 5430, 5933, 6010, 5430, 5795 },
	{ TX_DEM0N002, 6184, 5719, 6808, 6755, 5734, 6279, 6346, 5734, 6174 },
	{ TX_DEM0N003, 6290, 5809, 6925, 6867, 5824, 6376, 6448, 5824, 6280 },
	{ TX_DEM0N004, 5467, 5137, 6031, 5963, 5152, 5628, 5704, 5152, 5457 },
	{ TX_DEM0N005, 6076, 5626, 6685, 6638, 5641, 6177, 6237, 5641, 6066 },
	{ TX_DEM0N006, 5678, 5303, 6251, 6186, 5318, 5805, 5879, 5318, 5668 },
	{ TX_DEM0N007, 5897, 5492, 6494, 6449, 5507, 6033, 6092, 5507, 5887 },
	{ TX_DEM0N008, 5560, 5215, 6134, 6059, 5230, 5709, 5792, 5230, 5550 },
	{ TX_DEM0N009, 6036, 5601, 6645, 6595, 5616, 6149, 6212, 5616, 6026 },
	{ TX_DEM0N010, 5960, 5540, 6571, 6515, 5555, 6084, 6154, 5555, 5950 },
	{ TX_DEM0C001, 1501, 1382, 1574, 1553, 1396, 1443, 1469, 1396, 1491 },
	{          -1, 0,    0, 0, 0, 0, 0, 0, 0, 0 }
};

extern const RoomText demon0Texts[] = {
	{ -1, Common::UNK_LANG, "" }
};

void Room::demon0Tick1() {
	playVoc("DEM0LOOP");
	loadActorAnim(9, "s0r0d3", 252, 153, 0);
	loadActorAnim(10, "s0r0d4", 158, 130, 0);
}

void Room::demon0Tick2() {
	if (_awayMission->demon.talkedToPrelate)
		return;
	loadActorAnim(8, "prel", 120, 190, 0);
}

void Room::demon0Tick60() {
	if (_awayMission->demon.talkedToPrelate)
		return;
	loadActorAnim2(8, "prelclap", 120, 190, 0);
}

void Room::demon0Tick100() {
	if (_awayMission->demon.talkedToPrelate || _awayMission->demon.prelateWelcomedCrew)
		return;
	_awayMission->demon.prelateWelcomedCrew = true;
	showText(TX_SPEAKER_ANGIVEN, TX_DEM0_036);
}

void Room::demon0Tick140() {
	if (_awayMission->demon.mccoyMentionedFlora)
		return;

	showText(TX_SPEAKER_MCCOY, TX_DEM0_016);

	_awayMission->demon.mccoyMentionedFlora = true;
}

void Room::demon0TouchedWarp0() {
	_awayMission->demon.enteredFrom = 1;
	_awayMission->rdfStillDoDefaultAction = true;
}

void Room::demon0WalkToBottomDoor() {
	_awayMission->disableInput = 1;
	_roomVar.demon.movingToBottomDoor = true;
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_E;
	walkCrewman(OBJECT_KIRK, 243, 158, 1);
}

void Room::demon0TouchedHotspot1() {
	if (!_roomVar.demon.movingToBottomDoor)
		return;
	loadActorAnim2(9, "s0r0d1", 254, 153, 1);
	playSoundEffectIndex(kSfxDoor);
}

void Room::demon0ReachedBottomDoor() {
	if (++_roomVar.demon.bottomDoorCounter != 2)
		return;
	loadRoomIndex(5, 0);
}

void Room::demon0WalkToTopDoor() {
	_awayMission->disableInput = 1;
	_roomVar.demon.movingToTopDoor = true;
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_E;
	walkCrewman(OBJECT_KIRK, 157, 134, 2);
}

void Room::demon0TouchedHotspot0() {
	if (!_roomVar.demon.movingToTopDoor)
		return;
	loadActorAnim2(10, "s0r0d2", 158, 130, 2);
	playSoundEffectIndex(kSfxDoor);
}

void Room::demon0ReachedTopDoor() {
	if (++_roomVar.demon.topDoorCounter != 2)
		return;
	loadRoomIndex(6, 3);
}

void Room::demon0TalkToPrelate() {
	const TextRef options1[] = {
	    TX_SPEAKER_KIRK,
	    TX_DEM0_006,
	    TX_DEM0_008,
	    TX_DEM0_003,
	    TX_END
	};
	const TextRef firstResponse0[] = {
	    TX_SPEAKER_ANGIVEN,
	    TX_DEM0_038,
	    TX_END
	};
	const TextRef firstResponse1[] = {
	    TX_SPEAKER_ANGIVEN,
	    TX_DEM0_032,
	    TX_END
	};
	const TextRef options2[] = {
	    TX_SPEAKER_KIRK,
	    TX_DEM0_002,
	    TX_DEM0_005,
	    TX_END
	};
	const TextRef secondResponse[] = {
	    TX_SPEAKER_ANGIVEN,
	    TX_DEM0_031,
	    TX_END
	};
	const TextRef options3[] = {
	    TX_SPEAKER_KIRK,
	    TX_DEM0_010,
	    TX_DEM0_012,
	    TX_END
	};
	const TextRef thirdResponse[] = {
	    TX_SPEAKER_ANGIVEN,
	    TX_DEM0_035,
	    TX_END
	};
	const TextRef badConclusion[] = {
	    TX_SPEAKER_ANGIVEN,
	    TX_DEM0_037,
	    TX_END
	};
	const TextRef goodConclusion[] = {
	    TX_SPEAKER_ANGIVEN,
	    TX_DEM0_034,
	    TX_END
	};

	if (_awayMission->demon.talkedToPrelate)
		return;

	_awayMission->demon.missionScore += 3;
	_awayMission->demon.talkedToPrelate = true;

	const TextRef *response = nullptr;

	switch (showMultipleTexts(options1)) {
	case 0:
		response = firstResponse0;
		break;
	case 1:
		response = firstResponse1;
		break;
	case 2:
		demon0BadResponse();
		break;
	default:
		break;
	}

	if (response != nullptr)
		showMultipleTexts(response);

	if (showMultipleTexts(options2) == 1)
		demon0BadResponse();

	showMultipleTexts(secondResponse);

	if (showMultipleTexts(options3) == 1)
		demon0BadResponse();

	showMultipleTexts(thirdResponse);

	if (_awayMission->demon.wasRudeToPrelate)
		showMultipleTexts(badConclusion);
	else
		showMultipleTexts(goodConclusion);
}

void Room::demon0LookAtPrelate() {
	showDescription(TX_DEM0N004);
}

void Room::demon0UsePhaserOnSnow() {
	// BUG: doesn't check if redshirt is dead.
	showText(TX_SPEAKER_EVERTS, TX_DEM0_039);
}

void Room::demon0UsePhaserOnSign() {
	showText(TX_SPEAKER_MCCOY, TX_DEM0_017);
}

void Room::demon0UsePhaserOnShelter() {
	showText(TX_SPEAKER_MCCOY, TX_DEM0_015);
	showText(TX_SPEAKER_SPOCK, TX_DEM0_013);
	if (!_awayMission->redshirtDead)
		showText(TX_SPEAKER_EVERTS, TX_DEM0_040);
}

void Room::demon0UsePhaserOnPrelate() {
	showText(TX_SPEAKER_MCCOY, TX_DEM0_020);
}

void Room::demon0LookAtSign() {
	showText(TX_SPEAKER_SIGN, TX_DEM0C001);
}

void Room::demon0LookAtTrees() {
	showDescription(TX_DEM0N006);
}

void Room::demon0LookAtSnow() {
	showDescription(TX_DEM0N007);
}

void Room::demon0LookAnywhere() {
	showDescription(TX_DEM0N000);
}

void Room::demon0LookAtBushes() {
	showDescription(TX_DEM0N010);
}

void Room::demon0LookAtKirk() {
	showDescription(TX_DEM0N005);
}

void Room::demon0LookAtMcCoy() {
	showDescription(TX_DEM0N002);
}

void Room::demon0LookAtRedShirt() {
	showDescription(TX_DEM0N003);
}

void Room::demon0LookAtSpock() {
	showDescription(TX_DEM0N009);
}

void Room::demon0LookAtShelter() {
	showDescription(TX_DEM0N001);
}

void Room::demon0TalkToKirk() {
	showText(TX_SPEAKER_KIRK, TX_DEM0_009);
	showText(TX_SPEAKER_MCCOY, TX_DEM0_027);
	showText(TX_SPEAKER_KIRK, TX_DEM0_004);
	showText(TX_SPEAKER_MCCOY, TX_DEM0_024);
}

void Room::demon0TalkToRedshirt() {
	showText(TX_SPEAKER_EVERTS, TX_DEM0_043);
	showText(TX_SPEAKER_MCCOY, TX_DEM0_026);
	showText(TX_SPEAKER_EVERTS, TX_DEM0_042);
	showText(TX_SPEAKER_MCCOY, TX_DEM0_025);
	showText(TX_SPEAKER_KIRK, TX_DEM0_007);
	showText(TX_SPEAKER_EVERTS, TX_DEM0_044);
}

void Room::demon0TalkToMcCoy() {
	if (_awayMission->demon.talkedToPrelate) {
		showText(TX_SPEAKER_KIRK, TX_DEM0_011);
		showText(TX_SPEAKER_MCCOY, TX_DEM0_023);
		showText(TX_SPEAKER_SPOCK, TX_DEM0_029);
		if (!_awayMission->redshirtDead)
			showText(TX_SPEAKER_EVERTS, TX_DEM0_041);
	} else {
		showText(TX_SPEAKER_MCCOY, TX_DEM0_019);
		if (!_awayMission->demon.askedPrelateAboutSightings) {
			demon0AskPrelateAboutSightings();
		}
	}
}

void Room::demon0TalkToSpock() {
	showText(TX_SPEAKER_SPOCK, TX_DEM0_014);

	if (!_awayMission->demon.talkedToPrelate && !_awayMission->demon.askedPrelateAboutSightings)
		demon0AskPrelateAboutSightings();
}

void Room::demon0AskPrelateAboutSightings() {
	showText(TX_SPEAKER_KIRK, TX_DEM0_001);
	showText(TX_SPEAKER_ANGIVEN, TX_DEM0_030);

	_awayMission->demon.askedPrelateAboutSightings = true;
}

void Room::demon0UseSTricorderAnywhere() {
	loadActorAnim2(OBJECT_SPOCK, "sscans", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM0_028);
}

void Room::demon0UseMTricorderAnywhere() {
	loadActorAnim2(OBJECT_MCCOY, "mscans", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_MCCOY, TX_DEM0_021);
}

void Room::demon0UseMTricorderOnPrelate() {
	loadActorAnim2(OBJECT_MCCOY, "mscans", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);

	if (_awayMission->demon.talkedToPrelate)
		showText(TX_SPEAKER_MCCOY, TX_DEM0_018);
	else
		showText(TX_SPEAKER_MCCOY, TX_DEM0_022);
}

// Helper functions
void Room::demon0BadResponse() {
	if (_awayMission->demon.wasRudeToPrelate)
		return;

	_awayMission->demon.missionScore -= 3;
	_awayMission->demon.wasRudeToPrelate = true;

	showText(TX_SPEAKER_ANGIVEN, TX_DEM0_033);
}
}
