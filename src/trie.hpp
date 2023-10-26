#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "./HashTable/hash_table.hpp"

template<typename T>
class TrieNode {
public:
	ht::HashTable<char, std::shared_ptr<TrieNode<T>>> children;
	std::vector<T> data;
};

template<typename T>
class Trie {
public:
	using Node = TrieNode<T>;
	using NodePtr = std::shared_ptr<Node>;

	// Trie() : root(new Node()) {}
	Trie() : root(std::make_shared<Node>(Node())) {}

	// Insert "word" in Trie and map "data" with it
	void insert(const std::string& word, const T& data) {
		size_t word_len = word.length(), i = 0;
		NodePtr crawl = root;

		while (i < word_len) {
			std::optional<NodePtr> next_lvl = crawl->children.getValue(word[i]);
			if (next_lvl.has_value()) {
				crawl = next_lvl.value();
				i++;
			} else {
				do {
					crawl->children.insert(word[i], std::make_shared<Node>(Node()));
					crawl = crawl->children.getValue(word[i]).value();
					i++;
				} while (i < word_len);
				break;
			}
		}
		crawl->data.emplace_back(data);
	}

	// Returns a vector with all the data that was mapped with words prefixed by "prefix"
	std::vector<T> search(const std::string& prefix) {
		size_t prefix_len = prefix.length();
		std::vector<T> result;
		NodePtr crawl = root;

		for (int i = 0; i < prefix_len; i++) {
			std::optional<NodePtr> next_lvl = crawl->children.getValue(prefix[i]);

			if (next_lvl.has_value()) {
				crawl = next_lvl.value();
			} else {
				std::cout << "No words prefixed with \"" << prefix << "\" were found\n";
				return result;
			}
		}
		collect(crawl, result);
		return result;
	}

private:
	NodePtr root;

	void collect(NodePtr crawl, std::vector<T>& result) {
		if (!crawl->data.empty()) {
			result.insert(std::end(result), std::begin(crawl->data), std::end(crawl->data));
		}
		std::vector<NodePtr> children = crawl->children.getAll();
		for (NodePtr n : children) {
			collect(n, result);
		}
	}
};