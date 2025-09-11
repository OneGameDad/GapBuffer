#include "../includes/GapBuffer.hpp"
#include <cstddef>
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
		std::cout << "Removing character:" <<std::endl;
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
		std::cout << "Removing characters:" << std::endl;
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
		size_t	index = 4;//getRandomIndex(sample.size());
		for (size_t i = 0; i < sample.size(); i++)
			array5.insert(sample[i]);
		std::cout << array5.getVisibleText() << std::endl;
		std::cout << "Other string: " << other << std::endl;
		array5.setCursorPosition(index);
		size_t tempIndex = index;
		for (size_t j = 0; j < other.size(); j++)
		{
			//std::cout << array5
			array5.insert(other[j]);
			tempIndex++;
		}
		std::cout << "String with second string inserted" << std::endl;
		std::cout << array5.getVisibleText() << std::endl;
		std::cout << "Final Size: " << array5.getVisibleText().size() << " Current Index: " << tempIndex << " Gap Size: " << array5.getGapSize() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
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
		std::cout << "Removing characters:" << std::endl;
		std::cout << array6.getVisibleText() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << "Test 7 Multiple inserts and print, followed by moving cursor to a random index, 5 removes and print, followed by moving the cursor to a random index and inserting a string, using sample string and other string" << std::endl;
	try {
		GapBuffer array7;
		std::string other = "Peanut butter jelly time!";
		std::cout << "Sample Size: " << sample.size() << " Other Size: " << other.size() << std::endl;
		size_t	index = getRandomIndex(sample.size());
		for (size_t i = 0; i < sample.size(); i++)
			array7.insert(sample[i]);
		std::cout << array7.getVisibleText() << std::endl;
		array7.setCursorPosition(index);
		array7.remove();
		array7.remove();
		array7.remove();
		array7.remove();
		array7.remove();
		std::cout << "Removing characters:" << std::endl;
		std::cout << array7.getVisibleText() << std::endl;
		index = getRandomIndex(sample.size());
		array7.setCursorPosition(index);
		for (size_t i = 0; i < other.size(); i++)
			array7.insert(other[i]);
		std::cout << "String with second string inserted" << std::endl;
		std::cout << array7.getVisibleText() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << "Test 8 Multiple inserts and print, followed by moving cursor to a random index, 5 removes and print, followed by moving the cursor to a random index and inserting a string, using sample string and other string" << std::endl;
	try {
		GapBuffer array8;
		std::string other = "Peanut butter jelly time!";
		std::cout << "Sample Size: " << sample.size() << " Other Size: " << other.size() << std::endl;
		size_t	index = getRandomIndex(sample.size());
		for (size_t i = 0; i < sample.size(); i++)
			array8.insert(sample[i]);
		std::cout << array8.getVisibleText() << std::endl;
		array8.setCursorPosition(index);
		for (size_t i = 0; i < other.size(); i++)
			array8.insert(other[i]);
		std::cout << "String with second string inserted" << std::endl;
		std::cout << array8.getVisibleText() << std::endl;
		index = getRandomIndex(sample.size());
		array8.setCursorPosition(index);
		array8.remove();
		array8.remove();
		array8.remove();
		array8.remove();
		array8.remove();
		std::cout << "Removing characters:" << std::endl;
		std::cout << array8.getVisibleText() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << "Test 9 Multiple inserts and print, followed by moving cursor to a beyond the last index and inserting a string, using sample string and other string" << std::endl;
	try {
		GapBuffer array9;
		std::string other = "Peanut butter jelly time!";
		std::cout << "Sample Size: " << sample.size() << " Other Size: " << other.size() << std::endl;
		for (size_t i = 0; i < sample.size(); i++)
			array9.insert(sample[i]);
		std::cout << array9.getVisibleText() << std::endl;
		size_t	index = getRandomIndex(sample.size());
		array9.setCursorPosition(index);
		array9.setCursorPosition(array9.getVisibleText().size() + 5);
		for (size_t i = 0; i < other.size(); i++)
			array9.insert(other[i]);
		std::cout << array9.getVisibleText() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
