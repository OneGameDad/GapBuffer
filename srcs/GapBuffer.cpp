#include "../includes/GapBuffer.hpp"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <stdexcept>

GapBuffer::GapBuffer()
	: bufferSize_(STARTING_BUFFER_SIZE), gapStart_(0), gapSize_(STARTING_GAP_SIZE), filledIndices_(0), lastIndex_(0)
{
	buffer_ = new char[bufferSize_];
	calculateGapEnd();
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
		gapSize_ = copy.gapSize_;
		calculateGapEnd();
		filledIndices_ = copy.filledIndices_;
		lastIndex_ = copy.lastIndex_;
	}
	return (*this);
}

bool	GapBuffer::isBufferFull()
{
	return (gapSize_ == 0);
}

void	GapBuffer::resizeBuffer()
{
	size_t used = gapStart_ + (bufferSize_ - (gapEnd_ + 1));
	size_t newBufferSize = (bufferSize_ == STARTING_BUFFER_SIZE) ? STARTING_BUFFER_SIZE : bufferSize_ * 2;
	while (newBufferSize <= used)
		newBufferSize *= 2;
	size_t newGapSize = newBufferSize - used;

	char* newBuffer = new char[newBufferSize];
	std::copy(buffer_, buffer_ + gapStart_, newBuffer),
	std::copy(buffer_ + gapEnd_ + 1, buffer_ + bufferSize_, newBuffer + gapStart_ + newGapSize);
	std::fill(newBuffer + gapStart_, newBuffer + gapStart_ + newGapSize, 0);
	
	delete [] buffer_;
	buffer_ = newBuffer;
	bufferSize_ = newBufferSize;
	gapEnd_ = newGapSize;
	calculateGapSize();
	calculateFilledIndices();
	calculateLastIndex();
}

void	GapBuffer::calculateGapEnd()
{
	gapEnd_ = gapStart_ + gapSize_ - 1;
}

void	GapBuffer::calculateGapSize()
{
	if (gapEnd_ > gapStart_)
		gapSize_ = gapEnd_ - gapStart_ + 1;
	else
		resizeGap();
}

void	GapBuffer::calculateFilledIndices()
{
	size_t headSize = 0;
	if (gapStart_ != 0)
		headSize = gapStart_ - 1;
	size_t tailSize = 0;
	for (size_t i = gapEnd_ + 1; i < bufferSize_; i++)
	{
		if (buffer_[i] != 0)
			tailSize++;
	}
	filledIndices_ = headSize + tailSize;
	if (filledIndices_ == bufferSize_ - 1)
		resizeBuffer();
}

void	GapBuffer::calculateLastIndex()
{
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
	if (filledIndices_ > gapStart_)
		lastIndex_ = filledIndices_ + gapSize_;
	else
		lastIndex_ = filledIndices_;
}

void	GapBuffer::recalculateDerivedInfo()
{
	calculateGapSize();
	calculateFilledIndices();
	calculateLastIndex();
}

void	GapBuffer::setBuffer(size_t index, char ch)
{
	buffer_[index] = ch;
	lastIndex_++;
	shrinkGap();
}

void	GapBuffer::insert(char ch)
{
	if (gapSize_ == 0)
	{
		resizeBuffer();
		relocateGapTo(gapStart_);
	}
	if (gapStart_ >= bufferSize_)
		resizeBuffer();
	assert(gapStart_ < bufferSize_);
	setBuffer(gapStart_, ch);
}

void	GapBuffer::remove()
{
	if (gapStart_ == 0 || filledIndices_ == 0)
		return ;
	growGap();
}

void GapBuffer::relocateGapTo(size_t index)
{
	//TODO when moving the gapSize_ needs to be restored to a minimum size
	if (index == gapStart_)
		return ;
	if (index < gapStart_)
	{
		calculateLastIndex();
		if (index > lastIndex_ && lastIndex_ < gapStart_)
			index = lastIndex_ + 1;
		std::copy_backward(buffer_ + index,
		     buffer_ + gapStart_, 
		     buffer_ + gapStart_ + gapSize_ - 1);
		gapStart_ = index;
		calculateGapEnd();
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
		calculateGapEnd();
	}
	cleanGap();
}

bool	GapBuffer::isTailEmpty()
{
	for (size_t i = gapEnd_; i < bufferSize_; i++)
	{
		if (buffer_[i] != 0)
			return (false);
	}
	return (true);
}

void	GapBuffer::resizeGap()
{
	if (gapSize_ == 0)
	{
		size_t newGapSize = STARTING_GAP_SIZE;
		if (gapStart_ + newGapSize >= bufferSize_)
		{
			resizeBuffer();
			return ;
		}
		else if (!isTailEmpty())
		{
			size_t tailSize = bufferSize_ - gapEnd_;
			memmove(&buffer_[gapEnd_ + newGapSize], &buffer_[gapEnd_], tailSize);
			gapEnd_ += newGapSize;
			calculateGapSize();
			cleanGap();
		}
	}
}

void	GapBuffer::shrinkGap()
{
	gapStart_++;
	recalculateDerivedInfo();
	if (gapSize_ == 0)
		resizeGap();
}

void	GapBuffer::growGap()
{
	gapStart_--;
	recalculateDerivedInfo();
	cleanGap();
}

void	GapBuffer::cleanGap()
{
	std::fill(buffer_ + gapStart_, buffer_ + gapEnd_ + 1, 0);
}

std::string	GapBuffer::getVisibleText() const
{
	std::string visible;
	if (filledIndices_ == 0)
		return (visible);
	size_t count = 0;
	for (size_t i = 0; i < gapStart_; i++)
	{
		visible.push_back(buffer_[i]);
		count++;
	}
	if (gapStart_ < lastIndex_)
	{
		for (size_t i = gapEnd_; i < bufferSize_; i++)
		{
			if (buffer_[i] != 0)
			{
				visible.push_back(buffer_[i]);
				count++;
			}
		}
	}
	std::cout << "Filled Indices: " << filledIndices_ << " & Counted characters: " << count << " Visible Text Size: " << visible.size() << std::endl;
//	if (count != filledIndices_)
//		throw GapBufferException("The number of filled Indices does not match the number of visible characters");
	return (visible);
}

void	GapBuffer::setCursorPosition(size_t index)
{
	if (index < gapStart_)
		relocateGapTo(index);
	else if (index > filledIndices_ + gapSize_)
		relocateGapTo(filledIndices_ + gapSize_ + 1);
	else if (index >= gapStart_)
		relocateGapTo((index - gapStart_) + gapEnd_);
}

GapBuffer::GapBufferException::GapBufferException(const std::string &what_arg)
	: what_(what_arg) {}

const char* GapBuffer::GapBufferException::what() const noexcept
{
	return (what_.c_str());
}
