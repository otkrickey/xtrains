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
            int result = 0;
            result += model::test();
            result += preprocess::test();
            return result == 0 ? 0 : 1;
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