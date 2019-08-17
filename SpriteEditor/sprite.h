#ifndef SPRITE_H
#define SPRITE_H

#include <QWidget>

#define WIDTH 64
#define HEIGHT 64

class Palette;

class Sprite : public QWidget
{
    Q_OBJECT

public:
    Sprite(Palette *p, QWidget *parent = 0);

    int getPixel(int row, int col);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

signals:
    void spriteUpdated();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void putPixel(QMouseEvent *event, bool erase = false);

private:
    int pixels[HEIGHT][WIDTH];
    Palette *pal;
};

#endif
