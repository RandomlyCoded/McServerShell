#include "processhandler.h"
#include "ansi_to_ncurses.h"

#include <iostream>
#include <ncurses.h>
#include <poll.h>
#include <regex>

constexpr auto topSpacing = 3;

bool tryWriteToWindow(WINDOW *wnd, const char *str, size_t n)
{
    if (wmove(wnd, 1, 1) == ERR)
        return false;

    wdeleteln(wnd);

    if (wmove(wnd, LINES - topSpacing - 3, 1) == ERR)
        return false;

    // first of all, I don't like having my home directory (/home/$USER/) shown as output, so I'll just replace it here:
    const static auto userhome = std::regex(std::string("/home/") + getenv("USER"));
    auto stdstr = std::regex_replace(std::string(str), userhome, "~");

    const auto data = randomly::ansiToNcurses(stdstr);

    for (const auto &span: data) {
        wattr_set(wnd, 0, span.stylePair, nullptr);
        waddnstr(wnd, span.text.c_str(), span.text.length());
    }

    wclrtoeol(wnd);

    return true;
}

int main(int argc, char **argv)
{
    initscr();

    start_color();

    randomly::initAnsiToNCurses();

    auto serverWin = subwin(stdscr, LINES - topSpacing, COLS/2, topSpacing, 0);
    auto pluginWin = subwin(stdscr, LINES - topSpacing, 0, topSpacing, COLS/2);

    touchwin(stdscr);

    wrefresh(serverWin);
    wrefresh(pluginWin);

    wborder(serverWin, '|', '|', '-', '_', '/', '\\', '\\', '/');
    wborder(pluginWin, '|', '|', '-', '_', '/', '\\', '\\', '/');

    addstr("starting server: ");
    addstr(argv[1]);

    refresh();

    randomly::ProcessHandler server;

    server.run(argv[1]);

    randomly::ProcessHandler plugins;
    // plugins.run(argv[2]);

    char *buffer;
    size_t n;

    while(!feof(server.pStdout())) {
        if (server.tryReadStdout(&buffer, &n)) // got some stdout
            if (!tryWriteToWindow(serverWin, buffer, n))
                break;

        wborder(serverWin, '|', '|', '-', '_', '/', '\\', '\\', '/'); // keep the border

        wrefresh(serverWin);
    }

    endwin();

    return 0;
}
