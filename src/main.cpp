#include "preprocess.h"
// #include "dijkstra.cpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: xtrains.exe <command>" << endl;
        cout << "Commands:" << endl;
        cout << "  preprocess" << endl;
        cout << "  dijkstra" << endl;
        return 1;
    }

    string command = argv[1];

    if (command == "preprocess")
    {
        return preprocess::main();
    }
    else if (command == "dijkstra")
    {
        cout << "NOT Implemented." << endl;
    }
    else
    {
        cout << "Unknown command." << endl;
        return 1;
    }

    return 0;
}
