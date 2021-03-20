#include "pch.h"
#include "CppUnitTest.h"
#include "../Censor/Censor.h"
//#include "../Censor/Censor.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Native_Tests
{
	TEST_CLASS(Tests)
	{
	public:
		Censor c = Censor();


		TEST_METHOD(Test_empty_string) {
			std::string word = c.censor_line("");
			Assert::AreEqual(0, word.compare(""));
		}

		TEST_METHOD(Test_string_with_punctuation) {
			std::string word = c.censor_line("Hello, here is my purchase list: eggs, butter, cheese!");
			Assert::AreEqual(0, word.compare("Hello, here is my purchase list: eggs, butter, cheese!"));
		}
		TEST_METHOD(Test_string_with_uneven_spaces) {
			std::string word = c.censor_line("Hello  my name is John ");
			Assert::AreEqual(0, word.compare("Hello  my name is John "));
		}
		TEST_METHOD(Test_swear_all_lower)
		{
			std::string word = c.censor_line("shit");
			Assert::AreEqual(0, word.compare("****"));
		}
		TEST_METHOD(Test_swear_all_capital) {
			std::string word = c.censor_line("SHIT");
			Assert::AreEqual(0, word.compare("****"));
		}
		TEST_METHOD(Test_swear_while_preserving_capitol) {
			std::string word = c.censor_line("Why the fuck are you here?");
			Assert::AreEqual(0, word.compare("Why the **** are you here?"));
		}

		TEST_METHOD(Test_swear_with_special_symbols) {
			std::string word = c.censor_line("This $hitty game is annoying.");
			Assert::AreEqual(0, word.compare("This ****** game is annoying."));
		}

		TEST_METHOD(Test_Censor) {
			std::string word = "shit";
			std:: string s = c.censor_line("shit");
			Assert::AreEqual(0, s.compare("****"));
		}

		/*
				TEST_METHOD(Test_swear_all_lower_word) {
			std::string word = c.censor_word("fuck");
			Assert::AreEqual(0, word.compare("****"));
		}
		*/
	};
}

