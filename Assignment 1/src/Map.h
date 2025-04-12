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
	// Constructors
	// Default constructor
	Territory();
	// Parameterized constructor
	Territory(std::string name);
	// Copy constructor
	Territory(const Territory& source);
	// Destructor
	~Territory();

	// Operator overloads
	// Copy assignment operator overload
	Territory& operator=(const Territory& rhs);
	// Equality
	bool operator==(const Territory& rhs) const;
	// Stream insertion
	friend std::ostream& operator<<(std::ostream& os, const Territory& obj);

	// Getters
	std::string& GetName() const;
	int& GetNumOfArmies() const;
	std::string& GetOwner() const;
	// Setters
	void SetNumOfArmies(int& numOfArmies);
	void SetOwner(std::string& owner);
};

/* 
* Map class
* Contains the main map implemented as a connected graph with territories as nodes, and edges being adjacency between territories
* Also contains the continent map as connected subgraphs
* Attributes: mainMap, continents, isUndirected and isValid
*/ 
class Map {
private:
	std::unordered_map<Territory, std::unordered_set<Territory>>* mainMap;
	std::unordered_map<std::string, std::unordered_set<Territory>>* continents;
	bool* isUndirected;
	bool* isValid;
public:
	// Constructors
	// Default constructor
	Map();
	// Parameterized constructor
	Map(std::unordered_map<Territory, std::unordered_set<Territory>> mainMap, std::unordered_map<std::string, std::unordered_set<Territory>> continents);
	// Copy constructor
	Map(const Map& source);
	// Destructor
	~Map();

	// Operator overloads
	// Copy assignment operator overload
	Map& operator=(const Map& rhs);
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const Map& obj);

	// Getters (no setters needed for this class)
	std::unordered_map<Territory, std::unordered_set<Territory>>& GetMainMap() const;
	std::unordered_map<std::string, std::unordered_set<Territory>>& GetContinents() const;

	// Validation function to see whether or not the game map is valid
	bool validate();
	void dfsMainMap(const Territory& t, std::unordered_map<Territory, std::unordered_set<Territory>>& mainMap, std::unordered_set<Territory>& visited);
	void dfsContinents(const std::pair<std::string, std::unordered_set<Territory>>& continent,
		const Territory& t, 
		std::unordered_map<Territory, std::unordered_set<Territory>>& mainMap,
		std::unordered_set<Territory>& visited);

	
};

/*
* MapLoader class
* Creates the Map object by loading/reading a .map text file
*/ 
class MapLoader {
private:
	Map* map;
public:
	// Constructors
	// Parameterized constructor (no default because you need a file to read from)
	MapLoader(std::string filepath);
	// Copy constructor
	MapLoader(const MapLoader& source);
	// Destructor
	~MapLoader();

	//Operator overloads
	// Copy assignment operator overload
	MapLoader& operator=(const MapLoader& rhs);
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const MapLoader& obj);

	// Getter
	Map& GetMap() const;
};

// Custom specialization of std::hash injected in namespace std
// This is to make the Territory class hashable so it can be used for unordered_sets
template<>
struct std::hash<Territory>
{
	std::size_t operator()(const Territory& t) const noexcept
	{
		std::size_t h1 = std::hash<std::string>{}(t.GetName());
		return h1;
	}
};

#endif