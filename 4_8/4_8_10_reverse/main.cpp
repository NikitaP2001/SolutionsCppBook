#include <iostream>
#include <iomanip>
#include <cstdlib>

#define ARR_W 25
#define ARR_H 25

int arr[ARR_H][ARR_W];

template <size_t rows, size_t cols>
void reverse_array(int (&arr)[rows][cols]);

int main()
{
	for (int i = 0; i < ARR_H; i++) {
		for (int j = 0; j < ARR_W; j++) {
			arr[i][j] = std::rand() % 100;
			std::cout << std::setw(4) << arr[i][j];
		}
		std::cout << std::endl;
	}			
	
	reverse_array(arr);		
	std::cout << std::endl;
	
	for (int i = 0; i < ARR_H; i++) {
		for (int j = 0; j < ARR_W; j++) {			
			std::cout << std::setw(4) << arr[i][j];
		}	
		std::cout << std::endl;		
	}
	
	return 0;
}

template <size_t rows, size_t cols>
void reverse_array(int (&arr)[rows][cols])
{
	for (int i = 0; i < rows / 2; i++) {		
		for (int j = 0; j < cols; j++) {
			int temp = arr[i][j];
			arr[i][j] = arr[rows-1-i][cols-1-j];
			arr[rows-1-i][cols-1-j] = temp;			
		}
	}
}