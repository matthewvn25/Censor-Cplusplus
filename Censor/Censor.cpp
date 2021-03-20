// Censor.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

/**
* Censor class that censors text. Swear words are categorized as Foul and Parial Foul langauge.
* Foul language are words that should always be censored, whether the word stands on its own or any other word with the root swear word in this.
*  Ex. "fuck" Words like "fuck" "fuckary" "fucking" should all be censored
* Partial Foul are words that should only be censored when they stand on its own.
* Ex. "hell" words like "hello" "eggshell" "hellfire" should NOT be censored but "hell" would be censored.
*/
class Censor {
    public:
        std::vector<std::string> foul_language;
        std::vector<std::string> partial_foul_language;

    Censor() {
        std::ifstream file("Foul_Language.txt");
        std::string next;
        if (file.is_open()) {
            while (file >> next) {
                foul_language.push_back(next);
            }
        }
        else {
            std::cout << "File not found.";
        }
        file.close();

        std::ifstream file2("Partial_Foul_Language.txt");
       
        if (file2.is_open()) {
            while (file2 >> next) {
                partial_foul_language.push_back(next);
            }
        }
        else {
            std::cout << "File not found.";
        }
        file2.close();
    }
    //~Censor() make deallocator later

    /**
    * Censors the text
    *
    * @param line The line to censor.
    * @return the complete censored line.
    */
    std::string censor_line(std::string line) {
        line = censor_foul(line);
        line = censor_partial_foul(line);
        return line;
    }

    /**
    * Censors the text using swear words from Foul_Language.txt
    *
    * @param line The line to censor.
    * @return the censored line.
    */
    std::string censor_foul(std::string line) {
        //a copy of line is created to perform any neccessary work to check if some words should be censored without changing the actual line.
        std::string temp_line = line;

        std::vector<int> capitol_letter_indices = std::vector<int>();
        for (int i = 0; i < temp_line.length(); i++) {
            if (isupper(temp_line[i])) {
                capitol_letter_indices.push_back(i);
                temp_line[i] = tolower(temp_line[i]);

            }
        }
        temp_line = convert_special_chars(line);

        for (std::string curse_word : foul_language) {

            int find = temp_line.find(curse_word);
            while (find != -1) {
                std::string bleep = ""; //for repeats, this is inefficient, maybe add if
                for (int i = 0; i < curse_word.length(); i++) { //I can use line.length() if I want to censor the entire word, not just the root part.
                    bleep += "*";
                }
                line.replace(find, curse_word.length(), bleep);
                temp_line.replace(find, curse_word.length(), bleep);
                find = temp_line.find(curse_word);
            }
        }

        for (int i : capitol_letter_indices) {
            line[i] = toupper(line[i]);
        }
        return line;
    }

    /**
    * Censors the text using swear words from Partial_Foul_Language.txt
    *
    * @param line The line to censor.
    * @return the censored line.
    */
    std::string censor_partial_foul(std::string line) {

        for (std::string curse_word : partial_foul_language) {
            //std::string temp_line = line;
            std::string temp_line = convert_special_chars(line);
            int find = temp_line.find(curse_word);
            //std::cout << find << std::endl;
            while (find != -1) {
                std::string bleep = ""; //for repeats, this is inefficient, maybe add if
                for (int i = 0; i < curse_word.length(); i++) { //I can use line.length() if I want to censor the entire word, not just the root part.
                    bleep += "*";
                }

                if ((find - 1 == -1 || !isalpha(line[find - 1])) &&
                    (find + curse_word.length() == line.length() || !isalpha(line[find + curse_word.length()]))) {

                    line.replace(find, curse_word.length(), bleep);
                    temp_line.replace(find, curse_word.length(), bleep);

                }
                else {
                    temp_line.replace(find, curse_word.length(), bleep);
                }
                find = temp_line.find(curse_word);
            }
        }
        return line;
    }

    /**
    * Converts special characters into commonly their commonly associated letters.
    *
    * @param line The line to convert
    * @return the line with special characters converted into letters.
    */
    std::string convert_special_chars(std::string line) { //maybe private later?
        std::string s_lower = "";

        for (char c : line) {
            switch (c) {

            case('@'):
                s_lower += 'a';
                break;
            case ('$'):
                s_lower += 's';
                break;
            case ('0'):
                s_lower += 'o';
                break;
            case ('1'):
                s_lower += 'l';
                break;
            case ('3'):
                s_lower += 'e';
                break;
            case('4'):
                s_lower += 'h';
                break;
            default:
                s_lower += tolower(c);
                //break;
            }
        }
        return s_lower;
    }


}; //end Censor class

/**
* All the test cases used to test the program.
*/
#pragma region Test Cases
void Test_empty_string() {
    Censor c = Censor();
    std::string word = c.censor_line("");
    std::string soln = "";
    std::cout << "Test_empty_string: " << (word.compare(soln) == 0) << std::endl;
}

void Test_string_with_punctuation() {
    Censor c = Censor();
    std::string word = c.censor_line("Salutations, here is my purchase list: eggs, butter, cheese!");
    std::string soln = "Salutations, here is my purchase list: eggs, butter, cheese!";
    std::cout << "Test_string_with_punctuation: " << (word.compare(soln) == 0) << std::endl;
}

void Test_string_with_uneven_spaces() {
    Censor c = Censor();
    std::string word = c.censor_line("Hi  my name is John ");
    std::string soln = "Hi  my name is John ";
    std::cout << "Test_string_with_uneven_spaces: " << (word.compare(soln) == 0) << std::endl;
}

void Test_swear_all_lower() {
    Censor c = Censor();
    std::string word = c.censor_line("shit");
    std::string soln = "****";
    std::cout << "Test_swear_all_lower: " << (word.compare(soln) == 0) << std::endl;
}

void Test_swear_all_capital() {
    Censor c = Censor();
    std::string word = c.censor_line("SHIT");
    std::string soln = "****";
    std::cout << "Test_swear_all_capital: " << (word.compare(soln) == 0) << std::endl;
}

void Test_censor_preserve_capital() {
    Censor c = Censor();
    std::string word = c.censor_line("Why the fuck are you here?");
    std::string soln = "Why the **** are you here?";
    std::cout << "Test_censor_preserve_capital: " << (word.compare(soln) == 0) << std::endl;
}

void Test_swear_special_symbols() {
    Censor c = Censor();
    std::string word = c.censor_line("This $hitty game is annoying.");
    std::string soln = "This ****ty game is annoying.";
    std::cout << "Test_swear_special_symbol: " << (word.compare(soln) == 0) << std::endl;
}

void Test_swear_repeat() {
    Censor c = Censor();
    std::string word = c.censor_line("You fucking fuck.");
    std::string soln = "You ****ing ****.";
    std::cout << "Test_swear_repeat: " << (word.compare(soln) == 0) << std::endl;
}

void Test_hello() {
    Censor c = Censor();
    std::string word = c.censor_line("Hello, hello.");
    std::string soln = "Hello, hello.";
    std::cout << "Test_hello " << (word.compare(soln) == 0) << std::endl;
}

void Test_hell() {
    Censor c = Censor();
    std::string word = c.censor_line("The hell is this?");
    std::string soln = "The **** is this?";
    std::cout << "Test_hell " << (word.compare(soln) == 0) << std::endl;
}

void Test_hell_special_chars() {
    Censor c = Censor();
    std::string word = c.censor_line("The 4311 is this?");
    std::string soln = "The **** is this?";
    std::cout << "Test_hell_special_chars " << (word.compare(soln) == 0) << std::endl;
}

void Test_foul_and_partial() {
    Censor c = Censor();
    std::string word = c.censor_line("Hello, who the h3ll would fucking invite you?");
    std::string soln = "Hello, who the **** would ****ing invite you?";
    std::cout << "Test_foul_and_partial " << (word.compare(soln) == 0) << std::endl;
}

void Test_partial_hell() {
    Censor c = Censor();
    std::string word = c.censor_line("Hello, the eggshell is being cooked over the hellfire in hell.");
    std::string soln = "Hello, the eggshell is being cooked over the hellfire in ****.";
    std::cout << "Test_partial_hell " << (word.compare(soln) == 0) << std::endl;
}

#pragma endregion All the Test Cases for Censor class

/**
Runs all test cases.
*/
void run_tests() {
    std::cout << "1 = pass \t 0 = fail" << std::endl << std::endl;;
    Test_empty_string();
    Test_string_with_punctuation();
    Test_censor_preserve_capital();
    Test_string_with_uneven_spaces();
    Test_swear_all_capital();
    Test_swear_all_lower();
    Test_swear_special_symbols();
    Test_swear_repeat();
    Test_hello();
    Test_hell();
    Test_hell_special_chars();
    Test_foul_and_partial();
    Test_partial_hell();
}

int main()
{
    run_tests();
    return 0;
}