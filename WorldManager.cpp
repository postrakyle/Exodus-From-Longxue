#include "WorldManager.h"
#include "Passage.h"
#include "Item.h"

// Constructor: calls creation and connection routines
WorldManager::WorldManager() {
    createRooms();
    connectRooms();
}

void WorldManager::createRooms() {
    #define ADD_ROOM(key, desc) rooms[key] = std::make_shared<Room>(key, desc);

    // Back Streets
    ADD_ROOM("Back Streets",
        "You stand amid crumbling storefronts and shattered streetlights. A battered red car is half-buried in rubble."
    );
    rooms["Back Streets"]->addLookable(
        "red car",
        "The red car’s windshield is shattered. Bullet holes pockmark its hood."
    );
    rooms["Back Streets"]->addSearchable(
        "red car",
        "You search the red car’s glove compartment and find a bloodstained note: \"PMC convoy spotted near the old bridge at dawn.\""
    );

    // TV Station
    ADD_ROOM("TV Station",
        "The TV Station’s facade is blackened by fire. Broken cameras lie on the floor. A cracked rack is nearby."
    );
    rooms["TV Station"]->addLookable(
        "tv rack",
        "A small rack of electronics stands half-collapsed."
    );
    rooms["TV Station"]->addSearchable(
        "tv rack",
        "You search the rack and uncover a Lab Keycard."
    );

    // Suburbs
    ADD_ROOM("Suburbs",
        "Burned-out houses line a silent street. Three cars—a red car, a blue car, and a burnt-out car—sit in driveways."
    );
    rooms["Suburbs"]->addLookable(
        "blue car",
        "The blue car’s doors are ajar. The engine has been stripped for parts."
    );
    rooms["Suburbs"]->addSearchable(
        "blue car",
        "You search the blue car’s glove box and find a Government Issue 9mm Pistol."
    );
    rooms["Suburbs"]->addLookable(
        "burnt car",
        "A charred husk of a vehicle sits here, incinerated from top to bottom."
    );
    rooms["Suburbs"]->addSearchable(
        "burnt car",
        "You sift through the ash but find nothing besides carbonized metal."
    );

    // Factory
    ADD_ROOM("Factory",
        "Inside the Factory, rusted assembly lines stand frozen. A smashed crate labeled “Medical Supplies” lies half-open."
    );
    rooms["Factory"]->addLookable(
        "smashed crate",
        "The wooden crate is splintered. A red cross is barely visible on one side."
    );
    rooms["Factory"]->addSearchable(
        "smashed crate",
        "You rummage in the crate and pull out an AFAK Medkit."
    );

    // Theater
    ADD_ROOM("Theater",
        "Grand velvet seats are shredded, and the stage is littered with debris. A rifle case lies near the front row, and a shotgun rack rests by a broken chair."
    );
    rooms["Theater"]->addLookable(
        "rifle case",
        "A battered rifle case sits open. It looks like it was pried open in a hurry."
    );
    rooms["Theater"]->addSearchable(
        "rifle case",
        "You search inside the rifle case and find a functional Rifle."
    );
    rooms["Theater"]->addLookable(
        "shotgun rack",
        "A shotgun rack stands next to a shattered chair, one slot now empty."
    );
    rooms["Theater"]->addSearchable(
        "shotgun rack",
        "You pull a Shotgun off the rack. It’s still loaded."
    );

    // Sewer
    ADD_ROOM("Sewer",
        "The stench of sewage hangs heavy. Water pools around broken pipes and discarded sandbags."
    );
    rooms["Sewer"]->addLookable(
        "hatch",
        "The metal hatch is covered in grime. Its hinges are bent."
    );
    rooms["Sewer"]->addSearchable(
        "hatch",
        "You lift the hatch fully and find a discarded backpack containing a flashlight and a note."
    );
    rooms["Sewer"]->addLookable(
        "sandbags",
        "Old sandbags ring parts of the tunnel, probably used as cover during a firefight."
    );
    rooms["Sewer"]->addSearchable(
        "sandbags",
        "You search behind the sandbags and uncover a single 7.62×39mm magazine."
    );

    // Zoo
    ADD_ROOM("Zoo",
        "Empty animal cages creak in the wind. Metal feeding troughs sit empty."
    );
    rooms["Zoo"]->addLookable(
        "clipboard",
        "The clipboard’s papers are waterlogged but still legible in parts."
    );
    rooms["Zoo"]->addSearchable(
        "clipboard",
        "You salvage a page: \"Missing: tiger #12. Quarantine zone breached.\""
    );
    rooms["Zoo"]->addLookable(
        "feeding trough",
        "The trough is rusted and stained—no signs of recent animal activity."
    );
    rooms["Zoo"]->addSearchable(
        "feeding trough",
        "At the bottom, you find a vial labeled “sedative. Do not ingest.”"
    );

    // Subway Station
    ADD_ROOM("Subway Station",
        "The station is eerily quiet, illuminated by flickering emergency lights."
    );
    rooms["Subway Station"]->addLookable(
        "bulletin board",
        "The torn schedule hangs by a single nail."
    );
    rooms["Subway Station"]->addSearchable(
        "bulletin board",
        "Behind the torn schedule, you discover a map of hidden tunnels."
    );
    rooms["Subway Station"]->addLookable(
        "subway schedule",
        "The schedule’s ink has bled. You can make out only: \"Last stop: Walled City.\""
    );
    rooms["Subway Station"]->addSearchable(
        "subway schedule",
        "You find a pocketknife taped to the board."
    );

    // Lab Underground Entrance
    ADD_ROOM("Lab Underground Entrance",
        "A battered steel door stands ajar. Flickering lights spill out from below."
    );
    rooms["Lab Underground Entrance"]->addLookable(
        "security badge",
        "The badge bears the insignia of Longxue BioTech."
    );
    rooms["Lab Underground Entrance"]->addSearchable(
        "security badge",
        "You pick up the badge: \"Research Lead: Dr. Han Li.\""
    );
    rooms["Lab Underground Entrance"]->addLookable(
        "flickering lights",
        "The lights above buzz loudly, casting long shadows."
    );
    rooms["Lab Underground Entrance"]->addSearchable(
        "flickering lights",
        "You see a scorched circuit panel—someone tried to disable power here."
    );

    // Lab North Entrance
    ADD_ROOM("Lab North Entrance",
        "A massive barricade blocks the entry. Bullet holes pepper the steel plating."
    );
    rooms["Lab North Entrance"]->addLookable(
        "lab report",
        "The report’s pages are stained and smudged."
    );
    rooms["Lab North Entrance"]->addSearchable(
        "lab report",
        "You sift through the pages and find a Level-2 Lab Keycard."
    );
    rooms["Lab North Entrance"]->addLookable(
        "barricade",
        "The barricade is reinforced with sandbags and scrap metal."
    );
    rooms["Lab North Entrance"]->addSearchable(
        "barricade",
        "Behind a loose plank, you discover two rounds of 9mm ammo."
    );

    // The Lab
    ADD_ROOM("The Lab",
        "White tile floors are cracked. Broken test tubes litter the counters."
    );
    rooms["The Lab"]->addLookable(
        "digital console",
        "The console reads: \"Biohazard Containment Failure.\""
    );
    rooms["The Lab"]->addSearchable(
        "digital console",
        "You hack and retrieve a flash drive labeled \"Project Omega.\""
    );
    rooms["The Lab"]->addLookable(
        "test tubes",
        "Test tubes are shattered; some still hold a glowing green liquid."
    );
    rooms["The Lab"]->addSearchable(
        "test tubes",
        "You collect one intact tube—its fluid might be a potent healing agent."
    );

    // Lab Courtyard
    ADD_ROOM("Lab Courtyard",
        "Overgrown weeds choke the cracked pavement. A rusted barrel smolders with ash."
    );
    rooms["Lab Courtyard"]->addLookable(
        "weathered journal",
        "The faded journal page is stained with coffee."
    );
    rooms["Lab Courtyard"]->addSearchable(
        "weathered journal",
        "You retrieve the page: \"Day 12: Soldiers keep disappearing…\""
    );
    rooms["Lab Courtyard"]->addLookable(
        "smoldering barrel",
        "The barrel’s fire flickers weakly."
    );
    rooms["Lab Courtyard"]->addSearchable(
        "smoldering barrel",
        "Under the barrel’s lid, you find a half-empty canteen of water."
    );
}

void WorldManager::connectRooms() {
    // Note: Passages use the destination’s name string as the “direction” label
    Passage::createBasicPassage(rooms["Theater"].get(),           rooms["Suburbs"].get(),          "Suburbs",               false);
    Passage::createBasicPassage(rooms["Suburbs"].get(),           rooms["Theater"].get(),          "Theater",               false);
    Passage::createBasicPassage(rooms["Theater"].get(),           rooms["Zoo"].get(),              "Zoo",                   false);
    Passage::createBasicPassage(rooms["Zoo"].get(),               rooms["Theater"].get(),          "Theater",               false);
    Passage::createBasicPassage(rooms["Theater"].get(),           rooms["Sewer"].get(),            "Sewer",                 false);
    Passage::createBasicPassage(rooms["Sewer"].get(),             rooms["Theater"].get(),          "Theater",               false);
    Passage::createBasicPassage(rooms["Suburbs"].get(),           rooms["TV Station"].get(),       "TV Station",            false);
    Passage::createBasicPassage(rooms["TV Station"].get(),        rooms["Suburbs"].get(),          "Suburbs",               false);
    Passage::createBasicPassage(rooms["Suburbs"].get(),           rooms["Factory"].get(),          "Factory",               false);
    Passage::createBasicPassage(rooms["Factory"].get(),           rooms["Suburbs"].get(),          "Suburbs",               false);
    Passage::createBasicPassage(rooms["Suburbs"].get(),           rooms["Sewer"].get(),            "Sewer",                 false);
    Passage::createBasicPassage(rooms["Sewer"].get(),             rooms["Suburbs"].get(),          "Suburbs",               false);
    Passage::createBasicPassage(rooms["Zoo"].get(),               rooms["Subway Station"].get(),   "Subway Station",        false);
    Passage::createBasicPassage(rooms["Subway Station"].get(),    rooms["Zoo"].get(),              "Zoo",                   false);
    Passage::createBasicPassage(rooms["Zoo"].get(),               rooms["Back Streets"].get(),     "Back Streets",          false);
    Passage::createBasicPassage(rooms["Back Streets"].get(),      rooms["Zoo"].get(),              "Zoo",                   false);
    Passage::createBasicPassage(rooms["TV Station"].get(),        rooms["Back Streets"].get(),     "Back Streets",          false);
    Passage::createBasicPassage(rooms["Back Streets"].get(),      rooms["TV Station"].get(),       "TV Station",            false);
    Passage::createBasicPassage(rooms["Factory"].get(),           rooms["Lab North Entrance"].get(),"Lab North Entrance",   false);
    Passage::createBasicPassage(rooms["Lab North Entrance"].get(),rooms["Factory"].get(),          "Factory",               false);
    Passage::createBasicPassage(rooms["Factory"].get(),           rooms["Lab Underground Entrance"].get(),"Lab Underground Entrance", false);
    Passage::createBasicPassage(rooms["Lab Underground Entrance"].get(), rooms["Factory"].get(),            "Factory",                 false);
    Passage::createBasicPassage(rooms["Subway Station"].get(),    rooms["Lab Underground Entrance"].get(), "Lab Underground Entrance", false);
    Passage::createBasicPassage(rooms["Lab Underground Entrance"].get(), rooms["Subway Station"].get(),            "Subway Station",         false);
    Passage::createBasicPassage(rooms["Subway Station"].get(),    rooms["Sewer"].get(),             "Sewer",                false);
    Passage::createBasicPassage(rooms["Sewer"].get(),             rooms["Subway Station"].get(),     "Subway Station",        false);
    Passage::createBasicPassage(rooms["Lab North Entrance"].get(),rooms["The Lab"].get(),           "The Lab",                false);
    Passage::createBasicPassage(rooms["The Lab"].get(),           rooms["Lab North Entrance"].get(), "Lab North Entrance",    false);
    Passage::createBasicPassage(rooms["Lab North Entrance"].get(),rooms["Lab Courtyard"].get(),     "Lab Courtyard",         false);
    Passage::createBasicPassage(rooms["Lab Courtyard"].get(),     rooms["Lab North Entrance"].get(), "Lab North Entrance",    false);
    Passage::createBasicPassage(rooms["Lab Underground Entrance"].get(), rooms["The Lab"].get(),     "The Lab",               false);
    Passage::createBasicPassage(rooms["The Lab"].get(),           rooms["Lab Underground Entrance"].get(), "Lab Underground Entrance", false);
    Passage::createBasicPassage(rooms["Lab Courtyard"].get(),     rooms["The Lab"].get(),            "The Lab",               false);
    Passage::createBasicPassage(rooms["The Lab"].get(),           rooms["Lab Courtyard"].get(),      "Lab Courtyard",         false);
}

std::shared_ptr<Room> WorldManager::getStartingRoom() const {
    return rooms.at("Theater");
}

const std::map<std::string, std::shared_ptr<Room>>& WorldManager::getAllRooms() const {
    return rooms;
}
