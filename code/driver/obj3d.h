#ifndef _OBJ_H__
#define _OBJ_H__

// #define PIXEL_WIDTH 254
// #define PIXEL_HEIGHT 254
// #define PIXEL_DEPTH 625

#define PITCH_WIDTH 274
#define PITCH_HEIGHT 274
#define PITCH_DEPTH 1250

typedef enum
{
    PIXEL,
    UM
} unit_e;

struct Obj
{
    unsigned int width;
    unsigned int height;
    unsigned int depth;
    void *p_data;
    void *p_mask;
};

class Point
{
private:
    /* data */
public:
    int x;
    int y;
    int z;
    unit_e unit;
    Point(int x, int y, int z, unit_e unit);
    Point toUnit(unit_e unit);
    ~Point();
};

Point::Point(int x, int y, int z, unit_e unit)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->unit = unit;
}

Point Point::toUnit(unit_e unit)
{
    if (unit == this->unit)
    {
        return (*this);
    }
    if (unit == UM)
    {
        return Point(this->x * PIXEL_WIDTH, this->y * PIXEL_HEIGHT, this->z * PIXEL_DEPTH, UM);
    }
    if (unit == PIXEL)
    {
        return Point(this->x / PIXEL_WIDTH, this->y / PIXEL_HEIGHT, this->z / PIXEL_DEPTH, PIXEL);
    }
}

Point::~Point()
{
}

#endif