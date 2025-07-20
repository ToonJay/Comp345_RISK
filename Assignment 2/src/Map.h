#ifndef _MAP_H_
#define _MAP_H_
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>

/*
* Territory class
* Represents a node on the game map
*/
class Territory {
private:
	std::string* name;
	int* numOfArmies;
	std::string* owner;
public:
	//--Constructors--//
	// Default constructor
	Territory();
	// Parameterized constructor
	Territory(std::string name);
	// Copy constructor
	Territory(const Territory& source);
	// Destructor
	~Territory();

	//--Operator overloads--//
	// Copy assignment operator overload
	Territory& operator=(const Territory& rhs);
	// Equality
	bool operator==(const Territory& rhs) const;
	// Stream insertion
	friend std::ostream& operator<<(std::ostream& os, const Territory& obj);

	//--Getters--//
	const std::string& getName() const;
	const int& getNumOfArmies() const;
	const std::string& getOwner() const;

	//--Mutators--//
	void setOwner(const std::string& newOwner);
	void setNumOfArmies(int numOfArmies);
	void addArmies(int amount);
	void removeArmies(int amount);
};

/*
* Map class
* Contains the main map implemented as a connected graph with territories as nodes, and edges being adjacency between territories
* Also contains the continent map as connected subgraphs
* Attributes: gameMap, continents, isUndirected and isValid
*/ 
class Map {
private:
	std::unordered_map<std::string, Territory>* territories;
	std::unordered_map<Territory*, std::unordered_set<Territory*>>* gameMap;
	std::unordered_map<std::string, std::unordered_set<Territory*>>* continents;
	std::unordered_map<std::string, int>* continentBonuses;
	bool* isUndirected;
	bool* isValid;
public:
	//--Constructors--//
	// Default constructor
	Map();
	// Parameterized constructor
	Map(
		std::unordered_map<std::string, Territory> territories,
		std::unordered_map<Territory*, std::unordered_set<Territory*>> gameMap,
		std::unordered_map<std::string, std::unordered_set<Territory*>> continents,
		std::unordered_map<std::string, int> continentBonuses
	);
	// Copy constructor
	Map(const Map& source);
	// Destructor
	~Map();

	//--Operator overloads--//
	// Copy assignment operator overload
	Map& operator=(const Map& rhs);
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const Map& obj);

	//--Getters--//
	Territory& getTerritoryByName(const std::string& name);
	const std::unordered_map<std::string, Territory>& getTerritories() const;
	const std::unordered_map<Territory*, std::unordered_set<Territory*>>& getGameMap() const;
	const std::unordered_map<std::string, std::unordered_set<Territory*>>& getContinents() const;
	const std::unordered_map<std::string, int>& getContinentBonuses() const;

	//--Mutators--//
	void addTerritory(const std::string& name);
	void addContinent(const std::string& continent, int bonus);
	void addNeighbor(Territory* const territory, Territory* const neighbor);
	void assignToContinent(const std::string& continent, Territory* const t);

	// Validation function to see whether or not the game map is valid
	bool validate() const;
	void dfsgameMap(Territory* const t, std::unordered_map<Territory*, std::unordered_set<Territory*>>& gameMap, std::unordered_set<std::string>& visited) const;
	void dfsContinents(const std::pair<std::string, std::unordered_set<Territory*>>& continent,
		Territory* const t, 
		std::unordered_map<Territory*, std::unordered_set<Territory*>>& gameMap,
		std::unordered_set<std::string>& visited
	) const;
};

/*
* MapLoader class
* Creates the Map object by loading/reading a .map text file
*/ 
class MapLoader {
private:
	Map* map;
public:
	//--Constructors--//
	// Parameterized constructor (no default because you need a file to read from)
	MapLoader(std::string filepath);
	// Copy constructor
	MapLoader(const MapLoader& source);
	// Destructor
	~MapLoader();

	//--Operator overloads--//
	// Copy assignment operator overload
	MapLoader& operator=(const MapLoader& rhs);
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const MapLoader& obj);

	//--Getters--//
	Map& getMap();
	const Map& getMap() const;
};

// Custom specialization of std::hash injected in namespace std
// This is to make the Territory class hashable so it can be used for unordered_sets
template<>
struct std::hash<Territory> {
	std::size_t operator()(const Territory& t) const noexcept {
		return std::hash<std::string>{}(t.getName());
	}
};

#endif