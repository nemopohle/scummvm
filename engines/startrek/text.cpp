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
	{ "#BRID\\B_348#Unable to comply, Captain. We're under attack.",
	  "#BRID\\B_348#Kann ich nicht ausf\xfchren, Captain. Wir werden angegriffen." },
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

static const TranslatedText kFrenchTexts[] = {
	{ "Captain's Log",
	  "Journal du Capitaine" },
	{ "Elasi Captain",
	  "Elasi Capitaine" },
	{ "#BRID\\B_348#Unable to comply, Captain.  We're under attack.",
	  "#BRID\\B_348#Impossible, Capitaine. Nous sommes attaqu\xe9s." },
	{ "#BRID\\B_348#Unable to comply, Captain. We're under attack.",
	  "#BRID\\B_348#Impossible, Capitaine. Nous sommes attaqu\xe9s." },
	{ "#BRID\\B_338#May I respectfully remind the Captain that we haven't accomplished our mission, Sir.",
	  "#BRID\\B_338#Avec tout le respect que je vous dois Capitaine, nous n\x92" "avons pas accompli notre mission." },
	{ "#BRID\\B_349#We must first leave orbit, Sir.",
	  "#BRID\\B_349#Nous devons quitter l\x92orbite, mon Capitaine." },
	{ "#BRID\\B_155#Nothing to report, Captain.",
	  "#BRID\\B_155#Rien \xe0 signaler, Capitaine." },
	{ "#BRID\\BRIDU016#Hailing frequencies open.",
	  "#BRID\\BRIDU016#Fr\xe9quences d\x92" "appel ouvertes." },
	{ "#BRID\\B_332#Captain, the shields are up.",
	  "#BRID\\B_332#Capitaine, les boucliers sont sortis." },
	{ "#BRID\\B_350#We're not in orbit, Captain.",
	  "#BRID\\B_350#Nous ne sommes pas en orbite, Capitaine." },
	{ "#BRID\\B_346#This isn't our destination.",
	  "#BRID\\B_346#Ce n\x92" "est pas notre destination." },
	{ "#BRID\\C_060#Spock, come with me. Mr Scott, you have the conn.",
	  "#BRID\\C_060#Spock, venez avec moi. M. Scott, vous avez le contr\xf4le." },
	{ "#BRID\\C_006#Assemble a landing party. Unless we find this so-called criminal, we're going to war.",
	  "#BRID\\C_006#Rassemblez un d\xe9tachement de d\xe9" "barquement. Si l\x92on ne trouve pas ce fameux criminel, ce sera la guerre." },
	{ "#BRID\\B_199#I'll be frank, Kirk. Starfleet expects more of you than that. Try to do better on your next assignment.",
	  "#BRID\\B_199#Je serai franc, Kirk. Starfleet n\x92" "est pas satisfaite de vous. Essayez de faire mieux au cours de votre prochaine mission." },
	{ "#BRID\\B_197#A satisfactory performance, Captain, but there's still room for improvement.",
	  "#BRID\\B_197#Des r\xe9sultats satisfaisants, Capitaine, mais vous pouvez encore faire mieux." },
	{ "#BRID\\B_214#Well done, Captain. Keep up the good work.",
	  "#BRID\\B_214#Bravo, Capitaine. Continuez comme \xe7" "a." },
	{ "#BRID\\B_414#The top brass at Starfleet are impressed. Outstanding work, Jim.",
	  "#BRID\\B_414#Le haut commandement de Starfleet est satisfait. Excellent travail, Jim." },
	{ "#BRID\\B_195#A perfect mission, Jim! You are a model for all Starfleet!",
	  "#BRID\\B_195#Une mission parfaite, Jim! Vous \xeates un v\xe9ritable exemple pour Starfleet." },
	{ "#BRID\\B_340#Raising shields.",
	  "#BRID\\B_340#Boucliers sortis." },
	{ "#BRID\\B_337#Lowering shields, Captain.",
	  "#BRID\\B_337#Boucliers rentr\xe9s." },
	{ "#BRID\\B_351#Arming weapons.",
	  "#BRID\\B_351#Armer les armes." },
	{ "#BRID\\B_354#Disarming weapons.",
	  "#BRID\\B_354#D\xe9sarmer les armes." },
	{ "#BRID\\BRID_S32#I don't know how long she can take it, Captain.",
	  "#BRID\\BRID_S32#Je ne sais pas combien de temps elle va tenir, Capitaine." },
	{ "#BRID\\BRID_S41#She can't take it, Captain.",
	  "#BRID\\BRID_S41#Elle ne r\xe9sistera pas, Capitaine." },
	{ "#BRID\\B_344#Target Analysis On.",
	  "#BRID\\B_344#Analyse de Cible Activ\xe9" "e." },
	{ "#BRID\\B_343#Target Analysis Off.",
	  "#BRID\\B_343#Analyse de Cible D\xe9sactiv\xe9" "e." },
	{ "#BRID\\BRID_S45#The shields are falling!",
	  "#BRID\\BRID_S45#Les boucliers descendent!" },
	{ "#BRID\\BRID_S37#Our weapons are off-line!",
	  "#BRID\\BRID_S37#Nos armes sont d\xe9" "connect\xe9" "es!" },
	{ "#BRID\\SCOTSHIE#Shields",
	  "#BRID\\SCOTSHIE#Boucliers" },
	{ "#BRID\\SCOTHULL#Hull",
	  "#BRID\\SCOTHULL#Coque" },
	{ "#BRID\\SCOTBRID#Bridge",
	  "#BRID\\SCOTBRID#Passerelle" },
	{ "#BRID\\SCOTSENS#Sensors",
	  "#BRID\\SCOTSENS#Capteurs" },
	{ "#BRID\\SCOTTORP#Torpedoes",
	  "#BRID\\SCOTTORP#Torpilles" },
	{ "#BRID\\SCOTPHAS#Phasers",
	  "#BRID\\SCOTPHAS#Phaseurs" },
	{ "#BRID\\SCOTENGI#Engines",
	  "#BRID\\SCOTENGI#Moteur" },
	{ "%s repaired, Captain.",
	  "%s c\x92" "est r\xe9par\xe9, Capitaine." },
	{ "#BRID\\B_333#Entering standard orbit.",
	  "#BRID\\B_333#Entr\xe9" "e sur orbite standard." },
	{ "#BRID\\B_335#Leaving orbit.",
	  "#BRID\\B_335#D\xe9part d\x92orbite." },
	{ "#BRID\\C_007#We have earned ",
	  "#BRID\\C_007#Nous avons gagn\xe9 " },
	{ "for the following recently completed missions:\n\n",
	  "pour les missions suivantes, accomplies r\xe9" "cemment :\n\n" },
	{ "#BRID\\C_007#No recent missions have been completed.",
	  "#BRID\\C_007#Aucune mission r\xe9" "cente n\x92" "a \xe9t\xe9 accomplie." },
	{ "#BRID\\B_336#Lowering shields and disarming weapons.",
	  "#BRID\\B_336#Boucliers rentr\xe9s et armes d\xe9sarm\xe9" "es." },
	{ "#BRID\\B_160#Romulan ship decloaking, Captain.",
	  "#BRID\\B_160#Un vaisseau Romulan appara\xeet, Capitaine." },
	{ "Quit Game",
	  "Quitter jeu" },
	{ "#GENE\\GENER028#Yes, quit the game.",
	  "#GENE\\GENER028#Oui, quitter le jeu." },
	{ "#GENE\\GENER008#No, do not quit the game.",
	  "#GENE\\GENER008#Non, ne pas quitter le jeu." },
	{ "Game Over",
	  "Jeu termin\xe9" },
	{ "#GENE\\GENER006#Load a previously saved game.",
	  "#GENE\\GENER006#Charger un jeu sauvegard\xe9 au pr\xe9" "alable." },
	{ "#GENE\\GENER020#Restart the game.",
	  "#GENE\\GENER020#Recommencer le jeu." },
	{ "#GENE\\GENER018#Quit the game.",
	  "#GENE\\GENER018#Quitter le jeu." },
	{ "Text display",
	  "Affichage texte" },
	{ "Text subtitles.",
	  "Soutitrages texte." },
	{ "Display text until you press enter.",
	  "Appuyez sur Entr\xe9" "e pour arr\xeater affichage." },
	{ "No text displayed.",
	  "Pas de texte affich\xe9." },
	{ "#GENE\\GENE_F14#Turbolift access is blocked by an extremely high radiation level.",
	  "#GENE\\GENE_F14#Un niveau de radiation extr\xeament \xe9lev\xe9 bloque l'acc\xe8s \xe0 ce turbotransporteur." },
	{ "#GENE\\GENE_F15#This Turbolift cannot reach that area of the ship.",
	  "#GENE\\GENE_F15#Ce trubo-transporteur ne peut pas atteindre cette partie du vaisseau." },
	{ "#GENE\\SPACE000#The Enterprise has been destroyed!",
	  "#GENE\\SPACE000#L\x92" "Enterprise a \xe9t\xe9 d\xe9truit!" },
	{ nullptr, nullptr }
};

// The Amiga versions use Mac Roman accents
static const TranslatedText kAmigaGermanTexts[] = {
	{ "findObject: Clicked on an unknown sprite",
	  "FindObject: clicked on unknown sprite." },
	{ "Capt. Kirk",
	  "Kapit\x8an Kirk" },
	{ "Lieutenant Uhura",
	  "Leutnant Uhura" },
	{ "Elasi Captain",
	  "Elasi Kapit\x8an" },
	{ "Autobeacon",
	  "Funkboje" },
	{ "Voice",
	  "Stimme" },
	{ "#BRID\\B_348#Unable to comply, Captain.  We're under attack.",
	  "Kann ich nicht ausf\x9fhren, Kapit\x8an.  Wir werden angegriffen." },
	{ "#BRID\\B_348#Unable to comply, Captain. We're under attack.",
	  "Kann ich nicht ausf\x9fhren, Kapit\x8an. Wir werden angegriffen." },
	{ "#BRID\\B_338#May I respectfully remind the Captain that we haven't accomplished our mission, Sir.",
	  "Darf ich unseren Kapit\x8an respektvoll daran erinnern, da\xa7 wir die Mission nicht beendet haben, Sir." },
	{ "#BRID\\B_349#We must first leave orbit, Sir.",
	  "Wir m\x9fssen erst den Orbit verlassen, Sir." },
	{ "#BRID\\B_155#Nothing to report, Captain.",
	  "Keine besonderen Vorkommisse, Kapit\x8an." },
	{ "#BRID\\BRIDU016#Hailing frequencies open.",
	  "Ruf-Frequenzen ge\x9a" "ffnet." },
	{ "Captain, the shield's are up.",
	  "Kapit\x8an, die Schutzschilde sind noch aktiv." },
	{ "#BRID\\B_332#Captain, the shields are up.",
	  "Kapit\x8an, die Schutzschilde sind noch aktiv." },
	{ "We're not in orbit Captain.",
	  "Wir sind nicht im Umlaufbahn, Kapit\x8an." },
	{ "#BRID\\B_350#We're not in orbit, Captain.",
	  "Wir sind nicht im Umlaufbahn, Kapit\x8an." },
	{ "#BRID\\B_346#This isn't our destination.",
	  "Das ist nicht unser Ziel." },
	{ "Spock, come with me. Mr Scott, you have the con.",
	  "Spock, Sie kommen mit. Scott, Sie \x9f" "bernehmen hier." },
	{ "#BRID\\C_060#Spock, come with me. Mr Scott, you have the conn.",
	  "Spock, Sie kommen mit. Scott, Sie \x9f" "bernehmen hier." },
	{ "#BRID\\C_006#Assemble a landing party. Unless we find this so-called criminal, we're going to war.",
	  "Stellen Sie einen Landetrupp zusammen. Solange wir nicht diesen sogenannten Kriminellen finden, besteht das Risiko eines Krieges." },
	{ "#BRID\\B_199#I'll be frank, Kirk. Starfleet expects more of you than that. Try to do better on your next assignment.",
	  "Um ehrlich su sein, Kirk, die Sternenflotte hat mehr von Ihnen erwartet. Wir erwarten bessere Leistungen bei der n\x8a" "chsten Mission." },
	{ "#BRID\\B_197#A satisfactory performance, Captain, but there's still room for improvement.",
	  "Eine annehmbare Leistung, Kapit\x8an, aber Sie k\x9annten sich durchaus noch verbessern." },
	{ "#BRID\\B_214#Well done, Captain. Keep up the good work.",
	  "Gut gemacht, Kapit\x8an. Weiter so." },
	{ "#BRID\\B_414#The top brass at Starfleet are impressed. Outstanding work, Jim.",
	  "Der Oberbefehlsstab der Sternenflotte ist beeindruckt. Ganz hervorragende Arbeit, Jim." },
	{ "#BRID\\B_195#A perfect mission, Jim! You are a model for all Starfleet!",
	  "Einfach perfekt, Jim! Die ganze Sternenflotte wird davon reden!" },
	{ "I'll be frank, Kirk, Star Fleet expects more out of you than that. Try to do better on your next assignment.",
	  "Um ehrlich su sein, Kirk, die Sternenflotte hat mehr von Ihnen erwartet. Wir erwarten bessere Leistungen bei der n\x8a" "chsten Mission." },
	{ "A satisfactory performance, Captain, but there's still room for improvement.",
	  "Eine annehmbare Leistung, Kapit\x8an, aber Sie k\x9annten sich durchaus noch verbessern." },
	{ "Well done, Captain. Keep up the good work.",
	  "Gut gemacht, Kapit\x8an. Weiter so." },
	{ "The top brass at Star Fleet are impressed. Outstanding work, Jim.",
	  "Der Oberbefehlsstab der Sternenflotte ist beeindruckt. Ganz hervorragende Arbeit, Jim." },
	{ "A perfect mission, Jim! You are a model for all Star Fleet",
	  "Einfach perfekt, Jim! Die ganze Sternenflotte wird davon reden!" },
	{ "Magnification %d",
	  "Vergr\x9a\xa7" "erung %d" },
	{ "#BRID\\B_340#Raising shields.",
	  "Schutzschilde aktiviert." },
	{ "Lowering shields.",
	  "Schutzschilde deaktiviert." },
	{ "#BRID\\B_351#Arming weapons.",
	  "Waffen aktiviert." },
	{ "#BRID\\B_354#Disarming weapons.",
	  "Waffen gesichert." },
	{ "I don't know how long she can take it Captain.",
	  "Ich wei\xa7 nicht, wie lang das Schiff das noch mitmacht, Kapit\x8an." },
	{ "#BRID\\BRID_S32#I don't know how long she can take it, Captain.",
	  "Ich wei\xa7 nicht, wie lang das Schiff das noch mitmacht, Kapit\x8an." },
	{ "She can't take it Captain.",
	  "Das Schiff macht nicht mehr mit, Kapit\x8an." },
	{ "#BRID\\BRID_S41#She can't take it, Captain.",
	  "Das Schiff macht nicht mehr mit, Kapit\x8an." },
	{ "#BRID\\B_344#Target Analysis On.",
	  "Zielanalyse Ein." },
	{ "#BRID\\B_343#Target Analysis Off.",
	  "Zielanalyse Aus." },
	{ "#BRID\\BRID_S22#Aye Sir.",
	  "Ja Sir." },
	{ "#BRID\\BRID_S45#The shields are falling!",
	  "Die Schilde brechen zusammen!" },
	{ "#BRID\\BRID_S37#Our weapons are off-line!",
	  "Die Waffensysteme fallen aus!" },
	{ "#BRID\\SCOTSHIE#Shields",
	  "Schilde" },
	{ "#BRID\\SCOTHULL#Hull",
	  "Rumpf" },
	{ "#BRID\\SCOTBRID#Bridge",
	  "Br\x9f" "cke" },
	{ "#BRID\\SCOTSENS#Sensors",
	  "Sensoren" },
	{ "#BRID\\SCOTTORP#Torpedoes",
	  "Torpedos" },
	{ "#BRID\\SCOTPHAS#Phasers",
	  "Phaser" },
	{ "#BRID\\SCOTENGI#Engines",
	  "Antrieb" },
	{ "%s repaired, Captain.",
	  "%s repariert, Kapit\x8an." },
	{ "Captain",
	  "Kapit\x8an" },
	{ "#BRID\\B_333#Entering standard orbit.",
	  "Sind im Standard-Umlaufbahn." },
	{ "#BRID\\B_335#Leaving orbit.",
	  "Verlassen Umlaufbahn." },
	{ "Demon World",
	  "D\x8amonwelt" },
	{ "Hijacked",
	  "Entf\x9fhrt" },
	{ "Love's Labor Jeopardized",
	  "Der Liebe Arbeit in Gefahr" },
	{ "Another Fine Mess",
	  "Ein sch\x9aner Mist" },
	{ "Feathered Serpent",
	  "Versuch und Irrt\x86mer" },
	{ "That Old Devil Moon",
	  "T\x9a" "dlicher Mond" },
	{ "Vengeance",
	  "Rache" },
	{ "#BRID\\C_007#We have earned ",
	  "Wir erhielten " },
	{ "1 point ",
	  "1 Punkt " },
	{ "%d points ",
	  "%d Punkte " },
	{ "for the following recently completed missions:\n\n",
	  "f\x9fr die folgenden, abgeschlossenen Missionen:\n\n" },
	{ ":  1 point.\n\n",
	  ":  1 Punkt.\n\n" },
	{ ":  %d points.\n\n",
	  ":  %d Punkte.\n\n" },
	{ "#BRID\\C_007#No recent missions have been completed.",
	  "In letzter Zeit haben wir keine Mission beendet." },
	{ "#BRID\\B_336#Lowering shields and disarming weapons.",
	  "Fahre Schilde herunter und entsichere Waffen." },
	{ "Priest",
	  "Priester" },
	{ "#BRID\\B_160#Romulan ship decloaking, Captain.",
	  "Romulanisches Schiff enttarnt sich, Kapit\x8an." },
	{ "Romulan Captain",
	  "Romulanischer Kapit\x8an" },
	{ "Centurion Preax",
	  "Zenturio Preax" },
	{ "Quit Game",
	  "Spiel beenden" },
	{ "#GENE\\GENER028#Yes, quit the game.",
	  "Ja, Spiel beenden." },
	{ "#GENE\\GENER008#No, do not quit the game.",
	  "Nein, Spiel noch nicht beenden." },
	{ "Game Over",
	  "Spiel ist vorbei" },
	{ "#GENE\\GENER006#Load a previously saved game.",
	  "Spielstand laden." },
	{ "#GENE\\GENER020#Restart the game.",
	  "Spiel neu starten." },
	{ "#GENE\\GENER018#Quit the game.",
	  "Spiel beenden." },
	{ "Ensign",
	  "Kadett" },
	{ "#GENE\\SPACE000#The Enterprise has been destroyed!",
	  "Die Enterprise wurde zerst\x9art!" },
	{ " choice %d",
	  " auswahl %d" },
	{ nullptr, nullptr }
};

static const TranslatedText kAmigaFrenchTexts[] = {
	{ "findObject: Clicked on an unknown sprite",
	  "FindObject: clicked on unknown sprite." },
	{ "Mr. Spock",
	  "M. Spock" },
	{ "Mr. Sulu",
	  "M. Sulu" },
	{ "Mr. Chekov",
	  "M. Chekov" },
	{ "Mr. Scott",
	  "M. Scott" },
	{ "Captain's Log",
	  "Journal de bord du Capitaine" },
	{ "Elasi Captain",
	  "Elasi Capitaine" },
	{ "Admiral Kenka",
	  "Amiral Kenka" },
	{ "#BRID\\B_348#Unable to comply, Captain. We're under attack.",
	  "Impossible, Capitaine. Nous sommes attaqu\x8es." },
	{ "#BRID\\B_338#May I respectfully remind the Captain that we haven't accomplished our mission, Sir.",
	  "Avec tout le respect que je vous dois Capitaine, nous n\xd5" "avons pas accompli notre mission." },
	{ "#BRID\\B_349#We must first leave orbit, Sir.",
	  "Nous devons quitter l\xd5orbite, mon Capitaine." },
	{ "#BRID\\B_155#Nothing to report, Captain.",
	  "Rien \x88 signaler, Capitaine." },
	{ "#BRID\\BRIDU016#Hailing frequencies open.",
	  "Fr\x8equences d\xd5" "appel ouvertes." },
	{ "Captain, the shield's are up.",
	  "Capitaine, les boucliers sont sortis." },
	{ "#BRID\\B_332#Captain, the shields are up.",
	  "Capitaine, les boucliers sont sortis." },
	{ "We're not in orbit Captain.",
	  "Nous ne sommes pas en orbite, Capitaine." },
	{ "#BRID\\B_350#We're not in orbit, Captain.",
	  "Nous ne sommes pas en orbite, Capitaine." },
	{ "#BRID\\B_346#This isn't our destination.",
	  "Ce n\xd5" "est pas notre destination." },
	{ "Spock, come with me. Mr Scott, you have the con.",
	  "Spock, venez avec moi. M. Scott, vous avez le contr\x99le." },
	{ "#BRID\\C_060#Spock, come with me. Mr Scott, you have the conn.",
	  "Spock, venez avec moi. M. Scott, vous avez le contr\x99le." },
	{ "#BRID\\C_006#Assemble a landing party. Unless we find this so-called criminal, we're going to war.",
	  "Rassemblez un d\x8etachement de d\x8e" "barquement. Si l\xd5on ne trouve pas ce fameux criminel, ce sera la guerre." },
	{ "#BRID\\B_199#I'll be frank, Kirk. Starfleet expects more of you than that. Try to do better on your next assignment.",
	  "Je serai franc, Kirk. Starfleet n\xd5" "est pas satisfaite de vous. Essayez de faire mieux au cours de votre prochaine mission." },
	{ "#BRID\\B_197#A satisfactory performance, Captain, but there's still room for improvement.",
	  "Des r\x8esultats satisfaisants, Capitaine, mais vous pouvez encore faire mieux." },
	{ "#BRID\\B_214#Well done, Captain. Keep up the good work.",
	  "Bravo, Capitaine. Continuez comme \x8d" "a. Le haut commandement de Starfleet est satisfait. Excellent travail, Jim." },
	{ "#BRID\\B_414#The top brass at Starfleet are impressed. Outstanding work, Jim.",
	  "Tu as impressionn\x8e en haut-lier \x88 Starfleet. Beau travail, Jim." },
	{ "#BRID\\B_195#A perfect mission, Jim! You are a model for all Starfleet!",
	  "Une mission parfaite, Jim! Vous \x90tes un v\x8eritable exemple pour Starfleet." },
	{ "I'll be frank, Kirk, Star Fleet expects more out of you than that. Try to do better on your next assignment.",
	  "Je serai franc, Kirk. Starfleet n\xd5" "est pas satisfaite de vous. Essayez de faire mieux au cours de votre prochaine mission." },
	{ "A satisfactory performance, Captain, but there's still room for improvement.",
	  "Des r\x8esultats satisfaisants, Capitaine, mais vous pouvez encore faire mieux." },
	{ "Well done, Captain. Keep up the good work.",
	  "Bravo, Capitaine. Continuez comme \x8d" "a. Le haut commandement de Starfleet est satisfait. Excellent travail, Jim." },
	{ "The top brass at Star Fleet are impressed. Outstanding work, Jim.",
	  "Tu as impressionn\x8e en haut-lier \x88 Starfleet. Beau travail, Jim." },
	{ "A perfect mission, Jim! You are a model for all Star Fleet",
	  "Une mission parfaite, Jim! Vous \x90tes un v\x8eritable exemple pour Starfleet." },
	{ "Admiral",
	  "Amiral" },
	{ "#BRID\\B_340#Raising shields.",
	  "Boucliers sortis." },
	{ "Lowering shields.",
	  "Boucliers rentr\x8es." },
	{ "#BRID\\B_351#Arming weapons.",
	  "Armer les armes." },
	{ "#BRID\\B_354#Disarming weapons.",
	  "D\x8esarmer les armes." },
	{ "I don't know how long she can take it Captain.",
	  "Je ne sais pas combien de temps elle va tenir, Capitaine." },
	{ "#BRID\\BRID_S32#I don't know how long she can take it, Captain.",
	  "Je ne sais pas combien de temps elle va tenir, Capitaine." },
	{ "She can't take it Captain.",
	  "Elle ne r\x8esistera pas, Capitaine." },
	{ "#BRID\\BRID_S41#She can't take it, Captain.",
	  "Elle ne r\x8esistera pas, Capitaine." },
	{ "#BRID\\B_344#Target Analysis On.",
	  "Analyse de Cible Activ\x8e" "e." },
	{ "#BRID\\B_343#Target Analysis Off.",
	  "Analyse de Cible D\x8esactiv\x8e" "e." },
	{ "#BRID\\BRID_S22#Aye Sir.",
	  "Oui mon Capitaine." },
	{ "#BRID\\BRID_S45#The shields are falling!",
	  "Les boucliers descendent!" },
	{ "#BRID\\BRID_S37#Our weapons are off-line!",
	  "Nos armes sont d\x8e" "connect\x8e" "es!" },
	{ "#BRID\\SCOTSHIE#Shields",
	  "Boucliers" },
	{ "#BRID\\SCOTHULL#Hull",
	  "Coque" },
	{ "#BRID\\SCOTBRID#Bridge",
	  "Passerelle" },
	{ "#BRID\\SCOTSENS#Sensors",
	  "Capteurs" },
	{ "#BRID\\SCOTTORP#Torpedoes",
	  "Torpilles" },
	{ "#BRID\\SCOTPHAS#Phasers",
	  "Phaseurs" },
	{ "#BRID\\SCOTENGI#Engines",
	  "Moteur" },
	{ "%s repaired, Captain.",
	  "%s repare, Capitaine." },
	{ "Captain",
	  "Capitaine" },
	{ "#BRID\\B_333#Entering standard orbit.",
	  "Entr\x8e" "e sur orbite standard." },
	{ "#BRID\\B_335#Leaving orbit.",
	  "D\x8epart d\xd5orbite." },
	{ "Hijacked",
	  "D\x8etourn\x8e" },
	{ "#BRID\\C_007#No recent missions have been completed.",
	  "Aucune mission r\x8e" "cente n\xd5" "a \x8et\x8e accomplie." },
	{ "#BRID\\B_336#Lowering shields and disarming weapons.",
	  "Boucliers rentr\x8es et armes d\x8esarm\x8e" "es." },
	{ "#BRID\\B_160#Romulan ship decloaking, Captain.",
	  "Un vaisseau Romulan appara\x94t, Capitaine." },
	{ "Romulan Captain",
	  "Romulan Capitaine" },
	{ "Quit Game",
	  "Quitter jeu" },
	{ "#GENE\\GENER028#Yes, quit the game.",
	  "Oui, quitter le jeu." },
	{ "#GENE\\GENER008#No, do not quit the game.",
	  "Non, ne pas quitter le jeu." },
	{ "Game Over",
	  "Jeu termin\x8e" },
	{ "#GENE\\GENER006#Load a previously saved game.",
	  "Charger un jeu sauvegard\x8e au pr\x8e" "alable." },
	{ "#GENE\\GENER020#Restart the game.",
	  "Recommencer le jeu." },
	{ "#GENE\\GENER018#Quit the game.",
	  "Quitter le jeu." },
	{ "#GENE\\SPACE000#The Enterprise has been destroyed!",
	  "L\xd5" "Enterprise a \x8et\x8e d\x8etruit!" },
	{ " choice %d",
	  " choix %d" },
	{ nullptr, nullptr }
};

// The Mac versions correct some texts of the DOS versions
static const TranslatedText kMacTexts[] = {
	{ "#BRID\\B_348#Unable to comply, Captain.  We're under attack.",
	  "#BRID\\B_348#Unable to comply, Captain. We're under attack." },
	{ "#BRID\\B_199#I'll be frank, Kirk. Starfleet expects more of you than that. Try to do better on your next assignment.",
	  "#BRID\\B_199#I'll be frank, Kirk, Starfleet expects more out of you than that. Try to do better on your next assignment." },
	{ "#BRID\\B_195#A perfect mission, Jim! You are a model for all Starfleet!",
	  "#BRID\\B_195#A perfect mission, Jim! You are a model for all Starfleet." },
	{ "#BRID\\C_060#Spock, come with me. Mr Scott, you have the conn.",
	  "#BRID\\C_060#Spock, come with me. Mr. Scott, you have the conn." },
	{ "#GENE\\GENER006#Load a previously saved game.",
	  "#GENE\\GENER006#Open a previously saved game." },
	{ nullptr, nullptr }
};

static const TranslatedText kMacFloppyTexts[] = {
	{ "Captain, the shield's are up.",
	  "Captain, the shields are up." },
	{ "We're not in orbit Captain.",
	  "We're not in orbit, Captain." },
	{ "Spock, come with me. Mr Scott, you have the con.",
	  "Spock, come with me. Mr. Scott, you have the conn." },
	{ "I don't know how long she can take it Captain.",
	  "I don't know how long she can take it, Captain." },
	{ "She can't take it Captain.",
	  "She can't take it, Captain." },
	{ "I'll be frank, Kirk, Star Fleet expects more out of you than that. Try to do better on your next assignment.",
	  "I'll be frank, Kirk, Starfleet expects more out of you than that. Try to do better on your next assignment." },
	{ "The top brass at Star Fleet are impressed. Outstanding work, Jim.",
	  "The top brass at Starfleet are impressed. Outstanding work, Jim." },
	{ "A perfect mission, Jim! You are a model for all Star Fleet",
	  "A perfect mission, Jim! You are a model for all Starfleet." },
	{ "#GENE\\GENER028#Yes, quit the game.",
	  "#GENE\\GENER028#Yes, quit the game without saving." },
	{ nullptr, nullptr }
};

const char *StarTrekEngine::localizeText(const char *text) const {
	bool isAmiga = getPlatform() == Common::kPlatformAmiga;
	const TranslatedText *table;
	if (getLanguage() == Common::DE_DEU)
		table = isAmiga ? kAmigaGermanTexts : kGermanTexts;
	else if (getLanguage() == Common::FR_FRA)
		table = isAmiga ? kAmigaFrenchTexts : kFrenchTexts;
	else if (getPlatform() == Common::kPlatformMacintosh)
		return localizeMacText(text);
	else
		return text;

	for (const TranslatedText *t = table; t->english; t++) {
		if (!strcmp(t->english, text))
			return t->translated;
	}

	return text;
}

const char *StarTrekEngine::localizeMacText(const char *text) const {
	if (isFloppyVersion()) {
		for (const TranslatedText *t = kMacFloppyTexts; t->english; t++) {
			if (!strcmp(t->english, text))
				return t->translated;
		}
	}
	for (const TranslatedText *t = kMacTexts; t->english; t++) {
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
