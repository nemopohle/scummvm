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

#define OBJECT_VLICT 8
#define OBJECT_GUARD 9
#define OBJECT_QUETZECOATL 10

namespace StarTrek {

extern const RoomAction trial4ActionList[] = {
	{ {ACTION_TICK, 1,  0, 0, 0}, &Room::trial4Tick1 },
	{ {ACTION_TICK, 60, 0, 0, 0}, &Room::trial4Tick60 },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum trial4TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_UHURA, TX_SPEAKER_BENNIE,
	TX_SPEAKER_VLICT, TX_SPEAKER_QUETZECOATL,
	TX_TRI4_001, TX_TRI4_002, TX_TRI4_003, TX_TRI4_004, TX_TRI4_005,
	TX_TRI4_006, TX_TRI4_007, TX_TRI4_008, TX_TRI4_009, TX_TRI4_010,
	TX_TRI4_011, TX_TRI4_012, TX_TRI4_013, TX_TRI4_014, TX_TRI4_015,
	TX_TRI4_016, TX_TRI4_017, TX_TRI4_018, TX_TRI4_019, TX_TRI4_020,
	TX_TRI4_021, TX_TRI4_022, TX_TRI4_023, TX_TRI4_024, TX_TRI4_025,
	TX_TRI4_026, TX_TRI4_027, TX_TRI4_028, TX_TRI4_029, TX_TRI4_030,
	TX_TRI4_031, TX_TRI4_032, TX_TRI4_033, TX_TRI4_034, TX_TRI4_036,
	TX_TRI4_037, TX_TRI4_038, TX_TRI4_039, TX_TRI4_040, TX_TRI4_041,
	TX_TRI4_042, TX_TRI4_043, TX_TRI4_044, TX_TRI4_045, TX_TRI4_046,
	TX_TRI4_047, TX_TRI4_048, TX_TRI4_049, TX_TRI4_050, TX_TRI4_051,
	TX_TRI4_052, TX_TRI4_053, TX_TRI4_054, TX_TRI4_055, TX_TRI4_056,
	TX_TRI4_057, TX_TRI4_058, TX_TRI4_059, TX_TRI4_060, TX_TRI4_061,
	TX_TRI4_062, TX_TRI4_063, TX_TRI4_064, TX_TRI4_065, TX_TRI4_066,
	TX_TRI4_067, TX_TRI4_068, TX_TRI4_069, TX_TRI4_070, TX_TRI4_071,
	TX_TRI4_072, TX_TRI4_073, TX_TRI4_074, TX_TRI4_075, TX_TRI4_076
};

extern const RoomTextOffsets trial4TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 275, 275, 275, 275 },
	{ TX_SPEAKER_MCCOY, 286, 286, 286, 286 },
	{ TX_SPEAKER_SPOCK, 296, 296, 296, 296 },
	{ TX_SPEAKER_UHURA, 320, 320, 320, 320 },
	{ TX_SPEAKER_BENNIE, 306, 306, 306, 306 },
	{ TX_SPEAKER_VLICT, 330, 330, 330, 330 },
	{ TX_SPEAKER_QUETZECOATL, 336, 336, 336, 336 },
	{ TX_TRI4_001, 6207, 5462, 5962, 6730 },
	{ TX_TRI4_002, 1353, 1225, 1454, 1414 },
	{ TX_TRI4_003, 8171, 7140, 8116, 8863 },
	{ TX_TRI4_004, 4857, 4337, 4510, 5243 },
	{ TX_TRI4_005, 3358, 3004, 3250, 3627 },
	{ TX_TRI4_006, 7836, 6865, 7730, 8507 },
	{ TX_TRI4_007, 8534, 7428, 8534, 9244 },
	{ TX_TRI4_008, 4678, 4188, 4329, 5054 },
	{ TX_TRI4_009, 1275, 1162, 1363, 1331 },
	{ TX_TRI4_010, 8300, 7239, 8260, 8992 },
	{ TX_TRI4_011, 6615, 5810, 6392, 7174 },
	{ TX_TRI4_012, 8639, 7503, 8657, 9370 },
	{ TX_TRI4_013, 2919, 2624, 2794, 3149 },
	{ TX_TRI4_014, 1572, 1414, 1696, 1637 },
	{ TX_TRI4_015, 3131, 2807, 3024, 3377 },
	{ TX_TRI4_016, 7008, 6157, 6825, 7582 },
	{ TX_TRI4_017, 5392, 4782, 5086, 5842 },
	{ TX_TRI4_018, 5178, 4598, 4857, 5616 },
	{ TX_TRI4_019, 4384, 3924, 4029, 4754 },
	{ TX_TRI4_020, 2094, 1875, 2113, 2248 },
	{ TX_TRI4_021, 5691, 5021, 5425, 6182 },
	{ TX_TRI4_022, 683, 647, 695, 708 },
	{ TX_TRI4_023, 7415, 6504, 7257, 8063 },
	{ TX_TRI4_024, 7320, 6424, 7158, 7955 },
	{ TX_TRI4_025, 4550, 4075, 4195, 4934 },
	{ TX_TRI4_026, 7203, 6322, 7045, 7826 },
	{ TX_TRI4_027, 4994, 4444, 4643, 5394 },
	{ TX_TRI4_028, 5645, 4990, 5374, 6123 },
	{ TX_TRI4_029, 1992, 1788, 1979, 2125 },
	{ TX_TRI4_030, 2217, 1982, 2249, 2383 },
	{ TX_TRI4_031, 5790, 5105, 5530, 6291 },
	{ TX_TRI4_032, 922, 854, 977, 955 },
	{ TX_TRI4_033, 1211, 1113, 1306, 1274 },
	{ TX_TRI4_034, 5077, 4512, 4733, 5481 },
	{ TX_TRI4_036, 6007, 5292, 5760, 6515 },
	{ TX_TRI4_037, 6412, 5637, 6171, 6964 },
	{ TX_TRI4_038, 7692, 6751, 7575, 8365 },
	{ TX_TRI4_039, 774, 722, 812, 805 },
	{ TX_TRI4_040, 2495, 2230, 2549, 2683 },
	{ TX_TRI4_041, 3609, 3225, 3566, 3891 },
	{ TX_TRI4_042, 5425, 4800, 5119, 5874 },
	{ TX_TRI4_043, 1847, 1658, 1799, 1962 },
	{ TX_TRI4_044, 7742, 6786, 7625, 8414 },
	{ TX_TRI4_045, 8077, 7061, 8011, 8770 },
	{ TX_TRI4_046, 8440, 7349, 8429, 9151 },
	{ TX_TRI4_047, 8232, 7186, 8190, 8924 },
	{ TX_TRI4_048, 7893, 6907, 7802, 8582 },
	{ TX_TRI4_049, 8591, 7470, 8606, 9320 },
	{ TX_TRI4_050, 4077, 3647, 9460, 4411 },
	{ TX_TRI4_051, 3719, 3319, 3703, 4012 },
	{ TX_TRI4_052, 3412, 3043, 3324, 3685 },
	{ TX_TRI4_053, 3867, 3452, 9218, 4183 },
	{ TX_TRI4_054, 5579, 4939, 5293, 6044 },
	{ TX_TRI4_055, 7943, 6942, 7857, 8635 },
	{ TX_TRI4_056, 6073, 5343, 5841, 6591 },
	{ TX_TRI4_057, 7557, 6631, 7422, 8219 },
	{ TX_TRI4_058, 4919, 4384, 4569, 5311 },
	{ TX_TRI4_059, 6790, 5970, 6587, 7330 },
	{ TX_TRI4_060, 2709, 2429, 8972, 2924 },
	{ TX_TRI4_061, 538, 511, 538, 538 },
	{ TX_TRI4_062, 1458, 1315, 1574, 1518 },
	{ TX_TRI4_063, 2409, 2159, 2464, 2588 },
	{ TX_TRI4_064, 1117, 1034, 1213, 1182 },
	{ TX_TRI4_065, 6859, 6023, 6658, 7404 },
	{ TX_TRI4_066, 1647, 1474, 8721, 1736 },
	{ TX_TRI4_067, 8347, 7271, 8299, 9050 },
	{ TX_TRI4_068, 5935, 5235, 5677, 6429 },
	{ TX_TRI4_069, 4761, 4256, 4412, 5137 },
	{ TX_TRI4_070, 4249, 3804, 3901, 4610 },
	{ TX_TRI4_071, 3269, 2930, 3156, 3531 },
	{ TX_TRI4_072, 3044, 2735, 2921, 3272 },
	{ TX_TRI4_073, 6495, 5705, 6264, 7046 },
	{ TX_TRI4_074, 7109, 6243, 6941, 7707 },
	{ TX_TRI4_075, 5301, 4706, 4993, 5745 },
	{ TX_TRI4_076, 6315, 5555, 6067, 6852 },
	{          -1, 0,    0, 0, 0 }
};

extern const RoomText trial4Texts[] = {
	{ -1, Common::UNK_LANG, "" }
};

void Room::trial4Tick1() {
	playVoc("TRI4LOOP");

	loadActorAnim2(OBJECT_VLICT, "vlict1", 0x9f, 0x48);
	loadActorAnim2(OBJECT_GUARD, "kgstnd", 0xdc, 0x6a);
	loadActorAnim2(OBJECT_QUETZECOATL, "qstand", 0x10e, 0xaa);

	playMidiMusicTracks(MIDITRACK_32);
}

void Room::trial4Tick60() {
	showText(TX_SPEAKER_VLICT, TX_TRI4_061);

	const TextRef choices[] = {
		TX_SPEAKER_KIRK,
		TX_TRI4_022, TX_TRI4_039, TX_TRI4_032,
		TX_END
	};
	int choice = showMultipleTexts(choices);

	if (choice == 0) { // "You were trying to kill us"
		showText(TX_SPEAKER_VLICT, TX_TRI4_064);

		const TextRef choices2[] = {
			TX_SPEAKER_KIRK,
			TX_TRI4_033, TX_TRI4_009, TX_TRI4_002,
			TX_END
		};
		showMultipleTexts(choices2); // choice doesn't matter

		showText(TX_SPEAKER_VLICT, TX_TRI4_062);
		showText(TX_SPEAKER_KIRK,  TX_TRI4_014);
		showText(TX_SPEAKER_VLICT, TX_TRI4_066);
		showText(TX_SPEAKER_QUETZECOATL, TX_TRI4_043);

		const TextRef choices3[] = {
			TX_SPEAKER_KIRK,
			TX_TRI4_029, TX_TRI4_020, TX_TRI4_030,
			TX_END
		};
		choice = showMultipleTexts(choices3);

		if (choice == 0) { // "Then you have my deepest respect"
			// Nothing more
		} else if (choice == 1) { // "Make it a painless death"
			showText(TX_SPEAKER_VLICT, TX_TRI4_063);
		} else if (choice == 2) { // "I hope you chose the right time"
			showText(TX_SPEAKER_QUETZECOATL, TX_TRI4_040);
		}
	} else if (choice == 1) { // "Use of guile is honorable?"
		showText(TX_SPEAKER_VLICT, TX_TRI4_060);
		showText(TX_SPEAKER_KIRK,  TX_TRI4_013);
		showText(TX_SPEAKER_VLICT, TX_TRI4_072);
		showText(TX_SPEAKER_KIRK,  TX_TRI4_015);
		showText(TX_SPEAKER_VLICT, TX_TRI4_071);
		showText(TX_SPEAKER_KIRK,  TX_TRI4_005);
		showText(TX_SPEAKER_QUETZECOATL, TX_TRI4_052);
		showText(TX_SPEAKER_QUETZECOATL, TX_TRI4_041);
		showText(TX_SPEAKER_QUETZECOATL, TX_TRI4_051);
		showText(TX_SPEAKER_QUETZECOATL, TX_TRI4_053);
		showText(TX_SPEAKER_QUETZECOATL, TX_TRI4_050);
		showText(TX_SPEAKER_VLICT, TX_TRI4_070);

		const TextRef choices2[] = {
			TX_SPEAKER_KIRK,
			TX_TRI4_019, TX_TRI4_025, TX_TRI4_008,
			TX_END
		};
		choice = showMultipleTexts(choices2);

		if (choice == 1) { // Threatened Vlict
			showText(TX_SPEAKER_VLICT, TX_TRI4_069);
			showText(TX_SPEAKER_KIRK,  TX_TRI4_004);
		} // else, nothing more
	} else if (choice == 2) { // "You didn't say I couldn't use my ship"
		showText(TX_SPEAKER_VLICT, TX_TRI4_058);

		const TextRef choices2[] = {
			TX_SPEAKER_KIRK,
			TX_TRI4_027, TX_TRI4_034, TX_TRI4_018,
			TX_END
		};
		choice = showMultipleTexts(choices2);

		if (choice == 0) { // "Let's stop playing games"
			showText(TX_SPEAKER_VLICT, TX_TRI4_075);
			showText(TX_SPEAKER_KIRK,  TX_TRI4_017);
			showText(TX_SPEAKER_QUETZECOATL,  TX_TRI4_042);
			showText(TX_SPEAKER_VLICT, TX_TRI4_054);

			const TextRef choices3[] = {
				TX_SPEAKER_KIRK,
				TX_TRI4_028, TX_TRI4_021, TX_TRI4_031,
				TX_END
			};
			choice = showMultipleTexts(choices3);

			if (choice == 0 || choice == 1) {
				showText(TX_SPEAKER_VLICT, TX_TRI4_068);
				showText(TX_SPEAKER_MCCOY, TX_TRI4_036);
			} else if (choice == 2) {
				showText(TX_SPEAKER_VLICT, TX_TRI4_056);
				showText(TX_SPEAKER_KIRK, TX_TRI4_001);
			}
		} else if (choice == 1) { // "You lost, release Quetzecoatl"
			showText(TX_SPEAKER_VLICT, TX_TRI4_076);
			showText(TX_SPEAKER_MCCOY, TX_TRI4_037);
			showText(TX_SPEAKER_VLICT, TX_TRI4_073);
			showText(TX_SPEAKER_KIRK,  TX_TRI4_011);
			showText(TX_SPEAKER_VLICT, TX_TRI4_059);
		} else if (choice == 2) { // "Nature beat me to it"
			showText(TX_SPEAKER_VLICT, TX_TRI4_065);
			showText(TX_SPEAKER_KIRK,  TX_TRI4_016);
			showText(TX_SPEAKER_VLICT, TX_TRI4_074);

			const TextRef choices3[] = {
				TX_SPEAKER_KIRK,
				TX_TRI4_026, TX_TRI4_024, TX_TRI4_023,
				TX_END
			};
			choice = showMultipleTexts(choices3);

			if (choice == 0) { // "The empire will learn you betrayed them"
				showText(TX_SPEAKER_VLICT, TX_TRI4_057);
				showText(TX_SPEAKER_SPOCK, TX_TRI4_038);
				showText(TX_SPEAKER_QUETZECOATL, TX_TRI4_044);
				showText(TX_SPEAKER_KIRK, TX_TRI4_006);
				showText(TX_SPEAKER_QUETZECOATL, TX_TRI4_048);
			} else if (choice == 1) { // "You didn't give him a fair trial"
				showText(TX_SPEAKER_VLICT,       TX_TRI4_055);
				showText(TX_SPEAKER_QUETZECOATL, TX_TRI4_045);
				showText(TX_SPEAKER_KIRK,        TX_TRI4_003);
				showText(TX_SPEAKER_QUETZECOATL, TX_TRI4_047);
				showText(TX_SPEAKER_KIRK,        TX_TRI4_010);
			} else if (choice == 2) { // "How can a liar like you say 'honor'"
				showText(TX_SPEAKER_VLICT,       TX_TRI4_067);
				showText(TX_SPEAKER_QUETZECOATL, TX_TRI4_046);
				showText(TX_SPEAKER_KIRK,        TX_TRI4_007);
				showText(TX_SPEAKER_QUETZECOATL, TX_TRI4_049);
			}
		}
	}

	showText(TX_SPEAKER_KIRK, TX_TRI4_012);
	_awayMission->trial.missionEndMethod = 2;
	endMission(_awayMission->trial.missionScore, _awayMission->trial.field2b, 2);
}

}
