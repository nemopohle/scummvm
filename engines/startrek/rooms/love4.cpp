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
#include "startrek/startrek.h"

#define OBJECT_DOOR 8
#define OBJECT_ROMULAN_1 9
#define OBJECT_ROMULAN_2 10
#define OBJECT_ROMULAN_3 11
#define OBJECT_ROMULAN_4 12

#define HOTSPOT_DOOR 0x20
#define HOTSPOT_LADDER 0x21

namespace StarTrek {

extern const RoomAction love4ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::love4Tick1 },
	{ {ACTION_TICK, 10, 0, 0, 0}, &Room::love4Tick10 },

	{ {ACTION_WALK, 8, 0, 0, 0}, &Room::love4WalkToDoor },
	{ {ACTION_WALK, 0x20, 0, 0, 0}, &Room::love4WalkToDoor },
	{ {ACTION_TOUCHED_HOTSPOT, 0, 0, 0, 0}, &Room::love4TouchedHotspot0 },
	{ {ACTION_FINISHED_WALKING, 1, 0, 0, 0}, &Room::love4DoorOpenedOrReached },
	{ {ACTION_FINISHED_ANIMATION, 1, 0, 0, 0}, &Room::love4DoorOpenedOrReached },

	{ {ACTION_USE, OBJECT_KIRK, 0x21, 0, 0}, &Room::love4UseKirkOnLadder },
	{ {ACTION_FINISHED_WALKING, 2, 0, 0, 0}, &Room::love4ReachedLadder },
	{ {ACTION_USE, OBJECT_IPHASERS,  9, 0, 0}, &Room::love4UseStunPhaserOnRomulan },
	{ {ACTION_USE, OBJECT_IPHASERS, 10, 0, 0}, &Room::love4UseStunPhaserOnRomulan },
	{ {ACTION_USE, OBJECT_IPHASERS, 11, 0, 0}, &Room::love4UseStunPhaserOnRomulan },
	{ {ACTION_USE, OBJECT_IPHASERS, 12, 0, 0}, &Room::love4UseStunPhaserOnRomulan },
	// NOTE: nothing for OBJECT_IPHASERK. There's an unused audio file which might fit.
	{ {ACTION_LOOK, 0xff, 0, 0, 0}, &Room::love4LookAnywhere },
	{ {ACTION_LOOK, 0x21, 0, 0, 0}, &Room::love4LookAtLadder },
	{ {ACTION_LOOK, 8, 0, 0, 0}, &Room::love4LookAtDoor },
	{ {ACTION_LOOK, OBJECT_KIRK, 0, 0, 0}, &Room::love4LookAtKirk },
	{ {ACTION_LOOK, OBJECT_MCCOY, 0, 0, 0}, &Room::love4LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_SPOCK, 0, 0, 0}, &Room::love4LookAtSpock },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::love4LookAtRedshirt },
	{ {ACTION_LOOK,  9, 0, 0, 0}, &Room::love4LookAtRomulan },
	{ {ACTION_LOOK, 10, 0, 0, 0}, &Room::love4LookAtRomulan },
	{ {ACTION_LOOK, 11, 0, 0, 0}, &Room::love4LookAtRomulan },
	{ {ACTION_LOOK, 12, 0, 0, 0}, &Room::love4LookAtRomulan },
	{ {ACTION_TALK, OBJECT_KIRK, 0, 0, 0}, &Room::love4TalkToKirk },
	{ {ACTION_TALK, OBJECT_MCCOY, 0, 0, 0}, &Room::love4TalkToMccoy },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::love4TalkToRedshirt },
	{ {ACTION_TALK, OBJECT_SPOCK, 0, 0, 0}, &Room::love4TalkToSpock },
	{ {ACTION_TALK,  9, 0, 0, 0}, &Room::love4TalkToRomulan },
	{ {ACTION_TALK, 10, 0, 0, 0}, &Room::love4TalkToRomulan },
	{ {ACTION_TALK, 11, 0, 0, 0}, &Room::love4TalkToRomulan },
	{ {ACTION_TALK, 12, 0, 0, 0}, &Room::love4TalkToRomulan },
	{ {ACTION_USE, OBJECT_IMTRICOR,  9, 0, 0}, &Room::love4UseMTricorderOnRomulan },
	{ {ACTION_USE, OBJECT_IMTRICOR, 10, 0, 0}, &Room::love4UseMTricorderOnRomulan },
	{ {ACTION_USE, OBJECT_IMTRICOR, 11, 0, 0}, &Room::love4UseMTricorderOnRomulan },
	{ {ACTION_USE, OBJECT_IMTRICOR, 12, 0, 0}, &Room::love4UseMTricorderOnRomulan },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0xff, 0, 0}, &Room::love4UseMTricorderAnywhere },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0xff, 0, 0}, &Room::love4UseSTricorderAnywhere },

	{ {ACTION_USE, OBJECT_ICURE,  9, 0, 0}, &Room::love4UseCureOnRomulan },
	{ {ACTION_USE, OBJECT_ICURE, 10, 0, 0}, &Room::love4UseCureOnRomulan },
	{ {ACTION_USE, OBJECT_ICURE, 11, 0, 0}, &Room::love4UseCureOnRomulan },
	{ {ACTION_USE, OBJECT_ICURE, 12, 0, 0}, &Room::love4UseCureOnRomulan },
	{ {ACTION_FINISHED_WALKING,   3, 0, 0, 0}, &Room::love4MccoyReachedRomulan4 },
	{ {ACTION_FINISHED_ANIMATION, 2, 0, 0, 0}, &Room::love4MccoyCuredRomulan4 },
	{ {ACTION_FINISHED_WALKING,   4, 0, 0, 0}, &Room::love4MccoyReachedRomulan3 },
	{ {ACTION_FINISHED_ANIMATION, 3, 0, 0, 0}, &Room::love4MccoyCuredRomulan3 },
	{ {ACTION_FINISHED_WALKING,   5, 0, 0, 0}, &Room::love4MccoyReachedRomulan2 },
	{ {ACTION_FINISHED_ANIMATION, 4, 0, 0, 0}, &Room::love4MccoyCuredRomulan2 },
	{ {ACTION_FINISHED_WALKING,   6, 0, 0, 0}, &Room::love4MccoyReachedRomulan1 },
	{ {ACTION_FINISHED_ANIMATION, 5, 0, 0, 0}, &Room::love4MccoyCuredRomulan1 },

	{ {ACTION_USE, OBJECT_IH2O,   9, 0, 0}, &Room::love4UseWaterOnRomulan },
	{ {ACTION_USE, OBJECT_IH2O,  10, 0, 0}, &Room::love4UseWaterOnRomulan },
	{ {ACTION_USE, OBJECT_IH2O,  11, 0, 0}, &Room::love4UseWaterOnRomulan },
	{ {ACTION_USE, OBJECT_IH2O,  12, 0, 0}, &Room::love4UseWaterOnRomulan },
	{ {ACTION_USE, OBJECT_IMEDKIT,  9, 0, 0}, &Room::love4UseMedkitOnRomulan },
	{ {ACTION_USE, OBJECT_IMEDKIT, 10, 0, 0}, &Room::love4UseMedkitOnRomulan },
	{ {ACTION_USE, OBJECT_IMEDKIT, 11, 0, 0}, &Room::love4UseMedkitOnRomulan },
	{ {ACTION_USE, OBJECT_IMEDKIT, 12, 0, 0}, &Room::love4UseMedkitOnRomulan },
	{ {ACTION_USE, OBJECT_ISAMPLE,  9, 0, 0}, &Room::love4UseCureSampleOnRomulan },
	{ {ACTION_USE, OBJECT_ISAMPLE, 10, 0, 0}, &Room::love4UseCureSampleOnRomulan },
	{ {ACTION_USE, OBJECT_ISAMPLE, 11, 0, 0}, &Room::love4UseCureSampleOnRomulan },
	{ {ACTION_USE, OBJECT_ISAMPLE, 12, 0, 0}, &Room::love4UseCureSampleOnRomulan },
	{ {ACTION_USE, 0xff,  9, 0, 0}, &Room::love4UseAnythingOnRomulan },
	{ {ACTION_USE, 0xff, 10, 0, 0}, &Room::love4UseAnythingOnRomulan },
	{ {ACTION_USE, 0xff, 11, 0, 0}, &Room::love4UseAnythingOnRomulan },
	{ {ACTION_USE, 0xff, 12, 0, 0}, &Room::love4UseAnythingOnRomulan },

	// Common code
	{ {ACTION_TIMER_EXPIRED, 0, 0, 0, 0}, &Room::loveaTimer0Expired },
	{ {ACTION_TIMER_EXPIRED, 1, 0, 0, 0}, &Room::loveaTimer1Expired },
	{ {ACTION_USE, OBJECT_IMEDKIT, OBJECT_SPOCK, 0, 0}, &Room::loveaUseMedkitOnSpock },
	{ {ACTION_USE, OBJECT_ISAMPLE, OBJECT_SPOCK, 0, 0}, &Room::loveaUseCureSampleOnSpock },
	{ {ACTION_USE, OBJECT_ICURE, OBJECT_SPOCK, 0, 0}, &Room::loveaUseCureOnSpock },
	{ {ACTION_FINISHED_WALKING,   99, 0, 0, 0}, &Room::loveaSpockOrMccoyInPositionToUseCure },
	{ {ACTION_FINISHED_ANIMATION, 99, 0, 0, 0}, &Room::loveaFinishedCuringSpock },
	{ {ACTION_TIMER_EXPIRED, 2, 0, 0, 0}, &Room::loveaTimer2Expired },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_SPOCK, 0, 0}, &Room::loveaUseMTricorderOnSpock },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_KIRK, 0, 0}, &Room::loveaUseMTricorderOnHuman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_MCCOY, 0, 0}, &Room::loveaUseMTricorderOnHuman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_REDSHIRT, 0, 0}, &Room::loveaUseMTricorderOnHuman },
	{ {ACTION_USE, OBJECT_IRLG, 0xff, 0, 0}, &Room::loveaUseRomulanLaughingGas },
	{ {ACTION_USE, OBJECT_IN2O, 0xff, 0, 0}, &Room::loveaUseHumanLaughingGas },
	{ {ACTION_USE, OBJECT_INH3, 0xff, 0, 0}, &Room::loveaUseAmmonia },
	{ {ACTION_USE, OBJECT_ICOMM, 0xff, 0, 0}, &Room::loveaUseCommunicator },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum love4TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_FERRIS,
	TX_LOV0_011, TX_LOV0_012, TX_LOV0_013, TX_LOV0_014, TX_LOV0_015,
	TX_LOV0_016, TX_LOV0_017, TX_LOV0_019, TX_LOV0_020, TX_LOV0_021,
	TX_LOV0_022, TX_LOV0_025, TX_LOV0_026, TX_LOV0_029, TX_LOV0_033,
	TX_LOV0_039, TX_LOV0_040, TX_LOV0_041, TX_LOV0_042, TX_LOV0_043,
	TX_LOV0_045, TX_LOV0_046, TX_LOV0_047, TX_LOV0_048, TX_LOV0_049,
	TX_LOV0_050, TX_LOV0_101, TX_LOV0_102, TX_LOV0_103, TX_LOV0_104,
	TX_LOV0_105, TX_LOV0_106, TX_LOV0_107, TX_LOV0_124, TX_LOV4_001,
	TX_LOV4_002, TX_LOV4_003, TX_LOV4_004, TX_LOV4_005, TX_LOV4_006,
	TX_LOV4_007, TX_LOV4_009, TX_LOV4_010, TX_LOV4_011, TX_LOV4_012,
	TX_LOV4_013, TX_LOV4_014, TX_LOV4_015, TX_LOV4_016, TX_LOV4_017,
	TX_LOV4_018, TX_LOV4_019, TX_LOV4_020, TX_LOV4_021, TX_LOV4_022,
	TX_LOV4_023, TX_LOV4_024, TX_LOV4_025, TX_LOV4_026, TX_LOV4_027,
	TX_LOV4_028, TX_LOV4_029, TX_LOV5_015, TX_LOV5_019, TX_LOV5_027,
	TX_LOV5_030, TX_LOV5_038, TX_LOVA_100, TX_LOVA_F01, TX_LOVA_F02,
	TX_LOVA_F03, TX_LOVA_F04, TX_LOVA_F07, TX_LOVA_F08, TX_LOVA_F10,
	TX_LOVA_F54, TX_LOVA_F55, TX_MUD2_040, TX_MUD4_018, TX_TUG2_010,
	TX_VENA_F41, TX_FEA5_008, TX_LOV2N005, TX_LOV4N000, TX_LOV4N001,
	TX_LOV4N002, TX_LOV4N003, TX_LOV4N004, TX_LOV4N005, TX_LOV4N006,
	TX_LOV4N007, TX_LOV4N008, TX_LOV4N009, TX_LOV4N010, TX_LOV4N011,
	TX_LOV4N013, TX_BRIDU146, TX_GENER004, TX_LOV4F001, TX_LOV4F002
};

extern const RoomTextOffsets love4TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 7266, 7634, 7447, 7646, 7625, 8204, 8174, 7625, 7296 },
	{ TX_SPEAKER_MCCOY, 7277, 7645, 7458, 7657, 7636, 8217, 8185, 7636, 7307 },
	{ TX_SPEAKER_SPOCK, 7287, 7655, 7468, 7667, 7646, 8227, 8195, 7646, 7317 },
	{ TX_SPEAKER_FERRIS, 7297, 7665, 7478, 7677, 7656, 8237, 8205, 7656, 7327 },
	{ TX_LOV0_011, 8771, 8925, 9119, 9263, 8908, 9618, 9638, 8908, 8801 },
	{ TX_LOV0_012, 8898, 9007, 9249, 9388, 8990, 9701, 9723, 8990, 8928 },
	{ TX_LOV0_013, 9194, 9228, 9589, 9698, 9211, 9939, 9991, 9211, 9221 },
	{ TX_LOV0_014, 8958, 9052, 9323, 9446, 9035, 9745, 9785, 9035, 8988 },
	{ TX_LOV0_015, 9062, 9126, 9438, 9548, 9109, 9818, 9870, 9109, 9089 },
	{ TX_LOV0_016, 8865, 8989, 9213, 9355, 8972, 9680, 9702, 8972, 8895 },
	{ TX_LOV0_017, 9328, 9332, 9730, 9830, 9315, 10042, 10101, 9315, 9355 },
	{ TX_LOV0_019, 9248, 9267, 9651, 9749, 9250, 9975, 10038, 9250, 9275 },
	{ TX_LOV0_020, 8805, 8944, 9149, 9297, 8927, 9637, 9653, 8927, 8835 },
	{ TX_LOV0_021, 9116, 9165, 9502, 9597, 9148, 9853, 9919, 9148, 9143 },
	{ TX_LOV0_022, 9006, 9085, 9376, 9502, 9068, 9786, 9823, 9068, 9033 },
	{ TX_LOV0_025, 10254, 9739, 10774, 10798, 9722, 10538, 10617, 9722, 10281 },
	{ TX_LOV0_026, 10127, 9468, 10631, 10645, 9451, 10185, 10277, 9451, 10154 },
	{ TX_LOV0_029, 10007, 9378, 10479, 10517, 9361, 10087, 10155, 9361, 10034 },
	{ TX_LOV0_033, 10067, 9423, 10549, 10583, 9406, 10138, 10210, 9406, 10094 },
	{ TX_LOV0_039, 9389, 0, 9799, 9890, 8908, 9618, 9638, 8908, 9416 },
	{ TX_LOV0_040, 9516, 0, 9929, 10015, 8990, 9701, 9723, 8990, 9543 },
	{ TX_LOV0_041, 9812, 0, 10269, 10325, 9211, 9939, 9991, 9211, 9839 },
	{ TX_LOV0_042, 9576, 0, 10003, 10073, 9035, 9745, 9785, 9035, 9603 },
	{ TX_LOV0_043, 9680, 0, 10118, 10175, 9109, 9818, 9870, 9109, 9707 },
	{ TX_LOV0_045, 9483, 0, 9893, 9982, 8972, 9680, 9702, 8972, 9510 },
	{ TX_LOV0_046, 9946, 0, 10410, 10457, 9315, 10042, 10101, 9315, 9973 },
	{ TX_LOV0_047, 9866, 0, 10331, 10376, 9250, 9975, 10038, 9250, 9893 },
	{ TX_LOV0_048, 9423, 0, 9829, 9924, 8927, 9637, 9653, 8927, 9450 },
	{ TX_LOV0_049, 9734, 0, 10182, 10224, 9148, 9853, 9919, 9148, 9761 },
	{ TX_LOV0_050, 9624, 0, 10056, 10129, 9068, 9786, 9823, 9068, 9651 },
	{ TX_LOV0_101, 10335, 0, 10859, 10892, 0, 0, 0, 0, 10362 },
	{ TX_LOV0_102, 10399, 0, 10905, 10959, 0, 0, 0, 0, 10426 },
	{ TX_LOV0_103, 10469, 0, 10986, 11041, 0, 0, 0, 0, 10496 },
	{ TX_LOV0_104, 10519, 0, 11042, 11099, 0, 0, 0, 0, 10546 },
	{ TX_LOV0_105, 10576, 0, 11103, 11165, 0, 0, 0, 0, 10603 },
	{ TX_LOV0_106, 10688, 0, 11217, 11288, 0, 0, 0, 0, 10715 },
	{ TX_LOV0_107, 10756, 0, 11293, 11369, 0, 0, 0, 0, 10783 },
	{ TX_LOV0_124, 10204, 9530, 10720, 10742, 9513, 10267, 10351, 9513, 10231 },
	{ TX_LOV4_001, 2899, 2708, 3023, 2964, 2707, 2779, 2832, 2707, 2895 },
	{ TX_LOV4_002, 1211, 1155, 1196, 1210, 1155, 1154, 1140, 1155, 1207 },
	{ TX_LOV4_003, 8428, 8508, 8746, 8880, 8492, 9140, 9165, 8492, 8458 },
	{ TX_LOV4_004, 3355, 3103, 3521, 3452, 3102, 3206, 3270, 3102, 3351 },
	{ TX_LOV4_005, 1095, 1069, 1095, 1095, 1069, 1069, 1069, 1069, 1091 },
	{ TX_LOV4_006, 8685, 8854, 9018, 9168, 8837, 9538, 9552, 8837, 8715 },
	{ TX_LOV4_007, 7977, 8195, 8236, 8393, 8186, 8795, 8812, 8186, 8007 },
	{ TX_LOV4_009, 3010, 2803, 3152, 3083, 2802, 2883, 2946, 2802, 3006 },
	{ TX_LOV4_010, 8197, 8322, 8477, 8638, 8306, 8943, 8941, 8306, 8227 },
	{ TX_LOV4_011, 3416, 3149, 3579, 3518, 3148, 3257, 3313, 3148, 3412 },
	{ TX_LOV4_012, 3537, 3255, 3716, 3651, 3242, 3364, 3423, 3242, 3533 },
	{ TX_LOV4_013, 2078, 1962, 2130, 2117, 1961, 2007, 2014, 1961, 2074 },
	{ TX_LOV4_014, 7908, 8141, 8148, 8336, 8132, 8753, 8739, 8132, 7938 },
	{ TX_LOV4_015, 1733, 1647, 1777, 1771, 1646, 1690, 1691, 1646, 1729 },
	{ TX_LOV4_016, 10809, 9858, 11355, 11434, 9841, 10674, 10754, 9841, 10836 },
	{ TX_LOV4_017, 8130, 8285, 8404, 8558, 8269, 8893, 8898, 8269, 8160 },
	{ TX_LOV4_018, 1601, 1530, 1643, 1624, 1529, 1557, 1572, 1529, 1597 },
	{ TX_LOV4_019, 1855, 1754, 1912, 1900, 1753, 1804, 1811, 1753, 1851 },
	{ TX_LOV4_020, 8489, 8688, 8813, 8947, 8672, 9348, 9378, 8672, 8519 },
	{ TX_LOV4_021, 8618, 8802, 8948, 9089, 8785, 9474, 9497, 8785, 8648 },
	{ TX_LOV4_022, 1301, 1230, 1315, 1310, 1229, 1240, 1244, 1229, 1297 },
	{ TX_LOV4_023, 2545, 2399, 2632, 2594, 2398, 2454, 2486, 2398, 2541 },
	{ TX_LOV4_024, 1164, 1123, 1150, 1160, 1123, 1119, 1109, 1123, 1160 },
	{ TX_LOV4_025, 7610, 7903, 7812, 8023, 7894, 8498, 8464, 7894, 7640 },
	{ TX_LOV4_026, 2772, 2596, 2864, 2837, 2595, 2667, 2688, 2595, 2768 },
	{ TX_LOV4_027, 8360, 8455, 8666, 8813, 8439, 9088, 9100, 8439, 8390 },
	{ TX_LOV4_028, 8291, 8401, 8582, 8741, 8385, 9031, 9031, 8385, 8321 },
	{ TX_LOV4_029, 3295, 3058, 3453, 3380, 3057, 3148, 3217, 3057, 3291 },
	{ TX_LOV5_015, 4669, 0, 4842, 4804, 0, 0, 0, 0, 4665 },
	//{ TX_LOV5_015, 4490, 0, 0, 0 },
	{ TX_LOV5_019, 4412, 3918, 4598, 4534, 3905, 3364, 4093, 3905, 4408 },
	{ TX_LOV5_027, 4925, 4362, 5093, 5065, 4349, 4502, 4577, 4349, 4921 },
	{ TX_LOV5_030, 4829, 4293, 5001, 4968, 4280, 4424, 4504, 4280, 4825 },
	{ TX_LOV5_038, 4880, 4332, 5051, 5024, 4319, 4471, 4550, 4319, 4876 },
	{ TX_LOVA_100, 6034, 5460, 6178, 6290, 5449, 5725, 5778, 5449, 6056 },
	{ TX_LOVA_F01, 5476, 0, 5649, 5676, 0, 0, 0, 0, 5498 },
	{ TX_LOVA_F02, 5548, 0, 5714, 5748, 0, 0, 0, 0, 5570 },
	{ TX_LOVA_F03, 5747, 0, 5923, 5976, 0, 0, 0, 0, 5769 },
	{ TX_LOVA_F04, 5662, 0, 5831, 5874, 0, 0, 0, 0, 5684 },
	{ TX_LOVA_F07, 6111, 5522, 6257, 6368, 5511, 5788, 5842, 5511, 6133 },
	{ TX_LOVA_F08, 5100, 0, 5267, 5253, 0, 0, 0, 0, 5096 },
	//{ TX_LOVA_F08, 5815, 0, 0, 0 },
	{ TX_LOVA_F10, 5982, 0, 6131, 6230, 0, 0, 0, 0, 6004 },
	{ TX_LOVA_F54, 5256, 0, 5419, 5419, 0, 0, 0, 0, 5253 },
	{ TX_LOVA_F55, 4557, 4033, 4737, 4693, 4020, 4141, 4215, 4020, 4553 },
	{ TX_FEA5_008, 8051, 0, 8307, 8470, 0, 0, 0, 0, 8081 },
	{ TX_MUD2_040, 6307, 0, 6478, 6609, 0, 0, 0, 0, 6329 },
	{ TX_MUD4_018, 7013, 0, 7196, 7380, 0, 0, 0, 0, 7043 },
	//{ TX_MUD4_018, 6680, 0, 0, 0 },
	//{ TX_MUD4_018, 6516, 0, 0, 0 },
	//{ TX_MUD4_018, 6849, 0, 0, 0 },
	{ TX_TUG2_010, 5912, 5354, 6078, 6156, 5343, 5612, 5663, 5343, 5934 },
	{ TX_VENA_F41, 7136, 6551, 7317, 7515, 6542, 6945, 6951, 6542, 7166 },
	//{ TX_VENA_F41, 6803, 0, 0, 0 },
	//{ TX_VENA_F41, 6972, 0, 0, 0 },
	//{ TX_VENA_F41, 6639, 0, 0, 0 },
	{ TX_LOV2N005, 6170, 0, 6327, 6428, 0, 0, 0, 0, 6192 },
	//{ TX_LOV2N005, 6250, 0, 0, 0 },
	//{ TX_LOV2N005, 6396, 0, 0, 0 },
	{ TX_LOV4N000, 8170, 8310, 8444, 8602, 8294, 8922, 8923, 8294, 8200 },
	{ TX_LOV4N001, 7740, 8003, 7961, 8155, 7994, 8601, 8582, 7994, 7770 },
	{ TX_LOV4N002, 7554, 7862, 7755, 7954, 7853, 8454, 8422, 7853, 7584 },
	{ TX_LOV4N003, 7690, 7968, 7902, 8095, 7959, 8556, 8538, 7959, 7720 },
	{ TX_LOV4N004, 3206, 2984, 3352, 3277, 2983, 3063, 3131, 2983, 3202 },
	{ TX_LOV4N005, 2437, 2306, 2499, 2474, 2305, 2349, 2368, 2305, 2433 },
	{ TX_LOV4N006, 7309, 7677, 7490, 7689, 7668, 8249, 8217, 7668, 7339 },
	{ TX_LOV4N007, 1473, 1405, 1513, 1487, 1404, 1420, 1445, 1404, 1469 },
	{ TX_LOV4N008, 7843, 8091, 8081, 8271, 8082, 8703, 8687, 8082, 7873 },
	{ TX_LOV4N009, 7362, 7715, 7545, 7742, 7706, 8287, 8257, 7706, 7392 },
	{ TX_LOV4N010, 7495, 7818, 7688, 7901, 7809, 8416, 8370, 7809, 7525 },
	{ TX_LOV4N011, 7441, 7779, 7634, 7836, 7770, 8366, 8331, 7770, 7471 },
	{ TX_LOV4N013, 2692, 2531, 2782, 2762, 2530, 2607, 2621, 2530, 2688 },
	{ TX_BRIDU146, 6749, 6516, 6934, 7090, 6507, 6908, 6913, 6507, 6779 },
	//{ TX_BRIDU146, 7082, 0, 0, 0 },
	//{ TX_BRIDU146, 6918, 0, 0, 0 },
	//{ TX_BRIDU146, 6585, 0, 0, 0 },
	{ TX_GENER004, 5405, 4779, 5578, 5605, 4768, 4978, 5046, 4768, 5427 },
	{ TX_LOV4F001, 0, 8554, 0, 0, 8538, 9192, 9353, 8538, 0 },
	{ TX_LOV4F002, 0, 8671, 0, 0, 8655, 9325, 9353, 8655, 0 },
	{          -1, 0,    0, 0, 0, 0, 0, 0, 0, 0 }
};

extern const RoomText love4Texts[] = {
	{ -1, Common::UNK_LANG, "" }
};

void Room::love4Tick1() {
	playVoc("LOV4LOOP");

	if (_awayMission->love.romulansCured) {
		loadActorAnim2(OBJECT_ROMULAN_1,  "s3r5r1b", 0x36, 0xb3, 0);
		loadActorAnim2(OBJECT_ROMULAN_2, "s3r5r2b", 0xb9, 0xbb, 0);
		loadActorAnim2(OBJECT_ROMULAN_3, "s3r5r3b", 0xef, 0xc4, 0);
		loadActorAnim2(OBJECT_ROMULAN_4, "s3r5r4b", 0x12a, 0xaa, 0);
	} else {
		loadActorAnim2(OBJECT_ROMULAN_1,  "s3r5r1a", 0x36, 0xb3, 0);
		loadActorAnim2(OBJECT_ROMULAN_2, "s3r5r2a", 0xb9, 0xbb, 0);
		loadActorAnim2(OBJECT_ROMULAN_3, "s3r5r3a", 0xef, 0xc4, 0);
		loadActorAnim2(OBJECT_ROMULAN_4, "s3r5r4a", 0x12a, 0xaa, 0);
	}

	if (_awayMission->love.releasedHumanLaughingGas)
		_awayMission->timers[0] = getRandomWordInRange(200, 400);
	if (_awayMission->love.releasedRomulanLaughingGas)
		_awayMission->timers[1] = getRandomWordInRange(200, 400);
	_awayMission->timers[2] = 200;

	loadActorAnim(OBJECT_DOOR, "s3r5d1a", 0x90, 0x99, 0);

	_roomVar.love.cmnXPosToCureSpock = 0x6b;
	_roomVar.love.cmnYPosToCureSpock = 0xb2;
}

void Room::love4Tick10() {
	if (!_awayMission->love.visitedRoomWithRomulans) {
		playMidiMusicTracks(MIDITRACK_1);
		_awayMission->love.visitedRoomWithRomulans = true;
	}
}

void Room::love4WalkToDoor() {
	_awayMission->disableInput = true;
	_roomVar.love.walkingToDoor = true;
	walkCrewmanC(OBJECT_KIRK, 0x91, 0x9c, &Room::love4DoorOpenedOrReached);
}

// Triggers the door opening
void Room::love4TouchedHotspot0() {
	if (_roomVar.love.walkingToDoor) {
		loadActorAnimC(OBJECT_DOOR, "s3r5d1", 0x90, 0x99, &Room::love4DoorOpenedOrReached);
		playSoundEffectIndex(kSfxDoor);
	}
}

void Room::love4DoorOpenedOrReached() {
	_roomVar.love.doorOpenCounter++;
	if (_roomVar.love.doorOpenCounter == 2)
		loadRoomIndex(5, 1);
}

void Room::love4UseKirkOnLadder() {
	walkCrewmanC(OBJECT_KIRK, 0xf6, 0xac, &Room::love4ReachedLadder);
}

void Room::love4ReachedLadder() {
	loadRoomIndex(1, 2);
}

void Room::love4UseStunPhaserOnRomulan() {
	showText(TX_SPEAKER_MCCOY, TX_LOV4_007);
}

void Room::love4LookAnywhere() {
	showDescription(TX_LOV4N009);
}

void Room::love4LookAtLadder() {
	showDescription(TX_LOV4N010);
}

void Room::love4LookAtDoor() {
	showDescription(TX_LOV4N008);
}

void Room::love4LookAtKirk() {
	showDescription(TX_LOV4N002);
}

void Room::love4LookAtMccoy() {
	// NOTE: The audio is McCoy's voice; the unused audio LOV4N012 may fit the text better
	showDescription(TX_LOV4_025);
}

void Room::love4LookAtSpock() {
	showDescription(TX_LOV4N003);
}

void Room::love4LookAtRedshirt() {
	showDescription(TX_LOV4N001);
}

void Room::love4LookAtRomulan() {
	if (!_awayMission->love.romulansCured)
		showDescription(TX_LOV4N006);
	else
		showDescription(TX_LOV4N011);
}

void Room::love4TalkToKirk() {
	showText(TX_SPEAKER_KIRK,  TX_LOV4_005);
	showText(TX_SPEAKER_MCCOY, TX_LOV4_024);
	showText(TX_SPEAKER_KIRK,  TX_LOV4_002);
}

void Room::love4TalkToMccoy() {
	if (_awayMission->love.romulansCured)
		showText(TX_SPEAKER_MCCOY, TX_LOV4_022);
	else
		showText(TX_SPEAKER_MCCOY, TX_LOV4_010);
}

void Room::love4TalkToRedshirt() {
	showText(TX_SPEAKER_FERRIS, TX_LOV4_028);
}

void Room::love4TalkToSpock() {
	showText(TX_SPEAKER_SPOCK, TX_LOV4_027);
	showText(TX_SPEAKER_KIRK,  TX_LOV4_003);

	if (_vm->isFloppyVersion() && !_awayMission->redshirtDead) {
		showText(TX_SPEAKER_FERRIS, TX_LOV4F001);
		showText(TX_SPEAKER_SPOCK,  TX_LOV4F002);
	}
}

void Room::love4TalkToRomulan() {
	if (_awayMission->love.romulansCured)
		showDescription(TX_LOV4N007); // BUGFIX: original didn't play audio
}

void Room::love4UseMTricorderOnRomulan() {
	if (_awayMission->love.romulansCured)
		mccoyScan(DIR_S, TX_SPEAKER_MCCOY, TX_LOV4_018, false);
	else
		mccoyScan(DIR_S, TX_SPEAKER_MCCOY, TX_LOV4_015, false);
}

void Room::love4UseMTricorderAnywhere() {
	if (_awayMission->love.romulansCured)
		mccoyScan(DIR_S, TX_SPEAKER_MCCOY, TX_LOV4_019, false);
	else if (_awayMission->love.romulansUnconsciousFromLaughingGas)
		mccoyScan(DIR_S, TX_SPEAKER_MCCOY, TX_LOV4_020, false);
	else
		mccoyScan(DIR_S, TX_SPEAKER_MCCOY, TX_LOV4_021, false);
}

void Room::love4UseSTricorderAnywhere() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_LOV4_006, false);
}


// Mccoy walks around to all romulans, giving each the cure
void Room::love4UseCureOnRomulan() {
	if (_awayMission->love.romulansCured)
		showText(TX_SPEAKER_MCCOY, TX_LOV4_013);
	else {
		_awayMission->love.missionScore += 2;
		_awayMission->love.romulansCured = true;
		_awayMission->disableInput = true;
		walkCrewmanC(OBJECT_MCCOY, 0x10e, 0xb1, &Room::love4MccoyReachedRomulan4);
	}
}

void Room::love4MccoyReachedRomulan4() {
	loadActorAnimC(OBJECT_MCCOY, "museme", -1, -1, &Room::love4MccoyCuredRomulan4);
}

void Room::love4MccoyCuredRomulan4() {
	loadActorAnim2(OBJECT_ROMULAN_4, "s3r5r4", 0x12a, 0xaa);
	walkCrewmanC(OBJECT_MCCOY, 0xeb, 0xc4, &Room::love4MccoyReachedRomulan3);
}

void Room::love4MccoyReachedRomulan3() {
	loadActorAnimC(OBJECT_MCCOY, "musele", -1, -1, &Room::love4MccoyCuredRomulan3);
}

void Room::love4MccoyCuredRomulan3() {
	loadActorAnim2(OBJECT_ROMULAN_3, "s3r5r3", 0xef, 0xc4);
	walkCrewmanC(OBJECT_MCCOY, 0xb9, 0xc3, &Room::love4MccoyReachedRomulan2);
}

void Room::love4MccoyReachedRomulan2() {
	loadActorAnimC(OBJECT_MCCOY, "musemn", -1, -1, &Room::love4MccoyCuredRomulan2);
}

void Room::love4MccoyCuredRomulan2() {
	loadActorAnim2(OBJECT_ROMULAN_2, "s3r5r2", 0xb9, 0xbb);
	walkCrewmanC(OBJECT_MCCOY, 0x36, 0xba, &Room::love4MccoyReachedRomulan1);
}

void Room::love4MccoyReachedRomulan1() {
	loadActorAnimC(OBJECT_MCCOY, "musemn", -1, -1, &Room::love4MccoyCuredRomulan1);
}

void Room::love4MccoyCuredRomulan1() {
	loadActorAnim2(OBJECT_ROMULAN_1, "s3r5r1", 0x36, 0xb3, 0);

	showDescription(TX_LOV4N005);
	if (!_roomVar.love.gaveWaterToRomulans)
		showText(TX_SPEAKER_MCCOY, TX_LOV4_023);

	_awayMission->disableInput = false;
}


void Room::love4UseWaterOnRomulan() {
	// BUGFIX: If the romulans are unconscious, you can't use water on them.
	// In the original, you could use water on them, but there would be no corresponding
	// narration, you'd get no points for it, and you'd lose the water anyway.
	if (!_awayMission->love.romulansCured)
		showText(TX_SPEAKER_MCCOY, TX_LOV4_009);
	else {
		_roomVar.love.gaveWaterToRomulans = true;
		if (_awayMission->love.romulansCured) {
			showDescription(TX_LOV4N013);
			showText(TX_SPEAKER_MCCOY, TX_LOV4_026);
			showText(TX_SPEAKER_KIRK, TX_LOV4_001);
			if (!_awayMission->love.gotPointsForHydratingRomulans) {
				_awayMission->love.gotPointsForHydratingRomulans = true;
				_awayMission->love.missionScore += 2;
			}
		}

		loseItem(OBJECT_IH2O);
	}

	// Note the following unused block of code, an alternative implementation of the
	// function, resembling the version in love5. If they succumbed to the virus from
	// taking too long, McCoy doesn't let you hydrate them, saying it's dangerous.
	// Otherwise, the romulans get up even without receiving the cure. (At least the
	// narration says they do.)
	// These events don't make too much sense, probably cut in the original release, but
	// they did get voice acted anyway.
	if (false) {
		if (_awayMission->love.romulansUnconsciousFromVirus)
			showText(TX_SPEAKER_MCCOY, TX_LOV4_009);
		else {
			showDescription(TX_LOV4N004);
			if (!_awayMission->redshirtDead) {
				showText(TX_SPEAKER_FERRIS, TX_LOV4_029);
				showText(TX_SPEAKER_KIRK,   TX_LOV4_004);
			}
			showText(TX_SPEAKER_MCCOY, TX_LOV4_011);
			loseItem(OBJECT_IH2O);
		}
	}
}

void Room::love4UseMedkitOnRomulan() {
	showText(TX_SPEAKER_MCCOY, TX_LOV4_012);
}

void Room::love4UseCureSampleOnRomulan() {
	showText(TX_SPEAKER_MCCOY, TX_LOV4_014);
}

void Room::love4UseAnythingOnRomulan() {
	showText(TX_SPEAKER_MCCOY, TX_LOV4_017);
}

}
