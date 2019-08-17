#ifndef PALETTE_H
#define PALETTE_H

#include <QtWidgets>

QColor getRGB(int col);
int getPalette(int col);

class Palette : public QWidget
{
    Q_OBJECT

public:
    Palette(QWidget *parent = 0);

    int getColor() const { return color; }
    void setColor(int c);

    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    int color;
};

#endif
