#include "WorldManager.h"
#include "Passage.h"

WorldManager::WorldManager() {
    createRooms();
    connectRooms();
}

void WorldManager::createRooms() {
    #define ADD_ROOM(key, desc) rooms[key] = std::make_shared<Room>(key, desc);

    ADD_ROOM("Back Streets",             "Shadowy back streets littered with debris.");
    ADD_ROOM("TV Station",               "Ruined TV station, glass and cameras everywhere.");
    ADD_ROOM("Suburbs",                  "Quiet residential area, too quiet...");
    ADD_ROOM("Factory",                  "A decrepit factory, machines still and cold.");
    ADD_ROOM("Theater",                  "Grand old theater with velvet seats.");
    ADD_ROOM("Sewer",                    "Rank, dark sewers beneath the city.");
    ADD_ROOM("Zoo",                      "Abandoned zoo. Cages broken open.");
    ADD_ROOM("Subway Station",           "Dusty subway platforms.");
    ADD_ROOM("Lab Underground Entrance", "A hidden entrance deep below the city.");
    ADD_ROOM("Lab North Entrance",       "An imposing reinforced door to the lab.");
    ADD_ROOM("The Lab",                  "Sterile, abandoned corridors with flickering lights.");
    ADD_ROOM("Lab Courtyard",            "Overgrown yard, patrolled by silence.");
}

void WorldManager::connectRooms() {
    //
    // We explicitly create TWO one-way passages for each adjacent pair.
    // Each passage’s label is simply the destination room’s name.
    //

    // Theater ↔ Suburbs
    Passage::createBasicPassage(rooms["Theater"].get(), rooms["Suburbs"].get(),        "Suburbs",              false);
    Passage::createBasicPassage(rooms["Suburbs"].get(), rooms["Theater"].get(),        "Theater",              false);

    // Theater ↔ Zoo
    Passage::createBasicPassage(rooms["Theater"].get(), rooms["Zoo"].get(),            "Zoo",                  false);
    Passage::createBasicPassage(rooms["Zoo"].get(),     rooms["Theater"].get(),        "Theater",              false);

    // Theater ↔ Sewer
    Passage::createBasicPassage(rooms["Theater"].get(), rooms["Sewer"].get(),          "Sewer",                false);
    Passage::createBasicPassage(rooms["Sewer"].get(),   rooms["Theater"].get(),        "Theater",              false);

    // Suburbs ↔ TV Station
    Passage::createBasicPassage(rooms["Suburbs"].get(), rooms["TV Station"].get(),     "TV Station",           false);
    Passage::createBasicPassage(rooms["TV Station"].get(), rooms["Suburbs"].get(),     "Suburbs",              false);

    // Suburbs ↔ Factory
    Passage::createBasicPassage(rooms["Suburbs"].get(), rooms["Factory"].get(),        "Factory",              false);
    Passage::createBasicPassage(rooms["Factory"].get(),  rooms["Suburbs"].get(),        "Suburbs",              false);

    // Suburbs ↔ Sewer
    Passage::createBasicPassage(rooms["Suburbs"].get(), rooms["Sewer"].get(),          "Sewer",                false);
    Passage::createBasicPassage(rooms["Sewer"].get(),   rooms["Suburbs"].get(),        "Suburbs",              false);

    // Zoo ↔ Subway Station
    Passage::createBasicPassage(rooms["Zoo"].get(),           rooms["Subway Station"].get(), "Subway Station", false);
    Passage::createBasicPassage(rooms["Subway Station"].get(), rooms["Zoo"].get(),         "Zoo",             false);

    // Zoo ↔ Back Streets
    Passage::createBasicPassage(rooms["Zoo"].get(),           rooms["Back Streets"].get(),   "Back Streets",    false);
    Passage::createBasicPassage(rooms["Back Streets"].get(),   rooms["Zoo"].get(),            "Zoo",             false);

    // TV Station ↔ Back Streets
    Passage::createBasicPassage(rooms["TV Station"].get(),     rooms["Back Streets"].get(),   "Back Streets",    false);
    Passage::createBasicPassage(rooms["Back Streets"].get(),   rooms["TV Station"].get(),     "TV Station",      false);

    // Factory ↔ Lab North Entrance
    Passage::createBasicPassage(rooms["Factory"].get(),        rooms["Lab North Entrance"].get(), "Lab North Entrance", false);
    Passage::createBasicPassage(rooms["Lab North Entrance"].get(), rooms["Factory"].get(),        "Factory",            false);

    // Factory ↔ Lab Underground Entrance
    Passage::createBasicPassage(rooms["Factory"].get(),        rooms["Lab Underground Entrance"].get(), "Lab Underground Entrance", false);
    Passage::createBasicPassage(rooms["Lab Underground Entrance"].get(), rooms["Factory"].get(),        "Factory",                  false);

    // Subway Station ↔ Lab Underground Entrance
    Passage::createBasicPassage(rooms["Subway Station"].get(), rooms["Lab Underground Entrance"].get(), "Lab Underground Entrance", false);
    Passage::createBasicPassage(rooms["Lab Underground Entrance"].get(), rooms["Subway Station"].get(),     "Subway Station",          false);

    // Subway Station ↔ Sewer
    Passage::createBasicPassage(rooms["Subway Station"].get(), rooms["Sewer"].get(),       "Sewer",           false);
    Passage::createBasicPassage(rooms["Sewer"].get(),            rooms["Subway Station"].get(), "Subway Station", false);

    // Lab North Entrance ↔ The Lab
    Passage::createBasicPassage(rooms["Lab North Entrance"].get(), rooms["The Lab"].get(),   "The Lab",          false);
    Passage::createBasicPassage(rooms["The Lab"].get(),            rooms["Lab North Entrance"].get(), "Lab North Entrance", false);

    // Lab North Entrance ↔ Lab Courtyard
    Passage::createBasicPassage(rooms["Lab North Entrance"].get(), rooms["Lab Courtyard"].get(), "Lab Courtyard", false);
    Passage::createBasicPassage(rooms["Lab Courtyard"].get(),       rooms["Lab North Entrance"].get(), "Lab North Entrance", false);

    // Lab Underground Entrance ↔ The Lab
    Passage::createBasicPassage(rooms["Lab Underground Entrance"].get(), rooms["The Lab"].get(), "The Lab",                  false);
    Passage::createBasicPassage(rooms["The Lab"].get(),              rooms["Lab Underground Entrance"].get(), "Lab Underground Entrance", false);

    // Lab Courtyard ↔ The Lab
    Passage::createBasicPassage(rooms["Lab Courtyard"].get(),       rooms["The Lab"].get(), "The Lab",      false);
    Passage::createBasicPassage(rooms["The Lab"].get(),             rooms["Lab Courtyard"].get(), "Lab Courtyard", false);
}

std::shared_ptr<Room> WorldManager::getStartingRoom() const {
    return rooms.at("Theater");
}

const std::map<std::string, std::shared_ptr<Room>>& WorldManager::getAllRooms() const {
    return rooms;
}
