// Censor.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

class Censor {
    public:
        std::vector<std::string> foul_language;
        std::vector<std::string> partial_foul_language;

    Censor() {
        std::ifstream file("Foul_Language.txt");
        std::string line;
        if (file.is_open()) {
            while (file >> line) {
                foul_language.push_back(line);
            }
        }
        else {
            std::cout << "File not found.";
        }

        //std::ifstream file("Partial_Foul_Language.txt");
        //if (file.is_open()) {
        //    while (file >> line) {
        //        partial_foul_language.push_back(line);
        //    }
        //}
        //else {
        //    std::cout << "File not found.";
        //}
    }

    //findCaseInsensitive(std::string data, std::string toSearch, size_t pos = 0)
    //{
    //    // Convert complete given String to lower case
    //    std::transform(data.begin(), data.end(), data.begin(), ::tolower);
    //    // Convert complete given Sub String to lower case
    //    std::transform(toSearch.begin(), toSearch.end(), toSearch.begin(), ::tolower);
    //    // Find sub string in given string
    //    return data.find(toSearch, pos);
    //}
    //~Censor()

    


    //std::string censor_line(std::string line) {
    //    std::stringstream ss(line);
    //    std::string buf;
    //    std::string censored_line = "";
    //    while (ss >> buf) {
    //        censored_line += censor_word(buf) + " ";
    //    }

    //    censored_line.pop_back(); //removes last space from line
    //    return censored_line;
    //}

    std::string censor_line(std::string line) {

        //do not need to replace if there is * there.
        //std::string line = "FUCKingF"; // b1tch ass shit.";
        line = censor_foul(line);
        line = censor_partial_foul(line);
        return line;
        
    }

    std::string censor_foul(std::string line) {
        std::vector<int> capitol_letter_indices = std::vector<int>();
        for (int i = 0; i < line.length(); i++) {
            if (isupper(line[i])) {
                capitol_letter_indices.push_back(i);
                line[i] = tolower(line[i]);

            }
        }


        for (std::string curse_word : foul_language) {

            int find = line.find(curse_word);
            while (find != -1) {
                std::string bleep = ""; //for repeats, this is inefficient, maybe add if
                for (int i = 0; i < curse_word.length(); i++) { //I can use line.length() if I want to censor the entire word, not just the root part.
                    bleep += "*";
                }
                line.replace(find, curse_word.length(), bleep);
                find = line.find(curse_word);
            }

        }


        for (int i : capitol_letter_indices) {
            line[i] = toupper(line[i]);
        }
        return line;
    }

    std::string censor_partial_foul(std::string line) {
        std::vector<std::string> partial_foul_language = std::vector<std::string>();

        std::ifstream file2("Partial_Foul_Language.txt");
        std::string next;
        if (file2.is_open()) {
            while (file2 >> next) {
                partial_foul_language.push_back(next);
            }
        }
        else {
            std::cout << "File not found.";
        }

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

    //void writeFile() {
    //    //creates file if does not exist
    //    std::ofstream file("Test.txt");
    //    std::vector < std::string> v;
    //    v.push_back("Alpha");
    //    v.push_back("Bravo");
    //    v.push_back("Charlie");
    //    for (std::string s : v) {
    //        file << s << std::endl;
    //    }
    //    file.close();
    //}
    
}; //end Censor class

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
#pragma endregion All the Test Cases for Censor class
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
}




int main()
{

    //Censor c = Censor();

    ////test_vector();
    ////test_capital(); 
    //c.censor_line("Suck");

    //std::ifstream file("Message2.txt");
    //std::string line;
    //if (file.is_open()) {
    //    std::vector<std::string> foul_language;
    //    std::string input;
    //    std::string message;
    //    while (file >> input) {
    //        //will interpret file until input receives an invalid input, namely, white space.
    //        //if input was a char, then you get every char. If input was int, will stop until non-int.
    //        std::string s = c.censor(input);
    //        std::cout << s << std::endl;
    //    }
    //}

    

    //else {
    //    std::cout << "file not found.";
    //}

    //std::string word = "shit";
    //std::cout << word << std::endl;
    //word = c.censor_line("shit");
    //std::cout << word << std::endl;

    //std::string line = "This fucking game is shit.";
    //std::cout << c.censor_line(line) << std::endl;
    //std::cout << c.strict_censor_line(line) << std::endl;
    //std::cout << line;
    
    //std::cout << word.compare("****");
    //std::cout << word + "_";
    //std::cout << "Why the **** are you here?";
    //std::string word = "Why the fuck are you here?";

    //test_partial("hello hell hello hell hell");



    //std::cout << word;
    run_tests();
    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file