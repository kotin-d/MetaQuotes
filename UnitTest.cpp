#include "UnitTest.h"
#include <iostream>
#include "String.h"


void UnitTest::FullTest() {
	TrivialTest();
	KmpTest();
	FilterGroupTest();
	ContainTest();
	CheckTest();
}

void UnitTest::StringTest() {
	FILE* file = nullptr;
	file = fopen(mFileName, "r");

	String str;

	while (str.lineFromFile(file)) {
		std::cout << str.buffer() << std::endl;
	}

	fclose(file);
}


void UnitTest::TrivialTest()
{
	std::cout << "Trivial test" << std::endl;

	CLogReader reader;

	Equal(reader.trivialMatch("a", "abc"), 0);
	Equal(reader.trivialMatch("b", "abc"), 1);
	Equal(reader.trivialMatch("c", "abc"), 2);

	Equal(reader.trivialMatch("g", "Big string"), 2);
	Equal(reader.trivialMatch("s", "asasas"), 1);

	Equal(reader.trivialMatch("lemon", "apple lemon orange"), 6);
	Equal(reader.trivialMatch("ge", "apple lemon orange"), 16);

	Equal(reader.trivialMatch("???", "abc"), 0);
	Equal(reader.trivialMatch("a??", "abc"), 0); 
	Equal(reader.trivialMatch("?b?", "abc"), 0);
	Equal(reader.trivialMatch("?c", "abc"), 1);

	Equal(reader.trivialMatch("l?m?n", "apple lemon orange"), 6);
}


void UnitTest::KmpTest()
{
	std::cout << "KMP test" << std::endl;

	CLogReader reader;
	PrFunction func;

	func.calculate("a");
	Equal(reader.kmpMatch("a", "abc", func), 0);

	func.calculate("b");
	Equal(reader.kmpMatch("b", "abc", func), 1);

	func.calculate("c");
	Equal(reader.kmpMatch("c", "abc", func), 2);

	func.calculate("g");
	Equal(reader.kmpMatch("g", "Big string", func), 2);

	func.calculate("s");
	Equal(reader.kmpMatch("s", "asasas", func), 1);

	func.calculate("lemon");
	Equal(reader.kmpMatch("lemon", "apple lemon orange", func), 6);

	func.calculate("ge");
	Equal(reader.kmpMatch("ge", "apple lemon orange", func), 16);

	func.calculate("???");
	Equal(reader.kmpMatch("???", "abc", func), 0);

	func.calculate("a??");
	Equal(reader.kmpMatch("a??", "abc", func), 0);
	
	func.calculate("?b?");
	Equal(reader.kmpMatch("?b?", "abc", func), 0);
	
	func.calculate("?c");
	Equal(reader.kmpMatch("?c", "abc", func), 1);

	func.calculate("l?m?n");
	Equal(reader.kmpMatch("l?m?n", "apple lemon orange", func), 6);
}

void UnitTest::FilterGroupTest()
{
	std::cout << "filter group test" << std::endl;
	CLogReader reader;

	reader.SetFilter("*");
	Equal(reader.mFilterItemCount, 1);

	reader.SetFilter("abc");
	Equal(reader.mFilterItemCount, 1);

	reader.SetFilter("**");
	Equal(reader.mFilterItemCount, 2);

	reader.SetFilter("***");
	Equal(reader.mFilterItemCount, 3);

	reader.SetFilter("*abc*");
	Equal(reader.mFilterItemCount, 3);

	reader.SetFilter("*a*b*c*");
	Equal(reader.mFilterItemCount, 7);

	reader.SetFilter("*a**b**c*");
	Equal(reader.mFilterItemCount, 9);

	reader.SetFilter("*abc*abc*cab*");
	Equal(reader.mFilterItemCount, 7);

	reader.SetFilter("*abc***abc***cab*");
	Equal(reader.mFilterItemCount, 11);

	reader.SetFilter("a?c");
	Equal(reader.mFilterItemCount, 1);

	reader.SetFilter("*a?c*");
	Equal(reader.mFilterItemCount, 3);

	reader.SetFilter("*a?c***ab?***cab*");
	Equal(reader.mFilterItemCount, 11);
}

void UnitTest::ContainTest()
{
	std::cout << "contain test" << std::endl;
	CLogReader reader;

	Equal(reader.isContain("sas", "sas", 0), true);
	Equal(reader.isContain("as", "sas", 1), true);
	Equal(reader.isContain("sasa", "sas", 0), false);
	Equal(reader.isContain("sas", "sas", 1), false);
	Equal(reader.isContain("ss", "sas", 1), false);
	Equal(reader.isContain("la", "tra-la-la", 4), true);
	Equal(reader.isContain("li", "tra-la-la", 4), false);
	Equal(reader.isContain("la", "tra-la-la", 7), true);
	Equal(reader.isContain("la", "tra-la-la", 8), false);

	Equal(reader.isContain("s?s", "sas", 0), true);
	Equal(reader.isContain("l?", "tra-la-la", 4), true);
}

void UnitTest::CheckTest()
{
	std::cout << "check test" << std::endl;
	CLogReader reader;

	reader.SetFilter("*");
	Equal(reader.check("a"), true);
	Equal(reader.check("abc"), true);
	Equal(reader.check("abcabcabc"), true);

	reader.SetFilter("abc");
	Equal(reader.check("a"), false);
	Equal(reader.check("abc"), true);
	Equal(reader.check("abcd"), false);
	Equal(reader.check("abcabcabc"), false);

	reader.SetFilter("*abc");
	Equal(reader.check("a"), false);
	Equal(reader.check("abc"), true);
	Equal(reader.check("babc"), true);
	Equal(reader.check("bbabc"), true);
	Equal(reader.check("abcbb"), false);
	Equal(reader.check("bdabcbd"), false);

	reader.SetFilter("**abc");
	Equal(reader.check("a"), false);
	Equal(reader.check("abc"), true);
	Equal(reader.check("babc"), true);
	Equal(reader.check("bbabc"), true);
	Equal(reader.check("abcbb"), false);
	Equal(reader.check("bdabcbd"), false);

	reader.SetFilter("abc*");
	Equal(reader.check("a"), false);
	Equal(reader.check("abc"), true);
	Equal(reader.check("abcb"), true);
	Equal(reader.check("abcbb"), true);
	Equal(reader.check("bbabc"), false);
	Equal(reader.check("bdabcbd"), false);

	reader.SetFilter("*abc*");
	Equal(reader.check("a"), false);
	Equal(reader.check("abc"), true);
	Equal(reader.check("abcb"), true);
	Equal(reader.check("abcbb"), true);
	Equal(reader.check("bbabc"), true);
	Equal(reader.check("bdabcbd"), true);

	reader.SetFilter("a*b");
	Equal(reader.check("ab"), true);
	Equal(reader.check("abc"), false);
	Equal(reader.check("acfcdfdb"), true);
	Equal(reader.check("abab"), true);
	Equal(reader.check("ababab"), true);

	reader.SetFilter("*ab");
	Equal(reader.check("ababab"), true);

	reader.SetFilter("*a?c*");
	Equal(reader.check("a"), false);
	Equal(reader.check("abc"), true);
	Equal(reader.check("abcb"), true);
	Equal(reader.check("abcbb"), true);
	Equal(reader.check("bbabc"), true);
	Equal(reader.check("bdabcbd"), true);

	reader.SetFilter("*a?");
	Equal(reader.check("ababab"), true);
}

void UnitTest::Equal(int value1, int value2) const
{
	std::cout << ((value1 == value2) ? "Ok" : "Fail") << std::endl;
}

void UnitTest::Equal(bool value1, bool value2) const
{
	std::cout << ((value1 == value2) ? "Ok" : "Fail") << std::endl;
}
