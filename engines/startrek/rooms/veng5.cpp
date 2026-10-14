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
#define OBJECT_DEBRIS 9
#define OBJECT_POWER_PACK 10
#define OBJECT_DEAD_GUY 11

#define HOTSPOT_DOOR 0x20
#define HOTSPOT_LADDER 0x21

namespace StarTrek {

extern const RoomAction veng5ActionList[] = {
	{ {ACTION_TICK, 1,  0, 0, 0}, &Room::veng5Tick1 },
	{ {ACTION_TICK, 45, 0, 0, 0}, &Room::veng5Tick45 },

	{ {ACTION_WALK, HOTSPOT_DOOR, 0, 0, 0}, &Room::veng5WalkToDoor },
	{ {ACTION_DONE_WALK, 2,       0, 0, 0}, &Room::veng5ReachedDoor },
	{ {ACTION_DONE_ANIM, 1,       0, 0, 0}, &Room::veng5DoorOpened },
	{ {ACTION_TOUCHED_WARP, 1,    0, 0, 0}, &Room::veng5TouchedTurboliftDoor },

	{ {ACTION_LOOK, HOTSPOT_DOOR,      0, 0, 0}, &Room::veng5LookAtDoor },
	{ {ACTION_LOOK, OBJECT_DOOR,       0, 0, 0}, &Room::veng5LookAtDoor },
	{ {ACTION_LOOK, HOTSPOT_LADDER,    0, 0, 0}, &Room::veng5LookAtLadder },
	{ {ACTION_LOOK, OBJECT_DEAD_GUY,   0, 0, 0}, &Room::veng5LookAtDeadGuy },
	{ {ACTION_LOOK, OBJECT_POWER_PACK, 0, 0, 0}, &Room::veng5LookAtPowerPack },
	{ {ACTION_LOOK, OBJECT_DEBRIS,     0, 0, 0}, &Room::veng5LookAtDebris },
	{ {ACTION_LOOK, OBJECT_KIRK,       0, 0, 0}, &Room::veng5LookAtKirk },
	{ {ACTION_LOOK, OBJECT_SPOCK,      0, 0, 0}, &Room::veng5LookAtSpock },
	{ {ACTION_LOOK, OBJECT_MCCOY,      0, 0, 0}, &Room::veng5LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_REDSHIRT,   0, 0, 0}, &Room::veng5LookAtRedshirt },
	{ {ACTION_LOOK, 0xff,              0, 0, 0}, &Room::veng5LookAnywhere },

	{ {ACTION_USE, OBJECT_IPHASERS, OBJECT_POWER_PACK, 0, 0}, &Room::veng5UsePhaserOnPowerPack },
	{ {ACTION_USE, OBJECT_IPHASERK, OBJECT_POWER_PACK, 0, 0}, &Room::veng5UsePhaserOnPowerPack },
	{ {ACTION_USE, OBJECT_IPHASERS, OBJECT_DEBRIS,     0, 0}, &Room::veng5UseStunPhaserOnDebris },
	{ {ACTION_USE, OBJECT_IPHASERK, OBJECT_DEBRIS,     0, 0}, &Room::veng5UseKillPhaserOnDebris },
	{ {ACTION_DONE_WALK, 4,                         0, 0, 0}, &Room::veng5ReachedPositionToShootDebris },
	{ {ACTION_DONE_ANIM, 5,                         0, 0, 0}, &Room::veng5DrewPhaser },
	{ {ACTION_DONE_ANIM, 3,                         0, 0, 0}, &Room::veng5VaporizedDebris },

	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_DEBRIS,     0, 0}, &Room::veng5UseMTricorderOnDebris },
	{ {ACTION_USE, OBJECT_ISTRICOR, OBJECT_DEBRIS,     0, 0}, &Room::veng5UseSTricorderOnDebris },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_POWER_PACK, 0, 0}, &Room::veng5UseMTricorderOnPowerPack },
	{ {ACTION_USE, OBJECT_ISTRICOR, OBJECT_POWER_PACK, 0, 0}, &Room::veng5UseSTricorderOnPowerPack },

	{ {ACTION_TALK, OBJECT_KIRK,       0, 0, 0}, &Room::veng5TalkToKirk },
	{ {ACTION_TALK, OBJECT_MCCOY,      0, 0, 0}, &Room::veng5TalkToMccoy },
	{ {ACTION_TALK, OBJECT_SPOCK,      0, 0, 0}, &Room::veng5TalkToSpock },
	{ {ACTION_TALK, OBJECT_REDSHIRT,   0, 0, 0}, &Room::veng5TalkToRedshirt },

	{ {ACTION_TOUCHED_HOTSPOT, 0, 0, 0, 0}, &Room::veng5TouchedHotspot0 },

	{ {ACTION_GET, OBJECT_POWER_PACK, 0, 0, 0}, &Room::veng5GetPowerPack },
	{ {ACTION_DONE_WALK, 6,           0, 0, 0}, &Room::veng5ReachedPowerPack },
	{ {ACTION_DONE_ANIM, 7,           0, 0, 0}, &Room::veng5PickedUpPowerPack },

	// Common code
	{ {ACTION_TICK, 0xff, 0xff, 0xff, 0}, &Room::vengaTick },
	{ {ACTION_USE, OBJECT_IPHASERS, 0xff,     0, 0}, &Room::vengaUsePhaserAnywhere },
	{ {ACTION_USE, OBJECT_IPHASERK, 0xff,     0, 0}, &Room::vengaUsePhaserAnywhere },
	{ {ACTION_LOOK, OBJECT_IHYPO,          0, 0, 0}, &Room::vengaLookAtHypo },
	{ {ACTION_USE, OBJECT_ICOMM, OBJECT_KIRK, 0, 0}, &Room::vengaUseCommunicator },
	{ {ACTION_USE, OBJECT_IMEDKIT,  OBJECT_DEAD_GUY, 0, 0}, &Room::veng5UseMccoyOnDeadGuy },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_DEAD_GUY, 0, 0}, &Room::veng5UseMccoyOnDeadGuy },
	{ {ACTION_USE, OBJECT_MCCOY,    OBJECT_DEAD_GUY, 0, 0}, &Room::veng5UseMccoyOnDeadGuy },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum veng5TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_KIJE,
	TX_BRIDU146, TX_DEM3_019, TX_G_014, TX_MUD4_018, TX_VEN0_016,
	TX_VEN2U093, TX_VEN2_028, TX_VEN2_066, TX_VEN2_098, TX_VEN4N010,
	TX_VEN4_016, TX_VEN5N000, TX_VEN5N001, TX_VEN5N002, TX_VEN5N003,
	TX_VEN5N004, TX_VEN5N005, TX_VEN5N006, TX_VEN5N007, TX_VEN5N008,
	TX_VEN5N009, TX_VEN5N010, TX_VEN5_001, TX_VEN5_002, TX_VEN5_003,
	TX_VEN5_004, TX_VEN5_005, TX_VEN5_006, TX_VEN5_007, TX_VEN5_008,
	TX_VEN5_009, TX_VEN5_010, TX_VEN5_011, TX_VEN5_012, TX_VEN5_013,
	TX_VEN5_015, TX_VEN5_016, TX_VEN5_017, TX_VEN5_018, TX_VEN5_019,
	TX_VEN5_R19, TX_VEN5_R20, TX_VEN6N007, TX_VENA_F32, TX_VENA_F34,
	TX_VENA_F40
};

extern const RoomTextOffsets veng5TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 5963, 0, 6289, 6350, 0, 0, 0, 0, 5953 },
	{ TX_SPEAKER_MCCOY, 5974, 0, 6300, 6361, 0, 0, 0, 0, 5964 },
	{ TX_SPEAKER_SPOCK, 5984, 0, 6310, 6371, 0, 0, 0, 0, 5974 },
	{ TX_SPEAKER_KIJE, 5994, 0, 6320, 6381, 0, 0, 0, 0, 5984 },
	{ TX_BRIDU146, 5020, 0, 5264, 5368, 0, 0, 0, 0, 5010 },
	{ TX_DEM3_019, 5504, 0, 5799, 5874, 0, 0, 0, 0, 5494 },
	{ TX_G_014, 5627, 0, 5921, 5998, 0, 0, 0, 0, 5617 },
	{ TX_MUD4_018, 4951, 0, 5197, 5300, 0, 0, 0, 0, 4941 },
	{ TX_MUD4_018, 5084, 0, 5327, 5444, 0, 0, 0, 0, 5074 },
	{ TX_VEN0_016, 5741, 0, 6047, 6116, 0, 0, 0, 0, 5731 },
	//{ TX_VEN0_016, 5800, 0, 0, 0 },	// Skip duplicate text
	{ TX_VEN2U093, 5152, 0, 5394, 5512, 0, 0, 0, 0, 5142 },
	{ TX_VEN2_028, 5259, 0, 5514, 5618, 0, 0, 0, 0, 5249 },
	{ TX_VEN2_066, 5337, 0, 5609, 5700, 0, 0, 0, 0, 5327 },
	{ TX_VEN2_098, 4147, 0, 4386, 4440, 0, 0, 0, 0, 4137 },
	{ TX_VEN4N010, 4801, 0, 5048, 5152, 0, 0, 0, 0, 4791 },
	{ TX_VEN4_016, 5555, 0, 5852, 5926, 0, 0, 0, 0, 5545 },
	{ TX_VEN5N000, 897, 0, 937, 911, 0, 0, 0, 0, 887 },
	{ TX_VEN5N001, 1206, 0, 1305, 1275, 0, 0, 0, 0, 1196 },
	{ TX_VEN5N002, 1339, 0, 1444, 1426, 0, 0, 0, 0, 1329 },
	{ TX_VEN5N003, 1270, 0, 1373, 1343, 0, 0, 0, 0, 1260 },
	{ TX_VEN5N004, 1394, 0, 1501, 1489, 0, 0, 0, 0, 1384 },
	{ TX_VEN5N005, 1474, 0, 1583, 1584, 0, 0, 0, 0, 1464 },
	{ TX_VEN5N006, 1572, 0, 1704, 1717, 0, 0, 0, 0, 1562 },
	{ TX_VEN5N007, 668, 0, 659, 661, 0, 0, 0, 0, 658 },
	{ TX_VEN5N008, 739, 0, 748, 735, 0, 0, 0, 0, 729 },
	{ TX_VEN5N009, 993, 0, 1046, 1034, 0, 0, 0, 0, 983 },
	{ TX_VEN5N010, 3993, 0, 4224, 4284, 0, 0, 0, 0, 3983 },
	{ TX_VEN5_001, 3126, 0, 3330, 3343, 0, 0, 0, 0, 3116 },
	{ TX_VEN5_002, 3695, 0, 3927, 3975, 0, 0, 0, 0, 3685 },
	{ TX_VEN5_003, 2446, 0, 2640, 2641, 0, 0, 0, 0, 2436 },
	{ TX_VEN5_004, 3021, 0, 3214, 3228, 0, 0, 0, 0, 3011 },
	{ TX_VEN5_005, 372, 0, 372, 372, 0, 0, 0, 0, 362 },
	{ TX_VEN5_006, 3200, 0, 3410, 3432, 0, 0, 0, 0, 3190 },
	{ TX_VEN5_007, 1672, 0, 1800, 1817, 0, 0, 0, 0, 1662 },
	{ TX_VEN5_008, 2577, 0, 2774, 2782, 0, 0, 0, 0, 2567 },
	{ TX_VEN5_009, 2869, 0, 3061, 3077, 0, 0, 0, 0, 2859 },
	{ TX_VEN5_010, 2170, 0, 2329, 2341, 0, 0, 0, 0, 2160 },
	{ TX_VEN5_011, 2024, 0, 2170, 2186, 0, 0, 0, 0, 2014 },
	{ TX_VEN5_012, 2735, 0, 2923, 2937, 0, 0, 0, 0, 2725 },
	{ TX_VEN5_013, 1780, 0, 1914, 1937, 0, 0, 0, 0, 1770 },
	{ TX_VEN5_015, 3574, 0, 3810, 3847, 0, 0, 0, 0, 3564 },
	{ TX_VEN5_016, 3764, 0, 4001, 4057, 0, 0, 0, 0, 3754 },
	{ TX_VEN5_017, 3280, 0, 3505, 3512, 0, 0, 0, 0, 3270 },
	{ TX_VEN5_018, 2095, 0, 2244, 2259, 0, 0, 0, 0, 2085 },
	{ TX_VEN5_019, 2299, 0, 2478, 2511, 0, 0, 0, 0, 2289 },
	{ TX_VEN5_R19, 3511, 0, 3745, 3782, 0, 0, 0, 0, 3501 },
	{ TX_VEN5_R20, 3410, 0, 3643, 3665, 0, 0, 0, 0, 3400 },
	{ TX_VEN6N007, 4866, 0, 5104, 5220, 0, 0, 0, 0, 4856 },
	{ TX_VENA_F32, 4377, 0, 4632, 4684, 0, 0, 0, 0, 4367 },
	{ TX_VENA_F34, 4647, 0, 4909, 4981, 0, 0, 0, 0, 4637 },
	{ TX_VENA_F40, 4242, 0, 4492, 4549, 0, 0, 0, 0, 4232 },
	{          -1, 0,    0, 0, 0, 0, 0, 0, 0, 0 }
};

extern const RoomText veng5Texts[] = {
	  { -1, Common::UNK_LANG, "" }
};

void Room::veng5Tick1() {
	playVoc("VEN5LOOP");

	if (!_awayMission->veng.clearedDebrisInRoom5)
		loadActorAnim(OBJECT_DEBRIS, "s7r5de", 0xb1, 0x89);
	else if (!_awayMission->veng.havePowerPack)
		loadActorAnim(OBJECT_POWER_PACK, "s7r5fu", 0xb1, 0x89);

	loadActorAnim(OBJECT_DEAD_GUY, "s7r5c1", 0xe2, 0x91);
}

void Room::veng5Tick45() {
	if (!_awayMission->veng.enteredRoom5FirstTime) {
		_awayMission->veng.enteredRoom5FirstTime = true;
		showText(TX_SPEAKER_SPOCK, TX_VEN5_005); // "I'm picking up on a nearby power source"
	}
}

void Room::veng5WalkToDoor() {
	walkCrewmanC(OBJECT_KIRK, 0x78, 0x8e, &Room::veng5ReachedDoor);
}

void Room::veng5ReachedDoor() {
	_awayMission->disableInput = true;
	playSoundEffectIndex(kSfxDoor);
	loadActorAnimC(OBJECT_DOOR, "s7r5d1", 0x6b, 0x8c, &Room::veng5DoorOpened);
	walkCrewman(OBJECT_KIRK, 0x64, 0x8e);
}

void Room::veng5DoorOpened() {
	loadRoomIndex(6, 2);
}

void Room::veng5TouchedTurboliftDoor() {
	playSoundEffectIndex(kSfxDoor);
	showRepublicMap(5, 1);
}

void Room::veng5LookAtDoor() {
	showDescription(TX_VEN5N007);
}

void Room::veng5LookAtLadder() {
	showDescription(TX_VEN5N008);
}

void Room::veng5LookAtDeadGuy() {
	showDescription(TX_VEN5N000);
}

void Room::veng5UseMccoyOnDeadGuy() {
	vengaUseMccoyOnDeadGuy(TX_DEM3_019, TX_VEN4_016, TX_G_014, TX_VEN0_016);
}

void Room::veng5LookAtPowerPack() {
	showDescription(TX_VEN5N009);
}

void Room::veng5LookAtDebris() {
	showDescription(TX_VEN5N001);
}

void Room::veng5LookAtKirk() {
	showDescription(TX_VEN5N003);
}

void Room::veng5LookAtSpock() {
	showDescription(TX_VEN5N002);
}

void Room::veng5LookAtMccoy() {
	showDescription(TX_VEN5N004);
}

void Room::veng5LookAtRedshirt() {
	showDescription(TX_VEN5N005);
}

void Room::veng5LookAnywhere() {
	showDescription(TX_VEN5N006);
}

void Room::veng5UsePhaserOnPowerPack() {
	showText(TX_SPEAKER_MCCOY, TX_VEN5_007);
}

void Room::veng5UseStunPhaserOnDebris() {
	showText(TX_SPEAKER_SPOCK, TX_VEN5_013);
}

void Room::veng5UseKillPhaserOnDebris() {
	walkCrewmanC(OBJECT_KIRK, 0xaa, 0xb9, &Room::veng5ReachedPositionToShootDebris);
	_awayMission->disableInput = true;
}

void Room::veng5ReachedPositionToShootDebris() {
	loadActorAnimC(OBJECT_KIRK, "kfiren", -1, -1, &Room::veng5DrewPhaser);
}

void Room::veng5DrewPhaser() {
	loadActorAnimC(OBJECT_POWER_PACK, "s7r5p1", 0xb1, 0x89, &Room::veng5VaporizedDebris);
	playSoundEffectIndex(kSfxPhaser);
	loadActorStandAnim(OBJECT_DEBRIS);
}

void Room::veng5VaporizedDebris() {
	loadActorStandAnim(OBJECT_KIRK);

	showText(TX_SPEAKER_MCCOY, TX_VEN5_011);
	showText(TX_SPEAKER_SPOCK, TX_VEN5_018);
	showText(TX_SPEAKER_MCCOY, TX_VEN5_010);
	showText(TX_SPEAKER_SPOCK, TX_VEN5_019);
	showText(TX_SPEAKER_KIRK,  TX_VEN5_003);

	_awayMission->veng.clearedDebrisInRoom5 = true;
	_awayMission->disableInput = false;
}

void Room::veng5UseMTricorderOnDebris() {
	mccoyScan(DIR_W, TX_SPEAKER_MCCOY, TX_VEN5_008);
}

void Room::veng5UseSTricorderOnDebris() {
	spockScan(DIR_N, TX_SPEAKER_SPOCK, TX_VEN5_012);
}

void Room::veng5UseMTricorderOnPowerPack() {
	mccoyScan(DIR_W, TX_SPEAKER_MCCOY, TX_VEN5_009); // ENHANCEMENT: Use mccoyScan, not showText
}

void Room::veng5UseSTricorderOnPowerPack() {
	spockScan(DIR_N, TX_SPEAKER_SPOCK, TX_VEN5_004);
}

void Room::veng5TalkToKirk() {
	showText(TX_SPEAKER_KIRK, TX_VEN5_001);
}

void Room::veng5TalkToMccoy() {
	showText(TX_SPEAKER_MCCOY, TX_VEN5_006);
}

void Room::veng5TalkToSpock() {
	showText(TX_SPEAKER_SPOCK, TX_VEN5_017);
	showText(TX_SPEAKER_KIJE,  TX_VEN5_R20);
}

void Room::veng5TalkToRedshirt() {
	showText(TX_SPEAKER_KIJE, TX_VEN5_R19);
}

void Room::veng5TouchedHotspot0() { // Approached too close to fire
	showText(TX_SPEAKER_SPOCK, TX_VEN5_015);

	if (!_awayMission->veng.askedSpockAboutFire) {
		_awayMission->veng.askedSpockAboutFire = true;
		showText(TX_SPEAKER_KIRK,  TX_VEN5_002);
		showText(TX_SPEAKER_SPOCK, TX_VEN5_016);
	}
}

void Room::veng5GetPowerPack() {
	walkCrewmanC(OBJECT_KIRK, 0xb1, 0x8c, &Room::veng5ReachedPowerPack);
}

void Room::veng5ReachedPowerPack() {
	loadActorAnimC(OBJECT_KIRK, "kuseln", -1, -1, &Room::veng5PickedUpPowerPack);
}

void Room::veng5PickedUpPowerPack() {
	if (!_awayMission->veng.havePowerPack) {
		loadActorStandAnim(OBJECT_POWER_PACK);
		showDescription(TX_VEN5N010);
		_awayMission->veng.havePowerPack = true;
		giveItem(OBJECT_IFUSION);
	}
}

}
