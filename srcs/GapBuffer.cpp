#include "../includes/GapBuffer.hpp"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <utility>

GapBuffer::GapBuffer()
	: bufferSize_(STARTING_BUFFER_SIZE), gapStart_(0), filledIndices_(0), lastIndex_(0)
{
	buffer_ = new char[bufferSize_];
	zeroOutBuffer(buffer_, bufferSize_);
	gapEnd_ = setGapEnd(STARTING_GAP_SIZE);
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
		gapEnd_ = copy.gapEnd_;
		filledIndices_ = copy.filledIndices_;
		lastIndex_ = copy.lastIndex_;
	}
	return (*this);
}

//TODO This isn't called anywhere, should it be renamed to IsGapZero
bool	GapBuffer::isBufferFull()
{
	//TODO Is this better checking lastIndex?
	return (getGapSize() == 0);
}

void	GapBuffer::resizeBuffer()//TODO Buffer is copying over 1 less character
{
	std::cout << "RESIZE BUFFER CALLED" << std::endl;
	size_t newBufferSize = bufferSize_ * 2;
	size_t gapSize = getGapSize();
	size_t newGapEnd = gapStart_ + gapSize - 1;
	char* newBuffer = new char[newBufferSize];
	zeroOutBuffer(newBuffer, newBufferSize);

	std::cout << "Buffer before reset contains: " << getVisibleText() << std::endl;
	//Copy Head
	std::copy(buffer_, buffer_ + gapStart_, newBuffer);
	//Set Gap to Zeros
	for (size_t i = gapStart_; i < gapEnd_ - 1; i++)
		newBuffer[i] = 0;
	//Copy Tail
	std::copy(buffer_ + gapEnd_, buffer_ + bufferSize_, newBuffer + newGapEnd);

	std::cout << "New Buffer contains: " << std::endl;
	for (size_t i = 0; i < newBufferSize; i++)
	{
		if (newBuffer[i] != 0)
			std::cout << newBuffer[i];
	}
	std::cout << std::endl;
	std::cout << "Deleting old buffer\n";
	delete [] buffer_;
	std::cout << "Assigning new buffer\n";
	buffer_ = newBuffer;
	std::cout << "Assigning new buffer size\n";
	bufferSize_ = newBufferSize;
	std::cout << "Assigning new gap end\n";
	gapEnd_ = setGapEnd(gapSize);
	recalculateDerivedInfo();
}

void	GapBuffer::zeroOutBuffer(char* buffer, size_t size)
{
	for (size_t i = 0; i < size; i++)
		buffer[i] = 0;
}

size_t	GapBuffer::setGapEnd(size_t newSize)
{
	return (gapStart_ + newSize - 1);
}

size_t	GapBuffer::getGapSize()
{
	return (gapEnd_ - gapStart_);
}

void	GapBuffer::calculateFilledIndices()
{
	std::cout << "Calculating Filled Indices\n";
	size_t count = 0;
	for (size_t i = 0; i < bufferSize_; i++)
	{
		if (buffer_[i] != 0)
			count++;
	}
	filledIndices_ = count;
/*	size_t headSize = 0;
	if (gapStart_ != 0)
		headSize = gapStart_ - 1;
	size_t tailSize = 0;
	for (size_t i = gapEnd_ + 1; i < bufferSize_; i++)
	{
		if (buffer_[i] != 0)
			tailSize++;
	}
	filledIndices_ = headSize + tailSize;*/
/*	if (filledIndices_ == bufferSize_ - 1)
		resizeBuffer();
*/
}

void	GapBuffer::calculateLastIndex()
{
	std::cout << "Calculating Last Index\n";
/*	size_t headSize = 0;
	if (gapStart_ != 0)
		headSize = gapStart_;
	size_t tailSize = 0;
	for (size_t i = gapEnd_; i < bufferSize_; i++)
	{
		if (buffer_[i] != 0)
			tailSize++;
	}
	if (tailSize != 0)
		lastIndex_ = tailSize;
	else
		lastIndex_ = headSize;
	if (lastIndex_ == bufferSize_ - 1)
		resizeBuffer();
*/
	if (filledIndices_ >= gapStart_)
		lastIndex_ = filledIndices_ + getGapSize();
	else
		lastIndex_ = filledIndices_;
}

void	GapBuffer::recalculateDerivedInfo()
{
	std::cout << "Recalculating Derived Info\n";
	calculateFilledIndices();
	calculateLastIndex();
}

void	GapBuffer::setBuffer(size_t index, char ch)
{
	buffer_[index] = ch;
	std::cout << "setBuffer called, Current Last Index: " << lastIndex_ << " Filled Indices: " << filledIndices_ << " Buffer Size: " << bufferSize_ << " Gap Size: " << getGapSize() << " Char: " << ch << std::endl;
	lastIndex_++;
	shrinkGap();
//	std::cout << "New Last Index: " << lastIndex_ << " Filled Indices: " << filledIndices_ << " Buffer Size: " << bufferSize_ << " Gap Size: " << gapSize_ << std::endl;
}

void	GapBuffer::insert(char ch)
{
	std::cout << "Inserting Char: " << ch << std::endl;
/*	if (getGapSize() == 0)
	{
		resizeBuffer();
		relocateGapTo(gapStart_);
	}
	if (gapStart_ >= bufferSize_)
		resizeBuffer();
*/	assert(gapStart_ < bufferSize_);
	setBuffer(gapStart_, ch);
}

void	GapBuffer::remove()
{
	std::cout << "Removing Chararcter" << std::endl;
	if (gapStart_ == 0 || filledIndices_ == 0)
		return ;
	growGap();
}

void GapBuffer::relocateGapTo(size_t index)
{
	size_t gapSize = getGapSize();
	if (index == gapStart_)
		return ;
	if (index < gapStart_)
	{
		calculateLastIndex();
		if (index > lastIndex_ && lastIndex_ < gapStart_)
			index = lastIndex_ + 1;
		std::copy_backward(buffer_ + index,
		     buffer_ + gapStart_, 
		     buffer_ + gapStart_ + gapSize - 1);
		gapStart_ = index;
		gapEnd_ = setGapEnd(gapSize);
	}
	else if (index > gapStart_)
	{
		calculateLastIndex();
		if (index > lastIndex_)
			index = lastIndex_ + 1;
		std::copy(buffer_ + gapEnd_ + 1,
			buffer_ + gapEnd_ + 1 + (index - gapStart_),
			buffer_ + gapStart_);
		gapStart_ = index;
		gapEnd_ = setGapEnd(gapSize);
	}
	cleanGap();
}

size_t	GapBuffer::getTailSize()
{
	size_t tailSize;
	for (size_t i = gapEnd_; i < bufferSize_; i++)
	{
		if (buffer_[i] != 0)
			tailSize++;
	}
	return (tailSize);
}

void	GapBuffer::resizeGap()
{
	std::cout << "RESIZE GAP CALLED! Gap Size: " << getGapSize() << std::endl;
	size_t newGapSize = STARTING_GAP_SIZE;
	size_t tailSize = getTailSize();
	if (tailSize != 0 && (gapStart_ + newGapSize + tailSize >= bufferSize_ - 1))
		resizeBuffer();
	memmove(&buffer_[gapEnd_ + newGapSize], &buffer_[gapEnd_], tailSize);
	gapEnd_ += newGapSize - 1;
	cleanGap();

}

void	GapBuffer::shrinkGap()
{
	std::cout << "Shrinking Gap\n";
	gapStart_++;
	recalculateDerivedInfo();
	if (getGapSize() == 0)
		resizeGap();
}

void	GapBuffer::growGap()
{
	std::cout << "Growing Gap\n";
	gapStart_--;
	recalculateDerivedInfo();
	buffer_[gapStart_ + 1] = 0;
}

void	GapBuffer::cleanGap()
{
	std::cout << "Filling Gap with Zeros" << std::endl;
	std::fill(buffer_ + gapStart_, buffer_ + gapEnd_ - 1, 0);
}

std::string	GapBuffer::getVisibleText() const
{
	std::string visible;
	if (filledIndices_ == 0)
		return (visible);
	size_t count = 0;
	for(size_t i = 0; i < bufferSize_; i++)
	{
		if (buffer_[i] != 0)
		{
			visible.push_back(buffer_[i]);
			count++;
		}
	}
	std::cout << "Filled Indices: " << filledIndices_ << " & Counted characters: " << count << " Visible Text Size: " << visible.size() << std::endl;
//	if (count != filledIndices_)
//		throw GapBufferException("The number of filled Indices does not match the number of visible characters");
	return (visible);
}

void	GapBuffer::setCursorPosition(size_t index)
{
	size_t gapSize = getGapSize();
	if (index < gapStart_)
		relocateGapTo(index);
	else if (index > filledIndices_ + gapSize)
		relocateGapTo(filledIndices_ + gapSize + 1);
	else if (index >= gapStart_)
		relocateGapTo((index - gapStart_) + gapEnd_);
}

GapBuffer::GapBufferException::GapBufferException(const std::string &what_arg)
	: what_(what_arg) {}

const char* GapBuffer::GapBufferException::what() const noexcept
{
	return (what_.c_str());
}
