//WorldManager.cpp
#include "WorldManager.h"
#include "Passage.h"
#include "Item.h"
#include "Weapons.h"

// Constructor: calls creation and connection routines
WorldManager::WorldManager() {
    createRooms();
    connectRooms();
}

void WorldManager::createRooms() {
    #define ADD_ROOM(key, desc) rooms[key] = std::make_shared<Room>(key, desc);
    
    // Back Streets
    ADD_ROOM("Back Streets",
        "Ruined storefronts and shattered streetlights line cracked pavement stained with ash and blood.\n"
        "Flickering neon casts eerie shadows over abandoned debris.\n"
        "A red car sits battered near a wall, and a lifeless dead body slumps face-down nearby.\n"
        "This was where a Kiriko aligned Japanese PMC squad attempted a retreat before being bombed by Chinese forces.\n"
        "Evidence of their failed escape lies in blood smears and a trail of spent casings."
    );
    rooms["Back Streets"]->addLookable(
        "red car",
        "The red car's windshield is spiderwebbed with cracks. A note is carved crudely into the hood, barely legible."
    );
    rooms["Back Streets"]->addSearchable(
        "red car",
        "A bloodstained note inside warns of a PMC convoy stationed near the old bridge. Likely Kiriko's last operational unit. No equipment remains inside."
    );
    rooms["Back Streets"]->addLookable(
        "dead body",
        "A face-down dead body clutches a glowing overwrite card. The soldier wore Kiriko's emblem. Part of the escape squad ambushed by flanking Chinese forces."
    );
    rooms["Back Streets"]->addSearchable(
        "dead body",
        "The overwrite card bears a Kiriko BioTech seal, still warm. The soldier's other gear has been destroyed by blast damage or looted."
    );

    // TV Station
    ADD_ROOM("TV Station",
        "The TV Station's blackened studio holds scattered burnt equipment and melted cameras.\n"
        "This is the site of the first bloody encounter between Kiriko's hired PMCs and Chinese forces.\n"
        "A damaged tv rack stands against a cracked wall.\n"
        "A scorched broadcast desk holds a spilled cup of coffee and a faded photograph.\n"
        "Flickering broken monitors hum with static and show ghostly images of the chaos."
    );
    rooms["TV Station"]->addLookable(
        "tv rack",
        "The ash covered tv rack once held security tapes, now melted or jammed beyond use."
    );
    rooms["TV Station"]->addSearchable(
        "tv rack",
        "You find the only lab keycard, branded with the Longxue BioTech insignia, Kiriko's cover identity. All other contents are too badly burned to be useful."
    );
    rooms["TV Station"]->addLookable(
        "broadcast desk",
        "The broadcast desk's control panel is still faintly warm. A photograph lies face down in a puddle of coffee."
    );
    rooms["TV Station"]->addSearchable(
        "broadcast desk",
        "On the back of the photograph, a warning is scrawled: \"Do not let the truth reach daylight.\""
    );
    rooms["TV Station"]->addLookable(
        "broken monitors",
        "The broken monitors flicker with static, showing frozen frames of a gunfight."
    );
    rooms["TV Station"]->addSearchable(
        "broken monitors",
        "A journal page recounts the first wave of violence. Any data has been corrupted or wiped."
    );

    // Suburbs
    ADD_ROOM("Suburbs",
        "Burned out houses line a silent street in the outer suburbs.\n"
        "Towering skyscraper apartments loom over smaller homes, their windows shattered.\n"
        "Three cars, a blue car, a red car, and a scorched husk, sit in abandoned driveways.\n"
        "A torn child's backpack lies nearby, and a scorched flag flutters weakly on a mailbox."
    );
    rooms["Suburbs"]->addLookable(
        "blue car",
        "The blue car's doors hang open, and the engine is stripped, jammed, and useless."
    );
    rooms["Suburbs"]->addSearchable(
        "blue car",
        "A jammed 9mm pistol lies under the seat, wrapped in a cracked MP badge. The firing mechanism is fused solid and completely unusable."
    );
    rooms["Suburbs"]->addLookable(
        "backpack",
        "The torn child's backpack is worn, with a faded cartoon sticker on its flap."
    );
    rooms["Suburbs"]->addSearchable(
        "backpack",
        "Inside is a child's drawing signed \"Mei, Age 7.\" It shows a family under clouds with wires going into their heads."
    );
    rooms["Suburbs"]->addLookable(
        "flag",
        "The scorched flag conceals something tucked underneath."
    );
    rooms["Suburbs"]->addSearchable(
        "flag",
        "Hidden beneath is a handwritten evacuation order directing civilians to the subway tunnels."
    );

    // Factory
    ADD_ROOM("Factory",
        "The factory floor is filled with rusting machinery.\n"
        "A shattered crate lies beneath a collapsed catwalk, marked with a faded red cross.\n"
        "Lifeless mannequins stand frozen along the edges of old assembly lines, which are labeled Chemical Division."
    );
    rooms["Factory"]->addLookable(
        "crate",
        "The smashed crate once held medical gear. Its red cross is barely visible."
    );
    rooms["Factory"]->addSearchable(
        "crate",
        "You find an AFAK Medkit, but it has been breached and chemically contaminated. It is no longer safe or effective."
    );
    rooms["Factory"]->addLookable(
        "mannequins",
        "The mannequins are dressed in shredded hazmat suits. One is riddled with bullets."
    );
    rooms["Factory"]->addSearchable(
        "mannequins",
        "An ammo pouch is taped to a boot, but the bullets are corroded from chemical fumes and moisture."
    );
    rooms["Factory"]->addLookable(
        "assembly lines",
        "The assembly lines bear Kiriko's old division plaque: Chemical Weaponry R and D."
    );
    rooms["Factory"]->addSearchable(
        "assembly lines",
        "A lab slip details a virus breach and lockdown. Nearby gear is degraded beyond recovery."
    );

    // Theater
    ADD_ROOM("Theater",
        "Torn velvet seats face a ruined stage blackened by fire.\n"
        "A frayed rifle case lies cracked open but empty.\n"
        "A shotgun rack hangs bare nearby.\n"
        "Charred scorch marks line the walls, and scattered tickets litter the floor like ash."
    );
    rooms["Theater"]->addLookable(
        "rifle case",
        "The rifle case has frayed padding and scratches inside. No weapon remains."
    );
    rooms["Theater"]->addSearchable(
        "rifle case",
        "You discover a scratched numeric code: \"1914.\" The rifle once inside is long gone."
    );
    rooms["Theater"]->addLookable(
        "shotgun rack",
        "The shotgun rack has empty hooks swaying slowly, long looted."
    );
    rooms["Theater"]->addSearchable(
        "shotgun rack",
        "Dust outlines suggest weapons were taken just days before you arrived."
    );
    rooms["Theater"]->addLookable(
        "scorch marks",
        "The scorch marks streak across burnt wood and a painted backdrop."
    );
    rooms["Theater"]->addSearchable(
        "scorch marks",
        "A burnt playbill titled \"The Mirror's War\" reveals the theater was an evacuation point until it was shelled."
    );
    rooms["Theater"]->addLookable(
        "tickets",
        "The tickets are torn and mostly illegible."
    );
    rooms["Theater"]->addSearchable(
        "tickets",
        "One stub has strange coordinates scribbled in ink."
    );

    // Sewer
    ADD_ROOM("Sewer",
        "The damp sewers reek of decay.\n"
        "A rusted hatch leads deeper.\n"
        "Sandbags form a makeshift barricade, and faded graffiti covers the walls."
    );
    rooms["Sewer"]->addLookable(
        "hatch",
        "The hatch is bent at the hinges, water dripping from its seams."
    );
    rooms["Sewer"]->addSearchable(
        "hatch",
        "A backpack contains a flashlight, but the batteries are bloated and leaking acid. It is inoperable."
    );
    rooms["Sewer"]->addLookable(
        "sandbags",
        "The sandbags are stacked in haste, leaking dirt."
    );
    rooms["Sewer"]->addSearchable(
        "sandbags",
        "A magazine is hidden behind, but the bullets have corroded."
    );
    rooms["Sewer"]->addLookable(
        "graffiti",
        "The graffiti reads: \"STAY ABOVE. THEYRE BELOW.\""
    );
    rooms["Sewer"]->addSearchable(
        "graffiti",
        "A message scrawled in marker notes: \"Escape vent opens at 2:45 AM only.\""
    );

    // Zoo
    ADD_ROOM("Zoo",
        "The wide open zoo is cage free, with deep pits where animals once roamed.\n"
        "Rusted feeding troughs sit unused.\n"
        "A zoo map flaps on a broken pole, and a soaked clipboard dangles from a fence post."
    );
    rooms["Zoo"]->addLookable(
        "feeding troughs",
        "The feeding troughs are cracked and rusted with reddish stains."
    );
    rooms["Zoo"]->addSearchable(
        "feeding troughs",
        "A sedative vial and syringe are found, but the liquid has crystallized, making it useless."
    );
    rooms["Zoo"]->addLookable(
        "pits",
        "The pits are filled with bones and twisted restraints."
    );
    rooms["Zoo"]->addSearchable(
        "pits",
        "A journal scrap warns: \"Subjects escaped. Dont enter lion zone.\""
    );
    rooms["Zoo"]->addLookable(
        "zoo map",
        "The zoo map has zones marked out. \"Lab Entry\" is circled in red."
    );
    rooms["Zoo"]->addSearchable(
        "zoo map",
        "A note on the back reveals: \"Secret tunnel beneath lion den.\""
    );
    rooms["Zoo"]->addLookable(
        "clipboard",
        "The clipboard is soggy but legible."
    );
    rooms["Zoo"]->addSearchable(
        "clipboard",
        "A report says: \"Tiger missing. Evac revoked. Quarantine failed.\""
    );

    // Subway Station
    ADD_ROOM("Subway Station",
        "The subway station flickers under dim lights.\n"
        "A battered bulletin board, a torn subway schedule, and a grimy platform sign make up the scene."
    );
    rooms["Subway Station"]->addLookable(
        "bulletin board",
        "The bulletin board is covered in scribbled ink and blood."
    );
    rooms["Subway Station"]->addSearchable(
        "bulletin board",
        "A handmade map showing a safe route is taped to the underside."
    );
    rooms["Subway Station"]->addLookable(
        "subway schedule",
        "The subway schedule is faded and water damaged."
    );
    rooms["Subway Station"]->addSearchable(
        "subway schedule",
        "A rusty pocketknife is taped behind the board. Its blade is snapped and rusted shut."
    );
    rooms["Subway Station"]->addLookable(
        "platform sign",
        "The platform sign reads \"PLATFORM 3\" beneath layers of dirt."
    );
    rooms["Subway Station"]->addSearchable(
        "platform sign",
        "A message scrawled behind it reads: \"Meet agent at platform shift - 11:00 PM. Source Xi.\""
    );

    // Lab Underground Entrance
    ADD_ROOM("Lab Underground Entrance",
        "A heavy steel door pulses with bioluminescent veins through slime and rust.\n"
        "Its glow promises something buried within."
    );
    rooms["Lab Underground Entrance"]->addLookable(
        "bright door",
        "The bright door glows faintly with green pulses."
    );
    rooms["Lab Underground Entrance"]->addSearchable(
        "bright door",
        "Prying at a seam you uncover an overwrite card wedged in the sludge.\n"
        "This is the same card from the dead body in the Back Streets."
    );

    // Lab North Entrance
    ADD_ROOM("Lab North Entrance",
        "A barricade of scrap and shell casings marks an old skirmish site.\n"
        "Confidential lab reports litter the area, scattered by wind and time."
    );
    rooms["Lab North Entrance"]->addLookable(
        "lab reports",
        "The lab report is titled \"PROJECT ONRYO\" and bears the Kiriko seal."
    );
    rooms["Lab North Entrance"]->addSearchable(
        "lab reports",
        "A second lab keycard is wedged inside, but it is bent and unreadable by standard readers."
    );
    rooms["Lab North Entrance"]->addLookable(
        "barricade",
        "The barricade is built from rusted scrap and sandbags."
    );
    rooms["Lab North Entrance"]->addSearchable(
        "barricade",
        "A box of 9mm rounds is taped underneath, but moisture has ruined the powder inside."
    );
    rooms["Lab North Entrance"]->addLookable(
        "shell casings",
        "The shell casings bear Kiriko's mark."
    );
    rooms["Lab North Entrance"]->addSearchable(
        "shell casings",
        "You find the shattered lens from a Kiriko PMC helmet, no tech left intact."
    );

    // The Lab
    ADD_ROOM("The Lab",
        "The sterile lab interior is covered in shattered test tubes, cracked tiles, and a blinking digital console.\n"
        "Security cameras hang limp and broken."
    );
    rooms["The Lab"]->addLookable(
        "digital console",
        "The digital console flashes warnings: \"System Lockdown. Power Critical.\""
    );
    rooms["The Lab"]->addSearchable(
        "digital console",
        "A flash drive labeled \"PROJECT OMEGA\" is found, but it is encrypted with unknown tech."
    );
    rooms["The Lab"]->addLookable(
        "test tubes",
        "The test tubes glow faintly, some cracked, others bubbling."
    );
    rooms["The Lab"]->addSearchable(
        "test tubes",
        "A sealed vial of healing fluid is intact, but pressure has compromised its contents. Not safe to inject."
    );
    rooms["The Lab"]->addLookable(
        "security cameras",
        "The security cameras hang by threads, sparking."
    );
    rooms["The Lab"]->addSearchable(
        "security cameras",
        "A log plays: \"Unidentified breach - Level B compromised.\""
    );

    // Lab Courtyard
    ADD_ROOM("Lab Courtyard",
        "The lab courtyard is overgrown with weeds and tracked with muddy footprints.\n"
        "A smoldering barrel emits a thin wisp of smoke, and a weathered journal is half-buried nearby."
    );
    rooms["Lab Courtyard"]->addLookable(
        "weathered journal",
        "The journal is damp and warped."
    );
    rooms["Lab Courtyard"]->addSearchable(
        "weathered journal",
        "Its pages mention horrors unleashed in the sub labs."
    );
    rooms["Lab Courtyard"]->addLookable(
        "smoldering barrel",
        "The barrel glows faintly."
    );
    rooms["Lab Courtyard"]->addSearchable(
        "smoldering barrel",
        "A canteen rests inside, half full, but it smells like chemicals. Undrinkable."
    );
    rooms["Lab Courtyard"]->addLookable(
        "weeds",
        "The weeds are bent and flattened."
    );
    rooms["Lab Courtyard"]->addSearchable(
        "weeds",
        "Beneath them lies a collapsed ventilation shaft leading downward."
    );

}
void WorldManager::connectRooms() {
    // Passages use the destination’s name string as the “direction” label
    Passage::createBasicPassage(rooms["Theater"].get(),            rooms["Suburbs"].get(),             "Suburbs",                 false);
    Passage::createBasicPassage(rooms["Suburbs"].get(),            rooms["Theater"].get(),             "Theater",                 false);
    Passage::createBasicPassage(rooms["Theater"].get(),            rooms["Zoo"].get(),                 "Zoo",                     false);
    Passage::createBasicPassage(rooms["Zoo"].get(),                rooms["Theater"].get(),             "Theater",                 false);
    Passage::createBasicPassage(rooms["Theater"].get(),            rooms["Sewer"].get(),               "Sewer",                   false);
    Passage::createBasicPassage(rooms["Sewer"].get(),              rooms["Theater"].get(),             "Theater",                 false);
    Passage::createBasicPassage(rooms["Suburbs"].get(),            rooms["TV Station"].get(),          "TV Station",              false);
    Passage::createBasicPassage(rooms["TV Station"].get(),         rooms["Suburbs"].get(),             "Suburbs",                 false);
    Passage::createBasicPassage(rooms["Suburbs"].get(),            rooms["Factory"].get(),             "Factory",                 false);
    Passage::createBasicPassage(rooms["Factory"].get(),            rooms["Suburbs"].get(),             "Suburbs",                 false);
    Passage::createBasicPassage(rooms["Suburbs"].get(),            rooms["Sewer"].get(),               "Sewer",                   false);
    Passage::createBasicPassage(rooms["Sewer"].get(),              rooms["Suburbs"].get(),             "Suburbs",                 false);
    Passage::createBasicPassage(rooms["Zoo"].get(),                rooms["Subway Station"].get(),      "Subway Station",          false);
    Passage::createBasicPassage(rooms["Subway Station"].get(),     rooms["Zoo"].get(),                 "Zoo",                     false);
    Passage::createBasicPassage(rooms["Zoo"].get(),                rooms["Back Streets"].get(),        "Back Streets",            false);
    Passage::createBasicPassage(rooms["Back Streets"].get(),       rooms["Zoo"].get(),                 "Zoo",                     false);
    Passage::createBasicPassage(rooms["TV Station"].get(),         rooms["Back Streets"].get(),        "Back Streets",            false);
    Passage::createBasicPassage(rooms["Back Streets"].get(),       rooms["TV Station"].get(),          "TV Station",              false);
    Passage::createBasicPassage(rooms["Factory"].get(),            rooms["Lab North Entrance"].get(),   "Lab North Entrance",      false);
    Passage::createBasicPassage(rooms["Lab North Entrance"].get(), rooms["Factory"].get(),             "Factory",                 false);
    Passage::createBasicPassage(rooms["Factory"].get(),            rooms["Lab Underground Entrance"].get(),"Lab Underground Entrance", false);
    Passage::createBasicPassage(rooms["Lab Underground Entrance"].get(), rooms["Factory"].get(),           "Factory",                 false);
    Passage::createBasicPassage(rooms["Subway Station"].get(),     rooms["Lab Underground Entrance"].get(), "Lab Underground Entrance", false);
    Passage::createBasicPassage(rooms["Lab Underground Entrance"].get(), rooms["Subway Station"].get(),     "Subway Station",         false);
    Passage::createBasicPassage(rooms["Subway Station"].get(),     rooms["Sewer"].get(),                "Sewer",                   false);
    Passage::createBasicPassage(rooms["Sewer"].get(),              rooms["Subway Station"].get(),        "Subway Station",         false);
    Passage::createBasicPassage(rooms["Lab North Entrance"].get(), rooms["The Lab"].get(),              "The Lab",                 false);
    Passage::createBasicPassage(rooms["The Lab"].get(),            rooms["Lab North Entrance"].get(),     "Lab North Entrance",     false);
    Passage::createBasicPassage(rooms["Lab North Entrance"].get(), rooms["Lab Courtyard"].get(),         "Lab Courtyard",          false);
    Passage::createBasicPassage(rooms["Lab Courtyard"].get(),      rooms["Lab North Entrance"].get(),     "Lab North Entrance",     false);
    Passage::createBasicPassage(rooms["Lab Underground Entrance"].get(), rooms["The Lab"].get(),            "The Lab",                false);
    Passage::createBasicPassage(rooms["The Lab"].get(),            rooms["Lab Underground Entrance"].get(), "Lab Underground Entrance", false);
    Passage::createBasicPassage(rooms["Lab Courtyard"].get(),      rooms["The Lab"].get(),               "The Lab",                false);
    Passage::createBasicPassage(rooms["The Lab"].get(),            rooms["Lab Courtyard"].get(),          "Lab Courtyard",          false);
}

std::shared_ptr<Room> WorldManager::getStartingRoom() const {
    return rooms.at("Theater");
}

const std::map<std::string, std::shared_ptr<Room>>& WorldManager::getAllRooms() const {
    return rooms;
}