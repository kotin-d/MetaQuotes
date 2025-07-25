#pragma once

#include "CLogReader.h"

// ����� ����-������. ����� ���������� ��� ��� ������
// ����� �����������, ������� ������� ����-������ �� �����������
class UnitTest
{
public:
	void FullTest();

	void StringTest();

	void TrivialTest();
	void KmpTest();
	void FilterGroupTest();
	void ContainTest();
	void CheckTest();

private:
	void Equal(int value1, int value2) const;
	void Equal(bool value1, bool value2) const;

private:
	const char* mFileName = "log.txt";
};

