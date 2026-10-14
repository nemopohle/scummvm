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
#define OBJECT_LIFE_SUPPORT_GENERATOR 9
#define OBJECT_DOOR 10
#define OBJECT_ALIENDV 11

#define HOTSPOT_HATCH 0x20
#define HOTSPOT_ENGINE 0x21
#define HOTSPOT_DOOR 0x22
#define HOTSPOT_CRANE 0x23

const int16 GENERATOR_X = 0x54;
const int16 GENERATOR_Y = 0xb0;

namespace StarTrek {

extern const RoomAction mudd5ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::mudd5Tick1 },
	{ {ACTION_TIMER_EXPIRED, 1, 0, 0, 0},        &Room::mudd5Timer1Expired },
	{ {ACTION_TIMER_EXPIRED, 2, 0, 0, 0},        &Room::mudd5Timer2Expired },
	{ {ACTION_USE, OBJECT_ICOMM, 0xff, 0, 0},      &Room::mudd5UseCommunicator },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0xff, 0, 0},   &Room::mudd5UseSTricorderAnywhere },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x21, 0, 0}, &Room::mudd5UseSTricorderOnEngine },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x22, 0, 0}, &Room::mudd5UseSTricorderOnCrane },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x20, 0, 0}, &Room::mudd5UseSTricorderOnHatch },
	{ {ACTION_USE, OBJECT_ISTRICOR, 9,    0, 0}, &Room::mudd5UseSTricorderOnLifeSupportGenerator },
	{ {ACTION_USE, OBJECT_IPHASERS, 0x20, 0, 0}, &Room::mudd5UseStunPhaserOnHatch },
	{ {ACTION_USE, OBJECT_IPHASERK, 0x20, 0, 0}, &Room::mudd5UseKillPhaserOnHatch },
	{ {ACTION_USE, 0xff,            9,    0, 0}, &Room::mudd5UseAnythingOnLifeSupportGenerator },
	{ {ACTION_USE, OBJECT_IDOOVER,  9,    0, 0}, &Room::mudd5UseDooverOnLifeSupportGenerator },
	{ {ACTION_FINISHED_WALKING, 2, 0, 0, 0},     &Room::mudd5KirkReachedLifeSupportGenerator },
	{ {ACTION_TIMER_EXPIRED, 4, 0, 0, 0},        &Room::mudd5KirkTimer4Expired },
	{ {ACTION_FINISHED_ANIMATION, 2, 0, 0, 0},   &Room::mudd5KirkRepairedLifeSupportGenerator },
	{ {ACTION_TIMER_EXPIRED, 3, 0, 0, 0},        &Room::mudd5KirkTimer3Expired },

	// Common code (next 4 lines)
	{ {ACTION_USE, OBJECT_ILENSES, OBJECT_IDEGRIME, 0, 0}, &Room::muddaUseLenseOnDegrimer },
	{ {ACTION_USE, OBJECT_IALIENDV, 0xff, 0, 0}, &Room::muddaUseAlienDevice },
	{ {ACTION_FINISHED_ANIMATION, 9, 0, 0, 0}, &Room::muddaFiredAlienDevice },
	{ {ACTION_USE, OBJECT_IDEGRIME, 0xff, 0, 0}, &Room::muddaUseDegrimer },

	{ {ACTION_LOOK, 0x20, 0, 0, 0},         &Room::mudd5LookAtHatch },
	{ {ACTION_LOOK, 9,    0, 0, 0},         &Room::mudd5LookAtLifeSupportGenerator },
	{ {ACTION_TOUCHED_HOTSPOT, 0, 0, 0, 0}, &Room::mudd5TouchedHotspot0 },
	{ {ACTION_WALK, 0x22, 0, 0, 0},         &Room::mudd5WalkToDoor },
	{ {ACTION_LOOK, OBJECT_KIRK, 0, 0, 0},     &Room::mudd5LookAtKirk },
	{ {ACTION_LOOK, OBJECT_SPOCK, 0, 0, 0},    &Room::mudd5LookAtSpock },
	{ {ACTION_LOOK, OBJECT_MCCOY, 0, 0, 0},    &Room::mudd5LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::mudd5LookAtRedshirt },
	{ {ACTION_LOOK, 0x22, 0, 0, 0},            &Room::mudd5LookAtDoor },
	{ {ACTION_LOOK, 0x23, 0, 0, 0},            &Room::mudd5LookAtCrane },
	{ {ACTION_LOOK, 0x21, 0, 0, 0},            &Room::mudd5LookAtEngine },
	{ {ACTION_TALK, OBJECT_KIRK, 0, 0, 0},     &Room::mudd5TalkToKirk },
	{ {ACTION_TALK, OBJECT_SPOCK, 0, 0, 0},    &Room::mudd5TalkToSpock },
	{ {ACTION_TALK, OBJECT_MCCOY, 0, 0, 0},    &Room::mudd5TalkToMccoy },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::mudd5TalkToRedshirt },
	{ {ACTION_USE, OBJECT_IMEDKIT, 0xff, 0, 0},  &Room::mudd5UseMedkit },

	// Common code (countdown for losing atmosphere when life support malfunctioning)
	{ {ACTION_TICK, 0xff, 0xff, 0xff, 0},           &Room::muddaTick },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum mudd5TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_UHURA,
	TX_SPEAKER_BUCHERT, TX_SPEAKER_MUDD,
	TX_MUD5_001, TX_MUD5_002, TX_MUD5_003, TX_MUD5_004, TX_MUD5_005,
	TX_MUD5_006, TX_MUD5_007, TX_MUD5_009, TX_MUD5_010, TX_MUD5_011,
	TX_MUD5_012, TX_MUD5_013, TX_MUD5_014, TX_MUD5_015, TX_MUD5_016,
	TX_MUD5_017, TX_MUD5_018, TX_MUD5_019, TX_MUD5_020, TX_MUD5_021,
	TX_MUD5_022, TX_MUD5_023, TX_MUD5_024, TX_MUD5_025, TX_MUD5_026,
	TX_MUD5_027, TX_MUD5_028, TX_MUD5_030, TX_MUD5_031, TX_MUD5_032,
	TX_MUD5_033, TX_MUD5_034, TX_MUD5_035, TX_MUD5_036, TX_MUD5_037,
	TX_MUD5_038, TX_MUD5_039, TX_MUD5_040, TX_MUD2_002, TX_MUD5N000,
	TX_MUD5N001, TX_MUD5N002, TX_MUD5N003, TX_MUD5N004, TX_MUD5N005,
	TX_MUD5N006, TX_MUD5N007, TX_MUD5N008, TX_MUD5N009, TX_MUD5N010,
	TX_MUD5N011, TX_MUD5N012, TX_MUD5N105, TX_STATICU1
};

extern const RoomTextOffsets mudd5TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 2736, 2630, 2797, 2754, 2652, 2648, 2735 },
	{ TX_SPEAKER_MCCOY, 2747, 2641, 2808, 2765, 2663, 2662, 2747 },
	{ TX_SPEAKER_SPOCK, 2757, 2651, 2818, 2775, 2673, 2672, 2757 },
	{ TX_SPEAKER_UHURA, 646, 625, 645, 666, 643, 643, 641 },
	{ TX_SPEAKER_BUCHERT, 2767, 2661, 2828, 2785, 2683, 2682, 2767 },
	{ TX_SPEAKER_MUDD, 2779, 2673, 2840, 2797, 2695, 2694, 2779 },
	{ TX_MUD5_001, 3484, 3287, 3654, 3528, 3310, 3345, 3503 },
	{ TX_MUD5_002, 6809, 6147, 7438, 6851, 6169, 6225, 6823 },
	{ TX_MUD5_003, 609, 583, 609, 609, 601, 601, 601 },
	{ TX_MUD5_004, 7313, 6576, 7988, 7379, 6598, 6675, 7298 },
	{ TX_MUD5_005, 6045, 5517, 6573, 6073, 5539, 5576, 6093 },
	{ TX_MUD5_006, 5739, 5271, 6241, 5754, 5292, 5314, 5821 },
	{ TX_MUD5_007, 5037, 4659, 5469, 5049, 4679, 4695, 5139 },
	{ TX_MUD5_009, 3318, 3152, 3442, 3359, 3174, 3205, 3321 },
	{ TX_MUD5_010, 5829, 5346, 6341, 5847, 5367, 5392, 5906 },
	{ TX_MUD5_011, 2965, 2844, 3061, 2984, 2866, 2866, 2985 },
	{ TX_MUD5_012, 2481, 2390, 2536, 2503, 2412, 2412, 2489 },
	{ TX_MUD5_013, 5393, 4970, 5877, 5401, 4991, 5005, 5502 },
	{ TX_MUD5_014, 4477, 4145, 4847, 4524, 4166, 4197, 4562 },
	{ TX_MUD5_015, 3774, 3547, 4022, 3829, 3569, 3617, 3841 },
	{ TX_MUD5_016, 7112, 6405, 7763, 7186, 6427, 6512, 7103 },
	{ TX_MUD5_017, 7194, 6472, 7858, 7267, 6494, 6579, 7183 },
	{ TX_MUD5_018, 7524, 6757, 8186, 7579, 6779, 6839, 7466 },
	{ TX_MUD5_019, 4612, 4265, 4978, 4639, 4285, 4294, 4678 },
	{ TX_MUD5_020, 5276, 4868, 5760, 5297, 4889, 4916, 5400 },
	{ TX_MUD5_021, 4776, 4413, 5182, 4797, 4434, 4438, 4867 },
	{ TX_MUD5_022, 2791, 2685, 2852, 2809, 2707, 2706, 2791 },
	{ TX_MUD5_023, 7906, 7098, 8613, 7987, 7122, 7195, 7848 },
	{ TX_MUD5_024, 3359, 3178, 3482, 3403, 3200, 3234, 3346 },
	{ TX_MUD5_025, 3551, 3339, 3723, 3599, 3362, 3402, 3557 },
	{ TX_MUD5_026, 4382, 4065, 4731, 4427, 4086, 4115, 4461 },
	{ TX_MUD5_027, 4076, 3804, 4387, 4122, 3825, 3865, 4161 },
	{ TX_MUD5_028, 5137, 4744, 5594, 5155, 4764, 4787, 5249 },
	{ TX_MUD5_030, 3869, 3627, 4151, 3911, 3649, 3684, 3955 },
	{ TX_MUD5_031, 3192, 3041, 3315, 3240, 3063, 3092, 3209 },
	{ TX_MUD5_032, 6971, 6294, 7601, 7027, 6316, 6385, 6971 },
	{ TX_MUD5_033, 7022, 6330, 7657, 7079, 6352, 6421, 7012 },
	{ TX_MUD5_034, 5629, 5176, 6115, 5638, 5197, 5213, 5710 },
	{ TX_MUD5_035, 6005, 5492, 6540, 6034, 5514, 5551, 6075 },
	{ TX_MUD5_036, 6078, 5536, 6609, 6112, 5557, 5601, 6114 },
	{ TX_MUD5_037, 5884, 5386, 6402, 5905, 5407, 5436, 5952 },
	{ TX_MUD5_038, 5527, 5089, 5994, 5526, 5110, 5116, 5604 },
	{ TX_MUD5_039, 3928, 3671, 4222, 3967, 3693, 3725, 4011 },
	{ TX_MUD5_040, 7399, 6647, 8073, 7465, 6669, 6749, 7368 },
	{ TX_MUD2_002, 6142, 5585, 6700, 6188, 5607, 5671, 6190 },
	{ TX_MUD5N000, 7842, 7049, 8544, 7916, 7073, 7139, 7794 },
	{ TX_MUD5N001, 6351, 5764, 6925, 6387, 5786, 5842, 6385 },
	{ TX_MUD5N002, 6437, 5835, 7023, 6496, 5857, 5936, 6468 },
	{ TX_MUD5N003, 6718, 6071, 7342, 6766, 6093, 6158, 6742 },
	{ TX_MUD5N004, 6638, 6006, 7236, 6688, 6028, 6096, 6651 },
	{ TX_MUD5N005, 6557, 5940, 7147, 6611, 5962, 6035, 6577 },
	{ TX_MUD5N006, 7716, 6938, 8404, 7795, 6960, 7039, 7669 },
	{ TX_MUD5N007, 2122, 2046, 2150, 2146, 2068, 2072, 2128 },
	{ TX_MUD5N008, 4152, 3865, 4487, 4198, 3886, 3926, 4300 },
	{ TX_MUD5N009, 6270, 5698, 6831, 6311, 5720, 5781, 6306 },
	{ TX_MUD5N010, 8007, 7184, 8709, 8082, 7208, 7275, 7929 },
	{ TX_MUD5N011, 3084, 2948, 3175, 3101, 2970, 2968, 3084 },
	{ TX_MUD5N012, 4234, 3932, 4555, 4280, 3953, 3981, 4300 },
	{ TX_MUD5N105, 1928, 1867, 1927, 1945, 1889, 1886, 1920 },
	{          -1, 0,    0, 0, 0, 0, 0, 0 }
};

extern const RoomText mudd5Texts[] = {
	  { -1, Common::UNK_LANG, "" }
};

void Room::mudd5Tick1() {
	playVoc("MUD5LOOP");

	_awayMission->crewDirectionsAfterWalk[OBJECT_REDSHIRT] = DIR_E;
	if (_awayMission->mudd.lifeSupportMalfunctioning) {
		playMidiMusicTracks(MIDITRACK_19);
		loadActorAnim(OBJECT_LIFE_SUPPORT_GENERATOR, "s4epls", GENERATOR_X, GENERATOR_Y);
	} else {
		loadActorAnim(OBJECT_LIFE_SUPPORT_GENERATOR, "s4eplo", GENERATOR_X, GENERATOR_Y);
		_awayMission->mudd.numTimesEnteredRoom5++;
		// BUG: this event can become permanently unavailable? (test)
		if (_awayMission->mudd.numTimesEnteredRoom5 == 2 && !_awayMission->mudd.muddUnavailable && !_awayMission->mudd.repairedLifeSupportGenerator) {
			_awayMission->mudd.muddUnavailable = true;
			_awayMission->mudd.numTimesEnteredRoom5 = 1;
			_awayMission->disableInput = 2;
			playMidiMusicTracks(MIDITRACK_3);
			loadActorAnim(OBJECT_MUDD, "s4ephh", 0x0e, 0xa7);
			_awayMission->timers[1] = 112;
			_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_W;
		}
	}
}

void Room::mudd5Timer1Expired() { // Mudd enters room through hatch
	const TextRef choices[] = {
		TX_SPEAKER_KIRK,
		TX_MUD5_006,
		TX_MUD5_010,
		TX_END
	};

	showText(TX_SPEAKER_MUDD, TX_MUD5_034);
	showMultipleTexts(choices);
	showText(TX_SPEAKER_MUDD, TX_MUD5_037);

	loadActorAnim2(OBJECT_MUDD, "s4ephc");
	_awayMission->timers[2] = 140;
}

void Room::mudd5Timer2Expired() { // Life-support generator starts acting up
	_awayMission->disableInput = false;
	_awayMission->mudd.lifeSupportMalfunctioning = true;
	playMidiMusicTracks(MIDITRACK_19);
	loadActorAnim(OBJECT_LIFE_SUPPORT_GENERATOR, "s4epls", GENERATOR_X, GENERATOR_Y);
}

void Room::mudd5UseCommunicator() {
	showText(TX_SPEAKER_KIRK,  TX_MUD5_003);
	showText(TX_SPEAKER_UHURA, TX_STATICU1);
}

void Room::mudd5UseSTricorderAnywhere() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_MUD5_022, false);
}

void Room::mudd5UseSTricorderOnEngine() {
	spockScan(DIR_E, TX_SPEAKER_SPOCK, TX_MUD5_011, false);
}

void Room::mudd5UseSTricorderOnCrane() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_MUD5_023, false);
}

void Room::mudd5UseSTricorderOnHatch() {
	spockScan(DIR_W, TX_SPEAKER_SPOCK, TX_MUD5_024, false);
	showText(TX_SPEAKER_KIRK, TX_MUD5_001);
}

void Room::mudd5UseSTricorderOnLifeSupportGenerator() {
	if (_awayMission->mudd.lifeSupportMalfunctioning)
		spockScan(DIR_W, TX_SPEAKER_SPOCK, TX_MUD5_019, false);
	else
		spockScan(DIR_W, TX_SPEAKER_SPOCK, TX_MUD5_021, false);
}

void Room::mudd5UseStunPhaserOnHatch() {
	showText(TX_SPEAKER_SPOCK, TX_MUD5_025);
	showText(TX_SPEAKER_MCCOY, TX_MUD5_015);
	showText(TX_SPEAKER_SPOCK, TX_MUD5_030);
}

void Room::mudd5UseKillPhaserOnHatch() {
	showText(TX_SPEAKER_BUCHERT, TX_MUD5_039);
	showText(TX_SPEAKER_SPOCK,   TX_MUD5_027);
}

void Room::mudd5UseAnythingOnLifeSupportGenerator() {
	showText(TX_SPEAKER_MCCOY, TX_MUD5_014); // BUGFIX: speaker is McCoy, not none
}


void Room::mudd5UseDooverOnLifeSupportGenerator() {
	if (_awayMission->mudd.lifeSupportMalfunctioning) {
		_awayMission->disableInput = true;
		_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_N;
		walkCrewmanC(OBJECT_KIRK, 0x57, 0xb4, &Room::mudd5KirkReachedLifeSupportGenerator);
	} else
		showText(TX_SPEAKER_MCCOY, TX_MUD5_014); // BUGFIX: speaker is McCoy, not none
}

void Room::mudd5KirkReachedLifeSupportGenerator() {
	loadActorAnimC(OBJECT_KIRK, "kuseln", -1, -1, &Room::mudd5KirkRepairedLifeSupportGenerator);
	_awayMission->timers[4] = 20;
}

void Room::mudd5KirkTimer4Expired() {
	playMidiMusicTracks(MIDITRACK_NONE);
	loadActorAnim2(OBJECT_LIFE_SUPPORT_GENERATOR, "s4eplo");
	_awayMission->mudd.repairedLifeSupportGenerator = true;
	_awayMission->mudd.missionScore += 2;
}

void Room::mudd5KirkRepairedLifeSupportGenerator() {
	showText(TX_SPEAKER_KIRK,  TX_MUD5_007);
	showText(TX_SPEAKER_SPOCK, TX_MUD5_028);

	_awayMission->mudd.lifeSupportMalfunctioning = false;
	_awayMission->mudd.muddUnavailable = false;
	playMidiMusicTracks(MIDITRACK_3);
	loadActorAnim(OBJECT_MUDD, "s4ephh", 0x0e, 0xa7);
	_awayMission->disableInput = true;

	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_W;
	loadActorStandAnim(OBJECT_KIRK);

	_awayMission->timers[3] = 110;
}

void Room::mudd5KirkTimer3Expired() { // Mudd popped back in after repairing generator
	showText(TX_SPEAKER_MUDD, TX_MUD5_035);
	showText(TX_SPEAKER_KIRK, TX_MUD5_005);
	showText(TX_SPEAKER_MUDD, TX_MUD5_036);

	loadActorAnim2(OBJECT_MUDD, "s4ephc");
	_awayMission->disableInput = false;
}

void Room::mudd5LookAtHatch() {
	showDescription(TX_MUD5N011);
	showText(TX_SPEAKER_SPOCK, TX_MUD5_031);
	showText(TX_SPEAKER_KIRK,  TX_MUD5_009);
}

void Room::mudd5LookAtLifeSupportGenerator() {
	if (_awayMission->mudd.lifeSupportMalfunctioning)
		showDescription(TX_MUD5N008);
	else {
		showDescription(TX_MUD5N012);
		showText(TX_SPEAKER_SPOCK, TX_MUD5_026);
	}
}

void Room::mudd5TouchedHotspot0() { // Triggers door
	if (_roomVar.mudd.walkingToDoor) {
		playVoc("SMADOOR3");
		loadActorAnim(OBJECT_DOOR, "s4epdo", 0x85, 0x74);
	}
}

void Room::mudd5WalkToDoor() {
	_roomVar.mudd.walkingToDoor = true;
	_awayMission->disableInput = true;
	walkCrewman(OBJECT_KIRK, 0x85, 0x74);
}

void Room::mudd5LookAtKirk() {
	showDescription(TX_MUD5N002);
}

void Room::mudd5LookAtSpock() {
	showDescription(TX_MUD5N005);
}

void Room::mudd5LookAtMccoy() {
	showDescription(TX_MUD5N004);
}

void Room::mudd5LookAtRedshirt() {
	showDescription(TX_MUD5N003);
}

void Room::mudd5LookAtDoor() {
	showDescription(TX_MUD5N010);
}

void Room::mudd5LookAtCrane() {
	showDescription(TX_MUD5N000);
}

void Room::mudd5LookAtEngine() {
	showDescription(TX_MUD5N006);
}

void Room::mudd5TalkToKirk() {
	showText(TX_SPEAKER_KIRK,  TX_MUD5_002);
	showText(TX_SPEAKER_SPOCK, TX_MUD5_032);
}

void Room::mudd5TalkToSpock() {
	showText(TX_SPEAKER_SPOCK, TX_MUD5_033);
	showText(TX_SPEAKER_MCCOY, TX_MUD5_016);
}

void Room::mudd5TalkToMccoy() {
	showText(TX_SPEAKER_MCCOY, TX_MUD5_017);
	showText(TX_SPEAKER_KIRK,  TX_MUD5_004);
}

void Room::mudd5TalkToRedshirt() {
	showText(TX_SPEAKER_BUCHERT, TX_MUD5_040);
	showText(TX_SPEAKER_MCCOY,   TX_MUD5_018);
}

void Room::mudd5UseMedkit() {
	showText(TX_SPEAKER_MCCOY, TX_MUD5_012);
}

}
