#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "hash-table/hash-table.hpp"

template<typename T>
class TrieNode 
{
private:
	// Alias declarations
	using NodePtr = std::shared_ptr<TrieNode<T>>;

public:
	TrieNode() 
	{
	}

	TrieNode(const HashTable<char, NodePtr>& children, std::vector<T> data)
		: children(children), data(data)
	{
	}

	TrieNode(const HashTable<char, NodePtr>& children)
		: children(children)
	{
	}

	std::vector<T> getData() const 
	{
		return data;
	}

	HashTable<char, NodePtr> getChildren() const 
	{
		return children;
	}

	void setData(const std::vector<T>& data) 
	{
		this->data.clear();
		this->data = data;
	}

	void insertData(const T& data) {
		this->data.emplace_back(data);
	}

	void insertChildren(char key, NodePtr childNodePtr) {
		this->children[key] = childNodePtr;
	}

private:
	HashTable<char, NodePtr> children;
	std::vector<T> data;
};

template<typename T>
class Trie
{
private:
	// Alias declarations
	using Node = TrieNode<T>;
	using NodePtr = std::shared_ptr<Node>;

	using string = std::string;

public:
	Trie() 
		: root(std::make_shared<Node>(Node())) 
	{
	}

	// Insert "word" in trie and map "data" with it
	void insert(const string& word, const T& data) 
	{
		size_t wordLen = word.length(), i = 0;
		NodePtr pCrawl = root;

		while (i < wordLen) {

			HashTable<char, NodePtr> childNodes = pCrawl->getChildren();
			std::optional<NodePtr> nextLevelPtr = childNodes.getValue(word[i]);

			if (nextLevelPtr.has_value()) {
				pCrawl = nextLevelPtr.value();
				i++;
			} 
			else {
				do {
					pCrawl->insertChildren(word[i], std::make_shared<Node>(Node()));
					pCrawl = pCrawl->getChildren().getValue(word[i]).value();
					i++;
				} 
				while (i < wordLen);
				break;
			}
		}
		pCrawl->insertData(data);
	}

	// Returns a vector with all the data that was mapped with words 
	// prefixed by "prefix"
	std::vector<T> search(const string& prefix) 
	{
		size_t prefixLen = prefix.length();
		std::vector<T> result;
		NodePtr pCrawl = root;

		for (int i = 0; i < prefixLen; i++) {
			HashTable<char, NodePtr> childNodes = pCrawl->getChildren();
			std::optional<NodePtr> nextLevelPtr = childNodes.getValue(prefix[i]);

			if (nextLevelPtr.has_value()) {
				pCrawl = nextLevelPtr.value();
			} 
			else {
				std::cout << "No words prefixed with \"" << prefix << "\" were found.\n";
				return result;
			}
		}
		collect(pCrawl, result);
		return result;
	}

private:
	NodePtr root;

	void collect(NodePtr pCrawl, std::vector<T>& result) {
		std::vector<T> data = pCrawl->getData();
		if (!data.empty()) {
			result.insert(std::end(result), std::begin(data), std::end(data));
		}

		std::vector<NodePtr> children = pCrawl->getChildren().getValues();
		for (NodePtr n : children) {
			collect(n, result);
		}
	}
};