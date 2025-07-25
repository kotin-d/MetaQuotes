#include "String.h"


namespace  utils
{

	int strlen(const char* str)
	{
		int len = 0;
		while (*str++ != '\0')
			++len;
		return len;
	}

	void strcpy(char* dst, const char* src)
	{
		while (*src != '\0')
			*dst++ = *src++;
		*dst = '\0';
	}

	void strncpy(char* dst, const char* src, int size)
	{
		while (*src != '\0' && size-- > 0)
			*dst++ = *src++;
		*dst = '\0';
	}

	void memncpy(char* dst, const char* src, int size)
	{
		while (*src != '\0' && size-- > 0)
			*dst++ = *src++;
	}

} // namespace utils


String::String()
{
}

String::String(const String& rhs) 
{
	mReserve = rhs.mReserve;
	mSize = rhs.mSize;
	mBuffer = new char[mReserve];
	utils::memncpy(mBuffer, rhs.mBuffer, mSize);
}

String::String(const char* rhs)
{
	mReserve = utils::strlen(rhs) + 1;
	mSize = mReserve;
	mBuffer = new char[mReserve];
	utils::strcpy(mBuffer, rhs);
}

String& String::operator= (const String& rhs) 
{
	if (&rhs != this) 
	{
		if (mBuffer)
			delete[] mBuffer;
		mReserve = rhs.mReserve;
		mSize = rhs.mSize;
		mBuffer = new char[mReserve];
		utils::memncpy(mBuffer, rhs.mBuffer, mSize);
	}
	return *this;
}

String& String::operator= (const char* rhs)
{
	if (rhs != mBuffer)
	{
		if (mBuffer)
			delete[] mBuffer;
		mReserve = utils::strlen(rhs) + 1;
		mSize = mReserve;
		mBuffer = new char[mReserve];
		utils::strcpy(mBuffer, rhs);
	}
	return *this;
}

String::~String()
{
	if (mBuffer)
		delete[] mBuffer;
}

void String::clear() 
{
	mSize = 0;
}

void String::append(const char symbol) 
{
	if (!mBuffer)
		createBuffer();

	if (mSize == mReserve)
		resize();

	mBuffer[mSize++] = symbol;
}

void String::resize() 
{
	mReserve = mReserve * RESIZE_RATE;
	char* buffer = new char[mReserve];
	utils::memncpy(buffer, mBuffer, mSize);
	delete[] mBuffer;
	mBuffer = buffer;
}

void String::createBuffer()
{
	mReserve = START_SIZE;
	mBuffer = new char[mReserve];
}

bool String::lineFromFile(FILE* file) 
{
	clear();
	int symbol;

	while ((symbol = fgetc(file)) != EOF) 
	{
		if (symbol == '\n') 
		{
			append('\0');
			return true;
		}
		else 
		{
			append(symbol);
		}
	}

	if (mSize == 0)
		return false;

	append('\0');
	return true;
}

const char& String::operator[] (size_t index) const
{
	return mBuffer[index];
}