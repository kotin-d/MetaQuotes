#pragma once

#include <stdio.h>


// класс строки
class String
{
public:
	String();
	String(const String& rhs);
	String(const char* rhs);
	String& operator= (const String& rhs);
	String& operator= (const char* rhs);
	~String();

	size_t size() const { return mSize; }
	char* buffer() const { return mBuffer; }
	void clear();

	void append(const char symbol);
	bool lineFromFile(FILE* file);

	const char& operator[] (size_t index) const;

private:
	void resize();
	void createBuffer();

	const double RESIZE_RATE = 1.4;
	const int START_SIZE = 32;

	size_t mSize = 0;
	size_t mReserve = 0;

	char* mBuffer = nullptr;
};


namespace  utils
{
	int strlen(const char* str);
	void strcpy(char* dst, const char* src);
	void strncpy(char* dst, const char* src, int size);
	void memncpy(char* dst, const char* src, int size);
} // namespace utils