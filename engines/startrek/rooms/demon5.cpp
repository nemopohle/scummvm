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

extern const RoomAction demon5ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::demon5Tick1 },
	{ {ACTION_WALK, 0x22, 0, 0, 0}, &Room::demon5WalkToDoor },
	{ {ACTION_WALK, 12, 0, 0, 0}, &Room::demon5WalkToDoor },
	{ {ACTION_TOUCHED_HOTSPOT, 0, 0, 0, 0}, &Room::demon5TouchedDoorOpenTrigger },
	{ {ACTION_FINISHED_WALKING, 1, 0, 0, 0}, &Room::demon5DoorOpenedOrReachedDoor },
	{ {ACTION_FINISHED_ANIMATION, 1, 0, 0, 0}, &Room::demon5DoorOpenedOrReachedDoor },

	{ {ACTION_USE, OBJECT_ISTRICOR, 0x21, 0, 0}, &Room::demon5UseSTricorderOnCrate },
	{ {ACTION_USE, OBJECT_IPHASERK, 0xff, 0, 0}, &Room::demon5UsePhaserOnAnything },
	{ {ACTION_USE, OBJECT_IPHASERS, 0xff, 0, 0}, &Room::demon5UsePhaserOnAnything },

	{ {ACTION_USE, OBJECT_IHAND, 11, 0, 0}, &Room::demon5UseHandOnStephen },
	{ {ACTION_USE, OBJECT_IBERRY, 11, 0, 0}, &Room::demon5UseBerryOnStephen },
	{ {ACTION_USE, OBJECT_IDETOXIN, 9, 0, 0}, &Room::demon5UseHypoDytoxinOnChub },
	{ {ACTION_FINISHED_WALKING, 2, 0, 0, 0}, &Room::demon5MccoyReachedChub },
	{ {ACTION_FINISHED_ANIMATION, 2, 0, 0, 0}, &Room::demon5MccoyHealedChub },

	{ {ACTION_USE, OBJECT_IDETOXIN, 0xff, 0, 0}, &Room::demon5UseHypoDytoxinOnAnything },
	{ {ACTION_USE, OBJECT_IBERRY, 9, 0, 0}, &Room::demon5UseBerryOnChub },

	{ {ACTION_LOOK, 8, 0, 0, 0}, &Room::demon5LookAtRoberts },
	{ {ACTION_LOOK, 10, 0, 0, 0}, &Room::demon5LookAtGrisnash },
	{ {ACTION_LOOK, 11, 0, 0, 0}, &Room::demon5LookAtStephen },

	{ {ACTION_LOOK, OBJECT_KIRK, 0, 0, 0}, &Room::demon5LookAtKirk },
	{ {ACTION_LOOK, OBJECT_SPOCK, 0, 0, 0}, &Room::demon5LookAtSpock },
	{ {ACTION_LOOK, OBJECT_MCCOY, 0, 0, 0}, &Room::demon5LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::demon5LookAtRedshirt },

	{ {ACTION_LOOK, 0x20, 0, 0, 0}, &Room::demon5LookAtMountain },
	{ {ACTION_LOOK, 0x21, 0, 0, 0}, &Room::demon5LookAtCrate },
	{ {ACTION_LOOK, 0xff, 0, 0, 0}, &Room::demon5LookAnywhere },
	{ {ACTION_LOOK, 9, 0, 0, 0}, &Room::demon5LookAtChub },

	{ {ACTION_TALK, 8, 0, 0, 0}, &Room::demon5TalkToRoberts },
	{ {ACTION_TALK, 9, 0, 0, 0}, &Room::demon5TalkToChub },
	{ {ACTION_TALK, 10, 0, 0, 0}, &Room::demon5TalkToGrisnash },
	{ {ACTION_TALK, 11, 0, 0, 0}, &Room::demon5TalkToStephen },

	{ {ACTION_TALK, OBJECT_KIRK, 0, 0, 0}, &Room::demon5TalkToKirk },
	{ {ACTION_TALK, OBJECT_SPOCK, 0, 0, 0}, &Room::demon5TalkToSpock },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::demon5TalkToRedshirt },
	{ {ACTION_TALK, OBJECT_MCCOY, 0, 0, 0}, &Room::demon5TalkToMccoy },

	{ {ACTION_USE, OBJECT_IMTRICOR, 8, 0, 0}, &Room::demon5UseMTricorderOnRoberts },
	{ {ACTION_USE, OBJECT_IMEDKIT,  9, 0, 0}, &Room::demon5UseMTricorderOnChub },
	{ {ACTION_USE, OBJECT_MCCOY,    9, 0, 0}, &Room::demon5UseMTricorderOnChub },
	{ {ACTION_USE, OBJECT_IMTRICOR, 9, 0, 0}, &Room::demon5UseMTricorderOnChub },
	{ {ACTION_USE, OBJECT_IMTRICOR, 10, 0, 0}, &Room::demon5UseMTricorderOnGrisnash },
	{ {ACTION_USE, OBJECT_IMTRICOR, 11, 0, 0}, &Room::demon5UseMTricorderOnStephen },

	{ {ACTION_GET, 0x21, 0, 0, 0}, &Room::demon5GetCrate },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum demon5TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_EVERTS,
	TX_SPEAKER_ROBERTS, TX_SPEAKER_CHUB, TX_SPEAKER_GRISNASH, TX_SPEAKER_STEPHEN,
	TX_DEM5_001, TX_DEM5_002, TX_DEM5_003, TX_DEM5_004, TX_DEM5_005,
	TX_DEM5_006, TX_DEM5_007, TX_DEM5_008, TX_DEM5_009, TX_DEM5_010,
	TX_DEM5_011, TX_DEM5_012, TX_DEM5_013, TX_DEM5_014, TX_DEM5_015,
	TX_DEM5_016, TX_DEM5_017, TX_DEM5_018, TX_DEM5_019, TX_DEM5_020,
	TX_DEM5_021, TX_DEM5_022, TX_DEM5_023, TX_DEM5_024, TX_DEM5_025,
	TX_DEM5_026, TX_DEM5_027, TX_DEM5_028, TX_DEM5_029, TX_DEM5_030,
	TX_DEM5_031, TX_DEM5_032, TX_DEM5_033, TX_DEM5_034, TX_DEM5_035,
	TX_DEM5_036, TX_DEM5_037, TX_DEM5_038, TX_DEM5_039, TX_DEM5_040,
	TX_DEM5_041, TX_DEM5_042, TX_DEM5_043, TX_DEM5_044, TX_DEM5_045,
	TX_DEM5L027, TX_DEM5L028, TX_DEM5L029, TX_DEM5L030, TX_DEM5L031,
	TX_DEM5N000, TX_DEM5N001, TX_DEM5N002, TX_DEM5N003, TX_DEM5N004,
	TX_DEM5N005, TX_DEM5N006, TX_DEM5N007, TX_DEM5N008, TX_DEM5N009,
	TX_DEM5N010, TX_DEM5N011, TX_DEM5N012, TX_DEM5N013
};

extern const RoomTextOffsets demon5TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 3530, 3234, 0, 3710 },
	{ TX_SPEAKER_MCCOY, 3541, 3245, 0, 3721 },
	{ TX_SPEAKER_SPOCK, 3551, 3255, 0, 3731 },
	{ TX_SPEAKER_EVERTS, 3561, 3265, 0, 3741 },
	{ TX_SPEAKER_ROBERTS, 3575, 3279, 0, 3755 },
	{ TX_SPEAKER_CHUB, 3591, 3295, 0, 3770 },
	{ TX_SPEAKER_GRISNASH, 3604, 3308, 0, 3782 },
	{ TX_SPEAKER_STEPHEN, 3621, 3325, 0, 3798 },
	{ TX_DEM5_001, 7000, 6337, 0, 7488 },
	{ TX_DEM5_002, 4928, 4461, 0, 5173 },
	{ TX_DEM5_003, 7506, 6768, 0, 7993 },
	{ TX_DEM5_004, 7798, 7014, 0, 8297 },
	{ TX_DEM5_005, 5344, 4847, 0, 5652 },
	{ TX_DEM5_006,  636, 610, 0, 636 },
	{ TX_DEM5_007,  743, 701, 0, 766 },
	{ TX_DEM5_008, 1506, 1373, 0, 1580 },
	{ TX_DEM5_009, 1567, 1419, 0, 1645 },
	{ TX_DEM5_010, 2942, 2671, 0, 3102 },
	{ TX_DEM5_011, 9224, 8269, 0, 9793 },
	{ TX_DEM5_012, 8086, 7227, 0, 8599 },
	{ TX_DEM5_013, 8998, 8078, 0, 9582 },
	{ TX_DEM5_014, 9101, 8166, 0, 9693 },
	{ TX_DEM5_015, 8705, 7815, 0, 9248 },
	{ TX_DEM5_016, 8808, 7903, 0, 9363 },
	{ TX_DEM5_017, 7564, 6811, 0, 8051 },
	{ TX_DEM5_018, 7136, 6443, 0, 7614 },
	{ TX_DEM5_019, 8035, 7191, 0, 8546 },
	{ TX_DEM5_020, 7941, 7127, 0, 8445 },
	{ TX_DEM5_021, 7423, 6700, 0, 7906 },
	{ TX_DEM5_022, 6203, 5600, 0, 6604 },
	{ TX_DEM5_023, 7653, 6884, 0, 8138 },
	{ TX_DEM5_024, 5638, 5095, 0, 5965 },
	{ TX_DEM5_025, 5838, 5265, 0, 6187 },
	{ TX_DEM5_026, 7991, 7162, 0, 8500 },
	{ TX_DEM5_027, 7055, 6377, 0, 7529 },
	{ TX_DEM5_028, 5408, 4881, 0, 5726 },
	{ TX_DEM5_029, 5816, 5258, 0, 6157 },
	{ TX_DEM5_030, 4526, 4066, 0, 4745 },
	{ TX_DEM5_031, 6611, 5978, 0, 7074 },
	{ TX_DEM5_032, 2308, 2085, 0, 2438 },
	{ TX_DEM5_033, 1155, 1067, 0, 1231 },
	{ TX_DEM5_034, 2488, 2235, 0, 2643 },
	{ TX_DEM5_035, 2223, 2015, 0, 2335 },
	{ TX_DEM5_036, 1080, 1007, 0, 1151 },
	{ TX_DEM5_037, 1246, 1143, 0, 1324 },
	{ TX_DEM5_038, 2398, 2160, 0, 2543 },
	{ TX_DEM5_039,  903, 845, 0, 951 },
	{ TX_DEM5_040, 8411, 7536, 0, 8941 },
	{ TX_DEM5_041, 5897, 5309, 0, 6247 },
	{ TX_DEM5_042, 6680, 6032, 0, 7144 },
	{ TX_DEM5_043, 6286, 5668, 0, 6701 },
	{ TX_DEM5_044, 7849, 7050, 0, 8349 },
	{ TX_DEM5_045, 7268, 6560, 0, 7757 },
	{ TX_DEM5L027, 1423, 1305, 0, 1500 },
	{ TX_DEM5L028, 3700, 3389, 0, 3870 },
	{ TX_DEM5L029, 4772, 4320, 0, 5016 },
	{ TX_DEM5L030, 4982, 4500, 0, 5235 },
	{ TX_DEM5L031, 5389, 4877, 0, 5705 },
	{ TX_DEM5N000, 4183, 3798, 0, 4386 },
	{ TX_DEM5N001, 3637, 3341, 0, 3813 },
	{ TX_DEM5N002, 4033, 3678, 0, 4209 },
	{ TX_DEM5N003, 3918, 3577, 0, 4099 },
	{ TX_DEM5N004, 4111, 3741, 0, 4302 },
	{ TX_DEM5N005, 4298, 3883, 0, 4511 },
	{ TX_DEM5N006, 4440, 3995, 0, 4647 },
	{ TX_DEM5N007, 4245, 3845, 0, 4452 },
	{ TX_DEM5N008, 4378, 3948, 0, 4589 },
	{ TX_DEM5N009, 3777, 3451, 0, 3950 },
	{ TX_DEM5N010, 1811, 1648, 0, 1888 },
	{ TX_DEM5N011, 2066, 1873, 0, 2159 },
	{ TX_DEM5N012, 3276, 2995, 0, 3453 },
	{ TX_DEM5N013, 1923, 1745, 0, 2011 },
	{          -1, 0,    0, 0, 0 }
};

extern const RoomText demon5Texts[] = {
	{ -1, Common::UNK_LANG, "" }
};

void Room::demon5Tick1() {
	playVoc("DEM5LOOP");

	if (!_awayMission->demon.curedChub || _awayMission->demon.field37 != 1)
		loadActorAnim(11, "oldman", 0x7a, 0xb0, 0);
	loadActorAnim(8, "brorob", 0xc9, 0xa8, 0);
	loadActorAnim(9, "crip", 0xe3, 0xa8, 0);
	loadActorAnim(10, "brotel", 0xf9, 0xbc, 0);
	loadActorAnim(12, "s0r6d2", 0xa0, 0x92, 0);
}

void Room::demon5WalkToDoor() {
	_awayMission->disableInput = true;
	_roomVar.demon.movingToDoor = true;
	walkCrewman(OBJECT_KIRK, 0xa0, 0x94, 1);
}

void Room::demon5TouchedDoorOpenTrigger() {
	if (!_roomVar.demon.movingToDoor)
		return;
	loadActorAnim(12, "s0r6d1", 0xa0, 0x92, 1);
	playSoundEffectIndex(kSfxDoor);
}

void Room::demon5DoorOpenedOrReachedDoor() {
	// This is invoked when the door opens and when Kirk reaches the door.
	// Must wait for both to occur.
	_roomVar.demon.doorCounter++;
	if (_roomVar.demon.doorCounter == 2)
		loadRoomIndex(0, 1);
}

void Room::demon5UseSTricorderOnCrate() {
	loadActorAnim(OBJECT_SPOCK, "sscans", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_DEM5_006);
}

// Like in the original, the engine handles phasers used on crewmen itself
void Room::demon5UsePhaserOnAnything() {
	showText(TX_SPEAKER_SPOCK, TX_DEM5_007);
}

void Room::demon5UseHandOnStephen() {
	if (_awayMission->demon.repairedHand)
		showText(TX_SPEAKER_STEPHEN, TX_DEM5_036);
	else
		showText(TX_SPEAKER_STEPHEN, TX_DEM5_039);
}

void Room::demon5UseBerryOnStephen() {
	if (_awayMission->demon.knowAboutHypoDytoxin)
		showText(TX_SPEAKER_STEPHEN, TX_DEM5_033);
	else
		showText(TX_SPEAKER_STEPHEN, TX_DEM5_037);
}

void Room::demon5UseHypoDytoxinOnChub() {
	walkCrewman(OBJECT_MCCOY, 0xe1, 0xb0, 2);
}

void Room::demon5MccoyReachedChub() {
	loadActorAnim2(OBJECT_MCCOY, "musemn", -1, -1, 2);
}

void Room::demon5MccoyHealedChub() {
	walkCrewman(OBJECT_MCCOY, 0xc8, 0xba, 0);
	_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_W;
	showText(TX_SPEAKER_CHUB, TX_DEM5L027);
	loseItem(OBJECT_IDETOXIN);
	_awayMission->demon.curedChub = true;
	_awayMission->demon.missionScore += 2;
}

void Room::demon5UseHypoDytoxinOnAnything() {
	showText(TX_SPEAKER_MCCOY, TX_DEM5_008);
}

void Room::demon5UseBerryOnChub() {
	showText(TX_SPEAKER_MCCOY, TX_DEM5_009);
}

void Room::demon5LookAtRoberts() {
	showDescription(TX_DEM5N001);
}

void Room::demon5LookAtGrisnash() {
	showDescription(TX_DEM5N009);
}

void Room::demon5LookAtStephen() {
	showDescription(TX_DEM5N003);
}

void Room::demon5LookAtKirk() {
	showDescription(TX_DEM5N006);
}

void Room::demon5LookAtSpock() {
	showDescription(TX_DEM5N008);
}

void Room::demon5LookAtMccoy() {
	showDescription(TX_DEM5N007);
}

void Room::demon5LookAtRedshirt() {
	showDescription(TX_DEM5N005);
}

void Room::demon5LookAtMountain() {
	showDescription(TX_DEM5N002);
}

void Room::demon5LookAtCrate() {
	showDescription(TX_DEM5N004);
}

void Room::demon5LookAnywhere() {
	showDescription(TX_DEM5N000);
}

void Room::demon5LookAtChub() {
	showText(TX_SPEAKER_CHUB, TX_DEM5L028);
}

void Room::demon5TalkToRoberts() {
	if (_awayMission->demon.curedChub) {
		showText(TX_SPEAKER_ROBERTS, TX_DEM5_030);
		if (!_roomVar.demon.talkedToRoberts) {
			_roomVar.demon.talkedToRoberts = true;
			_roomVar.demon.numTalkedTo++;
			demon5CheckCompletedStudy();
		}
	} else {
		showDescription(TX_DEM5N010);
	}
}

void Room::demon5TalkToChub() {
	if (_awayMission->demon.curedChub) {
		showText(TX_SPEAKER_CHUB, TX_DEM5L029);
		showText(TX_SPEAKER_KIRK, TX_DEM5_002);
		showText(TX_SPEAKER_CHUB, TX_DEM5L030);
		showText(TX_SPEAKER_KIRK, TX_DEM5_005);
		showText(TX_SPEAKER_CHUB, TX_DEM5L031);

		if (!_roomVar.demon.talkedToChub) {
			_roomVar.demon.talkedToChub = true;
			_roomVar.demon.numTalkedTo++;
			demon5CheckCompletedStudy();
		}
	} else {
		showDescription(TX_DEM5N013);
	}
}

void Room::demon5TalkToGrisnash() {
	if (_awayMission->demon.curedChub) {
		showText(TX_SPEAKER_GRISNASH, TX_DEM5_028);
		showText(TX_SPEAKER_SPOCK, TX_DEM5_024);
		showText(TX_SPEAKER_GRISNASH, TX_DEM5_029);
		showText(TX_SPEAKER_SPOCK, TX_DEM5_025);

		if (!_roomVar.demon.talkedToGrisnash) {
			_roomVar.demon.talkedToGrisnash = true;
			_roomVar.demon.numTalkedTo++;
			demon5CheckCompletedStudy();
		}
	} else {
		showDescription(TX_DEM5N011);
	}
}

void Room::demon5TalkToStephen() {
	if (_awayMission->demon.curedChub) {
		showText(TX_SPEAKER_STEPHEN, TX_DEM5_041);
		showText(TX_SPEAKER_MCCOY, TX_DEM5_022);
		showText(TX_SPEAKER_STEPHEN, TX_DEM5_043);
		showText(TX_SPEAKER_ROBERTS, TX_DEM5_031);
		showText(TX_SPEAKER_STEPHEN, TX_DEM5_042);

		if (!_roomVar.demon.talkedToStephen) {
			_roomVar.demon.talkedToStephen = true;
			_roomVar.demon.numTalkedTo++;
			demon5CheckCompletedStudy();
		}
	} else if (!_awayMission->demon.knowAboutHypoDytoxin) {
		showText(TX_SPEAKER_ROBERTS, TX_DEM5_034);
	} else if (_awayMission->demon.madeHypoDytoxin) {
		showText(TX_SPEAKER_ROBERTS, TX_DEM5_035);
	} else if (_awayMission->demon.gotBerries) {
		showText(TX_SPEAKER_ROBERTS, TX_DEM5_032);
		_awayMission->demon.field3e = false;
	} else if (_awayMission->demon.field3e) {
		showText(TX_SPEAKER_ROBERTS, TX_DEM5_038);
	} else {
		showText(TX_SPEAKER_ROBERTS, TX_DEM5_034);
	}
}

void Room::demon5TalkToKirk() {
	showText(TX_SPEAKER_KIRK, TX_DEM5_001);
}

void Room::demon5TalkToSpock() {
	showText(TX_SPEAKER_SPOCK, TX_DEM5_027);
	showText(TX_SPEAKER_MCCOY, TX_DEM5_018);
}

void Room::demon5TalkToRedshirt() {
	showText(TX_SPEAKER_EVERTS, TX_DEM5_045);
	showText(TX_SPEAKER_MCCOY,  TX_DEM5_021);
	showText(TX_SPEAKER_KIRK,   TX_DEM5_003);
}

void Room::demon5TalkToMccoy() {
	if (_awayMission->demon.curedChub) {
		showText(TX_SPEAKER_MCCOY, TX_DEM5_023);
		showText(TX_SPEAKER_KIRK,  TX_DEM5_004);
		if (!_awayMission->redshirtDead) {
			showText(TX_SPEAKER_EVERTS, TX_DEM5_044);
			showText(TX_SPEAKER_MCCOY, TX_DEM5_020);
			showText(TX_SPEAKER_SPOCK, TX_DEM5_026);
			showText(TX_SPEAKER_MCCOY, TX_DEM5_019);
		}
	} else {
		showText(TX_SPEAKER_MCCOY, TX_DEM5_017);
	}
}

void Room::demon5UseMTricorderOnRoberts() {
	if (_roomVar.demon.scannedRoberts)
		return;
	loadActorAnim2(OBJECT_MCCOY, "mscane", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_MCCOY, TX_DEM5_015);

	_roomVar.demon.scannedRoberts = true;
	_roomVar.demon.numScanned++;
	demon5CheckCompletedStudy();
}

void Room::demon5UseMTricorderOnChub() {
	loadActorAnim2(OBJECT_MCCOY, "mscane", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);

	if (_awayMission->demon.curedChub) {
		if (_roomVar.demon.scannedChub)
			return;
		showText(TX_SPEAKER_MCCOY, TX_DEM5_016);
		_roomVar.demon.scannedChub = true;
		_roomVar.demon.numScanned++;
		demon5CheckCompletedStudy();
	} else {
		if (_awayMission->demon.field3e)
			showText(TX_SPEAKER_MCCOY, TX_DEM5_010);
		else {
			showText(TX_SPEAKER_MCCOY, TX_DEM5_012);
			showText(TX_SPEAKER_STEPHEN, TX_DEM5_040);
			_awayMission->demon.knowAboutHypoDytoxin = true;
			_awayMission->demon.field3e = true;
		}
	}
}


void Room::demon5UseMTricorderOnGrisnash() {
	if (_roomVar.demon.scannedGrisnash)
		return;
	loadActorAnim2(OBJECT_MCCOY, "mscane", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_MCCOY, TX_DEM5_013);

	_roomVar.demon.scannedGrisnash = true;
	_roomVar.demon.numScanned++;
	demon5CheckCompletedStudy();
}

void Room::demon5UseMTricorderOnStephen() {
	if (_roomVar.demon.scannedStephen)
		return;
	loadActorAnim2(OBJECT_MCCOY, "mscanw", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_MCCOY, TX_DEM5_014);

	_roomVar.demon.scannedStephen = true;
	_roomVar.demon.numScanned++;
	demon5CheckCompletedStudy();
}

void Room::demon5CheckCompletedStudy() {
	if (_roomVar.demon.numScanned == 4 && _roomVar.demon.numTalkedTo == 4) {
		showText(TX_SPEAKER_MCCOY, TX_DEM5_011);
		_roomVar.demon.numTalkedTo = 5;
	}
}

void Room::demon5GetCrate() {
	showDescription(TX_DEM5N012);
}

}
