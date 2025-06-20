// --- Location.cpp ---
#include "Location.h"
#include <utility>

Location::Location(const std::string &n, const std::string &d)
    : GameObject(n, d), enterCommand(std::make_shared<NullCommand>(this)) {}

Location::Location(const std::string &n, const std::string &d, std::shared_ptr<Command> c)
    : GameObject(n, d), enterCommand(std::move(c)) {}

void Location::enter() { enterCommand->execute(); }
void Location::setEnterCommand(std::shared_ptr<Command> c) { enterCommand = std::move(c); }
