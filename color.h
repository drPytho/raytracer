#ifndef COLOR_H_
#define COLOR_H_

#include <cmath>

struct Color {
    int red;
    int green;
    int blue;
    // Add some nice functionality to this later on.

    Color(int r, int g, int b) {
        red = clamp(r);
        green = clamp(g);
        blue = clamp(b);
    }


    Color operator* (double scale) const {
        return Color(static_cast<int>(this->red*scale),
                     static_cast<int>(this->green*scale),
                     static_cast<int>(this->blue*scale));
    }

    Color operator+ (const Color& c) const {
        return Color(this->red + c.red,
                     this->green + c.green,
                     this->blue + c.blue);
    }

    Color operator- (const Color& c) const {
        return Color(this->red - c.red,
                     this->green - c.green,
                     this->blue - c.blue);
    }

    Color tint(double scale) const {
        const Color c{0xFF, 0xFF, 0xFF};
        return *this + (c-*this)*scale;
    }

private:
    int clamp(int in) {
        return std::max(0, std::min(in, 255));
    }
};



#endif
