#include "palette.h"

static const int palette[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
          0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
                      0x33, 0x34, 0x35, 0x36, 0x37,
                            0x44, 0x45, 0x46, 0x47,
                                  0x55, 0x56, 0x57,
                                        0x66, 0x67,
                                              0x77,
};
static const size_t paletteSize = sizeof(palette) / sizeof(palette[0]);

int getPalette(int col)
{
    return palette[col];
}

QColor getRGB(int col)
{
    if (col == 0)
        return QColor(32, 32, 32);
    int c = getPalette(col);
    int r = ((c & 0x10) << 3) + ((c & 1) << 6);
    int g = ((c & 0x20) << 2) + ((c & 2) << 5);
    int b = ((c & 0x40) << 1) + ((c & 4) << 4);
    return QColor(r, g, b);
}

Palette::Palette(QWidget *parent)
{
    color = 0;

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

QSize Palette::minimumSizeHint() const
{
    return QSize(paletteSize * 15, 15);
}

void Palette::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.setPen(QColor(0, 0, 0));
    int pixelW = width() / paletteSize;
    int pixelH = height();
    for (int x = 0 ; x < paletteSize ; ++x)
    {
        QBrush brush(getRGB(x));
        painter.setBrush(brush);
        painter.drawRect(QRect(x * pixelW, 0, pixelW, pixelH));
        painter.setBrush(Qt::NoBrush);
    }
    QPen pen(color == 0 ? QColor(255, 255, 255) : QColor(0, 0, 0));
    painter.setPen(pen);
    painter.drawLine(color * pixelW, 0, (color + 1) * pixelW - 1, pixelH - 1);
    painter.drawLine((color + 1) * pixelW - 1, 0, color * pixelW, pixelH - 1);
}

void Palette::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        int pixelW = width() / paletteSize;
        int x = event->x() / pixelW;
        if (x >= 0 && x < paletteSize)
        {
            color = x;
        }
        update();
    }
}

void Palette::setColor(int c)
{
    color = c;
    update();
}
