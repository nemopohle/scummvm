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
	{ TX_SPEAKER_KIRK, 275, 275, 0, 275 },
	{ TX_SPEAKER_MCCOY, 286, 286, 0, 286 },
	{ TX_SPEAKER_SPOCK, 296, 296, 0, 296 },
	{ TX_SPEAKER_UHURA, 320, 320, 0, 320 },
	{ TX_SPEAKER_BENNIE, 306, 306, 0, 306 },
	{ TX_SPEAKER_VLICT, 330, 330, 0, 330 },
	{ TX_SPEAKER_QUETZECOATL, 336, 336, 0, 336 },
	{ TX_TRI4_001, 6207, 5462, 0, 6730 },
	{ TX_TRI4_002, 1353, 1225, 0, 1414 },
	{ TX_TRI4_003, 8171, 7140, 0, 8863 },
	{ TX_TRI4_004, 4857, 4337, 0, 5243 },
	{ TX_TRI4_005, 3358, 3004, 0, 3627 },
	{ TX_TRI4_006, 7836, 6865, 0, 8507 },
	{ TX_TRI4_007, 8534, 7428, 0, 9244 },
	{ TX_TRI4_008, 4678, 4188, 0, 5054 },
	{ TX_TRI4_009, 1275, 1162, 0, 1331 },
	{ TX_TRI4_010, 8300, 7239, 0, 8992 },
	{ TX_TRI4_011, 6615, 5810, 0, 7174 },
	{ TX_TRI4_012, 8639, 7503, 0, 9370 },
	{ TX_TRI4_013, 2919, 2624, 0, 3149 },
	{ TX_TRI4_014, 1572, 1414, 0, 1637 },
	{ TX_TRI4_015, 3131, 2807, 0, 3377 },
	{ TX_TRI4_016, 7008, 6157, 0, 7582 },
	{ TX_TRI4_017, 5392, 4782, 0, 5842 },
	{ TX_TRI4_018, 5178, 4598, 0, 5616 },
	{ TX_TRI4_019, 4384, 3924, 0, 4754 },
	{ TX_TRI4_020, 2094, 1875, 0, 2248 },
	{ TX_TRI4_021, 5691, 5021, 0, 6182 },
	{ TX_TRI4_022, 683, 647, 0, 708 },
	{ TX_TRI4_023, 7415, 6504, 0, 8063 },
	{ TX_TRI4_024, 7320, 6424, 0, 7955 },
	{ TX_TRI4_025, 4550, 4075, 0, 4934 },
	{ TX_TRI4_026, 7203, 6322, 0, 7826 },
	{ TX_TRI4_027, 4994, 4444, 0, 5394 },
	{ TX_TRI4_028, 5645, 4990, 0, 6123 },
	{ TX_TRI4_029, 1992, 1788, 0, 2125 },
	{ TX_TRI4_030, 2217, 1982, 0, 2383 },
	{ TX_TRI4_031, 5790, 5105, 0, 6291 },
	{ TX_TRI4_032, 922, 854, 0, 955 },
	{ TX_TRI4_033, 1211, 1113, 0, 1274 },
	{ TX_TRI4_034, 5077, 4512, 0, 5481 },
	{ TX_TRI4_036, 6007, 5292, 0, 6515 },
	{ TX_TRI4_037, 6412, 5637, 0, 6964 },
	{ TX_TRI4_038, 7692, 6751, 0, 8365 },
	{ TX_TRI4_039, 774, 722, 0, 805 },
	{ TX_TRI4_040, 2495, 2230, 0, 2683 },
	{ TX_TRI4_041, 3609, 3225, 0, 3891 },
	{ TX_TRI4_042, 5425, 4800, 0, 5874 },
	{ TX_TRI4_043, 1847, 1658, 0, 1962 },
	{ TX_TRI4_044, 7742, 6786, 0, 8414 },
	{ TX_TRI4_045, 8077, 7061, 0, 8770 },
	{ TX_TRI4_046, 8440, 7349, 0, 9151 },
	{ TX_TRI4_047, 8232, 7186, 0, 8924 },
	{ TX_TRI4_048, 7893, 6907, 0, 8582 },
	{ TX_TRI4_049, 8591, 7470, 0, 9320 },
	{ TX_TRI4_050, 4077, 3647, 0, 4411 },
	{ TX_TRI4_051, 3719, 3319, 0, 4012 },
	{ TX_TRI4_052, 3412, 3043, 0, 3685 },
	{ TX_TRI4_053, 3867, 3452, 0, 4183 },
	{ TX_TRI4_054, 5579, 4939, 0, 6044 },
	{ TX_TRI4_055, 7943, 6942, 0, 8635 },
	{ TX_TRI4_056, 6073, 5343, 0, 6591 },
	{ TX_TRI4_057, 7557, 6631, 0, 8219 },
	{ TX_TRI4_058, 4919, 4384, 0, 5311 },
	{ TX_TRI4_059, 6790, 5970, 0, 7330 },
	{ TX_TRI4_060, 2709, 2429, 0, 2924 },
	{ TX_TRI4_061, 538, 511, 0, 538 },
	{ TX_TRI4_062, 1458, 1315, 0, 1518 },
	{ TX_TRI4_063, 2409, 2159, 0, 2588 },
	{ TX_TRI4_064, 1117, 1034, 0, 1182 },
	{ TX_TRI4_065, 6859, 6023, 0, 7404 },
	{ TX_TRI4_066, 1647, 1474, 0, 1736 },
	{ TX_TRI4_067, 8347, 7271, 0, 9050 },
	{ TX_TRI4_068, 5935, 5235, 0, 6429 },
	{ TX_TRI4_069, 4761, 4256, 0, 5137 },
	{ TX_TRI4_070, 4249, 3804, 0, 4610 },
	{ TX_TRI4_071, 3269, 2930, 0, 3531 },
	{ TX_TRI4_072, 3044, 2735, 0, 3272 },
	{ TX_TRI4_073, 6495, 5705, 0, 7046 },
	{ TX_TRI4_074, 7109, 6243, 0, 7707 },
	{ TX_TRI4_075, 5301, 4706, 0, 5745 },
	{ TX_TRI4_076, 6315, 5555, 0, 6852 },
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
