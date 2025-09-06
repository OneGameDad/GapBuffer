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
	srand(0);
	std::string sample = "It was the best of times, it was the wurst of times! It was Schnitzel Time!";
	std::cout << "The sample string is:\n" << sample << std::endl;
	std::cout << "Sample size: " << sample.size() << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	// Test 1 Single insert and print
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
	//Test 2 Multiple inserts and print
	try {
		GapBuffer array2;
		std::string test = "It was the best of times, it was the wurst of times! It was Schnitzel Time!";
		for (size_t i = 0; i < test.size(); i++)
			array2.insert(test[i]);
		std::cout << array2.getVisibleText() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	//Test 3 Multiple inserts and print, followed by remove and print
	try {
		GapBuffer array3;
		std::string test = "It was the best of times, it was the wurst of times! It was Schnitzel Time!";
		for (size_t i = 0; i < test.size(); i++)
			array3.insert(test[i]);	
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
	//Test 4 Multiple insert and print, followed by multiple removes and print
	try {
		GapBuffer array4;
		std::string test = "It was the best of times, it was the wurst of times! It was Schnitzel Time!";
		for (size_t i = 0; i < test.size(); i++)
			array4.insert(test[i]);	
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
	//Test 5 Multiple inserts and print, followed by moving cursor to a random index, multiple inserts and print
	try {
		GapBuffer array5;
		std::string test = "It was the best of times, it was the wurst of times! It was Schnitzel Time!";
		std::string other = "Peanut butter jelly time!";
		size_t	index = getRandomIndex(test.size());
		for (size_t i = 0; i < test.size(); i++)
			array5.insert(test[i]);	
		std::cout << array5.getVisibleText() << std::endl;
		array5.setCursorPosition(index);
		for (size_t j = 0; j < other.size(); j++)
			array5.insert(other[j]);
		std::cout << array5.getVisibleText() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	//Test 6 Multiple inserts and print, followed by moving cursor to a random index, multiple removes and print
	try {
		GapBuffer array6;
		std::string test = "It was the best of times, it was the wurst of times! It was Schnitzel Time!";
		size_t	index = getRandomIndex(test.size());
		for (size_t i = 0; i < test.size(); i++)
			array6.insert(test[i]);	
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
