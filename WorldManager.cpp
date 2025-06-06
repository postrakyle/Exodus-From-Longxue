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
        "You stand amid crumbling storefronts and shattered streetlights. "
        "The pavement is cracked, and ash drifts through the empty avenues. "
        "Graffiti reads \"Longxue battles here\" in faded red paint. A battered red car is half-buried in rubble."
    );
    rooms["Back Streets"]->addLookable(
        "red car",
        "The red car’s windshield is spiderwebbed from bullet impacts. Its hood bears a crudely carved note: "
        "\"Ammo stash under engine—take what you need, but leave something behind. -Sergeant Liu\""
    );
    rooms["Back Streets"]->addSearchable(
        "red car",
        "You search the red car’s glove compartment and find a bloodstained note: "
        "\"PMC convoy spotted near the old bridge at dawn. Civilians fled east. -Corporal Zhang.\""
    );
    rooms["Back Streets"]->addLookable(
        "graffiti",
        "The graffiti scrawled on the corner wall says: \"They came in the night—no warning.\" Beneath it, a torn poster shows a smiling family."
    );
    rooms["Back Streets"]->addSearchable(
        "graffiti",
        "You scrape away some of the paint and find a folded scrap: \"Evacuate to Sector 7—rendezvous at 0400. -Citz. Lian\""
    );

    // TV Station
    ADD_ROOM("TV Station",
        "The TV Station’s facade is blackened by fire. Broken cameras and shattered monitors lie on the floor. "
        "Smoke still curls from a toppled broadcast desk. Faint static echoes through the hallway, as though the last signal never quite died."
    );
    rooms["TV Station"]->addLookable(
        "tv rack",
        "A charred rack of electronics stands half-collapsed. Ash coats the shelves where tapes once sat."
    );
    rooms["TV Station"]->addSearchable(
        "tv rack",
        "You search the rack and uncover a Lab Keycard tucked inside a melted plastic reel. It bears the insignia of Longxue BioTech."
    );
    rooms["TV Station"]->addLookable(
        "broadcast desk",
        "The desk’s control panel is still warm. A small photograph lies face-down in a puddle of spilled coffee."
    );
    rooms["TV Station"]->addSearchable(
        "broadcast desk",
        "You flip the photograph: a news anchor smiling beside a Chinese flag. On the back, scrawled: "
        "\"We tried to warn them. -Anchor Wei.\""
    );
    rooms["TV Station"]->addLookable(
        "broken monitors",
        "Fractured LCD screens flicker with static. Each shows a paused frame of a terrified reporter mid-scream."
    );
    rooms["TV Station"]->addSearchable(
        "broken monitors",
        "Behind a toppled monitor, you find a journal entry: \"Day 3: Soldiers stormed the studio. We broadcasted until the last moment. -Producer Chen.\""
    );

    // Suburbs
    ADD_ROOM("Suburbs",
        "Burned-out houses line a silent street. Three cars—a red car, a blue car, and a burnt-out car—sit in driveways. "
        "Window frames hang empty, and children’s toys lie abandoned on cracked porches. "
        "Wind whistles through a torn American flag draped over a mailbox."
    );
    rooms["Suburbs"]->addLookable(
        "blue car",
        "The blue car’s doors are ajar; the engine block has been stripped for parts. Inside, a child’s backpack lies open."
    );
    rooms["Suburbs"]->addSearchable(
        "blue car",
        "You search the blue car’s glove box and find a Government Issue 9mm Pistol wrapped in a laminated ID badge: \"Captain Wang, Military Police.\""
    );
    rooms["Suburbs"]->addLookable(
        "child’s backpack",
        "A small backpack lies on the backseat—its zipper broken. A faded cartoon sticker clings to one corner."
    );
    rooms["Suburbs"]->addSearchable(
        "child’s backpack",
        "Inside, you find a crumpled drawing: a family under a rainbow, signed \"Mei, Age 7.\""
    );
    rooms["Suburbs"]->addLookable(
        "burnt car",
        "A charred husk of a vehicle sits here, incinerated from top to bottom. The tires have melted into puddles of rubber."
    );
    rooms["Suburbs"]->addSearchable(
        "burnt car",
        "You sift through the ash in the trunk and find a singed note: \"We’re out of bullets—defend the children. -Lieutenant Gao.\""
    );
    rooms["Suburbs"]->addLookable(
        "flag",
        "The American flag is torn and blackened—its stars and stripes barely visible in the smoke-stained fabric."
    );
    rooms["Suburbs"]->addSearchable(
        "flag",
        "You lift the scorched flag and discover a hidden scrap of paper: \"Evacuate via subway—Tunnel 22. -City Council.\""
    );

    // Factory
    ADD_ROOM("Factory",
        "Inside the Factory, rusted assembly lines stand frozen. High overhead, a broken sign reads “Longxue Enterprises—Manufacturing.” "
        "A smashed crate labeled “Medical Supplies” lies half-open. Broken mannequins from a military surplus shipment slumped against a wall."
    );
    rooms["Factory"]->addLookable(
        "smashed crate",
        "The wooden crate is splintered. A red cross stenciled on the side is blackened by soot."
    );
    rooms["Factory"]->addSearchable(
        "smashed crate",
        "You rummage in the crate and pull out an AFAK Medkit."
    );
    rooms["Factory"]->addLookable(
        "mannequins",
        "The mannequins wear tattered military fatigues. One has a bullet hole in its chest."
    );
    rooms["Factory"]->addSearchable(
        "mannequins",
        "You check the pockets and find a single 5.56×45mm round taped to a boot: "
        "\"Keep moving—don’t stop. -Sergeant Wu.\""
    );
    rooms["Factory"]->addLookable(
        "assembly lines",
        "The conveyor belts are rusted. Plaques above them read “Chemical Division” in both Chinese and Japanese."
    );
    rooms["Factory"]->addSearchable(
        "assembly lines",
        "Beneath a conveyor bolt, you find a stained lab slip: \"Project: Cobalt-7. Virus incomplete—lockdown initiated. -Dr. Kuroda.\""
    );

    // Theater
    ADD_ROOM("Theater",
        "Grand velvet seats are shredded, and the stage is littered with debris. "
        "Echoes of an orchestra’s final notes linger in the air. A rifle case lies near the front row, and a shotgun rack rests by a broken chair."
    );
    rooms["Theater"]->addLookable(
        "rifle case",
        "A battered rifle case sits open. It looks like it was pried open in a hurry."
    );
    rooms["Theater"]->addSearchable(
        "rifle case",
        "You pore over the rifle case’s frayed lining and read a scratched manufacturer’s code—but no weapon is immediately visible."
    );
    rooms["Theater"]->addLookable(
        "shotgun rack",
        "A shotgun rack stands next to a shattered chair—one slot looks empty."
    );
    rooms["Theater"]->addSearchable(
        "shotgun rack",
        "You inspect the shotgun rack, noting the metal hooks where a shotgun once hung. It seems someone took the weapon recently."
    );
    rooms["Theater"]->addLookable(
        "stage curtain",
        "The once‐luxurious red curtain is torn. Behind it, a boarded‐up exit shows scorch marks."
    );
    rooms["Theater"]->addSearchable(
        "stage curtain",
        "You peel back part of the curtain and find a crumpled playbill: \"Tonight’s performance canceled—urgent military evacuation. -City Cultural Bureau.\""
    );
    rooms["Theater"]->addLookable(
        "tickets",
        "Scattered tickets on the floor bear the date: June 2, 2025—two weeks ago."
    );
    rooms["Theater"]->addSearchable(
        "tickets",
        "You pick up a ticket stub with scribbled on its back: \"Meet at main gate at dawn—plan B.\""
    );

    // Sewer
    ADD_ROOM("Sewer",
        "The stench of sewage hangs heavy. Water pools around broken pipes and discarded sandbags. "
        "Faint echoes suggest movement deeper in the tunnels."
    );
    rooms["Sewer"]->addLookable(
        "hatch",
        "The metal hatch is covered in grime. Its hinges are bent from repeated forced entry."
    );
    rooms["Sewer"]->addSearchable(
        "hatch",
        "You lift the hatch fully and find a discarded backpack containing a flashlight and a note: "
        "\"Do not trust patrols without crest. -Scout Li, Unit 7.\""
    );
    rooms["Sewer"]->addLookable(
        "sandbags",
        "Old sandbags ring parts of the tunnel, probably used as cover during a firefight."
    );
    rooms["Sewer"]->addSearchable(
        "sandbags",
        "You search behind the sandbags and uncover a single 7.62×39mm magazine filled with rounds."
    );
    rooms["Sewer"]->addLookable(
        "graffiti",
        "Someone scrawled \"Stay above ground\" in charcoal on the damp wall."
    );
    rooms["Sewer"]->addSearchable(
        "graffiti",
        "Under the charcoal, you find a hidden note: \"Escape hatch to Plaza B—it opens at 0600. -Medic Zhao.\""
    );

    // Zoo
    ADD_ROOM("Zoo",
        "Empty animal cages creak in the wind. Rusted feeding troughs sit empty. "
        "A tattered zoo map flutters on a broken post, showing zones now marked in red ink."
    );
    rooms["Zoo"]->addLookable(
        "clipboard",
        "The clipboard’s papers are waterlogged but still legible in parts. The top line reads \"Quarantine Perimeter\"."
    );
    rooms["Zoo"]->addSearchable(
        "clipboard",
        "You salvage a page: \"Missing: tiger #12. Quarantine zone breached. Evacuation order revoked. -Dr. Yang.\""
    );
    rooms["Zoo"]->addLookable(
        "feeding trough",
        "The trough is rusted and stained—no signs of recent animal activity."
    );
    rooms["Zoo"]->addSearchable(
        "feeding trough",
        "At the bottom, you find a vial labeled “sedative. Do not ingest.” and a smudged paw print beside it."
    );
    rooms["Zoo"]->addLookable(
        "zoo map",
        "The colorful zoo map lists animal locations. Someone circled \"Lab Entry\" in red marker."
    );
    rooms["Zoo"]->addSearchable(
        "zoo map",
        "You peel off the map and uncover a scrawled note: \"Secret tunnel under lion’s den—leads north. -Ranger Wu.\""
    );

    // Subway Station
    ADD_ROOM("Subway Station",
        "The station is eerily quiet, illuminated by flickering emergency lights. "
        "Bullet‐riddled posters advertise evacuation routes. A lone platform sign creaks overhead."
    );
    rooms["Subway Station"]->addLookable(
        "bulletin board",
        "The torn schedule hangs by a single nail, flapping in the stale breeze."
    );
    rooms["Subway Station"]->addSearchable(
        "bulletin board",
        "Behind the torn schedule, you discover a map of hidden tunnels—handwritten: \"Safe route to west exit.\""
    );
    rooms["Subway Station"]->addLookable(
        "subway schedule",
        "The schedule’s ink has bled. You can make out only: \"Last stop: Walled City. Next train canceled.\""
    );
    rooms["Subway Station"]->addSearchable(
        "subway schedule",
        "You find a pocketknife taped to the board. The handle is scratched with initials: \"G.W.\""
    );
    rooms["Subway Station"]->addLookable(
        "platform sign",
        "The sign overhead reads “Platform 3” but the rest is obscured by grime."
    );
    rooms["Subway Station"]->addSearchable(
        "platform sign",
        "Peeling away grime, you find a note stuck behind: \"Meet agent at platform shift—11:00 PM. -Source Xi.\""
    );

    // Lab Underground Entrance
    ADD_ROOM("Lab Underground Entrance",
        "A battered steel door stands ajar. Flickering lights spill out from below. "
        "A hazmat hazard symbol is spray-painted across the brick wall."
    );
    rooms["Lab Underground Entrance"]->addLookable(
        "security badge",
        "The badge bears the insignia of Longxue BioTech. The photo shows a stern-faced scientist."
    );
    rooms["Lab Underground Entrance"]->addSearchable(
        "security badge",
        "You pick up the badge: \"Research Lead: Dr. Han Li.\" Underneath: a smudged fingerprint."
    );
    rooms["Lab Underground Entrance"]->addLookable(
        "hazmat symbol",
        "The spray-painted biohazard symbol has blood spatters around it, as though someone was injured."
    );
    rooms["Lab Underground Entrance"]->addSearchable(
        "hazmat symbol",
        "You scrape away some paint and reveal a crumpled memo: \"Containment breach may be unnatural. -Lead Engineer Sun.\""
    );
    rooms["Lab Underground Entrance"]->addLookable(
        "flickering lights",
        "The lights overhead buzz loudly, casting long, jittery shadows."
    );
    rooms["Lab Underground Entrance"]->addSearchable(
        "flickering lights",
        "You see a scorched circuit panel—someone tried to disable power here. A charred wire dangles."
    );

    // Lab North Entrance
    ADD_ROOM("Lab North Entrance",
        "A massive barricade blocks the entry. Bullet holes pepper the steel plating. "
        "A small window has been welded shut. The ground around is littered with spent shell casings."
    );
    rooms["Lab North Entrance"]->addLookable(
        "lab report",
        "The report’s pages are stained and smudged. The title reads \"Longxue BioTech—Project Omega.\""
    );
    rooms["Lab North Entrance"]->addSearchable(
        "lab report",
        "You sift through the pages and find a Level-2 Lab Keycard inside an envelope stamped “CONFIDENTIAL.”"
    );
    rooms["Lab North Entrance"]->addLookable(
        "barricade",
        "The barricade is reinforced with sandbags and scrap metal. Rusted chains dangle from hooks."
    );
    rooms["Lab North Entrance"]->addSearchable(
        "barricade",
        "Behind a loose plank, you discover two rounds of 9mm ammo taped to a beam."
    );
    rooms["Lab North Entrance"]->addLookable(
        "shell casings",
        "Hundreds of empty shell casings crunch underfoot, as though a last stand took place here."
    );
    rooms["Lab North Entrance"]->addSearchable(
        "shell casings",
        "You spot a single casing marked “Longxue Corp.” —fitting, since this was their private militia’s last position."
    );

    // The Lab
    ADD_ROOM("The Lab",
        "White tile floors are cracked. Broken test tubes litter the counters. A flickering sign reads “Experiment Zone.”"
    );
    rooms["The Lab"]->addLookable(
        "digital console",
        "The console’s screen reads: \"Biohazard Containment Failure—Security Lockdown Engaged.\""
    );
    rooms["The Lab"]->addSearchable(
        "digital console",
        "You hack the console and retrieve a flash drive labeled \"Project Omega—Research Data.\" It might contain clues."
    );
    rooms["The Lab"]->addLookable(
        "test tubes",
        "Test tubes are shattered; some still hold a glowing green liquid that pulses faintly."
    );
    rooms["The Lab"]->addSearchable(
        "test tubes",
        "You collect one intact tube—its fluid might be a potent healing agent if used correctly."
    );
    rooms["The Lab"]->addLookable(
        "security cameras",
        "Broken security cameras dangle from the ceiling, their lenses cracked."
    );
    rooms["The Lab"]->addSearchable(
        "security cameras",
        "You check a camera feed log and find a timestamped entry: \"02:17—Unknown containment breach. -Technician Huang.\""
    );

    // Lab Courtyard
    ADD_ROOM("Lab Courtyard",
        "Overgrown weeds choke the cracked pavement. A rusted barrel smolders with ash, and the air smells of chemical residue."
    );
    rooms["Lab Courtyard"]->addLookable(
        "weathered journal",
        "The faded journal page is stained with coffee. The handwriting trembles."
    );
    rooms["Lab Courtyard"]->addSearchable(
        "weathered journal",
        "You retrieve the page: \"Day 12: Soldiers keep disappearing from the perimeter. We fear something else was unleashed. -Dr. Chen.\""
    );
    rooms["Lab Courtyard"]->addLookable(
        "smoldering barrel",
        "The barrel’s fire flickers weakly, illuminating rusty tools scattered around."
    );
    rooms["Lab Courtyard"]->addSearchable(
        "smoldering barrel",
        "Under the barrel’s lid, you find a half-empty canteen of water. The label reads ‘Field Issue—Expires 04/2025.’"
    );
    rooms["Lab Courtyard"]->addLookable(
        "weeds",
        "Thick weeds crack through the pavement, as though nature is reclaiming this forsaken place."
    );
    rooms["Lab Courtyard"]->addSearchable(
        "weeds",
        "You pull aside the weeds and discover footprints in dried mud—leading toward a collapsed ventilation shaft."
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
