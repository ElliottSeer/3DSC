#ifndef _SPACEBUF_H__
#define _SPACEBUF_H__

#include <iostream>
#include "obj3d.h"

using namespace std;

class SpaceBuf
{
private:
    unsigned int m_width = 128;
    unsigned int m_height = 56;
    unsigned int m_depth = 16;
    void *m_p_spacebuf = nullptr; // 以Byte来组织像素

    void _drawLine_Pixel(int x0, int y0, int z0, int x1, int y1, int z1, uint8_t color);
    void _drawCircle_Pixel(int xc, int yc, int zc, uint16_t radius, uint8_t color, bool full);
    void _drawBall_Pixel(int xc, int yc, int zc, uint16_t radius, uint8_t color, bool fill);

public:
    SpaceBuf(unsigned int width, unsigned int height, unsigned int depth);
    void clearBuffer();
    void dumpBuffer();
    void *getPFrameByDepth(unsigned int depth);
    void drawObj(int x, int y, int z, Obj obj);
    void drawPoint(Point p0, uint8_t color);
    void drawPixel(int x, int y, int z, uint8_t color);
    void drawLine(Point p0, Point p1, uint8_t color);
    void drawBall(Point center, uint16_t radius, uint8_t color, bool fill);
    ~SpaceBuf();
};

#endif