#pragma once

#include <sstream>
#include <functional>
#include <algorithm>
#include <unordered_map>

#include "coututils.hpp"

namespace ti {

enum class mouseEventType {
    Click,
    Release,
    Drag,
    Hover,
    Wheel
};

struct mouseEvent {
    bool valid = false;
    int button;

    int x;
    int y;

    mouseEventType type;

    bool shift;
    bool alt;
    bool ctrl;
};

struct inputEvent {
    bool valid = false;
    mouseEvent m;
    char key;
};

class InputManager {
    std::unordered_map<char, std::function<void()>> registeredKeys;
    std::unordered_map<mouseEventType, std::function<void(const mouseEvent&)>> registeredMouseEvents;
public:
    ~InputManager();
    static void enableCapture();
    static void disableCapture();

    void registerKey(char c, std::function<void()> callback);
    void registerMouseEvent(mouseEventType type, std::function<void(const mouseEvent&)> callback);

    inputEvent parseCinBuffer(std::string& buffer) const;
    void handleInput(const inputEvent& in);
};

}