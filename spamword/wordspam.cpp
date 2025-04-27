#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <thread>

std::random_device seed;
std::mt19937 gen{seed()};

void spamwords() {
    int max_word_count = 20;
    int min_word_count = 5;
    int unique_words = 352;
    
    std::uniform_int_distribution<> dist{min_word_count, max_word_count};
    std::uniform_int_distribution<> dist2{0, unique_words};

    std::string words[unique_words];
    std::ifstream file("final_conversational_1000_words.txt");

    for (int i = 0;i < unique_words;i++) {
        file >> words[i];
    }
    
    /*for (int i = 0;i < unique_words;i++) {
        std::cout << words[i] << ' ' << std::endl;
        }*/

    /*for (int i = 0;i < word_count;i++) {
        print_text = print_text + words[i] + ' ';    
        }*/
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    while (true) {
        std::cout << "Words" << std::endl;
        
        int word_count = dist(gen);

        system("xdotool key 'Enter'");

        for (int i = 0;i < word_count;i++) {
            int random_word = dist2(gen);
            std::string command_string = "xdotool type " + words[random_word];
            const char* print_command = command_string.c_str();
            system(print_command);
            system("xdotool key space");
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        system("xdotool key 'Enter'");

        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    }
    //std::cout << command_string << std::endl;

    /*
    system("sleep 1 && xdotool key 'Enter'");
    system(print_command);
    system("xdotool key 'Enter'");
    */
    
    //std::cout << print_text << std::endl;
    
    /*
    system("sleep 1 && xdotool key 'Enter'");
    system(print_command);
    system("xdotool key 'Enter'");
    */
    
    
}

int main() {
    spamwords();
}
