#include "Room.h"
#include "RoomDefaultEnterCommand.h"
#include "NullPassage.h"
#include <iostream>

//
// Constructor #1: name + description.
//   Uses the default “enter” command that just prints description.
//
Room::Room(const std::string &name, const std::string &desc)
    : Location(name, desc) {
    // By default, entering this room prints its description.
    enterCommand = std::make_shared<RoomDefaultEnterCommand>(this);
}

//
// Constructor #2: name + description + custom Command (for special behavior).
//
Room::Room(const std::string &name, const std::string &desc, std::shared_ptr<Command> c)
    : Location(name, desc, std::move(c)) {
    // If a specific Command is passed in, use that as the enter command.
}

//
// Add something the player can “look at” by name.
//
void Room::addLookable(const std::string &name, const std::string &lookDesc) {
    lookables[name] = lookDesc;
}

//
// Add something the player can “search” by name.
//
void Room::addSearchable(const std::string &name, const std::string &searchDesc) {
    searchables[name] = searchDesc;
}

bool Room::isLookable(const std::string &name) const {
    return lookables.find(name) != lookables.end();
}

bool Room::isSearchable(const std::string &name) const {
    return searchables.find(name) != searchables.end();
}

std::string Room::getLookDescription(const std::string &name) const {
    auto it = lookables.find(name);
    if (it != lookables.end()) {
        return it->second;
    }
    return "";
}

std::string Room::getSearchDescription(const std::string &name) const {
    auto it = searchables.find(name);
    if (it != searchables.end()) {
        return it->second;
    }
    return "";
}

std::vector<std::string> Room::getLookableNames() const {
    std::vector<std::string> names;
    names.reserve(lookables.size());
    for (const auto &kv : lookables) {
        names.push_back(kv.first);
    }
    return names;
}

std::vector<std::string> Room::getSearchableNames() const {
    std::vector<std::string> names;
    names.reserve(searchables.size());
    for (const auto &kv : searchables) {
        names.push_back(kv.first);
    }
    return names;
}

//
// Passage‐related methods:
//
void Room::addPassage(const std::string &label, std::shared_ptr<Passage> p) {
    passageMap[label] = std::move(p);
}

void Room::removePassage(const std::string &label) {
    passageMap.erase(label);
}

std::shared_ptr<Passage> Room::getPassage(const std::string &label) {
    if (passageMap.count(label)) {
        return passageMap[label];
    } else {
        std::cout << "You can’t go directly to \"" << label << "\" from here.\n";
        return std::make_shared<NullPassage>(this);
    }
}
