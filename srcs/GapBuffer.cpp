#include "../includes/GapBuffer.hpp"
#include <cassert>
#include <cstddef>

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

void	GapBuffer::calculateGapEnd()
{
	gapEnd_ = gapStart_ + gapSize_ - 1;
}

void	GapBuffer::calculateGapSize()
{
	if (gapEnd_ >= gapStart_)
		gapSize_ = gapEnd_ - gapStart_ + 1;
	else
		gapSize_ = 0;
}

void	GapBuffer::calculateFilledIndices()
{
	size_t headSize = gapStart_ - 1;
	size_t tailSize = 0;
	for (size_t i = gapEnd_ + 1; i < bufferSize_; i++)
	{
		if (buffer_[i] != 0)
			tailSize++;
	}
	filledIndices_ = headSize + tailSize;
}

void	GapBuffer::calculateLastIndex()//TODO why do I need to know last index?
{
	//last tail index is ?
	size_t tailSize = 0;
	for (size_t i = gapEnd_ + 1; i < bufferSize_; i++)
	{
		if (buffer_[i] != 0)
			tailSize++;
	}
	lastIndex_ = tailSize;//But this doesn't take into account gap size, does it need to?
}

void	GapBuffer::insert(char ch)
{
	if (gapSize_ == 0)
	{
		resizeBuffer();
		relocateGapTo(gapStart_);
	}
	assert(gapStart_ < bufferSize_);
	buffer_[gapStart_] = ch;
	calculateGapSize();
	calculateFilledIndices();
	//TODO lastIndex_
}

void	GapBuffer::remove()
{
	if (gapStart_ == 0 || filledIndices_ == 0)
		return ;
	gapStart_--;
	calculateGapSize();
	calculateFilledIndices();
	//TODO lastIndex_
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
	for (size_t i = gapEnd_; i < bufferSize_; i++)
	{
		visible.push_back(buffer_[i]);
		count++;
	}
	std::cout << "Filled Indices: " << filledIndices_ << " & Counted characters: " << count << std::endl;
	return (visible);
}
