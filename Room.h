#ifndef ZOORK_ROOM_H
#define ZOORK_ROOM_H

#include "Location.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

class Passage;
class Command;

class Room : public Location {
public:
    // Single‐argument constructor (name + description)
    Room(const std::string &name, const std::string &desc);

    // Three‐argument constructor (name, description, custom enter command)
    Room(const std::string &name, const std::string &desc, std::shared_ptr<Command> c);

    // Add an object the player can “look at”
    void addLookable(const std::string &name, const std::string &lookDesc);

    // Add an object the player can “search”
    void addSearchable(const std::string &name, const std::string &searchDesc);

    bool isLookable(const std::string &name) const;
    bool isSearchable(const std::string &name) const;

    std::string getLookDescription(const std::string &name) const;
    std::string getSearchDescription(const std::string &name) const;

    // Return a list of all lookable object names
    std::vector<std::string> getLookableNames() const;

    // Return a list of all searchable object names
    std::vector<std::string> getSearchableNames() const;

    // Passage‐related methods (no change)
    void addPassage(const std::string &label, std::shared_ptr<Passage> p);
    void removePassage(const std::string &label);
    std::shared_ptr<Passage> getPassage(const std::string &label);

    // Let engine iterate all adjacent passages
    std::map<std::string, std::shared_ptr<Passage>> getAllExits() const { return passageMap; }

private:
    std::map<std::string, std::shared_ptr<Passage>> passageMap;

    // Private maps for interactive objects
    std::map<std::string, std::string> lookables;    // name → detailed “look” description
    std::map<std::string, std::string> searchables;  // name → detailed “search” description
};

#endif //ZOORK_ROOM_H
