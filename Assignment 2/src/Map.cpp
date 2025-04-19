#include "Map.h"

//////////////////////////////////////////////////
// Territory class function definitions
//////////////////////////////////////////////////

// Default constructor, allocates memory, but doesn't assign values
Territory::Territory()
	: name{new std::string}, numOfArmies{new int}, owner{new std::string} {
}

// Parameterized constructor, only the name can be set
// The rest is to be set after construction
Territory::Territory(std::string name)
	: name{new std::string{name}}, numOfArmies{new int}, owner{new std::string} {
}

// Copy constructor
Territory::Territory(const Territory& source)
	: name{new std::string{*source.name}}, numOfArmies{new int{*source.numOfArmies}}, owner{new std::string{*source.owner}} {

}

// Destructor, deallocates memory for all the pointer data members
Territory::~Territory() {
	delete name;
	delete numOfArmies;
	delete owner;
}

// Copy assignment operator overload
Territory& Territory::operator=(const Territory& rhs) {
	if (this != &rhs) {
		*name = *rhs.name;
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

// Getters and setters
std::string& Territory::getName() const {
	return *name;
}

int& Territory::getNumOfArmies() const {
	return *numOfArmies;
}

std::string& Territory::getOwner() const {
	return *owner;
}

void Territory::setNumOfArmies(int& numOfArmies) {
	*this->numOfArmies = numOfArmies;
}

void Territory::setOwner(std::string& owner) {
	*this->owner = owner;
}

//////////////////////////////////////////////////
// Map class function definitions
//////////////////////////////////////////////////

// Default constructor, allocates memory, but doesn't assign values
Map::Map() : mainMap{new std::unordered_map<Territory, std::unordered_set<Territory>>},
	continents{new std::unordered_map<std::string, std::unordered_set<Territory>>},
	isUndirected{new bool}, isValid{new bool} {

}

// Parameterized constructor
Map::Map(std::unordered_map<Territory, std::unordered_set<Territory>> mainMap, std::unordered_map<std::string, std::unordered_set<Territory>> continents) 
	: mainMap{new std::unordered_map<Territory, std::unordered_set<Territory>>{mainMap}},
	continents{new std::unordered_map<std::string, std::unordered_set<Territory>>{continents}},
	isUndirected{new bool{true}}, isValid{new bool{true}} {

}

// Copy constructor that uses a delegate constructor
Map::Map(const Map& source)
	: mainMap{new std::unordered_map<Territory, std::unordered_set<Territory>>{*source.mainMap}},
	continents{new std::unordered_map<std::string, std::unordered_set<Territory>>{*source.continents}},
	isUndirected{new bool{*source.isUndirected}}, isValid{new bool{*source.isValid}} {

}

// Destructor, deallocate memory for all the pointer data members
Map::~Map() {
	delete mainMap;
	delete continents;
}

// Copy assignment operator overload
Map& Map::operator=(const Map& rhs) {
	if (this != &rhs) {
		*mainMap = *rhs.mainMap;
		*continents = *rhs.continents;
	}
	return *this;
}

// Stream insertion operator overload
std::ostream& operator<<(std::ostream& os, const Map& obj) {
	if (!obj.getMainMap().empty()) {
		os << "[Territories]" << std::endl;
		for (const auto& territory : obj.getMainMap()) {
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
			os << continent.first << " : - ";
			for (const auto& territory : continent.second) {
				os << territory << " - ";
			}
			os << std::endl;
		}
	}
	return os;
}

// Getters
std::unordered_map<Territory, std::unordered_set<Territory>>& Map::getMainMap() const {
	return *mainMap;
}

std::unordered_map<std::string, std::unordered_set<Territory>>& Map::getContinents() const {
	return *continents;
}

// Depth-first-search to check if the main map is a graph that's connected and undirected (every connection is bidirectional)
void Map::dfsMainMap(const Territory& t, std::unordered_map<Territory, std::unordered_set<Territory>>& mainMap, std::unordered_set<Territory>& visited) {
	visited.insert(t);
	for (const Territory& neighbor : mainMap[t]) {
		// Checks to see if t's neighbor has t as a neighbor
		if (mainMap[neighbor].find(t) == mainMap[neighbor].end()) {
			std::cout << neighbor << " doesn't have " << t << " as an edge, but " << t << " has " << neighbor << " as an edge." << std::endl;
			*isUndirected = false;
		}
		if (visited.find(neighbor) == visited.end()) {
			dfsMainMap(neighbor, mainMap, visited);
		}
	}
}

// Depth-first-search to check if territories in a continent are connected to eachother
void Map::dfsContinents(const std::pair<std::string, std::unordered_set<Territory>>& continent,
	const Territory& t,
	std::unordered_map<Territory, std::unordered_set<Territory>>& mainMap,
	std::unordered_set<Territory>& visited) {
	visited.insert(t);
	for (const Territory& neighbor : mainMap[t]) {
		// skip any neighbors that are not in the continent, since we're checking connectivity between the continent's territories alone
		if (visited.find(neighbor) == visited.end() && continent.second.find(neighbor) != continent.second.end()) {
			dfsContinents(continent, neighbor, mainMap, visited);
		}
	}
}

// Validates :
// 1) the map is a connected graph (and undirected) 
// 2) continents are connected subgraphs 
// 3) each country belongs to one and only one continent.
bool Map::validate() {
	// Verify if the map is empty
	if (mainMap->empty()) {
		std::cerr << "Map is invalid - Empty map" << std::endl;
	}

	// Verify if the map is a connected undirected graph 
	std::unordered_set<Territory>* visited{new std::unordered_set<Territory>};
	dfsMainMap(mainMap->begin()->first, *mainMap, *visited);
	if (!(*isUndirected && visited->size() == mainMap->size())) {
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
		visited->clear();
		dfsContinents(continent, *continent.second.begin(), *mainMap, *visited);
		if (!(visited->size() == continent.second.size())) {
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
				if (std::any_of(continent1.second.begin(), continent1.second.end(), [&](Territory t) { return continent2.second.count(t) > 0; })) {
					std::cerr << "Invalid Continents - Territory is part of 2 or more continents" << std::endl;
					return false;
				}
			}
		}
	}

	// Verify that each territory belongs to a continent
	if (totalTerritories != mainMap->size()) {
		std::cerr << "Invalid Continents - Territory doesn't belong to a continent" << std::endl;
		return false;
	}
	return true;
}

//////////////////////////////////////////////////
// MapLoader class function definitions
//////////////////////////////////////////////////

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
				}
			}
			if (line != "[Territories]" || continents.empty()) {
				in_file.close();
				throw std::string{"Invalid map file - Either no continents or no territories section"};
			}
			while (std::getline(in_file, line)) {
				if (line == "") {
					line.clear();
					continue;
				}
				std::istringstream iss{line};

				std::string territory;
				std::getline(iss, territory, ',');
				Territory t{territory};

				std::string continent;
				for (size_t i = 0; i < 3; i++) {
					std::getline(iss, continent, ',');
				}
				if (continents.find(continent) == continents.end()) {
					in_file.close();
					throw std::string{"Invalid map file - Non-existant continent"};
				}
				std::string edge;
				std::unordered_set<Territory> t_edges;
				while (std::getline(iss, edge, ',')) {
					Territory t_e{edge};
					t_edges.insert(t_e);
				}

				map->getMainMap().insert(std::make_pair(t, t_edges));
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
	// Only validate map if it's valide, otherwise, delete the data
	if (isValid) {
		if (!map->validate()) {
			map->getContinents().clear();
			map->getMainMap().clear();
		}
	} else {
		map->getContinents().clear();
		map->getMainMap().clear();
	}
}

// Copy constructor, uses parametized constructor as delegate
MapLoader::MapLoader(const MapLoader& source) 
	: map{new Map(*source.map)} {

}

// Destructor, deallocate memory for all the pointer data members
MapLoader::~MapLoader() {
	delete map;
}

// Copy assignment operator
MapLoader& MapLoader::operator=(const MapLoader& rhs) {
	if (this != &rhs) {
		*map = *rhs.map;
	}
	return *this;
}

// stream insertion operator, just inserts the map object into the stream
std::ostream& operator<<(std::ostream& os, const MapLoader& obj) {
	os << obj.getMap();
	return os;
}

// Getter
Map& MapLoader::getMap() const {
	return *map;
}