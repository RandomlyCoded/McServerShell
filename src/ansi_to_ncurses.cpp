#include "ansi_to_ncurses.h"

#include <iostream>
#include <ncurses.h>

using namespace std;

#if 0
Black  = "30"   COLOR_BLACK
Red    = "31"   COLOR_RED
Green  = "32"   COLOR_GREEN
Yellow = "33"   COLOR_YELLOW
Blue   = "34"   COLOR_BLUE
Purple = "35"   COLOR_PURPLE
Cyan   = "36"   COLOR_CYAN
LGray  = "37"   COLOR_WHITE

BgBlack  = "40"
BgRed    = "41"
BgGreen  = "42"
BgYellow = "43"
BgBlue   = "44"
BgPurple = "45"
BgCyan   = "46"
BgLGray  = "47"

LBlack  = "90"
LRed    = "91"
LGreen  = "92"
LYellow = "93"
LBlue   = "94"
LPurple = "95"
LCyan   = "96"
White   = "97"

BgLBlack  = "100"
BgLRed    = "101"
BgLGreen  = "102"
BgLYellow = "103"
BgLBlue   = "104"
BgLPurple = "105"
BgLCyan   = "106"
BgLWhite  = "107"
#endif // 0

namespace randomly
{

namespace
{

// all colors are saved as fg * 8 + bg
// fg * 8 + bg = (fg << 3) + bg
inline short getColorIndex(short f, short g)
{ return (f * 10) + g + 1; }

}

vector<NCursesSpan> ansiToNcurses(string line)
{
    vector<NCursesSpan> out;
    NCursesSpan span;

    for (int i = 0; i < line.length(); ++i) {
        // cout << line[i] << " -> " << span.text << endl;

        if (line[i] == '\e') {
            // push last span, since now we have a new one:
            // skip if i == 0, since there isn't any data yet
            if (i != 0) {
                out.push_back(std::move(span));

                // reset
                span = NCursesSpan();
            }

            short fg = 9; // 9 is the "default" color, as defined in randomly::initAnsiToNCurses
            short bg = 9; // I am aware that pair_content exists, but it returns ncurses' default color scheme, not the terminal default

            do {
                // [ or ;
                ++i;

                switch(line[i + 1]) {
                case '3': {// foreground
                    short c = line[i + 2] - '0';
                    fg = c;
                } break;

                case '4': {// background
                    short c = line[i + 2] - '0';
                    bg = c;
                } break;

                case 'm':
                    // unskip 2 characters, since we don't have a number
                    i -= 2;
                }

                if (line[i + 2] != 'm') // some people like \e[0m, so we don't skip one of the escaped characters if the 2nd of them is 'm'
                    ++i; // first escape symbol
                ++i; // second escape symbol;
            } while (line[i + 1] == ';');

            span.stylePair = getColorIndex(fg, bg);

            ++i; // m
        }

        else
            span.text += line[i];
    }

    out.push_back(span);

    out.emplace_back("", 0);

    return out;
}

void initAnsiToNCurses()
{
    assume_default_colors(-1, -1);

    for (short f = 0; f < 8; ++f)
        for (short b = 0; b < 8; ++b)
            init_pair(getColorIndex(f, b), f, b);

    // special pairs (using default background color): 9 now represents default
    for (short c = 0; c < 8; ++c) {
        init_pair(getColorIndex(c, 9), c, -1);
        init_pair(getColorIndex(9, c), -1, c);
    }

    // final special pair: true default, a.k.a. terminal default
    init_pair(getColorIndex(9, 9), -1, -1);
}

} // namespace randomly
