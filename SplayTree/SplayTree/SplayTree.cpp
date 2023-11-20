#include <iostream>
#include <string>
#include <queue>
#include <algorithm>

class SplayTree {

public:
	struct Node {
		long long int key;
		std::string value;
		Node* left;
		Node* right;
		Node* parent;

		Node() : key(0), value(""), left(nullptr), right(nullptr), parent(nullptr) {}

		Node(long long int k, std::string val = "", Node* l = nullptr, Node* r = nullptr, Node* p = nullptr)
			: key(k), value(val), left(l), right(r), parent(p) {
		}
		~Node() {
			delete left;
			delete right;
		}
	};

private:
	Node* root;

	void Rotate(Node* Parent, Node* Child) {
		if (Parent != nullptr){		
			Node* Grandparent = Parent->parent;
			if (Grandparent != nullptr) {
				if (Grandparent->left == Parent) {
					Grandparent->left = Child;
				}
				else {
					Grandparent->right = Child;
				}
			}

			if (Parent->left == Child) {
				Parent->left = Child->right;
				if (Child->right != nullptr) {
					Child->right->parent = Parent;
				}
				Child->right = Parent;
			}
			else {
				Parent->right = Child->left;
				if (Child->left != nullptr) {
					Child->left->parent = Parent;
				}
				Child->left = Parent;
			}

			Child->parent = Grandparent;
			Parent->parent = Child;
		}
		
	}

	void Splay(Node* Current) {
		Node* Parent = nullptr; 
		Node* Grandparent = nullptr;

		while (Current->parent != nullptr) {
			Parent = Current->parent;
			Grandparent = Parent->parent;

			root = Current;	

			if (Grandparent == nullptr) {
				// Zig case
				Rotate(Parent, Current);
			}
			else if (Grandparent->left == Parent && Parent->left == Current) {
				// Zig-Zig case
				Rotate(Grandparent, Parent);
				Rotate(Parent, Current);
			}
			else if (Grandparent->right == Parent && Parent->right == Current) {
				// Zig-Zig case 
				Rotate(Grandparent, Parent);
				Rotate(Parent, Current);
			}
			else {
				// Zig-Zag case
				Rotate(Parent, Current);
				Rotate(Grandparent, Current);
			}
		}
	}


	Node* Min(Node* Root) {
		Node* min = Root;

		while (min->left != nullptr) {
			min = min->left;
		}

		Splay(min);

		return min;
	}

	Node* Max(Node* Root) {
		Node* max = Root;

		while (max->right != nullptr) {
			max = max->right;
		}

		Splay(max);

		return max;
	}

	Node* Search(long long int key) {
		Node* searchElement = root;
		Node* closest = nullptr;

		while (searchElement != nullptr) {
			closest = searchElement;

			if (searchElement->key == key) {
				Splay(searchElement);
				return searchElement;
			}
			else if (searchElement->key < key) {
				searchElement = searchElement->right;
			}
			else {
				searchElement = searchElement->left;
			}
		}

		if (closest != nullptr) {
			Splay(closest);
		}

		return nullptr;
	}


public:

	SplayTree() : root(nullptr) {}

	virtual ~SplayTree() { delete root; }

	bool TreeEmpty() {
		if (root == nullptr)
		{
			return true;
		}
		return false;
	}

	const Node* GetMax() {

		Node* max = Max(root);

		if (max != nullptr)
		{
			return max;
		}
		return nullptr;


		
	}

	const Node* GetMin() {

		Node* min = Min(root);

		if (min != nullptr)
		{
			return min;
		}

		return nullptr;

	}

	const Node* GetSearch(long long int key) {
		Node* search = Search(key);

		if (search != nullptr)
		{
			return search;
		}

		return nullptr;

	}

	bool Add(const std::string& value, const long long int key) {

		 if(TreeEmpty()){
			 root = new Node(key, value);
			return true;
		 }

		 Node* searchPlace = root;
		 bool search = true;

		 while(search){
			 if (searchPlace->key == key){	
				 Splay(searchPlace);
				 return false;
			 }
			 if(searchPlace->key < key && searchPlace->right != nullptr){
				searchPlace = searchPlace->right;
			 }else if(searchPlace->key > key && searchPlace->left != nullptr){
				searchPlace = searchPlace->left;
			 }else if(searchPlace->right == nullptr || searchPlace->left == nullptr){
				search = false;
			 }	
		 }

		 Node* insertNode = new Node(key, value);
		 insertNode->parent = searchPlace;

		 if(searchPlace->key < insertNode->key){
			 searchPlace->right = insertNode;
		 }else if(searchPlace->key >= insertNode->key){
			 searchPlace->left = insertNode;
		 }

		 Splay(insertNode);

		 return true;

	}



	bool Set(const std::string& value, long long int key) {
		Node* tempNode = Search(key);
		if (tempNode != nullptr)
		{
			tempNode->value = value;
			Splay(tempNode);
			return true;
		}
		
		return false;
	}
	


	
	bool Delete(long long int key) {

		Node* removeElement = Search(key);

		if (removeElement != nullptr) {

			if (removeElement->left == nullptr && removeElement->right != nullptr) {
				root = removeElement->right;
				removeElement->right->parent = nullptr;
			}
			else if (removeElement->right == nullptr && removeElement->left != nullptr) {
				root = removeElement->left;
				removeElement->left->parent = nullptr;
			}
			else if (removeElement->left != nullptr && removeElement->right != nullptr){
				Node* maxLeftElement = Max(removeElement->left);
				maxLeftElement->right = removeElement->right;
				maxLeftElement->right->parent = maxLeftElement;
				root = maxLeftElement;
			}
			else {
				delete removeElement;
				root = nullptr;
				return true;
			}

			removeElement->left = nullptr;
			removeElement->right = nullptr;
			delete removeElement;
			return true;
		}

		return false;
	}

	void Print(std::ostream& os) {
		std::string s;
		
		if (TreeEmpty()) {
			std::cout << "_\n";
			return;
		}

		std::queue<Node*> nodes;
		nodes.push(root);

		int levelSize = 0;
		Node* current = nullptr;


		int numberOfFreeVertices = 0;

		while (!nodes.empty()) {
			levelSize = nodes.size();

			if (numberOfFreeVertices == levelSize) {
				break;
			}

			for (int i = 0; i < levelSize; i++) {
				current = nodes.front();
				nodes.pop();

				if (current != nullptr) {
					s+= "[" +  std::to_string(current->key) + " " + current->value;
					if (current->parent != nullptr) {
						s += " " + std::to_string(current->parent->key);
					}
					s += "]";

					if (i != levelSize - 1) {
						s += " ";
					}

					nodes.push(current->left);

					nodes.push(current->right);


					if (current->left == nullptr)
					{
						++numberOfFreeVertices;
					}

					if (current->right == nullptr)
					{
						++numberOfFreeVertices;
					}
				}
				else {
					s += "_";

					if (i != levelSize - 1) {
						s += " ";
					}

					nodes.push(nullptr);

					nodes.push(nullptr);

					++numberOfFreeVertices;

				} 
				
			}
			s += "\n";
		}
		os << s;
	}
	

	
};

bool CheckAddAndSet(const std::string& value) {

	if (value.empty())
	{
		return false;
	}

	if (value.find(' ') == std::string::npos)
	{
		return false;
	}

	if (std::count(value.begin(), value.end(), ' ') > 1)
	{
		return false;
	}

	for (int i = 0; i < value.find(' '); i++)
	{
		if ((value[i] < '0' || value[i] > '9') && value[i] != '-')
		{
			return false;
		}
	}

	return true;
}


bool CheckDeleteAndSearch(const std::string& value) {

	if (value.empty())
	{
		return false;
	}

	if (std::count(value.begin(), value.end(), ' ') > 0)
	{
		return false;
	}

	for (int i = 0; i < value.size(); i++)
	{
		if ((value[i] < '0' || value[i] > '9') && value[i] != '-')
		{
			return false;
		}
	}

	return true;
}


 
int main()
{
	 
	SplayTree a;

	std::string input;
	std::string value;


	while (std::getline(std::cin, input))
	{
		if (input.empty()) {
			continue;
		}

		if (input.find("add") == 0 && input.find(' ') == 3) {

			value = input.substr(4);

			if (CheckAddAndSet(value)) {
				if (a.Add(value.substr((value.find(' ') + 1)), std::stoll(value.substr(0, value.find(' ')))) == false) { // value : key
					std::cout << "error\n";
				}

			}
			else {
				std::cout << "error\n";
			}

		}
		else if (input.find("set") == 0 && input.find(' ') == 3) {

			value = input.substr(4);

			if (CheckAddAndSet(value)) {
				if (a.Set(value.substr((value.find(' ') + 1)), std::stoll(value.substr(0, value.find(' ')))) == false) {	// value : key
					std::cout << "error\n";
				}
			}
			else {
				std::cout << "error\n";
			}
		}
		else if (input.find("delete") == 0 && input.find(' ') == 6) {

			value = input.substr(7);

			if (CheckDeleteAndSearch(value) && !(a.TreeEmpty())) {
				if (a.Delete(std::stoll(value)) == false) {
					std::cout << "error\n";
				}
			}
			else {
				std::cout << "error\n";
			}

		}
		else if (input.find("search") == 0 && input.find(' ') == 6) {

			value = input.substr(7);

			if (CheckDeleteAndSearch(value)) {
				const auto res = a.GetSearch(std::stoll(value));
				if (res != nullptr) {
					std::cout << 1 << " " << res->value << "\n";

				}
				else {
					std::cout << "0\n";
				}
			}
			else {
				std::cout << "error\n";
			}
		}
		else if (input == "min") {

			if (!a.TreeEmpty()) {
				const auto res = a.GetMin();
				if (res != nullptr) {
					std::cout << res->key << " " << res->value << "\n";
				}
				else {
					std::cout << "error\n";
				}
			}
			else {
				std::cout << "error\n";
			}
		}
		else if (input == "max") {

			if (!a.TreeEmpty()) {

				const auto res =  a.GetMax();
				if (res != nullptr) {
					std::cout << res->key << " " << res->value << "\n";
				}
				else {
					std::cout << "error\n";
				}
			}
			else {
				std::cout << "error\n";
			}
		}
		else if (input == "print") {
			a.Print(std::cout);
		}
		else {
			std::cout << "error\n";
		}

	}
	


}