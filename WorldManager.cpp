#include "WorldManager.h"
#include "Passage.h"

WorldManager::WorldManager() {
    createRooms();
    connectRooms();
}

void WorldManager::createRooms() {
    #define ADD_ROOM(key, desc) rooms[key] = std::make_shared<Room>(key, desc);

    // Back Streets
    ADD_ROOM("Back Streets",
        "You stand amid crumbling storefronts and shattered streetlights. "
        "A battered red car is half-buried in rubble. An overturned trash bin lies nearby.");

    rooms["Back Streets"]->addLookable(
        "red car",
        "The red car’s windshield is shattered. Bullet holes pockmark its hood."
    );
    rooms["Back Streets"]->addSearchable(
        "red car",
        "You search the red car’s glove compartment and find a bloodstained note: "
        "\"PMC convoy spotted near the old bridge at dawn.\""
    );

    rooms["Back Streets"]->addLookable(
        "trash bin",
        "The metal trash bin lies tipped over. Rotten food scraps and a torn flyer peek out."
    );
    rooms["Back Streets"]->addSearchable(
        "trash bin",
        "Inside the trash bin, you find a crushed energy bar—still edible—and a scrap: "
        "\"Evac orders: all civilians must leave by 1800 hours.\""
    );

    // TV Station
    ADD_ROOM("TV Station",
        "The TV Station’s facade is blackened by fire. Broken cameras lie on the floor. "
        "A cracked control panel flickers faintly.");

    rooms["TV Station"]->addLookable(
        "cameras",
        "Two tripod-mounted cameras lie shattered. Their lenses are cracked and useless."
    );
    rooms["TV Station"]->addSearchable(
        "cameras",
        "Searching the broken cameras, you recover a small memory card labeled: "
        "\"0412_TRANSMIT—Longxue covert ops.\""
    );

    rooms["TV Station"]->addLookable(
        "control panel",
        "The control panel flickers with static. Buttons are melted, but a single keycard rests on it."
    );
    rooms["TV Station"]->addSearchable(
        "control panel",
        "You pry the keycard off the scorched panel. Its label reads: \"Level 2 Access—Bio Lab.\""
    );

    // Suburbs
    ADD_ROOM("Suburbs",
        "Burned-out houses line a silent street. Three cars—a red car, a blue car, and a burnt car—"
        "sit in driveways. Wildflowers now grow through cracked asphalt.");

    rooms["Suburbs"]->addLookable(
        "blue car",
        "The blue car’s doors are ajar. The engine has been stripped, likely for parts."
    );
    rooms["Suburbs"]->addSearchable(
        "blue car",
        "Inside the glove box of the blue car, you find a small pistol with one round—"
        "\"Government issue 9mm.\""
    );

    rooms["Suburbs"]->addLookable(
        "burnt car",
        "The burnt-out husk of a car is nothing but char and twisted metal."
    );
    rooms["Suburbs"]->addSearchable(
        "burnt car",
        "You sift through the ash. Beneath a melted hubcap, you discover a crumpled photograph: "
        "\"Family at the Jinpu Bridge, 03-01-20xx.\""
    );

    // Factory
    ADD_ROOM("Factory",
        "Inside the Factory, rusted assembly lines stand frozen. The ceiling is caved in near the loading dock. "
        "A smashed crate marked “Medical Supplies” lies half-open.");

    rooms["Factory"]->addLookable(
        "smashed crate",
        "The wooden crate is splintered. The red cross on its side is still faintly visible."
    );
    rooms["Factory"]->addSearchable(
        "smashed crate",
        "You dig inside the crate and find a sealed first‐aid kit—bandages, antiseptic. "
        "There’s also a scrap: \"PharmaCorp plant infected—recall immediately.\""
    );

    rooms["Factory"]->addLookable(
        "ceiling",
        "The ceiling’s collapse exposed a rusted gantry crane. Debris litters the floor."
    );
    rooms["Factory"]->addSearchable(
        "ceiling",
        "Behind a fallen beam, you find a half‐burned manual: \"PMC armor specs—Level 3 Kevlar.\""
    );

    // Theater
    ADD_ROOM("Theater",
        "Grand velvet seats are shredded, and the stage is littered with debris. "
        "A trophy case stands shattered to one side.");

    rooms["Theater"]->addLookable(
        "trophy case",
        "The glass is broken; trophies lie scattered and tarnished."
    );
    rooms["Theater"]->addSearchable(
        "trophy case",
        "In the rubble you find a small military medal inscribed: \"Longxue Defense 20XX.\""
    );

    rooms["Theater"]->addLookable(
        "stage",
        "The stage lights hang crooked. A folded script lies near center stage."
    );
    rooms["Theater"]->addSearchable(
        "stage",
        "Unfolding the script, you read the final scene: "
        "\"… and the city falls at midnight. We have failed.\"\nIt’s dated days before the war’s outbreak."
    );

    // Sewer
    ADD_ROOM("Sewer",
        "The stench of sewage hangs heavy. Water pools around broken pipes and discarded sandbags. "
        "A rusted hatch lies partially open.");

    rooms["Sewer"]->addLookable(
        "hatch",
        "The metal hatch is covered in grime. Its hinges are bent."
    );
    rooms["Sewer"]->addSearchable(
        "hatch",
        "You lift the hatch fully and find a discarded backpack containing:\n"
        "  • One flashlight (batteries half‐dead)\n"
        "  • A small hand‐written note: \"Stash supplies in East tunnel—code 7429.\""
    );

    rooms["Sewer"]->addLookable(
        "sandbags",
        "Old sandbags ring parts of the tunnel, probably used as cover during a firefight."
    );
    rooms["Sewer"]->addSearchable(
        "sandbags",
        "You search behind the sandbags and uncover a single magazine of ammo—7.62×39mm."
    );

    // Zoo
    ADD_ROOM("Zoo",
        "Empty animal cages creak in the wind. Metal feeding troughs sit empty. "
        "A battered keeper’s clipboard lies on the ground.");

    rooms["Zoo"]->addLookable(
        "clipboard",
        "The clipboard’s papers are waterlogged but still legible in parts."
    );
    rooms["Zoo"]->addSearchable(
        "clipboard",
        "You salvage a partially intact page: \"Missing: tiger #12. Quarantine zone breached.\""
    );

    rooms["Zoo"]->addLookable(
        "feeding trough",
        "The trough is rusted and stained—no signs of recent animal activity."
    );
    rooms["Zoo"]->addSearchable(
        "feeding trough",
        "At the bottom, you find a small vial labeled \"sedative. Do not ingest.\""
    );

    // Subway Station
    ADD_ROOM("Subway Station",
        "The station is eerily quiet, illuminated by flickering emergency lights. "
        "A ripped subway schedule flaps on a bulletin board.");

    rooms["Subway Station"]->addLookable(
        "bulletin board",
        "The torn schedule hangs by a single nail. It lists trains that never came."
    );
    rooms["Subway Station"]->addSearchable(
        "bulletin board",
        "Pinned behind the schedule is a handwritten map of hidden tunnels under Longxue."
    );

    rooms["Subway Station"]->addLookable(
        "subway schedule",
        "The schedule’s ink has bled. You can make out only one line: \"Last stop: Walled City.\""
    );
    rooms["Subway Station"]->addSearchable(
        "subway schedule",
        "Hidden behind the ripped paper, you discover a pocketknife taped to the board."
    );

    // Lab Underground Entrance
    ADD_ROOM("Lab Underground Entrance",
        "A battered steel door stands ajar. Flickering lights spill out from below. "
        "A torn security badge lies at the threshold.");

    rooms["Lab Underground Entrance"]->addLookable(
        "security badge",
        "The badge bears the insignia of Longxue BioTech, with a name scratched out."
    );
    rooms["Lab Underground Entrance"]->addSearchable(
        "security badge",
        "You pick up the badge and find etched on the back: \"Research Lead: Dr. Han Li.\""
    );

    rooms["Lab Underground Entrance"]->addLookable(
        "flickering lights",
        "The lights above buzz loudly, casting long shadows on cracked tile."
    );
    rooms["Lab Underground Entrance"]->addSearchable(
        "flickering lights",
        "You see a scorched circuit panel underneath—someone tried to disable power here."
    );

    // Lab North Entrance
    ADD_ROOM("Lab North Entrance",
        "A massive barricade blocks the entry. Bullet holes pepper the steel plating. "
        "A crumpled lab report flutters on a nearby crate.");

    rooms["Lab North Entrance"]->addLookable(
        "lab report",
        "The report’s pages are stained with what looks like spilled chemicals."
    );
    rooms["Lab North Entrance"]->addSearchable(
        "lab report",
        "Sifting through the pages, you read: \"Experiment Theta-3 facility breach—initiating lockdown.\""
    );

    rooms["Lab North Entrance"]->addLookable(
        "barricade",
        "The barricade is reinforced with sandbags and scrap metal—clearly built in haste."
    );
    rooms["Lab North Entrance"]->addSearchable(
        "barricade",
        "Behind a loose plank in the barricade, you discover two rounds of 9mm ammo."
    );

    // The Lab
    ADD_ROOM("The Lab",
        "White tile floors are cracked. Broken test tubes litter the counters. "
        "A digital console at one end flickers with a red warning light.");

    rooms["The Lab"]->addLookable(
        "digital console",
        "The console reads: \"Alert: Biohazard Containment Failure.\" The red light blinks."
    );
    rooms["The Lab"]->addSearchable(
        "digital console",
        "You manage to hack the console and retrieve a flash drive labeled \"Project Omega\"."
    );

    rooms["The Lab"]->addLookable(
        "test tubes",
        "Many test tubes are shattered. Some still hold a glowing green liquid."
    );
    rooms["The Lab"]->addSearchable(
        "test tubes",
        "You carefully collect one intact tube—its fluid might be a potent healing agent, or a—?"
    );

    // Lab Courtyard
    ADD_ROOM("Lab Courtyard",
        "Overgrown weeds choke the cracked pavement. A rusted barrel smolders with ash. "
        "A weathered journal page sticks out beneath a rock.");

    rooms["Lab Courtyard"]->addLookable(
        "weathered journal",
        "The faded journal page is stained with coffee and what looks like fingerprints."
    );
    rooms["Lab Courtyard"]->addSearchable(
        "weathered journal",
        "You retrieve the page: \"Day 12: Soldiers keep disappearing at night. We’re running low on meds.\""
    );

    rooms["Lab Courtyard"]->addLookable(
        "smoldering barrel",
        "The barrel’s fire flickers weakly. Smoke curls upward."
    );
    rooms["Lab Courtyard"]->addSearchable(
        "smoldering barrel",
        "Under the barrel’s lid, you find a half‐empty canteen of clean water."
    );
}

void WorldManager::connectRooms() {
    //
    // EXACTLY as before, but using destination room names only:
    //

    Passage::createBasicPassage(rooms["Theater"].get(), rooms["Suburbs"].get(),        "Suburbs",               false);
    Passage::createBasicPassage(rooms["Suburbs"].get(), rooms["Theater"].get(),        "Theater",               false);

    Passage::createBasicPassage(rooms["Theater"].get(), rooms["Zoo"].get(),            "Zoo",                   false);
    Passage::createBasicPassage(rooms["Zoo"].get(),     rooms["Theater"].get(),        "Theater",               false);

    Passage::createBasicPassage(rooms["Theater"].get(), rooms["Sewer"].get(),          "Sewer",                 false);
    Passage::createBasicPassage(rooms["Sewer"].get(),   rooms["Theater"].get(),        "Theater",               false);

    Passage::createBasicPassage(rooms["Suburbs"].get(), rooms["TV Station"].get(),     "TV Station",            false);
    Passage::createBasicPassage(rooms["TV Station"].get(), rooms["Suburbs"].get(),     "Suburbs",               false);

    Passage::createBasicPassage(rooms["Suburbs"].get(), rooms["Factory"].get(),        "Factory",               false);
    Passage::createBasicPassage(rooms["Factory"].get(),  rooms["Suburbs"].get(),        "Suburbs",               false);

    Passage::createBasicPassage(rooms["Suburbs"].get(), rooms["Sewer"].get(),          "Sewer",                 false);
    Passage::createBasicPassage(rooms["Sewer"].get(),   rooms["Suburbs"].get(),        "Suburbs",               false);

    Passage::createBasicPassage(rooms["Zoo"].get(),           rooms["Subway Station"].get(), "Subway Station", false);
    Passage::createBasicPassage(rooms["Subway Station"].get(), rooms["Zoo"].get(),         "Zoo",             false);

    Passage::createBasicPassage(rooms["Zoo"].get(),           rooms["Back Streets"].get(),   "Back Streets",    false);
    Passage::createBasicPassage(rooms["Back Streets"].get(),   rooms["Zoo"].get(),            "Zoo",             false);

    Passage::createBasicPassage(rooms["TV Station"].get(),     rooms["Back Streets"].get(),   "Back Streets",    false);
    Passage::createBasicPassage(rooms["Back Streets"].get(),   rooms["TV Station"].get(),     "TV Station",      false);

    Passage::createBasicPassage(rooms["Factory"].get(),        rooms["Lab North Entrance"].get(), "Lab North Entrance", false);
    Passage::createBasicPassage(rooms["Lab North Entrance"].get(), rooms["Factory"].get(),        "Factory",            false);

    Passage::createBasicPassage(rooms["Factory"].get(),        rooms["Lab Underground Entrance"].get(), "Lab Underground Entrance", false);
    Passage::createBasicPassage(rooms["Lab Underground Entrance"].get(), rooms["Factory"].get(),        "Factory",                  false);

    Passage::createBasicPassage(rooms["Subway Station"].get(), rooms["Lab Underground Entrance"].get(), "Lab Underground Entrance", false);
    Passage::createBasicPassage(rooms["Lab Underground Entrance"].get(), rooms["Subway Station"].get(),     "Subway Station",          false);

    Passage::createBasicPassage(rooms["Subway Station"].get(), rooms["Sewer"].get(),       "Sewer",           false);
    Passage::createBasicPassage(rooms["Sewer"].get(),            rooms["Subway Station"].get(), "Subway Station", false);

    Passage::createBasicPassage(rooms["Lab North Entrance"].get(), rooms["The Lab"].get(),   "The Lab",          false);
    Passage::createBasicPassage(rooms["The Lab"].get(),            rooms["Lab North Entrance"].get(), "Lab North Entrance", false);

    Passage::createBasicPassage(rooms["Lab North Entrance"].get(), rooms["Lab Courtyard"].get(), "Lab Courtyard", false);
    Passage::createBasicPassage(rooms["Lab Courtyard"].get(),       rooms["Lab North Entrance"].get(), "Lab North Entrance", false);

    Passage::createBasicPassage(rooms["Lab Underground Entrance"].get(), rooms["The Lab"].get(), "The Lab",                  false);
    Passage::createBasicPassage(rooms["The Lab"].get(),              rooms["Lab Underground Entrance"].get(), "Lab Underground Entrance", false);

    Passage::createBasicPassage(rooms["Lab Courtyard"].get(),       rooms["The Lab"].get(), "The Lab",      false);
    Passage::createBasicPassage(rooms["The Lab"].get(),             rooms["Lab Courtyard"].get(), "Lab Courtyard", false);
}

std::shared_ptr<Room> WorldManager::getStartingRoom() const {
    return rooms.at("Theater");
}

const std::map<std::string, std::shared_ptr<Room>>& WorldManager::getAllRooms() const {
    return rooms;
}
