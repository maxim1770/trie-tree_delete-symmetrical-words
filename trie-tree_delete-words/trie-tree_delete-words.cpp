#include <iostream>
#include <cctype>
#include<string>
#include <Windows.h>
#include <istream>

#define ALPHABET_SIZE 33
#define ROOT_LETTER '\0'

using namespace std;

struct ValueContainer
{
private:
	int value;

public:
	ValueContainer(int value)
	{
		this->value = value;
	}

	int getValue()
	{
		return this->value;
	}

};

struct TrieNode
{
private:
	char letter;
	ValueContainer* value;
	TrieNode* parent;
	TrieNode** children;

	int findChildIndex(char letter)
	{
		for (int i = 0; i < ALPHABET_SIZE; i++)
		{
			TrieNode* node = this->children[i];
			if (node != nullptr && node->letter == letter)
				return i;
		}

		return -1;
	}

public:
	TrieNode(char letter, ValueContainer* value = nullptr)
	{
		this->letter = letter;
		this->value = value;
		this->parent = nullptr;
		this->children = new TrieNode * [ALPHABET_SIZE];

		for (int i = 0; i < ALPHABET_SIZE; i++)
		{
			this->children[i] = nullptr;
		}
	}


	TrieNode* findChild(char letter)
	{
		int index = this->findChildIndex(letter);

		return index > -1 ? this->children[index] : nullptr;
	}

	TrieNode* addChild(char letter, ValueContainer* value = nullptr)
	{
		for (int i = 0; i < ALPHABET_SIZE; i++)
		{
			if (!this->children[i])
			{
				TrieNode* node = new TrieNode(letter, value);
				node->parent = this;

				this->children[i] = node;

				return node;
			}
		}
		return nullptr;
	}

	void deleteChild(char letter)
	{
		int index = this->findChildIndex(letter);
		if (index == -1)
			return;


		delete this->children[index];
		this->children[index] = nullptr;

	}

	void deleteSelf()
	{
		if (this->parent)
			this->parent->deleteChild(this->letter);

	}

	bool hasChildren()
	{
		for (int i = 0; i < ALPHABET_SIZE; i++)
		{
			TrieNode* node = this->children[i];
			if (node != nullptr)
				return true;
		}

		return false;
	}

	void setValue(ValueContainer* value)
	{
		this->value = value;
	}

	ValueContainer* getValue()
	{
		return this->value;
	}

	TrieNode* getParent()
	{
		return this->parent;
	}

	char getLetter()
	{
		return this->letter;
	}
	void print(TrieNode* node, string str)
	{
		TrieNode* current;
		current = node;
		if (node != nullptr)
		{
			str = str + current->letter;
			for (int i = 0; i < ALPHABET_SIZE; i++)
			{
				if (current->children[i] != nullptr)
				{
					current = current->children[i];
					print(current, str);
					current = current->parent;
				}
			}
			if (current->getValue() != nullptr)
				cout << "\n" << str;
		}

	}
	bool check(char* set, int n, string str1)
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < str1.size(); j++)
			{
				if (set[i] == str1[j])
					return true;
			}
		}
		return false;
	}

	void count(TrieNode* node, int* c)
	{
		TrieNode* current;
		current = node;
		if (node != nullptr)
		{

			for (int i = 0; i < ALPHABET_SIZE; i++)
			{
				if (current->children[i] != nullptr)
				{
					current = current->children[i];
					count(current, c);
					current = current->parent;
				}
			}
			if (current->getValue() != nullptr)
				*c = *c + 1;
		}
	}

	bool check_polindrom(string str1)
	{
		str1 = str1.erase(0, 1);
		int len = str1.length();
		for (int i = 0; i < len / 2; i++)
		{
			if (str1[i] != str1[len - i - 1])
			{
				return false;
			}
		}
		return true;
	}

	void delSet(TrieNode* node, string* str, string str1)
	{
		TrieNode* current;
		current = node;
		if (node != nullptr)
		{
			str1 = str1 + current->letter;
			for (int i = 0; i < ALPHABET_SIZE; i++)
			{
				if (current->children[i] != nullptr)
				{
					current = current->children[i];
					delSet(current, str, str1);
					current = current->parent;
				}
			}
			if (current->getValue() != nullptr && check_polindrom(str1) == true)
			{
				int i = 0;
				while (str[i] != "0")
					i++;
				str[i] = str1;
				str1 = " ";
			}

		}
	}
};

struct TrieTree
{
private:
	TrieNode* root;


	TrieNode* getNode(string key)
	{
		TrieNode* node = this->root;

		for (int i = 0; i < key.size(); i++)
		{
			char letter = key[i];
			node = node->findChild(letter);
			if (!node)
				return nullptr;
		}

		return node;
	}

public:
	TrieTree()
	{
		this->root = new TrieNode(ROOT_LETTER);
	}

	ValueContainer* get(string key)
	{
		TrieNode* node = this->getNode(key);

		return node ? node->getValue() : nullptr;
	}

	bool set(string key, ValueContainer* value)
	{
		TrieNode* node = this->root;

		bool isNewBranch = false;

		for (int i = 0; i < key.size(); i++)
		{
			char letter = key[i];
			TrieNode* next = isNewBranch ? nullptr : node->findChild(letter);
			if (next)
				node = next;
			else
			{
				node = node->addChild(letter);
				isNewBranch = true;
			}
		}

		if (node)
			node->setValue(value);

		return node != nullptr;
	}

	bool remove(string key)
	{
		TrieNode* node = this->getNode(key);
		if (!node)
			return false;

		TrieNode* current = node;

		while (current && current->getLetter() != ROOT_LETTER)
		{
			TrieNode* parent = current->getParent();

			if (current->hasChildren())
			{
				if (current == node)
					current->setValue(nullptr);
				return true;
			}

			current->deleteSelf();
			current = parent;
		}

		return true;
	}
	void delSet(int* c, string* str)
	{
		string str1;
		root->delSet(root, str, str1);
	}
	void count(int* c)
	{
		root->count(root, c);
	}
	void print()
	{
		string str = " ";
		root->print(root, str);
	}
};

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int* c = new int[1];
	*c = 0;
	TrieTree tree;
	int i = 1;
	string word;

	cout << "Введите слова:";
	cin >> word;
	while (word != "0")
	{
		ValueContainer value(i);
		tree.set(word, &value);
		cin >> word;
		i++;
	}



	cout << "\n" << "Первоначальное дерево:";
	tree.count(c);
	string* str = new string[*c];
	string* str3 = new string[*c];
	for (int i = 0; i < *c; i++)
	{
		str[i] = "0";
	}
	tree.print();

	cout << "\n" << "Итог:";
	tree.delSet(c, str);
	for (int i = 0; i < *c; i++)
	{
		str3[i] = str[i];
		for (int j = 1; j < str[i].size(); j++)
		{
			str3[i][j - 1] = str[i][j];
		}
		str3[i].pop_back();
		tree.remove(str3[i]);
	}
	tree.print();
	cout << endl;
}


//уму
//мама
//лишил
//тащат
//лил
//ребер
//ангел
//пап
//еше
//нон
//зеркало