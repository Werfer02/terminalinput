#include "inputManager.hpp"
#include "renderScreen.hpp"

#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>

int main(){
    tr::RenderScreen scr(32, 32);
    ti::InputManager in;
    tr::point p {16, 16};
    tr::point c {31, 31};

    in.registerKey('w', [&] () {
        p.y -= 1;
    });
    in.registerKey('a', [&] () {
        p.x -= 1;
    });
    in.registerKey('s', [&] () {
        p.y += 1;
    });
    in.registerKey('d', [&] () {
        p.x += 1;
    }); 

    ti::mouseEventType latest;
    std::vector<tr::point> drawn;
    in.registerMouseEvent(ti::mouseEventType::Drag, [&] (ti::mouseEvent m) {
        int x = std::max(m.x/2 - 1, 0);
        int y = std::max(m.y - 1, 0);
        if(x < 32 && y < 32){
            drawn.push_back({x, y});
        }
        latest = m.type;
    });
    in.registerMouseEvent(ti::mouseEventType::Hover, [&] (ti::mouseEvent m) {
        int x = std::max(m.x/2 - 1, 0);
        int y = std::max(m.y - 1, 0);
        if(x < 32 && y < 32) c = {x, y};
        latest = m.type;
    });

    in.registerMouseEvent(ti::mouseEventType::Click, [&] (ti::mouseEvent m) {
        latest = m.type;
    });
    in.registerMouseEvent(ti::mouseEventType::Release, [&] (ti::mouseEvent m) {
        latest = m.type;
    });

    bool running = true;
    in.registerKey('q', [&] () {
        running = false;
    });
    in.registerKey('c', [&] () {
        drawn.clear();
    });

    in.enableCapture();
    std::thread input([&] () {
        char c;
        std::string buffer;
        while (running) {
            if (!std::cin.get(c)) break;
            buffer += c;
            in.handleInput(in.parseCinBuffer(buffer));
        }
    });

    std::cout << ansi::hide_cursor;
    while(running){
        scr.clear();
        scr.setDrawColor(tr::color::blue);
        for(auto p : drawn){
            scr.drawCircle(p, 0);
        }
        scr.drawCircle(p, 1, tr::color::green);
        scr.drawCircle(c, 0, tr::color::red);

        switch(latest){
            case ti::mouseEventType::Click : {
                scr.putPixel({0,0}, tr::color::yellow);
                break;
            }
            case ti::mouseEventType::Release : {
                scr.putPixel({1,0}, tr::color::magenta);
                break;
            }
            case ti::mouseEventType::Hover : {
                scr.putPixel({2,0}, tr::color::red);
                break;
            }
            case ti::mouseEventType::Drag : {
                scr.putPixel({3,0}, tr::color::blue);
                break;
            }
        }

        std::cout << ansi::cursor_topleft;
        scr.doublePrint();
        std::cout << "wasd to move, mouse to draw, c to clear, q to quit \n";
        std::cout << "last mousEventType indicator in top left:\n";
        std::cout << ansi::yellow  << "██ yellow " << ansi::reset << " is ti::mouseEventType::Click\n";
        std::cout << ansi::magenta << "██ magenta" << ansi::reset << " is ti::mouseEventType::Release\n";
        std::cout << ansi::red     << "██ red    " << ansi::reset << " is ti::mouseEventType::Hover\n";
        std::cout << ansi::blue    << "██ blue   " << ansi::reset << " is ti::mouseEventType::Drag\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    input.join();
    std::cout << ansi::show_cursor << ansi::cursor_topleft << ansi::clear;
    std::cout << ansi::yellow << "exiting...";
}