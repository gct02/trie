#include <fstream>

#include "trie.hpp"

using std::shared_ptr;
using std::vector;
using std::string;

class Player 
{
public:
	Player(int id, string name, string positions)
		: id(id), name(name), positions(positions) 
	{
	}

	int id;
	string name;
	string positions;
};

// Tokenize string
vector<string> split(const char* str, char sep = ',') 
{
	vector<string> result;

	do {
		const char* begin = str;

		while (*str != sep && *str)
			str++;

		result.push_back(string(begin, str));
	} 
	while (0 != *str++);

	return result;
}

int main() 
{
	// Testing trie.hpp

	std::ifstream test_file("./../../../test/players.csv");
	Trie<Player> trie;
	string line;

	// Get file header
	std::getline(test_file, line);

	while (std::getline(test_file, line)) {
		vector<string> tokens = split(line.c_str());

		int id = std::stoi(tokens[0]);
		string name = tokens[1];
		string positions = tokens[2];

		Player player(id, name, positions);
		trie.insert(name, player);
	}
	test_file.close();
	
	vector<Player> result = trie.search("Lionel");

	if (!result.empty()) {
		std::cout << "Results:\n";

		for (Player player : result) {
			std::cout << player.id << ", " << player.name << ", " << player.positions << "\n";
		}
	}

	return 0;
}