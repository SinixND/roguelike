#ifndef _20240212004900
#define _20240212004900

#include "ICommand.h"
#include <vector>

class Invoker
{
public:
    void executeCommand(ICommand& command)
    {
        command.execute();
    };

    void queueCommand(ICommand& command)
    {
        queue.push_back(command);
    };

    void executeQueue()
    {
        while (!queue.empty())
        {
            // queue.front().execute();
            queue.front().get().execute();
            queue.erase(queue.begin());
        }
    };

private:
    std::vector<std::reference_wrapper<ICommand>> queue{};
    //* std::vector<ICommand*> queue;
};

#endif