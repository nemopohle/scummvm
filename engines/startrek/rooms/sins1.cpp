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
#define OBJECT_9 9

#define HOTSPOT_DOOR 0x20
#define HOTSPOT_KEYPAD 0x21
#define HOTSPOT_PANEL 0x22
#define HOTSPOT_LENS 0x23
#define HOTSPOT_24 0x24 // Unused hotspot; just a sliver on the right side of the door?

namespace StarTrek {

extern const RoomAction sins1ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::sins1Tick1 },
	{ {ACTION_LOOK, 0xff, 0, 0, 0}, &Room::sins1LookAnywhere },
	{ {ACTION_LOOK, OBJECT_DOOR,    0, 0, 0}, &Room::sins1LookAtLock },
	{ {ACTION_LOOK, HOTSPOT_DOOR,   0, 0, 0}, &Room::sins1LookAtDoor },
	{ {ACTION_LOOK, HOTSPOT_PANEL,  0, 0, 0}, &Room::sins1LookAtPanel },
	{ {ACTION_LOOK, HOTSPOT_KEYPAD, 0, 0, 0}, &Room::sins1LookAtKeypad },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_PANEL,  0, 0}, &Room::sins1UseSTricorderOnPanel },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_KEYPAD, 0, 0}, &Room::sins1UseSTricorderOnKeypad },
	{ {ACTION_USE, OBJECT_ISTRICOR, OBJECT_DOOR,    0, 0}, &Room::sins1UseSTricorderOnLock },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_DOOR,   0, 0}, &Room::sins1UseSTricorderOnDoor },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0xff,           0, 0}, &Room::sins1UseSTricorderAnywhere },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_LENS,   0, 0}, &Room::sins1UseSTricorderOnLens },
	{ {ACTION_USE, OBJECT_IS8ROCKS, HOTSPOT_DOOR,   0, 0}, &Room::sins1UseRockOnDoor },

	{ {ACTION_USE, OBJECT_REDSHIRT, HOTSPOT_KEYPAD, 0, 0}, &Room::sins1UseRedshirtOnKeypad },
	{ {ACTION_USE, OBJECT_MCCOY,    HOTSPOT_KEYPAD, 0, 0}, &Room::sins1UseMccoyOnKeypad },
	{ {ACTION_USE, OBJECT_KIRK,     HOTSPOT_KEYPAD, 0, 0}, &Room::sins1UseKirkOnKeypad },
	{ {ACTION_USE, OBJECT_SPOCK,    HOTSPOT_KEYPAD, 0, 0}, &Room::sins1UseSpockOnKeypad },
	{ {ACTION_DONE_WALK, 6,                      0, 0, 0}, &Room::sins1SpockReachedKeypad },
	{ {ACTION_DONE_ANIM, 8,                      0, 0, 0}, &Room::sins1EnteredCorrectCode },
	{ {ACTION_DONE_ANIM, 1,                      0, 0, 0}, &Room::sins1DoorDoneOpening },
	{ {ACTION_DONE_ANIM, 7,                      0, 0, 0}, &Room::sins1EnteredIncorrectCode },
	{ {ACTION_DONE_ANIM, 9,                      0, 0, 0}, &Room::sins1EnteredSacredSofNumber },
	{ {ACTION_USE, OBJECT_IPHASERS, HOTSPOT_DOOR,   0, 0}, &Room::sins1UsePhaserOnDoor },
	{ {ACTION_USE, OBJECT_IPHASERK, HOTSPOT_DOOR,   0, 0}, &Room::sins1UsePhaserOnDoor },
	{ {ACTION_DONE_ANIM, 2,                      0, 0, 0}, &Room::sins1DoorUsedLaser },

	{ {ACTION_TICK, 40,              0, 0, 0}, &Room::sins1Tick40 },
	{ {ACTION_TALK, OBJECT_KIRK,     0, 0, 0}, &Room::sins1TalkToKirk },
	{ {ACTION_TALK, OBJECT_SPOCK,    0, 0, 0}, &Room::sins1TalkToSpock },
	{ {ACTION_TALK, OBJECT_MCCOY,    0, 0, 0}, &Room::sins1TalkToMccoy },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::sins1TalkToRedshirt },
	{ {ACTION_LOOK, OBJECT_KIRK,     0, 0, 0}, &Room::sins1LookAtKirk },
	{ {ACTION_LOOK, OBJECT_SPOCK,    0, 0, 0}, &Room::sins1LookAtSpock },
	{ {ACTION_LOOK, OBJECT_MCCOY,    0, 0, 0}, &Room::sins1LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::sins1LookAtRedshirt },
	{ {ACTION_LOOK, HOTSPOT_LENS,    0, 0, 0}, &Room::sins1LookAtLens },

	{ {ACTION_USE, OBJECT_ICOMM,    0xff,            0, 0}, &Room::sins1UseCommunicator },
	{ {ACTION_USE, OBJECT_IMEDKIT,  OBJECT_KIRK,     0, 0}, &Room::sins1UseMedkitOnCrewman },
	{ {ACTION_USE, OBJECT_IMEDKIT,  OBJECT_SPOCK,    0, 0}, &Room::sins1UseMedkitOnCrewman },
	{ {ACTION_USE, OBJECT_IMEDKIT,  OBJECT_MCCOY,    0, 0}, &Room::sins1UseMedkitOnCrewman },
	{ {ACTION_USE, OBJECT_IMEDKIT,  OBJECT_REDSHIRT, 0, 0}, &Room::sins1UseMedkitOnCrewman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_KIRK,     0, 0}, &Room::sins1UseMTricorderOnCrewman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_SPOCK,    0, 0}, &Room::sins1UseMTricorderOnCrewman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_MCCOY,    0, 0}, &Room::sins1UseMTricorderOnCrewman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_REDSHIRT, 0, 0}, &Room::sins1UseMTricorderOnCrewman },

	{ {ACTION_WALK, HOTSPOT_DOOR, 0, 0, 0}, &Room::sins1WalkToDoor },
	{ {ACTION_WALK, OBJECT_DOOR,  0, 0, 0}, &Room::sins1WalkToDoor },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum sins1TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_MOSHER,
	TX_SPEAKER_SCOTT, TX_SPEAKER_UHURA,
	TX_SIN1_001, TX_SIN1_002, TX_SIN1_003, TX_SIN1_004, TX_SIN1_005,
	TX_SIN1_006, TX_SIN1_007, TX_SIN1_008, TX_SIN1_009, TX_SIN1_010,
	TX_SIN1_011, TX_SIN1_012, TX_SIN1_013, TX_SIN1_014, TX_SIN1_015,
	TX_SIN1_016, TX_SIN1_017, TX_SIN1_018, TX_SIN1_019, TX_SIN1_020,
	TX_SIN1_021, TX_SIN1_022, TX_SIN1_023, TX_SIN1_024, TX_SIN1_025,
	TX_SIN1_026, TX_SIN1_027, TX_SIN1_028, TX_SIN1_029, TX_SIN1_030,
	TX_SIN1_S10, TX_SIN1_S17, TX_SIN1_S18, TX_SIN1_S20, TX_SIN1_S51,
	TX_SIN1_S85, TX_SIN1U070, TX_SIN1N000, TX_SIN1N001, TX_SIN1N002,
	TX_SIN1N003, TX_SIN1N004, TX_SIN1N005, TX_SIN1N006, TX_SIN1N007,
	TX_SIN1N008, TX_SIN1N009, TX_SIN1N010, TX_SIN1N011
};

extern const RoomTextOffsets sins1TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 301, 301, 301, 301, 301, 301, 301 },
	{ TX_SPEAKER_MCCOY, 312, 312, 312, 312, 312, 314, 312 },
	{ TX_SPEAKER_SPOCK, 322, 322, 322, 322, 322, 324, 322 },
	{ TX_SPEAKER_MOSHER, 332, 332, 332, 332, 332, 334, 332 },
	{ TX_SPEAKER_SCOTT, 356, 356, 356, 356, 356, 358, 358 },
	{ TX_SPEAKER_UHURA, 346, 346, 346, 346, 346, 348, 348 },
	{ TX_SIN1_001, 2352, 2085, 2454, 2408, 2086, 2140, 2186 },
	{ TX_SIN1_002, 5887, 5149, 6370, 6098, 5173, 5420, 5643 },
	{ TX_SIN1_003, 4101, 3619, 4357, 4246, 3630, 3760, 3876 },
	{ TX_SIN1_004, 6100, 5332, 6602, 6327, 5357, 5619, 5845 },
	{ TX_SIN1_005, 3891, 3439, 4114, 4019, 3450, 3567, 3663 },
	{ TX_SIN1_006, 4283, 3771, 4542, 4440, 3782, 3924, 4031 },
	{ TX_SIN1_007, 3659, 3233, 3848, 3773, 3248, 3351, 3427 },
	{ TX_SIN1_008, 5648, 4940, 6093, 5875, 4964, 5217, 5397 },
	{ TX_SIN1_009, 1571, 1423, 1620, 1593, 1426, 1447, 1477 },
	{ TX_SIN1_010, 1341, 1223, 1369, 1369, 1226, 1253, 1256 },
	{ TX_SIN1_011, 2860, 2505, 2974, 2937, 2506, 2580, 2617 },
	{ TX_SIN1_012, 2268, 2016, 2368, 2318, 2017, 2065, 2115 },
	{ TX_SIN1_013, 6436, 5623, 7004, 6682, 5649, 5929, 6201 },
	{ TX_SIN1_014, 6257, 5459, 6788, 6495, 5485, 5757, 6000 },
	{ TX_SIN1_015, 4577, 4020, 4877, 4739, 4031, 4201, 4319 },
	{ TX_SIN1_016, 3301, 2920, 3455, 3398, 2922, 3010, 3068 },
	{ TX_SIN1_017, 3034, 2668, 3161, 3121, 2669, 2748, 2789 },
	{ TX_SIN1_018, 3428, 3032, 3591, 3533, 3035, 3129, 3188 },
	{ TX_SIN1_019, 1682, 1519, 1745, 1713, 1522, 1549, 1587 },
	{ TX_SIN1_020, 1162, 1061, 1183, 1193, 1062, 1095, 1085 },
	{ TX_SIN1_021, 1454, 1321, 1496, 1476, 1324, 1345, 1368 },
	{ TX_SIN1_022, 1900, 1707, 1954, 1957, 1710, 1749, 1763 },
	{ TX_SIN1_023, 1980, 1773, 2036, 2016, 1775, 1808, 1829 },
	{ TX_SIN1_024, 1801, 1623, 1854, 1837, 1626, 1658, 1681 },
	{ TX_SIN1_025, 4455, 3913, 4721, 4610, 3924, 4087, 4179 },
	{ TX_SIN1_026, 4813, 4225, 5138, 4993, 4237, 4444, 4550 },
	{ TX_SIN1_027, 4365, 3838, 4633, 4525, 3849, 0, 4107 },
	{ TX_SIN1_028, 2195, 1958, 2275, 2240, 1959, 2002, 2037 },
	{ TX_SIN1_029, 4970, 4367, 5336, 5172, 4379, 4607, 4733 },
	{ TX_SIN1_030, 4713, 4140, 5029, 4873, 4152, 4316, 4456 },
	{ TX_SIN1_S10, 3583, 3172, 3760, 3687, 3187, 3280, 3354 },
	{ TX_SIN1_S17, 4180, 3683, 4431, 4330, 3694, 3829, 3935 },
	{ TX_SIN1_S18, 3706, 3265, 3907, 3822, 3280, 3385, 3471 },
	{ TX_SIN1_S20, 6016, 5263, 6514, 6231, 5287, 5538, 5772 },
	{ TX_SIN1_S51, 5702, 4979, 6147, 5929, 5003, 5268, 5436 },
	{ TX_SIN1_S85, 3974, 3507, 4219, 4111, 3518, 3644, 3753 },
	{ TX_SIN1U070, 6169, 5386, 6691, 6402, 5411, 5679, 5919 },
	{ TX_SIN1N000, 5548, 4855, 5974, 5773, 4867, 5118, 5281 },
	{ TX_SIN1N001, 738, 712, 738, 738, 713, 715, 715 },
	{ TX_SIN1N002, 5366, 4703, 5759, 5588, 4715, 4963, 5096 },
	{ TX_SIN1N003, 1014, 943, 1032, 1042, 944, 974, 964 },
	{ TX_SIN1N004, 5131, 4498, 5499, 5343, 4510, 4748, 4866 },
	{ TX_SIN1N005, 5023, 4405, 5391, 5226, 4417, 4646, 4773 },
	{ TX_SIN1N006, 5266, 4618, 5634, 5488, 4630, 4878, 4986 },
	{ TX_SIN1N007, 2065, 1843, 2123, 2096, 1844, 1873, 1900 },
	{ TX_SIN1N008, 5477, 4799, 5903, 5703, 4811, 5063, 5225 },
	{ TX_SIN1N009, 813, 772, 821, 818, 773, 780, 783 },
	{ TX_SIN1N010, 893, 837, 900, 905, 838, 852, 847 },
	{ TX_SIN1N011, 1089, 1003, 1109, 1108, 1004, 1025, 1026 },
	{          -1, 0,    0, 0, 0, 0, 0, 0 }
};

extern const RoomText sins1Texts[] = {
	  { -1, Common::UNK_LANG, "" }
};

void Room::sins1Tick1() {
	playVoc("SIN1LOOP");

	if (!_awayMission->sins.scottyInformedKirkAboutVirus)
		// BUGFIX: Set this to 2, not 1 (disable input even after walking animation is done)
		_awayMission->disableInput = 2;

	if (_awayMission->sins.openedOuterDoor) {
		loadActorAnim2(OBJECT_DOOR, "s1dro", 0, 0);
		loadMapFile("sins12");
	} else
		loadMapFile("sins1");

	if (_awayMission->sins.openedInnerDoor)
		loadActorAnim2(OBJECT_DOOR, "s1ndro", 0, 0);

	_awayMission->sins.field32 = 1;
	_awayMission->sins.field31 = 1;

	playMidiMusicTracks(MIDITRACK_27, kLoopTypeRepeat);

	if (!_awayMission->sins.enteredRoom1FirstTime) {
		playMidiMusicTracks(MIDITRACK_1, kLoopTypeLast);
		_awayMission->sins.enteredRoom1FirstTime = true;
	}
}

void Room::sins1LookAnywhere() {
	showDescription(TX_SIN1N001);
}

void Room::sins1LookAtLock() {
	showDescription(TX_SIN1N009);
}

void Room::sins1LookAtDoor() {
	// NOTE: This function had two implementations (one unused).
	showDescription(TX_SIN1N010);
}

void Room::sins1LookAtPanel() {
	showDescription(TX_SIN1N003);
}

void Room::sins1LookAtKeypad() {
	showDescription(TX_SIN1N011);
}

void Room::sins1UseSTricorderOnPanel() {
	spockScan(DIR_N, TX_SPEAKER_SPOCK, TX_SIN1_010);
}

void Room::sins1UseSTricorderOnKeypad() {
	spockScan(DIR_N, TX_SPEAKER_SPOCK, TX_SIN1_021);
}

void Room::sins1UseSTricorderOnLock() {
	spockScan(DIR_N, TX_SPEAKER_SPOCK, TX_SIN1_009);
}

void Room::sins1UseSTricorderOnDoor() {
	if (!_awayMission->sins.openedOuterDoor)
		spockScan(DIR_N, TX_SPEAKER_SPOCK, TX_SIN1_019);
}

void Room::sins1UseSTricorderAnywhere() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_SIN1_024);
}

void Room::sins1UseSTricorderOnLens() {
	if (!_awayMission->sins.doorLaserFiredOnce)
		spockScan(DIR_N, TX_SPEAKER_SPOCK, TX_SIN1_022);
	else // BUGFIX: Original didn't do the tricorder animation, etc. in this case
		spockScan(DIR_N, TX_SPEAKER_SPOCK, TX_SIN1_023);
}

void Room::sins1UseRockOnDoor() {
	showDescription(TX_SIN1N007);
}

void Room::sins1UseRedshirtOnKeypad() {
	if (!_awayMission->sins.openedOuterDoor)
		showText(TX_SPEAKER_MOSHER, TX_SIN1_028);
}

void Room::sins1UseMccoyOnKeypad() {
	if (!_awayMission->sins.openedOuterDoor)
		showText(TX_SPEAKER_MCCOY, TX_SIN1_012);
}

void Room::sins1UseKirkOnKeypad() {
	if (!_awayMission->sins.openedOuterDoor) {
		showText(TX_SPEAKER_KIRK, TX_SIN1_001);
		sins1UseSpockOnKeypad();
	}
}

void Room::sins1UseSpockOnKeypad() {
	if (!_awayMission->sins.openedOuterDoor) {
		_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_N;
		walkCrewmanC(OBJECT_SPOCK, 0xfd, 0xad, &Room::sins1SpockReachedKeypad);
	}
}

void Room::sins1SpockReachedKeypad() {
	const char * const codes[] = {
		"01210", "1210", "10200", nullptr
	};
	int ans = showCodeInputBox(codes);

	if (ans == -1) {
		// ENHANCEMENT: Do nothing if no code was entered.
		_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_W;
		walkCrewman(OBJECT_SPOCK, 0xf3, 0xad);
	} else if (ans == 1 || ans == 2) {
		playVoc("EFX14S");
		loadActorAnimC(OBJECT_SPOCK, "susehn", -1, -1, &Room::sins1EnteredSacredSofNumber);
	} else if (ans == 3) {
		playVoc("EFX14S");
		loadActorAnimC(OBJECT_SPOCK, "susehn", -1, -1, &Room::sins1EnteredCorrectCode);
	} else {
		playVoc("EFX14S");
		loadActorAnimC(OBJECT_SPOCK, "susehn", -1, -1, &Room::sins1EnteredIncorrectCode);
	}
}

void Room::sins1EnteredCorrectCode() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_W;
	walkCrewman(OBJECT_SPOCK, 0xf3, 0xad);
	_awayMission->sins.openedOuterDoor = true;
	loadMapFile("sins12");
	loadActorAnimC(OBJECT_DOOR, "s1door", 0, 0, &Room::sins1DoorDoneOpening);
	playVoc("HUGEDOO2");
}

void Room::sins1DoorDoneOpening() {
	showText(TX_SPEAKER_SPOCK, TX_SIN1_011);
}

void Room::sins1EnteredIncorrectCode() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_W;
	walkCrewman(OBJECT_SPOCK, 0xf3, 0xad);
	showText(TX_SPEAKER_SPOCK, TX_SIN1_017);
}

void Room::sins1EnteredSacredSofNumber() {
	// Entered a sacred Sof number, which is wrong since this is a Lucr base
	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_W;
	walkCrewman(OBJECT_SPOCK, 0xf3, 0xad);
	loadActorAnimC(OBJECT_9, "s1ph1", 0, 0, &Room::sins1DoorUsedLaser);
	playSoundEffectIndex(kSfxPhaser);
}

void Room::sins1UsePhaserOnDoor() {
	showText(TX_SPEAKER_SPOCK, TX_SIN1_016);
}

void Room::sins1DoorUsedLaser() {
	loadActorStandAnim(OBJECT_9);
	showText(TX_SPEAKER_SPOCK, TX_SIN1_018);

	_awayMission->sins.field33 = true;
	_awayMission->sins.doorLaserFiredOnce = true;
}

void Room::sins1Tick40() {
	if (!_awayMission->sins.scottyInformedKirkAboutVirus) {
		showText(TX_SPEAKER_SCOTT, TX_SIN1_S10);
		showText(TX_SPEAKER_KIRK,  TX_SIN1_007);
		showText(TX_SPEAKER_SCOTT, TX_SIN1_S18);
		showText(TX_SPEAKER_KIRK,  TX_SIN1_005);
		showText(TX_SPEAKER_SCOTT, TX_SIN1_S85);
		showText(TX_SPEAKER_KIRK,  TX_SIN1_003);
		showText(TX_SPEAKER_SCOTT, TX_SIN1_S17);

		_awayMission->sins.scottyInformedKirkAboutVirus = true;
		_awayMission->disableInput = false;
	}
}

void Room::sins1TalkToKirk() {
	showText(TX_SPEAKER_KIRK,  TX_SIN1_006);
	showText(TX_SPEAKER_SPOCK, TX_SIN1_027);
}

void Room::sins1TalkToSpock() {
	// NOTE: This function has two implementations. The first (used) one talks about where
	// the energy source is coming from. The second (unused) one says to "carefully
	// consider what we know about this culture".
	showText(TX_SPEAKER_SPOCK, TX_SIN1_020);
}

void Room::sins1TalkToMccoy() {
	showText(TX_SPEAKER_MCCOY, TX_SIN1_015);
}

void Room::sins1TalkToRedshirt() {
	showText(TX_SPEAKER_MOSHER, TX_SIN1_030);
	showText(TX_SPEAKER_SPOCK,  TX_SIN1_026);
	showText(TX_SPEAKER_MOSHER, TX_SIN1_029);
}

void Room::sins1LookAtKirk() {
	showDescription(TX_SIN1N005);
}

void Room::sins1LookAtSpock() {
	showDescription(TX_SIN1N004);
}

void Room::sins1LookAtMccoy() {
	showDescription(TX_SIN1N006);
}

void Room::sins1LookAtRedshirt() {
	showDescription(TX_SIN1N002);
}

void Room::sins1LookAtLens() {
	showDescription(TX_SIN1N000);
}

void Room::sins1UseCommunicator() {
	if (!_awayMission->sins.enteredRoom2FirstTime) {
		showText(TX_SPEAKER_KIRK,  TX_SIN1_008);
		showText(TX_SPEAKER_SCOTT, TX_SIN1_S51);
		showText(TX_SPEAKER_KIRK,  TX_SIN1_002);
		showText(TX_SPEAKER_SCOTT, TX_SIN1_S20);
		showText(TX_SPEAKER_KIRK,  TX_SIN1_004);
	} else
		showText(TX_SPEAKER_UHURA, TX_SIN1U070);
}

void Room::sins1UseMedkitOnCrewman() {
	showText(TX_SPEAKER_MCCOY, TX_SIN1_014);
}

void Room::sins1UseMTricorderOnCrewman() {
	mccoyScan(DIR_S, TX_SPEAKER_SPOCK, TX_SIN1_013);
}

void Room::sins1WalkToDoor() {
	if (_awayMission->sins.openedOuterDoor)
		walkCrewman(OBJECT_KIRK, 0x98, 0x9e, 5); // NOTE: Callback 5 not defined
}

}
