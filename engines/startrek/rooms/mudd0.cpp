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

#define OBJECT_MUDD 8
#define OBJECT_ALIENDV 9
#define OBJECT_DOOR 10

#define HOTSPOT_MEMORY_DISK_BOX 0x20
#define HOTSPOT_LENSE 0x21
#define HOTSPOT_DEGRIMER_BOX 0x22
#define HOTSPOT_FOOD_BOX 0x23
#define HOTSPOT_COMPONENT_BOX 0x24
#define HOTSPOT_DOOR 0x25
#define HOTSPOT_BOTTOM_OF_ROOM 0x26

// BUG-ish: Sometimes when looking at Mudd, Kirk walks toward him, but not always. Does
// this happen in original?

namespace StarTrek {

extern const RoomAction mudd0ActionList[] = {
	{ {ACTION_TICK,  1, 0, 0, 0}, &Room::mudd0Tick1 },
	{ {ACTION_TICK, 50, 0, 0, 0}, &Room::mudd0Tick50 },
	{ {ACTION_TICK, 60, 0, 0, 0}, &Room::mudd0Tick60 },
	{ {ACTION_USE, OBJECT_IPHASERS, 8, 0, 0}, &Room::mudd0UsePhaserOnMudd },
	{ {ACTION_USE, OBJECT_IPHASERK, 8, 0, 0}, &Room::mudd0UsePhaserOnMudd },
	{ {ACTION_USE, OBJECT_ICOMM, 0xff, 0, 0}, &Room::mudd0UseCommunicator },
	{ {ACTION_LOOK, 0x23, 0, 0, 0}, &Room::mudd0LookAtFoodBox },
	{ {ACTION_LOOK, 0x24, 0, 0, 0}, &Room::mudd0LookAtComponentBox },
	{ {ACTION_LOOK, 0xff, 0, 0, 0}, &Room::mudd0LookAnywhere },
	{ {ACTION_LOOK, 0x20, 0, 0, 0}, &Room::mudd0LookAtMemoryDiskBox },
	{ {ACTION_LOOK, 0x22, 0, 0, 0}, &Room::mudd0LookAtDegrimerBox },
	{ {ACTION_LOOK, 0x21, 0, 0, 0}, &Room::mudd0LookAtLense },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0xff, 0, 0}, &Room::mudd0UseSTricorderAnywhere },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0xff, 0, 0}, &Room::mudd0UseMTricorderAnywhere },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x20, 0, 0}, &Room::mudd0UseSTricorderOnMemoryDiskBox },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x22, 0, 0}, &Room::mudd0UseSTricorderOnDegrimerBox },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0x21, 0, 0}, &Room::mudd0UseMTricorderOnLense },

	// Common code (next 4 lines)
	{ {ACTION_USE, OBJECT_ILENSES, OBJECT_IDEGRIME, 0, 0}, &Room::muddaUseLenseOnDegrimer },
	{ {ACTION_USE, OBJECT_IALIENDV, 0xff, 0, 0}, &Room::muddaUseAlienDevice },
	{ {ACTION_FINISHED_ANIMATION, 9, 0, 0, 0}, &Room::muddaFiredAlienDevice },
	{ {ACTION_USE, OBJECT_IDEGRIME, 0xff, 0, 0}, &Room::muddaUseDegrimer },

	{ {ACTION_GET, 0x21, 0, 0, 0}, &Room::mudd0GetLense },
	{ {ACTION_GET, 0x20, 0, 0, 0}, &Room::mudd0GetMemoryDisk },
	{ {ACTION_GET, 0x22, 0, 0, 0}, &Room::mudd0GetDegrimer },
	{ {ACTION_TIMER_EXPIRED, 1, 0, 0, 0}, &Room::mudd0PickedUpLense },
	{ {ACTION_FINISHED_ANIMATION, 1, 0, 0, 0}, &Room::mudd0PickedUpItem },
	{ {ACTION_WALK, 0x26, 0, 0, 0}, &Room::mudd0WalkToSouthDoor },
	{ {ACTION_TOUCHED_HOTSPOT, 1, 0, 0, 0}, &Room::mudd0TouchedHotspot1 },
	{ {ACTION_WALK, 0x25, 0, 0, 0}, &Room::mudd0WalkToNorthDoor },
	{ {ACTION_TOUCHED_HOTSPOT, 0, 0, 0, 0}, &Room::mudd0TouchedHotspot0 },
	{ {ACTION_USE, OBJECT_IMEDKIT, 0xff, 0, 0}, &Room::mudd0UseMedkit },
	{ {ACTION_LOOK, OBJECT_KIRK,     0, 0, 0}, &Room::mudd0LookAtKirk },
	{ {ACTION_LOOK, OBJECT_SPOCK,    0, 0, 0}, &Room::mudd0LookAtSpock },
	{ {ACTION_LOOK, OBJECT_MCCOY,    0, 0, 0}, &Room::mudd0LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::mudd0LookAtRedshirt },
	{ {ACTION_LOOK, 8,               0, 0, 0}, &Room::mudd0LookAtMudd },
	{ {ACTION_TALK, OBJECT_KIRK,     0, 0, 0}, &Room::mudd0TalkToKirk },
	{ {ACTION_TALK, OBJECT_SPOCK,    0, 0, 0}, &Room::mudd0TalkToSpock },
	{ {ACTION_TALK, OBJECT_MCCOY,    0, 0, 0}, &Room::mudd0TalkToMccoy },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::mudd0TalkToRedshirt },
	{ {ACTION_TALK, 8,               0, 0, 0}, &Room::mudd0TalkToMudd },

	// Common code (countdown for losing atmosphere when life support malfunctioning)
	{ {ACTION_TICK, 0xff, 0xff, 0xff, 0},           &Room::muddaTick },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum mudd0TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_MUDD,
	TX_SPEAKER_UHURA, TX_SPEAKER_BUCHERT,
	TX_MUD0_001, TX_MUD0_003, TX_MUD0_004, TX_MUD0_005, TX_MUD0_006,
	TX_MUD0_007, TX_MUD0_008, TX_MUD0_009, TX_MUD0_010, TX_MUD0_011,
	TX_MUD0_012, TX_MUD0_013, TX_MUD0_014, TX_MUD0_015, TX_MUD0_016,
	TX_MUD0_017, TX_MUD0_018, TX_MUD0_019, TX_MUD0_020, TX_MUD0_021,
	TX_MUD0_022, TX_MUD0_023, TX_MUD0_024, TX_MUD0_025, TX_MUD0_026,
	TX_MUD0_027, TX_MUD0_028, TX_MUD0_029, TX_MUD0_030, TX_MUD0_031,
	TX_MUD0_032, TX_MUD0_033, TX_MUD0_034, TX_MUD0_035, TX_MUD0_036,
	TX_MUD0_037, TX_MUD0_038, TX_MUD0_039, TX_MUD0_040, TX_MUD0_041,
	TX_MUD0N000, TX_MUD0N001, TX_MUD0N002, TX_MUD0N003, TX_MUD0N004,
	TX_MUD0N005, TX_MUD0N006, TX_MUD0N007, TX_MUD0N008, TX_MUD0N009,
	TX_MUD0N010, TX_MUD0N011, TX_MUD0N012, TX_MUD0N013, TX_MUD0N014,
	TX_MUD0N015, TX_MUD0N016, TX_MUD0N019, TX_LOV1N007,
	TX_STATICU1
};

extern const RoomTextOffsets mudd0TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 2941, 2778, 3028, 2954, 2800, 2796, 2887 },
	{ TX_SPEAKER_MCCOY, 2966, 2803, 3053, 2979, 2825, 2809, 2898 },
	{ TX_SPEAKER_SPOCK, 2976, 2813, 3063, 2989, 2835, 2819, 2908 },
	{ TX_SPEAKER_MUDD, 2999, 2836, 3086, 3012, 2858, 2842, 2931 },
	{ TX_SPEAKER_UHURA, 727, 708, 726, 747, 730, 730, 728 },
	{ TX_SPEAKER_BUCHERT, 2986, 2823, 3073, 2999, 2845, 2829, 2918 },
	{ TX_MUD0_001, 690, 666, 690, 690, 688, 688, 688 },
	{ TX_MUD0_003, 7333, 6710, 7903, 7482, 6726, 6845, 7299 },
	{ TX_MUD0_004, 7962, 7256, 8602, 8196, 7272, 7461, 7908 },
	{ TX_MUD0_005, 4373, 4104, 4631, 4412, 4122, 4136, 4368 },
	{ TX_MUD0_006, 4091, 3853, 4329, 4114, 3870, 3868, 4096 },
	{ TX_MUD0_007, 6698, 6176, 7163, 6754, 6192, 6227, 6664 },
	{ TX_MUD0_008, 8489, 7693, 9204, 8785, 7709, 7991, 8421 },
	{ TX_MUD0_009, 3167, 2989, 3271, 3181, 3011, 2998, 3101 },
	{ TX_MUD0_010, 6572, 6075, 7005, 6613, 6091, 6117, 6537 },
	{ TX_MUD0_011, 1774, 1701, 1772, 1793, 1723, 1722, 1721 },
	{ TX_MUD0_012, 5833, 5441, 6241, 5914, 5457, 5518, 5879 },
	{ TX_MUD0_013, 5700, 5323, 6095, 5817, 5339, 5430, 5748 },
	{ TX_MUD0_014, 7508, 6862, 8134, 7686, 6878, 7017, 7500 },
	{ TX_MUD0_015, 7108, 6526, 7642, 7217, 6542, 6627, 7083 },
	{ TX_MUD0_016, 7243, 6635, 7797, 7380, 6651, 6759, 7208 },
	{ TX_MUD0_017, 3644, 3436, 3789, 3678, 3456, 3464, 3588 },
	{ TX_MUD0_018, 2523, 2405, 2549, 2536, 2427, 2420, 2453 },
	{ TX_MUD0_019, 2473, 2370, 2488, 2487, 2392, 2386, 2407 },
	{ TX_MUD0_020, 2423, 2335, 2427, 2438, 2357, 2352, 2361 },
	{ TX_MUD0_021, 5215, 4841, 5571, 5292, 4857, 4923, 5202 },
	{ TX_MUD0_022, 6751, 6214, 7221, 6808, 6230, 6265, 6707 },
	{ TX_MUD0_023, 6970, 6403, 7476, 7042, 6419, 6468, 6932 },
	{ TX_MUD0_024, 7671, 6995, 8305, 7842, 7011, 7141, 7641 },
	{ TX_MUD0_025, 8568, 7757, 9292, 8883, 7774, 8072, 8494 },
	{ TX_MUD0_026, 3693, 3470, 3850, 3730, 3490, 3501, 3634 },
	{ TX_MUD0_027, 7440, 6809, 8049, 7602, 6825, 6949, 7430 },
	{ TX_MUD0_028, 3258, 3065, 3382, 3284, 3087, 3086, 3197 },
	{ TX_MUD0_029, 4528, 4244, 4810, 4570, 4262, 4283, 4533 },
	{ TX_MUD0_030, 8093, 7357, 8756, 8367, 7373, 7604, 8032 },
	{ TX_MUD0_031, 6649, 6142, 7101, 6701, 6158, 6189, 6618 },
	{ TX_MUD0_032, 7581, 6920, 8210, 7758, 6936, 7072, 7561 },
	{ TX_MUD0_033, 4188, 3934, 4437, 4218, 3952, 3957, 4189 },
	{ TX_MUD0_034, 6861, 6309, 7342, 6932, 6325, 6372, 6813 },
	{ TX_MUD0_035, 7889, 7198, 8535, 8122, 7214, 7403, 7856 },
	{ TX_MUD0_036, 8345, 7579, 9036, 8629, 7595, 7842, 8283 },
	{ TX_MUD0_037, 3064, 2901, 3151, 3077, 2923, 2907, 2996 },
	{ TX_MUD0_038, 7179, 6586, 7721, 7306, 6602, 6700, 7147 },
	{ TX_MUD0_039, 8416, 7635, 9116, 8702, 7651, 7923, 8348 },
	{ TX_MUD0_040, 8007, 7286, 8652, 8252, 7302, 7504, 7943 },
	{ TX_MUD0_041, 8265, 7514, 8943, 8558, 7530, 7782, 8205 },
	{ TX_MUD0N000, 5404, 5072, 5790, 5495, 5088, 5155, 5488 },
	{ TX_MUD0N001, 6253, 5801, 6697, 6326, 5817, 5879, 6274 },
	{ TX_MUD0N002, 5513, 5166, 5894, 5612, 5182, 5257, 5577 },
	{ TX_MUD0N003, 6478, 5996, 6920, 6523, 6012, 6043, 6467 },
	{ TX_MUD0N004, 6117, 5695, 6559, 6197, 5711, 5778, 6167 },
	{ TX_MUD0N005, 6374, 5907, 6827, 6433, 5923, 5970, 6389 },
	{ TX_MUD0N006, 2573, 2440, 2610, 2585, 2462, 2454, 2499 },
	{ TX_MUD0N007, 6183, 5746, 6629, 6256, 5762, 5821, 6221 },
	{ TX_MUD0N008, 5330, 5013, 5718, 5433, 5029, 5108, 5431 },
	{ TX_MUD0N009, 2781, 2633, 2841, 2796, 2655, 2653, 2714 },
	{ TX_MUD0N010, 5139, 4780, 5476, 5207, 4796, 4853, 5122 },
	{ TX_MUD0N011, 6036, 5629, 6465, 6121, 5645, 5717, 6088 },
	{ TX_MUD0N012, 4711, 4412, 5001, 4754, 4429, 4451, 4708 },
	{ TX_MUD0N013, 4816, 4502, 5130, 4872, 4518, 4553, 4821 },
	{ TX_MUD0N014, 5599, 5237, 5992, 5721, 5253, 5351, 5660 },
	{ TX_MUD0N015, 5067, 4723, 5401, 5144, 4739, 4793, 5062 },
	{ TX_MUD0N016, 8799, 7951, 9566, 9150, 7969, 8296, 8723 },
	{ TX_MUD0N019, 4899, 4570, 5209, 4970, 4586, 4636, 4885 },
	{ TX_LOV1N007, 8778, 0, 9528, 9120, 0, 0, 0 },
	{ TX_STATICU1,  737, 718, 736, 757, 740, 740, 738 },
	{          -1, 0,    0, 0, 0, 0, 0, 0 }
};

extern const RoomText mudd0Texts[] = {
	  { -1, Common::UNK_LANG, "" }
};

void Room::mudd0Tick1() {
	playVoc("MUD0LOOP");

	// NOTE: Using the floppy version's code here; Mudd disappears from this room after
	// the first meeting. Fixes an issue where Mudd appears in the first room when he's
	// supposed to be in the medbay.
	// TODO: verify this matches with floppy version's code
	if (_awayMission->mudd.muddFirstRoomState != 0)
		_awayMission->mudd.muddFirstRoomState = 2;
	else {
		_awayMission->disableInput = 2;
		loadActorAnim(OBJECT_MUDD, "s4cbhr", 0xa2, 0xa9, 0);
	}
}

void Room::mudd0Tick50() {
	if (!_awayMission->mudd.enteredRoom0ForFirstTime) {
		playMidiMusicTracks(MIDITRACK_3);
		_awayMission->mudd.enteredRoom0ForFirstTime = true;
	}
}

void Room::mudd0Tick60() {
	if (_awayMission->mudd.muddFirstRoomState == 0) {
		// ENHANCEMENT: Move this into if statement (related to above enhancement)
		_awayMission->disableInput = false;

		_awayMission->mudd.muddFirstRoomState++;

		showText(TX_SPEAKER_MUDD,  TX_MUD0_037);
		showText(TX_SPEAKER_KIRK,  TX_MUD0_009);
		showText(TX_SPEAKER_MUDD,  TX_MUD0_028);
		showText(TX_SPEAKER_MCCOY, TX_MUD0_017);
		showText(TX_SPEAKER_MUDD,  TX_MUD0_026);
		showText(TX_SPEAKER_KIRK,  TX_MUD0_006);
		showText(TX_SPEAKER_MUDD,  TX_MUD0_033);
		showText(TX_SPEAKER_KIRK,  TX_MUD0_005);
		showText(TX_SPEAKER_MUDD,  TX_MUD0_029);
	}
}

void Room::mudd0UsePhaserOnMudd() {
	showText(TX_SPEAKER_MUDD, TX_MUD0_025);
}

void Room::mudd0UseCommunicator() {
	showText(TX_SPEAKER_KIRK,  TX_MUD0_001);
	showText(TX_SPEAKER_UHURA, TX_STATICU1);
}

void Room::mudd0LookAtFoodBox() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_S;
	loadActorStandAnim(OBJECT_KIRK);
	showDescription(TX_MUD0N012);
}

void Room::mudd0LookAtComponentBox() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_S;
	loadActorStandAnim(OBJECT_KIRK);
	showDescription(TX_MUD0N013);
}

void Room::mudd0LookAnywhere() {
	showDescription(TX_MUD0N019);
}

void Room::mudd0LookAtMemoryDiskBox() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_N;
	loadActorStandAnim(OBJECT_KIRK);
	showDescription(TX_MUD0N010);
}

void Room::mudd0LookAtDegrimerBox() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_W;
	loadActorStandAnim(OBJECT_KIRK);
	showDescription(TX_MUD0N008);
}

void Room::mudd0LookAtLense() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_E;
	loadActorStandAnim(OBJECT_KIRK);
	showDescription(TX_MUD0N014);
}

void Room::mudd0UseSTricorderAnywhere() {
	// Narrator says something, not Spock (so we don't use "spockScan" function)
	loadActorAnim2(OBJECT_SPOCK, "sscans", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showDescription(TX_MUD0N015);
}

void Room::mudd0UseMTricorderAnywhere() {
	// Narrator says something, not Mccoy (so we don't use "mccoyScan" function)
	// BUGFIX-ish: original game had "McCoy" as the speaker, which is inconsistent with
	// above.
	loadActorAnim2(OBJECT_MCCOY, "mscans", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showDescription(TX_MUD0N015);
}

void Room::mudd0UseSTricorderOnMemoryDiskBox() {
	spockScan(DIR_N, TX_SPEAKER_SPOCK, TX_MUD0_021, true);
}

void Room::mudd0UseSTricorderOnDegrimerBox() {
	// Narrator says something, not Spock (so we don't use "spockScan" function)
	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_W;
	loadActorAnim2(OBJECT_SPOCK, "sscanw", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showDescription(TX_MUD0N000);
}

void Room::mudd0UseMTricorderOnLense() {
	if (_awayMission->mudd.discoveredLenseAndDegrimerFunction)
		mccoyScan(DIR_E, TX_SPEAKER_MCCOY, TX_MUD0_012, true);
	else
		mccoyScan(DIR_E, TX_SPEAKER_MCCOY, TX_MUD0_013, true);
}

void Room::mudd0GetLense() {
	if (_awayMission->mudd.gotLense)
		showDescription(TX_MUD0N016);
	else {
		_awayMission->mudd.gotLense = true;
		_awayMission->mudd.missionScore++;
		giveItem(OBJECT_ILENSES);
		_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_E;
		loadActorAnim2(OBJECT_KIRK, "s5r1kg", -1, -1);
		_awayMission->timers[1] = 27;
	}
}

void Room::mudd0GetMemoryDisk() {
	if (_awayMission->mudd.gotMemoryDisk)
		showDescription(TX_MUD0N016);
	else {
		_awayMission->mudd.gotMemoryDisk = true;
		_awayMission->mudd.missionScore++;
		giveItem(OBJECT_IDISKS);
		_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_N;
		loadActorAnimC(OBJECT_KIRK, "kuseln", -1, -1, &Room::mudd0PickedUpItem);
	}
}

void Room::mudd0GetDegrimer() {
	if (_awayMission->mudd.gotDegrimer)
		showDescription(TX_MUD0N016);
	else {
		_awayMission->mudd.gotDegrimer = true;
		_awayMission->mudd.missionScore++;
		giveItem(OBJECT_IDEGRIME);
		_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_S;
		loadActorAnimC(OBJECT_KIRK, "kusemw", -1, -1, &Room::mudd0PickedUpItem);
	}
}

// Timer 1 expired
void Room::mudd0PickedUpLense() {
	loadActorStandAnim(OBJECT_KIRK);
	mudd0PickedUpItem();
}

void Room::mudd0PickedUpItem() {
	_awayMission->disableInput = false; // NOTE: this was never set to true
	showDescription(TX_LOV1N007);
}


void Room::mudd0WalkToSouthDoor() {
	_roomVar.mudd.walkingToDoor = true;
	_awayMission->disableInput = true;
	walkCrewman(OBJECT_KIRK, 0x5a, 0xc7);
}

void Room::mudd0TouchedHotspot1() { // Trigger bottom door opening
	if (_roomVar.mudd.walkingToDoor)
		playVoc("SMADOOR3");
}

void Room::mudd0WalkToNorthDoor() {
	_roomVar.mudd.walkingToDoor = true;
	_awayMission->disableInput = true;
	walkCrewman(OBJECT_KIRK, 0xa0, 0x68);
}

void Room::mudd0TouchedHotspot0() { // Trigger top door opening
	if (_roomVar.mudd.walkingToDoor) {
		playVoc("SMADOOR3");
		loadActorAnim2(OBJECT_DOOR, "s4cbdo", 0xa0, 0x6b);
	}
}

void Room::mudd0UseMedkit() {
	showText(TX_SPEAKER_MCCOY, TX_MUD0_011);
}

void Room::mudd0LookAtKirk() {
	showDescription(TX_MUD0N004);
}

void Room::mudd0LookAtSpock() {
	showDescription(TX_MUD0N007);
}

void Room::mudd0LookAtMccoy() {
	showDescription(TX_MUD0N001);
}

void Room::mudd0LookAtRedshirt() {
	showDescription(TX_MUD0N005);
}

void Room::mudd0LookAtMudd() {
	showDescription(TX_MUD0N003);
}

void Room::mudd0TalkToKirk() {
	if (_awayMission->mudd.muddFirstRoomState == 2)
		showText(TX_SPEAKER_KIRK, TX_MUD0_010);
	else {
		showText(TX_SPEAKER_KIRK, TX_MUD0_010);
		showText(TX_SPEAKER_MUDD, TX_MUD0_031);
		showText(TX_SPEAKER_KIRK, TX_MUD0_007);
	}
}

void Room::mudd0TalkToSpock() {
	if (_awayMission->mudd.muddFirstRoomState == 2)
		showText(TX_SPEAKER_SPOCK, TX_MUD0_022);
	else {
		showText(TX_SPEAKER_SPOCK, TX_MUD0_022);
		showText(TX_SPEAKER_MUDD,  TX_MUD0_034);
		showText(TX_SPEAKER_SPOCK, TX_MUD0_023);
		showText(TX_SPEAKER_MCCOY, TX_MUD0_015);
		showText(TX_SPEAKER_MUDD,  TX_MUD0_038);
	}
}

void Room::mudd0TalkToMccoy() {
	if (_awayMission->mudd.muddFirstRoomState == 2)
		showText(TX_SPEAKER_MCCOY, TX_MUD0_016);
	else {
		showText(TX_SPEAKER_MCCOY, TX_MUD0_016);
		showText(TX_SPEAKER_KIRK,  TX_MUD0_003);
		showText(TX_SPEAKER_MUDD,  TX_MUD0_027);
		showText(TX_SPEAKER_MCCOY, TX_MUD0_014);
	}
}

void Room::mudd0TalkToRedshirt() {
	if (_awayMission->mudd.muddFirstRoomState == 2) {
		showText(TX_SPEAKER_BUCHERT, TX_MUD0_039);
		showText(TX_SPEAKER_KIRK,    TX_MUD0_008);
	} else {
		showText(TX_SPEAKER_BUCHERT, TX_MUD0_040);
		showText(TX_SPEAKER_MUDD,    TX_MUD0_030);
		showText(TX_SPEAKER_BUCHERT, TX_MUD0_041);
		showText(TX_SPEAKER_MUDD,    TX_MUD0_036);
	}
}

void Room::mudd0TalkToMudd() {
	showText(TX_SPEAKER_MUDD,  TX_MUD0_032);
	showText(TX_SPEAKER_SPOCK, TX_MUD0_024);
	showText(TX_SPEAKER_MUDD,  TX_MUD0_035);
	showText(TX_SPEAKER_KIRK,  TX_MUD0_004);
}

}
