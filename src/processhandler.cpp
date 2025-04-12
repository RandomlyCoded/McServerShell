#include "processhandler.h"

#include <cstring>
#include <iostream>
#include <sys/wait.h>
#include <poll.h>
#include <unistd.h>
#include <vector>

namespace randomly {

ProcessHandler::ProcessHandler() {}

std::optional<intptr_t> ProcessHandler::run(std::string command)
{
    int pipeToProc[2];
    if (pipe(pipeToProc) == -1) {
        std::cerr << "failed to open pipe (To proc)!" << std::endl;

        return {};
    }

    int pipeFromProc[2];
    if (pipe(pipeFromProc) == -1) {
        std::cerr << "failed to open pipe (From proc)!" << std::endl;

        return {};
    }

    if (const auto pid = ::fork();
        pid == pid_t{-1}) {
        // This is in the calling process when fork() has failed.

        std::cerr << "fork(" << command << ") has failed: "
                  << std::strerror(errno) << std::endl;

        return {};
    } else if (pid != 0) {
        // This is in the calling process when fork() succeeded, and we became parent of a process.
        m_initialized = true;

        m_stdin = fdopen(pipeToProc[1], "w");
        m_stdout = fdopen(pipeFromProc[0], "r");

        close(pipeToProc[0]);
        close(pipeFromProc[1]);

        return {};
    } else {
        // This is in the child process created by fork().


        // stdout = fdopen(pipeFromProc[1], "w");
        // stderr = fdopen(pipeFromProc[1], "w");
        // stdin = fdopen(pipeToProc[0], "r");
        dup2(pipeFromProc[1], fileno(stdout));
        dup2(pipeFromProc[1], fileno(stderr));
        dup2(pipeToProc[0], fileno(stdin));

        close(pipeToProc[1]);
        close(pipeFromProc[0]);

        std::vector<const char *> args = {command.c_str(), nullptr};

        if (execvp(args[0], const_cast<char **>(args.data())) == -1) {
            std::cerr << "execvp(" << command << ") has failed: "
                      << std::strerror(errno) << std::endl;

            std::abort();
        }

        return {}; // actually this line is never reached
    }

}

bool ProcessHandler::tryReadStdout(char **buffer, size_t *n)
{
    auto serverPollFd = pollfd{fileno(pStdout()), POLLIN};

    poll(&serverPollFd, 1, 0);

    if ((serverPollFd.revents & POLLIN) == 0)
        return false;

    getline(buffer, n, pStdout());

    return true;
}

} // namespace randomly
