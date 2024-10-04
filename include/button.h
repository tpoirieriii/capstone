#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include "raylib.h"

class Button {
public:
    Button(int x, int y, int width, int height, const std::string& text);

    void Draw() const;
    void Update(const Vector2& mousePoint);
    bool WasClicked() const { return clicked; }

private:
    Rectangle rect;
    std::string text;
    bool hovered = false;
    bool clicked = false;
};

#endif // BUTTON_H
