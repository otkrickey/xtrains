#include "../include/main.h"

using namespace std;

int main(int argc, char *argv[])
{
    bool dijkstra = readMode("dijkstra");
    bool preprocess = readMode("preprocess");

    if (dijkstra)
    {
        return dijkstra::main();
    }
    else if (preprocess)
    {
        return preprocess::main();
    }
    else
    {
        cout << "No mode specified." << endl;
        return 1;
    }

    return 0;
} // namespace preprocess