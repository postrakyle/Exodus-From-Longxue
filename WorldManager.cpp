#include "WorldManager.h"
#include "Passage.h"

WorldManager::WorldManager() {
    createRooms();
    connectRooms();
}

void WorldManager::createRooms() {
    #define ADD_ROOM(name, desc) rooms[name] = std::make_shared<Room>(name, desc);

    ADD_ROOM("Back Streets", "Shadowy back streets littered with debris.");
    ADD_ROOM("TV Station", "Ruined TV station, glass and cameras everywhere.");
    ADD_ROOM("Suburbs", "Quiet residential area, too quiet...");
    ADD_ROOM("Factory", "A decrepit factory, machines still and cold.");
    ADD_ROOM("Theater", "Grand old theater with velvet seats.");
    ADD_ROOM("Sewer", "Rank, dark sewers beneath the city.");
    ADD_ROOM("Zoo", "Abandoned zoo. Cages broken open.");
    ADD_ROOM("Subway Station", "Dusty subway platforms.");
    ADD_ROOM("Lab Underground Entrance", "A hidden entrance deep below the city.");
    ADD_ROOM("Lab North Entrance", "An imposing reinforced door to the lab.");
    ADD_ROOM("The Lab", "Sterile, abandoned corridors with flickering lights.");
    ADD_ROOM("Lab courtyard", "Overgrown yard, patrolled by silence.");
}

void WorldManager::connectRooms() {
    Passage::createBasicPassage(rooms["Theater"].get(), rooms["Suburbs"].get(), "to suburbs", true);
    Passage::createBasicPassage(rooms["Theater"].get(), rooms["Zoo"].get(), "to zoo", true);
    Passage::createBasicPassage(rooms["Theater"].get(), rooms["Sewer"].get(), "to sewers", true);

    Passage::createBasicPassage(rooms["Suburbs"].get(), rooms["Theater"].get(), "to theater", true);
    Passage::createBasicPassage(rooms["Suburbs"].get(), rooms["TV Station"].get(), "to tv station", true);
    Passage::createBasicPassage(rooms["Suburbs"].get(), rooms["Factory"].get(), "to factory", true);
    Passage::createBasicPassage(rooms["Suburbs"].get(), rooms["Sewer"].get(), "to sewers", true);

    Passage::createBasicPassage(rooms["Zoo"].get(), rooms["Theater"].get(), "to theater", true);
    Passage::createBasicPassage(rooms["Zoo"].get(), rooms["Subway Station"].get(), "to subway station", true);
    Passage::createBasicPassage(rooms["Zoo"].get(), rooms["Back Streets"].get(), "to back streets", true);

    Passage::createBasicPassage(rooms["Sewer"].get(), rooms["Theater"].get(), "to theater", true);
    Passage::createBasicPassage(rooms["Sewer"].get(), rooms["Suburbs"].get(), "to suburbs", true);
    Passage::createBasicPassage(rooms["Sewer"].get(), rooms["Subway Station"].get(), "to subway station", true);

    Passage::createBasicPassage(rooms["Back Streets"].get(), rooms["Zoo"].get(), "to zoo", true);
    Passage::createBasicPassage(rooms["Back Streets"].get(), rooms["TV Station"].get(), "to tv station", true);

    Passage::createBasicPassage(rooms["TV Station"].get(), rooms["Back Streets"].get(), "to back streets", true);
    Passage::createBasicPassage(rooms["TV Station"].get(), rooms["Suburbs"].get(), "to suburbs", true);

    Passage::createBasicPassage(rooms["Factory"].get(), rooms["Suburbs"].get(), "to suburbs", true);
    Passage::createBasicPassage(rooms["Factory"].get(), rooms["Lab North Entrance"].get(), "to lab north entrance", true);
    Passage::createBasicPassage(rooms["Factory"].get(), rooms["Lab Underground Entrance"].get(), "to lab underground entrance", true);

    Passage::createBasicPassage(rooms["Subway Station"].get(), rooms["Zoo"].get(), "to zoo", true);
    Passage::createBasicPassage(rooms["Subway Station"].get(), rooms["Lab Underground Entrance"].get(), "to lab underground entrance", true);
    Passage::createBasicPassage(rooms["Subway Station"].get(), rooms["Sewer"].get(), "to sewers", true);

    Passage::createBasicPassage(rooms["Lab North Entrance"].get(), rooms["Factory"].get(), "to factory", true);
    Passage::createBasicPassage(rooms["Lab North Entrance"].get(), rooms["The Lab"].get(), "to the lab", true);
    Passage::createBasicPassage(rooms["Lab North Entrance"].get(), rooms["Lab courtyard"].get(), "to lab courtyard", true);

    Passage::createBasicPassage(rooms["Lab Underground Entrance"].get(), rooms["Sewer"].get(), "to sewer", true);
    Passage::createBasicPassage(rooms["Lab Underground Entrance"].get(), rooms["Subway Station"].get(), "to subway station", true);
    Passage::createBasicPassage(rooms["Lab Underground Entrance"].get(), rooms["The Lab"].get(), "to the lab", true);
    Passage::createBasicPassage(rooms["Lab Underground Entrance"].get(), rooms["Factory"].get(), "to factory", true);

    Passage::createBasicPassage(rooms["Lab courtyard"].get(), rooms["Lab North Entrance"].get(), "to lab north entrance", true);
    Passage::createBasicPassage(rooms["Lab courtyard"].get(), rooms["The Lab"].get(), "to the lab", true);
}

std::shared_ptr<Room> WorldManager::getStartingRoom() const {
    return rooms.at("Theater");
}

const std::map<std::string, std::shared_ptr<Room>>& WorldManager::getAllRooms() const {
    return rooms;
}
