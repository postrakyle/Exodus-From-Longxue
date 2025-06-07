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
        "Ash drifts through cracked pavement under shattered streetlights. A battered red car sits half-buried in rubble, faded graffiti stains a nearby wall, and a dead body slumps against the brick. Each holds its own secret."
    );
    rooms["Back Streets"]->addLookable(
        "red car",
        "The red car’s windshield is spiderwebbed from bullet impacts, and its hood bears a crudely carved note: "
        "\"Ammo stash under engine—take what you need, but leave something behind. -Sergeant Liu.\" "
        "Perhaps its glove compartment will say more."
    );
    rooms["Back Streets"]->addSearchable(
        "red car",
        "You open the glove compartment and find a bloodstained note: "
        "\"PMC convoy spotted near the old bridge at dawn. Civilians fled east. -Corporal Zhang.\""
    );
    rooms["Back Streets"]->addLookable(
        "graffiti",
        "Faded red paint spells out: \"They came in the night—no warning.\" Beneath it, a torn poster of a smiling family suggests someone tried to hold on to hope."
    );
    rooms["Back Streets"]->addSearchable(
        "graffiti",
        "Scraping away old paint reveals a folded scrap: "
        "\"Evacuate to Sector 7—rendezvous at 0400. -Citz. Lian\""
    );
    rooms["Back Streets"]->addLookable(
        "dead body",
        "A tattered uniform covers a corpse face-down, one hand clutching an Overwrite Card that glints unnaturally."
    );
    rooms["Back Streets"]->addSearchable(
        "dead body",
        "You search the pockets and pull out the Overwrite Card stamped with the KIRIKO BioTech seal."
    );

    // TV Station
    ADD_ROOM("TV Station",
        "The TV Station’s facade is blackened by fire. Charred electronics lie scattered, a toppled broadcast desk still emits heat, and broken monitors flicker with static—each a clue waiting to be unearthed."
    );
    rooms["TV Station"]->addLookable(
        "tv rack",
        "Ash-coated shelves once held broadcast tapes, now half-melted by the blaze."
    );
    rooms["TV Station"]->addSearchable(
        "tv rack",
        "Inside a melted reel you uncover a Lab Keycard bearing the insignia of Longxue BioTech."
    );
    rooms["TV Station"]->addLookable(
        "broadcast desk",
        "The control panel’s surface is still warm. A photograph lies face-down in a spill of stale coffee."
    );
    rooms["TV Station"]->addSearchable(
        "broadcast desk",
        "Flipping the photograph reveals an anchor smiling beside a flag, scrawled on the back: \"We tried to warn them. -Anchor Wei.\""
    );
    rooms["TV Station"]->addLookable(
        "broken monitors",
        "Cracked screens display frozen frames of terror. Their frames hide logs beneath."
    );
    rooms["TV Station"]->addSearchable(
        "broken monitors",
        "Behind one monitor you find a journal entry: \"Day 3: Soldiers stormed the studio. We broadcasted until the last moment. -Producer Chen.\""
    );

    // Suburbs
    ADD_ROOM("Suburbs",
        "Burned-out houses line a silent street. Three abandoned cars rust in driveways, a child’s backpack lies open on a seat, and a torn American flag flutters from a mailbox—each a remnant of normal life."
    );
    rooms["Suburbs"]->addLookable(
        "blue car",
        "Its doors hang open; the engine block stripped for parts."
    );
    rooms["Suburbs"]->addSearchable(
        "blue car",
        "Inside the glove box you find a 9mm pistol wrapped in an ID badge: \"Captain Wang, Military Police.\""
    );
    rooms["Suburbs"]->addLookable(
        "child’s backpack",
        "A faded cartoon sticker clings to the corner of its torn fabric."
    );
    rooms["Suburbs"]->addSearchable(
        "child’s backpack",
        "You find a crumpled drawing of a family under a rainbow, signed \"Mei, Age 7.\""
    );
    rooms["Suburbs"]->addLookable(
        "burnt car",
        "The charred shell pools rubber where its tires melted."
    );
    rooms["Suburbs"]->addSearchable(
        "burnt car",
        "Sifting ash in the trunk reveals a note: \"We’re out of bullets—defend the children. -Lieutenant Gao.\""
    );
    rooms["Suburbs"]->addLookable(
        "flag",
        "The scorched flag’s fabric hides something beneath its folds."
    );
    rooms["Suburbs"]->addSearchable(
        "flag",
        "You lift the flag and uncover a scrap: \"Evacuate via subway—Tunnel 22. -City Council.\""
    );

    // Factory
    ADD_ROOM("Factory",
        "Inside the Factory, rusted conveyor belts stand still under a cracked “Longxue Enterprises—Manufacturing” sign. A splintered medical crate and slumped mannequins hint at sudden panic."
    );
    rooms["Factory"]->addLookable(
        "smashed crate",
        "A blackened red cross marks its side."
    );
    rooms["Factory"]->addSearchable(
        "smashed crate",
        "Inside you find an AFAK Medkit."
    );
    rooms["Factory"]->addLookable(
        "mannequins",
        "Military fatigues hang loose on lifeless forms; one has a bullet hole."
    );
    rooms["Factory"]->addSearchable(
        "mannequins",
        "You discover a 5.56×45mm round taped to a boot: \"Keep moving—don’t stop. -Sergeant Wu.\""
    );
    rooms["Factory"]->addLookable(
        "assembly lines",
        "Plaques above read “Chemical Division” in Chinese and Japanese."
    );
    rooms["Factory"]->addSearchable(
        "assembly lines",
        "Under a loose bolt you find a lab slip: \"Project: Cobalt-7. Virus incomplete—lockdown initiated. -Dr. Kuroda.\""
    );

    // Theater
    ADD_ROOM("Theater",
        "Shredded velvet seats face a stage littered with debris. A rifle case lies open, a shotgun rack stands empty, and a torn curtain reveals scorch marks overhead."
    );
    rooms["Theater"]->addLookable(
        "rifle case",
        "Its lining is frayed and pried apart."
    );
    rooms["Theater"]->addSearchable(
        "rifle case",
        "You read a scratched code inside—but find no weapon."
    );
    rooms["Theater"]->addLookable(
        "shotgun rack",
        "Hooks sway where a shotgun once hung."
    );
    rooms["Theater"]->addSearchable(
        "shotgun rack",
        "Someone took the weapon recently."
    );
    rooms["Theater"]->addLookable(
        "stage curtain",
        "Torn fabric hangs heavy; scorch marks streak the wood behind it."
    );
    rooms["Theater"]->addSearchable(
        "stage curtain",
        "Peeling the fabric reveals a canceled playbill: \"Tonight’s performance canceled—urgent evacuation. -City Cultural Bureau.\""
    );
    rooms["Theater"]->addLookable(
        "tickets",
        "Scattered stubs bear the date June 2, 2025."
    );
    rooms["Theater"]->addSearchable(
        "tickets",
        "One stub reads: \"Meet at main gate at dawn—plan B.\""
    );

    // Sewer
    ADD_ROOM("Sewer",
        "The stench of sewage fills damp tunnels. A heavy hatch, piled sandbags, and charcoal graffiti on the walls each speak of past desperation."
    );
    rooms["Sewer"]->addLookable(
        "hatch",
        "Grime coats the metal, hinges bend at odd angles."
    );
    rooms["Sewer"]->addSearchable(
        "hatch",
        "Under it you find a backpack with a flashlight and a note: \"Do not trust patrols without crest. -Scout Li, Unit 7.\""
    );
    rooms["Sewer"]->addLookable(
        "sandbags",
        "Old bags ring a makeshift barricade."
    );
    rooms["Sewer"]->addSearchable(
        "sandbags",
        "You uncover a 7.62×39mm magazine tucked behind them."
    );
    rooms["Sewer"]->addLookable(
        "graffiti",
        "\"Stay above ground\" scrawled in charcoal on the damp wall."
    );
    rooms["Sewer"]->addSearchable(
        "graffiti",
        "Beneath the charcoal you find: \"Escape hatch to Plaza B—it opens at 0600. -Medic Zhao.\""
    );

    // Zoo
    ADD_ROOM("Zoo",
        "Empty cages creak in the wind. Rusted feeding troughs lie abandoned and a torn map flutters on a broken post—its markings hint at hidden paths."
    );
    rooms["Zoo"]->addLookable(
        "clipboard",
        "Waterlogged papers top it; the header reads \"Quarantine Perimeter.\""
    );
    rooms["Zoo"]->addSearchable(
        "clipboard",
        "You salvage a page: \"Missing: tiger #12. Zone breached. Evacuation revoked. -Dr. Yang.\""
    );
    rooms["Zoo"]->addLookable(
        "feeding trough",
        "Stained metal shows no sign of recent feeding."
    );
    rooms["Zoo"]->addSearchable(
        "feeding trough",
        "At the bottom you find a vial labeled “Sedative. Do not ingest.” and a smudged paw print."
    );
    rooms["Zoo"]->addLookable(
        "zoo map",
        "Colored zones are crossed out, and \"Lab Entry\" circled in red."
    );
    rooms["Zoo"]->addSearchable(
        "zoo map",
        "Peeling it off reveals: \"Secret tunnel under lion’s den—leads north. -Ranger Wu.\""
    );

    // Subway Station
    ADD_ROOM("Subway Station",
        "Flickering emergency lights reveal bullet-riddled posters and a rusted platform sign—each piece marking a frantic last train."
    );
    rooms["Subway Station"]->addLookable(
        "bulletin board",
        "A torn schedule hangs by one nail."
    );
    rooms["Subway Station"]->addSearchable(
        "bulletin board",
        "Behind it you discover a hand-drawn map: \"Safe route to west exit.\""
    );
    rooms["Subway Station"]->addLookable(
        "subway schedule",
        "Bled ink makes out: \"Last stop: Walled City. Next train canceled.\""
    );
    rooms["Subway Station"]->addSearchable(
        "subway schedule",
        "You find a pocketknife taped to the board, initials \"G.W.\" carved into the handle."
    );
    rooms["Subway Station"]->addLookable(
        "platform sign",
        "The sign overhead reads “Platform 3,” grime clinging to the edges."
    );
    rooms["Subway Station"]->addSearchable(
        "platform sign",
        "Wiping the grime reveals: \"Meet agent at platform shift—11:00 PM. -Source Xi.\""
    );

    // Lab Underground Entrance
    ADD_ROOM("Lab Underground Entrance",
        "A heavy steel door pulses with bioluminescent veins through slime and rust. Its glow promises something buried within."
    );
    rooms["Lab Underground Entrance"]->addLookable(
        "bright door",
        "Veins of sickly green light flicker across its surface."
    );
    rooms["Lab Underground Entrance"]->addSearchable(
        "bright door",
        "Prying at a seam you uncover an Overwrite Card wedged in the sludge."
    );

    // Lab North Entrance
    ADD_ROOM("Lab North Entrance",
        "A fortified barricade blocks your path. Bullet holes and empty shell casings speak of a desperate last stand."
    );
    rooms["Lab North Entrance"]->addLookable(
        "lab report",
        "Stained pages titled \"Project Onryō: Neural Digitization Test\" lie scattered."
    );
    rooms["Lab North Entrance"]->addSearchable(
        "lab report",
        "Inside an envelope stamped “CONFIDENTIAL” you find a Kiriko Lab Keycard."
    );
    rooms["Lab North Entrance"]->addLookable(
        "barricade",
        "Sandbags and scrap metal reinforce rusted chains."
    );
    rooms["Lab North Entrance"]->addSearchable(
        "barricade",
        "Behind a plank you discover two 9mm rounds taped to a beam."
    );
    rooms["Lab North Entrance"]->addLookable(
        "shell casings",
        "Hundreds of spent casings crunch underfoot."
    );
    rooms["Lab North Entrance"]->addSearchable(
        "shell casings",
        "One casing bears the stamp “Kiriko Corp.” —proof of their militia."
    );

    // The Lab
    ADD_ROOM("The Lab",
        "White tiles crack beneath shattered test tubes and a flickering “Experiment Zone” sign. Broken cameras dangle overhead."
    );
    rooms["The Lab"]->addLookable(
        "digital console",
        "Its screen flashes: \"Containment Failure—Lockdown Engaged.\""
    );
    rooms["The Lab"]->addSearchable(
        "digital console",
        "You hack it and retrieve a flash drive labeled \"Project Omega—Research Data.\""
    );
    rooms["The Lab"]->addLookable(
        "test tubes",
        "Glowing green liquid pulses faintly in the shattered glass."
    );
    rooms["The Lab"]->addSearchable(
        "test tubes",
        "You collect an intact tube; its fluid may be a potent healer."
    );
    rooms["The Lab"]->addLookable(
        "security cameras",
        "Cracked lenses point down at the chaos below."
    );
    rooms["The Lab"]->addSearchable(
        "security cameras",
        "You find a log entry: \"02:17—Unknown containment breach. -Technician Huang.\""
    );

    // Lab Courtyard
    ADD_ROOM("Lab Courtyard",
        "Overgrown weeds crack through the pavement. A rusted barrel smolders with chemical ash, and footprints weave through the ruins."
    );
    rooms["Lab Courtyard"]->addLookable(
        "weathered journal",
        "Coffee-stained pages tremble with faint handwriting."
    );
    rooms["Lab Courtyard"]->addSearchable(
        "weathered journal",
        "You read: \"Day 12: Soldiers keep disappearing. We fear something else was unleashed. -Dr. Chen.\""
    );
    rooms["Lab Courtyard"]->addLookable(
        "smoldering barrel",
        "A weak fire flickers inside a rusted drum."
    );
    rooms["Lab Courtyard"]->addSearchable(
        "smoldering barrel",
        "Under the lid is a half-empty field canteen labeled ‘Expires 04/2025.’"
    );
    rooms["Lab Courtyard"]->addLookable(
        "weeds",
        "Thick stalks bend under muddy footprints."
    );
    rooms["Lab Courtyard"]->addSearchable(
        "weeds",
        "Pulling them aside reveals a collapsed ventilation shaft below."
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