#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <ctime>

class Record {
public:
	int count;
	int price;
	Record(int count, int price) : count{count}, price{price} {}
};

bool comp_count(const Record &rec1, const Record &rec2)
{
	return (rec1.count > rec2.count) ? true : false;
}

bool comp_price(const Record &rec1, const Record &rec2)
{
	return (rec1.price > rec2.price) ? true : false;
}

void out_data(std::vector<Record> &data)
{
	for (auto &rec : data)
	{
		std::cout << rec.price << std::setw(6) << rec.count << std::endl;
	}
	std::cout << std::string(9, '-') << std::endl;
	std::cout << "price" << std::setw(6) << "count" << std::endl;
}

int main()
{
	srand(time(NULL));
	std::vector<Record> data;
	for (int i = 0; i < 20; i++)
		data.push_back(Record(rand() % 1000, rand() % 1000));

	std::cout << std::endl << "Sort by price" << std::endl;
	std::sort(data.begin(), data.end(), comp_price);
	out_data(data);

	std::cout << std::endl << "Sort by count" << std::endl;
	std::sort(data.begin(), data.end(), comp_count);
	out_data(data);
}

