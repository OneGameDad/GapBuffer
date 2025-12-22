#include "../includes/GapBuffer.hpp"
#include "../includes/utf8GapBuffer.hpp"
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
	std::u8string u8sample = u8"It was the best of times, it was the wurst of times! It was Schnitzel Time!";
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
		
		utf8GapBuffer utf8array1;
		utf8array1.insert('A');
		std::cout << utf8array1.getStdString() << std::endl;
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

		utf8GapBuffer utf8array2;
		for (size_t i = 0; i < sample.size(); i++)
			utf8array2.insert(sample[i]);
		std::cout << utf8array2.getStdString() << std::endl;
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

		utf8GapBuffer utf8array3;
		for (size_t i = 0; i < sample.size(); i++)
			utf8array3.insert(sample[i]);
		std::cout << utf8array3.getStdString() << std::endl;
		std::cout << "Removing character:" <<std::endl;
		utf8array3.remove();
		std::cout << utf8array3.getStdString() << std::endl;
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
		
		utf8GapBuffer utf8array4;
		for (size_t i = 0; i < sample.size(); i++)
			utf8array4.insert(sample[i]);
		std::cout << utf8array4.getStdString() << std::endl;
		std::cout << "Removing characters:" << std::endl;
		utf8array4.remove();
		utf8array4.remove();
		utf8array4.remove();
		utf8array4.remove();
		utf8array4.remove();
		std::cout << utf8array4.getStdString() << std::endl;
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
		std::cout << "New Index: " << index << " Last Index: " << array5.getLastByteIndex() << std::endl;
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

		utf8GapBuffer utf8array5;
		std::cout << "Sample Size: " << sample.size() << " Other Size: " << other.size() << std::endl;
		index = 4;//getRandomIndex(sample.size());
		for (size_t i = 0; i < sample.size(); i++)
			utf8array5.insert(sample[i]);
		std::cout << utf8array5.getStdString() << std::endl;
		std::cout << "Other string: " << other << std::endl;
		utf8array5.setCursorPosition(index);
		tempIndex = index;
		std::cout << "New Index: " << index << " Last Index: " << utf8array5.getLastByteIndex() << std::endl;
		for (size_t j = 0; j < other.size(); j++)
		{
			//std::cout << array5
			utf8array5.insert(other[j]);
			tempIndex++;
		}
		std::cout << "String with second string inserted" << std::endl;
		std::cout << utf8array5.getStdString() << std::endl;
		std::cout << "Final Size: " << utf8array5.getStdString().size() << " Current Index: " << tempIndex << " Gap Size: " << utf8array5.getGapSize() << std::endl;
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
		std::cout << "New Index: " << index <<  " Last Index: " << array6.getLastByteIndex() << std::endl;
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

		utf8GapBuffer utf8array6;
		index = getRandomIndex(sample.size());
		for (size_t i = 0; i < sample.size(); i++)
			utf8array6.insert(sample[i]);
		std::cout << utf8array6.getStdString() << std::endl;
		std::cout << "New Index: " << index <<  " Last Index: " << utf8array6.getLastByteIndex() << std::endl;
		utf8array6.setCursorPosition(index);
		utf8array6.remove();
		utf8array6.remove();
		utf8array6.remove();
		utf8array6.remove();
		utf8array6.remove();
		std::cout << "Removing characters:" << std::endl;
		std::cout << utf8array6.getStdString() << std::endl;
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
		std::cout << "Current Index: " << array7.getGapStart() << " New Index: " << index << " Gap size: " << array7.getGapSize() << " Tail Start:" << array7.getTailStart() << " Last Index: " << array7.getLastByteIndex() << " Array Length: " << array7.getByteCount() << " Buffer Length: " << array7.getBufferSize() << std::endl;
		array7.setCursorPosition(index);
		array7.remove();
		array7.remove();
		array7.remove();
		array7.remove();
		array7.remove();
		std::cout << "Removing characters:" << std::endl;
		std::cout << array7.getVisibleText() << std::endl;
		index = getRandomIndex(sample.size());
		std::cout << "Current Index: " << array7.getGapStart() << " New Index: " << index << " Gap size: " << array7.getGapSize() << " Tail Start:" << array7.getTailStart() << " Last Index: " << array7.getLastByteIndex() << " Array Length: " << array7.getByteCount() << " Buffer Size: " << array7.getBufferSize() << std::endl;
		array7.setCursorPosition(index);
		for (size_t i = 0; i < other.size(); i++)
			array7.insert(other[i]);
		std::cout << "String with second string inserted" << std::endl;
		std::cout << array7.getVisibleText() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;

		utf8GapBuffer utf8array7;
		std::cout << "Sample Size: " << sample.size() << " Other Size: " << other.size() << std::endl;
		index = getRandomIndex(sample.size());
		for (size_t i = 0; i < sample.size(); i++)
			utf8array7.insert(sample[i]);
		std::cout << utf8array7.getStdString() << std::endl;
		std::cout << "Current Index: " << utf8array7.getGapStart() << " New Index: " << index << " Gap size: " << utf8array7.getGapSize() << " Tail Start:" << utf8array7.getTailStart() << " Last Index: " << utf8array7.getLastByteIndex() << " Array Length: " << utf8array7.getByteCount() << " Buffer Length: " << utf8array7.getBufferSize() << std::endl;
		utf8array7.setCursorPosition(index);
		utf8array7.remove();
		utf8array7.remove();
		utf8array7.remove();
		utf8array7.remove();
		utf8array7.remove();
		std::cout << "Removing characters:" << std::endl;
		std::cout << utf8array7.getStdString() << std::endl;
		index = getRandomIndex(sample.size());
		std::cout << "Current Index: " << utf8array7.getGapStart() << " New Index: " << index << " Gap size: " << utf8array7.getGapSize() << " Tail Start:" << utf8array7.getTailStart() << " Last Index: " << utf8array7.getLastByteIndex() << " Array Length: " << utf8array7.getByteCount() << " Buffer Size: " << utf8array7.getBufferSize() << std::endl;
		utf8array7.setCursorPosition(index);
		for (size_t i = 0; i < other.size(); i++)
			utf8array7.insert(other[i]);
		std::cout << "String with second string inserted" << std::endl;
		std::cout << utf8array7.getStdString() << std::endl;
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
		std::cout << "Current Index: " << array8.getGapStart() << " New Index: " << index << " Gap size: " << array8.getGapSize() << " Tail Start:" << array8.getTailStart() << " Last Index: " << array8.getLastByteIndex() << " Array Length: " << array8.getByteCount() <<  " Buffer Size: " << array8.getBufferSize() << std::endl;
		array8.setCursorPosition(index);
		for (size_t i = 0; i < other.size(); i++)
			array8.insert(other[i]);
		std::cout << "String with second string inserted" << std::endl;
		std::cout << array8.getVisibleText() << std::endl;
		index = getRandomIndex(sample.size());
		std::cout << "Current Index: " << array8.getGapStart() << " New Index: " << index << " Gap size: " << array8.getGapSize() << " Tail Start:" << array8.getTailStart() << " Last Index: " << array8.getLastByteIndex() << " Array Length: " << array8.getByteCount() << " Buffer Size: " << array8.getBufferSize() << std::endl;
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

		utf8GapBuffer utf8array8;
		std::cout << "Sample Size: " << sample.size() << " Other Size: " << other.size() << std::endl;
		index = getRandomIndex(sample.size());
		for (size_t i = 0; i < sample.size(); i++)
			utf8array8.insert(sample[i]);
		std::cout << utf8array8.getStdString() << std::endl;
		std::cout << "Current Index: " << utf8array8.getGapStart() << " New Index: " << index << " Gap size: " << utf8array8.getGapSize() << " Tail Start:" << utf8array8.getTailStart() << " Last Index: " << utf8array8.getLastByteIndex() << " Array Length: " << utf8array8.getByteCount() <<  " Buffer Size: " << utf8array8.getBufferSize() << std::endl;
		utf8array8.setCursorPosition(index);
		for (size_t i = 0; i < other.size(); i++)
			utf8array8.insert(other[i]);
		std::cout << "String with second string inserted" << std::endl;
		std::cout << utf8array8.getStdString() << std::endl;
		index = getRandomIndex(sample.size());
		std::cout << "Current Index: " << utf8array8.getGapStart() << " New Index: " << index << " Gap size: " << utf8array8.getGapSize() << " Tail Start:" << utf8array8.getTailStart() << " Last Index: " << utf8array8.getLastByteIndex() << " Array Length: " << utf8array8.getByteCount() << " Buffer Size: " << utf8array8.getBufferSize() << std::endl;
		utf8array8.setCursorPosition(index);
		utf8array8.remove();
		utf8array8.remove();
		utf8array8.remove();
		utf8array8.remove();
		utf8array8.remove();
		std::cout << "Removing characters:" << std::endl;
		std::cout << utf8array8.getStdString() << std::endl;
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
		size_t	index = array9.getVisibleText().size() + 5;
		std::cout << "Current Index: " << array9.getGapStart() << " New Index: " << index << " Gap size: " << array9.getGapSize() << " Tail Start:" << array9.getTailStart() << " Last Index: " << array9.getLastByteIndex() << " Array Length: " << array9.getByteCount() << " Buffer Size: " << array9.getBufferSize() << std::endl;
		array9.setCursorPosition(index);
		for (size_t i = 0; i < other.size(); i++)
			array9.insert(other[i]);
		std::cout << array9.getVisibleText() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;

		utf8GapBuffer utf8array9;
		std::cout << "Sample Size: " << sample.size() << " Other Size: " << other.size() << std::endl;
		for (size_t i = 0; i < sample.size(); i++)
			utf8array9.insert(sample[i]);
		std::cout << utf8array9.getStdString() << std::endl;
		index = utf8array9.getStdString().size() + 5;
		std::cout << "Current Index: " << utf8array9.getGapStart() << " New Index: " << index << " Gap size: " << utf8array9.getGapSize() << " Tail Start:" << utf8array9.getTailStart() << " Last Index: " << utf8array9.getLastByteIndex() << " Array Length: " << utf8array9.getByteCount() << " Buffer Size: " << utf8array9.getBufferSize() << std::endl;
		utf8array9.setCursorPosition(index);
		for (size_t i = 0; i < other.size(); i++)
			utf8array9.insert(other[i]);
		std::cout << utf8array9.getStdString() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << "Test 10 Multiple inserts and print, followed by removing 60 characters, and moving the cursor to a lower index" << std::endl;
	try {
		GapBuffer array10;
		std::cout << "Sample Size: " << sample.size() << std::endl;
		for (size_t i = 0; i < sample.size(); i++)
			array10.insert(sample[i]);
		std::cout << array10.getVisibleText() << std::endl;
		std::cout << "Current Index: " << array10.getGapStart() << " Gap size: " << array10.getGapSize() << " Tail Start:" << array10.getTailStart() << " Last Index: " << array10.getLastByteIndex() << " Array Length: " << array10.getByteCount() << " Buffer Size: " << array10.getBufferSize() << std::endl;
		for (size_t i = 0; i < 60; i++)
			array10.remove();
		size_t	index = 5;
		std::cout << "Current Index: " << array10.getGapStart() << " Gap size: " << array10.getGapSize() << " Tail Start:" << array10.getTailStart() << " Last Index: " << array10.getLastByteIndex() << " Array Length: " << array10.getByteCount() << " Buffer Size: " << array10.getBufferSize() << std::endl;
		array10.setCursorPosition(index);
		std::cout << array10.getVisibleText() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;

		utf8GapBuffer utf8array10;
		std::cout << "Sample Size: " << sample.size() << std::endl;
		for (size_t i = 0; i < sample.size(); i++)
			utf8array10.insert(sample[i]);
		std::cout << utf8array10.getStdString() << std::endl;
		std::cout << "Current Index: " << utf8array10.getGapStart() << " Gap size: " << utf8array10.getGapSize() << " Tail Start:" << utf8array10.getTailStart() << " Last Index: " << utf8array10.getLastByteIndex() << " Array Length: " << utf8array10.getByteCount() << " Buffer Size: " << utf8array10.getBufferSize() << std::endl;
		for (size_t i = 0; i < 60; i++)
			utf8array10.remove();
		index = 5;
		std::cout << "Current Index: " << utf8array10.getGapStart() << " Gap size: " << utf8array10.getGapSize() << " Tail Start:" << utf8array10.getTailStart() << " Last Index: " << utf8array10.getLastByteIndex() << " Array Length: " << utf8array10.getByteCount() << " Buffer Size: " << utf8array10.getBufferSize() << std::endl;
		utf8array10.setCursorPosition(index);
		std::cout << utf8array10.getStdString() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << "Test 11 Multiple inserts and print, followed by removing 60 characters, and moving the cursor to a lower index and inserting a second string" << std::endl;
	try {
		GapBuffer array11;
		std::string other = "Peanut butter jelly time!";
		std::cout << "Sample Size: " << sample.size() << " Other Size: " << other.size() << std::endl;
		for (size_t i = 0; i < sample.size(); i++)
			array11.insert(sample[i]);
		std::cout << array11.getVisibleText() << std::endl;
		std::cout << "Current Index: " << array11.getGapStart() << " Gap size: " << array11.getGapSize() << " Tail Start:" << array11.getTailStart() << " Last Index: " << array11.getLastByteIndex() << " Array Length: " << array11.getByteCount() << " Buffer Size: " << array11.getBufferSize() << std::endl;
		for (size_t i = 0; i < 60; i++)
			array11.remove();
		size_t	index = 5;
		std::cout << "Current Index: " << array11.getGapStart() << " Gap size: " << array11.getGapSize() << " Tail Start:" << array11.getTailStart() << " Last Index: " << array11.getLastByteIndex() << " Array Length: " << array11.getByteCount() << " Buffer Size: " << array11.getBufferSize() << std::endl;
		array11.setCursorPosition(index);
		std::cout << array11.getVisibleText() << std::endl;
		for (size_t i = 0; i < other.size(); i++)
			array11.insert(other[i]);
		std::cout << array11.getVisibleText() << std::endl;
		std::cout << "Current Index: " << array11.getGapStart() << " Gap size: " << array11.getGapSize() << " Tail Start:" << array11.getTailStart() << " Last Index: " << array11.getLastByteIndex() << " Array Length: " << array11.getByteCount() << " Buffer Size: " << array11.getBufferSize() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;

		utf8GapBuffer utf8array11;
		std::cout << "Sample Size: " << sample.size() << " Other Size: " << other.size() << std::endl;
		for (size_t i = 0; i < sample.size(); i++)
			utf8array11.insert(sample[i]);
		std::cout << utf8array11.getStdString() << std::endl;
		std::cout << "Current Index: " << utf8array11.getGapStart() << " Gap size: " << utf8array11.getGapSize() << " Tail Start:" << utf8array11.getTailStart() << " Last Index: " << utf8array11.getLastByteIndex() << " Array Length: " << utf8array11.getByteCount() << " Buffer Size: " << utf8array11.getBufferSize() << std::endl;
		for (size_t i = 0; i < 60; i++)
			utf8array11.remove();
		index = 5;
		std::cout << "Current Index: " << utf8array11.getGapStart() << " Gap size: " << utf8array11.getGapSize() << " Tail Start:" << utf8array11.getTailStart() << " Last Index: " << utf8array11.getLastByteIndex() << " Array Length: " << utf8array11.getByteCount() << " Buffer Size: " << utf8array11.getBufferSize() << std::endl;
		utf8array11.setCursorPosition(index);
		std::cout << utf8array11.getStdString() << std::endl;
		for (size_t i = 0; i < other.size(); i++)
			utf8array11.insert(other[i]);
		std::cout << utf8array11.getStdString() << std::endl;
		std::cout << "Current Index: " << utf8array11.getGapStart() << " Gap size: " << utf8array11.getGapSize() << " Tail Start:" << utf8array11.getTailStart() << " Last Index: " << utf8array11.getLastByteIndex() << " Array Length: " << utf8array11.getByteCount() << " Buffer Size: " << utf8array11.getBufferSize() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << "Test 12 Multiple inserts and print, followed by removing all characters, and moving the cursor to another index and inserting a second string" << std::endl;
	try {
		GapBuffer array12;
		std::string other = "Peanut butter jelly time!";
		std::cout << "Sample Size: " << sample.size() << " Other Size: " << other.size() << std::endl;
		for (size_t i = 0; i < sample.size(); i++)
			array12.insert(sample[i]);
		std::cout << array12.getVisibleText() << std::endl;
		std::cout << "Current Index: " << array12.getGapStart() << " Gap size: " << array12.getGapSize() << " Tail Start:" << array12.getTailStart() << " Last Index: " << array12.getLastByteIndex() << " Array Length: " << array12.getByteCount() << " Buffer Size: " << array12.getBufferSize() << std::endl;
		for (ssize_t i = static_cast<ssize_t>(array12.getByteCount()); i > -1; i--)
			array12.remove();
		size_t	index = 5;
		std::cout << "Current Index: " << array12.getGapStart() << " Gap size: " << array12.getGapSize() << " Tail Start:" << array12.getTailStart() << " Last Index: " << array12.getLastByteIndex() << " Array Length: " << array12.getByteCount() << " Buffer Size: " << array12.getBufferSize() << std::endl;
		array12.setCursorPosition(index);
		std::cout << array12.getVisibleText() << std::endl;
		for (size_t i = 0; i < other.size(); i++)
			array12.insert(other[i]);
		std::cout << array12.getVisibleText() << std::endl;
		std::cout << "Current Index: " << array12.getGapStart() << " Gap size: " << array12.getGapSize() << " Tail Start:" << array12.getTailStart() << " Last Index: " << array12.getLastByteIndex() << " Array Length: " << array12.getByteCount() << " Buffer Size: " << array12.getBufferSize() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;

		utf8GapBuffer utf8array12;
		std::cout << "Sample Size: " << sample.size() << " Other Size: " << other.size() << std::endl;
		for (size_t i = 0; i < sample.size(); i++)
			utf8array12.insert(sample[i]);
		std::cout << utf8array12.getStdString() << std::endl;
		std::cout << "Current Index: " << utf8array12.getGapStart() << " Gap size: " << utf8array12.getGapSize() << " Tail Start:" << utf8array12.getTailStart() << " Last Index: " << utf8array12.getLastByteIndex() << " Array Length: " << utf8array12.getByteCount() << " Buffer Size: " << utf8array12.getBufferSize() << std::endl;
		for (ssize_t i = static_cast<ssize_t>(utf8array12.getByteCount()); i > -1; i--)
			utf8array12.remove();
		index = 5;
		std::cout << "Current Index: " << utf8array12.getGapStart() << " Gap size: " << utf8array12.getGapSize() << " Tail Start:" << utf8array12.getTailStart() << " Last Index: " << utf8array12.getLastByteIndex() << " Array Length: " << utf8array12.getByteCount() << " Buffer Size: " << utf8array12.getBufferSize() << std::endl;
		utf8array12.setCursorPosition(index);
		std::cout << utf8array12.getStdString() << std::endl;
		for (size_t i = 0; i < other.size(); i++)
			utf8array12.insert(other[i]);
		std::cout << utf8array12.getStdString() << std::endl;
		std::cout << "Current Index: " << utf8array12.getGapStart() << " Gap size: " << utf8array12.getGapSize() << " Tail Start:" << utf8array12.getTailStart() << " Last Index: " << utf8array12.getLastByteIndex() << " Array Length: " << utf8array12.getByteCount() << " Buffer Size: " << utf8array12.getBufferSize() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << "Test 13 Multiple inserts of non-ASCII characters and print using sample string" << std::endl;
	try {
		std::u8string nonAscii = u8"äöüÄÖÜß åøæ ÅØÆ ñÑ çÇ šŠ žŽ øØ łŁ ďĎ ěĚ ğĞ İı Ελληνικά Русский العربية 中文 日本語 한국어 😀🚀✨";
		GapBuffer array13;
		for (size_t i = 0; i < nonAscii.size(); i++)
			array13.insert(nonAscii[i]);
		std::cout << array13.getVisibleText() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;

		utf8GapBuffer utf8array13;
		for (size_t i = 0; i < nonAscii.size(); i++)
			utf8array13.insert(nonAscii[i]);
		std::cout << utf8array13.getStdString() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << "Test 14 Paste functionality - insert content at cursor position" << std::endl;
	try {
		GapBuffer array14;
		std::string toInsert = "Beautiful ";
		for (size_t i = 0; i < sample.size(); i++)
			array14.insert(sample[i]);
		std::cout << "Initial content: " << array14.getVisibleText() << std::endl;
		array14.paste(toInsert, 6);
		std::cout << "After paste at position 6: " << array14.getVisibleText() << std::endl;
		std::cout << "Byte Count: " << array14.getByteCount() << " Char Count: " << array14.getCharCount() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;

		utf8GapBuffer utf8array14;
		std::u8string utf8ToInsert = u8"Schöne ";
		for (size_t i = 0; i < u8sample.size(); i++)
			utf8array14.insert(u8sample[i]);
		std::cout << "Initial content: " << utf8array14.getStdString() << std::endl;
		utf8array14.utf8paste(utf8ToInsert, 6);
		std::cout << "After utf8paste at position 6: " << utf8array14.getStdString() << std::endl;
		std::cout << "Byte Count: " << utf8array14.getByteCount() << " Char Count: " << utf8array14.getCharCount() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << "Test 15 DeleteSelection functionality - delete range of characters" << std::endl;
	try {
		GapBuffer array15;
		for (size_t i = 0; i < sample.size(); i++)
			array15.insert(sample[i]);
		std::cout << "Initial content: " << array15.getVisibleText() << std::endl;
		std::cout << "Byte Count: " << array15.getByteCount() << std::endl;
		array15.deleteSelection(4, 10);
		std::cout << "After deleteSelection(4, 10): " << array15.getVisibleText() << std::endl;
		std::cout << "Byte Count: " << array15.getByteCount() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;

		utf8GapBuffer utf8array15;
		for (size_t i = 0; i < u8sample.size(); i++)
			utf8array15.insert(u8sample[i]);
		std::cout << "Initial content: " << utf8array15.getStdString() << std::endl;
		std::cout << "Byte Count: " << utf8array15.getByteCount() << " Char Count: " << utf8array15.getCharCount() << std::endl;
		utf8array15.deleteSelection(4, 13);
		std::cout << "After deleteSelection(4, 13): " << utf8array15.getStdString() << std::endl;
		std::cout << "Byte Count: " << utf8array15.getByteCount() << " Char Count: " << utf8array15.getCharCount() << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	return (0);
}
