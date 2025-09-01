#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <string>
#include <iostream>

#define STARTING_BUFFER_SIZE	1024
#define STARTING_GAP_SIZE	64

class GapBuffer
{
private:
	char*	buffer_;
	size_t	bufferSize_;
	size_t	gapStart_;
	size_t	gapSize_;
	size_t	gapEnd_;
	size_t	filledIndices_;
	size_t	lastIndex_;

	//Buffer
	bool	isBufferFull();
	void	resizeBuffer();

	//Gap
	void	calculateGapEnd();
	void	calculateGapSize();
	void	calculateFilledIndices();
	void	calculateLastIndex();
public:
	GapBuffer();
	~GapBuffer();
	GapBuffer(const GapBuffer &copy);
	GapBuffer &operator=(const GapBuffer &copy);
	char	&operator[](size_t index);
	char	&operator[](size_t index) const;

	void	relocateGapTo(size_t index);
	
	//Getters
	std::string	getVisibleText() const;
	
	//Writing
	void	insert(char ch);
	void	remove();
};
