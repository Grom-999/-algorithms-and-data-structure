#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

class MinHeap {

	struct Node {
		long long int key;
		std::string value;

		Node() : key(0), value("") {}

		Node(long long int k, std::string val = "")
			: key(k), value(val) {
		}
	};

public:
	std::vector<Node> heap;
	std::unordered_map<long long int, size_t> keyToIndex;

	bool MinHeapEmpty() {
		if (heap.empty()) {
			return true;
		}

		return false;
	}

	void swap(int index1, int index2) {
		Node node1 = heap[index1];
		Node node2 = heap[index2];

		heap[index1] = node2;
		heap[index2] = node1;

		keyToIndex[node1.key] = index2;
		keyToIndex[node2.key] = index1;
	}

	void heapify(size_t i) {
		if (heap.empty() || !(i < heap.size())) {
			return;
		}

		size_t parent;
		while (i > 0) {
			parent = (i - 1) / 2;
			if (heap[i].key >= heap[parent].key) {
				break;
			}
			swap(i, parent);
			i = parent;
		}

		size_t size = heap.size();
		size_t smallest = i;
		size_t left = 2 * i + 1;
		size_t right = 2 * i + 2;

		if (left < size && heap[left].key < heap[smallest].key) {
			smallest = left;
		}

		if (right < size && heap[right].key < heap[smallest].key) {
			smallest = right;
		}

		if (smallest != i) {
			swap(i, smallest);
			heapify(smallest);
		}
	}



	bool Add(long long int key, const std::string& value) {

		if (keyToIndex.find(key) != keyToIndex.end()) {
			return false;
		}

		Node newNode(key, value);
		heap.push_back(newNode);

		int index = heap.size() - 1;
		keyToIndex[key] = index;

		while (index > 0) {
			size_t pi = (index - 1) / 2;
			if (heap[pi].key < heap[index].key) {
				return true;
			}
			swap(index, pi);
			index = pi;
		}

		return true;
	}

	bool Delete(long long int key) {

		if (MinHeapEmpty()) {
			return false;
		}

		auto it = keyToIndex.find(key);
		if (it == keyToIndex.end()) {
			return false;
		}

		int index = it->second;
		swap(index, heap.size() - 1);
		keyToIndex.erase(key);
		heap.pop_back();
		heapify(index);

		return true;
	}

	void Search(long long int key, std::ostream& os) {
		if (keyToIndex.find(key) != keyToIndex.end()) {
			size_t index = keyToIndex[key];
			os << "1 " + std::to_string(index) + " " + heap[index].value << std::endl;
			return;
		}

		os << "0" << std::endl;
	}

	bool Set(int key, const std::string& value) {
		if (keyToIndex.find(key) == keyToIndex.end()) {
			return false;
		}

		size_t index = keyToIndex[key];
		heap[index].value = value;

		return true;
	}

	bool Min(std::ostream& os) {
		if (MinHeapEmpty()) {
			return false;
		}

		os << heap[0].key << " 0 " << heap[0].value << std::endl;

		return true;
	}

	bool Max(std::ostream& os) {
		if (MinHeapEmpty()) {
			return false;
		}

		Node max = heap[0];
		for (const auto& node : heap) {
			if (node.key > max.key) {
				max = node;
			}
		}

		os << max.key << " " << keyToIndex[max.key] << " " << max.value << std::endl;

		return true;
	}


	bool Extract(std::ostream& os) {
		if (MinHeapEmpty()) {
			return false;
		}

		Node extracted = heap[0];
		swap(0, heap.size() - 1);
		keyToIndex.erase(heap.back().key);
		heap.pop_back();
		heapify(0);

		os << std::to_string(extracted.key) + " " + extracted.value << std::endl;
		return true;
	}

	void Print(std::ostream& os) {
		int layer = 1, i = 0;
		bool stop = false;

		while (!stop) {
			std::vector<std::string> ins(layer, "");
			int li = 0;
			int upper_bound = (i + layer < heap.size()) ? i + layer : heap.size();

			if (i + layer >= heap.size()) {
				stop = true;
				int it = heap.size() - i;
				while (it < layer) {
					ins[it] = "_";
					it++;
				}
			}

			while (i < upper_bound) {
				Node v = heap[i];
				if (i == 0) {
					ins[li] = "[" + std::to_string(v.key) + " " + v.value + "]";
				}
				else if (li == 0) {
					int parentIndex = (i - 1) / 2;
					ins[li] = "[" + std::to_string(v.key) + " " + v.value + " " + std::to_string(heap[parentIndex].key) + "]";
				}
				else {
					int parentIndex = (i - 1) / 2;
					ins[li] = "[" + std::to_string(v.key) + " " + v.value + " " + std::to_string(heap[parentIndex].key) + "]";
				}
				li++;
				i++;
			}

			for (const auto& str : ins) {
				os << str << " ";
			}
			os << std::endl;

			layer *= 2;
		}
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

	MinHeap a;

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
				if (a.Add(std::stoll(value.substr(0, value.find(' '))), value.substr((value.find(' ') + 1))) == false) {	// value : key
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
				if (a.Set(std::stoll(value.substr(0, value.find(' '))), value.substr((value.find(' ') + 1))) == false) {	// value : key
					std::cout << "error\n";
				}
			}
			else {
				std::cout << "error\n";
			}
		}
		else if (input.find("delete") == 0 && input.find(' ') == 6) {

			value = input.substr(7);

			if (CheckDeleteAndSearch(value)) {

				if (a.Delete(std::stoll(value)) == false) {	// value : key
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

				a.Search(std::stoll(value), std::cout);
			}
			else {
				std::cout << "error\n";
			}
		}
		else if (input == "min") {
			if (a.Min(std::cout) == false)
			{
				std::cout << "error\n";
			}
		}
		else if (input == "max") {
			if (a.Max(std::cout) == false)
			{
				std::cout << "error\n";
			}
		}
		else if (input == "extract") {

			if (a.Extract(std::cout) == false)
			{
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