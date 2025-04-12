#ifndef PROCESSHANDLER_H
#define PROCESSHANDLER_H

#include <cstdint>
#include <optional>
#include <stdio.h>
#include <string>

namespace randomly {

class ProcessHandler
{
public:
    ProcessHandler();

    auto pStdin()  const { return m_stdin ; }
    auto pStdout() const { return m_stdout; }

    std::optional<intptr_t> run(std::string command);

    bool tryReadStdout(char **buffer, size_t *n);

private:
    bool m_initialized = false;
    FILE *m_stdin = nullptr;
    FILE *m_stdout = nullptr;
};

} // namespace randomly

#endif // PROCESSHANDLER_H
