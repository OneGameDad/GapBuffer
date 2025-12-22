#include "../includes/GapBuffer.hpp"
#include <cstddef>
#include <utility>

GapBuffer::GapBuffer()
	: bufferSize_(STARTING_BUFFER_SIZE), gapStart_(0), byteCount_(0), byteLastIndex_(0), charCount_(0), charLastIndex_(0)
{
	buffer_ = new char[bufferSize_];
	zeroOutBuffer(buffer_, bufferSize_);
	tailStart_ = setTailStart(GAP_SIZE);
	tailEnd_ = tailStart_;
}

GapBuffer::GapBuffer(std::string &newContent)
	: gapStart_(0), byteCount_(0), byteLastIndex_(0), charCount_(0), charLastIndex_(0)
{
	size_t size = newContent.size() + 1 + GAP_SIZE;
	buffer_ = new char[size];
	zeroOutBuffer(buffer_, size);
	tailStart_ = setTailStart(GAP_SIZE);
	tailEnd_ = tailStart_;
	for (size_t i = 0; i < newContent.size(); i++)
		insert(newContent[i]);
	relocateGapTo(0);
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
		tailEnd_ = copy.tailEnd_;
		byteCount_ = copy.byteCount_;
		byteLastIndex_ = copy.byteLastIndex_;
		charCount_ = copy.charCount_;
		charLastIndex_ = copy.charLastIndex_;
	}
	return (*this);
}

char	&GapBuffer::operator[](size_t index)
{
	size_t visibleSize = bufferSize_ - (tailStart_ - gapStart_ + 1) ;
	if (index >= visibleSize)
		throw std::out_of_range("Index beyond the gap buffer range");
	if (index < gapStart_)
		return (buffer_[index]);
	return (buffer_[index + (tailStart_ - gapStart_ + 1)]);
}

char	&GapBuffer::operator[](size_t index) const
{
	size_t visibleSize = bufferSize_ - (tailStart_ - gapStart_ + 1);
	if (index >= visibleSize)
		throw std::out_of_range("Index beyond the gap buffer range");
	if (index < gapStart_)
		return (buffer_[index]);
	return (buffer_[index + (tailStart_ - gapStart_ + 1)]);
}

bool	GapBuffer::isBufferFull()
{
	if (byteLastIndex_ == bufferSize_ - 1)
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

	std::copy(buffer_, buffer_ + gapStart_, newBuffer);
	for (size_t i = gapStart_; i < tailStart_; i++)
		newBuffer[i] = 0;
	size_t tailSize = tailEnd_ - tailStart_;
	std::copy(buffer_ + tailStart_, buffer_ + tailEnd_, newBuffer + newGapEnd);

	delete [] buffer_;
	buffer_ = newBuffer;
	bufferSize_ = newBufferSize;
	tailStart_ = newGapEnd;
	tailEnd_ = tailStart_ + tailSize;
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

size_t	GapBuffer::getGapSize() const { return (tailStart_ - gapStart_); }

size_t	GapBuffer::getLastByteIndex() const { return (byteLastIndex_); }

size_t	GapBuffer::getTailStart() const { return (tailStart_); }

size_t	GapBuffer::getTailEnd() const { return (tailEnd_); }

size_t	GapBuffer::getByteCount() const { return (byteCount_); }

size_t	GapBuffer::getGapStart() const { return (gapStart_); }

size_t	GapBuffer::getBufferSize() const { return (bufferSize_); }

size_t	GapBuffer::getCharCount() const { return (charCount_); }

size_t	GapBuffer::getLastCharIndex() const { return (charLastIndex_); }

void	GapBuffer::calculateByteCount()
{
	size_t count = 0;
	size_t headSize = gapStart_;
	size_t tailSize = tailEnd_ - tailStart_;
	count = headSize + tailSize;
	byteCount_ = count;
}

void	GapBuffer::calculateByteLastIndex()
{
	size_t headSize = gapStart_;
	size_t tailSize = getTailByteSize();
	byteLastIndex_ = headSize + tailSize;
}

void	GapBuffer::calculateCharCount()
{
	charCount_ = byteCount_;
}

void	GapBuffer::calculateCharLastIndex()
{
	charLastIndex_ = byteLastIndex_;
}

void	GapBuffer::recalculateDerivedInfo()
{
	calculateByteCount();
	calculateByteLastIndex();
	calculateCharCount();
	calculateCharLastIndex();
}

void	GapBuffer::setBuffer(size_t index, char ch)
{
	assert(index == gapStart_);
	assert(buffer_[index] == 0);
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
	if (gapStart_ == 0 || byteCount_ == 0)
		return ;
	growGap();
}

void GapBuffer::moveBytesToHigherIndices(size_t newIndex)
{
	assert(gapStart_ + getGapSize() + getTailByteSize() < bufferSize_);
	size_t gapSize = getGapSize();
	size_t bytesToMove = gapStart_ - newIndex;
	size_t newTailStart = tailStart_ - bytesToMove;
	size_t originalTailSize = tailEnd_ - tailStart_;
	size_t newTailEnd = newTailStart + bytesToMove + originalTailSize;
	assert(newTailStart == newIndex + gapSize);
	char tempArray[bytesToMove + 1];
	for (size_t i = newIndex, j = 0; i < gapStart_; i++, j++)
	{
		tempArray[j] = buffer_[i];
		buffer_[i] = '\0';
	}
	tempArray[bytesToMove] = '\0';
	for (size_t i = newTailStart, j = 0; j < bytesToMove; i++, j++)
			buffer_[i] = tempArray[j];
	gapStart_ = newIndex;
	tailStart_ = newTailStart;
	tailEnd_ = newTailEnd;
	cleanGap();
	recalculateDerivedInfo();
}

void GapBuffer::moveBytesToLowerIndices(size_t newIndex)
{
	assert(gapStart_ + getGapSize() + getTailByteSize() < bufferSize_);
	size_t gapSize = getGapSize();
	calculateByteLastIndex();
	if (newIndex > byteLastIndex_)
		newIndex = byteLastIndex_;
	size_t bufferNewIndex = tailStart_ + (newIndex - gapStart_);
	size_t tailSize = getTailByteSize();
	if (newIndex - gapStart_ > tailSize)
		bufferNewIndex = tailStart_ + tailSize;
	size_t bytesToMove = bufferNewIndex - tailStart_;
	if (bytesToMove == 0)
		return;
	char tempArray[bytesToMove + 1];
	for (size_t i = tailStart_, j = 0; i < bufferNewIndex; i++, j++)
	{
		tempArray[j] = buffer_[i];
		buffer_[i] = '\0';
	}
	tempArray[bytesToMove] = '\0';
	for (size_t i = gapStart_, j = 0; j < bytesToMove; i++, j++)
		buffer_[i] = tempArray[j];
	gapStart_ = gapStart_ + bytesToMove;
	tailStart_ = setTailStart(gapSize);
	tailEnd_ = tailStart_ + (tailSize - bytesToMove);
	recalculateDerivedInfo();
	cleanGap();
}

void GapBuffer::shiftTailBytesToHigherIndices(size_t newGapSize, size_t tailSize)
{
	assert(gapStart_ + getGapSize() + getTailByteSize() < bufferSize_);
	size_t tailDiff = gapStart_ + newGapSize - tailStart_;
	size_t newTailStart = tailStart_ + tailDiff;
	assert((gapStart_ + newGapSize + tailSize) < bufferSize_);
	size_t n = tailSize;
	assert(getTailByteSize() != tailDiff);
	char tempArray[n + 1];
	for (size_t i = tailStart_, j = 0; j < n; i++, j++)
	{
		tempArray[j] = buffer_[i];
		buffer_[i] = '\0';
	}
	tempArray[n] = '\0';
	for (size_t i = newTailStart, j = 0; j < n; i++, j++)
		buffer_[i] = tempArray[j];
	tailStart_ = newTailStart;
	tailEnd_ = newTailStart + n;
	recalculateDerivedInfo();
	cleanGap();
}

void GapBuffer::relocateGapTo(size_t newIndex)
{
	if (newIndex == gapStart_ || (byteCount_ == 0 && gapStart_ == 0) || (getGapSize() == bufferSize_ - 1))
		return ;
	if (newIndex < gapStart_)
		moveBytesToHigherIndices(newIndex);
	else if (newIndex > gapStart_)
		moveBytesToLowerIndices(newIndex);
}

size_t	GapBuffer::getTailByteSize()
{
	return (byteCount_ -  gapStart_);
}

void	GapBuffer::resizeGap()
{
	size_t newGapSize = GAP_SIZE;
	size_t tailSize = getTailByteSize();
	if (tailSize != 0 || (gapStart_ + newGapSize + tailSize >= bufferSize_ - 1))
	{
		resizeBuffer();
		return ;
	}
		shiftTailBytesToHigherIndices(newGapSize, tailSize);
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
		buffer_[i] = '\0';
}

std::string	GapBuffer::getVisibleText() const
{
	std::string visible;
	if (byteCount_ == 0)
		return (visible);
	visible.append(buffer_, gapStart_);
	size_t tailSize = tailEnd_ - tailStart_;
	visible.append(buffer_ + tailStart_, tailSize);
	return (visible);
}

void	GapBuffer::setCursorPosition(size_t newIndex)
{
	recalculateDerivedInfo();
	if (newIndex == gapStart_)
		return ;
	else if (newIndex < gapStart_)
		relocateGapTo(newIndex);
	else if (newIndex > byteCount_)
		relocateGapTo(byteLastIndex_);
	else if (newIndex > gapStart_)
		relocateGapTo(tailStart_ + (newIndex - gapStart_));
}

GapBuffer::GapBufferException::GapBufferException(const std::string &what_arg)
	: what_(what_arg) {}

const char* GapBuffer::GapBufferException::what() const noexcept
{
	return (what_.c_str());
}

void	GapBuffer::deleteSelection(size_t start, size_t end)
{
	if (start == end)
		return ;
	if (start > end)
		std::swap(start, end);
	setCursorPosition(end);
	for (size_t i = end; i >= start; i--)
		remove();
}

void	GapBuffer::paste(std::string &newContent, size_t cursorPosition)
{
	setCursorPosition(cursorPosition);
	for (size_t i = 0; i < newContent.size(); i++)
		insert(newContent[i]);
}

void	GapBuffer::assertInvariants() const
{
	assert(gapStart_ <= tailStart_);
	assert(tailEnd_ <= bufferSize_);
	assert(byteCount_ == gapStart_ + (tailEnd_ - tailStart_));
	assert(charCount_ == byteCount_);
	assert(charLastIndex_ == byteLastIndex_);
}
