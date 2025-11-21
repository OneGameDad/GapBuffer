#include "../includes/utf8GapBuffer.hpp"
#include <cstddef>
#include <utility>

utf8GapBuffer::utf8GapBuffer()
	: bufferSize_(STARTING_BUFFER_SIZE), gapStart_(0), arrayLength_(0), arrayLastIndex_(0)
{
	buffer_ = new char8_t[bufferSize_];
	zeroOutBuffer(buffer_, bufferSize_);
	tailStart_ = setTailStart(GAP_SIZE);
}

utf8GapBuffer::utf8GapBuffer(std::u8string &newContent)
	: gapStart_(0), arrayLength_(0), arrayLastIndex_(0)
{
	size_t size = newContent.size() + 1 + GAP_SIZE;
	buffer_ = new char8_t[size];
	zeroOutBuffer(buffer_, size);
	tailStart_ = setTailStart(GAP_SIZE);
	for (size_t i = 0; i < newContent.size(); i++)
		insert(newContent[i]);
	relocateGapTo(0);
}

utf8GapBuffer::~utf8GapBuffer()
{
	delete [] buffer_;
	buffer_ = nullptr;
}

utf8GapBuffer::utf8GapBuffer(const utf8GapBuffer &copy)
{
	*this = copy;
}

utf8GapBuffer	&utf8GapBuffer::operator=(const utf8GapBuffer &copy)
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

char8_t	&utf8GapBuffer::operator[](size_t index)
{
	size_t visibleSize = bufferSize_ - (tailStart_ - gapStart_ + 1) ;
	if (index >= visibleSize)
		throw std::out_of_range("Index beyond the gap buffer range");
	if (index < gapStart_)
		return (buffer_[index]);
	return (buffer_[index + (tailStart_ - gapStart_ + 1)]);
}

char8_t	&utf8GapBuffer::operator[](size_t index) const
{
	size_t visibleSize = bufferSize_ - (tailStart_ - gapStart_ + 1);
	if (index >= visibleSize)
		throw std::out_of_range("Index beyond the gap buffer range");
	if (index < gapStart_)
		return (buffer_[index]);
	return (buffer_[index + (tailStart_ - gapStart_ + 1)]);
}

bool	utf8GapBuffer::isBufferFull()
{
	if (arrayLastIndex_ == bufferSize_ - 1)
		return (true);
	return (false);
}

void	utf8GapBuffer::resizeBuffer()
{
	size_t newBufferSize = bufferSize_ * 2;
	size_t gapSize = GAP_SIZE;
	size_t newGapEnd = gapStart_ + gapSize;
	char8_t* newBuffer = new char8_t[newBufferSize];
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

void	utf8GapBuffer::zeroOutBuffer(char8_t* buffer, size_t size)
{
	for (size_t i = 0; i < size; i++)
		buffer[i] = 0;
}

size_t	utf8GapBuffer::setTailStart(size_t newSize)
{
	return (gapStart_ + newSize - 1);
}

size_t	utf8GapBuffer::getGapSize() const { return (tailStart_ - gapStart_); }

size_t	utf8GapBuffer::getLastIndex() const { return (arrayLastIndex_); }

size_t	utf8GapBuffer::getTailStart() const { return (tailStart_); }

size_t	utf8GapBuffer::getArrayLength() const { return (arrayLength_); }

size_t	utf8GapBuffer::getGapStart() const { return (gapStart_); }

size_t	utf8GapBuffer::getBufferSize() const { return (bufferSize_); }

void	utf8GapBuffer::calculateArrayLength()
{
	size_t count = 0;
	for (size_t i = 0; i < bufferSize_; i++)
	{
		if (buffer_[i] != 0)
			count++;
	}
	arrayLength_ = count;
}

void	utf8GapBuffer::calculateArrayLastIndex()
{
	size_t headSize = gapStart_;
	size_t tailSize = getTailSize();
	if (tailSize != 0)
		arrayLastIndex_ = gapStart_ + getGapSize() + tailSize;
	else
		arrayLastIndex_ = headSize;
}

void	utf8GapBuffer::recalculateDerivedInfo()
{
	calculateArrayLength();
	calculateArrayLastIndex();
}

void	utf8GapBuffer::setBuffer(size_t index, char8_t ch)
{
	assert(index == gapStart_);
	assert(buffer_[index] == 0);
	buffer_[index] = ch;
	shrinkGap();
}

void	utf8GapBuffer::insert(char8_t ch)
{
	assert(gapStart_ < bufferSize_);
	setBuffer(gapStart_, ch);
}

void	utf8GapBuffer::remove()
{
	if (gapStart_ == 0 || arrayLength_ == 0)
		return ;
	growGap();
}

void utf8GapBuffer::moveBytesToHigherIndices(size_t newIndex)
{
	assert(gapStart_ + getGapSize() + getTailSize() < bufferSize_);
	size_t gapSize = getGapSize();
	size_t bytesToMove = gapStart_ - newIndex;
	size_t newTailStart = tailStart_ - bytesToMove;
	assert(newTailStart == newIndex + gapSize);
	char tempArray[bytesToMove + 1];
	for (size_t i = newIndex, j = 0; i < gapStart_ && j < bytesToMove + 1; i++, j++)
	{
		tempArray[j] = buffer_[i];
		buffer_[i] = '\0';
	}
	tempArray[bytesToMove] = '\0';
	for (size_t i = newTailStart, j = 0; j < bytesToMove; i++, j++)
			buffer_[i] = tempArray[j];
	gapStart_ = newIndex;
	tailStart_ = newTailStart;
	cleanGap();
	recalculateDerivedInfo();
}

void utf8GapBuffer::moveBytesToLowerIndices(size_t newIndex)
{
	assert(gapStart_ + getGapSize() + getTailSize() < bufferSize_);
	size_t gapSize = getGapSize();
	calculateArrayLastIndex();
	if (newIndex > arrayLastIndex_)
		newIndex = arrayLastIndex_;
	else
		newIndex = newIndex - gapStart_ + gapSize;
	size_t bytesToMove;
	if (newIndex > tailStart_)
		bytesToMove = newIndex - tailStart_;
	else
		bytesToMove = tailStart_ - newIndex;
	char tempArray[bytesToMove + 1];
	for (size_t i = tailStart_, j = 0; i < newIndex; i++, j++)
	{
		tempArray[j] = buffer_[i];
		buffer_[i] = '\0';
	}
	tempArray[bytesToMove] = '\0';
	for (size_t i = gapStart_, j = 0; j < bytesToMove; i++, j++)
		buffer_[i] = tempArray[j];
	gapStart_ = gapStart_ + bytesToMove;
	tailStart_ = setTailStart(gapSize);
	recalculateDerivedInfo();
	cleanGap();
}

void utf8GapBuffer::shiftTailBytesToHigherIndices(size_t newGapSize, size_t tailSize)
{
	assert(gapStart_ + getGapSize() + getTailSize() < bufferSize_);
	size_t tailDiff = gapStart_ + newGapSize - tailStart_;
	size_t newTailStart = tailStart_ + tailDiff;
	assert((gapStart_ + newGapSize + tailSize) < bufferSize_);
	size_t n = arrayLastIndex_ - tailStart_;
	assert(getTailSize() != tailDiff);
	char tempArray[n + 1];
	for (size_t i = tailStart_, j = 0; i <= arrayLastIndex_; i++, j++)
	{
		tempArray[j] = buffer_[i];
		buffer_[i] = '\0';
	}
	tempArray[n] = '\0';
	for (size_t i = newTailStart, j = 0; j < n; i++, j++)
		buffer_[i] = tempArray[j];
	tailStart_ = newTailStart;
	recalculateDerivedInfo();
	cleanGap();
}

void utf8GapBuffer::relocateGapTo(size_t newIndex)
{
	if (newIndex == gapStart_ || (arrayLength_ == 0 && gapStart_ == 0) || (getGapSize() == bufferSize_ - 1))
		return ;
	if (newIndex < gapStart_)
		moveBytesToHigherIndices(newIndex);
	else if (newIndex > gapStart_)
		moveBytesToLowerIndices(newIndex);
}

size_t	utf8GapBuffer::getTailSize()
{
	return (arrayLength_ -  gapStart_);
}

void	utf8GapBuffer::resizeGap()
{
	size_t newGapSize = GAP_SIZE;
	size_t tailSize = getTailSize();
	if (tailSize != 0 || (gapStart_ + newGapSize + tailSize >= bufferSize_ - 1))
	{
		resizeBuffer();
		return ;
	}
		shiftTailBytesToHigherIndices(newGapSize, tailSize);
}

void	utf8GapBuffer::shrinkGap()
{
	gapStart_++;
	recalculateDerivedInfo();
	if (isBufferFull())
		resizeBuffer();
	if (getGapSize() == 0)
		resizeGap();
}

void	utf8GapBuffer::growGap()
{
	gapStart_--;
	buffer_[gapStart_] = 0;
	recalculateDerivedInfo();
}

void	utf8GapBuffer::cleanGap()
{
	for (size_t i = gapStart_; i < tailStart_; i++)
		buffer_[i] = '\0';
}

std::string	utf8GapBuffer::getVisibleText() const
{
	std::u8string visible;
	if (arrayLength_ == 0)
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
	if (count != arrayLength_)
		throw utf8GapBuffer::utf8GapBufferException("The number of filled Indices does not match the number of visible characters");
	std::string result = std::string(visible.begin(), visible.end());
	return (result);
}

std::u8string	utf8GapBuffer::getVisibleU8Text() const
{
	std::u8string visible;
	if (arrayLength_ == 0)
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
	if (count != arrayLength_)
		throw utf8GapBuffer::utf8GapBufferException("The number of filled Indices does not match the number of visible characters");
	return (visible);
}

void	utf8GapBuffer::setCursorPosition(size_t newIndex)
{
	size_t gapSize = getGapSize();
	recalculateDerivedInfo();
	if (newIndex == gapStart_)
		return ;
	else if (newIndex < gapStart_)
		relocateGapTo(newIndex);
	else if (newIndex > arrayLength_ + gapSize)
		relocateGapTo(arrayLastIndex_);
	else if (newIndex > gapStart_)
		relocateGapTo((newIndex - gapStart_) + tailStart_);
}

utf8GapBuffer::utf8GapBufferException::utf8GapBufferException(const std::string &what_arg)
	: what_(what_arg) {}

const char* utf8GapBuffer::utf8GapBufferException::what() const noexcept
{
	return (what_.c_str());
}

void	utf8GapBuffer::deleteSelection(size_t start, size_t end)
{
	if (start == end)
		return ;
	if (start > end)
		std::swap(start, end);
	setCursorPosition(end);
	for (size_t i = end; i >= start; i--)
		remove();
}

void	utf8GapBuffer::paste(std::u8string &newContent, size_t cursorPosition)
{
	setCursorPosition(cursorPosition);
	for (size_t i = 0; i < newContent.size(); i++)
		insert(newContent[i]);
}
