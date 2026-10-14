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

#define HOTSPOT_EYES     0x20
#define HOTSPOT_MUSHROOM 0x21
#define HOTSPOT_FERN     0x22
#define HOTSPOT_LIGHT_1  0x23
#define HOTSPOT_LIGHT_2  0x24
#define HOTSPOT_LIGHT_3  0x25

namespace StarTrek {

extern const RoomAction feather4ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0},                            &Room::feather4Tick1 },
	{ {ACTION_USE, OBJECT_ICOMM, 0xff, 0, 0},               &Room::feather4UseCommunicator },
	{ {ACTION_USE, OBJECT_IPHASERS, 0xff, 0, 0},            &Room::feather4UsePhaser },
	{ {ACTION_USE, OBJECT_IPHASERK, 0xff, 0, 0},            &Room::feather4UsePhaser },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_LIGHT_1, 0, 0}, &Room::feather4UseSTricorderOnLight },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_LIGHT_2, 0, 0}, &Room::feather4UseSTricorderOnLight },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_LIGHT_3, 0, 0}, &Room::feather4UseSTricorderOnLight },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_MUSHROOM, 0, 0}, &Room::feather4UseSTricorderOnMushroom },
	{ {ACTION_USE, OBJECT_ISTRICOR, HOTSPOT_FERN, 0, 0},    &Room::feather4UseSTricorderOnFern },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0xff, 0, 0},            &Room::feather4UseSTricorderAnywhere },
	{ {ACTION_USE, OBJECT_IMTRICOR, HOTSPOT_LIGHT_1, 0, 0}, &Room::feather4UseMTricorderOnLight },
	{ {ACTION_USE, OBJECT_IMTRICOR, HOTSPOT_LIGHT_2, 0, 0}, &Room::feather4UseMTricorderOnLight },
	{ {ACTION_USE, OBJECT_IMTRICOR, HOTSPOT_LIGHT_3, 0, 0}, &Room::feather4UseMTricorderOnLight },
	{ {ACTION_USE, OBJECT_IMTRICOR, HOTSPOT_MUSHROOM, 0, 0}, &Room::feather4UseMTricorderOnMushroom },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0xff, 0, 0},            &Room::feather4UseMTricorderAnywhere },
	{ {ACTION_USE, OBJECT_IMEDKIT, 0xff, 0, 0},             &Room::feather4UseMedkitAnywhere },
	{ {ACTION_TALK, OBJECT_MCCOY, 0, 0, 0},     &Room::feather4TalkToMccoy },
	{ {ACTION_TALK, OBJECT_SPOCK, 0, 0, 0},     &Room::feather4TalkToSpock },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0},  &Room::feather4TalkToRedshirt },
	{ {ACTION_LOOK, HOTSPOT_EYES, 0, 0, 0},     &Room::feather4LookAtEyes },
	{ {ACTION_LOOK, 0xff, 0, 0, 0},             &Room::feather4LookAnywhere },
	{ {ACTION_LOOK, HOTSPOT_MUSHROOM, 0, 0, 0}, &Room::feather4LookAtMushroom },
	{ {ACTION_LOOK, HOTSPOT_FERN, 0, 0, 0},     &Room::feather4LookAtFern },
	{ {ACTION_LOOK, HOTSPOT_LIGHT_1, 0, 0, 0},  &Room::feather4LookAtLight },
	{ {ACTION_LOOK, HOTSPOT_LIGHT_2, 0, 0, 0},  &Room::feather4LookAtLight },
	{ {ACTION_LOOK, HOTSPOT_LIGHT_3, 0, 0, 0},  &Room::feather4LookAtLight },
	{ {ACTION_LOOK, OBJECT_KIRK, 0, 0, 0},      &Room::feather4LookAtKirk },
	{ {ACTION_LOOK, OBJECT_SPOCK, 0, 0, 0},     &Room::feather4LookAtSpock },
	{ {ACTION_LOOK, OBJECT_MCCOY, 0, 0, 0},     &Room::feather4LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0},  &Room::feather4LookAtRedshirt },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum feather4TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_STRAGEY, TX_SPEAKER_QUETZECOATL,
	TX_FEA4_001, TX_FEA4_002, TX_FEA4_003, TX_FEA4_004, TX_FEA4_006,
	TX_FEA4_007, TX_FEA4_008, TX_FEA4_009, TX_FEA4_010, TX_FEA4_011,
	TX_FEA4_012, TX_FEA4_013, TX_FEA4N000, TX_FEA4N001, TX_FEA4N002,
	TX_FEA4N003, TX_FEA4N004, TX_FEA4N005, TX_FEA4N006, TX_FEA4N007,
	TX_FEA4N008
};

extern const RoomTextOffsets feather4TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 1205, 1164, 1211, 1207, 1166, 1168, 1172 },
	{ TX_SPEAKER_MCCOY, 1228, 1187, 1234, 1230, 1189, 1193, 1195 },
	{ TX_SPEAKER_SPOCK, 1238, 1197, 1244, 1240, 1199, 1203, 1205 },
	{ TX_SPEAKER_STRAGEY, 1248, 1207, 1254, 1250, 1209, 1213, 1215 },
	{ TX_SPEAKER_QUETZECOATL, 1216, 1175, 1222, 1218, 1177, 1181, 1183 },
	{ TX_FEA4_001, 2525, 2216, 2646, 2555, 2219, 2293, 2352 },
	{ TX_FEA4_002, 700, 674, 700, 700, 676, 676, 676 },
	{ TX_FEA4_003, 2674, 2335, 2814, 2711, 2338, 2420, 2488 },
	{ TX_FEA4_004, 1779, 1603, 1865, 1792, 1605, 1619, 1691 },
	{ TX_FEA4_006, 1729, 1568, 1816, 1742, 1570, 1584, 1657 },
	{ TX_FEA4_007, 1945, 1742, 2029, 1977, 1744, 1803, 1840 },
	{ TX_FEA4_008, 1667, 1521, 1751, 1682, 1523, 1539, 1607 },
	{ TX_FEA4_009, 2768, 2413, 2916, 2782, 2417, 2476, 2575 },
	{ TX_FEA4_010, 2458, 2164, 2575, 2487, 2167, 2240, 2296 },
	{ TX_FEA4_011, 1607, 1476, 1687, 1628, 1478, 1500, 1558 },
	{ TX_FEA4_012, 2567, 2243, 2688, 2601, 2246, 2324, 2379 },
	{ TX_FEA4_013, 2298, 2019, 2393, 2337, 2022, 2094, 2129 },
	{ TX_FEA4N000, 1324, 1268, 1334, 1317, 1270, 1265, 1280 },
	{ TX_FEA4N001, 1261, 1220, 1267, 1263, 1222, 1226, 1228 },
	{ TX_FEA4N002, 1370, 1299, 1392, 1368, 1301, 1301, 1323 },
	{ TX_FEA4N003, 1426, 1340, 1466, 1435, 1342, 1352, 1382 },
	{ TX_FEA4N004, 2045, 1826, 2140, 2070, 1829, 1882, 1936 },
	{ TX_FEA4N005, 2235, 1971, 2326, 2264, 1974, 2036, 2077 },
	{ TX_FEA4N006, 2175, 1926, 2262, 2210, 1929, 1997, 2028 },
	{ TX_FEA4N007, 2123, 1889, 2216, 2160, 1892, 1962, 1997 },
	{ TX_FEA4N008, 1576, 1460, 1653, 1593, 1462, 1480, 1539 },
	{          -1, 0,    0, 0, 0, 0, 0, 0 }
};

extern const RoomText feather4Texts[] = {
	{ -1, Common::UNK_LANG, "" }
};

void Room::feather4Tick1() {
	playVoc("FEA4LOOP");
	playMidiMusicTracks(MIDITRACK_27);
}

void Room::feather4UseCommunicator() {
	showText(TX_SPEAKER_MCCOY, TX_FEA4N008);
}

void Room::feather4UsePhaser() {
	showText(TX_SPEAKER_SPOCK, TX_FEA4_011);
}

void Room::feather4UseSTricorderOnLight() {
	spockScan(DIR_W, TX_SPEAKER_SPOCK, TX_FEA4_010);
}

void Room::feather4UseSTricorderOnMushroom() {
	spockScan(DIR_W, TX_SPEAKER_SPOCK, TX_FEA4_012);
}

void Room::feather4UseSTricorderOnFern() {
	spockScan(DIR_W, TX_SPEAKER_SPOCK, TX_FEA4_009);
}

void Room::feather4UseSTricorderAnywhere() {
	spockScan(DIR_W, TX_SPEAKER_SPOCK, TX_FEA4_008);
}

void Room::feather4UseMTricorderOnLight() {
	mccoyScan(DIR_W, TX_SPEAKER_MCCOY, TX_FEA4_001);
}

void Room::feather4UseMTricorderOnMushroom() {
	mccoyScan(DIR_W, TX_SPEAKER_MCCOY, TX_FEA4_003);
}

void Room::feather4UseMTricorderAnywhere() {
	mccoyScan(DIR_W, TX_SPEAKER_MCCOY, TX_FEA4_006);
}

void Room::feather4UseMedkitAnywhere() {
	showText(TX_SPEAKER_MCCOY, TX_FEA4_002);
}

void Room::feather4TalkToMccoy() {
	showText(TX_SPEAKER_MCCOY, TX_FEA4_004);
}

void Room::feather4TalkToSpock() {
	showText(TX_SPEAKER_SPOCK, TX_FEA4_007);
}

void Room::feather4TalkToRedshirt() {
	showText(TX_SPEAKER_STRAGEY, TX_FEA4_013);
}

void Room::feather4LookAtEyes() {
	showDescription(TX_FEA4N003);
}

void Room::feather4LookAnywhere() {
	showDescription(TX_FEA4N008);
}

void Room::feather4LookAtMushroom() {
	showDescription(TX_FEA4N001);
}

void Room::feather4LookAtFern() {
	showDescription(TX_FEA4N000);
}

void Room::feather4LookAtLight() {
	showDescription(TX_FEA4N002);
}

void Room::feather4LookAtKirk() {
	showDescription(TX_FEA4N004);
}

void Room::feather4LookAtSpock() {
	showDescription(TX_FEA4N007);
}

void Room::feather4LookAtMccoy() {
	showDescription(TX_FEA4N006);
}

void Room::feather4LookAtRedshirt() {
	showDescription(TX_FEA4N005);
}

}
