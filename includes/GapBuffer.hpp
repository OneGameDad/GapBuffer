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

#define STARTING_BUFFER_SIZE	1024
#define GAP_SIZE	64

class GapBuffer
{
private:
	char*	buffer_;
	size_t	bufferSize_;
	size_t	gapStart_;
	size_t	tailStart_;
	size_t	arrayLength_;
	size_t	arrayLastIndex_;

	//Buffer
	bool	isBufferFull();
	void	resizeBuffer();
	void	setBuffer(size_t index, char ch);
	void	recalculateDerivedInfo();
	void	zeroOutBuffer(char * buffer, size_t size);
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
	GapBuffer();
	~GapBuffer();
	GapBuffer(const GapBuffer &copy);
	GapBuffer &operator=(const GapBuffer &copy);
	char	&operator[](size_t index);
	char	&operator[](size_t index) const;


	//Getters
	std::string	getVisibleText() const;
	size_t	getGapSize() const;
	size_t	getLastIndex() const;
	size_t	getTailStart() const;
	size_t	getArrayLength() const;
	size_t	getGapStart() const;
	size_t	getBufferSize() const;

	//Writing
	void	insert(char ch);
	void	remove();
	void	setCursorPosition(size_t newIndex);

	//Saving
	void	saveVisibleText(std::filesystem::path &filename) const;

	class GapBufferException: public std::exception
	{
	private:
		std::string what_;
	public:
		explicit GapBufferException(const std::string &what_arg);
		virtual const char* what() const noexcept override;
	};
};
