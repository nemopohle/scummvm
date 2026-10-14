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

#define OBJECT_ELASI_1 8
#define OBJECT_ELASI_2 9
#define OBJECT_ELASI_3 10
#define OBJECT_ELASI_4 11
#define OBJECT_12 12
#define OBJECT_13 13
#define OBJECT_14 14
#define OBJECT_15 15

#define GUARDSTAT_UP 0
#define GUARDSTAT_STUNNED 1
#define GUARDSTAT_DEAD 2
#define GUARDSTAT_SURRENDERED 4
#define GUARDSTAT_TIED 8

extern const RoomAction tug3ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::tug3Tick1 },
	{ {ACTION_TICK, 40, 0, 0, 0}, &Room::tug3Tick40 },
	{ {ACTION_LOOK, 0xff, 0, 0, 0}, &Room::tug3LookAnywhere },
	{ {ACTION_USE, OBJECT_IPHASERS, 0xff, 0, 0}, &Room::tug3UsePhaserAnywhere },
	{ {ACTION_USE, OBJECT_IPHASERK, 0xff, 0, 0}, &Room::tug3UsePhaserAnywhere },
	{ {ACTION_USE, OBJECT_IPHASERS, 8, 0, 0}, &Room::tug3UseStunPhaserOnElasi1 },
	{ {ACTION_USE, OBJECT_IPHASERS, 9, 0, 0}, &Room::tug3UseStunPhaserOnElasi2 },
	{ {ACTION_USE, OBJECT_IPHASERS, 10, 0, 0}, &Room::tug3UseStunPhaserOnElasi3 },
	{ {ACTION_USE, OBJECT_IPHASERS, 11, 0, 0}, &Room::tug3UseStunPhaserOnElasi4 },
	{ {ACTION_USE, OBJECT_IPHASERK, 8, 0, 0}, &Room::tug3UseKillPhaserOnElasi1 },
	{ {ACTION_USE, OBJECT_IPHASERK, 9, 0, 0}, &Room::tug3UseKillPhaserOnElasi2 },
	{ {ACTION_USE, OBJECT_IPHASERK, 10, 0, 0}, &Room::tug3UseKillPhaserOnElasi3 },
	{ {ACTION_USE, OBJECT_IPHASERK, 11, 0, 0}, &Room::tug3UseKillPhaserOnElasi4 },
	{ {ACTION_FINISHED_ANIMATION, 12, 0, 0, 0}, &Room::tug3ElasiStunnedOrKilled },
	{ {ACTION_TALK, 8, 0, 0, 0}, &Room::tug3TalkToElasi1 },
	{ {ACTION_FINISHED_ANIMATION, 3, 0, 0, 0}, &Room::tug3Elasi1DrewPhaser },
	{ {ACTION_FINISHED_ANIMATION, 4, 0, 0, 0}, &Room::tug3Elasi1ShotConsole },
	{ {ACTION_FINISHED_ANIMATION, 5, 0, 0, 0}, &Room::tug3Elasi1DrewPhaser2 },
	{ {ACTION_FINISHED_ANIMATION, 6, 0, 0, 0}, &Room::tug3Elasi1ShotConsoleAndSurrenders },
	{ {ACTION_LOOK, OBJECT_MCCOY, 0, 0, 0}, &Room::tug3LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_SPOCK, 0, 0, 0}, &Room::tug3LookAtSpock },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::tug3LookAtRedshirt },
	{ {ACTION_LOOK, 8, 0, 0, 0}, &Room::tug3LookAtElasi1 },
	{ {ACTION_LOOK, 9, 0, 0, 0}, &Room::tug3LookAtElasi2 },
	{ {ACTION_LOOK, 10, 0, 0, 0}, &Room::tug3LookAtElasi3 },
	{ {ACTION_LOOK, 11, 0, 0, 0}, &Room::tug3LookAtElasi4 },
	{ {ACTION_TALK, OBJECT_MCCOY, 0, 0, 0}, &Room::tug3TalkToMccoy },
	{ {ACTION_TALK, OBJECT_SPOCK, 0, 0, 0}, &Room::tug3TalkToSpock },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::tug3TalkToRedshirt },
	{ {ACTION_USE, OBJECT_ICOMM, 0xff, 0, 0}, &Room::tug3UseCommunicator },
	{ {ACTION_TIMER_EXPIRED, 0, 0, 0, 0}, &Room::tug3Timer0Expired },
	{ {ACTION_FINISHED_ANIMATION, 13, 0, 0, 0}, &Room::tug3AllCrewmenDead },
	{ {ACTION_TIMER_EXPIRED, 1, 0, 0, 0}, &Room::tug3Timer1Expired },
	{ {ACTION_FINISHED_ANIMATION, 7, 0, 0, 0}, &Room::tug3SecurityTeamBeamedIn },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum tug3TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_SULU, TX_SPEAKER_SHIPS_COMPUTER,
	TX_SPEAKER_CHRISTENSEN,	TX_SPEAKER_ELASI_CERETH, TX_SPEAKER_ELASI_CLANSMAN, TX_SPEAKER_STARFLEET, TX_SPEAKER_SCOTT,
	TX_TUG3_001, TX_TUG3_002, TX_TUG3_003, TX_TUG3_004, TX_TUG3_005,
	TX_TUG3_006, TX_TUG3_007, TX_TUG3_008, TX_TUG3_009, TX_TUG3_011,
	TX_TUG3_013, TX_TUG3_015, TX_TUG3_F27, TX_TUG3_S07, TX_TUG3_S08,
	TX_TUG3L003, TX_TUG3L080, TX_TUG3L081, TX_TUG3L083, TX_TUG3L084,
	TX_TUG3N000, TX_TUG3N001, TX_TUG3N002, TX_TUG3N003, TX_TUG3N004,
	TX_TUG3N005, TX_TUG3N006, TX_TUG3N007, TX_TUG3N008, TX_COMPA180,
	TX_COMPU181, TX_COMPU182, TX_TUG3_012
};

extern const RoomTextOffsets tug3TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 5426, 5136, 5748, 5748, 5167, 5393, 5401 },
	{ TX_SPEAKER_MCCOY, 5437, 5147, 5759, 5759, 5178, 5406, 5412 },
	{ TX_SPEAKER_SPOCK, 5447, 5157, 5769, 5769, 5188, 5416, 5422 },
	{ TX_SPEAKER_SULU, 5457, 5167, 5779, 5779, 5198, 5426, 5432 },
	{ TX_SPEAKER_SHIPS_COMPUTER, 5466, 5176, 5788, 5788, 5207, 5435, 5441 },
	{ TX_SPEAKER_CHRISTENSEN, 5482, 5192, 5804, 5804, 5223, 5450, 5457 },
	{ TX_SPEAKER_ELASI_CERETH, 5498, 5208, 5820, 5820, 5239, 5466, 5473 },
	{ TX_SPEAKER_ELASI_CLANSMAN, 5511, 5221, 5833, 5833, 5252, 5479, 5486 },
	{ TX_SPEAKER_STARFLEET, 5526, 5236, 5848, 5848, 5267, 5494, 5501 },
	{ TX_SPEAKER_SCOTT, 5536, 5246, 5858, 5858, 5277, 5504, 5511 },
	{ TX_TUG3_001, 4914, 4670, 5212, 5208, 4684, 4889, 4894 },
	{ TX_TUG3_002, 6171, 5791, 6576, 6547, 5822, 6074, 6110 },
	{ TX_TUG3_003, 6121, 5726, 6461, 6447, 5757, 5988, 6009 },
	{ TX_TUG3_004, 6026, 5646, 6356, 6341, 5677, 5897, 5919 },
	{ TX_TUG3_005, 5224, 4965, 5538, 5541, 4995, 5216, 5221 },
	{ TX_TUG3_006, 3810, 3642, 4046, 4054, 3657, 3811, 3804 },
	{ TX_TUG3_007, 3553, 3430, 3760, 3786, 3446, 3578, 3563 },
	{ TX_TUG3_008, 3349, 3256, 3523, 3565, 3256, 3371, 3341 },
	{ TX_TUG3_009, 3231, 3153, 3399, 3438, 3153, 3254, 3232 },
	{ TX_TUG3_011, 3117, 3054, 3269, 3313, 3054, 3144, 3117 },
	{ TX_TUG3_013, 3042, 2921, 3095, 3126, 2921, 2981, 2973 },
	{ TX_TUG3_015, 3618, 3480, 3829, 3853, 3496, 3630, 3617 },
	{ TX_TUG3_F27, 2224, 2173, 2251, 2261, 2173, 2206, 2200 },
	{ TX_TUG3_S07, 3867, 3684, 4101, 4116, 3699, 3858, 3844 },
	{ TX_TUG3_S08, 5282, 5008, 5602, 5604, 5038, 5264, 5270 },
	{ TX_TUG3L003, 3470, 3362, 3657, 3698, 3362, 3489, 3460 },
	{ TX_TUG3L080, 2701, 2610, 2742, 2767, 2610, 2652, 2650 },
	{ TX_TUG3L081,  792, 771, 808, 812, 771, 787, 787 },
	{ TX_TUG3L083, 2480, 2414, 2517, 2539, 2414, 2449, 2450 },
	{ TX_TUG3L084, 2057, 2021, 2076, 2085, 2021, 2045, 2040 },
	{ TX_TUG3N000, 5824, 5474, 6166, 6142, 5505, 5728, 5759 },
	{ TX_TUG3N001, 2845, 2739, 2897, 2925, 2739, 2795, 2790 },
	{ TX_TUG3N002, 5873, 5508, 6212, 6194, 5539, 5765, 5790 },
	{ TX_TUG3N003, 5634, 5329, 5964, 5953, 5360, 5584, 5602 },
	{ TX_TUG3N004, 5751, 5416, 6088, 6070, 5447, 5671, 5696 },
	{ TX_TUG3N005, 5694, 5374, 6027, 6017, 5405, 5633, 5650 },
	{ TX_TUG3N006, 4725, 4496, 4992, 4991, 4512, 4687, 4689 },
	{ TX_TUG3N007,  421, 415, 421, 421, 415, 415, 415 },
	{ TX_TUG3N008, 5547, 5257, 5869, 5869, 5288, 5515, 5522 },
	{ TX_COMPA180, 3724, 3571, 3962, 3979, 3587, 3746, 3735 },
	{ TX_COMPU181, 4618, 4404, 4865, 4876, 4420, 4588, 4578 },
	{ TX_COMPU182, 4535, 4336, 4771, 4784, 4352, 4511, 4499 },
	{          -1, 0,    0, 0, 0, 0, 0, 0 }
};

extern const RoomText tug3Texts[] = {
	{ TX_TUG3_012, Common::EN_ANY, "#TUG3\\TUG3_012#I'm a doctor, not a space jockey. Talk to Mr. Sulu about fixing our orbit." },
	{ TX_TUG3_012, Common::DE_DEU, "#TUG3\\TUG3_012#Ich bin Arzt, kein Weltraum-Jockey. Hol dir Mr. Sulu wenn du Probleme mit der Umlaufbahn hast." },
	{ TX_TUG3_012, Common::FR_FRA, "#TUG3\\TUG3_012#Je suis m\xe9" "decin, pas jockey de l'espace. Demandez \xe0 M. Sulu de r\xe9gler notre orbite." },
	{ -1, Common::UNK_LANG, "" }
};

void Room::tug3Tick1() {
	playVoc("TUG3LOOP");

	_awayMission->disableWalking = true;
	loadActorAnim2(OBJECT_ELASI_1, "p1turn", 0xa4, 0x98, 0);
	loadActorAnim2(OBJECT_ELASI_2, "p2turn", 0xd1, 0x88, 0);
	loadActorAnim2(OBJECT_ELASI_3, "p3turn", 0xfb, 0xc4, 0);
	loadActorAnim2(OBJECT_ELASI_4, "p4turn", 0x5a, 0x9e, 0);
}

void Room::tug3Tick40() {
	loadActorAnim2(OBJECT_KIRK, "kdraws", -1, -1, 0); // Draw phaser
}

void Room::tug3LookAnywhere() {
	showDescription(TX_TUG3N007);
}

void Room::tug3ElasiSurrendered() {
	// BUGFIX: the original game had the condition below. However, this would cause
	// problems if you start shooting, then get them to surrender; if timed correctly,
	// they could shoot Kirk as he's beaming out and the mission is "successful". To
	// prevent that, the below condition is removed in ScummVM.
	/*
	if (_awayMission->tug.bridgeElasiDrewPhasers)
		return;
	*/

	// Also part of the bugfix (stop shooting)
	_awayMission->timers[0] = 0;

	_awayMission->tug.elasiSurrendered = true;
	loadActorAnim2(OBJECT_ELASI_1, "p1surr", -1, -1, 0);
	_awayMission->tug.bridgeElasi1Status = GUARDSTAT_SURRENDERED;

	if (_awayMission->tug.bridgeElasi2Status == GUARDSTAT_UP) {
		loadActorAnim2(OBJECT_ELASI_2, "p2surr", -1, -1, 0);
		_awayMission->tug.bridgeElasi2Status = GUARDSTAT_SURRENDERED;
	}
	if (_awayMission->tug.bridgeElasi3Status == GUARDSTAT_UP) {
		loadActorAnim2(OBJECT_ELASI_3, "p3surr", -1, -1, 0);
		_awayMission->tug.bridgeElasi3Status = GUARDSTAT_SURRENDERED;
	}
	if (_awayMission->tug.bridgeElasi4Status == GUARDSTAT_UP) {
		loadActorAnim2(OBJECT_ELASI_4, "p4surr", -1, -1, 0);
		_awayMission->tug.bridgeElasi4Status = GUARDSTAT_SURRENDERED;
	}

	_awayMission->tug.missionScore += 4;
}

void Room::tug3UsePhaserAnywhere() {
	// Stub function to suppress "you can't do that" messages
}

void Room::tug3ElasiDrawPhasers() {
	if (_awayMission->tug.bridgeElasiDrewPhasers)
		return;

	_awayMission->tug.bridgeElasiDrewPhasers = true;

	// If brig guards are alive & untied, kill the hostages
	if (_awayMission->tug.guard1Status == 0 || _awayMission->tug.guard2Status == 0) {
		showText(TX_SPEAKER_ELASI_CLANSMAN, TX_TUG3L081);
		_awayMission->tug.missionScore = 0;
	}

	if (_awayMission->timers[0] == 0)
		_awayMission->timers[0] = 10;

	// Make all elasi draw their phasers
	if (_awayMission->tug.bridgeElasi1Status == 0)
		loadActorAnim2(OBJECT_ELASI_1, "p1draw", -1, -1, 0);
	if (_awayMission->tug.bridgeElasi2Status == 0)
		loadActorAnim2(OBJECT_ELASI_2, "p2draw", -1, -1, 0);
	if (_awayMission->tug.bridgeElasi3Status == 0)
		loadActorAnim2(OBJECT_ELASI_3, "p3draw", -1, -1, 0);
	if (_awayMission->tug.bridgeElasi4Status == 0)
		loadActorAnim2(OBJECT_ELASI_4, "p4draw", -1, -1, 0);
}

void Room::tug3UseStunPhaserOnElasi1() {
	if (_awayMission->tug.crewmanKilled[OBJECT_KIRK] || _awayMission->tug.bridgeElasi1Status != GUARDSTAT_UP)
		return;
	loadActorAnim2(OBJECT_ELASI_1, "p1stun", -1, -1, 12);
	playSoundEffectIndex(kSfxPhaser);
	showBitmapFor5Ticks("t3beem05", 5);
	_awayMission->tug.bridgeElasi1Status = GUARDSTAT_STUNNED;
	tug3ElasiDrawPhasers();
}

void Room::tug3UseStunPhaserOnElasi2() {
	if (_awayMission->tug.crewmanKilled[OBJECT_KIRK] || _awayMission->tug.bridgeElasi2Status != GUARDSTAT_UP)
		return;
	loadActorAnim2(OBJECT_ELASI_2, "p2stun", -1, -1, 12);
	playSoundEffectIndex(kSfxPhaser);
	showBitmapFor5Ticks("t3beem06", 5);
	_awayMission->tug.bridgeElasi2Status = GUARDSTAT_STUNNED;
	tug3ElasiDrawPhasers();
}

void Room::tug3UseStunPhaserOnElasi3() {
	if (_awayMission->tug.crewmanKilled[OBJECT_KIRK] || _awayMission->tug.bridgeElasi3Status != GUARDSTAT_UP)
		return;
	loadActorAnim2(OBJECT_ELASI_3, "p3stun", -1, -1, 12);
	playSoundEffectIndex(kSfxPhaser);
	showBitmapFor5Ticks("t3beem07", 5);
	_awayMission->tug.bridgeElasi3Status = GUARDSTAT_STUNNED;
	tug3ElasiDrawPhasers();
}

void Room::tug3UseStunPhaserOnElasi4() {
	if (_awayMission->tug.crewmanKilled[OBJECT_KIRK] || _awayMission->tug.bridgeElasi4Status != GUARDSTAT_UP)
		return;
	loadActorAnim2(OBJECT_ELASI_4, "p4stun", -1, -1, 12);
	playSoundEffectIndex(kSfxPhaser);
	showBitmapFor5Ticks("t3beem04", 5);
	_awayMission->tug.bridgeElasi4Status = GUARDSTAT_STUNNED;
	tug3ElasiDrawPhasers();
}

void Room::tug3UseKillPhaserOnElasi1() {
	if (_awayMission->tug.crewmanKilled[OBJECT_KIRK] || _awayMission->tug.bridgeElasi1Status != GUARDSTAT_UP)
		return;
	loadActorAnim2(OBJECT_ELASI_1, "p1Kill", -1, -1, 12);
	playSoundEffectIndex(kSfxPhaser);
	showBitmapFor5Ticks("t3beem25", 5);
	_awayMission->tug.bridgeElasi1Status = GUARDSTAT_DEAD;
	_awayMission->tug.missionScore -= 2;
	tug3ElasiDrawPhasers();
}

void Room::tug3UseKillPhaserOnElasi2() {
	if (_awayMission->tug.crewmanKilled[OBJECT_KIRK] || _awayMission->tug.bridgeElasi2Status != GUARDSTAT_UP)
		return;
	loadActorAnim2(OBJECT_ELASI_2, "p2Kill", -1, -1, 12);
	playSoundEffectIndex(kSfxPhaser);
	showBitmapFor5Ticks("t3beem27", 5);
	_awayMission->tug.bridgeElasi2Status = GUARDSTAT_DEAD;
	_awayMission->tug.missionScore -= 2;
	tug3ElasiDrawPhasers();
}

void Room::tug3UseKillPhaserOnElasi3() {
	if (_awayMission->tug.crewmanKilled[OBJECT_KIRK] || _awayMission->tug.bridgeElasi3Status != GUARDSTAT_UP)
		return;
	loadActorAnim2(OBJECT_ELASI_3, "p3Kill", -1, -1, 12);
	playSoundEffectIndex(kSfxPhaser);
	showBitmapFor5Ticks("t3beem26", 5);
	_awayMission->tug.bridgeElasi3Status = GUARDSTAT_DEAD;
	_awayMission->tug.missionScore -= 2;
	tug3ElasiDrawPhasers();
}

void Room::tug3UseKillPhaserOnElasi4() {
	if (_awayMission->tug.crewmanKilled[OBJECT_KIRK] || _awayMission->tug.bridgeElasi4Status != GUARDSTAT_UP)
		return;
	loadActorAnim2(OBJECT_ELASI_4, "p4Kill", -1, -1, 12);
	playSoundEffectIndex(kSfxPhaser);
	showBitmapFor5Ticks("t3beem24", 5);
	_awayMission->tug.bridgeElasi4Status = GUARDSTAT_DEAD;
	_awayMission->tug.missionScore -= 2;
	tug3ElasiDrawPhasers();
}

void Room::tug3ElasiStunnedOrKilled() {
	if (_awayMission->tug.bridgeWinMethod == 1)
		return;
	if (_awayMission->tug.bridgeElasi1Status == GUARDSTAT_UP || _awayMission->tug.bridgeElasi2Status == GUARDSTAT_UP
	        || _awayMission->tug.bridgeElasi3Status == GUARDSTAT_UP || _awayMission->tug.bridgeElasi4Status == GUARDSTAT_UP)
		return;

	// BUGFIX: if the ship is deorbiting, the mission isn't won yet.
	if (_awayMission->tug.orbitalDecayCounter != 0)
		return;

	_awayMission->tug.bridgeWinMethod = 1;
	tug3EndMission();
}

void Room::tug3TalkToElasi1() {
	if (_awayMission->tug.bridgeElasi1Status != GUARDSTAT_UP || _awayMission->tug.talkedToCereth)
		return;
	_awayMission->tug.talkedToCereth = true;

	const TextRef choices[] = {
		TX_SPEAKER_KIRK,
		TX_TUG3_004,
		TX_TUG3_003,
		TX_TUG3_002,
		TX_END
	};
	int choice = showMultipleTexts(choices);

	switch (choice) {
	case 0: // They surrender
		showText(TX_SPEAKER_ELASI_CERETH, TX_TUG3L084);
		_awayMission->tug.bridgeElasi1Status = GUARDSTAT_SURRENDERED;
		loadActorAnim2(OBJECT_ELASI_1, "p1surr", -1, -1, 0);
		tug3ElasiSurrendered();
		_awayMission->tug.bridgeWinMethod = 2;
		_awayMission->tug.missionScore += 8;
		tug3EndMission();
		break;

	case 1: // Cereth shoots console, doesn't surrender
		_awayMission->disableInput = true;
		showText(TX_SPEAKER_ELASI_CERETH, TX_TUG3_F27);
		loadActorAnim2(OBJECT_ELASI_1, "p1draw", -1, -1, 3);
		break;

	case 2: // Cereth shoots console and surrenders
		_awayMission->disableInput = true;
		showText(TX_SPEAKER_ELASI_CERETH, TX_TUG3L083);
		loadActorAnim2(OBJECT_ELASI_1, "p1draw", -1, -1, 5);

		// BUGFIX: they're going to surrender, so stop the firefight.
		_awayMission->timers[0] = 0;
		break;
	}
}

void Room::tug3Elasi1DrewPhaser() {
	playSoundEffectIndex(kSfxPhaser);
	showBitmapFor5Ticks("t3beem41", 13);
	loadActorAnim2(OBJECT_12, "sparks", 0xa0, 0xad, 4);
}

void Room::tug3Elasi1ShotConsole() {
	tug3ElasiDrawPhasers();
	_awayMission->disableInput = false;
	_awayMission->tug.bridgeWinMethod = 3;
	_awayMission->timers[1] = 10;
}

void Room::tug3Elasi1DrewPhaser2() {
	playSoundEffectIndex(kSfxPhaser);
	showBitmapFor5Ticks("t3beem41", 13);
	loadActorAnim2(OBJECT_12, "sparks", 0xa0, 0xad, 6);
}

void Room::tug3Elasi1ShotConsoleAndSurrenders() {
	showText(TX_SPEAKER_ELASI_CERETH, TX_TUG3L080);
	tug3ElasiSurrendered();
	_awayMission->disableInput = false;
	_awayMission->tug.bridgeWinMethod = 3;
	_awayMission->timers[1] = 10;
}

void Room::tug3LookAtMccoy() {
	showDescription(TX_TUG3N000);
}

void Room::tug3LookAtSpock() {
	showDescription(TX_TUG3N002);
}

void Room::tug3LookAtRedshirt() {
	showDescription(TX_TUG3N001);
}

void Room::tug3LookAtElasi1() {
	// BUGFIX: there were two implementations of this function; the first was the same as
	// the other 3 elasi, the second was specific to the captain. The second was never
	// called in the original game, but it's used here instead for more variety.
	// The function itself is changed by checking both if he's stunned or if he's dead,
	// instead of just checking if he's stunned.

	if (_awayMission->tug.bridgeElasi1Status == GUARDSTAT_STUNNED || _awayMission->tug.bridgeElasi1Status == GUARDSTAT_DEAD)
		showDescription(TX_TUG3N003);
	else
		showDescription(TX_TUG3N008);
}

void Room::tug3LookAtElasi2() {
	// BUGFIX: also check if stunned. They can't "glare at the crewmembers" if they're
	// unconscious. (applies to below functions too.)
	if (_awayMission->tug.bridgeElasi2Status == GUARDSTAT_DEAD || _awayMission->tug.bridgeElasi2Status == GUARDSTAT_STUNNED)
		showDescription(TX_TUG3N004);
	else
		showDescription(TX_TUG3N005);
}

void Room::tug3LookAtElasi3() {
	if (_awayMission->tug.bridgeElasi3Status == GUARDSTAT_DEAD || _awayMission->tug.bridgeElasi2Status == GUARDSTAT_STUNNED)
		showDescription(TX_TUG3N004);
	else
		showDescription(TX_TUG3N005);
}

void Room::tug3LookAtElasi4() {
	if (_awayMission->tug.bridgeElasi4Status == GUARDSTAT_DEAD || _awayMission->tug.bridgeElasi2Status == GUARDSTAT_STUNNED)
		showDescription(TX_TUG3N004);
	else
		showDescription(TX_TUG3N005);
}

void Room::tug3TalkToMccoy() {
	if (_awayMission->tug.orbitalDecayCounter != 0 || _awayMission->tug.bridgeElasi1Status != GUARDSTAT_UP) {
		if (_awayMission->tug.orbitalDecayCounter >= 10) {
			if (_awayMission->tug.orbitalDecayCounter < 16)
				showText(TX_SPEAKER_MCCOY, TX_TUG3_011);
		} else {
			// BUGFIX: the English CD version displays a blank textbox. Use the text
			// of the floppy version, for which the CD has an audio file.
			showText(TX_SPEAKER_MCCOY, TX_TUG3_012);
		}
	} else
		showText(TX_SPEAKER_MCCOY, TX_TUG3_013);
}

void Room::tug3TalkToSpock() {
	if (_awayMission->tug.orbitalDecayCounter != 0) {
		if (_awayMission->tug.orbitalDecayCounter >= 10) {
			if (_awayMission->tug.orbitalDecayCounter < 16)
				showText(TX_SPEAKER_SPOCK, TX_TUG3_008);
		} else
			showText(TX_SPEAKER_SPOCK, TX_TUG3_009);
	}
}

void Room::tug3TalkToRedshirt() {
	showText(TX_SPEAKER_CHRISTENSEN, TX_TUG3L003);
}

void Room::tug3UseCommunicator() {
	if (_awayMission->tug.orbitalDecayCounter == 0)
		return;

	if (_awayMission->tug.orbitalDecayCounter < 10) {
		showText(TX_SPEAKER_KIRK, TX_TUG3_007);
		showText(TX_SPEAKER_SULU, TX_TUG3_015);
		showText(TX_SPEAKER_SHIPS_COMPUTER, TX_COMPA180);
		_awayMission->timers[1] = 0;

		// BUGFIX: if still fighting the elasi, the mission isn't done yet.
		_awayMission->tug.orbitalDecayCounter = 0;
		if (!_awayMission->tug.elasiSurrendered &&
		        (_awayMission->tug.bridgeElasi1Status == GUARDSTAT_UP
		         || _awayMission->tug.bridgeElasi2Status == GUARDSTAT_UP
		         || _awayMission->tug.bridgeElasi3Status == GUARDSTAT_UP
		         || _awayMission->tug.bridgeElasi4Status == GUARDSTAT_UP))
			return;

		tug3EndMission();
	} else {
		if (_awayMission->tug.orbitalDecayCounter < 16) {
			showText(TX_SPEAKER_KIRK, TX_TUG3_006);
			showText(TX_SPEAKER_SCOTT, TX_TUG3_S07);

			playMidiMusicTracks(MIDITRACK_NONE);

			_awayMission->disableInput = true;
			if (_awayMission->tug.missionScore < 0)
				_awayMission->tug.missionScore = 0;
			endMission(_awayMission->tug.missionScore, _awayMission->tug.field2b, _awayMission->tug.field2d);
		}
	}
}

// One of the elasi shoots one of the crewmen
void Room::tug3Timer0Expired() {
	const char *beamAnims[][4] = {
		{ "t3beem33", "t3beem35", "t3beem34", "t3beem32" },
		{ "t3beem37", "t3beem39", "t3beem38", "t3beem36" },
		{ "t3beem29", "t3beem31", "t3beem30", "t3beem28" },
	};

	int elasiShooter, elasiTarget;

	if (_awayMission->tug.bridgeElasi2Status == GUARDSTAT_UP)
		elasiShooter = 0;
	else if (_awayMission->tug.bridgeElasi3Status == GUARDSTAT_UP)
		elasiShooter = 1;
	else if (_awayMission->tug.bridgeElasi4Status == GUARDSTAT_UP)
		elasiShooter = 2;
	else
		return;

	if (!_awayMission->tug.crewmanKilled[OBJECT_REDSHIRT]) {
		_awayMission->tug.crewmanKilled[OBJECT_REDSHIRT] = 2;
		_awayMission->redshirtDead = true;
		loadActorAnim2(OBJECT_REDSHIRT, "rkills", -1, -1, 0);
		elasiTarget = OBJECT_REDSHIRT;
	} else if (!_awayMission->tug.crewmanKilled[OBJECT_KIRK]) {
		_awayMission->tug.crewmanKilled[OBJECT_KIRK] = 2;
		_awayMission->disableInput = true;
		loadActorAnim2(OBJECT_KIRK, "kkills", -1, -1, 0);
		elasiTarget = OBJECT_KIRK;
	} else if (!_awayMission->tug.crewmanKilled[OBJECT_SPOCK]) {
		_awayMission->tug.crewmanKilled[OBJECT_SPOCK] = 2;
		loadActorAnim2(OBJECT_SPOCK, "skills", -1, -1, 0);
		elasiTarget = OBJECT_SPOCK;
	} else if (!_awayMission->tug.crewmanKilled[OBJECT_MCCOY]) {
		_awayMission->tug.crewmanKilled[OBJECT_MCCOY] = 2;
		loadActorAnim2(OBJECT_MCCOY, "mkills", -1, -1, 13);
		elasiTarget = OBJECT_MCCOY;
	} else
		return;

	playSoundEffectIndex(kSfxPhaser);
	showBitmapFor5Ticks(beamAnims[elasiShooter][elasiTarget], 5);
	_awayMission->timers[0] = 50;
}

void Room::tug3AllCrewmenDead() {
	playMidiMusicTracks(MIDITRACK_2);
	showGameOverMenu();
}

// "Orbital decay" countdown timer
void Room::tug3Timer1Expired() {
	if (_awayMission->tug.orbitalDecayCounter < 10) { // Decay still preventable
		showText(TX_SPEAKER_SHIPS_COMPUTER, TX_COMPU182);
		_awayMission->timers[1] = 100;
		_awayMission->tug.orbitalDecayCounter++;
	} else if (_awayMission->tug.orbitalDecayCounter < 16) { // Decay now unavoidable
		showText(TX_SPEAKER_SHIPS_COMPUTER, TX_COMPU181);
		_awayMission->timers[1] = 100;
		_awayMission->tug.orbitalDecayCounter++;
	} else { // Game over
		showDescription(TX_TUG3N006);
		showGameOverMenu();
	}
}

void Room::tug3EndMission() {
	playMidiMusicTracks(MIDITRACK_28);
	showText(TX_SPEAKER_KIRK, TX_TUG3_001);
	playSoundEffectIndex(kSfxTransporterMaterialize);
	loadActorAnim2(OBJECT_13, "rteleb", 0x14, 0xa0, 7);
	loadActorAnim2(OBJECT_14, "rteleb", 0x118, 0xa0, 0);
	loadActorAnim2(OBJECT_15, "rteleb", 0x96, 0xbe, 0);
}

void Room::tug3SecurityTeamBeamedIn() {
	loadActorAnim2(OBJECT_13, "rdrawe", -1, -1, 8);
	loadActorAnim2(OBJECT_14, "rdraws", -1, -1, 9);
	loadActorAnim2(OBJECT_15, "rfiren", -1, -1, 10);

	showText(TX_SPEAKER_KIRK, TX_TUG3_005);
	showText(TX_SPEAKER_SCOTT, TX_TUG3_S08);

	playMidiMusicTracks(MIDITRACK_NONE);
	_awayMission->disableInput = true;
	if (_awayMission->tug.missionScore < 0)
		_awayMission->tug.missionScore = 0;
	endMission(_awayMission->tug.missionScore, _awayMission->tug.field2b, _awayMission->tug.field2d);
}

}
