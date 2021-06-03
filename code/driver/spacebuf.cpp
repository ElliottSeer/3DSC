#include "spacebuf.h"

SpaceBuf::SpaceBuf(unsigned int width, unsigned int height, unsigned int depth)
{
    m_width = width;
    m_height = height;
    m_depth = depth;
    m_p_spacebuf = new char[m_width * m_width * m_depth / 8];
}

void SpaceBuf::clearBuffer()
{
    if (m_p_spacebuf == nullptr)
    {
        return;
    }
    else
    {
        memset(m_p_spacebuf, 0, m_width * m_height * m_depth / 8);
    }
}

void SpaceBuf::dumpBuffer()
{
    if (m_p_spacebuf == nullptr)
    {
        return;
    }
    else
    {
        for (int i = 0; i < m_width * m_height * m_depth / 8; i++)
        {
            printf("%d ", ((uint8_t *)m_p_spacebuf)[i]);
        }
        printf("\n  ********* dump end *********\n");
    }
}

void *SpaceBuf::getPFrameByDepth(unsigned int depth)
{
    if (m_p_spacebuf == nullptr)
    {
        return nullptr;
    }
    else
    {
        return (void *)((int)m_p_spacebuf + (m_width * m_height * depth / 8));
    }
}

void SpaceBuf::drawObj(int x, int y, int z, Obj obj)
{
}

void SpaceBuf::drawPixel(int x, int y, int z, uint8_t color)
{
    if ((x > m_width) || (y > m_height) || (z > m_depth) || x < 0 || y < 0 || z < 0)
        return;

    uint8_t mask = 0x01 << (y % 8);

    if (color == 1)
        ((uint8_t *)m_p_spacebuf)[m_width * m_height * z / 8 + m_width + x] |= mask;
    else if (color == 0)
        ((uint8_t *)m_p_spacebuf)[m_width * m_height * z / 8 + m_width + x] &= ~mask;
    else if (color == 2)
        ((uint8_t *)m_p_spacebuf)[m_width * m_height * z / 8 + m_width + x] ^= mask;
}

void SpaceBuf::drawPoint(Point p, uint8_t color)
{
    if (p.unit == UM)
    {
        drawPixel(p.x / PITCH_WIDTH, p.y / PITCH_HEIGHT, p.z / PITCH_DEPTH, color);
    }
    else if (p.unit == PIXEL)
    {
        drawPixel(p.x, p.y, p.z, color);
    }
}

void SpaceBuf::_drawLine_Pixel(int x0, int y0, int z0, int x1, int y1, int z1, uint8_t color)
{
    int delta_x = x1 - x0;
    int delta_y = y1 - y0;
    int delta_z = z1 - z0;
    int x_bias = 0;
    int y_bias = 0;
    int z_bias = 0;
    int u_x = x0;
    int u_y = y0;
    int u_z = z0;
    int inc_x = 0;
    int inc_y = 0;
    int inc_z = 0;
    int distance = 0;

    if (delta_x > 0)
    {
        inc_x = 1;
    }
    else if (delta_x < 0)
    {
        inc_x = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
    {
        inc_y = 1;
    }
    else if (delta_y < 0)
    {
        inc_y = -1;
        delta_y = -delta_y;
    }
    if (delta_z > 0)
    {
        inc_z = 1;
    }
    else if (delta_z < 0)
    {
        inc_z = -1;
        delta_z = -delta_z;
    }
    if (delta_x >= delta_y && delta_x >= delta_z)
        distance = delta_x;
    if (delta_y >= delta_x && delta_y >= delta_z)
        distance = delta_y;
    if (delta_z >= delta_y && delta_z >= delta_x)
        distance = delta_z;

    for (int t = 0; t < distance + 1; t++)
    {
        drawPixel(u_x, u_y, u_z, color);
        // print(u_x, u_y, u_z);
        x_bias += delta_x;
        y_bias += delta_y;
        z_bias += delta_z;
        if (x_bias > distance)
        {
            x_bias -= distance;
            u_x += inc_x;
        }
        if (y_bias > distance)
        {
            y_bias -= distance;
            u_y += inc_y;
        }
        if (z_bias > distance)
        {
            z_bias -= distance;
            u_z += inc_z;
        }
    }
}

void SpaceBuf::drawLine(Point p0, Point p1, uint8_t color)
{
    if (p0.unit == PIXEL && p1.unit == PIXEL)
    {
        _drawLine_Pixel(p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, color);
    }
    else if (p0.unit == UM && p1.unit == UM)
    {
        _drawLine_Pixel(p0.x / PITCH_WIDTH, p0.y / PITCH_HEIGHT, p0.z / PITCH_DEPTH,
                        p1.x / PITCH_WIDTH, p1.y / PITCH_HEIGHT, p1.z / PITCH_DEPTH, color);
    }
}

void SpaceBuf::_drawCircle_Pixel(int xc, int yc, int zc, uint16_t radius, uint8_t color, bool fill)
{
    // https://blog.csdn.net/wekic/article/details/88941183
    if (xc + radius < 0 || xc - radius >= m_width ||
        yc + radius < 0 || yc - radius >= m_height)
        return;

    int x = 0, y = radius, yi, d;
    d = 3 - 2 * radius;

    if (fill)
    {
        while (x <= y)
        {
            for (yi = x; yi <= y; yi++)
            {
                drawPixel(xc + x, yc + y, zc, color);
                drawPixel(xc - x, yc + y, zc, color);
                drawPixel(xc + x, yc - y, zc, color);
                drawPixel(xc - x, yc - y, zc, color);
                drawPixel(xc + y, yc + x, zc, color);
                drawPixel(xc - y, yc + x, zc, color);
                drawPixel(xc + y, yc - x, zc, color);
                drawPixel(xc - y, yc - x, zc, color);
            }

            if (d < 0)
            {
                d = d + 4 * x + 6;
            }
            else
            {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    }
    else
    {
        while (x <= y)
        {
            drawPixel(xc + x, yc + y, zc, color);
            drawPixel(xc - x, yc + y, zc, color);
            drawPixel(xc + x, yc - y, zc, color);
            drawPixel(xc - x, yc - y, zc, color);
            drawPixel(xc + y, yc + x, zc, color);
            drawPixel(xc - y, yc + x, zc, color);
            drawPixel(xc + y, yc - x, zc, color);
            drawPixel(xc - y, yc - x, zc, color);
            if (d < 0)
            {
                d = d + 4 * x + 6;
            }
            else
            {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    }
}

void SpaceBuf::_drawBall_Pixel(int xc, int yc, int zc, uint16_t radius, uint8_t color, bool fill)
{
    _drawCircle_Pixel(xc, yc, zc, radius, color, fill);
    int i = 0;
    while (1)
    {
        i += 1;
        if (radius * radius * PITCH_WIDTH * PITCH_HEIGHT - i * i * PITCH_DEPTH * PITCH_DEPTH <= 0)
            break;
        int radius_pixel = pow((radius * radius * PITCH_HEIGHT * PITCH_WIDTH - i * i * PITCH_DEPTH * PITCH_DEPTH), 0.5);
        _drawCircle_Pixel(xc, yc, zc, radius_pixel, color, fill);
        _drawCircle_Pixel(xc, yc, zc, radius_pixel, color, fill);
    }
}

void SpaceBuf::drawBall(Point center, uint16_t radius, uint8_t color, bool fill)
{
    if (center.unit == PIXEL)
        _drawBall_Pixel(center.x, center.y, center.z, radius, color, fill);

    else if (center.unit == UM)
    {
        _drawBall_Pixel(center.x / PITCH_WIDTH, center.y / PITCH_HEIGHT, center.z / PIXEL_DEPTH, radius / PITCH_DEPTH, color, fill);
    }
}

SpaceBuf::~SpaceBuf()
{
    delete (m_p_spacebuf);
}