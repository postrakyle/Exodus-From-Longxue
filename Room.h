#ifndef ZOORK_ROOM_H
#define ZOORK_ROOM_H

#include "Location.h"
#include <map>
#include <memory>
#include <string>

class Passage;

class Room : public Location {
public:
    Room(const std::string &, const std::string &);
    Room(const std::string &, const std::string &, std::shared_ptr<Command>);
    void addPassage(const std::string &, std::shared_ptr<Passage>);
    void removePassage(const std::string &);
    std::shared_ptr<Passage> getPassage(const std::string &);

    // *** Add this method ***
    std::map<std::string, std::shared_ptr<Passage>> getAllExits() const { return passageMap; }

protected:
    std::map<std::string, std::shared_ptr<Passage>> passageMap;
};

#endif //ZOORK_ROOM_H
