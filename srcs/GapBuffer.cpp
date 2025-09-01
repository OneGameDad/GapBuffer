#include "../includes/GapBuffer.hpp"

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

void	GapBuffer::calculateLastIndex()//Use this to check if repositioning gap is going past the last element
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
	if (tailSize != 0)
		lastIndex_ = tailSize;
	else
		lastIndex_ = headSize;
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
	relocateGapTo(gapStart_ + 1);
	calculateGapSize();
	calculateFilledIndices();
	calculateLastIndex();
}

void	GapBuffer::remove()
{
	if (gapStart_ == 0 || filledIndices_ == 0)
		return ;
	relocateGapTo(gapStart_ - 1);
	calculateGapSize();
	calculateFilledIndices();
	calculateLastIndex();
}

void GapBuffer::relocateGapTo(size_t index)
{
	if (index == gapStart_)
		return ;
	if (index < gapStart_)
	{
		calculateLastIndex();
		if (index > lastIndex_ && lastIndex_ < gapStart_)
			index = lastIndex_ + 1;
		//TODO move backward?
	}
	else if (index > gapStart_)
	{
		calculateLastIndex();
		if (index > lastIndex_)
			index = lastIndex_ + 1;
		//TODO move forward?
	}
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
	if (count != filledIndices_)
		throw GapBufferException::what("The number of filled Indices does not match the number of visible characters");
	return (visible);
}
