#include "button.h"  // Include the header to access the Button class definition

Button::Button(int x, int y, int width, int height, const std::string& text)
    : rect{static_cast<float>(x), static_cast<float>(y), static_cast<float>(width),
           static_cast<float>(height)},
      text{text} {}

void Button::Draw() const {
    Color buttonColor = hovered ? LIGHTGRAY : DARKGRAY;
    DrawRectangleRec(rect, buttonColor);
    DrawText(text.c_str(), rect.x + (rect.width / 2) - MeasureText(text.c_str(), 20) / 2,
             rect.y + (rect.height / 2) - 10, 20, BLACK);
}

void Button::Update(const Vector2& mousePoint) {
    hovered = CheckCollisionPointRec(mousePoint, rect);
    if (hovered && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        clicked = true;
    } else {
        clicked = false;
    }
}