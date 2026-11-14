#include "inputManager.hpp"

namespace ti {

mouseEvent parseMouseCode(const std::string& input) { // assumes a properly formatted SGR code ([<BB;x;yM)
    int button, x, y;
    char ending;

    std::stringstream stream(input.substr(2));

    if (!(stream >> button)) return {};
    if (stream.get() != ';') return {};
    if (!(stream >> x))      return {};
    if (stream.get() != ';') return {};
    if (!(stream >> y))      return {};
    if (!stream.get(ending)) return {};
    
    mouseEvent event {.valid = true};
    event.button = button & 3;
    event.x = x;
    event.y = y;

    if(ending == 'M' && (button & 3) != 3) event.type = mouseEventType::Click;
    if(ending == 'm') event.type = mouseEventType::Release;
    if(button & 64)   event.type = mouseEventType::Wheel;
    if(button & 32) { // motion
        if((button & 3) == 3) event.type = mouseEventType::Hover;
        else event.type = mouseEventType::Drag;
    }
    event.shift  = button & 4;
    event.alt    = button & 8;
    event.ctrl   = button & 16;

    return event;
}

InputManager::~InputManager(){
    disableCapture();
}
void InputManager::enableCapture(){ // remember to call disableCapture() before exiting
    ansi::enableVirtualInput();
    ansi::enableMouseTracking();
}
void InputManager::disableCapture(){
    ansi::disableVirtualInput();
    ansi::disableMouseTracking();
}
void InputManager::registerKey(char c, std::function<void()> callback){
    registeredKeys.emplace(c, callback);
}
void InputManager::registerMouseEvent(mouseEventType type, std::function<void(const mouseEvent&)> callback){
    registeredMouseEvents.emplace(type, callback);
}
inputEvent InputManager::parseCinBuffer(std::string& buffer) const { // parse, also decides whether to clear buffer
    char last = buffer[buffer.size() - 1];
    if (buffer.length() >= 1 && buffer[0] == '\033'){ // escape code
        std::string code = buffer.substr(1);

        if (code.length() == 0) return {}; // if code empty
        else if (last == '\033') { // at least 1 char, last char is ESC, discard (happens when pressing esc key ... and another esc arrives)
            buffer = "\033";
            return {};
        }
        else if (code.length() >= 2 && code.substr(0, 2) == "[<" && (last == 'M' || last == 'm')) { // mouse code
            mouseEvent event = parseMouseCode(code);
            buffer.clear();
            return {.valid = true, .m = event};
        }
        else if (code.length() >= 2 && code[0] == '[' && code[1] != '<') { // escaped keys (arrows, etc.)
            buffer.clear();
            return {}; // for now
        }
    }
    else if (buffer.length() >= 1 && buffer[0] != '\033') { // plaintext input, non-escaped special keys (enter, backspace)
        char c = buffer[0];
        buffer.clear();
        return {.valid = true, .key = c};
    }
    return {};
}
void InputManager::handleInput(const inputEvent& in){
    if(!in.valid) return;
    else if(in.m.valid){
        if(registeredMouseEvents.find(in.m.type) != registeredMouseEvents.end()){
            registeredMouseEvents[in.m.type](in.m);
        }
    }
    else if(registeredKeys.find(in.key) != registeredKeys.end()){
        registeredKeys[in.key](); 
    }
}

}