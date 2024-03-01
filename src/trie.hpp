#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "hash-table/hash-table.hpp"

template<typename T>
class Trie 
{
private:
	class Node 
	{
	public:
		HashTable<char, std::shared_ptr<Node>> children;
		std::vector<T> data;
	};

	// Alias declarations.
	using NodePtr = std::shared_ptr<Node>;
	using string = std::string;

public:
	Trie() 
		: root(std::make_shared<Node>(Node())) 
	{
	}

	// Insert "word" in trie and map "data" with it.
	void insert(const string& word, const T& data) 
	{
		size_t len = word.length(), i = 0;
		NodePtr pCrawl = root;

		while (i < len) {
			HashTable<char, NodePtr> children = pCrawl->children;
			std::optional<NodePtr> nextPtr = children.getValue(word[i]);

			if (nextPtr.has_value()) {
				pCrawl = nextPtr.value();
				i++;
			} 
			else {
				do {
					pCrawl->children.insert(word[i], std::make_shared<Node>(Node()));
					pCrawl = pCrawl->children.getValue(word[i]).value();
					i++;
				}
				while (i < len);
			}
		}
		pCrawl->data.emplace_back(data);
	}

	// Returns a vector with the data that was mapped with all of the words 
	// prefixed by "prefix".
	std::vector<T> search(const string& prefix) 
	{
		size_t len = prefix.length();
		std::vector<T> result;
		NodePtr pCrawl = root;

		for (int i = 0; i < len; i++) {
			HashTable<char, NodePtr> children = pCrawl->children;
			std::optional<NodePtr> nextPtr = children.getValue(prefix[i]);

			if (!nextPtr.has_value()) {
				std::cout << "No words prefixed with \"" << prefix << "\" were found.\n";
				return result;
			} 
			pCrawl = nextPtr.value();
		}
		collect(pCrawl, result);
		return result;
	}

	// Remove all of the words prefixed by "prefix" and returns the data that 
	// was mapped with it.
	std::vector<T> remove(const string& prefix) 
	{
		size_t len = prefix.length();
		std::vector<T> result;
		NodePtr pCrawl = root;

		for (int i = 0; i < len; i++) {
			HashTable<char, NodePtr> children = pCrawl->children;
			std::optional<NodePtr> nextPtr = children.getValue(prefix[i]);

			if (!nextPtr.has_value()) {
				std::cout << "No words prefixed with \"" << prefix << "\" were found.\n";
				return result;
			}
			pCrawl = nextPtr.value();
		}
		collectAndRemove(pCrawl, result);
		return result;
	}

private:
	NodePtr root;

	void collect(NodePtr pCrawl, std::vector<T>& result) 
	{
		std::vector<T> data = pCrawl->data;
		if (!data.empty()) 
			result.insert(std::end(result), std::begin(data), std::end(data));

		std::vector<NodePtr> children = pCrawl->children.getValues();
		for (NodePtr nodePtr : children) 
			collect(nodePtr, result);
	}

	void collectAndRemove(NodePtr pCrawl, std::vector<T>& result) 
	{
		std::vector<T> data = pCrawl->data;
		if (!data.empty()) 
			result.insert(std::end(result), std::begin(data), std::end(data));

		std::vector<NodePtr> children = pCrawl->children.getValues();
		for (NodePtr nodePtr : children) 
			collectAndRemove(nodePtr, result);

		pCrawl->data.clear();
		pCrawl.reset();
	}
};