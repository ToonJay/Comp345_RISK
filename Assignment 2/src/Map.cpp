#include "Map.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Territory class function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor, allocates memory, but doesn't assign values
Territory::Territory()
	: name{new std::string}, numOfArmies{new int}, owner{new std::string{"Neutral"}} {
	// std::cout << "Called Territory default constructor" << std::endl;
}

// Parameterized constructor, only the name can be set
// The rest is to be set after construction
Territory::Territory(std::string name)
	: name{new std::string{name}}, numOfArmies{new int}, owner{new std::string{"Neutral"}} {
	// std::cout << "Called Territory parameterized constructor" << std::endl;
}

// Copy constructor
Territory::Territory(const Territory& source)
	: name{new std::string{*source.name}}, numOfArmies{new int{*source.numOfArmies}}, owner{new std::string{*source.owner}} {
	// std::cout << "Called Territory copy constructor" << std::endl;
}

// Destructor, deallocates memory for all the pointer data members
Territory::~Territory() {
	delete name;
	delete numOfArmies;
	delete owner;
	// std::cout << "Called Territory destructor" << std::endl;
}

// Copy assignment operator overload
Territory& Territory::operator=(const Territory& rhs) {
	if (this != &rhs) {
		delete name;
		delete numOfArmies;
		delete owner;
		name = new std::string{*rhs.name};
		numOfArmies = new int{*rhs.numOfArmies};
		owner = new std::string{*rhs.owner};
	}
	return *this;
}

// Equality operator overload
bool Territory::operator==(const Territory& rhs) const {
	return *this->name == *rhs.name;
}

// Stream insertion operator overload
std::ostream& operator<<(std::ostream& os, const Territory& obj) {
	os << obj.getName();
	return os;
}

// Getters
std::string& Territory::getName() {
	return *name;
}

int& Territory::getNumOfArmies() {
	return *numOfArmies;
}

std::string& Territory::getOwner() {
	return *owner;
}


const std::string& Territory::getName() const {
	return *name;
}

const int& Territory::getNumOfArmies() const {
	return *numOfArmies;
}

const std::string& Territory::getOwner() const {
	return *owner;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Map class function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor, allocates memory, but doesn't assign values
Map::Map() : territories{new std::unordered_map<std::string, Territory>},
	gameMap{new std::unordered_map<Territory*, std::unordered_set<Territory*>>},
	continents{new std::unordered_map<std::string, std::unordered_set<Territory*>>},
	continentBonuses{new std::unordered_map<std::string, int>},
	isUndirected{new bool}, isValid{new bool} {
	// std::cout << "Called Map default constructor" << std::endl;
}

// Parameterized constructor
Map::Map(
	std::unordered_map<std::string, Territory> territories,
	std::unordered_map<Territory*, std::unordered_set<Territory*>> gameMap,
	std::unordered_map<std::string, std::unordered_set<Territory*>> continents,
	std::unordered_map<std::string, int> continentBonuses
) 
	: territories{new std::unordered_map<std::string, Territory>{territories}},
	gameMap{new std::unordered_map<Territory*, std::unordered_set<Territory*>>{gameMap}},
	continents{new std::unordered_map<std::string, std::unordered_set<Territory*>>{continents}},
	continentBonuses{new std::unordered_map<std::string, int>{continentBonuses}},
	isUndirected{new bool{true}}, isValid{new bool{true}} {
	// std::cout << "Called Map parameterized constructor" << std::endl;
}

// Copy constructor that uses a delegate constructor
Map::Map(const Map& source)
	: territories{new std::unordered_map<std::string, Territory>{*source.territories}},
	gameMap{new std::unordered_map<Territory*, std::unordered_set<Territory*>>{*source.gameMap}},
	continents{new std::unordered_map<std::string, std::unordered_set<Territory*>>{*source.continents}},
	continentBonuses{new std::unordered_map<std::string, int>{*source.continentBonuses}},
	isUndirected{new bool{*source.isUndirected}}, isValid{new bool{*source.isValid}} {
	// std::cout << "Called Map copy constructor" << std::endl;
}

// Destructor, deallocate memory for all the pointer data members
Map::~Map() {
	delete territories;
	delete gameMap;
	delete continents;
	delete continentBonuses;
	delete isUndirected;
	delete isValid;
	// std::cout << "Called Map destructor" << std::endl;
}

// Copy assignment operator overload
Map& Map::operator=(const Map& rhs) {
	if (this != &rhs) {
		delete territories;
		delete gameMap;
		delete continents;
		delete continentBonuses;
		delete isUndirected;
		delete isValid;
		territories = new std::unordered_map<std::string, Territory>{*rhs.territories};
		gameMap = new std::unordered_map<Territory*, std::unordered_set<Territory*>>{*rhs.gameMap};
		continents = new std::unordered_map<std::string, std::unordered_set<Territory*>>{*rhs.continents};
		continentBonuses = new std::unordered_map<std::string, int>{*rhs.continentBonuses};
		isUndirected = new bool{*rhs.isUndirected};
		isValid = new bool{*rhs.isValid};
	}
	return *this;
}

// Stream insertion operator overload
std::ostream& operator<<(std::ostream& os, const Map& obj) {
	if (!obj.getGameMap().empty()) {
		os << "[Territories]" << std::endl;
		for (const auto& territory : obj.getGameMap()) {
			os << territory.first << " : - ";
			for (const auto& adjacent_territory : territory.second) {
				os << adjacent_territory << " - ";
			}
			os << std::endl;
		}
	}
	if (!obj.getContinents().empty()) {
		os << std::endl << "[Continents]" << std::endl;
		for (const auto& continent : obj.getContinents()) {
			os << continent.first << " (" << obj.getContinentBonuses().at(continent.first) << ") : - ";
			for (const auto& territory : continent.second) {
				os << territory << " - ";
			}
			os << std::endl;
		}
	}
	return os;
}

// Getters
std::unordered_map<std::string, Territory>& Map::getTerritories() {
	return *territories;
}

std::unordered_map<Territory*, std::unordered_set<Territory*>>& Map::getGameMap() {
	return *gameMap;
}

std::unordered_map<std::string, std::unordered_set<Territory*>>& Map::getContinents() {
	return *continents;
}

std::unordered_map<std::string, int>& Map::getContinentBonuses() {
	return *continentBonuses;
}

const std::unordered_map<std::string, Territory>& Map::getTerritories() const {
	return *territories;
}

const std::unordered_map<Territory*, std::unordered_set<Territory*>>& Map::getGameMap() const {
	return *gameMap;
}

const std::unordered_map<std::string, std::unordered_set<Territory*>>& Map::getContinents() const {
	return *continents;
}

const std::unordered_map<std::string, int>& Map::getContinentBonuses() const {
	return *continentBonuses;
}

// Depth-first-search to check if the main map is a graph that's connected and undirected (every connection is bidirectional)
void Map::dfsgameMap(Territory* const t, std::unordered_map<Territory*, std::unordered_set<Territory*>>& gameMap, std::unordered_set<std::string>& visited) {
	visited.insert(t->getName());
	for (const auto& neighbor : gameMap[t]) {
		// Checks to see if t's neighbor has t as a neighbor
		if (gameMap[neighbor].find(t) == gameMap[neighbor].end()) {
			std::cout << neighbor << " doesn't have " << t << " as an edge, but " << t << " has " << neighbor << " as an edge." << std::endl;
			*isUndirected = false;
		}
		if (visited.find(neighbor->getName()) == visited.end()) {
			dfsgameMap(neighbor, gameMap, visited);
		}
	}
}

// Depth-first-search to check if territories in a continent are connected to eachother
void Map::dfsContinents(const std::pair<std::string, std::unordered_set<Territory*>>& continent,
	Territory* const t,
	std::unordered_map<Territory*, std::unordered_set<Territory*>>& gameMap,
	std::unordered_set<std::string>& visited) {
	visited.insert(t->getName());
	for (const auto& neighbor : gameMap[t]) {
		// skip any neighbors that are not in the continent, since we're checking connectivity between the continent's territories alone
		if (visited.find(neighbor->getName()) == visited.end() && continent.second.find(neighbor) != continent.second.end()) {
			dfsContinents(continent, neighbor, gameMap, visited);
		}
	}
}

// Validates :
// 1) the map is a connected graph (and undirected) 
// 2) continents are connected subgraphs 
// 3) each country belongs to one and only one continent.
bool Map::validate() {
	// Verify if the map is empty
	if (gameMap->empty()) {
		std::cerr << "Map is invalid - Empty map" << std::endl;
		return false;
	}

	// Verify if the map is a connected undirected graph 
	std::unordered_set<std::string> visited{};
	dfsgameMap(gameMap->begin()->first, *gameMap, visited);
	if (!(*isUndirected && visited.size() == gameMap->size())) {
		std::cerr << "Map is invalid - Not a connected undirected graph" << std::endl;
		return false;
	}

	// Verify if there are any continents
	if (continents->empty()) {
		std::cerr << "Map is invalid - No continents" << std::endl;
		return false;
	}

	// Verify if each continent is a connected subgraph
	for (const auto& continent : *continents) {
		visited.clear();
		dfsContinents(continent, *continent.second.begin(), *gameMap, visited);
		if (!(visited.size() == continent.second.size())) {
			std::cerr << "Continent is invalid - Not a connected subgraph" << std::endl;
			return false;
		}
	}

	std::unordered_set<std::string> verifiedContinents;
	std::size_t totalTerritories{0};
	// Verify that each territory only belongs to 1 continent
	for (const auto& continent1 : *continents) {
		verifiedContinents.insert(continent1.first);
		totalTerritories += continent1.second.size();
		for (const auto& continent2 : *continents) {
			if (verifiedContinents.find(continent2.first) == verifiedContinents.end()) {
				if (std::any_of(continent1.second.begin(), continent1.second.end(), [&](Territory* const t) { return continent2.second.count(t) > 0; })) {
					std::cerr << "Invalid Continents - Territory is part of 2 or more continents" << std::endl;
					return false;
				}
			}
		}
	}

	// Verify that each territory belongs to a continent
	if (totalTerritories != gameMap->size()) {
		std::cerr << "Invalid Continents - Territory doesn't belong to a continent" << std::endl;
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MapLoader class function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Parameterized constructor
// Creates Map object with data from a .map file
MapLoader::MapLoader(std::string filepath) 
	: map{new Map()} {
	std::ifstream in_file{filepath};
	std::unordered_set<std::string>continents;
	std::string line;
	bool isValid{true};
	if (in_file) {
		// if any of these exceptions are thrown, isValid becomes false and the data in the pointers get deleted
		// these are the errors that will cancel the creation of the map
		try {
			do {
				std::getline(in_file, line);
			} while (line != "[Continents]" && !in_file.eof());
			if (line != "[Continents]") {
				in_file.close();
				throw std::string{"Invalid map file - Either no continents or no territories section"};
			}
			while (line != "[Territories]" && !in_file.eof()) {
				std::getline(in_file, line);
				if (line != "[Territories]" && line != "") {
					continents.insert(line.substr(0, line.find('=')));
					map->getContinentBonuses()[line.substr(0, line.find('='))] = static_cast<int>(line.at(line.find('=') + 1)) - 48;
				}
			}
			if (line != "[Territories]" || continents.empty()) {
				in_file.close();
				throw std::string{"Invalid map file - Either no continents or no territories section"};
			}
			std::streampos pos = in_file.tellg();
			while (std::getline(in_file, line)) {
				if (line == "") {
					line.clear();
					continue;
				}
				std::istringstream iss{line};

				std::string territory;
				std::getline(iss, territory, ',');
				Territory t{territory};

				map->getTerritories().insert(std::make_pair(territory, t));
			}
			in_file.clear();
			in_file.seekg(pos);
			while (std::getline(in_file, line)) {
				if (line == "") {
					line.clear();
					continue;
				}
				std::istringstream iss{line};

				std::string territory;
				std::getline(iss, territory, ',');
				Territory* const t{&map->getTerritories().at(territory)};

				std::string continent;
				for (size_t i = 0; i < 3; i++) {
					std::getline(iss, continent, ',');
				}
				if (continents.find(continent) == continents.end()) {
					in_file.close();
					throw std::string{"Invalid map file - Non-existant continent"};
				}

				std::string edge;
				std::unordered_set<Territory*> t_edges;
				while (std::getline(iss, edge, ',')) {
					Territory* t_e{&map->getTerritories().at(edge)};
					t_edges.insert(t_e);
				}

				map->getGameMap().insert(std::make_pair(t, t_edges));
				map->getContinents()[continent].insert(t);
			}
			
		}
		catch (std::string& ex) {
			std::cout << ex << std::endl;
			isValid = false;
		}
	}
	in_file.close();
	// notify of the continents that had no territories, but continue with the map since
	// we can just remove them and maybe still have a valid map
	if (continents.size() != map->getContinents().size()) {
		std::cerr << "The following continents were removed since no territories were assigned to them: " << std::endl;
		for (const std::string& c : continents) {
			if (map->getContinents().find(c) == map->getContinents().end()) {
				std::cerr << c << std::endl;
			}
		}
		std::cerr << std::endl;
	}
	// If not valid, delete the data
	if (!isValid) {
		map->getContinents().clear();
		map->getGameMap().clear();
	}
	// std::cout << "Called Maploader parameterized constructor" << std::endl;
}

// Copy constructor, uses parameterized constructor as delegate
MapLoader::MapLoader(const MapLoader& source) 
	: map{new Map(*source.map)} {
	// std::cout << "Called Maploader copy constructor" << std::endl;
}

// Destructor, deallocate memory for all the pointer data members
MapLoader::~MapLoader() {
	delete map;
	// std::cout << "Called Maploader destructor" << std::endl;
}

// Copy assignment operator
MapLoader& MapLoader::operator=(const MapLoader& rhs) {
	if (this != &rhs) {
		delete map;
		map = new Map{*rhs.map};
	}
	return *this;
}

// stream insertion operator, just inserts the map object into the stream
std::ostream& operator<<(std::ostream& os, const MapLoader& obj) {
	os << obj.getMap();
	return os;
}

// Getter
Map& MapLoader::getMap() {
	return *map;
}

const Map& MapLoader::getMap() const {
	return *map;
}