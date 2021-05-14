#ifndef _SPACEBUF_H__
#define _SPACEBUF_H__

#include <iostream>
using namespace std;

typedef enum
{
    MVLSB, // bit
} sp_format_t;

class SpaceBuf
{
private:
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_depth;
    sp_format_t m_format;
    void *m_p_spacebuf;

public:
    SpaceBuf(unsigned int width, unsigned int height, unsigned int depth, sp_format_t format);
    void clearBuffer();
    ~SpaceBuf();
};

SpaceBuf::SpaceBuf(unsigned int width, unsigned int height, unsigned int depth, sp_format_t format)
{
    m_width = width;
    m_height = height;
    m_depth = depth;
    m_format = format;
    switch (m_format)
    {
    case MVLSB:
        m_p_spacebuf = new char[m_width * m_width * m_depth / 8];
        break;

    default:
        break;
    }
}

SpaceBuf::~SpaceBuf()
{
    delete (m_p_spacebuf);
}


#endif