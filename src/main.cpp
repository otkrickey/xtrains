#include "../include/preprocess.h"
#include "../include/dijkstra.h"
#include "../include/config.h"

using namespace std;

int main(int argc, char *argv[])
{
    // if (argc < 2)
    // {
    //     cout << "Usage: xtrains.exe <command>" << endl;
    //     cout << "Commands:" << endl;
    //     cout << "  preprocess" << endl;
    //     cout << "  dijkstra" << endl;
    //     return 1;
    // }

    // string command = argv[1];
    // vector<string> args;
    // for (int i = 2; i < argc; i++)
    // {
    //     args.push_back(argv[i]);
    // }

    // if (command == "preprocess")
    // {
    //     return preprocess::main(argc - 2, argv + 2);
    // }
    // else if (command == "dijkstra")
    // {
    //     return dijkstra::main();
    // }
    // else
    // {
    //     cout << "Unknown command." << endl;
    //     return 1;
    // }

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
            int result = 0;
            result += api::main();
            result += preprocess::main();
            return result == 0 ? 0 : 1;
        }
        else if (readOption("test"))
        {
            int result = 0;
            // result += model::test();
            // result += preprocess::test();
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
        cout << "No mode specified." << endl;
        return 1;
    }

    return 0;
} // namespace preprocess