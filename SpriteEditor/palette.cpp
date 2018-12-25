#include "palette.h"

int getPalette(int col)
{
    return ((col & 0x38) << 1) | (col & 7);
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
    return QSize(COLORS * 15, 15);
}

void Palette::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.setPen(QColor(0, 0, 0));
    int pixelW = width() / COLORS;
    int pixelH = height();
    for (int x = 0 ; x < COLORS ; ++x)
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
        int pixelW = width() / COLORS;
        int x = event->x() / pixelW;
        if (x >= 0 && x < COLORS)
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
