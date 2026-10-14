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

#define HOTSPOT_BENCH 0x23
#define HOTSPOT_WINDOW 0x25
#define HOTSPOT_CENTER 0x26

namespace StarTrek {

extern const RoomAction trial0ActionList[] = {
	{ {ACTION_TICK, 1,  0, 0, 0}, &Room::trial0Tick1 },
	{ {ACTION_TICK, 40, 0, 0, 0}, &Room::trial0Tick40 },
	{ {ACTION_TIMER_EXPIRED, 0, 0, 0, 0}, &Room::trial0Timer0Expired },

	{ {ACTION_WALK, HOTSPOT_CENTER, 0, 0, 0}, &Room::trial0WalkToRoomCenter },
	{ {ACTION_DONE_WALK, 1, 0, 0, 0},         &Room::trial0ReachedRoomCenter },

	{ {ACTION_LOOK, HOTSPOT_WINDOW,     0, 0, 0}, &Room::trial0LookAtWindow },
	{ {ACTION_LOOK, OBJECT_VLICT,       0, 0, 0}, &Room::trial0LookAtVlict },
	{ {ACTION_LOOK, OBJECT_GUARD,       0, 0, 0}, &Room::trial0LookAtGuard },
	{ {ACTION_LOOK, HOTSPOT_BENCH,      0, 0, 0}, &Room::trial0LookAtBench },
	{ {ACTION_LOOK, HOTSPOT_CENTER,     0, 0, 0}, &Room::trial0LookAtSeal },
	{ {ACTION_LOOK, OBJECT_KIRK,        0, 0, 0}, &Room::trial0LookAtKirk },
	{ {ACTION_LOOK, OBJECT_SPOCK,       0, 0, 0}, &Room::trial0LookAtSpock },
	{ {ACTION_LOOK, OBJECT_MCCOY,       0, 0, 0}, &Room::trial0LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_REDSHIRT,    0, 0, 0}, &Room::trial0LookAtRedshirt },
	{ {ACTION_LOOK, OBJECT_QUETZECOATL, 0, 0, 0}, &Room::trial0LookAtQuetzecoatl },
	{ {ACTION_TALK, OBJECT_VLICT,       0, 0, 0}, &Room::trial0TalkToVlict },
	{ {ACTION_TALK, OBJECT_GUARD,       0, 0, 0}, &Room::trial0TalkToGuard },
	{ {ACTION_TALK, OBJECT_QUETZECOATL, 0, 0, 0}, &Room::trial0TalkToQuetzecoatl },
	{ {ACTION_TALK, OBJECT_MCCOY,       0, 0, 0}, &Room::trial0TalkToMccoy },
	{ {ACTION_TALK, OBJECT_SPOCK,       0, 0, 0}, &Room::trial0TalkToSpock },
	{ {ACTION_TALK, OBJECT_REDSHIRT,    0, 0, 0}, &Room::trial0TalkToRedshirt },

	{ {ACTION_USE, OBJECT_IPHASERS, 0xff, 0, 0}, &Room::trial0UsePhaserOrRock },
	{ {ACTION_USE, OBJECT_IPHASERK, 0xff, 0, 0}, &Room::trial0UsePhaserOrRock },
	{ {ACTION_USE, OBJECT_IROCK,    0xff, 0, 0}, &Room::trial0UsePhaserOrRock },
	{ {ACTION_USE, OBJECT_ISNAKE, OBJECT_VLICT, 0, 0}, &Room::trial0UseSnakeOnVlict },
	{ {ACTION_USE, OBJECT_ISNAKE, OBJECT_GUARD, 0, 0}, &Room::trial0UseSnakeOnGuard },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_QUETZECOATL, 0, 0}, &Room::trial0UseMTricorderOnQuetzecoatl },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_VLICT, 0, 0}, &Room::trial0UseMTricorderOnVlict },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0xff, 0, 0}, &Room::trial0UseMTricorderAnywhere },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_WINDOW, 0, 0}, &Room::trial0UseSTricorderOnWindow },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0xff, 0, 0}, &Room::trial0UseSTricorderAnywhere },
	{ {ACTION_USE, OBJECT_ICOMM, 0xff, 0, 0}, &Room::trial0UseCommunicator },

	{ {ACTION_USE, OBJECT_SPOCK,    OBJECT_GUARD, 0, 0}, &Room::trial0UseCrewmanOnKlingon },
	{ {ACTION_USE, OBJECT_KIRK,     OBJECT_GUARD, 0, 0}, &Room::trial0UseCrewmanOnKlingon },
	{ {ACTION_USE, OBJECT_REDSHIRT, OBJECT_GUARD, 0, 0}, &Room::trial0UseCrewmanOnKlingon },
	{ {ACTION_USE, OBJECT_SPOCK,    OBJECT_VLICT, 0, 0}, &Room::trial0UseCrewmanOnKlingon },
	{ {ACTION_USE, OBJECT_KIRK,     OBJECT_VLICT, 0, 0}, &Room::trial0UseCrewmanOnKlingon },
	{ {ACTION_USE, OBJECT_REDSHIRT, OBJECT_VLICT, 0, 0}, &Room::trial0UseCrewmanOnKlingon },
	{ {ACTION_USE, OBJECT_MCCOY,    OBJECT_VLICT, 0, 0}, &Room::trial0UseMccoyOnVlict },
	{ {ACTION_USE, OBJECT_MCCOY,    OBJECT_GUARD, 0, 0}, &Room::trial0UseMccoyOnGuard },
	{ {ACTION_USE, OBJECT_IMEDKIT,  0xff, 0, 0}, &Room::trial0UseMedkitAnywhere },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum trial0TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_UHURA, TX_SPEAKER_BENNIE,
	TX_SPEAKER_VLICT, TX_SPEAKER_QUETZECOATL, TX_SPEAKER_KLINGON_GUARD,
	TX_TRI0_001, TX_TRI0_002, TX_TRI0_003, TX_TRI0_004,	TX_TRI0_006,
	TX_TRI0_007, TX_TRI0_008, TX_TRI0_009, TX_TRI0_010, TX_TRI0_011,
	TX_TRI0_013, TX_TRI0_014, TX_TRI0_015, TX_TRI0_016, TX_TRI0_017,
	TX_TRI0_018, TX_TRI0_019, TX_TRI0_020, TX_TRI0_021, TX_TRI0_022,
	TX_TRI0_023, TX_TRI0_024, TX_TRI0_025, TX_TRI0_026, TX_TRI0_027,
	TX_TRI0_028, TX_TRI0_029, TX_TRI0_030, TX_TRI0_031, TX_TRI0_034,
	TX_TRI0_035, TX_TRI0_036, TX_TRI0_037, TX_TRI0_038, TX_TRI0_039,
	TX_TRI0_040, TX_TRI0_041, TX_TRI0_042, TX_TRI0_043, TX_TRI0_044,
	TX_TRI0_045, TX_TRI0_046, TX_TRI0_047, TX_TRI0_048, TX_TRI0_049,
	TX_TRI0_050, TX_TRI0_051, TX_TRI0_052, TX_TRI0_053, TX_TRI0_054,
	TX_TRI0_055, TX_TRI0_056, TX_TRI0_057, TX_TRI0_058, TX_TRI0_059,
	TX_TRI0_060, TX_TRI0_061, TX_TRI0_062, TX_TRI0_063, TX_TRI0_064,
	TX_TRI0_065, TX_TRI0_066, TX_TRI0_F24, TX_TRI4_076, TX_TRI0N000,
	TX_TRI0N001, TX_TRI0N002, TX_TRI0N003, TX_TRI0N004, TX_TRI0N005,
	TX_TRI0N006, TX_TRI0N007, TX_TRI0N008, TX_TRI0N009, TX_TRI0U075
};

extern const RoomTextOffsets trial0TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 279, 279, 279, 279 },
	{ TX_SPEAKER_MCCOY, 290, 290, 290, 290 },
	{ TX_SPEAKER_SPOCK, 300, 300, 300, 300 },
	{ TX_SPEAKER_UHURA, 324, 324, 324, 324 },
	{ TX_SPEAKER_BENNIE, 310, 310, 310, 310 },
	{ TX_SPEAKER_VLICT, 334, 334, 334, 334 },
	{ TX_SPEAKER_QUETZECOATL, 354, 354, 354, 354 },
	{ TX_SPEAKER_KLINGON_GUARD, 340, 340, 340, 340 },
	{ TX_TRI0_001, 2728, 2500, 2795, 2695 },
	{ TX_TRI0_002, 2315, 2127, 10737, 2220 },
	{ TX_TRI0_003, 4950, 4706, 5032, 5095 },
	{ TX_TRI0_004, 1556, 1459, 1669, 1385 },
	{ TX_TRI0_006, 4097, 3719, 4101, 4193 },
	{ TX_TRI0_007, 7911, 7335, 7968, 7973 },
	{ TX_TRI0_008, 4721, 4507, 4800, 4857 },
	{ TX_TRI0_009, 5176, 4902, 5256, 5345 },
	{ TX_TRI0_010, 4268, 3879, 4284, 4368 },
	{ TX_TRI0_011, 1832, 1704, 1976, 1697 },
	{ TX_TRI0_013, 5423, 5117, 5525, 5599 },
	{ TX_TRI0_014, 5269, 4979, 5355, 5447 },
	{ TX_TRI0_015, 2067, 1909, 2247, 1953 },
	{ TX_TRI0_016, 1083, 1031, 1131, 1080 },
	{ TX_TRI0_017, 4409, 4226, 4454, 4522 },
	{ TX_TRI0_018, 1977, 1834, 2140, 1851 },
	{ TX_TRI0_019, 957, 920, 1006, 950 },
	{ TX_TRI0_020, 4570, 4371, 4629, 4701 },
	{ TX_TRI0_021, 1174, 1107, 1232, 1182 },
	{ TX_TRI0_022, 8073, 7466, 8170, 8151 },
	{ TX_TRI0_023, 3960, 3597, 3963, 4056 },
	{ TX_TRI0_024, 9850, 9050, 10103, 10065 },
	{ TX_TRI0_025, 8440, 7773, 8544, 8520 },
	{ TX_TRI0_026, 8636, 7938, 8758, 8719 },
	{ TX_TRI0_027, 9956, 9141, 10219, 10171 },
	{ TX_TRI0_028, 10033, 9203, 10296, 10249 },
	{ TX_TRI0_029, 9149, 8443, 9341, 9332 },
	{ TX_TRI0_030, 9268, 8547, 9476, 9454 },
	{ TX_TRI0_031, 8370, 7718, 8480, 8455 },
	{ TX_TRI0_034, 10125, 9280, 10407, 10347 },
	{ TX_TRI0_035, 9383, 8643, 9598, 9572 },
	{ TX_TRI0_036, 8794, 8081, 8938, 8898 },
	{ TX_TRI0_037, 9487, 8732, 9702, 9678 },
	{ TX_TRI0_038, 9621, 8851, 9852, 9835 },
	{ TX_TRI0_039, 3448, 3145, 3611, 3515 },
	{ TX_TRI0_040, 3765, 3432, 3748, 3829 },
	{ TX_TRI0_041, 3124, 2851, 3258, 3137 },
	{ TX_TRI0_042, 8269, 7632, 8380, 8341 },
	{ TX_TRI0_043, 2194, 2021, 2382, 2081 },
	{ TX_TRI0_044, 6132, 5764, 6049, 6099 },
	{ TX_TRI0_045, 5787, 5449, 10991, 10924 },
	{ TX_TRI0_046, 3831, 3483, 3818, 3905 },
	{ TX_TRI0_047, 1665, 1553, 1792, 1522 },
	{ TX_TRI0_048, 2981, 2724, 3065, 2973 },
	{ TX_TRI0_049, 6054, 5701, 5971, 6016 },
	{ TX_TRI0_050, 3269, 2981, 3422, 3296 },
	{ TX_TRI0_051, 5011, 4752, 5090, 5162 },
	{ TX_TRI0_052, 10206, 3998, 10501, 10434 },
	{ TX_TRI0_053, 831, 809, 843, 839 },
	{ TX_TRI0_054, 1350, 1268, 1449, 10690 },
	{ TX_TRI0_055, 5619, 5297, 5742, 5813 },
	{ TX_TRI0_056, 2500, 2302, 2527, 2421 },
	{ TX_TRI0_057, 4878, 4649, 4961, 5021 },
	{ TX_TRI0_058, 653, 647, 653, 653 },
	{ TX_TRI0_059, 3550, 3232, 11233, 3622 },
	{ TX_TRI0_060, 2851, 2608, 2937, 2833 },
	{ TX_TRI0_061, 2570, 2357, 2607, 2500 },
	{ TX_TRI0_062, 7660, 7129, 7687, 7681 },
	{ TX_TRI0_063, 8557, 7874, 8669, 8636 },
	{ TX_TRI0_064, 7945, 7354, 8005, 8008 },
	{ TX_TRI0_065, 7809, 7248, 7860, 7857 },
	{ TX_TRI0_066, 8227, 7605, 8328, 8295 },
	{ TX_TRI0_F24, 9031, 7202, 9206, 9202 },
	{ TX_TRI0_F24, 7748, 7202, 7782, 7786 },
	{ TX_TRI4_076, 8927, 0, 9095, 9044 },
	{ TX_TRI0N000, 6620, 6211, 6599, 6633 },
	{ TX_TRI0N001, 6528, 6135, 6502, 6534 },
	{ TX_TRI0N002, 7287, 6802, 7303, 7296 },
	{ TX_TRI0N003, 7433, 6933, 7460, 7455 },
	{ TX_TRI0N004, 7021, 6567, 7009, 7037 },
	{ TX_TRI0N005, 7158, 6689, 7171, 7178 },
	{ TX_TRI0N006, 6778, 6354, 6768, 6782 },
	{ TX_TRI0N007, 6363, 5985, 6314, 6364 },
	{ TX_TRI0N008, 6941, 6502, 6932, 6955 },
	{ TX_TRI0N009, 7545, 7029, 7572, 7568 },
	{ TX_TRI0U075, 9675, 8890, 9906, 9890 },
	{          -1, 0,    0, 0, 0 }
};

extern const RoomText trial0Texts[] = {
	{ -1, Common::UNK_LANG, "" }
};

void Room::trial0Tick1() {
	playVoc("TRI0LOOP");
	_awayMission->timers[0] = getRandomWordInRange(180, 600);
	_awayMission->trial.field2b = 0x1d;
	loadActorAnim2(OBJECT_VLICT, "vlict1", 0x9f, 0x48);
	loadActorAnim2(OBJECT_GUARD, "kgstnd", 0xdc, 0x6a);
	loadActorAnim2(OBJECT_QUETZECOATL, "qteleg", 0x10e, 0xaa);
}

void Room::trial0Tick40() {
	showText(TX_SPEAKER_VLICT, TX_TRI0_058);
}

void Room::trial0Timer0Expired() { // Doesn't do anything?
	_awayMission->timers[0] = getRandomWordInRange(180, 600);
}

void Room::trial0WalkToRoomCenter() {
	_awayMission->disableInput = true;
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_N;
	walkCrewmanC(OBJECT_KIRK, 0xa0, 0xaa, &Room::trial0ReachedRoomCenter);
}

void Room::trial0ReachedRoomCenter() {
	bool gaveUp = false;

	_awayMission->disableInput = false;
	showText(TX_SPEAKER_VLICT, TX_TRI0_053);

	const TextRef choices1[] = {
		TX_SPEAKER_KIRK,
		TX_TRI0_019, TX_TRI0_016, TX_TRI0_021,
		TX_END
	};
	showMultipleTexts(choices1);

	showText(TX_SPEAKER_VLICT, TX_TRI0_054);
	showText(TX_SPEAKER_KIRK,  TX_TRI0_004);
	showText(TX_SPEAKER_VLICT, TX_TRI0_047);

	const TextRef choices2[] = {
		TX_SPEAKER_KIRK,
		TX_TRI0_011, TX_TRI0_018, TX_TRI0_015,
	    TX_END
	};
	int choice = showMultipleTexts(choices2);

	if (choice == 0) { // Kirk intervenes as a warrior
		showText(TX_SPEAKER_VLICT, TX_TRI0_043);
		showText(TX_SPEAKER_KIRK,  TX_TRI0_002);
		showText(TX_SPEAKER_VLICT, TX_TRI0_056);
		showText(TX_SPEAKER_VLICT, TX_TRI0_061);
		showText(TX_SPEAKER_KIRK,  TX_TRI0_001);
		showText(TX_SPEAKER_VLICT, TX_TRI0_060);
		_awayMission->disableInput = true;
		loadRoomIndex(1, 4);
	} else if (choice == 1) { // "This trial is a mockery"
		showText(TX_SPEAKER_VLICT,        TX_TRI0_048);
		showText(TX_SPEAKER_QUETZECOATL,  TX_TRI0_041);
		showText(TX_SPEAKER_VLICT,        TX_TRI0_050);
		showText(TX_SPEAKER_QUETZECOATL,  TX_TRI0_039);
		showText(TX_SPEAKER_VLICT,        TX_TRI0_059);
		showText(TX_SPEAKER_QUETZECOATL,  TX_TRI0_040);
		showText(TX_SPEAKER_VLICT,        TX_TRI0_046);

		const TextRef choices3[] = {
			TX_SPEAKER_KIRK,
			TX_TRI0_023, TX_TRI0_006, TX_TRI0_010,
		    TX_END
		};
		choice = showMultipleTexts(choices3);

		if (choice == 0 || choice == 1) {
			showText(TX_SPEAKER_VLICT, TX_TRI0_052);

			const TextRef choices4[] = {
				TX_SPEAKER_KIRK,
				TX_TRI0_017, TX_TRI0_020, TX_TRI0_008,
			    TX_END
			};
			choice = showMultipleTexts(choices4);

			if (choice == 0 || choice == 1) {
				showText(TX_SPEAKER_VLICT, TX_TRI0_057);
				showText(TX_SPEAKER_KIRK,  TX_TRI0_003);
				showText(TX_SPEAKER_VLICT, TX_TRI0_051);

				const TextRef choices5[] = {
					TX_SPEAKER_KIRK,
					TX_TRI0_009, TX_TRI0_014, TX_TRI0_013,
				    TX_END
				};
				choice = showMultipleTexts(choices5);

				if (choice == 0 || choice == 1) {
					showText(TX_SPEAKER_VLICT, TX_TRI0_055);
					_awayMission->disableInput = true;
					loadRoomIndex(1, 4);
				} else { // choice == 2
					showText(TX_SPEAKER_VLICT, TX_TRI0_045);
					_awayMission->disableInput = true;
					loadRoomIndex(1, 4);
				}
			} else // choice == 2
				gaveUp = true;
		} else // choice == 2
			gaveUp = true;
	} else // choice == 2
		gaveUp = true;

	if (gaveUp) {
		playMidiMusicTracks(MIDITRACK_2);
		showText(TX_SPEAKER_VLICT, TX_TRI0_049);
		showText(TX_SPEAKER_VLICT, TX_TRI0_044);
		_awayMission->trial.missionEndMethod = 0;

		endMission(_awayMission->trial.missionScore, _awayMission->trial.field2b, 0);
	}
}

void Room::trial0LookAtWindow() {
	showDescription(TX_TRI0N007);
}

void Room::trial0LookAtVlict() {
	showDescription(TX_TRI0N001);
}

void Room::trial0LookAtGuard() {
	showDescription(TX_TRI0N000);
}

void Room::trial0LookAtBench() {
	showDescription(TX_TRI0N006);
}

void Room::trial0LookAtSeal() {
	showDescription(TX_TRI0N008);
}

void Room::trial0LookAtKirk() {
	showDescription(TX_TRI0N004);
}

void Room::trial0LookAtSpock() {
	showDescription(TX_TRI0N005);
}

void Room::trial0LookAtMccoy() {
	showDescription(TX_TRI0N002);
}

void Room::trial0LookAtRedshirt() {
	showDescription(TX_TRI0N003);
}

void Room::trial0LookAtQuetzecoatl() {
	showDescription(TX_TRI0N009);
}

void Room::trial0TalkToVlict() {
	showText(TX_SPEAKER_VLICT, TX_TRI0_062);
}

void Room::trial0TalkToGuard() {
	showText(TX_SPEAKER_KLINGON_GUARD, TX_TRI0_F24);
	showText(TX_SPEAKER_BENNIE, TX_TRI0_065);
	showText(TX_SPEAKER_KIRK,   TX_TRI0_007);
	showText(TX_SPEAKER_BENNIE, TX_TRI0_064);
	showText(TX_SPEAKER_KIRK,   TX_TRI0_022);
	showText(TX_SPEAKER_BENNIE, TX_TRI0_066);
}

void Room::trial0TalkToQuetzecoatl() {
	showText(TX_SPEAKER_QUETZECOATL, TX_TRI0_042);
}

void Room::trial0TalkToMccoy() {
	showText(TX_SPEAKER_MCCOY, TX_TRI0_031);
}

void Room::trial0TalkToSpock() {
	showText(TX_SPEAKER_SPOCK, TX_TRI0_025);
}

void Room::trial0TalkToRedshirt() {
	showText(TX_SPEAKER_BENNIE, TX_TRI0_063);
}

void Room::trial0UsePhaserOrRock() {
	showText(TX_SPEAKER_SPOCK, TX_TRI0_026);
	showText(TX_SPEAKER_MCCOY, TX_TRI0_036);
}

void Room::trial0UseSnakeOnVlict() {
	// Unused, since the snake item doesn't carry over to the courtroom
	showText(TX_SPEAKER_VLICT, TX_TRI4_076);
}

void Room::trial0UseSnakeOnGuard() {
	// Unused, since the snake item doesn't carry over to the courtroom
	showText(TX_SPEAKER_KLINGON_GUARD, TX_TRI0_F24);
}

void Room::trial0UseMTricorderOnQuetzecoatl() {
	mccoyScan(DIR_S, TX_SPEAKER_MCCOY, TX_TRI0_029, true);
}

void Room::trial0UseMTricorderOnVlict() {
	mccoyScan(DIR_S, TX_SPEAKER_MCCOY, TX_TRI0_030, true);
}

void Room::trial0UseMTricorderAnywhere() {
	mccoyScan(DIR_S, TX_SPEAKER_MCCOY, TX_TRI0_035, true);
}

void Room::trial0UseSTricorderOnWindow() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_TRI0_037, true);
}

void Room::trial0UseSTricorderAnywhere() {
	// ENHANCEMENT: Originally didn't play tricorder sound, etc
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_TRI0_038, true);
}

void Room::trial0UseCommunicator() {
	showText(TX_SPEAKER_UHURA, TX_TRI0U075);
}

void Room::trial0UseCrewmanOnKlingon() {
	showText(TX_SPEAKER_SPOCK, TX_TRI0_024);
}

void Room::trial0UseMccoyOnVlict() {
	showText(TX_SPEAKER_MCCOY, TX_TRI0_027);
}

void Room::trial0UseMccoyOnGuard() {
	showText(TX_SPEAKER_MCCOY, TX_TRI0_028);
}

void Room::trial0UseMedkitAnywhere() {
	showText(TX_SPEAKER_MCCOY, TX_TRI0_034);
}

}
