#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>

int main(){
    
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    while (true){
        system("xdotool mousemove 2386 490");
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        system("xdotool click 1");
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        system("xdotool mousemove_relative 100 0");
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        system("xdotool click 1");
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        std::cout << "loop" << std::endl;
    }
}
