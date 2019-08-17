#include "window.h"
#include "sprite.h"
#include "palette.h"

Window::Window()
{
    palette = new Palette;
    sprite = new Sprite(palette);
    code = new QTextEdit;

    code->setReadOnly(true);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(palette);
    mainLayout->addSpacing(1);
    mainLayout->addWidget(sprite);
    mainLayout->addSpacing(1);
    mainLayout->addWidget(code);
    setLayout(mainLayout);

    setWindowTitle(tr("Gamebox sprite editor"));

    connect(sprite, SIGNAL(spriteUpdated()), this, SLOT(updateSprite()));
}

void Window::updateSprite()
{
    QString s;
    int left = WIDTH, right = 0, top = HEIGHT, bottom = 0;
    int lastColor = 0;
    bool multicolor = false;
    for (int r = 0 ; r < HEIGHT ; ++r)
    {
        for (int c = 0 ; c < WIDTH ; ++c)
        {
            int col = sprite->getPixel(r, c);
            if (col)
            {
                if (left > c)
                    left = c;
                if (right < c)
                    right = c;
                if (top > r)
                    top = r;
                if (bottom < r)
                    bottom = r;
                if (lastColor && lastColor != col)
                    multicolor = true;
                lastColor = col;
            }
        }
    }
    if (left > right || top > bottom)
    {
    }
    else if (multicolor)
    {
        s = "static const uint8_t spriteLines[] PROGMEM = {\n";
        for (int r = top ; r <= bottom ; ++r)
        {
            s += "    ";
            for (int c = left ; c <= right ; ++c)
            {
                s += QString("0x%1, ").arg(getPalette(sprite->getPixel(r, c)), 2, 16, QChar('0'));
            }
            s += "\n";
        }
        s += "};\n";
        s += QString("static const game_color_sprite sprite PROGMEM = {%1, %2, spriteLines};\n")
            .arg(right - left + 1).arg(bottom - top + 1);
    }
    else
    {
        s = "static const uint8_t spriteLines[] PROGMEM = {\n";
        for (int r = top ; r <= bottom ; ++r)
        {
            s += "    ";
            QString bytes;
            for (int c = left ; c <= right ; ++c)
            {
                int p = sprite->getPixel(r, c);
                bytes += p ? "1" : "0";
                if (bytes.length() == 8)
                {
                    s += "B" + bytes + ", ";
                    bytes.clear();
                }
            }
            if (bytes.length())
            {
                while (bytes.length() < 8)
                    bytes += "0";
                s += "B" + bytes + ", ";
            }
            s += "\n";
        }
        s += "};\n";
        s += QString("static const game_sprite sprite PROGMEM = {%1, %2, spriteLines};\n")
            .arg(right - left + 1).arg(bottom - top + 1);
    }
    code->setPlainText(s);
}
