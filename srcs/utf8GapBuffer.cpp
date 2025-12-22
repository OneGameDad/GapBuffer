#include "../includes/utf8GapBuffer.hpp"
#include <algorithm>
#include <cstring>

utf8GapBuffer::utf8GapBuffer()
	: bufferSize_(STARTING_BUFFER_SIZE), gapStart_(0), byteCount_(0), byteLastIndex_(0), charCount_(0), charLastIndex_(0)
{
	buffer_ = new char8_t[bufferSize_];
	zeroOutBuffer(buffer_, bufferSize_);
	tailStart_ = setTailStart(GAP_SIZE);
	tailEnd_ = tailStart_;
	recalculateDerivedInfo();
	assertInvariants();
}

utf8GapBuffer::utf8GapBuffer(const std::u8string &newContent)
	: bufferSize_(newContent.size() + 1 + GAP_SIZE), gapStart_(0), byteCount_(0), byteLastIndex_(0), charCount_(0), charLastIndex_(0)
{
	buffer_ = new char8_t[bufferSize_];
	zeroOutBuffer(buffer_, bufferSize_);
	tailStart_ = setTailStart(GAP_SIZE);
	tailEnd_ = tailStart_;
	for (size_t i = 0; i < newContent.size(); i++)
		insert(newContent[i]);
	recalculateDerivedInfo();
	relocateGapTo(0);
	assertInvariants();
}

utf8GapBuffer::~utf8GapBuffer()
{
	delete [] buffer_;
	buffer_ = nullptr;
}

utf8GapBuffer::utf8GapBuffer(const utf8GapBuffer &copy)
	: bufferSize_(copy.bufferSize_), gapStart_(copy.gapStart_), tailStart_(copy.tailStart_), tailEnd_(copy.tailEnd_), byteCount_(copy.byteCount_), byteLastIndex_(copy.byteLastIndex_), charCount_(copy.charCount_), charLastIndex_(copy.charLastIndex_)
{
	buffer_ = new char8_t[bufferSize_];
	std::copy(copy.buffer_, copy.buffer_ + copy.bufferSize_, buffer_);
	recalculateDerivedInfo();
	assertInvariants();
}

utf8GapBuffer	&utf8GapBuffer::operator=(const utf8GapBuffer &copy)
{
if (this != &copy)
	{
		char8_t* newBuffer_ = new char8_t[copy.bufferSize_];
		std::copy(copy.buffer_, copy.buffer_ + copy.bufferSize_, newBuffer_);
		delete [] buffer_;
		buffer_ = newBuffer_;
		bufferSize_ = copy.bufferSize_;
		gapStart_ = copy.gapStart_;
		tailStart_ = copy.tailStart_;
		tailEnd_ = copy.tailEnd_;
		byteCount_ = copy.byteCount_;
		byteLastIndex_ = copy.byteLastIndex_;
		charCount_ = copy.charCount_;
		charLastIndex_ = copy.charLastIndex_;

	}
	assertInvariants();
	return (*this);
}

char8_t	&utf8GapBuffer::operator[](size_t index)
{
	size_t	GapSize = getGapSize();
	size_t visibleSize = bufferSize_ - GapSize;
	if (index >= visibleSize)
		throw std::out_of_range("Index beyond the gap buffer range");
	assertInvariants();
	if (index < gapStart_)
		return (buffer_[index]);
	return (buffer_[index + GapSize]);
}

const char8_t	&utf8GapBuffer::operator[](size_t index) const
{
	size_t	GapSize = getGapSize();
	size_t visibleSize = bufferSize_ - GapSize;
	if (index >= visibleSize)
		throw std::out_of_range("Index beyond the gap buffer range");
	assertInvariants();
	if (index < gapStart_)
		return (buffer_[index]);
	return (buffer_[index + GapSize]);
}

bool	utf8GapBuffer::isBufferFull()
{
	if (byteLastIndex_ == bufferSize_ - 1)
		return (true);
	return (false);
}

void	utf8GapBuffer::resizeBuffer()
{
	size_t newBufferSize = bufferSize_ * 2;
	size_t gapSize = GAP_SIZE;
	size_t newGapEnd = gapStart_ + gapSize;
	size_t oldTailSize = tailEnd_ - tailStart_;
	char8_t* newBuffer = new char8_t[newBufferSize];
	zeroOutBuffer(newBuffer, newBufferSize);

	std::copy(buffer_, buffer_ + gapStart_, newBuffer);
	for (size_t i = gapStart_; i < newGapEnd; i++)
		newBuffer[i] = 0;
	std::copy(buffer_ + tailStart_, buffer_ + tailEnd_, newBuffer + newGapEnd);

	delete [] buffer_;
	buffer_ = newBuffer;
	bufferSize_ = newBufferSize;
	tailStart_ = setTailStart(gapSize);
	tailEnd_ = tailStart_ + oldTailSize;
	assert(gapStart_ < tailStart_);
	recalculateDerivedInfo();
	assertInvariants();
}

void	utf8GapBuffer::zeroOutBuffer(char8_t* buffer, size_t size)
{
	std::memset(buffer, 0, size);
}

size_t	utf8GapBuffer::setTailStart(size_t newSize)
{
	return (gapStart_ + newSize);
}

size_t	utf8GapBuffer::getGapSize() const { return (tailStart_ - gapStart_); }

size_t	utf8GapBuffer::getLastByteIndex() const { return (byteLastIndex_); }

size_t	utf8GapBuffer::getTailStart() const { return (tailStart_); }

size_t	utf8GapBuffer::getTailEnd() const { return (tailEnd_); }

size_t	utf8GapBuffer::getByteCount() const { return (byteCount_); }

size_t	utf8GapBuffer::getGapStart() const { return (gapStart_); }

size_t	utf8GapBuffer::getBufferSize() const { return (bufferSize_); }

size_t	utf8GapBuffer::getCharCount() const { return (charCount_); }

size_t	utf8GapBuffer::getLastCharIndex() const { return (charLastIndex_); }

void	utf8GapBuffer::calculateByteCount()
{
	byteCount_ = gapStart_ + (tailEnd_ - tailStart_);
}

void	utf8GapBuffer::calculateByteLastIndex()
{
	size_t headSize = gapStart_;
	size_t tailSize = getTailByteSize();
	byteLastIndex_ = headSize + tailSize;
}

void	utf8GapBuffer::calculateCharCount()
{
	charCount_ = 0;
	
	for (size_t i = 0; i < gapStart_; i++) {
		if ((buffer_[i] & 0xC0) != 0x80) {
			charCount_++;
		}
	}
	
	for (size_t i = tailStart_; i < tailEnd_; i++) {
		if ((buffer_[i] & 0xC0) != 0x80) {
			charCount_++;
		}
	}
}

void	utf8GapBuffer::calculateCharLastIndex()
{
	charLastIndex_ = charCount_;
}

void	utf8GapBuffer::recalculateDerivedInfo()
{
	calculateByteCount();
	calculateByteLastIndex();
	calculateCharCount();
	calculateCharLastIndex();
}

void	utf8GapBuffer::setBuffer(size_t index, char8_t ch)
{
	assert(index == gapStart_);
	assert(buffer_[index] == 0);
	buffer_[index] = ch;
	shrinkGap();
	assertInvariants();
}

void	utf8GapBuffer::insert(char8_t ch)
{
	if (gapStart_ == tailStart_)
		resizeGap();
	assert(gapStart_ < bufferSize_);
	setBuffer(gapStart_, ch);
	assertInvariants();
}

void	utf8GapBuffer::remove()
{
	if (gapStart_ == 0 || byteCount_ == 0)
		return ;
	growGap();
	assertInvariants();
}

void utf8GapBuffer::moveBytesToHigherIndices(size_t newIndex)
{
	assert(gapStart_ + getGapSize() + (tailEnd_ - tailStart_) <= bufferSize_);
	size_t gapSize = getGapSize();
	size_t bytesToMove = gapStart_ - newIndex;
	size_t newTailStart = tailStart_ - bytesToMove;
	size_t originalTailSize = tailEnd_ - tailStart_;
	size_t newTailEnd = newTailStart + bytesToMove + originalTailSize;
	assert(newTailStart == newIndex + gapSize);
	std::vector<char8_t> tempArray(bytesToMove + 1);
	for (size_t i = newIndex, j = 0; i < gapStart_ && j < bytesToMove + 1; i++, j++)
	{
		tempArray[j] = buffer_[i];
		buffer_[i] = u8'\0';
	}
	tempArray[bytesToMove] = u8'\0';
	for (size_t i = newTailStart, j = 0; j < bytesToMove; i++, j++)
			buffer_[i] = tempArray[j];
	gapStart_ = newIndex;
	tailStart_ = newTailStart;
	tailEnd_ = newTailEnd;
	cleanGap();
	recalculateDerivedInfo();
	assertInvariants();
}

void utf8GapBuffer::moveBytesToLowerIndices(size_t newIndex)
{
	assert(gapStart_ + getGapSize() + (tailEnd_ - tailStart_) <= bufferSize_);
	size_t gapSize = getGapSize();
	calculateByteLastIndex();
	
	if (newIndex >= byteLastIndex_)
		newIndex = byteLastIndex_;
	
	if (newIndex <= gapStart_)
		return;
	
	size_t bufferNewIndex = tailStart_ + (newIndex - gapStart_);
	
	size_t bytesToMove = bufferNewIndex - tailStart_;
	assert(bytesToMove <= (tailEnd_ - tailStart_));
	
	std::vector<char8_t> tempArray(bytesToMove + 1);
	for (size_t i = tailStart_, j = 0; i < bufferNewIndex; i++, j++)
	{
		tempArray[j] = buffer_[i];
		buffer_[i] = u8'\0';
	}
	tempArray[bytesToMove] = u8'\0';
	
	for (size_t i = gapStart_, j = 0; j < bytesToMove; i++, j++)
		buffer_[i] = tempArray[j];
	
	size_t oldTailSize = tailEnd_ - tailStart_;
	gapStart_ = gapStart_ + bytesToMove;
	tailStart_ = setTailStart(gapSize);
	tailEnd_ = tailStart_ + (oldTailSize - bytesToMove);
	recalculateDerivedInfo();
	cleanGap();
	assertInvariants();
}

void utf8GapBuffer::shiftTailBytesToHigherIndices(size_t newGapSize, size_t tailSize)
{
	assert(gapStart_ + getGapSize() + (tailEnd_ - tailStart_) <= bufferSize_);
	size_t tailDiff = gapStart_ + newGapSize - tailStart_;
	size_t newTailStart = tailStart_ + tailDiff;
	size_t newTailEnd = tailEnd_ + tailDiff;
	assert((gapStart_ + newGapSize + tailSize) < bufferSize_);
	size_t n = tailEnd_ - tailStart_;
	assert(getTailByteSize() != tailDiff);
	std::vector<char8_t> tempArray(n + 1);
	for (size_t i = tailStart_, j = 0; i < tailEnd_; i++, j++)
	{
		tempArray[j] = buffer_[i];
		buffer_[i] = u8'\0';
	}
	tempArray[n] = u8'\0';
	for (size_t i = newTailStart, j = 0; j < n; i++, j++)
		buffer_[i] = tempArray[j];
	tailStart_ = newTailStart;
	tailEnd_ = newTailEnd;
	recalculateDerivedInfo();
	cleanGap();
	assertInvariants();
}

void utf8GapBuffer::relocateGapTo(size_t newIndex)
{
	if (newIndex == gapStart_ || (byteCount_ == 0 && gapStart_ == 0) || (getGapSize() == bufferSize_ - 1))
		return ;
	if (newIndex < gapStart_)
		moveBytesToHigherIndices(newIndex);
	else if (newIndex > gapStart_)
		moveBytesToLowerIndices(newIndex);
	assertInvariants();
}

size_t	utf8GapBuffer::getTailByteSize()
{
	return (byteCount_ -  gapStart_);
}

void	utf8GapBuffer::resizeGap()
{
	size_t newGapSize = GAP_SIZE;
	size_t tailSize = getTailByteSize();
	if (gapStart_ + newGapSize + tailSize >= bufferSize_ - 1)
	{
		resizeBuffer();
		return ;
	}
	if (tailSize > 0)
		shiftTailBytesToHigherIndices(newGapSize, tailSize);
	else
	{
		tailStart_ = gapStart_ + newGapSize;
		tailEnd_ = tailStart_;
		recalculateDerivedInfo();
	}
	assertInvariants();
}

void	utf8GapBuffer::shrinkGap()
{
	gapStart_++;
	recalculateDerivedInfo();
	if (isBufferFull())
		resizeBuffer();
	if (getGapSize() == 0)
		resizeGap();
	assertInvariants();
}

void	utf8GapBuffer::growGap()
{
	gapStart_--;
	buffer_[gapStart_] = 0;
	recalculateDerivedInfo();
	assertInvariants();
}

void	utf8GapBuffer::cleanGap()
{
	for (size_t i = gapStart_; i < tailStart_; i++)
		buffer_[i] = u8'\0';
}

std::string	utf8GapBuffer::getStdString() const
{
	if (byteCount_ == 0)
		return (std::string());
	std::string result;
	result.reserve(bufferSize_ - getGapSize());
	result.append(reinterpret_cast<const char*>(buffer_), getGapStart());
	size_t tailSize = tailEnd_ - tailStart_;
	result.append(reinterpret_cast<const char*>(buffer_ + getTailStart()), tailSize);
	assertInvariants();
	return (result);
}

std::u8string	utf8GapBuffer::getVisibleU8Text() const
{
	std::u8string visible;
	if (byteCount_ == 0)
		return (visible);
	visible.reserve(byteCount_);
	visible.append(reinterpret_cast<const char8_t*>(buffer_), gapStart_);
	size_t tailSize = tailEnd_ - tailStart_;
	visible.append(reinterpret_cast<const char8_t*>(buffer_ + tailStart_), tailSize);
	assertInvariants();
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
	else if (newIndex > byteCount_ + gapSize)
		relocateGapTo(byteLastIndex_);
	else if (newIndex > gapStart_ && newIndex <= byteCount_)
		relocateGapTo(tailStart_ + (newIndex - gapStart_));
	assertInvariants();
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

	size_t count = end - start;
	for (size_t i = 0; i < count; i++)
		remove();
	assertInvariants();
}

void	utf8GapBuffer::paste(const std::string &newContent, size_t cursorPosition)
{
	setCursorPosition(cursorPosition);
	for (size_t i = 0; i < newContent.size(); i++)
		insert(static_cast<char8_t>(newContent[i]));
	assertInvariants();
}

void	utf8GapBuffer::utf8paste(const std::u8string &newContent, size_t cursorPosition)
{
	setCursorPosition(cursorPosition);
	for (size_t i = 0; i < newContent.size(); i++)
		insert(newContent[i]);
	assertInvariants();
}

void utf8GapBuffer::assertInvariants() const
{
    assert(gapStart_ <= tailStart_);
    assert(tailStart_ <= bufferSize_);
    assert(tailEnd_ >= tailStart_);
    assert(tailEnd_ <= bufferSize_);
    assert(byteCount_ == gapStart_ + (tailEnd_ - tailStart_));
    assert((tailStart_ - gapStart_) <= bufferSize_);
    assert(gapStart_ <= bufferSize_);
}

