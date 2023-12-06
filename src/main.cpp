#include "../include/main.h"

int main()
{
    bool dijkstra = readMode("dijkstra");
    bool preprocess = readMode("preprocess");

    if (dijkstra)
    {
        return dijkstra::main();
    }
    else if (preprocess)
    {
        if (readOption("api"))
        {
            return preprocess::main();
        }
        else if (readOption("test"))
        {
            return preprocess::test();
        }
        else
        {
            std::cout << "No option specified." << std::endl;
            return 1;
        }
    }
    else
    {
        std::cout << "No mode specified." << std::endl;
        return 1;
    }

    return 0;
} // namespace preprocess