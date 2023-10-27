#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

int gen_data()
{
    srand(time(0)); // 乱数の初期化

    // int n = 5000;         // 駅の数
    // int m = 5000;         // 接続の数
    int n, m;
    cout << "n: ";
    cin >> n;
    cout << "m: ";
    cin >> m;
    int max_weight = 100; // 最大所要時間
    int max_line = 10;    // 路線数

    ofstream outFile("large_data.txt");

    outFile << n << endl;
    outFile << m << endl;
    for (int i = 0; i < m; ++i)
    {
        int u = rand() % n;
        int v = rand() % n;
        int w = rand() % max_weight + 1;
        int l = rand() % max_line;
        outFile << u << " " << v << " " << w << " " << l << endl;
    }

    int transfer_time = 5; // 乗り換え時間
    outFile << transfer_time << endl;

    int start = 0;
    int goal = n - 1;
    outFile << start << " " << goal << endl;

    outFile.close();

    return 0;
}
