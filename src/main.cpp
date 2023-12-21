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
            return preprocess_v2::main();
        }
        else if (readOption("test"))
        {
            return preprocess_v2::test();
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