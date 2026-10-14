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

#define HOTSPOT_DOOR 0x20
#define HOTSPOT_GROUND 0x21
#define HOTSPOT_STATUE 0x22
#define HOTSPOT_PLANET 0x23
#define HOTSPOT_SKY 0x24 // Not mapped anywhere?

namespace StarTrek {

extern const RoomAction sins0ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::sins0Tick1 },

	{ {ACTION_LOOK, 0xff,            0, 0, 0}, &Room::sins0LookAnywhere },
	{ {ACTION_LOOK, OBJECT_DOOR,     0, 0, 0}, &Room::sins0LookAtOpenDoor },
	{ {ACTION_LOOK, HOTSPOT_DOOR,    0, 0, 0}, &Room::sins0LookAtClosedDoor },
	{ {ACTION_TALK, OBJECT_SPOCK,    0, 0, 0}, &Room::sins0TalkToSpock },
	{ {ACTION_TALK, OBJECT_MCCOY,    0, 0, 0}, &Room::sins0TalkToMccoy },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::sins0TalkToRedshirt },
	{ {ACTION_LOOK, HOTSPOT_GROUND,  0, 0, 0}, &Room::sins0LookAtGround },
	{ {ACTION_GET, HOTSPOT_GROUND,   0, 0, 0}, &Room::sins0GetRock },
	{ {ACTION_DONE_ANIM, 1,          0, 0, 0}, &Room::sins0PickedUpRock },

	{ {ACTION_USE, OBJECT_ISTRICOR,           0xff, 0, 0}, &Room::sins0UseSTricorderAnywhere },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_GROUND, 0, 0}, &Room::sins0UseSTricorderOnGround },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_PLANET, 0, 0}, &Room::sins0UseSTricorderOnPlanet },

	{ {ACTION_TICK, 40,              0, 0, 0}, &Room::sins0Tick40 },
	{ {ACTION_LOOK, HOTSPOT_STATUE,  0, 0, 0}, &Room::sins0LookAtStatue },
	{ {ACTION_LOOK, HOTSPOT_PLANET,  0, 0, 0}, &Room::sins0LookAtPlanet },
	{ {ACTION_LOOK, HOTSPOT_SKY,     0, 0, 0}, &Room::sins0LookAtSky },
	{ {ACTION_LOOK, OBJECT_KIRK,     0, 0, 0}, &Room::sins0LookAtKirk },
	{ {ACTION_LOOK, OBJECT_SPOCK,    0, 0, 0}, &Room::sins0LookAtSpock },
	{ {ACTION_LOOK, OBJECT_MCCOY,    0, 0, 0}, &Room::sins0LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::sins0LookAtRedshirt },

	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_DOOR,    0, 0}, &Room::sins0UseSTricorderOnClosedDoor },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_STATUE,  0, 0}, &Room::sins0UseSTricorderOnStatue },
	{ {ACTION_USE, OBJECT_IMEDKIT,  OBJECT_KIRK,     0, 0}, &Room::sins0UseMedkitOnCrewman },
	{ {ACTION_USE, OBJECT_IMEDKIT,  OBJECT_SPOCK,    0, 0}, &Room::sins0UseMedkitOnCrewman },
	{ {ACTION_USE, OBJECT_IMEDKIT,  OBJECT_MCCOY,    0, 0}, &Room::sins0UseMedkitOnCrewman },
	{ {ACTION_USE, OBJECT_IMEDKIT,  OBJECT_REDSHIRT, 0, 0}, &Room::sins0UseMedkitOnCrewman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_KIRK,     0, 0}, &Room::sins0UseMTricorderOnCrewman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_SPOCK,    0, 0}, &Room::sins0UseMTricorderOnCrewman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_MCCOY,    0, 0}, &Room::sins0UseMTricorderOnCrewman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_REDSHIRT, 0, 0}, &Room::sins0UseMTricorderOnCrewman },

	{ {ACTION_USE, OBJECT_ICOMM, 0xff, 0, 0}, &Room::sins0UseCommunicator },
	{ {ACTION_WALK, HOTSPOT_DOOR,   0, 0, 0}, &Room::sins0WalkToDoor },
	{ {ACTION_WALK, OBJECT_DOOR,    0, 0, 0}, &Room::sins0WalkToDoor },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum sins0TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_SCOTT, TX_SPEAKER_UHURA,
	TX_SPEAKER_MOSHER,
	TX_SIN0_001, TX_SIN0_002, TX_SIN0_003, TX_SIN0_004, TX_SIN0_005,
	TX_SIN0_006, TX_SIN0_007, TX_SIN0_008, TX_SIN0_009, TX_SIN0_010,
	TX_SIN0_011, TX_SIN0_012, TX_SIN0_013, TX_SIN0_014, TX_SIN0_015,
	TX_SIN0_016, TX_SIN0_017, TX_SIN0_018, TX_SIN0_019, TX_SIN0_020,
	TX_SIN0_021, TX_SIN0_022, TX_SIN0_023, TX_SIN0_024, TX_SIN0_025,
	TX_SIN0_026, TX_SIN0_027, TX_SIN0_028, TX_SIN0_S01, TX_SIN0_S48,
	TX_SIN0_S49, TX_SIN0_S50, TX_SIN0N000, TX_SIN0N001, TX_SIN0N002,
	TX_SIN0N003, TX_SIN0N004, TX_SIN0N005, TX_SIN0N006, TX_SIN0N007,
	TX_SIN0N008, TX_SIN0N009, TX_SIN0N010, TX_SIN0N011, TX_SIN0U069,
	TX_SIN0U082
};

extern const RoomTextOffsets sins0TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 289, 289, 289, 289, 289, 289, 289, 289, 289 },
	{ TX_SPEAKER_MCCOY, 300, 300, 300, 300, 300, 302, 300, 300, 300 },
	{ TX_SPEAKER_SPOCK, 310, 310, 310, 310, 310, 312, 310, 310, 310 },
	{ TX_SPEAKER_SCOTT, 344, 344, 344, 344, 344, 346, 346, 344, 344 },
	{ TX_SPEAKER_UHURA, 334, 334, 334, 334, 334, 336, 336, 334, 334 },
	{ TX_SPEAKER_MOSHER, 320, 320, 320, 320, 320, 322, 320, 320, 320 },
	{ TX_SIN0N000, 3805, 3312, 4130, 4036, 3326, 3559, 3649, 3326, 3801 },
	{ TX_SIN0N001, 3994, 3486, 4311, 4226, 3500, 3734, 3815, 3500, 3990 },
	{ TX_SIN0N002, 3594, 3131, 3920, 3811, 3145, 3366, 3469, 3145, 3590 },
	{ TX_SIN0N003, 3694, 3216, 4016, 3914, 3230, 3454, 3550, 3230, 3690 },
	{ TX_SIN0N004, 3461, 3013, 3789, 3686, 3027, 3257, 3353, 3027, 3457 },
	{ TX_SIN0N005, 729, 689, 729, 736, 688, 696, 690, 688, 725 },
	{ TX_SIN0N006, 1252, 1137, 1297, 1286, 1136, 1169, 1181, 1136, 1248 },
	{ TX_SIN0N007, 3341, 2908, 3665, 3552, 2922, 3139, 3244, 2922, 3337 },
	{ TX_SIN0N008, 804, 749, 814, 811, 748, 756, 760, 748, 800 },
	{ TX_SIN0N009, 648, 622, 648, 648, 622, 624, 624, 622, 644 },
	{ TX_SIN0N010, 1377, 1250, 1427, 1413, 1246, 1281, 1296, 1246, 1373 },
	{ TX_SIN0N011, 2840, 2480, 3032, 2990, 2496, 2644, 2687, 2496, 2836 },
	{ TX_SIN0U069, 5389, 4927, 5886, 5700, 4959, 5026, 5221, 4959, 5386 },
	{ TX_SIN0U082, 1915, 1723, 1996, 1987, 1735, 1808, 1816, 1735, 1911 },
	{ TX_SIN0_001, 2544, 2244, 2713, 2653, 2259, 2368, 2428, 2259, 2540 },
	{ TX_SIN0_002, 5317, 4870, 5801, 5622, 4902, 4963, 5151, 4902, 5314 },
	{ TX_SIN0_003, 1986, 1779, 2064, 2057, 1791, 1863, 1869, 1791, 1982 },
	{ TX_SIN0_004, 5131, 4714, 5606, 5423, 4745, 4807, 4986, 4745, 5128 },
	{ TX_SIN0_005, 2356, 2086, 2506, 2445, 2101, 2192, 2251, 2101, 2352 },
	{ TX_SIN0_006, 2758, 2413, 2954, 2896, 2429, 2565, 2624, 2429, 2754 },
	{ TX_SIN0_007, 2108, 1871, 2195, 2181, 1883, 1957, 1970, 1883, 2104 },
	{ TX_SIN0_008, 4505, 4154, 4870, 4775, 4171, 4208, 4298, 4171, 4502 },
	{ TX_SIN0_009, 924, 854, 944, 946, 853, 875, 874, 853, 920 },
	{ TX_SIN0_010, 4922, 4526, 5354, 5207, 4543, 4595, 4737, 4543, 4919 },
	{ TX_SIN0_011, 1054, 969, 1095, 1071, 968, 984, 1009, 968, 1050 },
	{ TX_SIN0_012, 4742, 4361, 5132, 5017, 4378, 4420, 4530, 4378, 4739 },
	{ TX_SIN0_013, 4362, 3809, 4717, 4606, 3825, 4069, 4175, 3825, 4358 },
	{ TX_SIN0_014, 3214, 2794, 3497, 3406, 2810, 3008, 3090, 2810, 3210 },
	{ TX_SIN0_015, 2896, 2521, 3097, 3054, 2537, 2693, 2737, 2537, 2892 },
	{ TX_SIN0_016, 1750, 1573, 1834, 1802, 1573, 1625, 1657, 1573, 1746 },
	{ TX_SIN0_017, 1613, 1452, 1693, 1671, 1452, 1509, 1532, 1452, 1609 },
	{ TX_SIN0_018, 1476, 1330, 1528, 1515, 1330, 1368, 1382, 1330, 1472 },
	{ TX_SIN0_019, 4132, 3609, 4459, 4363, 3623, 3856, 3948, 3623, 4128 },
	{ TX_SIN0_020, 5506, 3914, 6011, 5822, 3931, 5129, 5327, 3931, 5503 },
	{ TX_SIN0_021, 2978, 2588, 3205, 3135, 2604, 2770, 2829, 2604, 2974 },
	{ TX_SIN0_022, 2715, 2385, 2906, 2843, 2401, 2527, 2591, 2401, 2711 },
	{ TX_SIN0_023, 4595, 4229, 4967, 4865, 4246, 4283, 4380, 4246, 4592 },
	{ TX_SIN0_024, 1197, 1097, 1242, 1221, 1096, 1119, 1141, 1096, 1193 },
	{ TX_SIN0_025, 4233, 3695, 4577, 4470, 3710, 3948, 4050, 3710, 4229 },
	{ TX_SIN0_026, 2634, 2319, 2811, 2755, 2335, 2455, 2511, 2335, 2630 },
	{ TX_SIN0_027, 3086, 2681, 3352, 3266, 2697, 2885, 2960, 2697, 3082 },
	{ TX_SIN0_028, 4421, 3853, 4780, 4682, 3870, 4130, 4223, 3870, 4418 },
	{ TX_SIN0_S01, 5200, 4768, 5687, 5498, 4799, 4863, 5052, 4799, 5197 },
	{ TX_SIN0_S48, 2023, 1801, 2100, 2094, 1813, 1885, 1890, 1813, 2019 },
	{ TX_SIN0_S49, 2430, 2145, 2589, 2517, 2160, 2249, 2319, 2160, 2426 },
	{ TX_SIN0_S50, 2175, 1920, 2269, 2250, 1935, 2012, 2029, 1935, 2171 },
	{          -1, 0,    0, 0, 0, 0, 0, 0, 0, 0 }
};

extern const RoomText sins0Texts[] = {
	  { -1, Common::UNK_LANG, "" }
};

void Room::sins0Tick1() {
	playVoc("SIN0LOOP");

	if (!_awayMission->sins.enteredRoom0FirstTime)
		_awayMission->disableInput = 2;

	if (_awayMission->sins.openedInnerDoor)
		loadActorAnim2(OBJECT_DOOR, "s0dr2", 0, 0);
	else if (_awayMission->sins.openedOuterDoor)
		loadActorAnim2(OBJECT_DOOR, "s0dr1", 0, 0);

	playMidiMusicTracks(MIDITRACK_27, kLoopTypeRepeat);
}

void Room::sins0LookAnywhere() {
	showDescription(TX_SIN0N009);
}

void Room::sins0LookAtOpenDoor() {
	showDescription(TX_SIN0N005);
}

void Room::sins0LookAtClosedDoor() {
	showDescription(TX_SIN0N008);
}

void Room::sins0TalkToSpock() {
	showText(TX_SPEAKER_SPOCK, TX_SIN0_009);
}

void Room::sins0TalkToMccoy() {
	showText(TX_SPEAKER_MCCOY, TX_SIN0_011);
}

void Room::sins0TalkToRedshirt() {
	// ENHANCEMENT: This function had two implementations. The first (the originally used
	// one) is a generic "area is secure" text. In the second (which was unused), he
	// comments on how the small moon could have an atmosphere. This is more interesting
	// and relevant, so that implementation is used instead.
	if (false)
		showText(TX_SPEAKER_MOSHER, TX_SIN0_024);
	else {
		showText(TX_SPEAKER_MOSHER, TX_SIN0_026);
		showText(TX_SPEAKER_SPOCK,  TX_SIN0_022);
		showText(TX_SPEAKER_KIRK,   TX_SIN0_006);
	}
}

void Room::sins0LookAtGround() {
	showDescription(TX_SIN0N006);
}

void Room::sins0GetRock() {
	_awayMission->disableInput = true;
	loadActorAnimC(OBJECT_KIRK, "kpickw", -1, -1, &Room::sins0PickedUpRock);
	_awayMission->sins.gatheredClues |= 1;
}

void Room::sins0PickedUpRock() {
	_awayMission->disableInput = false;
	loadActorStandAnim(OBJECT_KIRK);
	showDescription(TX_SIN0N010);
	giveItem(OBJECT_IS8ROCKS);
}

void Room::sins0UseSTricorderAnywhere() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_SIN0_018);
}

void Room::sins0UseSTricorderOnGround() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_SIN0_017);
	_awayMission->sins.gatheredClues |= 1;
}

void Room::sins0UseSTricorderOnPlanet() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_SIN0_016);
}

void Room::sins0Tick40() {
	if (!_awayMission->sins.enteredRoom0FirstTime) {
		_awayMission->disableInput = false;
		showText(TX_SPEAKER_UHURA, TX_SIN0U082);
		showText(TX_SPEAKER_KIRK,  TX_SIN0_003);
		showText(TX_SPEAKER_SCOTT, TX_SIN0_S48);
		showText(TX_SPEAKER_KIRK,  TX_SIN0_007);
		showText(TX_SPEAKER_SCOTT, TX_SIN0_S50);
		showText(TX_SPEAKER_KIRK,  TX_SIN0_005);
		showText(TX_SPEAKER_SCOTT, TX_SIN0_S49);
		showText(TX_SPEAKER_KIRK,  TX_SIN0_001);
		_awayMission->sins.enteredRoom0FirstTime = true;
	}
}

void Room::sins0LookAtStatue() {
	showDescription(TX_SIN0N011);
	showText(TX_SPEAKER_MCCOY,  TX_SIN0_015);
	showText(TX_SPEAKER_SPOCK,  TX_SIN0_021);
	showText(TX_SPEAKER_MOSHER, TX_SIN0_027);
	showText(TX_SPEAKER_MCCOY,  TX_SIN0_014);
}

void Room::sins0LookAtPlanet() {
	showDescription(TX_SIN0N007);
}

void Room::sins0LookAtSky() {
	// This seems unused, due to HOTSPOT_SKY not being mapped anywhere?
	showDescription(TX_SIN0N004);
}

void Room::sins0LookAtKirk() {
	showDescription(TX_SIN0N002);
}

void Room::sins0LookAtSpock() {
	showDescription(TX_SIN0N003);
}

void Room::sins0LookAtMccoy() {
	showDescription(TX_SIN0N000);
}

void Room::sins0LookAtRedshirt() {
	showDescription(TX_SIN0N001);
}

void Room::sins0UseSTricorderOnClosedDoor() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_SIN0_019);
	showText(TX_SPEAKER_MOSHER, TX_SIN0_025);
	showText(TX_SPEAKER_MCCOY,  TX_SIN0_013);
	showText(TX_SPEAKER_MOSHER, TX_SIN0_028);
}

void Room::sins0UseSTricorderOnStatue() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_SIN0_020);
	showText(TX_SPEAKER_KIRK,  TX_SIN0_008);
	showText(TX_SPEAKER_SPOCK, TX_SIN0_023);

	if (!_awayMission->sins.gotPointsForScanningStatue) {
		_awayMission->sins.gotPointsForScanningStatue = true;
		_awayMission->sins.missionScore += 1;
	}
}

void Room::sins0UseMedkitOnCrewman() {
	showText(TX_SPEAKER_MCCOY, TX_SIN0_012);
}

void Room::sins0UseMTricorderOnCrewman() {
	mccoyScan(DIR_S, TX_SPEAKER_MCCOY, TX_SIN0_010);
}

void Room::sins0UseCommunicator() {
	if (!_awayMission->sins.scottyInformedKirkAboutVirus) {
		showText(TX_SPEAKER_KIRK,  TX_SIN0_004);
		showText(TX_SPEAKER_SCOTT, TX_SIN0_S01);
		showText(TX_SPEAKER_KIRK,  TX_SIN0_002);
	} else
		showText(TX_SPEAKER_UHURA, TX_SIN0U069);
}

void Room::sins0WalkToDoor() {
	walkCrewman(OBJECT_KIRK, 0x5f, 0x8c);
}

}
