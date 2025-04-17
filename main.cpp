#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <atomic>
#include <cassert>
#include <ostream>
#include <thread>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <X11/keysym.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <stdexcept>
#include <memory>
#include <array>
#include <cstring>

struct Pixel{
    int x;
    int y;
    int red;
    int green;
    int blue;
};

std::atomic<bool> start_auto_cycle{false};
std::atomic<int> x{0};

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;    
    using PipeCloser = int(*)(FILE*);
    std::unique_ptr<FILE, PipeCloser> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

void auto_cycle() {
    int delay = 2200;
    while(true) {

        auto start = std::chrono::system_clock::now();
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);

        if (start_auto_cycle.load()) {
            std::cout << "autofarm loop: " << std::ctime(&end_time);
            system("xdotool mousemove 829 520");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            system("xdotool click 1");
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            system("xdotool mousemove_relative 70 0");
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            system("xdotool click 1");
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            system("xdotool keydown r");
            std::this_thread::sleep_for(std::chrono::milliseconds(delay+x.load())); // + x
            system("xdotool keyup r");
            std::this_thread::sleep_for(std::chrono::milliseconds(201));
            //std::cout << "break delay: " << delay+x.load() << std::endl;
            start_auto_cycle.store(false);
       }
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); 
    }
}

int level;

void break_position() {
    std::cout << "Going to break position" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    system("xdotool keydown 'a' && sleep 0.5 && xdotool keyup 'a'");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    system("xdotool keydown 'a' && sleep 0.5 && xdotool keyup 'a'");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    system("xdotool keydown 'd' && sleep 0.1 && xdotool keyup 'd'");
    for (int j = 0;j < level; j++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        system("xdotool keydown w && sleep 0.1 && xdotool keyup w");
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    system("xdotool keydown 'a' && sleep 0.03 && xdotool keyup 'a'");    

    std::cout << "Position reached!" << std::endl;
}

void escape_news() {
    std::cout << "Closing news" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    system("xdotool mousemove 1049 411 && xdotool mousedown 1 && sleep 0.5 && xdotool mouseup 1");
    std::cout << "Closed news!" << std::endl;
}

void select_lgrid() {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    system("sleep 0.5 && xdotool mousemove 920 884 && xdotool click 1");
}

int main() {

    //break_position();
    //escape_news();
    
    std::thread t1(auto_cycle);

    int number_of_screens;
    Bool result;
    Window *root_windows;
    Window window_returned;
    Window window;
    int root_x, root_y;
    int win_x, win_y;
    unsigned int mask_return;
    XEvent event;
    Display *display = XOpenDisplay(nullptr);
    window = DefaultRootWindow(display);

    int screen = DefaultScreen(display); 

    XSelectInput(display, window, KeyPressMask);
    assert(display);
    number_of_screens = XScreenCount(display);
 
    int pixel_count = 0;
    std::vector<XColor> c{};
    std::vector<XImage*> image{};
    std::vector<Pixel> pixel{}; 
    
    //window = exec("xwininfo -name 'Growtopia' | grep 'Window id:' | awk '{print $4}'"):

    std::string grow_window = exec("xwininfo -name 'Growtopia' | grep 'Window id:' | awk '{print $4}'");
    char p_0[80];
    strncpy(p_0, grow_window.c_str(), sizeof(p_0));
    p_0[sizeof(p_0) - 1] = '\0';
        
    window = (Window)strtoul(p_0, NULL, 0);
    
    std::cout << "Window found: " << grow_window << std::endl;
    
    std::cout << "Level (1): ";
    std::cin >> level;

    std::cout << "(L)oad or (n)ew?" << ' ';
    char choice;
    std::cin >> choice;

    if (choice == 'n'){
        std::cout << "(w)indow or (r)oot?" << ' ';
        char win_or_root;
        std::cin >> win_or_root;
        /*if (win_or_root == 'w'){
            std::cout << "window id: ";
            char p[80];        
            std::cin >> p;
            window = (Window)strtoul(p, NULL, 0);
            }*/
        XSelectInput(display, window, KeyPressMask);

        std::cout << "Pixel count: ";
        std::cin >> pixel_count;

        pixel.reserve(pixel_count);
        int count_times = 0;

        std::cout << "Pixel positions: " << std::endl;
        while (count_times < pixel_count){
            XNextEvent(display, &event);

            if (event.type == KeyPress){
                char buffer[10];
                KeySym keysym;
                int num_chars = XLookupString(&event.xkey, buffer, sizeof(buffer) - 1, &keysym, NULL);
                buffer[num_chars] = '\0';

                if (keysym == XK_space) {
                    root_windows = (Window*)malloc(sizeof(Window) * number_of_screens);
                    for (int i = 0; i < number_of_screens; ++i){
                        root_windows[i] = XRootWindow(display, i);
                    }

                    for (int i = 0; i < number_of_screens; ++i){
                        result = XQueryPointer(display, root_windows[i], &window_returned, &window_returned,
                                               &root_x, &root_y, &win_x, &win_y, &mask_return);
                        if (result == True){
                            break;
                        }
                    }
                    pixel[count_times].x = root_x;
                    pixel[count_times].y = root_y;
                    
                    std::cout << pixel[count_times].x << ' ' << pixel[count_times].y << std::endl; 

                    count_times++;
                }
            }
        }
    }
    if (choice != 'n' && choice != 'l')
        return 0; 
    if (choice == 'l') {
        std::ifstream file("coords.txt");
        pixel_count = 0;
        while (true) {
            Pixel p;
            file >> p.x;
            file >> p.y;
            if(file.eof()) break;
            pixel.push_back(p);
            pixel_count++;
        }
        std::cout << "Pixel count: " << pixel_count << std::endl;

        for (int i = 0; i < pixel_count;i++) {
            std::cout << pixel[i].x << ' ' << pixel[i].y << std::endl;
        }
    } 
    int correct_pixel = 0;
    int breaking_count = 0;

    for (int i = 0;i < pixel_count;i++) {
        pixel[i].red = 255;
        pixel[i].green = 255;
        pixel[i].blue = 255;
    }

    std::cout << "Checking for value..." << std::endl;

    c.reserve(pixel_count);
    image.reserve(pixel_count);

    bool cooldown_start{false};
    bool detected_white{true};
    bool detected_place{true};
    int cooldown = 0;
    int place_count = 0;
    int cooldown2 = 0;
    bool cooldown2_start{false};
    bool need_jump{false};

    bool max_item_timer_start{false};
    int max_item_timer = 0;
    
    main_label:

    std::cout << "main while loop" << std::endl;

    while(true){
        for (int i = 0;i<pixel_count;i++) {
            image[i] = XGetImage(display, XRootWindow (display, XDefaultScreen (display)), pixel[i].x, pixel[i].y, 1, 1, AllPlanes, XYPixmap);
            c[i].pixel = XGetPixel(image[i], 0, 0);
            XFree(image[i]);
            XQueryColor (display, XDefaultColormap(display, XDefaultScreen (display)), &c[i]);
        }

        for (int i = 0;i < pixel_count;i++) {
            if (c[i].red/256 == pixel[i].red &&
                c[i].green/256 == pixel[i].green &&
                c[i].blue/256 == pixel[i].blue){

                correct_pixel++;
            }
        }
        if (c[4].red/256 != 96 &&
            c[4].green/256 != 242 &&
            c[4].blue/256 != 215){

            std::cout << "Paused" << std::endl;
            break;
           //return 0;
        }

        
        if ((c[5].red/256 != 39 &&
            c[5].green/256 != 125 &&
            c[5].blue/256 != 159) && max_item_timer >= 1000){

            std::cout << "Empty!" << std::endl;
            need_jump = true;
            break;
            //return 0;
        }
        
        if ((c[5].red/256 == 39 &&
            c[5].green/256 == 125 &&
            c[5].blue/256 == 159)){

            max_item_timer = 0;
            max_item_timer_start = true;
            //return 0;
        }
        
        if (correct_pixel == pixel_count-1) {
            cooldown = 0;
            cooldown_start = true;
            detected_place = false;
        }
        correct_pixel = 0;
        if (cooldown_start){
            cooldown++;
        }
        if (cooldown2_start && cooldown2 < 500){
            cooldown2++;
        }

        if (max_item_timer_start) {
            max_item_timer++;
        }

        //std::cout << "Max item timer:" << max_item_timer << std::endl;
        
        if (!detected_place && cooldown == 40) {
            x.store(cooldown2);
            cooldown2 = 0;
            cooldown2_start = true;
            cooldown_start = false;
            detected_place = true;
        }
        start_auto_cycle.store(true);
    }

    //
    
    if (need_jump) {
        max_item_timer = 0;
        max_item_timer_start = false;
        need_jump = false;
        system("sleep 0.5 && xdotool keydown w && sleep 0.1 && xdotool keyup w");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
	level++;
        goto main_label;
    }

    bool world_timer_start{false};
    int world_timer = 0;
    
    window = (Window)strtoul(p_0, NULL, 0);
    XSelectInput(display, window, KeyPressMask);

    bool found_dirt{false};
    
    while(true) {

        for (int i = 0;i<pixel_count;i++) {
            image[i] = XGetImage(display, XRootWindow (display, XDefaultScreen (display)), pixel[i].x, pixel[i].y, 1, 1, AllPlanes, XYPixmap);
            c[i].pixel = XGetPixel(image[i], 0, 0);
            XFree(image[i]);
            XQueryColor (display, XDefaultColormap(display, XDefaultScreen (display)), &c[i]);
        }
        
        //std::cout << "Loop" << std::endl;
        
        while(XPending(display)) {
            XNextEvent(display, &event);    
        }

        if (c[6].red/256 == 46 && // 74 54 34
            c[6].green/256 == 113 &&
            c[6].blue/256 == 137 && !found_dirt){
            found_dirt = true;
            std::cout << "Found dirt" << std::endl;
            escape_news();

        }
        
        if ((c[4].red/256 == 96 &&
            c[4].green/256 == 215 &&
            c[4].blue/256 == 242) &&

	    c[7].red/256 == 96 &&
            c[7].green/256 == 215 &&
            c[7].blue/256 == 242){

            //std::cout << "hello" << std::endl;
            
            world_timer++;
            std::cout << "World timer:" << world_timer << std::endl;
        }
        else {
            world_timer = 0;
        }
        
        if (world_timer >= 200) {
            world_timer_start = false;

            std::cout << "Selecting lgrid" << std::endl;
            select_lgrid();
            
            std::cout << "Ready to go to break position" << std::endl;
            //return 0;
            
            break_position();

            goto main_label;
        }
        
        if (event.type == KeyPress) {
            char buffer[10];
            KeySym keysym;int num_chars = XLookupString(&event.xkey, buffer, sizeof(buffer) - 1, &keysym, NULL);
            buffer[num_chars] = '\0';

            if (keysym == XK_v) {
                std::cout << "found v" << std::endl;
                goto main_label;
            }    
        }

        /*char c;
        std::cout << "Continue: ";
        std::cin >> c;

        if (c == 'c') {
            std::cout << "Continuing" << std::endl;
            goto label;
        }*/
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    std::cout << "End" << std::endl;

    free(root_windows);
    XCloseDisplay(display);
}
