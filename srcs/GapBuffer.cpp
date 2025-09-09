#include "../includes/GapBuffer.hpp"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <utility>

GapBuffer::GapBuffer()
	: bufferSize_(STARTING_BUFFER_SIZE), gapStart_(0), arrayLength_(0), arrayLastIndex_(0)
{
	buffer_ = new char[bufferSize_];
	zeroOutBuffer(buffer_, bufferSize_);
	tailStart_ = setTailStart(GAP_SIZE);
}

GapBuffer::~GapBuffer()
{
	if (buffer_ != nullptr)
	{
		delete [] buffer_;
		buffer_ = nullptr;
	}
}

GapBuffer::GapBuffer(const GapBuffer &copy)
{
	*this = copy;
}

GapBuffer	&GapBuffer::operator=(const GapBuffer &copy)
{
if (this != &copy)
	{
		delete [] buffer_;
		buffer_ = copy.buffer_;
		bufferSize_ = copy.bufferSize_;
		gapStart_ = copy.gapStart_;
		tailStart_ = copy.tailStart_;
		arrayLength_ = copy.arrayLength_;
		arrayLastIndex_ = copy.arrayLastIndex_;
	}
	return (*this);
}

bool	GapBuffer::isBufferFull()
{
	if (arrayLastIndex_ == bufferSize_ - 1)
		return (true);
	return (false);
}

void	GapBuffer::resizeBuffer()
{
	size_t newBufferSize = bufferSize_ * 2;
	size_t gapSize = GAP_SIZE;
	size_t newGapEnd = gapStart_ + gapSize;
	char* newBuffer = new char[newBufferSize];
	zeroOutBuffer(newBuffer, newBufferSize);

	//Copy Head
	std::copy(buffer_, buffer_ + gapStart_, newBuffer);
	//Set Gap to Zeros
	for (size_t i = gapStart_; i < tailStart_; i++)
		newBuffer[i] = 0;
	//Copy Tail
	std::copy(buffer_ + tailStart_, buffer_ + bufferSize_, newBuffer + newGapEnd);

	delete [] buffer_;
	buffer_ = newBuffer;
	bufferSize_ = newBufferSize;
	tailStart_ = setTailStart(gapSize);
	assert(gapStart_ < tailStart_);
	recalculateDerivedInfo();
}

void	GapBuffer::zeroOutBuffer(char* buffer, size_t size)
{
	for (size_t i = 0; i < size; i++)
		buffer[i] = 0;
}

size_t	GapBuffer::setTailStart(size_t newSize)
{
	return (gapStart_ + newSize - 1);
}

size_t	GapBuffer::getGapSize()
{
	return (tailStart_ - gapStart_);
}

void	GapBuffer::calculateArrayLength()
{
	size_t count = 0;
	for (size_t i = 0; i < bufferSize_; i++)
	{
		if (buffer_[i] != 0)
			count++;
	}
	arrayLength_ = count;
}

void	GapBuffer::calculateArrayLastIndex()
{
	size_t headSize = gapStart_;
	size_t tailSize = getTailSize();
	if (tailSize != 0)
		arrayLastIndex_ = gapStart_ + getGapSize() + tailSize;
	else
		arrayLastIndex_ = headSize;
}

void	GapBuffer::recalculateDerivedInfo()
{
	calculateArrayLength();
	calculateArrayLastIndex();
}

void	GapBuffer::setBuffer(size_t index, char ch)
{
	buffer_[index] = ch;
	shrinkGap();
}

void	GapBuffer::insert(char ch)
{
	assert(gapStart_ < bufferSize_);
	setBuffer(gapStart_, ch);
}

void	GapBuffer::remove()
{
	if (gapStart_ == 0 || arrayLength_ == 0)
		return ;
	growGap();
}

void GapBuffer::relocateGapTo(size_t index)
{
//	std::cout << "String before Relocation:" << std::endl;
//	std::cout << getVisibleText() << std::endl;
	size_t gapSize = getGapSize();
	if (index == gapStart_)
		return ;
	if (index < gapStart_)
	{
	//	calculateArrayLastIndex();
	//	if (index > arrayLastIndex_ && arrayLastIndex_ < gapStart_)
	//		index = arrayLastIndex_ + 1;
		std::copy_backward(buffer_ + index,
		     buffer_ + gapStart_,
		     buffer_ + gapStart_ + gapSize);
		gapStart_ = index;
		tailStart_ = setTailStart(gapSize);
	}
	else if (index > gapStart_)
	{
		calculateArrayLastIndex();
		if (index > arrayLastIndex_)
			index = arrayLastIndex_ + 1;
		else
			index = index - gapStart_ + gapSize;
		std::copy(buffer_ + tailStart_/* + 1*/,
			buffer_ + tailStart_ /*+ 1 */+ (index/* - gapStart_*/),
			buffer_ + gapStart_);
		gapStart_ = index;
		tailStart_ = setTailStart(gapSize);
	}
	cleanGap();
//	std::cout << "String after Relocation:" << std::endl;
//	std::cout << getVisibleText() << std::endl;
}

size_t	GapBuffer::getTailSize()
{
	return (arrayLength_ -  gapStart_);
}

void	GapBuffer::resizeGap()
{
	size_t newGapSize = GAP_SIZE;
	size_t tailSize = getTailSize();
	if (tailSize != 0 && (gapStart_ + newGapSize + tailSize >= bufferSize_ - 1))
	{
		resizeBuffer();
		return ;
	}
	memmove(&buffer_[tailStart_ + newGapSize], &buffer_[tailStart_], tailSize);
	tailStart_ += newGapSize - 1;
	cleanGap();
}

void	GapBuffer::shrinkGap()
{
	gapStart_++;
	recalculateDerivedInfo();
	if (isBufferFull())
		resizeBuffer();
	if (getGapSize() == 0)
		resizeGap();
}

void	GapBuffer::growGap()
{
	gapStart_--;
	buffer_[gapStart_] = 0;
	recalculateDerivedInfo();
}

void	GapBuffer::cleanGap()
{
	for (size_t i = gapStart_; i < tailStart_; i++)
		buffer_[i] = 0;
}

std::string	GapBuffer::getVisibleText() const
{
	std::string visible;
	if (arrayLength_ == 0)
		return (visible);
	size_t count = 0;
	for(size_t i = 0; i < bufferSize_; i++)
	{
		if (buffer_[i] != 0)
		{
			if (buffer_[i] == ' ')//TODO remove
			{
				visible.push_back('_');
				count++;
			}
			else
			{
				visible.push_back(buffer_[i]);
				count++;
			}
		}
	}
//	std::cout << "Array Length: " << arrayLength_ << " & Counted characters: " << count << " Visible Text Size: " << visible.size() << std::endl;
	if (count != arrayLength_)
		throw GapBufferException("The number of filled Indices does not match the number of visible characters");
	return (visible);
}

void	GapBuffer::setCursorPosition(size_t index)
{
	size_t gapSize = getGapSize();
	recalculateDerivedInfo();
	if (index < gapStart_)
		relocateGapTo(index);
	else if (index > arrayLength_ + gapSize)
		relocateGapTo(arrayLastIndex_);
	else if (index >= gapStart_)
		relocateGapTo((index - gapStart_) + tailStart_);
}

GapBuffer::GapBufferException::GapBufferException(const std::string &what_arg)
	: what_(what_arg) {}

const char* GapBuffer::GapBufferException::what() const noexcept
{
	return (what_.c_str());
}
