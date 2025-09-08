#include "../includes/GapBuffer.hpp"
#include <cstdlib>
#include <iostream>

/** Used to pick a random index move moving the cursor
* @param size The size of the visibleText string
* @returns a size_t ranging from 0 to size - 1
*/
size_t	getRandomIndex(size_t size)
{
	return (rand() % size);
}

int main()
{
	srand(time(0));
	std::string sample = "It was the best of times, it was the wurst of times! It was Schnitzel Time!";
	std::cout << "The sample string is:\n" << sample << std::endl;
	std::cout << "Sample size: " << sample.size() << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Test 1 Single insert and print" << std::endl;
	try {
		GapBuffer array1;
		array1.insert('A');
		std::cout << array1.getVisibleText() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << "Test 2 Multiple inserts and print using sample string" << std::endl;
	try {
		GapBuffer array2;
		for (size_t i = 0; i < sample.size(); i++)
			array2.insert(sample[i]);
		std::cout << array2.getVisibleText() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << "Test 3 Multiple inserts and print, followed by remove and print using sample string" << std::endl;
	try {
		GapBuffer array3;
		for (size_t i = 0; i < sample.size(); i++)
			array3.insert(sample[i]);
		std::cout << array3.getVisibleText() << std::endl;
		array3.remove();
		std::cout << array3.getVisibleText() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << "Test 4 Multiple insert and print, followed by 5 removes and print using sample string" << std::endl;
	try {
		GapBuffer array4;
		for (size_t i = 0; i < sample.size(); i++)
			array4.insert(sample[i]);
		std::cout << array4.getVisibleText() << std::endl;
		array4.remove();
		array4.remove();
		array4.remove();
		array4.remove();
		array4.remove();
		std::cout << array4.getVisibleText() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << "Test 5 Multiple inserts and print, followed by moving cursor to a random index, multiple inserts and print using sample string and another string" << std::endl;
	try {
		GapBuffer array5;
		std::string other = "Peanut butter jelly time!";
		std::cout << "Sample Size: " << sample.size() << " Other Size: " << other.size() << std::endl;
		size_t	index = getRandomIndex(sample.size());
		for (size_t i = 0; i < sample.size(); i++)
			array5.insert(sample[i]);
		std::cout << array5.getVisibleText() << std::endl;
		std::cout << "Other string: " << other << std::endl;
		array5.setCursorPosition(index);
		for (size_t j = 0; j < other.size(); j++)
			array5.insert(other[j]);
		std::cout << array5.getVisibleText() << std::endl;
		std::cout << "Final Size: " << array5.getVisibleText().size() << std::endl;

	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Test 6 Multiple inserts and print, followed by moving cursor to a random index, 5 removes and print using sample string" << std::endl;
	try {
		GapBuffer array6;
		size_t	index = getRandomIndex(sample.size());
		for (size_t i = 0; i < sample.size(); i++)
			array6.insert(sample[i]);
		std::cout << array6.getVisibleText() << std::endl;
		array6.setCursorPosition(index);
		array6.remove();
		array6.remove();
		array6.remove();
		array6.remove();
		array6.remove();
		std::cout << array6.getVisibleText() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
