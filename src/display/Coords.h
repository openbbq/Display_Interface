#pragma once

#include <Arduino.h>

namespace display
{

    class Size
    {
    public:
        Size() : _width(0), _height(0) {}
        Size(const Size &value) : _width(value._width), _height(value._height) {}
        Size(uint16_t width, uint16_t height) : _width(width), _height(height) {}

        bool operator==(const Size &value) const { return _width == value._width && _height == value._height; }
        bool operator!=(const Size &value) const { return !operator==(value); }
        bool operator!() const { return empty(); }

        Size operator+(const Size &value) const { return Size(_width + value._width, _height + value._height); }
        Size operator-(const Size &value) const { return Size(_width - value._width, _height - value._height); }
        Size operator*(uint16_t value) const { return Size(_width * value, _height * value); }
        Size operator/(uint16_t value) const { return Size(_width / value, _height / value); }

        uint16_t width() const { return _width; }
        uint16_t height() const { return _height; }
        bool empty() const { return _width == 0 || _height == 0; }
        Size max(const Size &value) { return Size(_max(_width, value._width), _max(_height, value._height)); }

    private:
        uint16_t _width;
        uint16_t _height;
    };

    class Point
    {
    public:
        Point() : _x(0), _y(0) {}
        Point(const Point &value) : _x(value._x), _y(value._y) {}
        Point(const Size &value) : _x(value.width()), _y(value.height()) {}
        Point(int16_t x, int16_t y) : _x(x), _y(y) {}

        bool operator==(const Point &value) const { return _x == value._x && _y == value._y; }
        bool operator!=(const Point &value) const { return !operator==(value); }

        Point operator+(const Point &value) const { return offset(value); }
        Point operator-(const Point &value) const { return offset(value); }
        Point operator-() const { return Point(-_x, -_y); }

        Point offset(const Point &value) const { return Point(_x + value._x, _y + value._y); }
        Point offset(const Size &value) const { return Point(_x + value.width(), _y + value.height()); }
        Point offset(int16_t x, int16_t y) const { return Point(_x + x, _y + y); }

        int16_t x() const
        {
            return _x;
        }
        int16_t y() const { return _y; }

    private:
        int16_t _x;
        int16_t _y;
    };

    class Rect
    {
    public:
        Rect() {}
        Rect(const Rect &value) : _origin(value._origin), _size(value._size) {}
        Rect(const Point &origin, const Size &size) : _origin(origin), _size(size) {}
        Rect(const Size &size) : _size(size) {}
        Rect(int16_t left, int16_t top, int16_t right, int16_t bottom) : _origin(left, top), _size(right - left, bottom - top) {}

        String toString() const
        {
            String s;
            s += "{";
            s += String(left());
            s += ",";
            s += String(top());
            s += ",";
            s += String(right());
            s += ",";
            s += String(bottom());
            s += "}";
            return s;
        }

        bool operator==(const Rect &value) const { return _origin == value._origin && _size == value._size; }
        bool operator!=(const Rect &value) const { return !operator==(value); }
        bool operator!() const { return empty(); }

        const Point &origin() const { return _origin; }
        const Size &size() const { return _size; }
        bool empty() const { return _size.empty(); }

        int16_t x() const { return _origin.x(); }
        int16_t y() const { return _origin.y(); }
        int16_t width() const { return _size.width(); }
        int16_t height() const { return _size.height(); }

        int16_t left() const { return _origin.x(); }
        int16_t top() const { return _origin.y(); }
        int16_t right() const { return _origin.x() + _size.width(); }
        int16_t bottom() const { return _origin.y() + _size.height(); }

        Rect offset(const Point &value) const { return Rect(_origin + value, _size); }
        Rect offset(const Size &value) const { return Rect(_origin + value, _size); }

        Rect indent(int16_t value) const { return indent(value, value, value, value); }
        Rect indent(const Size &value) const { return indent(value.width(), value.height(), value.width(), value.height()); }
        Rect indent(int16_t left, int16_t top, int16_t right, int16_t bottom) const
        {
            return Rect(this->left() + left, this->top() + top, this->right() - right, this->bottom() - bottom);
        }

        bool contains(const Point &value) const
        {
            return contains(value.x(), value.y());
        }
        bool contains(int16_t x, int16_t y) const
        {
            return x >= left() && y >= top() && x < right() && y < bottom();
        }

        Rect intersection(int16_t left, int16_t top, int16_t right, int16_t bottom) const
        {
            if (left < this->left())
            {
                left = this->left();
            }
            if (top < this->top())
            {
                top = this->top();
            }
            if (right > this->right())
            {
                right = this->right();
            }
            if (bottom > this->bottom())
            {
                bottom = this->bottom();
            }
            if (left >= right || top >= bottom)
            {
                return Rect();
            }

            return Rect(left, top, right, bottom);
        }

        Rect intersection(const Rect &rc) const
        {
            return intersection(rc.left(), rc.top(), rc.right(), rc.bottom());
        }

    private:
        Point _origin;
        Size _size;
    };
}
