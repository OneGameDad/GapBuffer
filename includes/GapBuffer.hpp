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

#define STARTING_BUFFER_SIZE	10
#define STARTING_GAP_SIZE	6

class GapBuffer
{
private:
	char*	buffer_;
	size_t	bufferSize_;
	size_t	gapStart_;
	size_t	gapEnd_;
	size_t	filledIndices_;
	size_t	lastIndex_;

	//Buffer
	bool	isBufferFull();
	void	resizeBuffer();
	void	setBuffer(size_t index, char ch);
	void	recalculateDerivedInfo();
	size_t	getTailSize();
	void	zeroOutBuffer(char * buffer, size_t size);

	//Gap
	size_t	setGapEnd(size_t newSize);
	size_t	getGapSize();
	void	calculateFilledIndices();
	void	calculateLastIndex();
	void	relocateGapTo(size_t index);
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

	//Writing
	void	insert(char ch);
	void	remove();
	void	setCursorPosition(size_t index);

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
