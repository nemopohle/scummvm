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

#define OBJECT_ENTITY 8
#define OBJECT_MOLTEN_ROCK 9
#define OBJECT_DOOR 10
#define OBJECT_THROWN_IRON_ROD 11

#define HOTSPOT_DOOR 0x20
#define HOTSPOT_LOCK 0x21
#define HOTSPOT_RODS 0x22
#define HOTSPOT_WALL 0x23
#define HOTSPOT_FLOOR 0x24

namespace StarTrek {

extern const RoomAction trial1ActionList[] = {
	{ {ACTION_TICK, 1,  0, 0, 0}, &Room::trial1Tick1 },
	{ {ACTION_TICK, 50, 0, 0, 0}, &Room::trial1Tick50 },
	{ {ACTION_DONE_ANIM, 9, 0, 0, 0}, &Room::trial1DoorOpened },

	{ {ACTION_TOUCHED_HOTSPOT, 0, 0, 0, 0}, &Room::trial1TouchedHotspot0 },
	{ {ACTION_DONE_ANIM, 1, 0, 0, 0},       &Room::trial1KirkDied },

	{ {ACTION_TOUCHED_HOTSPOT, 1, 0, 0, 0}, &Room::trial1TouchedHotspot1 },

	{ {ACTION_GET, HOTSPOT_RODS, 0, 0, 0}, &Room::trial1GetRod },
	{ {ACTION_DONE_WALK, 4, 0, 0, 0},      &Room::trial1ReachedRod },
	{ {ACTION_DONE_ANIM, 5, 0, 0, 0},      &Room::trial1PickedUpRod },

	{ {ACTION_LOOK, OBJECT_KIRK, 0, 0, 0},      &Room::trial1LookAtKirk },
	{ {ACTION_LOOK, OBJECT_SPOCK, 0, 0, 0},     &Room::trial1LookAtSpock },
	{ {ACTION_LOOK, OBJECT_MCCOY, 0, 0, 0},     &Room::trial1LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0},  &Room::trial1LookAtRedshirt },
	{ {ACTION_LOOK, HOTSPOT_WALL, 0, 0, 0},     &Room::trial1LookAtWall },
	{ {ACTION_LOOK, HOTSPOT_FLOOR, 0, 0, 0},     &Room::trial1LookAtFloor },
	{ {ACTION_LOOK, HOTSPOT_RODS, 0, 0, 0},     &Room::trial1LookAtRods },
	{ {ACTION_LOOK, HOTSPOT_DOOR, 0, 0, 0},     &Room::trial1LookAtDoor },
	{ {ACTION_LOOK, HOTSPOT_LOCK, 0, 0, 0},     &Room::trial1LookAtLock },
	{ {ACTION_LOOK, OBJECT_ENTITY, 0, 0, 0},     &Room::trial1LookAtEntity },
	{ {ACTION_LOOK, OBJECT_MOLTEN_ROCK, 0, 0, 0}, &Room::trial1LookAtMoltenRock },
	{ {ACTION_TALK, OBJECT_KIRK, 0, 0, 0},      &Room::trial1TalkToKirk },
	{ {ACTION_TALK, OBJECT_SPOCK, 0, 0, 0},     &Room::trial1TalkToSpock },
	{ {ACTION_TALK, OBJECT_MCCOY, 0, 0, 0},     &Room::trial1TalkToMccoy },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0},  &Room::trial1TalkToRedshirt },
	{ {ACTION_TALK, OBJECT_ENTITY, 0, 0, 0},  &Room::trial1TalkToEntity },

	{ {ACTION_USE, OBJECT_IPHASERS, OBJECT_KIRK,     0, 0}, &Room::trial1UsePhaserOnCrewman },
	{ {ACTION_USE, OBJECT_IPHASERK, OBJECT_KIRK,     0, 0}, &Room::trial1UsePhaserOnCrewman },
	{ {ACTION_USE, OBJECT_IPHASERS, OBJECT_SPOCK,    0, 0}, &Room::trial1UsePhaserOnCrewman },
	{ {ACTION_USE, OBJECT_IPHASERK, OBJECT_SPOCK,    0, 0}, &Room::trial1UsePhaserOnCrewman },
	{ {ACTION_USE, OBJECT_IPHASERS, OBJECT_MCCOY,    0, 0}, &Room::trial1UsePhaserOnCrewman },
	{ {ACTION_USE, OBJECT_IPHASERK, OBJECT_MCCOY,    0, 0}, &Room::trial1UsePhaserOnCrewman },
	{ {ACTION_USE, OBJECT_IPHASERS, OBJECT_REDSHIRT, 0, 0}, &Room::trial1UsePhaserOnCrewman },
	{ {ACTION_USE, OBJECT_IPHASERK, OBJECT_REDSHIRT, 0, 0}, &Room::trial1UsePhaserOnCrewman },

	{ {ACTION_USE, OBJECT_IPHASERS, OBJECT_ENTITY, 0, 0}, &Room::trial1UsePhaserOnEntity },
	{ {ACTION_USE, OBJECT_IPHASERK, OBJECT_ENTITY, 0, 0}, &Room::trial1UsePhaserOnEntity },
	{ {ACTION_USE, OBJECT_IPHASERS, HOTSPOT_WALL, 0, 0}, &Room::trial1UsePhaserOnWall },
	{ {ACTION_USE, OBJECT_IPHASERK, HOTSPOT_WALL, 0, 0}, &Room::trial1UsePhaserOnWall },

	{ {ACTION_USE, OBJECT_IPHASERS, HOTSPOT_FLOOR, 0, 0}, &Room::trial1UseStunPhaserOnFloor },
	{ {ACTION_DONE_WALK, 12, 0, 0, 0}, &Room::trial1ReachedFloorToUseStunPhaser },
	{ {ACTION_DONE_ANIM, 13, 0, 0, 0}, &Room::trial1DoneShootingFloorWithStunPhaser },

	{ {ACTION_USE, OBJECT_IPHASERK, HOTSPOT_FLOOR, 0, 0}, &Room::trial1UseKillPhaserOnFloor },
	{ {ACTION_DONE_WALK, 14, 0, 0, 0}, &Room::trial1ReachedFloorToUseKillPhaser },
	{ {ACTION_DONE_ANIM, 15, 0, 0, 0}, &Room::trial1DoneShootingFloorWithKillPhaser },

	{ {ACTION_USE, OBJECT_IPHASERS, HOTSPOT_RODS, 0, 0}, &Room::trial1UsePhaserOnRod },
	{ {ACTION_USE, OBJECT_IPHASERK, HOTSPOT_RODS, 0, 0}, &Room::trial1UsePhaserOnRod },
	{ {ACTION_USE, OBJECT_IPHASERS, HOTSPOT_DOOR, 0, 0}, &Room::trial1UsePhaserOnDoor },
	{ {ACTION_USE, OBJECT_IPHASERK, HOTSPOT_DOOR, 0, 0}, &Room::trial1UsePhaserOnDoor },
	{ {ACTION_USE, OBJECT_IPHASERS, HOTSPOT_LOCK, 0, 0}, &Room::trial1UsePhaserOnLock },
	{ {ACTION_USE, OBJECT_IPHASERK, HOTSPOT_LOCK, 0, 0}, &Room::trial1UsePhaserOnLock },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_KIRK, 0, 0}, &Room::trial1UseMTricorderOnKirk },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_SPOCK, 0, 0}, &Room::trial1UseMTricorderOnSpock },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_MCCOY, 0, 0}, &Room::trial1UseMTricorderOnMccoy },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_REDSHIRT, 0, 0}, &Room::trial1UseMTricorderOnRedshirt },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_ENTITY, 0, 0}, &Room::trial1UseMTricorderOnEntity },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_WALL, 0, 0}, &Room::trial1UseSTricorderOnWall },
	{ {ACTION_USE, OBJECT_ISTRICOR, OBJECT_ENTITY, 0, 0}, &Room::trial1UseSTricorderOnEntity },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_RODS, 0, 0}, &Room::trial1UseSTricorderOnRods },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_FLOOR, 0, 0}, &Room::trial1UseSTricorderOnFloor },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_DOOR, 0, 0}, &Room::trial1UseSTricorderOnDoor },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_LOCK, 0, 0}, &Room::trial1UseSTricorderOnLock },
	{ {ACTION_USE, OBJECT_ICOMM, 0xff, 0, 0}, &Room::trial1UseCommunicator },
	{ {ACTION_USE, OBJECT_MCCOY, OBJECT_ENTITY, 0, 0}, &Room::trial1UseMccoyOnEntity },
	{ {ACTION_USE, OBJECT_MCCOY, HOTSPOT_LOCK, 0, 0}, &Room::trial1UseMccoyOnLock },
	{ {ACTION_USE, OBJECT_MCCOY, HOTSPOT_FLOOR, 0, 0}, &Room::trial1UseMccoyOnFloor },
	{ {ACTION_USE, OBJECT_SPOCK, OBJECT_ENTITY, 0, 0}, &Room::trial1UseSpockOnEntity },

	{ {ACTION_USE, OBJECT_SPOCK, HOTSPOT_LOCK, 0, 0}, &Room::trial1UseSpockOnLock },
	{ {ACTION_DONE_WALK, 18, 0, 0, 0},                &Room::trial1SpockReachedKeypad },
	{ {ACTION_DONE_ANIM, 19, 0, 0, 0},                &Room::trial1SpockUsedKeypad },
	{ {ACTION_DONE_WALK, 20, 0, 0, 0},                &Room::trial1SpockReachedKeypadWithExtraProgram },
	{ {ACTION_DONE_ANIM, 21, 0, 0, 0},                &Room::trial1SpockUsedKeypadWithExtraProgram },

	{ {ACTION_USE, OBJECT_SPOCK, HOTSPOT_FLOOR, 0, 0},    &Room::trial1UseSpockOnFloor },
	{ {ACTION_USE, OBJECT_REDSHIRT, OBJECT_ENTITY, 0, 0}, &Room::trial1UseRedshirtOnEntity },

	{ {ACTION_USE, OBJECT_REDSHIRT, HOTSPOT_LOCK, 0, 0}, &Room::trial1UseRedshirtOnLock },
	{ {ACTION_DONE_WALK, 24, 0, 0, 0},                   &Room::trial1RedshirtReachedKeypad },
	{ {ACTION_DONE_ANIM, 25, 0, 0, 0},                   &Room::trial1RedshirtUsedKeypad },
	{ {ACTION_DONE_WALK, 26, 0, 0, 0},                   &Room::trial1RedshirtReachedKeypadWithExtraProgram },
	{ {ACTION_DONE_ANIM, 27, 0, 0, 0},                   &Room::trial1RedshirtUsedKeypadWithExtraProgram },

	{ {ACTION_USE, OBJECT_REDSHIRT, HOTSPOT_FLOOR, 0, 0}, &Room::trial1UseRedshirtOnFloor },
	{ {ACTION_USE, OBJECT_IWROD, OBJECT_MCCOY, 0, 0},     &Room::trial1UseRodOnMccoy },
	{ {ACTION_USE, OBJECT_IWROD, OBJECT_SPOCK, 0, 0},     &Room::trial1UseRodOnSpock },
	{ {ACTION_USE, OBJECT_IWROD, OBJECT_REDSHIRT, 0, 0},  &Room::trial1UseRodOnRedshirt },
	{ {ACTION_USE, OBJECT_IWROD, HOTSPOT_FLOOR, 0, 0},    &Room::trial1UseRodOnFloorOrWall },
	{ {ACTION_USE, OBJECT_IWROD, HOTSPOT_WALL, 0, 0},     &Room::trial1UseRodOnFloorOrWall },

	{ {ACTION_USE, OBJECT_IWROD, OBJECT_ENTITY, 0, 0},    &Room::trial1UseWoodRodOnEntity },
	{ {ACTION_DONE_WALK, 10, 0, 0, 0},                    &Room::trial1ReachedPositionToThrowWoodRod },
	{ {ACTION_DONE_ANIM,  6, 0, 0, 0},                    &Room::trial1DoneThrowingWoodRod },

	{ {ACTION_USE, OBJECT_IWROD, HOTSPOT_LOCK, 0, 0},    &Room::trial1UseWoodRodOnLock },

	{ {ACTION_USE, OBJECT_IWROD, OBJECT_MOLTEN_ROCK, 0, 0}, &Room::trial1UseWoodRodOnMoltenRock },
	{ {ACTION_DONE_WALK, 16, 0, 0, 0},                      &Room::trial1ReachedMoltenRock },
	{ {ACTION_DONE_ANIM, 17, 0, 0, 0},                      &Room::trial1DoneCoatingWoodRod },

	{ {ACTION_USE, OBJECT_IIROD, OBJECT_MCCOY, 0, 0},     &Room::trial1UseRodOnMccoy },
	{ {ACTION_USE, OBJECT_IIROD, OBJECT_SPOCK, 0, 0},     &Room::trial1UseRodOnSpock },
	{ {ACTION_USE, OBJECT_IIROD, OBJECT_REDSHIRT, 0, 0},  &Room::trial1UseRodOnRedshirt },
	{ {ACTION_USE, OBJECT_IIROD, HOTSPOT_FLOOR, 0, 0},    &Room::trial1UseRodOnFloorOrWall },
	{ {ACTION_USE, OBJECT_IIROD, HOTSPOT_WALL, 0, 0},     &Room::trial1UseRodOnFloorOrWall },

	{ {ACTION_USE, OBJECT_IIROD, OBJECT_ENTITY, 0, 0},    &Room::trial1UseIronRodOnEntity },
	{ {ACTION_DONE_WALK, 11, 0, 0, 0},                    &Room::trial1ReachedPositionToThrowIronRod },
	{ {ACTION_DONE_ANIM,  7, 0, 0, 0},                    &Room::trial1DoneThrowingIronRod },

	{ {ACTION_USE, OBJECT_IIROD, HOTSPOT_LOCK, 0, 0},   &Room::trial1UseIronRodOnLock },
	{ {ACTION_GET, OBJECT_THROWN_IRON_ROD, 0, 0, 0},    &Room::trial1GetThrownIronRod },
	{ {ACTION_WALK, HOTSPOT_DOOR, 0, 0, 0},             &Room::trial1WalkToDoor },
	{ {ACTION_USE, OBJECT_IMEDKIT, 0xff, 0, 0},         &Room::trial1UseMedkit },

	// ENHANCEMENT: Implement actions for the door object, not just the hotspot
	{ {ACTION_LOOK, OBJECT_DOOR, 0, 0, 0},              &Room::trial1LookAtDoor },
	{ {ACTION_USE, OBJECT_IPHASERS, OBJECT_DOOR, 0, 0}, &Room::trial1UsePhaserOnDoor },
	{ {ACTION_USE, OBJECT_IPHASERK, OBJECT_DOOR, 0, 0}, &Room::trial1UsePhaserOnDoor },
	{ {ACTION_USE, OBJECT_ISTRICOR, OBJECT_DOOR, 0, 0}, &Room::trial1UseSTricorderOnDoor },
	{ {ACTION_WALK, OBJECT_DOOR, 0, 0, 0},              &Room::trial1WalkToDoor },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum trial1TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_UHURA, TX_SPEAKER_BENNIE,
	TX_SPEAKER_ENTITY,
	TX_TRI1_002, TX_TRI1_003, TX_TRI1_004, TX_TRI1_005, TX_TRI1_006,
	TX_TRI1_007, TX_TRI1_008, TX_TRI1_009, TX_TRI1_010, TX_TRI1_011,
	TX_TRI1_012, TX_TRI1_013, TX_TRI1_014, TX_TRI1_015, TX_TRI1_016,
	TX_TRI1_017, TX_TRI1_018, TX_TRI1_019, TX_TRI1_020, TX_TRI1_021,
	TX_TRI1_022, TX_TRI1_023, TX_TRI1_024, TX_TRI1_025, TX_TRI1_026,
	TX_TRI1_027, TX_TRI1_029, TX_TRI1_030, TX_TRI1_031, TX_TRI1_032,
	TX_TRI1_033, TX_TRI1_034, TX_TRI1_035, TX_TRI1_036, TX_TRI1_037,
	TX_TRI1_038, TX_TRI1_039, TX_TRI1_040, TX_TRI1_041, TX_TRI1_042,
	TX_TRI1_043, TX_TRI1_044, TX_TRI1_045, TX_TRI1_046, TX_TRI1_047,
	TX_TRI1_048, TX_TRI1_049, TX_TRI1_050, TX_TRI1_051, TX_TRI1_052,
	TX_TRI1_053, TX_TRI1_054, TX_TRI1_055, TX_TRI1_056, TX_TRI1_057,
	TX_TRI1_J00, TX_TRI1N000, TX_TRI1N001, TX_TRI1N002, TX_TRI1N003,
	TX_TRI1N004, TX_TRI1N005, TX_TRI1N006, TX_TRI1N007, TX_TRI1N008,
	TX_TRI1N009, TX_TRI1N011, TX_TRI1N012, TX_TRI1N013, TX_TRI1N014,
	TX_TRI1N015, TX_TRI1N016, TX_TRI1N017, TX_TRI1U068, TX_TRI1U079,
	TX_TRI1U080, TX_TRI1U086, TX_TRI1U090, TX_TRI1U095, TX_TRI1U098,
	TX_TRI1U101, TX_TRI1U102, TX_TRI1U106, TX_TRI1C001, TX_TRI1C002
};

extern const RoomTextOffsets trial1TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 275, 275, 275, 275, 275, 275, 275, 275, 275 },
	{ TX_SPEAKER_MCCOY, 286, 286, 286, 286, 286, 288, 286, 286, 286 },
	{ TX_SPEAKER_SPOCK, 296, 296, 296, 296, 296, 298, 296, 296, 296 },
	{ TX_SPEAKER_UHURA, 320, 320, 320, 320, 320, 322, 322, 320, 320 },
	{ TX_SPEAKER_BENNIE, 306, 306, 306, 306, 306, 308, 306, 306, 306 },
	{ TX_SPEAKER_ENTITY, 330, 330, 330, 330, 330, 332, 332, 330, 330 },
	{ TX_TRI1_002, 7154, 6368, 7177, 7081, 6389, 6439, 6703, 6389, 7144 },
	{ TX_TRI1_003, 7575, 6746, 7648, 7529, 6768, 6842, 7129, 6768, 7565 },
	{ TX_TRI1_004, 6149, 5493, 6049, 6059, 5513, 5552, 5708, 5513, 6139 },
	{ TX_TRI1_005, 6076, 5435, 5964, 5986, 5455, 5494, 5638, 5455, 6066 },
	{ TX_TRI1_006, 7100, 6324, 7112, 7015, 6345, 6388, 6653, 6345, 7090 },
	{ TX_TRI1_007, 936, 889, 11322, 951, 885, 923, 908, 885, 926 },
	{ TX_TRI1_008, 7619, 6775, 7691, 7569, 6797, 6868, 7157, 6797, 7609 },
	{ TX_TRI1_009, 6209, 5538, 6116, 6115, 5558, 5593, 5760, 5558, 6199 },
	{ TX_TRI1_010, 1276, 1199, 11553, 11239, 1195, 10144, 10390, 1195, 1266 },
	{ TX_TRI1_011, 6761, 6015, 6718, 6664, 6036, 6067, 6289, 6036, 6751 },
	{ TX_TRI1_012, 5106, 4570, 4906, 4991, 4573, 4573, 4666, 4573, 5096 },
	{ TX_TRI1_013, 5610, 5029, 5453, 5522, 5032, 5065, 5168, 5032, 5600 },
	{ TX_TRI1_014, 5737, 5141, 5589, 5646, 5144, 5174, 5289, 5144, 5727 },
	{ TX_TRI1_015, 10856, 9646, 11018, 10936, 9669, 9872, 10116, 9670, 10846 },
	{ TX_TRI1_016, 7808, 6934, 7898, 7762, 6956, 7031, 7334, 6956, 7798 },
	{ TX_TRI1_017, 4855, 4349, 4650, 4737, 4352, 4345, 4440, 4352, 4845 },
	{ TX_TRI1_018, 7936, 7047, 8036, 7903, 7069, 7157, 7457, 7069, 7926 },
	{ TX_TRI1_019, 3040, 2740, 2723, 2864, 2739, 2680, 2704, 2739, 3030 },
	{ TX_TRI1_020, 8025, 7121, 8126, 8008, 7143, 7247, 7532, 7143, 8015 },
	{ TX_TRI1_021, 4261, 3841, 4047, 4130, 3844, 3816, 3912, 3844, 4251 },
	{ TX_TRI1_022, 4596, 4131, 4399, 4481, 4134, 4119, 4219, 4134, 4586 },
	{ TX_TRI1_023, 4723, 4239, 4523, 4611, 4242, 4234, 4219, 4242, 4713 },
	{ TX_TRI1_024, 4387, 3952, 4184, 4264, 3955, 3935, 4034, 3955, 4377 },
	{ TX_TRI1_025, 4457, 4007, 4256, 4331, 4010, 3987, 4091, 4010, 4447 },
	{ TX_TRI1_026, 11067, 9842, 11218, 11144, 9865, 10065, 10301, 9866, 11057 },
	{ TX_TRI1_027, 9814, 8714, 9941, 9805, 8737, 8846, 9151, 8737, 9804 },
	{ TX_TRI1_029, 8075, 7156, 8180, 8061, 7178, 7285, 7571, 7178, 8065 },
	{ TX_TRI1_030, 8140, 7206, 8235, 8119, 7228, 7328, 7611, 7228, 8130 },
	{ TX_TRI1_031, 8211, 7262, 8296, 8183, 7284, 7377, 7657, 7284, 8201 },
	{ TX_TRI1_032, 865, 833, 871, 874, 829, 837, 840, 829, 855 },
	{ TX_TRI1_033, 10751, 9571, 10904, 10803, 9594, 9769, 10033, 9595, 10741 },
	{ TX_TRI1_034, 2991, 2707, 2669, 2808, 2705, 2634, 2665, 2705, 2981 },
	{ TX_TRI1_035, 9017, 7992, 9130, 8980, 8015, 8096, 8415, 8015, 9007 },
	{ TX_TRI1_036, 8488, 7493, 8599, 8462, 7516, 7608, 7914, 7516, 8478 },
	{ TX_TRI1_037, 9853, 8738, 9980, 9846, 8761, 8872, 9175, 8761, 9843 },
	{ TX_TRI1_038, 8412, 7433, 8519, 8393, 7455, 7557, 7850, 7455, 8402 },
	{ TX_TRI1_039, 8292, 7328, 8378, 8266, 7350, 7445, 7724, 7350, 8282 },
	{ TX_TRI1_040, 4132, 3727, 3915, 3996, 3730, 3697, 3795, 3730, 4122 },
	{ TX_TRI1_041, 5347, 4796, 5163, 5253, 4799, 4826, 4908, 4799, 5337 },
	{ TX_TRI1_042, 5488, 4922, 5319, 5388, 4925, 4946, 5049, 4925, 5478 },
	{ TX_TRI1_043, 4978, 4457, 4778, 4858, 4460, 4455, 4553, 4460, 4968 },
	{ TX_TRI1_044, 8651, 7641, 8754, 8612, 7664, 7743, 8054, 7664, 8641 },
	{ TX_TRI1_045, 10642, 9475, 10773, 10658, 9498, 9624, 9917, 9498, 10632 },
	{ TX_TRI1_046, 2905, 2636, 2592, 2707, 2634, 2548, 2603, 2634, 2895 },
	{ TX_TRI1_047, 695, 674, 694, 700, 674, 681, 678, 674, 685 },
	{ TX_TRI1_048, 3398, 3068, 3148, 3240, 3071, 3022, 3103, 3071, 3388 },
	{ TX_TRI1_049, 9712, 8627, 9827, 9707, 8650, 8763, 9052, 8650, 9702 },
	{ TX_TRI1_050, 9100, 8060, 9212, 9070, 8083, 8171, 8482, 8083, 9090 },
	{ TX_TRI1_051, 9218, 8163, 9324, 9202, 8186, 8288, 8579, 8186, 9208 },
	{ TX_TRI1_052, 9892, 8762, 10019, 9887, 8785, 8898, 9199, 8785, 9882 },
	{ TX_TRI1_053, 3193, 2878, 2918, 3021, 2877, 2816, 2884, 2877, 3183 },
	{ TX_TRI1_054, 9318, 8248, 9418, 9316, 8271, 8387, 8658, 8271, 9308 },
	{ TX_TRI1_055, 1502, 1413, 1126, 1279, 1406, 1263, 1284, 1406, 1492 },
	{ TX_TRI1_056, 648, 642, 648, 648, 642, 644, 644, 642, 638 },
	{ TX_TRI1_057, 1140, 1078, 963, 1141, 1074, 1113, 1136, 1074, 1130 },
	{ TX_TRI1_J00, 2812, 0, 2513, 2613, 0, 0, 0, 0, 2802 },
	{ TX_TRI1N000, 2606, 2367, 2302, 2402, 2360, 2238, 2310, 2360, 2596 },
	{ TX_TRI1N001, 2431, 2222, 2101, 2220, 2215, 2086, 2139, 2215, 2421 },
	{ TX_TRI1N002, 2527, 2303, 2212, 2329, 2296, 2180, 2235, 2296, 2517 },
	{ TX_TRI1N003, 1870, 1766, 1502, 1639, 1759, 1611, 1645, 1759, 1860 },
	{ TX_TRI1N004, 10811, 9616, 10971, 10882, 9639, 9833, 10085, 9640, 10801 },
	//{ TX_TRI1N004, 3577, 0, 0, 0 },		// ignore duplicate line
	{ TX_TRI1N005, 10082, 8937, 10207, 10079, 8960, 9075, 10085, 8960, 10072 },
	{ TX_TRI1N006, 2249, 2070, 1903, 2043, 2063, 1941, 1971, 2063, 2239 },
	{ TX_TRI1N007, 2714, 2460, 2418, 2522, 2453, 2343, 2411, 2453, 2704 },
	{ TX_TRI1N008, 3629, 3269, 3399, 3473, 3272, 3225, 3324, 3272, 3619 },
	{ TX_TRI1N009, 2340, 2146, 1995, 2135, 2139, 2016, 2048, 2139, 2330 },
	{ TX_TRI1N011, 2154, 1990, 1800, 1946, 1983, 1859, 1883, 1983, 2144 },
	{ TX_TRI1N012, 4065, 3675, 3845, 3922, 3678, 3638, 3740, 3678, 4055 },
	{ TX_TRI1N013, 10207, 9047, 10334, 10213, 9070, 9192, 9485, 9070, 10197 },
	{ TX_TRI1N014, 3932, 3557, 3698, 3782, 3560, 3519, 3608, 3560, 3922 },
	{ TX_TRI1N015, 2076, 1927, 1710, 1842, 1920, 1770, 1808, 1920, 2066 },
	{ TX_TRI1N016, 2004, 1870, 1639, 1778, 1863, 1721, 1752, 1863, 1994 },
	{ TX_TRI1N017, 1938, 1819, 1571, 1705, 1812, 1662, 1699, 1812, 1928 },
	{ TX_TRI1U068, 6299, 5613, 6205, 6204, 5633, 5667, 5834, 5633, 6289 },
	{ TX_TRI1U079, 7248, 6447, 7270, 7177, 6468, 6520, 6781, 6468, 7238 },
	{ TX_TRI1U080, 5936, 5325, 5815, 5851, 5328, 5371, 5500, 5328, 5926 },
	{ TX_TRI1U086, 6358, 5657, 6268, 6269, 5678, 5717, 5882, 5678, 6348 },
	{ TX_TRI1U090, 6011, 5385, 5887, 5924, 5404, 5447, 5576, 5404, 6001 },
	{ TX_TRI1U095, 7738, 6879, 7823, 7691, 6901, 6975, 7274, 6901, 7728 },
	{ TX_TRI1U098, 6613, 5882, 6519, 6517, 5903, 5935, 6103, 5903, 6603 },
	{ TX_TRI1U101, 6947, 6186, 6933, 6839, 6207, 6227, 6489, 6207, 6937 },
	{ TX_TRI1U102, 7379, 6563, 7429, 7309, 6585, 6637, 6925, 6585, 7369 },
	{ TX_TRI1U106, 6444, 5728, 6361, 6357, 5749, 5790, 5960, 5749, 6434 },
	{          -1, 0,    0, 0, 0, 0, 0, 0, 0, 0 }
};

extern const RoomText trial1Texts[] = {
	{ TX_TRI1C001, Common::EN_ANY, "Zzzt! Sptttz! Zzzt! Tttt!" },
	{ TX_TRI1C002, Common::EN_ANY, "Clunk." },
	{ -1, Common::UNK_LANG, "" }
};

void Room::trial1Tick1() {
	if (!_awayMission->trial.doorOpen) {
		_awayMission->disableInput = 2;
		loadActorAnim2(OBJECT_DOOR, "t1drc");
	}
	if (!_awayMission->trial.entityDefeated) {
		loadActorAnim2(OBJECT_ENTITY, "ec", 0x5f, 0xb1, 3); // No callback defined
		playVoc("TRI1LOOP");
	}
}

void Room::trial1Tick50() {
	if (!_awayMission->trial.doorOpen)
		_awayMission->disableInput = false;
	if (!_awayMission->trial.entityDefeated) {
		showText(TX_SPEAKER_BENNIE, TX_TRI1_056);
		showText(TX_SPEAKER_SPOCK,  TX_TRI1_047);
		showText(TX_SPEAKER_MCCOY,  TX_TRI1_032);
		showText(TX_SPEAKER_KIRK,   TX_TRI1_007);
		showText(TX_SPEAKER_BENNIE, TX_TRI1_057);
		showText(TX_SPEAKER_KIRK,   TX_TRI1_010);
		showText(TX_SPEAKER_BENNIE, TX_TRI1_055);
	}
}

void Room::trial1DoorOpened() { // Called after Spock opens the door
	_awayMission->trial.doorOpen = true;
	loadActorStandAnim(OBJECT_DOOR);
}

void Room::trial1TouchedHotspot0() { // Kirk touched the energy thing
	if (!_awayMission->trial.entityDefeated) {
		// BUGFIX: Disable input, otherwise you can cancel the death
		_awayMission->disableInput = true;
		loadActorAnimC(OBJECT_KIRK, "kkills", -1, -1, &Room::trial1KirkDied);
	}
}

void Room::trial1KirkDied() {
	showGameOverMenu();
}

void Room::trial1TouchedHotspot1() { // Entered the door
	if (_awayMission->trial.doorOpen) {
		_awayMission->disableInput = true;
		loadRoomIndex(2, 2);
	}
}


void Room::trial1GetRod() {
	_awayMission->disableInput = true;
	walkCrewmanC(OBJECT_KIRK, 0xe9, 0xa9, &Room::trial1ReachedRod);
}

void Room::trial1ReachedRod() {
	loadActorAnimC(OBJECT_KIRK, "t1pkup", 0xe9, 0xa9, &Room::trial1PickedUpRod);
	giveItem(OBJECT_IWROD);
	if (!_awayMission->trial.gotPointsForGettingRod) {
		_awayMission->trial.gotPointsForGettingRod = true;
		_awayMission->trial.missionScore += 1;
	}
}

void Room::trial1PickedUpRod() {
	_awayMission->disableInput = false;
}

void Room::trial1LookAtKirk() {
	showDescription(TX_TRI1N003);
}

void Room::trial1LookAtSpock() {
	showDescription(TX_TRI1N017);
}

void Room::trial1LookAtMccoy() {
	showDescription(TX_TRI1N016);
}

void Room::trial1LookAtRedshirt() {
	showDescription(TX_TRI1N015);
}

void Room::trial1LookAtWall() {
	showDescription(TX_TRI1N011);
}

void Room::trial1LookAtFloor() {
	showDescription(TX_TRI1N006);
}

void Room::trial1LookAtRods() {
	showDescription(TX_TRI1N009);
}

void Room::trial1LookAtDoor() {
	showDescription(TX_TRI1N001);
}

void Room::trial1LookAtLock() {
	showDescription(TX_TRI1N002);
}

void Room::trial1LookAtEntity() {
	showDescription(TX_TRI1N000);
}

void Room::trial1LookAtMoltenRock() {
	showDescription(TX_TRI1N007);
}

void Room::trial1TalkToKirk() {
	showText(TX_SPEAKER_KIRK, TX_TRI1_J00);
}

void Room::trial1TalkToSpock() {
	showText(TX_SPEAKER_SPOCK, TX_TRI1_046);
	showText(TX_SPEAKER_MCCOY, TX_TRI1_034);
}

void Room::trial1TalkToMccoy() {
	showText(TX_SPEAKER_MCCOY, TX_TRI1_019);
}

void Room::trial1TalkToRedshirt() {
	showText(TX_SPEAKER_BENNIE, TX_TRI1_053);
}

void Room::trial1TalkToEntity() {
	showText(TX_SPEAKER_ENTITY, TX_TRI1C001);
}

void Room::trial1UsePhaserOnCrewman() {
	showText(TX_SPEAKER_BENNIE, TX_TRI1_048);
}

void Room::trial1UsePhaserOnEntity() {
	showDescription(TX_TRI1N004);
}

void Room::trial1UsePhaserOnWall() {
	showDescription(TX_TRI1N008);
}


void Room::trial1UseStunPhaserOnFloor() {
	_awayMission->disableInput = true;
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_E;
	walkCrewmanC(OBJECT_KIRK, 0xca, 0xbc, &Room::trial1ReachedFloorToUseStunPhaser);
}

void Room::trial1ReachedFloorToUseStunPhaser() {
	playSoundEffectIndex(kSfxPhaser);
	loadActorAnimC(OBJECT_KIRK, "t1mlts", 0xca, 0xbc, &Room::trial1DoneShootingFloorWithStunPhaser);
}

void Room::trial1DoneShootingFloorWithStunPhaser() {
	_awayMission->disableInput = false;
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_E;
	loadActorStandAnim(OBJECT_KIRK);
}


void Room::trial1UseKillPhaserOnFloor() {
	_awayMission->disableInput = true;
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_E;
	walkCrewmanC(OBJECT_KIRK, 0xca, 0xbc, &Room::trial1ReachedFloorToUseKillPhaser);
}

void Room::trial1ReachedFloorToUseKillPhaser() {
	playSoundEffectIndex(kSfxPhaser);
	loadActorAnimC(OBJECT_KIRK, "t1mltk", 0xca, 0xbc, &Room::trial1DoneShootingFloorWithKillPhaser);
}

void Room::trial1DoneShootingFloorWithKillPhaser() {
	showDescription(TX_TRI1N014);
	loadActorAnim2(OBJECT_MOLTEN_ROCK, "t1mltd", 0xca, 0xbc);

	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_E;
	loadActorStandAnim(OBJECT_KIRK);

	// BUG: Infinite score mechanism
	_awayMission->trial.missionScore += 1;
	_awayMission->disableInput = false;
}

void Room::trial1UsePhaserOnRod() {
	showDescription(TX_TRI1N012);
}

void Room::trial1UsePhaserOnDoor() {
	showText(TX_SPEAKER_SPOCK, TX_TRI1_040);
}

void Room::trial1UsePhaserOnLock() {
	showText(TX_SPEAKER_MCCOY, TX_TRI1_021);
}

void Room::trial1UseMTricorderOnKirk() {
	mccoyScan(DIR_S, TX_SPEAKER_MCCOY, TX_TRI1_024, true);
}

void Room::trial1UseMTricorderOnSpock() {
	// ENHANCEMENT: Original didn't play tricorder sound, etc
	mccoyScan(DIR_S, TX_SPEAKER_MCCOY, TX_TRI1_025, true);
}

void Room::trial1UseMTricorderOnMccoy() {
	mccoyScan(DIR_S, TX_SPEAKER_MCCOY, TX_TRI1_022, true);
}

void Room::trial1UseMTricorderOnRedshirt() {
	mccoyScan(DIR_S, TX_SPEAKER_MCCOY, TX_TRI1_023, true);
}

void Room::trial1UseMTricorderOnEntity() {
	mccoyScan(DIR_S, TX_SPEAKER_MCCOY, TX_TRI1_017, true);
}

void Room::trial1UseSTricorderOnWall() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_TRI1_043, true);
}

void Room::trial1UseSTricorderOnEntity() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_TRI1_012, true);

	// BUG: infinite score mechanism
	_awayMission->trial.missionScore += 1;
}

void Room::trial1UseSTricorderOnRods() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_TRI1_041, true);
}

void Room::trial1UseSTricorderOnFloor() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_TRI1_042, true);

	// BUG: infinite score mechanism
	_awayMission->trial.missionScore += 1;
}

void Room::trial1UseSTricorderOnDoor() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_TRI1_013, true);
}

void Room::trial1UseSTricorderOnLock() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_TRI1_014, true);

	if (!_awayMission->trial.scannedLock) {
		_awayMission->trial.scannedLock = true;
		// BUGFIX: Moved this into the if statement (used to be an infinite score mechanism)
		_awayMission->trial.missionScore += 1;
	}
}

void Room::trial1UseCommunicator() {
	if (_awayMission->trial.forceFieldDown) {
		showText(TX_SPEAKER_UHURA, TX_TRI1U090);

		const TextRef choices[] = {
			TX_SPEAKER_KIRK,
			TX_TRI1_005, TX_TRI1_004, TX_TRI1_009,
			TX_END
		};
		int choice = showMultipleTexts(choices);

		if (choice == 0) { // "Beam us back to the enterprise"
			_awayMission->trial.missionEndMethod = 1;
			endMission(1, 1, 1);
		} else if (choice == 1) { // "Beam us to Vlict's position"
			showText(TX_SPEAKER_UHURA, TX_TRI1U080);
			_awayMission->disableInput = true;
			loadRoomIndex(4, 4);
		} // Else don't transport anywhere
	} else { // Force field still up
		if (_awayMission->trial.uhuraAnalyzedCode)
			showText(TX_SPEAKER_UHURA, TX_TRI1U068);
		else {
			showText(TX_SPEAKER_UHURA, TX_TRI1U086);
			showText(TX_SPEAKER_UHURA, TX_TRI1U106);
			showText(TX_SPEAKER_UHURA, TX_TRI1U098);

			if (_awayMission->trial.scannedLock) {
				showText(TX_SPEAKER_KIRK,  TX_TRI1_011);
				showText(TX_SPEAKER_UHURA, TX_TRI1U101);

				const TextRef choices[] = {
					TX_SPEAKER_KIRK,
					TX_TRI1_006, TX_TRI1_002,
					TX_END
				};
				int choice = showMultipleTexts(choices);

				if (choice == 0) { // Don't analyze the anomalous program
					showText(TX_SPEAKER_UHURA, TX_TRI1U079);
					_awayMission->trial.doorCodeBehaviour = 2;
					_awayMission->trial.uhuraAnalyzedCode = true;
					_awayMission->trial.missionScore += 1;
				} else { // Analyze the anomalous program
					showText(TX_SPEAKER_UHURA, TX_TRI1U102);

					const TextRef choices2[] = {
						TX_SPEAKER_KIRK,
						TX_TRI1_003, TX_TRI1_008,
						TX_END
					};
					choice = showMultipleTexts(choices2);

					if (choice == 0) { // Activate the program
						showText(TX_SPEAKER_UHURA, TX_TRI1U095);
						_awayMission->trial.doorCodeBehaviour = 5;
						_awayMission->trial.uhuraAnalyzedCode = true;
						_awayMission->trial.missionScore += 3;
					} else { // Don't activate, only open the door
						showText(TX_SPEAKER_UHURA, TX_TRI1U079);
						_awayMission->trial.doorCodeBehaviour = 2;
						_awayMission->trial.uhuraAnalyzedCode = true;
						_awayMission->trial.missionScore += 1;
					}
				}
			}
		}
	}
}

void Room::trial1UseMccoyOnEntity() {
	showText(TX_SPEAKER_MCCOY, TX_TRI1_016);
}

void Room::trial1UseMccoyOnLock() {
	if (!_awayMission->trial.entityDefeated)
		showText(TX_SPEAKER_MCCOY, TX_TRI1_018);
	else if (_awayMission->trial.doorCodeBehaviour == 0)
		showText(TX_SPEAKER_MCCOY, TX_TRI1_020);
	else
		showText(TX_SPEAKER_MCCOY, TX_TRI1_029);
}

void Room::trial1UseMccoyOnFloor() {
	showText(TX_SPEAKER_MCCOY, TX_TRI1_031);
}

void Room::trial1UseSpockOnEntity() {
	showText(TX_SPEAKER_SPOCK, TX_TRI1_039);
}


void Room::trial1UseSpockOnLock() {
	if (_awayMission->trial.doorOpen)
		showText(TX_SPEAKER_SPOCK, TX_TRI1_038);
	else if (!_awayMission->trial.entityDefeated)
		showText(TX_SPEAKER_SPOCK, TX_TRI1_036);
	else if (_awayMission->trial.doorCodeBehaviour == 0)
		showText(TX_SPEAKER_SPOCK, TX_TRI1_044);
	else if (_awayMission->trial.doorCodeBehaviour == 2)
		walkCrewmanC(OBJECT_SPOCK, 0x1f, 0xb8, &Room::trial1SpockReachedKeypad);
	else
		walkCrewmanC(OBJECT_SPOCK, 0x1f, 0xb8, &Room::trial1SpockReachedKeypadWithExtraProgram);
}

void Room::trial1SpockReachedKeypad() { // Spock opens the door
	playSoundEffectIndex(kSfxButton);
	loadActorAnimC(OBJECT_SPOCK, "susemw", -1, -1, &Room::trial1SpockUsedKeypad);
}

void Room::trial1SpockUsedKeypad() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_W;
	loadActorStandAnim(OBJECT_SPOCK);
	playSoundEffectIndex(kSfxDoor);
	loadActorAnimC(OBJECT_DOOR, "t1drco", -1, -1, &Room::trial1DoorOpened);
	_awayMission->trial.doorOpen = true;
}

void Room::trial1SpockReachedKeypadWithExtraProgram() { // Spock activates the unknown program
	playSoundEffectIndex(kSfxButton);
	loadActorAnimC(OBJECT_SPOCK, "susemw", -1, -1, &Room::trial1SpockUsedKeypadWithExtraProgram);
}

void Room::trial1SpockUsedKeypadWithExtraProgram() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_W;
	loadActorStandAnim(OBJECT_SPOCK);

	_awayMission->disableInput = true;
	loadRoomIndex(5, 5);
}


void Room::trial1UseSpockOnFloor() {
	showText(TX_SPEAKER_SPOCK, TX_TRI1_035);
}

void Room::trial1UseRedshirtOnEntity() {
	showText(TX_SPEAKER_BENNIE, TX_TRI1_050);
}


void Room::trial1UseRedshirtOnLock() {
	if (_awayMission->trial.doorOpen)
		return;
	else if (!_awayMission->trial.entityDefeated)
		showText(TX_SPEAKER_BENNIE, TX_TRI1_051);
	else if (_awayMission->trial.doorCodeBehaviour == 0)
		showText(TX_SPEAKER_BENNIE, TX_TRI1_054);
	else if (_awayMission->trial.doorCodeBehaviour == 2)
		walkCrewmanC(OBJECT_REDSHIRT, 0x1f, 0xb8, &Room::trial1RedshirtReachedKeypad);
	else
		walkCrewmanC(OBJECT_REDSHIRT, 0x1f, 0xb8, &Room::trial1RedshirtReachedKeypadWithExtraProgram);
}

void Room::trial1RedshirtReachedKeypad() { // Redshirt opens the lock
	playSoundEffectIndex(kSfxButton);
	loadActorAnimC(OBJECT_REDSHIRT, "rusemw", -1, -1, &Room::trial1RedshirtUsedKeypad);
}

void Room::trial1RedshirtUsedKeypad() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_REDSHIRT] = DIR_W;
	loadActorStandAnim(OBJECT_REDSHIRT);
	playSoundEffectIndex(kSfxDoor);
	loadActorAnimC(OBJECT_DOOR, "t1drco", -1, -1, &Room::trial1DoorOpened);
	_awayMission->trial.doorOpen = true;
}

void Room::trial1RedshirtReachedKeypadWithExtraProgram() {
	playSoundEffectIndex(kSfxButton);
	loadActorAnimC(OBJECT_REDSHIRT, "rusemw", -1, -1, &Room::trial1RedshirtUsedKeypadWithExtraProgram);
}

void Room::trial1RedshirtUsedKeypadWithExtraProgram() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_REDSHIRT] = DIR_W;
	loadActorStandAnim(OBJECT_REDSHIRT);

	_awayMission->disableInput = true;
	loadRoomIndex(5, 5);
}


void Room::trial1UseRedshirtOnFloor() {
	showText(TX_SPEAKER_BENNIE, TX_TRI1_049);
}

void Room::trial1UseRodOnMccoy() {
	// ENHANCEMENT: This was originally only voiced with the wooden rod, not the iron rod.
	// (This applies to the Spock and Redshirt functions below, too.)
	showText(TX_SPEAKER_MCCOY, TX_TRI1_027);
}

void Room::trial1UseRodOnSpock() {
	showText(TX_SPEAKER_SPOCK, TX_TRI1_037);
}

void Room::trial1UseRodOnRedshirt() {
	showText(TX_SPEAKER_BENNIE, TX_TRI1_052);
}

void Room::trial1UseRodOnFloorOrWall() {
	showDescription(TX_TRI1C002);
}


void Room::trial1UseWoodRodOnEntity() {
	_awayMission->disableInput = true;
	walkCrewmanC(OBJECT_KIRK, 0xe9, 0xad, &Room::trial1ReachedPositionToThrowWoodRod);
	loseItem(OBJECT_IWROD);
}

void Room::trial1ReachedPositionToThrowWoodRod() {
	loadActorAnimC(OBJECT_KIRK, "kthrwr", 0xe9, 0xad, &Room::trial1DoneThrowingWoodRod);
}

void Room::trial1DoneThrowingWoodRod() {
	_awayMission->disableInput = false;
}


void Room::trial1UseWoodRodOnLock() {
	showDescription(TX_TRI1N005);
}


void Room::trial1UseWoodRodOnMoltenRock() {
	_awayMission->disableInput = true;
	walkCrewmanC(OBJECT_KIRK, 0xe4, 0xc4, &Room::trial1ReachedMoltenRock);
}

void Room::trial1ReachedMoltenRock() {
	loadActorAnimC(OBJECT_KIRK, "t1dip", 0xe4, 0xc4, &Room::trial1DoneCoatingWoodRod);
}

void Room::trial1DoneCoatingWoodRod() {
	_awayMission->disableInput = false;
	showDescription(TX_TRI1N013);
	loseItem(OBJECT_IWROD);
	giveItem(OBJECT_IIROD);

	if (!_awayMission->trial.gotPointsForCoatingRodWithIron) {
		_awayMission->trial.gotPointsForCoatingRodWithIron = true;
		_awayMission->trial.missionScore += 1;
	}
}


void Room::trial1UseIronRodOnEntity() {
	_awayMission->disableInput = true;
	walkCrewmanC(OBJECT_KIRK, 0xe9, 0xac, &Room::trial1ReachedPositionToThrowIronRod);
	_awayMission->trial.entityDefeated = true;
	loseItem(OBJECT_IIROD);
}

void Room::trial1ReachedPositionToThrowIronRod() {
	loadActorAnimC(OBJECT_KIRK, "kthri", 0xe9, 0xac, &Room::trial1DoneThrowingIronRod);
	loadActorStandAnim(OBJECT_ENTITY);
}

void Room::trial1DoneThrowingIronRod() {
	loadActorAnim(OBJECT_THROWN_IRON_ROD, "t1irod", 0xe9, 0xac, 8); // No callback defined

	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_W;
	loadActorStandAnim(OBJECT_KIRK);

	_awayMission->disableInput = false;
	stopAllVocSounds();
	showText(TX_SPEAKER_SPOCK, TX_TRI1_045);
	showText(TX_SPEAKER_MCCOY, TX_TRI1_033);

	_awayMission->trial.missionScore += 1;
}


void Room::trial1UseIronRodOnLock() {
	showDescription(TX_TRI1N004);
}

void Room::trial1GetThrownIronRod() {
	showText(TX_SPEAKER_SPOCK, TX_TRI1_015);
}

void Room::trial1WalkToDoor() {
	if (_awayMission->trial.doorOpen)
		walkCrewman(OBJECT_KIRK, 0x1a, 0xac, 28);
}

void Room::trial1UseMedkit() {
	showText(TX_SPEAKER_MCCOY, TX_TRI1_026);
}

}
