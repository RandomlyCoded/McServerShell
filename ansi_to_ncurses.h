#ifndef ANSI_TO_NCURSES_H
#define ANSI_TO_NCURSES_H

#include <string>
#include <vector>

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

struct NCursesSpan
{
    NCursesSpan() : text(""), stylePair(0) {}
    NCursesSpan(const char *d, int s) : text(d), stylePair(s) {}
    std::string text;
    int stylePair;
};

void initAnsiToNCurses();
std::vector<NCursesSpan> ansiToNcurses(std::string line);

} // namespace randomly

#endif // ANSI_TO_NCURSES_H
