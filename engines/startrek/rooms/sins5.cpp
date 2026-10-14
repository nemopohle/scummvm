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

#define OBJECT_LEFT_COMPUTER_EXPLOSION 8
#define OBJECT_RIGHT_COMPUTER_EXPLOSION 9
#define OBJECT_MIDDLE_COMPUTER_EXPLOSION 10
#define OBJECT_CABLE 11

#define HOTSPOT_RIGHT_COMPUTER 0x20
#define HOTSPOT_LEFT_COMPUTER 0x21
#define HOTSPOT_MIDDLE_COMPUTER 0x22
#define HOTSPOT_NORTH_DOOR 0x23
#define HOTSPOT_LIGHT 0x24
#define HOTSPOT_MISSILE_1 0x25
#define HOTSPOT_MISSILE_2 0x26
#define HOTSPOT_MISSILE_3 0x27
#define HOTSPOT_MISSILE_4 0x28

namespace StarTrek {

extern const RoomAction sins5ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::sins5Tick1 },

	{ {ACTION_USE, OBJECT_ISTRICOR, 0xff,                    0, 0}, &Room::sins5UseSTricorderAnywhere },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_RIGHT_COMPUTER,  0, 0}, &Room::sins5UseSTricorderOnRightComputer },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_LEFT_COMPUTER,   0, 0}, &Room::sins5UseSTricorderOnLeftComputer },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_MIDDLE_COMPUTER, 0, 0}, &Room::sins5UseSTricorderOnMiddleComputer },
	{ {ACTION_USE, OBJECT_KIRK,     HOTSPOT_RIGHT_COMPUTER,  0, 0}, &Room::sins5UseKirkOnRightComputer },
	{ {ACTION_USE, OBJECT_KIRK,     HOTSPOT_LEFT_COMPUTER,   0, 0}, &Room::sins5UseKirkOnLeftComputer },
	{ {ACTION_USE, OBJECT_KIRK,     HOTSPOT_MIDDLE_COMPUTER, 0, 0}, &Room::sins5UseKirkOnMiddleComputer },
	{ {ACTION_USE, OBJECT_MCCOY,    HOTSPOT_RIGHT_COMPUTER,  0, 0}, &Room::sins5UseMccoyOnComputer },
	{ {ACTION_USE, OBJECT_MCCOY,    HOTSPOT_LEFT_COMPUTER,   0, 0}, &Room::sins5UseMccoyOnComputer },
	{ {ACTION_USE, OBJECT_MCCOY,    HOTSPOT_MIDDLE_COMPUTER, 0, 0}, &Room::sins5UseMccoyOnComputer },
	{ {ACTION_USE, OBJECT_REDSHIRT, HOTSPOT_RIGHT_COMPUTER,  0, 0}, &Room::sins5UseRedshirtOnComputer },
	{ {ACTION_USE, OBJECT_REDSHIRT, HOTSPOT_LEFT_COMPUTER,   0, 0}, &Room::sins5UseRedshirtOnComputer },
	{ {ACTION_USE, OBJECT_REDSHIRT, HOTSPOT_MIDDLE_COMPUTER, 0, 0}, &Room::sins5UseRedshirtOnComputer },
	{ {ACTION_USE, OBJECT_SPOCK,    HOTSPOT_MIDDLE_COMPUTER, 0, 0}, &Room::sins5UseSpockOnMiddleComputer },
	{ {ACTION_USE, OBJECT_SPOCK,    HOTSPOT_RIGHT_COMPUTER,  0, 0}, &Room::sins5UseSpockOnRightComputer },
	{ {ACTION_DONE_WALK, 11,                              0, 0, 0}, &Room::sins5SpockReachedRightComputer },
	{ {ACTION_DONE_ANIM, 13,                              0, 0, 0}, &Room::sins5SpockUsedRightComputer },
	{ {ACTION_USE, OBJECT_SPOCK,    HOTSPOT_LEFT_COMPUTER,   0, 0}, &Room::sins5UseSpockOnLeftComputer },
	{ {ACTION_DONE_WALK, 10,                              0, 0, 0}, &Room::sins5SpockReachedLeftComputer },
	{ {ACTION_DONE_ANIM, 12,                              0, 0, 0}, &Room::sins5SpockUsedLeftComputer },
	{ {ACTION_DONE_WALK, 16,                              0, 0, 0}, &Room::sins5CrewmanReadyToBeamOut },

	{ {ACTION_TICK, 20, 0, 0, 0}, &Room::sins5Tick20 },

	{ {ACTION_USE, OBJECT_ICONECT, HOTSPOT_LEFT_COMPUTER,    0, 0}, &Room::sins5UseWireOnComputer },
	{ {ACTION_USE, OBJECT_ICONECT, HOTSPOT_RIGHT_COMPUTER,   0, 0}, &Room::sins5UseWireOnComputer },
	{ {ACTION_DONE_WALK, 14,                              0, 0, 0}, &Room::sins5KirkOrSpockInPositionToUseWire },
	{ {ACTION_DONE_ANIM, 15,                              0, 0, 0}, &Room::sins5WireConnected },
	{ {ACTION_USE, OBJECT_IPHASERS, HOTSPOT_LEFT_COMPUTER,   0, 0}, &Room::sins5UseStunPhaserOnComputer },
	{ {ACTION_USE, OBJECT_IPHASERS, HOTSPOT_MIDDLE_COMPUTER, 0, 0}, &Room::sins5UseStunPhaserOnComputer },
	{ {ACTION_USE, OBJECT_IPHASERS, HOTSPOT_RIGHT_COMPUTER,  0, 0}, &Room::sins5UseStunPhaserOnComputer },

	{ {ACTION_USE, OBJECT_IPHASERK, HOTSPOT_LEFT_COMPUTER,   0, 0}, &Room::sins5UseKillPhaserOnLeftComputer },
	{ {ACTION_DONE_WALK, 2,                               0, 0, 0}, &Room::sins5ReachedPositionToShootLeftComputer },
	{ {ACTION_DONE_ANIM, 3,                               0, 0, 0}, &Room::sins5DrewPhaserToShootLeftComputer },
	{ {ACTION_TIMER_EXPIRED, 0,                           0, 0, 0}, &Room::sins5Timer0Expired },

	{ {ACTION_USE, OBJECT_IPHASERK, HOTSPOT_MIDDLE_COMPUTER, 0, 0}, &Room::sins5UseKillPhaserOnMiddleComputer },
	{ {ACTION_DONE_WALK, 6,                               0, 0, 0}, &Room::sins5ReachedPositionToShootMiddleComputer },
	{ {ACTION_DONE_ANIM, 7,                               0, 0, 0}, &Room::sins5DrewPhaserToShootMiddleComputer },
	{ {ACTION_TIMER_EXPIRED, 1,                           0, 0, 0}, &Room::sins5Timer1Expired },

	{ {ACTION_USE, OBJECT_IPHASERK, HOTSPOT_RIGHT_COMPUTER,  0, 0}, &Room::sins5UseKillPhaserOnRightComputer },
	{ {ACTION_DONE_WALK, 4,                               0, 0, 0}, &Room::sins5ReachedPositionToShootRightComputer },
	{ {ACTION_DONE_ANIM, 5,                               0, 0, 0}, &Room::sins5DrewPhaserToShootRightComputer },
	{ {ACTION_TIMER_EXPIRED, 2,                           0, 0, 0}, &Room::sins5Timer2Expired },
	{ {ACTION_DONE_ANIM, 1,                               0, 0, 0}, &Room::sins5ComputerLaunchesMissiles },
	{ {ACTION_TIMER_EXPIRED, 3,                           0, 0, 0}, &Room::sins5Timer3Expired },

	{ {ACTION_LOOK, 0xff, 0, 0, 0}, &Room::sins5LookAnywhere },
	{ {ACTION_LOOK, OBJECT_KIRK,             0, 0, 0}, &Room::sins5LookAtKirk },
	{ {ACTION_LOOK, OBJECT_SPOCK,            0, 0, 0}, &Room::sins5LookAtSpock },
	{ {ACTION_LOOK, OBJECT_MCCOY,            0, 0, 0}, &Room::sins5LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_REDSHIRT,         0, 0, 0}, &Room::sins5LookAtRedshirt },
	{ {ACTION_LOOK, HOTSPOT_LIGHT,           0, 0, 0}, &Room::sins5LookAtLight },
	{ {ACTION_LOOK, HOTSPOT_LEFT_COMPUTER,   0, 0, 0}, &Room::sins5LookAtLeftComputer },
	{ {ACTION_LOOK, HOTSPOT_MIDDLE_COMPUTER, 0, 0, 0}, &Room::sins5LookAtMiddleComputer },
	{ {ACTION_LOOK, HOTSPOT_RIGHT_COMPUTER,  0, 0, 0}, &Room::sins5LookAtRightComputer },
	{ {ACTION_LOOK, HOTSPOT_MISSILE_1,       0, 0, 0}, &Room::sins5LookAtMissile },
	{ {ACTION_LOOK, HOTSPOT_MISSILE_2,       0, 0, 0}, &Room::sins5LookAtMissile },
	{ {ACTION_LOOK, HOTSPOT_MISSILE_3,       0, 0, 0}, &Room::sins5LookAtMissile },
	{ {ACTION_LOOK, HOTSPOT_MISSILE_4,       0, 0, 0}, &Room::sins5LookAtMissile },
	{ {ACTION_LOOK, HOTSPOT_NORTH_DOOR,      0, 0, 0}, &Room::sins5LookAtNorthDoor },

	{ {ACTION_USE, OBJECT_IMEDKIT, OBJECT_KIRK,     0, 0}, &Room::sins5UseMedkitOnCrewman },
	{ {ACTION_USE, OBJECT_IMEDKIT, OBJECT_SPOCK,    0, 0}, &Room::sins5UseMedkitOnCrewman },
	{ {ACTION_USE, OBJECT_IMEDKIT, OBJECT_MCCOY,    0, 0}, &Room::sins5UseMedkitOnCrewman },
	{ {ACTION_USE, OBJECT_IMEDKIT, OBJECT_REDSHIRT, 0, 0}, &Room::sins5UseMedkitOnCrewman },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0xff,           0, 0}, &Room::sins5UseMTricorderAnywhere },

	{ {ACTION_TALK, OBJECT_KIRK,     0, 0, 0}, &Room::sins5TalkToKirk },
	{ {ACTION_TALK, OBJECT_SPOCK,    0, 0, 0}, &Room::sins5TalkToSpock },
	{ {ACTION_TALK, OBJECT_MCCOY,    0, 0, 0}, &Room::sins5TalkToMccoy },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::sins5TalkToRedshirt },

	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_MISSILE_1,  0, 0}, &Room::sins5UseSTricorderOnMissile },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_MISSILE_2,  0, 0}, &Room::sins5UseSTricorderOnMissile },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_MISSILE_3,  0, 0}, &Room::sins5UseSTricorderOnMissile },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_MISSILE_4,  0, 0}, &Room::sins5UseSTricorderOnMissile },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_NORTH_DOOR, 0, 0}, &Room::sins5UseSTricorderOnNorthDoor },

	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_KIRK,     0, 0}, &Room::sins5UseMTricorderOnCrewman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_SPOCK,    0, 0}, &Room::sins5UseMTricorderOnCrewman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_MCCOY,    0, 0}, &Room::sins5UseMTricorderOnCrewman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_REDSHIRT, 0, 0}, &Room::sins5UseMTricorderOnCrewman },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum sins5TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_MOSHER,
	TX_SPEAKER_SCOTT, TX_SPEAKER_COMPUTER,
	TX_SIN5_001, TX_SIN5_002, TX_SIN5_003, TX_SIN5_004, TX_SIN5_005,
	TX_SIN5_006, TX_SIN5_007, TX_SIN5_008, TX_SIN5_009, TX_SIN5_010,
	TX_SIN5_011, TX_SIN5_012, TX_SIN5_013, TX_SIN5_014, TX_SIN5_015,
	TX_SIN5_016, TX_SIN5_017, TX_SIN5_018, TX_SIN5_019, TX_SIN5_020,
	TX_SIN5_021, TX_SIN5_022, TX_SIN5_023, TX_SIN5_024, TX_SIN5_025,
	TX_SIN5_026, TX_SIN5_027, TX_SIN5_028, TX_SIN5_029, TX_SIN5_030,
	TX_SIN5_031, TX_SIN5_032, TX_SIN5_033, TX_SIN5_034, TX_SIN5_035,
	TX_SIN5_036, TX_SIN5_037, TX_SIN5_038, TX_SIN5_039, TX_SIN5_040,
	TX_SIN5_041, TX_SIN5_042, TX_SIN5_043, TX_SIN5_044, TX_SIN5_045,
	TX_SIN5_046, TX_SIN5_S19, TX_SIN5N000, TX_SIN5N001, TX_SIN5N002,
	TX_SIN5N003, TX_SIN5N004, TX_SIN5N005, TX_SIN5N006, TX_SIN5N007,
	TX_SIN5N008, TX_SIN5N009, TX_SIN5N010, TX_SIN5N011, TX_SIN5N012,
	TX_SIN5N013
};

extern const RoomTextOffsets sins5TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 291, 291, 291, 291 },
	{ TX_SPEAKER_MCCOY, 302, 302, 302, 302 },
	{ TX_SPEAKER_SPOCK, 312, 312, 312, 312 },
	{ TX_SPEAKER_MOSHER, 322, 322, 322, 322 },
	{ TX_SPEAKER_SCOTT, 346, 346, 346, 346 },
	{ TX_SPEAKER_COMPUTER, 356, 356, 356, 356 },
	{ TX_SIN5_001, 1654, 1537, 1708, 1648 },
	{ TX_SIN5_002, 7449, 6971, 7751, 7590 },
	{ TX_SIN5_003, 1470, 1383, 1518, 1452 },
	{ TX_SIN5_004, 1562, 1460, 1614, 1550 },
	{ TX_SIN5_005, 3254, 2985, 3460, 3283 },
	{ TX_SIN5_006, 3938, 3825, 4208, 3969 },
	{ TX_SIN5_007, 4382, 4210, 4702, 4447 },
	{ TX_SIN5_008, 3069, 2830, 3251, 3103 },
	{ TX_SIN5_009, 5927, 5649, 6140, 6017 },
	{ TX_SIN5_010, 7984, 7416, 8332, 8127 },
	{ TX_SIN5_011, 8427, 8102, 8880, 8602 },
	{ TX_SIN5_012, 8584, 8245, 9049, 8772 },
	{ TX_SIN5_013, 803, 777, 803, 803 },
	{ TX_SIN5_014, 8958, 8589, 9468, 9151 },
	{ TX_SIN5_015, 1751, 1619, 1802, 1742 },
	{ TX_SIN5_016, 7310, 6847, 7617, 7434 },
	{ TX_SIN5_017, 7201, 6753, 7499, 7313 },
	{ TX_SIN5_018, 7924, 7371, 8259, 8052 },
	{ TX_SIN5_019, 7818, 7280, 8161, 7949 },
	{ TX_SIN5_020, 3843, 3513, 4091, 3880 },
	{ TX_SIN5_021, 4169, 4027, 4455, 4199 },
	{ TX_SIN5_022, 5697, 5450, 5871, 5772 },
	{ TX_SIN5_023, 7621, 7113, 7934, 7759 },
	{ TX_SIN5_024, 8754, 8400, 9244, 8934 },
	{ TX_SIN5_025, 2638, 2429, 2782, 2652 },
	{ TX_SIN5_026, 3341, 3057, 3553, 3373 },
	{ TX_SIN5_027, 2019, 1857, 2087, 2015 },
	{ TX_SIN5_028, 1287, 1216, 1303, 1292 },
	{ TX_SIN5_029, 1119, 1063, 1127, 1120 },
	{ TX_SIN5_030, 942, 901, 943, 939 },
	{ TX_SIN5_031, 5819, 5556, 6005, 5896 },
	{ TX_SIN5_032, 4269, 4112, 4583, 4317 },
	{ TX_SIN5_033, 7722, 7199, 8048, 7854 },
	{ TX_SIN5_034, 8228, 7630, 8634, 8406 },
	{ TX_SIN5_035, 9061, 3585, 9577, 9243 },
	{ TX_SIN5_036, 3673, 3358, 3918, 3721 },
	{ TX_SIN5_037, 4034, 3906, 4315, 4054 },
	{ TX_SIN5_038, 7529, 7036, 7827, 7667 },
	{ TX_SIN5_039, 2234, 2056, 2317, 2233 },
	{ TX_SIN5_040, 2785, 2561, 2937, 2812 },
	{ TX_SIN5_041, 2499, 2306, 2613, 2506 },
	{ TX_SIN5_042, 1887, 1740, 1947, 1887 },
	{ TX_SIN5_043, 4758, 4556, 5079, 4826 },
	{ TX_SIN5_044, 3551, 3251, 3766, 3588 },
	{ TX_SIN5_045, 9305, 7883, 9848, 9547 },
	{ TX_SIN5_046, 8108, 7525, 8476, 8263 },
	{ TX_SIN5_S19, 3149, 2895, 3347, 3185 },
	{ TX_SIN5N000, 6129, 5821, 6371, 6216 },
	{ TX_SIN5N001, 6373, 6045, 6638, 6464 },
	{ TX_SIN5N002, 6462, 6119, 6733, 6553 },
	{ TX_SIN5N003, 6964, 6546, 7254, 7077 },
	{ TX_SIN5N004, 4647, 4460, 4973, 4719 },
	{ TX_SIN5N005, 6017, 5724, 6242, 6111 },
	{ TX_SIN5N006, 6268, 5945, 6530, 6356 },
	{ TX_SIN5N007, 6524, 6166, 6803, 6624 },
	{ TX_SIN5N008, 6709, 6322, 6989, 6817 },
	{ TX_SIN5N009, 6839, 6437, 7129, 6947 },
	{ TX_SIN5N010, 6609, 6236, 6888, 6712 },
	{ TX_SIN5N011, 7092, 6659, 7383, 7190 },
	{ TX_SIN5N012, 5352, 5135, 10094, 5432 },
	{ TX_SIN5N013, 5579, 5347, 5708, 5649 },
	{          -1, 0,    0, 0, 0 }
};

extern const RoomText sins5Texts[] = {
	  { -1, Common::UNK_LANG, "" }
};

void Room::sins5Tick1() {
	playVoc("SIN5LOOP");

	if (!_awayMission->sins.enteredRoom5FirstTime)
		_awayMission->disableInput = 2;

	if (_awayMission->sins.wireConnected1 && _awayMission->sins.wireConnected2)
		loadActorAnim2(OBJECT_CABLE, "s5cabl", 0, 0);

	if (!_awayMission->sins.playedMusicUponEnteringRoom5FirstTime) {
		// NOTE: The music gets drowned out due to a different track being played
		// immediately after.
		playMidiMusicTracks(MIDITRACK_0);
		_awayMission->sins.playedMusicUponEnteringRoom5FirstTime = true;
	}

	playMidiMusicTracks(MIDITRACK_27, kLoopTypeRepeat);
}

void Room::sins5UseSTricorderAnywhere() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_SIN5_013);
}

void Room::sins5UseSTricorderOnRightComputer() {
	if (!_awayMission->sins.gotPointsForScanningRightComputer) {
		_awayMission->sins.missionScore += 1;
		_awayMission->sins.gotPointsForScanningRightComputer = true; // BUGFIX: add this line to prevent infinite score mechanism
	}
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_SIN5_030);
	_awayMission->sins.scannedAndUsedComputers |= 2;
	sins5CheckGatheredAllClues();
}

void Room::sins5UseSTricorderOnLeftComputer() {
	if (!_awayMission->sins.gotPointsForScanningLeftComputer) {
		_awayMission->sins.missionScore += 1;
		_awayMission->sins.gotPointsForScanningLeftComputer = true; // BUGFIX: add this line to prevent infinite score mechanism
	}
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_SIN5_029);
	_awayMission->sins.scannedAndUsedComputers |= 1;
	sins5CheckGatheredAllClues();
}

void Room::sins5UseSTricorderOnMiddleComputer() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_SIN5_028);
}

void Room::sins5UseKirkOnRightComputer() {
	showText(TX_SPEAKER_KIRK, TX_SIN5_003);
	sins5UseSpockOnRightComputer();
}

void Room::sins5UseKirkOnLeftComputer() {
	showText(TX_SPEAKER_KIRK, TX_SIN5_004);
	sins5UseSpockOnLeftComputer();
}

void Room::sins5UseKirkOnMiddleComputer() {
	showText(TX_SPEAKER_KIRK, TX_SIN5_001);
}

void Room::sins5UseMccoyOnComputer() {
	showText(TX_SPEAKER_MCCOY, TX_SIN5_015);
}

void Room::sins5UseRedshirtOnComputer() {
	showText(TX_SPEAKER_MOSHER, TX_SIN5_042);
}

void Room::sins5UseSpockOnMiddleComputer() {
	showText(TX_SPEAKER_SPOCK, TX_SIN5_027);
}

void Room::sins5UseSpockOnRightComputer() {
	if (!_awayMission->sins.gotPointsForUsingRightComputer) {
		_awayMission->sins.missionScore += 1;
		_awayMission->sins.gotPointsForUsingRightComputer = true; // BUGFIX: add this line to prevent infinite score mechanism
	}

	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_S;
	_awayMission->disableInput = true;
	walkCrewmanC(OBJECT_SPOCK, 0xe1, 0xb3, &Room::sins5SpockReachedRightComputer);
}

void Room::sins5SpockReachedRightComputer() {
	loadActorAnimC(OBJECT_SPOCK, "susemn", -1, -1, &Room::sins5SpockUsedRightComputer);
}

void Room::sins5SpockUsedRightComputer() {
	_awayMission->disableInput = false;
	showText(TX_SPEAKER_COMPUTER, TX_SIN5_039);
	_awayMission->sins.scannedAndUsedComputers |= 8;
	sins5CheckGatheredAllClues();
}

void Room::sins5UseSpockOnLeftComputer() {
	if (!_awayMission->sins.gotPointsForUsingLeftComputer) {
		_awayMission->sins.missionScore += 1;
		_awayMission->sins.gotPointsForUsingLeftComputer = true; // BUGFIX: add this line to prevent infinite score mechanism
	}

	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_S;
	_awayMission->disableInput = true;
	walkCrewmanC(OBJECT_SPOCK, 0x40, 0xb3, &Room::sins5SpockReachedLeftComputer);
}

void Room::sins5SpockReachedLeftComputer() {
	loadActorAnimC(OBJECT_SPOCK, "susemn", -1, -1, &Room::sins5SpockUsedLeftComputer);
}

void Room::sins5SpockUsedLeftComputer() {
	if (_awayMission->sins.wireConnected1 && _awayMission->sins.wireConnected2) {
		showText(TX_SPEAKER_COMPUTER, TX_SIN5_041);
		showText(TX_SPEAKER_SPOCK,    TX_SIN5_025);

		walkCrewmanC(OBJECT_KIRK,     0x8c, 0xb5, &Room::sins5CrewmanReadyToBeamOut);
		walkCrewmanC(OBJECT_SPOCK,    0x82, 0xab, &Room::sins5CrewmanReadyToBeamOut);
		walkCrewmanC(OBJECT_MCCOY,    0x96, 0xab, &Room::sins5CrewmanReadyToBeamOut);
		walkCrewmanC(OBJECT_REDSHIRT, 0x91, 0xa1, &Room::sins5CrewmanReadyToBeamOut);
	} else {
		_awayMission->disableInput = false;
		showText(TX_SPEAKER_COMPUTER, TX_SIN5_040);
		_awayMission->sins.scannedAndUsedComputers |= 4;
		sins5CheckGatheredAllClues();
	}
}

void Room::sins5CrewmanReadyToBeamOut() {
	if (++_roomVar.sins.numCrewmenReadyToBeamOut == 4) {
		showText(TX_SPEAKER_KIRK,  TX_SIN5_008);
		showText(TX_SPEAKER_SCOTT, TX_SIN5_S19);
		showText(TX_SPEAKER_KIRK,  TX_SIN5_005);
		_awayMission->sins.missionScore += 19;
		endMission(_awayMission->sins.missionScore, 28, 0);
	}
}

void Room::sins5Tick20() {
	if (!_awayMission->sins.enteredRoom5FirstTime) {
		_awayMission->disableInput = false;
		showText(TX_SPEAKER_SPOCK, TX_SIN5_026);
		_awayMission->sins.enteredRoom5FirstTime = true;
	}
}

// Checks whether both computers have been scanned and interacted with
void Room::sins5CheckGatheredAllClues() {
	if (_awayMission->sins.scannedAndUsedComputers == 0xf && !_awayMission->sins.discoveredComputersOutOfSync) {
		_awayMission->sins.discoveredComputersOutOfSync = true;
		showText(TX_SPEAKER_MOSHER, TX_SIN5_044);
		showText(TX_SPEAKER_SPOCK,  TX_SIN5_036);
		showText(TX_SPEAKER_MCCOY,  TX_SIN5_020);
		showText(TX_SPEAKER_SPOCK,  TX_SIN5_035);
		showText(TX_SPEAKER_KIRK,   TX_SIN5_006);
		showText(TX_SPEAKER_SPOCK,  TX_SIN5_037);
		showText(TX_SPEAKER_MCCOY,  TX_SIN5_021);
		showText(TX_SPEAKER_SPOCK,  TX_SIN5_032);
		showText(TX_SPEAKER_KIRK,   TX_SIN5_007);
	}
}

void Room::sins5UseWireOnComputer() {
	walkCrewmanC(OBJECT_SPOCK, 0x60, 0xab, &Room::sins5KirkOrSpockInPositionToUseWire);
	walkCrewmanC(OBJECT_KIRK,  0xbc, 0xab, &Room::sins5KirkOrSpockInPositionToUseWire);
	_awayMission->disableInput = true;
}

void Room::sins5KirkOrSpockInPositionToUseWire() {
	if (++_roomVar.sins.numCrewmenInPositionForWire == 2) {
		loadActorAnimC(OBJECT_SPOCK, "suselw", -1, -1, &Room::sins5WireConnected);
		loadActorAnim2(OBJECT_KIRK,  "kusele");
	}
}

void Room::sins5WireConnected() {
	if (!_awayMission->sins.wireConnected1) {
		_awayMission->sins.wireConnected1 = true;
		_awayMission->sins.wireConnected2 = true;
		loadActorAnim2(OBJECT_CABLE, "s5cabl", 0, 0);
		showDescription(TX_SIN5N004);
		loseItem(OBJECT_ICONECT);
		playMidiMusicTracks(MIDITRACK_30);
	}
	_awayMission->disableInput = false;
}

void Room::sins5UseStunPhaserOnComputer() {
	showText(TX_SPEAKER_MOSHER, TX_SIN5_043);
}


void Room::sins5UseKillPhaserOnLeftComputer() {
	_awayMission->disableInput = true;
	walkCrewmanC(OBJECT_KIRK, 0x84, 0xbc, &Room::sins5ReachedPositionToShootLeftComputer);
}

void Room::sins5ReachedPositionToShootLeftComputer() {
	loadActorAnimC(OBJECT_KIRK, "kdraww", -1, -1, &Room::sins5DrewPhaserToShootLeftComputer);
}

void Room::sins5DrewPhaserToShootLeftComputer() {
	loadActorAnimC(OBJECT_LEFT_COMPUTER_EXPLOSION, "s5phal", 0, 0xaf, &Room::sins5ComputerLaunchesMissiles);
	playSoundEffectIndex(kSfxPhaser);
	_awayMission->timers[3] = 10;
	_awayMission->timers[0] = 24;
}

void Room::sins5Timer0Expired() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_W;
	loadActorStandAnim(OBJECT_KIRK);
	_awayMission->disableInput = false;
}


void Room::sins5UseKillPhaserOnMiddleComputer() {
	_awayMission->disableInput = true;
	walkCrewmanC(OBJECT_KIRK, 0x8b, 0xbc, &Room::sins5ReachedPositionToShootMiddleComputer);
}

void Room::sins5ReachedPositionToShootMiddleComputer() {
	loadActorAnimC(OBJECT_KIRK, "kfiren", -1, -1, &Room::sins5DrewPhaserToShootMiddleComputer);
}

void Room::sins5DrewPhaserToShootMiddleComputer() {
	loadActorAnimC(OBJECT_MIDDLE_COMPUTER_EXPLOSION, "s5phac", 0, 0x8c, &Room::sins5ComputerLaunchesMissiles);
	playSoundEffectIndex(kSfxPhaser);
	_awayMission->timers[3] = 10;
	_awayMission->timers[1] = 24;
}

void Room::sins5Timer1Expired() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_N;
	loadActorStandAnim(OBJECT_KIRK);
	_awayMission->disableInput = false;
}


void Room::sins5UseKillPhaserOnRightComputer() {
	_awayMission->disableInput = true;
	walkCrewmanC(OBJECT_KIRK, 0x9e, 0xbc, &Room::sins5ReachedPositionToShootRightComputer);
}

void Room::sins5ReachedPositionToShootRightComputer() {
	loadActorAnimC(OBJECT_KIRK, "kdrawe", -1, -1, &Room::sins5DrewPhaserToShootRightComputer);
}

void Room::sins5DrewPhaserToShootRightComputer() {
	loadActorAnimC(OBJECT_RIGHT_COMPUTER_EXPLOSION, "s5phar", 0, 0xaf, &Room::sins5ComputerLaunchesMissiles);
	playSoundEffectIndex(kSfxPhaser);
	_awayMission->timers[3] = 10;
	_awayMission->timers[2] = 24;
}

void Room::sins5Timer2Expired() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_E;
	loadActorStandAnim(OBJECT_KIRK);
	_awayMission->disableInput = false;
}

void Room::sins5ComputerLaunchesMissiles() {
	playMidiMusicTracks(MIDITRACK_2);
	showDescription(TX_SIN5N012);
	showDescription(TX_SIN5N013);
	showGameOverMenu();
}

void Room::sins5Timer3Expired() {
}

void Room::sins5LookAnywhere() {
	showDescription(TX_SIN5N005);
}

void Room::sins5LookAtKirk() {
	showDescription(TX_SIN5N000);
}

void Room::sins5LookAtSpock() {
	showDescription(TX_SIN5N006);
}

void Room::sins5LookAtMccoy() {
	showDescription(TX_SIN5N001);
}

void Room::sins5LookAtRedshirt() {
	showDescription(TX_SIN5N002);
}

void Room::sins5LookAtLight() {
	showDescription(TX_SIN5N007);
}

void Room::sins5LookAtLeftComputer() {
	showDescription(TX_SIN5N010);
}

void Room::sins5LookAtMiddleComputer() {
	showDescription(TX_SIN5N008);
}

void Room::sins5LookAtRightComputer() {
	showDescription(TX_SIN5N009);
}

void Room::sins5LookAtMissile() {
	showDescription(TX_SIN5N003);
}

void Room::sins5LookAtNorthDoor() {
	showDescription(TX_SIN5N011);
}

void Room::sins5UseMedkitOnCrewman() {
	showText(TX_SPEAKER_MCCOY, TX_SIN5_017);
}

void Room::sins5UseMTricorderAnywhere() {
	mccoyScan(DIR_S, TX_SPEAKER_MCCOY, TX_SIN5_016);
	if (!_awayMission->sins.gotPointsForScanningRoom5) {
		_awayMission->sins.missionScore += 1;
		_awayMission->sins.gotPointsForScanningRoom5 = true; // BUGFIX: add this line to prevent infinite score mechanism
	}
}

void Room::sins5TalkToKirk() {
	showText(TX_SPEAKER_KIRK, TX_SIN5_002);
}

void Room::sins5TalkToSpock() {
	showText(TX_SPEAKER_SPOCK, TX_SIN5_038);
	showText(TX_SPEAKER_MCCOY, TX_SIN5_023);
	showText(TX_SPEAKER_SPOCK, TX_SIN5_033);
	showText(TX_SPEAKER_MCCOY, TX_SIN5_019);
}

void Room::sins5TalkToMccoy() {
	showText(TX_SPEAKER_MCCOY, TX_SIN5_018);
	showText(TX_SPEAKER_KIRK,  TX_SIN5_010);
}

void Room::sins5TalkToRedshirt() {
	showText(TX_SPEAKER_MOSHER, TX_SIN5_046);
	showText(TX_SPEAKER_SPOCK,  TX_SIN5_034);
	showText(TX_SPEAKER_MOSHER, TX_SIN5_045);
	showText(TX_SPEAKER_KIRK,   TX_SIN5_011);
}

void Room::sins5UseSTricorderOnMissile() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_SIN5_012);
}

void Room::sins5UseSTricorderOnNorthDoor() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_SIN5_024);
}

void Room::sins5UseMTricorderOnCrewman() {
	mccoyScan(DIR_S, TX_SPEAKER_SPOCK, TX_SIN5_014);
}

}
