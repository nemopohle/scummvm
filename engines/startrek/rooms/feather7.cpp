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

#define OBJECT_QUETZECOATL 8

namespace StarTrek {

extern const RoomAction feather7ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0},       &Room::feather7Tick1 },
	{ {ACTION_TICK, 40, 0, 0, 0},      &Room::feather7Tick40 },
	{ {ACTION_DONE_WALK, 1, 0, 0, 0},  &Room::feather7KirkReachedSeat },
	{ {ACTION_DONE_WALK, 2, 0, 0, 0},  &Room::feather7SpockReachedSeat },
	{ {ACTION_DONE_WALK, 3, 0, 0, 0},  &Room::feather7MccoyReachedSeat },
	{ {ACTION_DONE_ANIM, 4, 0, 0, 0},  &Room::feather7QuetzecoatlReachedSeat },
	{ {ACTION_DONE_ANIM, 1, 0, 0, 0},  &Room::feather7KirkSatDown },
	{ {ACTION_DONE_ANIM, 2, 0, 0, 0},  &Room::feather7ReadyToBeamOut },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum feather7TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_STRAGEY, TX_SPEAKER_QUETZECOATL,
	TX_FEA7_001, TX_FEA7_002, TX_FEA7_003, TX_FEA7_004, TX_FEA7_005,
	TX_FEA7_006, TX_FEA7_007, TX_FEA7_008, TX_FEA7_009, TX_FEA7_010,
	TX_FEA7_011, TX_FEA7_012, TX_FEA7_013, TX_FEA7_014, TX_FEA7_015,
	TX_FEA7_016, TX_FEA7_017, TX_FEA7_018, TX_FEA7_019, TX_FEA7_020,
	TX_FEA7_021, TX_FEA7_022, TX_FEA7_023, TX_FEA7_024, TX_FEA7_025,
	TX_FEA7_026, TX_FEA7_027, TX_FEA7_028, TX_FEA7_029, TX_FEA7_030
};

extern const RoomTextOffsets feather7TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 1064, 1038, 1064, 1064, 1052, 1052, 1052 },
	{ TX_SPEAKER_MCCOY, 1075, 1049, 1075, 1075, 1063, 1065, 1063 },
	{ TX_SPEAKER_SPOCK, 1085, 1059, 1085, 1085, 1073, 1075, 1073 },
	{ TX_SPEAKER_STRAGEY, 1095, 1069, 1095, 1095, 1083, 1085, 1083 },
	{ TX_SPEAKER_QUETZECOATL, 1107, 1081, 1107, 1107, 1095, 1097, 1095 },
	{ TX_FEA7_001, 4842, 4377, 5468, 5362, 4395, 5003, 5020 },
	{ TX_FEA7_002, 4802, 4352, 5424, 5313, 4370, 4969, 4991 },
	{ TX_FEA7_003, 4674, 4239, 5280, 5164, 4257, 4833, 4862 },
	{ TX_FEA7_004, 2517, 2295, 2764, 2674, 2309, 2506, 2557 },
	{ TX_FEA7_005, 1697, 1596, 1806, 1754, 1610, 1691, 1719 },
	{ TX_FEA7_006, 1896, 1780, 2054, 2001, 1794, 1915, 1952 },
	{ TX_FEA7_007, 2446, 2239, 2692, 2614, 2253, 2461, 2500 },
	{ TX_FEA7_008, 3887, 3557, 4341, 4257, 3575, 4038, 4028 },
	{ TX_FEA7_009, 4020, 3675, 4492, 4396, 3693, 4163, 4164 },
	{ TX_FEA7_010, 2002, 1871, 2190, 2121, 1885, 2021, 2073 },
	{ TX_FEA7_011, 4128, 3768, 4620, 4508, 3786, 4260, 4277 },
	{ TX_FEA7_012, 2687, 2450, 2975, 2862, 2464, 2688, 2753 },
	{ TX_FEA7_013, 4602, 4182, 5195, 5078, 4200, 4762, 4792 },
	{ TX_FEA7_014, 3614, 3315, 4015, 3951, 3333, 3753, 3732 },
	{ TX_FEA7_015, 4443, 4038, 4974, 4874, 4056, 4563, 4586 },
	{ TX_FEA7_016, 2250, 0, 2477, 2410, 0, 0, 0 },
	{ TX_FEA7_017, 1120, 1094, 1120, 1120, 1108, 1110, 1108 },
	{ TX_FEA7_018, 3069, 2801, 3411, 3324, 2816, 3126, 3159 },
	{ TX_FEA7_019, 2117, 1956, 2328, 2272, 1970, 2151, 2181 },
	{ TX_FEA7_020, 2044, 1898, 2234, 2173, 1912, 2058, 2102 },
	{ TX_FEA7_021, 1334, 1278, 1381, 1354, 1292, 1331, 1339 },
	{ TX_FEA7_022, 1214, 1173, 1231, 1218, 1187, 1206, 1204 },
	{ TX_FEA7_023, 1547, 1461, 1627, 1577, 1475, 1529, 1555 },
	{ TX_FEA7_024, 3724, 3409, 4161, 4066, 3427, 3853, 3863 },
	{ TX_FEA7_025, 4164, 3789, 4665, 4545, 3807, 4282, 4307 },
	{ TX_FEA7_026, 3297, 3013, 3672, 3585, 3029, 3383, 3404 },
	{ TX_FEA7_027, 1490, 1419, 1557, 1514, 1433, 1476, 1500 },
	{ TX_FEA7_028, 4381, 3991, 4909, 4781, 4009, 4483, 4536 },
	{ TX_FEA7_029, 2295, 2074, 2522, 2454, 2088, 2303, 2345 },
	{ TX_FEA7_030, 2806, 2554, 3114, 3026, 2568, 2833, 2877 },
	{          -1, 0,    0, 0, 0, 0, 0, 0 }
};

extern const RoomText feather7Texts[] = {
	{ -1, Common::UNK_LANG, "" }
};

void Room::feather7Tick1() {
	playVoc("FEA7LOOP");
	playMidiMusicTracks(MIDITRACK_33);
	_awayMission->disableInput = 2;
	loadActorAnim(OBJECT_QUETZECOATL, "s5r7qn", 0x106, 0x98);
}

void Room::feather7Tick40() {
	if (_awayMission->feather.diedFromStalactites)
		showText(TX_SPEAKER_QUETZECOATL, TX_FEA7_017);

	showText(TX_SPEAKER_QUETZECOATL, TX_FEA7_022);

	if (_awayMission->redshirtDead)
		showText(TX_SPEAKER_QUETZECOATL, TX_FEA7_021);

	// BUGFIX: Show this even if redshirt isn't dead (he wishes you wouldn't have knocked
	// out Tlaoxac)
	if (_awayMission->feather.knockedOutTlaoxac)
		showText(TX_SPEAKER_QUETZECOATL, TX_FEA7_023);

	showText(TX_SPEAKER_QUETZECOATL, TX_FEA7_027);

	walkCrewmanC(OBJECT_KIRK, 0x6c, 0x93, &Room::feather7KirkReachedSeat);
	walkCrewmanC(OBJECT_SPOCK, 0xbb, 0x8c, &Room::feather7SpockReachedSeat);
	walkCrewmanC(OBJECT_MCCOY, 0x8d, 0x8c, &Room::feather7MccoyReachedSeat);
	loadActorAnimC(OBJECT_QUETZECOATL, "s5r7qw", -1, -1, &Room::feather7QuetzecoatlReachedSeat);
}

void Room::feather7KirkReachedSeat() {
	loadActorAnimC(OBJECT_KIRK, "s5r7ks", -1, -1, &Room::feather7KirkSatDown);
}

void Room::feather7SpockReachedSeat() {
	loadActorAnim2(OBJECT_SPOCK, "s5r7ss");
}

void Room::feather7MccoyReachedSeat() {
	loadActorAnim2(OBJECT_MCCOY, "s5r7ms");
}

void Room::feather7QuetzecoatlReachedSeat() {
	loadActorAnim2(OBJECT_QUETZECOATL, "s5r7qn", 0x97, 0x98);
}

void Room::feather7KirkSatDown() {
	const TextRef choices1[] = {
		TX_SPEAKER_KIRK,
		TX_FEA7_005, TX_FEA7_006, TX_FEA7_010,
		TX_END
	};
	int choice = showMultipleTexts(choices1);

	if (choice == 0) {
		_roomVar.feather.insultedQuetzecoatl = true;
		showText(TX_SPEAKER_QUETZECOATL, TX_FEA7_020);
	} else if (choice == 1)
		showText(TX_SPEAKER_QUETZECOATL, TX_FEA7_019);
	else
		showText(TX_SPEAKER_QUETZECOATL, TX_FEA7_016);

	showText(TX_SPEAKER_QUETZECOATL, TX_FEA7_029);

	const TextRef choices2[] = {
		TX_SPEAKER_KIRK,
		TX_FEA7_007, TX_FEA7_004, TX_FEA7_012,
		TX_END
	};
	choice = showMultipleTexts(choices2);

	if (choice == 0) {
		_roomVar.feather.insultedQuetzecoatl = true;
		showText(TX_SPEAKER_QUETZECOATL, TX_FEA7_030);
	} else if (choice == 1)
		showText(TX_SPEAKER_QUETZECOATL, TX_FEA7_018);
	else {
		_roomVar.feather.insultedQuetzecoatl = true;
		showText(TX_SPEAKER_QUETZECOATL, TX_FEA7_026);
	}

	showText(TX_SPEAKER_SPOCK, TX_FEA7_014);
	showText(TX_SPEAKER_QUETZECOATL, TX_FEA7_024);

	const TextRef choices3[] = {
		TX_SPEAKER_KIRK,
		TX_FEA7_008, TX_FEA7_009, TX_FEA7_011,
		TX_END
	};
	choice = showMultipleTexts(choices3);

	if (choice == 0) {
		_roomVar.feather.insultedQuetzecoatl = true;
		showText(TX_SPEAKER_QUETZECOATL, TX_FEA7_025);
	} else if (choice == 1)
		showText(TX_SPEAKER_QUETZECOATL, TX_FEA7_028);

	showText(TX_SPEAKER_QUETZECOATL, TX_FEA7_015);
	showText(TX_SPEAKER_MCCOY, TX_FEA7_013);
	showText(TX_SPEAKER_KIRK, TX_FEA7_003);

	loadActorAnimC(OBJECT_KIRK, "s5r7ku", -1, -1, &Room::feather7ReadyToBeamOut);
	loadActorAnim2(OBJECT_SPOCK, "s5r7su");
	loadActorAnim2(OBJECT_MCCOY, "s5r7mu");
}

void Room::feather7ReadyToBeamOut() {
	if (_awayMission->redshirtDead)
		showText(TX_SPEAKER_KIRK, TX_FEA7_002);
	else {
		_awayMission->feather.missionScore += 1;
		showText(TX_SPEAKER_KIRK, TX_FEA7_001);
	}

	if (!_roomVar.feather.insultedQuetzecoatl)
		_awayMission->feather.missionScore += 2;
	_awayMission->feather.missionScore += 4;

	loadActorAnim2(OBJECT_QUETZECOATL, "s5r7qt");
	endMission(_awayMission->feather.missionScore, 0x13, 0);
}

}
