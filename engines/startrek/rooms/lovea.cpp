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

// Common code for all rooms in LOVE mission.

#include "startrek/room.h"
#include "startrek/startrek.h"

namespace StarTrek {

enum loveTextIds {
	TX_COMMON_IDS_OFFSET_START = 5000,	// needed to distinguish common IDs
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_UHURA, TX_SPEAKER_FERRIS,
	TX_LOV0_011, TX_LOV0_012, TX_LOV0_013, TX_LOV0_014, TX_LOV0_015,
	TX_LOV0_016, TX_LOV0_017, TX_LOV0_019, TX_LOV0_020, TX_LOV0_021,
	TX_LOV0_022, TX_LOV0_025, TX_LOV0_026, TX_LOV0_029, TX_LOV0_033,
	TX_LOV0_039, TX_LOV0_040, TX_LOV0_041, TX_LOV0_042, TX_LOV0_043,
	TX_LOV0_045, TX_LOV0_046, TX_LOV0_047, TX_LOV0_048, TX_LOV0_050,
	TX_LOV0_101, TX_LOV0_102, TX_LOV0_103, TX_LOV0_104, TX_LOV0_105,
	TX_LOV0_106, TX_LOV0_107, TX_LOV0_124, TX_LOV5_015, TX_LOV5_019,
	TX_LOV5_027, TX_LOV5_030, TX_LOV5_038, TX_LOVA_100, TX_LOVA_F01,
	TX_LOVA_F02, TX_LOVA_F03, TX_LOVA_F04, TX_LOVA_F07, TX_LOVA_F08,
	TX_LOVA_F10, TX_LOVA_F54, TX_LOVA_F55, TX_TUG2_010, TX_MUD2_040,
	TX_LOV2N005, TX_MUD4_018, TX_BRIDU146, TX_VENA_F41, TX_GENER004,
	TX_LOV5C001, TX_SPOKCOFF, TX_SPEAKER_SCOTT, TX_LOVAF001, TX_LOVAF002,
	TX_LOVAF003, TX_LOVAF004, TX_LOVAF005, TX_LOVAF006, TX_LOVAF007,
	TX_LOVAF008, TX_LOVAF009, TX_LOVAF010, TX_LOVAF011, TX_LOVAF012,
	TX_LOVAF013, TX_LOVAF014, TX_LOVAF015, TX_LOVAF016, TX_LOVAF017,
	TX_LOVAF018, TX_LOVAF019, TX_LOVAF020, TX_LOVAF021, TX_LOVAF022,
	TX_LOVAF023, TX_LOVAF024, TX_LOVAF025, TX_LOVAF026, TX_LOVAF027
};

extern const RoomTextOffsets loveTextOffsets[] = {
	{ TX_SPEAKER_KIRK, 5915, 6466, 6024, 6270, 6469, 7096, 7069, 6469, 5939 },
	{ TX_SPEAKER_MCCOY, 5926, 6477, 6035, 6281, 6480, 7109, 7080, 6480, 5950 },
	{ TX_SPEAKER_SPOCK, 5936, 6487, 6045, 6291, 6490, 7119, 7090, 6490, 5960 },
	{ TX_SPEAKER_UHURA, 4990, 4608, 5107, 5293, 4611, 4937, 5019, 4611, 5014 },
	{ TX_SPEAKER_FERRIS, 5946, 6497, 6055, 6301, 6500, 7129, 7100, 6500, 5970 },
	{ TX_LOV0_011, 10498, 10477, 11169, 11106, 10469, 11234, 11500, 10469, 10523 },
	{ TX_LOV0_012, 10625, 10559, 11299, 11231, 10551, 11317, 11585, 10551, 10650 },
	{ TX_LOV0_013, 10921, 10780, 11639, 11541, 10772, 11555, 11853, 10772, 10946 },
	{ TX_LOV0_014, 10685, 10604, 11373, 11289, 10596, 11361, 11647, 10596, 10710 },
	{ TX_LOV0_015, 10789, 10678, 11488, 11391, 10670, 11434, 11732, 10670, 10814 },
	{ TX_LOV0_016, 10592, 10541, 11263, 11198, 10533, 11296, 11564, 10533, 10617 },
	{ TX_LOV0_017, 11055, 10884, 11780, 11673, 10876, 11658, 11963, 10876, 11080 },
	{ TX_LOV0_019, 10975, 10819, 11701, 11592, 10811, 11591, 11900, 10811, 11000 },
	{ TX_LOV0_020, 10532, 10496, 11199, 11140, 10488, 11253, 11515, 10488, 10557 },
	{ TX_LOV0_021, 10843, 10717, 11552, 11440, 10709, 11469, 11781, 10709, 10868 },
	{ TX_LOV0_022, 10733, 10637, 11426, 11345, 10629, 11402, 11685, 10629, 10758 },
	{ TX_LOV0_025, 11981, 11291, 12824, 12641, 11284, 12154, 12479, 11284, 12006 },
	{ TX_LOV0_026, 11854, 11020, 12681, 12488, 11013, 11801, 12139, 11013, 11879 },
	{ TX_LOV0_029, 11734, 10930, 12529, 12360, 10922, 11703, 12017, 10922, 11759 },
	{ TX_LOV0_033, 11794, 10975, 12599, 12426, 10968, 11754, 12072, 10968, 11819 },
	{ TX_LOV0_039, 11116, 0, 11849, 11733, 10469, 11234, 11500, 10469, 11141 },
	{ TX_LOV0_040, 11243, 0, 11979, 11858, 10551, 11317, 11585, 10551, 11268 },
	{ TX_LOV0_041, 11539, 0, 12319, 12168, 10772, 11555, 11853, 10772, 11564 },
	{ TX_LOV0_042, 11303, 0, 12053, 11916, 10596, 11361, 11647, 10596, 11328 },
	{ TX_LOV0_043, 11407, 0, 12168, 12018, 10670, 11434, 11732, 10670, 11432 },
	{ TX_LOV0_045, 11210, 0, 11943, 11825, 10533, 11296, 11564, 10533, 11235 },
	{ TX_LOV0_046, 11673, 0, 12460, 12300, 10876, 11658, 11963, 10876, 11698 },
	{ TX_LOV0_047, 11593, 0, 12381, 12219, 10811, 11591, 11900, 10811, 11618 },
	{ TX_LOV0_048, 11150, 0, 11879, 11767, 10488, 11253, 11515, 10488, 11175 },
	{ TX_LOV0_050, 11351, 0, 12106, 11972, 10629, 11402, 11685, 10629, 11376 },
	{ TX_LOV0_101, 12062, 0, 12909, 12735, 0, 0, 0, 0, 12087 },
	{ TX_LOV0_102, 12126, 0, 12955, 12802, 0, 0, 0, 0, 12151 },
	{ TX_LOV0_103, 12196, 0, 13036, 12884, 0, 0, 0, 0, 12221 },
	{ TX_LOV0_104, 12246, 0, 13092, 12942, 0, 0, 0, 0, 12271 },
	{ TX_LOV0_105, 12303, 0, 13153, 13008, 0, 0, 0, 0, 12328 },
	{ TX_LOV0_106, 12415, 0, 13267, 13131, 0, 0, 0, 0, 12440 },
	{ TX_LOV0_107, 12483, 0, 13343, 13212, 0, 0, 0, 0, 12508 },
	{ TX_LOV0_124, 11931, 11082, 12770, 12585, 11075, 11883, 12213, 11075, 11956 },
	{ TX_LOV5_015,  3009, 0, 3118, 3116, 0, 0, 0, 0, 2999 },
	{ TX_LOV5_019,  2931, 2613, 3045, 3028, 2612, 2790, 2851, 2612, 2921 },
	{ TX_LOV5_027,  3444, 3057, 3540, 3559, 3056, 0, 3335, 3056, 3434 },
	{ TX_LOV5_030,  3348, 2988, 3448, 3462, 2987, 3226, 3262, 2987, 3338 },
	{ TX_LOV5_038,  3399, 3027, 3498, 3518, 3026, 3226, 3308, 3026, 3389 },
	{ TX_LOVA_100,  4553, 4155, 4625, 4784, 4156, 4480, 4536, 4156, 4569 },
	{ TX_LOVA_F01,  3995, 3530, 4096, 4170, 3531, 3789, 3862, 3531, 4011 },
	{ TX_LOVA_F02,  4067, 3622, 4161, 4242, 3623, 3899, 3963, 3623, 4083 },
	{ TX_LOVA_F03,  4266, 3891, 4370, 4470, 3892, 4181, 4238, 3892, 4282 },
	{ TX_LOVA_F04,  4181, 3790, 4278, 4368, 3791, 4072, 4114, 3791, 4197 },
	{ TX_LOVA_F07,  4630, 4217, 4704, 4862, 4218, 4543, 4600, 4218, 4646 },
	{ TX_LOVA_F08,  3619, 3992, 3714, 3747, 3993, 4290, 4362, 3993, 3609 },
	{ TX_LOVA_F10,  4501, 4096, 4578, 4724, 4097, 4419, 4466, 4097, 4517 },
	{ TX_LOVA_F54,  3775, 3303, 3866, 3913, 3302, 3533, 3591, 3302, 3766 },
	{ TX_LOVA_F55,  3076, 2728, 3184, 3187, 2727, 2896, 2973, 2727, 3066 },
	{ TX_TUG2_010,  4431, 4049, 4525, 4650, 4050, 4367, 4421, 4050, 4447 },
	{ TX_MUD2_040,  4826, 0, 4925, 5103, 0, 0, 0, 0, 4842 },
	{ TX_LOV2N005,  4689, 4269, 4774, 4922, 4270, 4593, 4655, 4270, 4705 },
	{ TX_MUD4_018,  5035, 0, 5152, 5338, 0, 0, 0, 0, 5059 },
	{ TX_BRIDU146,  5104, 5211, 5219, 5407, 5214, 5663, 5671, 5214, 5128 },
	{ TX_VENA_F41,  5158, 5246, 5273, 5473, 5249, 5700, 5709, 5249, 5182 },
	{ TX_LOV5C001,  3497, 3095, 3594, 3624, 3094, 3323, 3383, 3094, 3487 },
	{ TX_SPOKCOFF,  3564, 3162, 3661, 3694, 3161, 3490, 3450, 3161, 3554 },
	{ TX_GENER004,  3924, 3474, 4025, 4099, 3475, 3733, 3804, 3475, 3940 },	// Game over
	{ TX_SPEAKER_SCOTT, 0, 4598, 0, 0, 4601, 4923, 5006, 4601, 0 },
	{ TX_LOVAF001, 0, 2676, 0, 0, 2675, 2844, 2918, 2675, 0 },
	{ TX_LOVAF002, 0, 2840, 0, 0, 2839, 3027, 3100, 2839, 0 },
	{ TX_LOVAF003, 0, 3203, 0, 0, 3202, 3432, 3487, 3202, 0 },
	{ TX_LOVAF004, 0, 4416, 0, 0, 4417, 4730, 4824, 4417, 0 },
	{ TX_LOVAF005, 0, 4563, 0, 0, 4566, 4886, 4975, 4566, 0 },
	{ TX_LOVAF006, 0, 4653, 0, 0, 4656, 4982, 5064, 4656, 0 },
	{ TX_LOVAF007, 0, 4707, 0, 0, 4710, 5046, 5123, 4710, 0 },
	{ TX_LOVAF008, 0, 4741, 0, 0, 4744, 5085, 5161, 4744, 0 },
	{ TX_LOVAF009, 0, 4890, 0, 0, 4893, 5270, 5323, 4893, 0 },
	{ TX_LOVAF010, 0, 5050, 0, 0, 5053, 5451, 5502, 5053, 0 },
	{ TX_LOVAF011, 0, 5081, 0, 0, 5084, 5500, 5534, 5084, 0 },
	{ TX_LOVAF012, 0, 5156, 0, 0, 5159, 5587, 5615, 5159, 0 },
	{ TX_LOVAF013, 0, 5185, 0, 0, 5188, 5637, 5645, 5188, 0 },
	{ TX_LOVAF014, 0, 5277, 0, 0, 5280, 5732, 5739, 5280, 0 },
	{ TX_LOVAF015, 0, 5303, 0, 0, 5306, 5758, 5765, 5306, 0 },
	{ TX_LOVAF016, 0, 5352, 0, 0, 5355, 5819, 5816, 5355, 0 },
	{ TX_LOVAF017, 0, 5474, 0, 0, 5477, 5965, 5962, 5477, 0 },
	{ TX_LOVAF018, 0, 5570, 0, 0, 5573, 6100, 6089, 5573, 0 },
	{ TX_LOVAF019, 0, 5688, 0, 0, 5691, 6242, 6231, 5691, 0 },
	{ TX_LOVAF020, 0, 5827, 0, 0, 5830, 6392, 6586, 5830, 0 },
	{ TX_LOVAF021, 0, 6067, 0, 0, 6070, 6656, 6415, 6070, 0 },
	{ TX_LOVAF022, 0, 6197, 0, 0, 6200, 6808, 6555, 6200, 0 },
	{ TX_LOVAF023, 0, 11117, 0, 0, 11110, 11924, 12252, 11110, 0 },
	{ TX_LOVAF024, 0, 11151, 0, 0, 11144, 11977, 12287, 11144, 0 },
	{ TX_LOVAF025, 0, 11188, 0, 0, 11181, 12029, 12332, 11181, 0 },
	{ TX_LOVAF026, 0, 11247, 0, 0, 11240, 12098, 12420, 11240, 0 },
	{ TX_LOVAF027, 0, 11357, 0, 0, 11350, 12224, 12546, 11350, 0 },
	{          -1,  0,    0, 0, 0, 0, 0, 0, 0, 0 }
};

// Mccoy or Ferris say something under effects of laughing gas
void Room::loveaTimer0Expired() {
	const TextRef ferrisText[] = {
		TX_LOV0_039,
		TX_LOV0_048,
		TX_LOV0_045,
		TX_LOV0_040,
		TX_LOV0_042,
		TX_LOV0_050,
		TX_LOV0_043,
		TX_LOV0_041,
		TX_LOV0_047,
		TX_LOV0_046,
		TX_LOV0_039
	};

	const TextRef mccoyText[] = {
		TX_LOV0_011,
		TX_LOV0_020,
		TX_LOV0_016,
		TX_LOV0_012,
		TX_LOV0_014,
		TX_LOV0_022,
		TX_LOV0_015,
		TX_LOV0_021,
		TX_LOV0_013,
		TX_LOV0_019,
		TX_LOV0_017
	};

	byte randomVal;
	TextRef speaker;

	const TextRef *textTable;

	if (_vm->isFloppyVersion()) {
		// In the floppy version, Kirk is affected as well and everyone uses McCoy's lines
		randomVal = getRandomWordInRange(0, 2);
		if (randomVal == 1)
			speaker = TX_SPEAKER_MCCOY;
		else if (randomVal == 2 && !_awayMission->redshirtDead)
			speaker = TX_SPEAKER_FERRIS;
		else
			speaker = TX_SPEAKER_KIRK;
		textTable = mccoyText;
	} else {
		// BUGFIX: should range from 0-1, not 0-2. Original had "get name errors" when it
		// rolled a 2.
		randomVal = getRandomWordInRange(0, 1);
		if (randomVal == 0 || _awayMission->redshirtDead) {
			speaker = TX_SPEAKER_MCCOY;
			textTable = mccoyText;
		} else {
			speaker = TX_SPEAKER_FERRIS;
			textTable = ferrisText;
		}
	}

	randomVal = getRandomWordInRange(0, 10);

	showText(speaker, textTable[randomVal]);

	if (!_awayMission->love.releasedRomulanLaughingGas) {
		const int spockText[] = {
			TX_LOV0_029,
			TX_LOV0_033,
			TX_LOV0_026,
			TX_LOV0_124
		};
		showText(TX_SPEAKER_SPOCK, spockText[getRandomWordInRange(0, 3)]);

		// BUG(?): This is in an if statement, meaning the human crewmen stop talking from
		// laughing gas if Spock is under laughing gas effects. Might be intentional, to
		// reduce "spamming" of text?
		_awayMission->timers[0] = getRandomWordInRange(200, 400);
	}
}

// Spock says something under effects of laughing gas
void Room::loveaTimer1Expired() {
	const int spockText[] = {
		TX_LOV0_025,
		TX_LOV0_101,
		TX_LOV0_102,
		TX_LOV0_103,
		TX_LOV0_104,
		TX_LOV0_105,
		TX_LOV0_106,
		TX_LOV0_107
	};

	const int spockTextFloppy[] = {
		TX_LOVAF023,
		TX_LOVAF024,
		TX_LOVAF025,
		TX_LOVAF026,
		TX_LOV0_025,
		TX_LOVAF027
	};

	if (_vm->isFloppyVersion())
		showText(TX_SPEAKER_SPOCK, spockTextFloppy[getRandomWordInRange(0, 5)]);
	else
		showText(TX_SPEAKER_SPOCK, spockText[getRandomWordInRange(0, 7)]);

	_awayMission->timers[1] = getRandomWordInRange(200, 400);
}

void Room::loveaUseMedkitOnSpock() {
	if (_awayMission->love.spockCured && _vm->isFloppyVersion())
		showText(TX_SPEAKER_SPOCK, TX_LOVAF001);
	else if (_awayMission->love.spockCured)
		showText(TX_SPEAKER_MCCOY, TX_LOV5_015);
	else
		showText(TX_SPEAKER_MCCOY, TX_LOV5_019);
}

void Room::loveaUseCureSampleOnSpock() {
	if (_awayMission->love.spockCured) {
		walkCrewman(OBJECT_SPOCK, _roomVar.love.cmnXPosToCureSpock, _roomVar.love.cmnYPosToCureSpock, 99);
		walkCrewman(OBJECT_MCCOY, _roomVar.love.cmnXPosToCureSpock, _roomVar.love.cmnYPosToCureSpock + 10, 99);
	} else
		showText(TX_SPEAKER_MCCOY, TX_LOVA_F55);
}


void Room::loveaUseCureOnSpock() {
	if (_awayMission->love.spockCured && _vm->isFloppyVersion())
		showText(TX_SPEAKER_SPOCK, TX_LOVAF002);
	else if (_awayMission->love.spockCured)
		showText(TX_SPEAKER_MCCOY, TX_LOV5_015);
	else {
		walkCrewman(OBJECT_SPOCK, _roomVar.love.cmnXPosToCureSpock, _roomVar.love.cmnYPosToCureSpock, 99);
		walkCrewman(OBJECT_MCCOY, _roomVar.love.cmnXPosToCureSpock, _roomVar.love.cmnYPosToCureSpock + 10, 99);
	}
}

void Room::loveaSpockOrMccoyInPositionToUseCure() {
	_roomVar.love.spockAndMccoyReadyToUseCure++;
	if (_roomVar.love.spockAndMccoyReadyToUseCure == 2)
		loadActorAnim2(OBJECT_MCCOY, "musemn", -1, -1, 99);
}

void Room::loveaFinishedCuringSpock() {
	showText(TX_SPEAKER_MCCOY, TX_LOV5_030);
	showText(TX_SPEAKER_SPOCK, TX_LOV5_038);
	showText(TX_SPEAKER_MCCOY, TX_LOV5_027);
	showText(TX_SPEAKER_SPOCK, TX_LOV5C001);
	_awayMission->love.spockCured = true;
}


// Timer 2 counts down the time until Spock and the Romulans succumb to the virus.
// BUG(-ish): once Spock is cured, the romulans will never succumb to the virus.
void Room::loveaTimer2Expired() {
	if (!_awayMission->love.spockCured) {
		_awayMission->love.spockInfectionCounter++;
		if (_awayMission->love.spockInfectionCounter >= 100) { // Spock succumbs, game over
			loadActorAnim(OBJECT_SPOCK, "sgetdn", -1, -1, 0);
			playMidiMusicTracks(MIDITRACK_2);
			showDescription(TX_GENER004);
			showGameOverMenu();
		} else if (_awayMission->love.spockInfectionCounter == 15) {
			showText(TX_SPEAKER_SPOCK, TX_SPOKCOFF);
			_awayMission->love.spockInfectionCounter++;
		} else if (_awayMission->love.spockInfectionCounter == 30) {
			if (_vm->isFloppyVersion())
				showText(TX_SPEAKER_SPOCK, TX_LOVAF003);
			else
				showText(TX_SPEAKER_MCCOY, TX_LOVA_F08);
			_awayMission->love.spockInfectionCounter++;
		} else if (_awayMission->love.spockInfectionCounter == 45) {
			showText(TX_SPEAKER_SPOCK, TX_SPOKCOFF);
			_awayMission->love.spockInfectionCounter++;
		} else if (_awayMission->love.spockInfectionCounter == 60) {
			showText(TX_SPEAKER_SPOCK, TX_LOVA_F54);
			_awayMission->love.romulansUnconsciousFromVirus = true;
			_awayMission->love.spockInfectionCounter++;
		}
	}

	_awayMission->timers[2] = 200;
}

void Room::loveaUseMTricorderOnSpock() {
	// Mccoy doesn't do his animation, I guess because he doesn't know which way to face?
	// (possible future enhancement?)
	playSoundEffectIndex(kSfxTricorder);

	if (_awayMission->love.spockCured)
		showText(TX_SPEAKER_MCCOY, TX_LOVA_F01);
	else if (!_awayMission->love.knowAboutVirus)
		showText(TX_SPEAKER_MCCOY, TX_LOVA_F02);
	else if (_awayMission->love.spockInfectionCounter < 10)
		showText(TX_SPEAKER_MCCOY, TX_LOVA_F04);
	else if (_awayMission->love.spockInfectionCounter < 30)
		showText(TX_SPEAKER_MCCOY, TX_LOVA_F03);
	else if (_awayMission->love.spockInfectionCounter < 50)
		showText(TX_SPEAKER_MCCOY, TX_LOVA_F08);
	else if (_awayMission->love.spockInfectionCounter < 70) // BUGFIX: < 70 instead of == 70
		showText(TX_SPEAKER_MCCOY, TX_TUG2_010);
	else if (_awayMission->love.spockInfectionCounter < 100)
		showText(TX_SPEAKER_MCCOY, TX_LOVA_F10);
	else
		showText(TX_SPEAKER_MCCOY, TX_LOVA_100);
}

void Room::loveaUseMTricorderOnHuman() {
	playSoundEffectIndex(kSfxTricorder);
	showText(TX_SPEAKER_MCCOY, TX_LOVA_F07);
}

void Room::loveaUseRomulanLaughingGas() {
	showDescription(TX_LOV2N005);
	_awayMission->love.releasedRomulanLaughingGas = true;
	loseItem(OBJECT_IRLG);

	// BUGFIX: start the effects immediately
	_awayMission->timers[1] = getRandomWordInRange(200, 400);
}

void Room::loveaUseHumanLaughingGas() {
	showDescription(TX_LOV2N005);
	if (_vm->isFloppyVersion())
		showText(TX_SPEAKER_MCCOY, TX_LOVAF004);
	else
		showText(TX_SPEAKER_SPOCK, TX_MUD2_040);
	_awayMission->love.releasedHumanLaughingGas = true;
	loseItem(OBJECT_IN2O);
	playMidiMusicTracks(MIDITRACK_3);

	// BUGFIX: start the effects immediately
	_awayMission->timers[0] = getRandomWordInRange(200, 400);
}

void Room::loveaUseAmmonia() {
	showDescription(TX_LOV2N005);
	if (_vm->isFloppyVersion() && !_awayMission->redshirtDead)
		showText(TX_SPEAKER_FERRIS, TX_LOVAF005);
	loseItem(OBJECT_INH3);
}

void Room::loveaUseCommunicator() {
	if (_vm->isFloppyVersion()) {
		if (!_awayMission->love.gotCure) {
			if (!_awayMission->love.contactedEnterpriseBeforeCure) {
				showText(TX_SPEAKER_KIRK,  TX_LOVAF006);
				showText(TX_SPEAKER_UHURA, TX_LOVAF007);
				showText(TX_SPEAKER_UHURA, TX_LOVAF008);
				showText(TX_SPEAKER_KIRK,  TX_LOVAF009);
				showText(TX_SPEAKER_MCCOY, TX_LOVAF010);
				showText(TX_SPEAKER_SPOCK, TX_LOVAF011);
				showText(TX_SPEAKER_MCCOY, TX_LOVAF012);
				_awayMission->love.contactedEnterpriseBeforeCure = true;
			} else {
				showText(TX_SPEAKER_KIRK,  TX_LOVAF013);
				showText(TX_SPEAKER_UHURA, TX_BRIDU146);
				showText(TX_SPEAKER_KIRK,  TX_VENA_F41);
			}
		} else if (!_awayMission->love.contactedEnterpriseAfterCure) {
			showText(TX_SPEAKER_KIRK,  TX_LOVAF014);
			showText(TX_SPEAKER_UHURA, TX_LOVAF015);
			showText(TX_SPEAKER_KIRK,  TX_LOVAF016);
			showText(TX_SPEAKER_SCOTT, TX_LOVAF017);
			showText(TX_SPEAKER_KIRK,  TX_LOVAF018);
			showText(TX_SPEAKER_MCCOY, TX_LOVAF019);
			showText(TX_SPEAKER_SPOCK, TX_LOVAF020);
			_awayMission->love.contactedEnterpriseAfterCure = true;
		} else {
			showText(TX_SPEAKER_KIRK,  TX_LOVAF013);
			showText(TX_SPEAKER_UHURA, TX_LOVAF021);
			showText(TX_SPEAKER_KIRK,  TX_LOVAF022);
		}
		return;
	}

	// There were originally number of branches here, based on whether we've got the cure
	// and whether we've contacted the enterprise already; but all of them branch to the
	// exact same code.
	showText(TX_SPEAKER_KIRK,  TX_MUD4_018);
	showText(TX_SPEAKER_UHURA, TX_BRIDU146);
	showText(TX_SPEAKER_KIRK,  TX_VENA_F41);
	_awayMission->love.contactedEnterpriseBeforeCure = true;
}

}
