#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <stdexcept>
#include <string>
#include <iostream>
#include <filesystem>
#include <cstring>
#include <utility>
#include <memory>

#define STARTING_BUFFER_SIZE	10
#define GAP_SIZE	6

class utf8GapBuffer
{
private:
	char8_t*	buffer_;
	size_t	bufferSize_;
	size_t	gapStart_;
	size_t	tailStart_;
	size_t	arrayLength_;
	size_t	arrayLastIndex_;

	//Buffer
	bool	isBufferFull();
	void	resizeBuffer();
	void	setBuffer(size_t index, char8_t ch);
	void	recalculateDerivedInfo();
	void	zeroOutBuffer(char8_t* buffer, size_t size);
	void	moveBytesToLowerIndices(size_t newIndex);
	void	moveBytesToHigherIndices(size_t newIndex);
	void	shiftTailBytesToHigherIndices(size_t newGapSize, size_t tailSize);
	//Gap
	size_t	setTailStart(size_t newSize);
	size_t	getTailSize();
	void	calculateArrayLength();
	void	calculateArrayLastIndex();
	void	relocateGapTo(size_t newIndex);
	void	resizeGap();
	void	shrinkGap();
	void	growGap();
	void	cleanGap();
public:
	utf8GapBuffer();
	utf8GapBuffer(std::u8string &newContent);
	~utf8GapBuffer();
	utf8GapBuffer(const utf8GapBuffer &copy);
	utf8GapBuffer &operator=(const utf8GapBuffer &copy);
	char8_t	&operator[](size_t index);
	char8_t	&operator[](size_t index) const;


	//Getters
	std::string	getVisibleText() const;
	std::u8string	getVisibleU8Text() const;
	size_t	getGapSize() const;
	size_t	getLastIndex() const;
	size_t	getTailStart() const;
	size_t	getArrayLength() const;
	size_t	getGapStart() const;
	size_t	getBufferSize() const;

	//Writing
	void	insert(char8_t ch);
	void	remove();
	void	setCursorPosition(size_t newIndex);
	void	deleteSelection(size_t start, size_t end);
	void	paste(std::u8string &newContent, size_t cursorPosition);

	class utf8GapBufferException: public std::exception
	{
	private:
		std::string what_;
	public:
		explicit utf8GapBufferException(const std::string &what_arg);
		virtual const char* what() const noexcept override;
	};
};
