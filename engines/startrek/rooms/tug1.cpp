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

#define OBJECT_JUNKPILE 8
#define OBJECT_PHASERSHOT 9

#define HOTSPOT_BRIDGEDOOR 0x21
#define HOTSPOT_BRIDGEARCH 0x22
#define HOTSPOT_TERMINAL   0x23
#define HOTSPOT_DEBRIS     0x24
#define HOTSPOT_BRIGDOOR   0x25

extern const RoomAction tug1ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::tug1Tick1 },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0xff, 0, 0}, &Room::tug1UseSTricorderOnAnything },
	{ {ACTION_LOOK, 0x21, 0, 0, 0}, &Room::tug1LookAtBridgeDoor },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x21, 0, 0}, &Room::tug1UseSTricorderOnBridgeDoor },
	{ {ACTION_USE, OBJECT_IPHASERS, 0x21, 0, 0}, &Room::tug1UsePhaserOnBridgeDoor },
	{ {ACTION_USE, OBJECT_IPHASERK, 0x21, 0, 0}, &Room::tug1UsePhaserOnBridgeDoor },
	{ {ACTION_FINISHED_WALKING, 3, 0, 0, 0}, &Room::tug1KirkReachedFiringPosition },
	{ {ACTION_FINISHED_ANIMATION, 4, 0, 0, 0}, &Room::tug1KirkPulledOutPhaser },
	{ {ACTION_FINISHED_ANIMATION, 5, 0, 0, 0}, &Room::tug1KirkFinishedFiringPhaser },
	{ {ACTION_TALK, OBJECT_SPOCK, 0, 0, 0}, &Room::tug1TalkToSpock },
	{ {ACTION_USE, OBJECT_ISTRICOR, 8, 0, 0}, &Room::tug1UseSTricorderOnJunkPile },
	{ {ACTION_LOOK, 8, 0, 0, 0}, &Room::tug1LookAtJunkPile },
	{ {ACTION_GET, 8, 0, 0, 0}, &Room::tug1GetJunkPile },
	{ {ACTION_FINISHED_WALKING, 1, 0, 0, 0}, &Room::tug1KirkReachedJunkPile },
	{ {ACTION_FINISHED_ANIMATION, 2, 0, 0, 0}, &Room::tug1KirkFinishedTakingJunkPile },

	{ {ACTION_USE, OBJECT_IPHASERS, OBJECT_IPWE, 0, 0}, &Room::tug1UsePhaserOnWelder },
	{ {ACTION_USE, OBJECT_IPHASERK, OBJECT_IPWE, 0, 0}, &Room::tug1UsePhaserOnWelder },
	{ {ACTION_USE, OBJECT_IPWF, OBJECT_IWIRSCRP, 0, 0}, &Room::tug1UseWelderOnWireScraps },
	{ {ACTION_USE, OBJECT_IPWF, OBJECT_IJNKMETL, 0, 0}, &Room::tug1UseWelderOnMetalScraps },
	{ {ACTION_USE, OBJECT_ICOMBBIT, OBJECT_IRT, 0, 0}, &Room::tug1UseCombBitOnTransmogrifier },

	{ {ACTION_USE, OBJECT_IPWF, 0x22, 0, 0}, &Room::tug1UsePhaserWelderOnBridgeDoor },
	{ {ACTION_USE, OBJECT_IPWF, 0x21, 0, 0}, &Room::tug1UsePhaserWelderOnBridgeDoor },
	{ {ACTION_USE, OBJECT_IPWF, 0x20, 0, 0}, &Room::tug1UsePhaserWelderOnBridgeDoorInLeftSpot },
	{ {ACTION_FINISHED_WALKING, 6, 0, 0, 0}, &Room::tug1KirkReachedBridgeDoorWithWelder },
	{ {ACTION_FINISHED_ANIMATION, 7, 0, 0, 0}, &Room::tug1KirkFinishedUsingWelder },
	{ {ACTION_FINISHED_WALKING, 8, 0, 0, 0}, &Room::tug1KirkReachedBridgeDoorWithWelderInLeftSpot },
	{ {ACTION_FINISHED_ANIMATION, 9, 0, 0, 0}, &Room::tug1KirkFinishedUsingWelderInLeftSpot },
	{ {ACTION_LOOK, 0xff, 0, 0, 0}, &Room::tug1LookAnywhere },
	{ {ACTION_LOOK, OBJECT_MCCOY, 0, 0, 0}, &Room::tug1LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_SPOCK, 0, 0, 0}, &Room::tug1LookAtSpock },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::tug1LookAtRedshirt },
	{ {ACTION_TALK, OBJECT_MCCOY, 0, 0, 0}, &Room::tug1TalkToMccoy },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::tug1TalkToRedshirt },
	{ {ACTION_LOOK, 0x23, 0, 0, 0}, &Room::tug1LookAtTerminal },
	{ {ACTION_LOOK, 0x24, 0, 0, 0}, &Room::tug1LookAtDebris },
	{ {ACTION_LOOK, 0x25, 0, 0, 0}, &Room::tug1LookAtBrigDoor },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x25, 0, 0}, &Room::tug1UseSTricorderOnBrigDoor },
	{ {ACTION_TALK, OBJECT_KIRK, 0, 0, 0}, &Room::tug1TalkToKirk },
	{ {ACTION_USE, OBJECT_ICOMM, 0xff, 0, 0}, &Room::tug1UseCommunicator },
	{ {ACTION_WALK, 0x21, 0, 0, 0}, &Room::tug1WalkToBridgeDoor },
	{ {ACTION_FINISHED_WALKING, 10, 0, 0, 0}, &Room::tug1KirkReachedBridgeDoor },
	{ {ACTION_FINISHED_ANIMATION, 11, 0, 0, 0}, &Room::tug1BridgeDoorOpened },
	{ {ACTION_WALK, 0x25, 0, 0, 0}, &Room::tug1WalkToBrigDoor },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0xff, 0, 0}, &Room::tug1UseMTricorderAnywhere },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0x21, 0, 0}, &Room::tug1UseMTricorderOnBridgeDoor },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0x20, 0, 0}, &Room::tug1UseMTricorderOnBridgeDoor },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0x22, 0, 0}, &Room::tug1UseMTricorderOnBridgeDoor },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0x25, 0, 0}, &Room::tug1UseMTricorderOnBrigDoor },
	{ {ACTION_USE, OBJECT_SPOCK, 0x20, 0, 0}, &Room::tug1UseSpockOnBridgeDoor },
	{ {ACTION_USE, OBJECT_SPOCK, 0x21, 0, 0}, &Room::tug1UseSpockOnBridgeDoor },
	{ {ACTION_USE, OBJECT_SPOCK, 0x22, 0, 0}, &Room::tug1UseSpockOnBridgeDoor },
	{ {ACTION_USE, OBJECT_REDSHIRT, 0x20, 0, 0}, &Room::tug1UseRedshirtOnBridgeDoor },
	{ {ACTION_USE, OBJECT_REDSHIRT, 0x21, 0, 0}, &Room::tug1UseRedshirtOnBridgeDoor },
	{ {ACTION_USE, OBJECT_REDSHIRT, 0x22, 0, 0}, &Room::tug1UseRedshirtOnBridgeDoor },
	{ {ACTION_USE, OBJECT_IMEDKIT, 0x20, 0, 0}, &Room::tug1UseMedkitOnBridgeDoor },
	{ {ACTION_USE, OBJECT_IMEDKIT, 0x21, 0, 0}, &Room::tug1UseMedkitOnBridgeDoor },
	{ {ACTION_USE, OBJECT_IMEDKIT, 0x22, 0, 0}, &Room::tug1UseMedkitOnBridgeDoor },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum tug1TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_CHRISTENSEN,
	TX_TUG1_001, TX_TUG1_002, TX_TUG1_003, TX_TUG1_004, TX_TUG1_005,
	TX_TUG1_006, TX_TUG1_007, TX_TUG1_008, TX_TUG1_009, TX_TUG1_010,
	TX_TUG1_011, TX_TUG1_012, TX_TUG1_013, TX_TUG1_014, TX_TUG1_015,
	TX_TUG1_016, TX_TUG1_017, TX_TUG1_018, TX_SIN3_012, TX_TUG1L000,
	TX_TUG1L005, TX_TUG1N000, TX_TUG1N001, TX_TUG1N002, TX_TUG1N003,
	TX_TUG1N004, TX_TUG1N005, TX_TUG1N006, TX_TUG1N007, TX_TUG1N008,
	TX_TUG1N009, TX_TUG1N010, TX_TUG1N011, TX_TUG1N013
};

extern const RoomTextOffsets tug1TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 5549, 5045, 5789, 5545 },
	{ TX_SPEAKER_MCCOY, 5560, 5056, 5800, 5556 },
	{ TX_SPEAKER_SPOCK, 5570, 5066, 5810, 5566 },
	{ TX_SPEAKER_CHRISTENSEN, 5580, 5076, 5820, 5576 },
	{ TX_TUG1_001, 4050, 3689, 4172, 4082 },
	{ TX_TUG1_002,  751, 695, 759, 753 },
	{ TX_TUG1_003, 4598, 4192, 4745, 4623 },
	{ TX_TUG1_004, 5380, 4891, 5621, 5377 },
	{ TX_TUG1_005, 4920, 4480, 5106, 4928 },
	{ TX_TUG1_006, 5788, 5224, 6048, 5797 },
	{ TX_TUG1_007, 4764, 4340, 4925, 4790 },
	{ TX_TUG1_008, 4106, 3730, 4227, 4140 },
	{ TX_TUG1_009, 1461, 1359, 1504, 1474 },
	{ TX_TUG1_010, 1233, 1147, 1231, 1247 },
	{ TX_TUG1_011, 4223, 3832, 4345, 4252 },
	{ TX_TUG1_012, 2253, 2089, 2336, 2302 },
	{ TX_TUG1_013, 5098, 4639, 5302, 5111 },
	{ TX_TUG1_014,  482, 456, 482, 482 },
	{ TX_TUG1_015, 3075, 2819, 3177, 3127 },
	{ TX_TUG1_016, 2895, 2663, 3004, 2950 },
	{ TX_TUG1_017, 2426, 2247, 2539, 2484 },
	{ TX_TUG1_018, 3872, 3526, 3989, 3900 },
	{ TX_SIN3_012, 5745, 0, 5987, 5755 },
	{ TX_TUG1L000, 5267, 4793, 5504, 5263 },
	{ TX_TUG1L005, 3312, 3026, 3418, 3348 },
	{ TX_TUG1N000, 1620, 1502, 1683, 1653 },
	{ TX_TUG1N001, 3693, 3362, 3817, 3723 },
	{ TX_TUG1N002, 3213, 2942, 3315, 3262 },
	{ TX_TUG1N003, 5596, 5092, 5836, 5592 },
	{ TX_TUG1N004, 2040, 1907, 2132, 2094 },
	{ TX_TUG1N005,  606, 565, 612, 608 },
	{ TX_TUG1N006, 5638, 5119, 5884, 5633 },
	{ TX_TUG1N007, 2673, 2465, 2790, 2733 },
	//{ TX_TUG1N007, 1017, 0, 0, 0 },	 // ignore duplicate text
	{ TX_TUG1N008, 2552, 2359, 2677, 2617 },
	{ TX_TUG1N009, 2107, 1959, 2193, 2178 },
	{ TX_TUG1N010, 3489, 3188, 3591, 3545 },
	{ TX_TUG1N011, 3594, 3278, 3725, 3651 },
	{ TX_TUG1N013, 5690, 5156, 5927, 5679 },
	{          -1, 0,    0, 0, 0 }
};

extern const RoomText tug1Texts[] = {
	{ -1, Common::UNK_LANG, "" }
};

void Room::tug1Tick1() {
	playVoc("TUG1LOOP");

	if (!_awayMission->tug.gotJunkPile)
		loadActorAnim2(OBJECT_JUNKPILE, "jnkpil", 0xfd, 0xa0, 0);
}

void Room::tug1UseSTricorderOnAnything() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_N;
	loadActorAnim2(OBJECT_SPOCK, "sscann", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_TUG1_014);

	// NOTE: this action has a second implementation (which is never called). It displayed
	// TX_TUG1_006. Was this meant to be displayed after the force field is down?
}

void Room::tug1LookAtBridgeDoor() {
	if (_awayMission->tug.bridgeForceFieldDown)
		return;
	showDescription(TX_TUG1N005);
}

void Room::tug1UseSTricorderOnBridgeDoor() {
	if (_awayMission->tug.bridgeForceFieldDown)
		return;

	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_N;
	loadActorAnim2(OBJECT_SPOCK, "sscann", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_TUG1_002);
}

void Room::tug1UsePhaserOnBridgeDoor() {
	if (_awayMission->tug.bridgeForceFieldDown)
		showDescription(TX_TUG1N007);
	else {
		_awayMission->disableInput = true;
		_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_N;
		walkCrewman(OBJECT_KIRK, 0xbe, 0x78, 3);
	}
}

void Room::tug1KirkReachedFiringPosition() {
	loadActorAnim2(OBJECT_KIRK, "kfiren", -1, -1, 4);
}

void Room::tug1KirkPulledOutPhaser() {
	loadActorAnim2(OBJECT_PHASERSHOT, "t1phas", 0, 0, 5);
	playSoundEffectIndex(kSfxPhaser);
}

void Room::tug1KirkFinishedFiringPhaser() {
	_awayMission->disableInput = false;
	loadActorStandAnim(OBJECT_KIRK);
	loadActorStandAnim(OBJECT_PHASERSHOT);
}

void Room::tug1TalkToSpock() {
	if (!_awayMission->tug.bridgeForceFieldDown)
		showText(TX_SPEAKER_SPOCK, TX_TUG1_010);
}

void Room::tug1UseSTricorderOnJunkPile() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_N;
	loadActorAnim2(OBJECT_SPOCK, "sscann", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_TUG1_009);
}

void Room::tug1LookAtJunkPile() {
	showDescription(TX_TUG1N000);
}

void Room::tug1GetJunkPile() {
	walkCrewman(OBJECT_KIRK, 0xe1, 0xaa, 1);
}

void Room::tug1KirkReachedJunkPile() {
	loadActorAnim2(OBJECT_KIRK, "kpicke", 0xe1, 0xaa, 2);
}

void Room::tug1KirkFinishedTakingJunkPile() {
	_awayMission->tug.gotJunkPile = true;
	giveItem(OBJECT_IDEADPH);
	giveItem(OBJECT_IPWE);
	_awayMission->tug.missionScore++;
	giveItem(OBJECT_IWIRSCRP);
	giveItem(OBJECT_IJNKMETL);

	loadActorStandAnim(OBJECT_JUNKPILE);
	loadActorStandAnim(OBJECT_KIRK);
}

void Room::tug1UsePhaserOnWelder() {
	loseItem(OBJECT_IPWE);
	giveItem(OBJECT_IPWF);
	showDescription(TX_TUG1N004);

	_awayMission->tug.missionScore += 3;
}

void Room::tug1UseWelderOnWireScraps() {
	showDescription(TX_TUG1N009);
	loseItem(OBJECT_IWIRSCRP);
}

void Room::tug1UseWelderOnMetalScraps() {
	showText(TX_SPEAKER_SPOCK, TX_TUG1_012);
	loseItem(OBJECT_IJNKMETL);
	giveItem(OBJECT_ICOMBBIT);
}

void Room::tug1UseCombBitOnTransmogrifier() {
	showText(TX_SPEAKER_SPOCK, TX_TUG1_017);
	loseItem(OBJECT_ICOMBBIT);
	loseItem(OBJECT_IRT);
	giveItem(OBJECT_IRTWB);
}

void Room::tug1UsePhaserWelderOnBridgeDoor() {
	if (_awayMission->tug.bridgeForceFieldDown)
		showDescription(TX_TUG1N008);
	else {
		_awayMission->disableInput = true;
		_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_N;
		walkCrewman(OBJECT_KIRK, 0xc2, 0x6a, 6);
	}
}

void Room::tug1UsePhaserWelderOnBridgeDoorInLeftSpot() {
	if (_awayMission->tug.bridgeForceFieldDown)
		showDescription(TX_TUG1N007);
	else {
		_awayMission->disableInput = true;
		_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_N;
		walkCrewman(OBJECT_KIRK, 0xb1, 0x6a, 8);
	}
}

void Room::tug1KirkReachedBridgeDoorWithWelder() {
	playVoc("MUR4E9S");
	loadActorAnim2(OBJECT_KIRK, "kusemn", -1, -1, 0);
	loadActorAnim2(OBJECT_PHASERSHOT, "t1weld", 0, 0, 7);
}

void Room::tug1KirkFinishedUsingWelder() {
	loadActorStandAnim(OBJECT_PHASERSHOT);
	showText(TX_SPEAKER_SPOCK, TX_TUG1_016);
	_awayMission->disableInput = false;
}

void Room::tug1KirkReachedBridgeDoorWithWelderInLeftSpot() {
	playVoc("MUR4E9S");
	loadActorAnim2(OBJECT_KIRK, "kuseln", -1, -1, 0);
	loadActorAnim2(OBJECT_PHASERSHOT, "t1weld", 0, 0, 9);
}

void Room::tug1KirkFinishedUsingWelderInLeftSpot() {
	loadActorStandAnim(OBJECT_PHASERSHOT);
	showText(TX_SPEAKER_SPOCK, TX_TUG1_015);
	_awayMission->tug.bridgeForceFieldDown = true;
	_awayMission->disableInput = false;
	_awayMission->tug.missionScore++;
}

void Room::tug1LookAnywhere() {
	showDescription(TX_TUG1N013);
}

void Room::tug1LookAtMccoy() {
	showDescription(TX_TUG1N003);
}

void Room::tug1LookAtSpock() {
	showDescription(TX_TUG1N006);
}

void Room::tug1LookAtRedshirt() {
	showDescription(TX_TUG1N002);
}

void Room::tug1TalkToMccoy() {
	showText(TX_SPEAKER_MCCOY, TX_SIN3_012); // NOTE: uses "SIN3" text?
}

void Room::tug1TalkToRedshirt() {
	showText(TX_SPEAKER_CHRISTENSEN, TX_TUG1L005);
}

void Room::tug1LookAtTerminal() {
	showDescription(TX_TUG1N010);
}

void Room::tug1LookAtDebris() {
	showDescription(TX_TUG1N011);
}

void Room::tug1LookAtBrigDoor() {
	showDescription(TX_TUG1N001);
}

void Room::tug1UseSTricorderOnBrigDoor() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_N;
	loadActorAnim2(OBJECT_SPOCK, "sscann", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_SPOCK, TX_TUG1_018);
}

void Room::tug1TalkToKirk() {
	showText(TX_SPEAKER_KIRK,  TX_TUG1_001);
	showText(TX_SPEAKER_MCCOY, TX_TUG1_008);
}

void Room::tug1UseCommunicator() {
	showText(TX_SPEAKER_SPOCK, TX_TUG1_011);
}

void Room::tug1WalkToBridgeDoor() {
	if (!_awayMission->tug.bridgeForceFieldDown)
		return;
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_N;
	walkCrewman(OBJECT_KIRK, 0xc2, 0x6e, 10);
}

void Room::tug1KirkReachedBridgeDoor() {
	_awayMission->disableInput = true;
	loadActorAnim2(OBJECT_PHASERSHOT, "h1do", 0, 0, 11);
}

void Room::tug1BridgeDoorOpened() {
	walkCrewman(OBJECT_KIRK, 0xc2, 0x63, 0);
}

void Room::tug1WalkToBrigDoor() {
	walkCrewman(OBJECT_KIRK, 0xe9, 0x81, 0);
}

void Room::tug1UseMTricorderAnywhere() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_N;
	loadActorAnim2(OBJECT_MCCOY, "mscann", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_MCCOY, TX_TUG1_003);
}

void Room::tug1UseMTricorderOnBridgeDoor() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_N;
	loadActorAnim2(OBJECT_MCCOY, "mscann", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_MCCOY, TX_TUG1_007);
}

void Room::tug1UseMTricorderOnBrigDoor() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_N;
	loadActorAnim2(OBJECT_MCCOY, "mscann", -1, -1, 0);
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_MCCOY, TX_TUG1_005);
}

void Room::tug1UseSpockOnBridgeDoor() {
	if (!_awayMission->tug.bridgeForceFieldDown)
		showText(TX_SPEAKER_SPOCK, TX_TUG1_013);
}

void Room::tug1UseRedshirtOnBridgeDoor() {
	if (!_awayMission->tug.bridgeForceFieldDown)
		showText(TX_SPEAKER_CHRISTENSEN, TX_TUG1L000);
}

void Room::tug1UseMedkitOnBridgeDoor() {
	if (!_awayMission->tug.bridgeForceFieldDown)
		showText(TX_SPEAKER_MCCOY, TX_TUG1_004);
}

}
