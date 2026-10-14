#include "common/scummsys.h"
#include "common/str.h"

#include "startrek/startrek.h"

namespace StarTrek {

struct TranslatedText {
	const char *english;
	const char *translated;
};

// Texts of the German executable, keyed by those of the English one
static const TranslatedText kGermanTexts[] = {
	{ "Captain's Log",
	  "Logbuch des Captains" },
	{ "Autobeacon",
	  "Funkboje" },
	{ "Voice",
	  "Stimme" },
	{ "#BRID\\B_336#Lowering shields and disarming weapons.",
	  "#BRID\\B_336#Fahre Schilde herunter und entsichere Waffen." },
	{ "#BRID\\B_160#Romulan ship decloaking, Captain.",
	  "#BRID\\B_160#Romulanisches Schiff wird sichtbar, Captain." },
	{ "Romulan Captain",
	  "Romulanischer Captain" },
	{ "#BRID\\B_348#Unable to comply, Captain.  We're under attack.",
	  "#BRID\\B_348#Kann ich nicht ausf\xfchren, Captain.  Wir werden angegriffen." },
	{ "#BRID\\B_338#May I respectfully remind the Captain that we haven't accomplished our mission, Sir.",
	  "#BRID\\B_338#Darf ich unseren Captain respektvoll erinnern, da\xdf wir die Mission nicht beendet haben, Sir." },
	{ "#BRID\\B_349#We must first leave orbit, Sir.",
	  "#BRID\\B_349#Wir m\xfcssen erst den Orbit verlassen, Sir." },
	{ "#BRID\\B_155#Nothing to report, Captain.",
	  "#BRID\\B_155#Keine besonderen Vorkomnisse, Captain." },
	{ "#BRID\\BRIDU016#Hailing frequencies open.",
	  "#BRID\\BRIDU016#Ruf-Frequenzen ge\xf6" "ffnet." },
	{ "#BRID\\B_332#Captain, the shields are up.",
	  "#BRID\\B_332#Captain, die Schutzschilde sind noch aktiv." },
	{ "#BRID\\B_350#We're not in orbit, Captain.",
	  "#BRID\\B_350#Wir sind nicht im Orbit, Captain." },
	{ "#BRID\\B_346#This isn't our destination.",
	  "#BRID\\B_346#Das ist nicht unser Ziel." },
	{ "#BRID\\C_060#Spock, come with me. Mr Scott, you have the conn.",
	  "#BRID\\C_060#Spock, Sie kommen mit. Scott, Sie \xfc" "bernehmen hier." },
	{ "#BRID\\C_006#Assemble a landing party. Unless we find this so-called criminal, we're going to war.",
	  "#BRID\\C_006#Stellen Sie einen Landetrupp zusammen. Solange wir nicht diesen sogenannten Kriminellen finden, besteht das Risiko eines Krieges." },
	{ "Centurion Preax",
	  "Zenturio Preax" },
	{ "#BRID\\B_199#I'll be frank, Kirk. Starfleet expects more of you than that. Try to do better on your next assignment.",
	  "#BRID\\B_199#Ich bin ehrlich, Kirk, Starfleet hat mehr von Ihnen erwartet. Wir erwarten bessere Leistungen bei der n\xe4" "chsten Mission." },
	{ "#BRID\\B_197#A satisfactory performance, Captain, but there's still room for improvement.",
	  "#BRID\\B_197#Eine annehmbare Leistung, Captain, aber Sie k\xf6nnen sich durchaus verbessern." },
	{ "#BRID\\B_214#Well done, Captain. Keep up the good work.",
	  "#BRID\\B_214#Gut gemacht, Captain. Weiter so." },
	{ "#BRID\\B_414#The top brass at Starfleet are impressed. Outstanding work, Jim.",
	  "#BRID\\B_414#Der Oberbefehlsstab von Star Fleet ist beeindruckt. Ganz hervorragende Arbeit, Jim." },
	{ "#BRID\\B_195#A perfect mission, Jim! You are a model for all Starfleet!",
	  "#BRID\\B_195#Einfach perfekt, Jim! Ganz Star Fleet wird davon reden!" },
	{ "#BRID\\B_340#Raising shields.",
	  "#BRID\\B_340#Schutzschilde aktiviert." },
	{ "#BRID\\B_337#Lowering shields, Captain.",
	  "#BRID\\B_337#Schutzschilde deaktiviert." },
	{ "#BRID\\B_351#Arming weapons.",
	  "#BRID\\B_351#Waffen scharf." },
	{ "#BRID\\B_354#Disarming weapons.",
	  "#BRID\\B_354#Waffen gesichert.." },
	{ "#BRID\\BRID_S32#I don't know how long she can take it, Captain.",
	  "#BRID\\BRID_S32#Ich wei\xdf nicht, wie lang das Schiff das noch mitmacht, Captain." },
	{ "#BRID\\BRID_S41#She can't take it, Captain.",
	  "#BRID\\BRID_S41#Das Schiff macht nicht mehr mit, Captain." },
	{ "#BRID\\B_344#Target Analysis On.",
	  "#BRID\\B_344#Zielanalyse Ein." },
	{ "#BRID\\B_343#Target Analysis Off.",
	  "#BRID\\B_343#Zielanalyse Aus." },
	{ "#BRID\\B_333#Entering standard orbit.",
	  "#BRID\\B_333#Sind in Standard-Orbit." },
	{ "#BRID\\B_335#Leaving orbit.",
	  "#BRID\\B_335#Verlassen Orbit." },
	{ "Demon World",
	  "D\xe4monen" },
	{ "Hijacked",
	  "Entf\xfchrt" },
	{ "Love's Labor Jeopardized",
	  "Bakterienkrieg" },
	{ "Another Fine Mess",
	  "Ein sch\xf6ner Mist" },
	{ "Feathered Serpent",
	  "Gefiederte Schlange" },
	{ "That Old Devil Moon",
	  "T\xf6" "dlicher Mond" },
	{ "Vengeance",
	  "Rache" },
	{ "#BRID\\C_007#We have earned ",
	  "#BRID\\C_007#Wir erhielten " },
	{ "for the following recently completed missions:\n\n",
	  "f\xfcr die folgenden, abgeschlossenen Missionen:\n\n" },
	{ "#BRID\\C_007#No recent missions have been completed.",
	  "#BRID\\C_007#In letzter Zeit haben wir keine Mission beendet." },
	{ "Quit Game",
	  "Spiel beenden" },
	{ "#GENE\\GENER028#Yes, quit the game.",
	  "#GENE\\GENER028#Ja, Spiel beenden." },
	{ "#GENE\\GENER008#No, do not quit the game.",
	  "#GENE\\GENER008#Nein, Spiel noch nicht beenden." },
	{ "#GENE\\GENER006#Load a previously saved game.",
	  "#GENE\\GENER006#Spielstand laden." },
	{ "#GENE\\GENER020#Restart the game.",
	  "#GENE\\GENER020#Spiel neu starten." },
	{ "#GENE\\GENER018#Quit the game.",
	  "#GENE\\GENER018#Spiel beenden." },
	{ "Text display",
	  "Textanzeige" },
	{ "Text subtitles.",
	  "Textuntertitel." },
	{ "Display text until you press enter.",
	  "Text anzeigen, bis Enter gedr\xfc" "ckt wird." },
	{ "No text displayed.",
	  "Keinen Text anzeigen." },
	{ "#GENE\\GENE_F14#Turbolift access is blocked by an extremely high radiation level.",
	  "#GENE\\GENE_F14#Der Zugang zum Turbolift ist durch extrem hohe Strahlenwerte blockiert." },
	{ "#GENE\\GENE_F15#This Turbolift cannot reach that area of the ship.",
	  "#GENE\\GENE_F15#Dieser Turbolift erm\xf6glicht keinen Zugang in diesen Schiffsbereich.." },
	{ "#GENE\\SPACE000#The Enterprise has been destroyed!",
	  "#GENE\\SPACE000#Die Enterprise wurde zerst\xf6rt!" },
	{ " choice %d",
	  " Auswahl %d" },
	{ "#BRID\\BRID_S45#The shields are falling!",
	  "#BRID\\BRID_S45#Die Schilde brechen zusammen!" },
	{ "#BRID\\BRID_S37#Our weapons are off-line!",
	  "#BRID\\BRID_S37#Die Waffensysteme steigen aus!" },
	{ "%s repaired, Captain.",
	  "%s repariert, Captain." },
	{ "#BRID\\SCOTSHIE#Shields",
	  "#BRID\\SCOTSHIE#Schilde" },
	{ "#BRID\\SCOTHULL#Hull",
	  "#BRID\\SCOTHULL#Rumpf" },
	{ "#BRID\\SCOTBRID#Bridge",
	  "#BRID\\SCOTBRID#Br\xfc" "cke" },
	{ "#BRID\\SCOTSENS#Sensors",
	  "#BRID\\SCOTSENS#Sensoren" },
	{ "#BRID\\SCOTTORP#Torpedoes",
	  "#BRID\\SCOTTORP#Torpedos" },
	{ "#BRID\\SCOTPHAS#Phasers",
	  "#BRID\\SCOTPHAS#Phaser" },
	{ "#BRID\\SCOTENGI#Engines",
	  "#BRID\\SCOTENGI#Antrieb" },
	{ "Magnification %d",
	  "Vergr\xf6\xdf" "erung %d" },
	{ nullptr, nullptr }
};

const char *StarTrekEngine::localizeText(const char *text) const {
	if (getLanguage() != Common::DE_DEU)
		return text;

	for (const TranslatedText *t = kGermanTexts; t->english; t++) {
		if (!strcmp(t->english, text))
			return t->translated;
	}

	return text;
}

struct TypoFix {
	Common::String origText;
	Common::String newText;
};

Common::String patchRoomMessage(const char *text) {
	Common::String txt = text;
	int i = 0;

	TypoFix typoFixes[] = {
	    { "#LOV2\\LOV2_012#", "#LOV1\\LOV1_010#" }, // LOV2_012: Audio file missing
	    { "#LOV3\\LOV3_#", "#LOV3\\LOV3_000#" },       // LOV3_000: Message index missing
		{ "They are in no condition to talk right now", "#LOV4\\LOV4N007#They are in no condition to talk right now." },	// LOV4N007: add missing audio
		{ "#FEA3\\FEA3_030#", "#LOVA\\LOVA_100#" }, // FEA3_030: Wrong voice actor when McCoy says "He's dead, Jim"
		{ "#FEA4\\TX_FEA4N008", "#FEA4\\FEA4_005" },	// FEA4N008: Fix wrong audio
		{ "#TRI3\\TRI3U080", "#TRI1\\TRI1U080" },	// Fix audio with missing filter
	    { "spock", "Spock" },	// LOVA_F08
	    { "sysnthesize", "synthesize" },	// LOVA_F55
	    { "gullability", "gullibility" },	// MUD0_023
	    { "Well, now! I think", "Well, now I think" },	// MUD2_002
	    { "I don't understand enough of the alien's thinking", "I don't understand enough of how the aliens thought," },	// MUD2_014
	    { "to think after all the stunts that Harry has pulled", "to think that after all the stunts that Harry has pulled," },	// MUD3_011
	    { "and they were certain", "and they are certain" },	// MUD3_022
	    { "DId you know", "Did you know" },	// MUD4_008
	    { "before it retreats Captain", "before it retreats, Captain" },	// FEA1_035
	    { "it must have a nasty bite", "it may have a nasty bite" },	// FEA1_041
	    { "he'll be up in about an hour", "he'll be up in about a half hour" },	// FEA3_012
	    { "sHe's dead, Jim!", "He's dead, Jim!" },	// FEA3_030
	    { "those thorns.You might", "those thorns. You might" },	// FEA5_009
	    { "with our phaser not working", "with our phasers not working" },	// FEA5_018
	    { "in a previous life", "in your previous life" },	// FEA5_020
	    { "isn't that just great", "isn't this just great" },	// FEA6_017
	    { "that action, Captain It may", "that action, Captain. It may" },	// FEA6_019
	    { "that attack you", "that attacked you" },	// FEA6N016
	    { "I'm a surgeon not a alien", "I'm a surgeon, not an alien" },	// SIN2_012
	    { "to bypass it's lock system", "to bypass its lock system" },	// SIN4_023
	    { "Sparks explode and", "Sparks fly and" },	// SIN5N012
	    { "the Enterprise!We've", "the Enterprise! We've" },	// TRI0_036
	    { "Male Human-Vulcan", "One male Human-Vulcan" },	// TRI1_025
	    { "with a phaser", "with your phaser" },	// TRI1_048
	    { "Male Human,", "He's a male Human," },	// TRI2_015
	    { "Male Human-Vulcan", "One male Human-Vulcan" },	// TRI2_017
	    { "He's a Male Human", "One male Human" },	// TRI3_013
	    { "Male Human,", "He's a male Human," },	// TRI3_014
	    { "Male Human-Vulcan", "One male Human-Vulcan" },	// TRI3_016
	    { "Captain, come in please!", "Captain, please come in!" },	// TRI3U084
	    { "I didn't want it", "I don't want it" },	// TRI4_003
	    { "a fair trail", "a fair trial" },	// TRI4_024
	    { "what an enemy does not expect", "what the enemy does not expect" },	// TRI4_039
	    { "will believe you", "to believe you" },	// TRI4_057
	    { "at which to transport you", "to which to transport you" },	// TRI5_045
	    { "a beam light", "a beam of light" },	// TRI5N002
	    { "saphire", "sapphire" },	// TRI5N016, TRI5N017, TRI5N018
	    { "a emerald", "an emerald" },	// TRI5N019, TRI5N020, TRI5N021
	    { "torpedo is loaded", "torpedoes are loaded" },	// VEN2_050
	    { "><upon", "upon" },	// VEN6_005
	    { "Its not", "It's not" },	// VEN8_037
	    { "", "" }
	};

	// Fix typos where some messages contain a hyphen instead of an underscore
	// (e.g in LOV2)
	if (txt[10] == '-')
		txt.replace(10, 1, "_");

	// Fix typos where some messages contain double spacing (e.g. in FEA3_020)
	int32 spacePos = txt.find("  ");
	if (spacePos > 0)
		txt.deleteChar(spacePos);

	// Fix typos
	do {
		const Common::String origText = typoFixes[i].origText;
		const Common::String newText = typoFixes[i].newText;

		int32 pos = txt.find(origText);
		if (pos > 0)
			txt.replace(pos, origText.size(), newText, pos, newText.size());

		i++;
	} while (typoFixes[i].origText != "");

	return txt;
}

} // End of namespace StarTrek
