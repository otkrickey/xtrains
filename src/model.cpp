#include "../include/model.h"

namespace model
{
    time_ Edge::weight(time_ prev_arrival)
    {
        // search for some available train by time
        return 0;
    }

    std::map<railway_, Railway_> __test__Railway_s = {
        {0, {0, "A", "odpt.Railway:Xtrains.alpha"}},
        {1, {1, "B", "odpt.Railway:Xtrains.beta"}},
        {2, {2, "C", "odpt.Railway:Xtrains.gamma"}},
    };
    std::map<station_, Station_> __test__Station_s = {
        {0, {0, "A", 1, "odpt.Station:Xtrains.alpha.center"}},
        {1, {1, "A", 2, "odpt.Station:Xtrains.alpha.epark"}},
        {2, {2, "A", 3, "odpt.Station:Xtrains.alpha.wpark"}},
        {3, {3, "A", 4, "odpt.Station:Xtrains.alpha.spark"}},
        {4, {4, "A", 5, "odpt.Station:Xtrains.alpha.npark"}},
        {5, {5, "B", 1, "odpt.Station:Xtrains.beta.center"}},
        {6, {6, "B", 2, "odpt.Station:Xtrains.beta.egym"}},
        {7, {7, "B", 3, "odpt.Station:Xtrains.beta.wgym"}},
        {8, {8, "B", 4, "odpt.Station:Xtrains.beta.sgym"}},
        {9, {9, "B", 5, "odpt.Station:Xtrains.beta.ngym"}},
        {10, {10, "C", 1, "odpt.Station:Xtrains.gamma.center"}},
        {11, {11, "C", 2, "odpt.Station:Xtrains.gamma.eschool"}},
        {12, {12, "C", 3, "odpt.Station:Xtrains.gamma.wschool"}},
        {13, {13, "C", 4, "odpt.Station:Xtrains.gamma.sschool"}},
        {14, {14, "C", 5, "odpt.Station:Xtrains.gamma.nschool"}},
    };
    std::map<train_, Train_> __test__Train_s = {
        {0, {0, "A001", "A", {{0, 21600}, {1, 21900}, {2, 22200}, {3, 22500}, {4, 22800}}}},
        {1, {1, "A002", "A", {{0, 25200}, {1, 25500}, {2, 25800}, {3, 26100}, {4, 26400}}}},
        {2, {2, "A003", "A", {{0, 28800}, {1, 29100}, {2, 29400}, {3, 29700}, {4, 30000}}}},
        {3, {3, "A004", "A", {{0, 32400}, {1, 32700}, {2, 33000}, {3, 33300}, {4, 33600}}}},
        {4, {4, "A005", "A", {{0, 36000}, {1, 36300}, {2, 36600}, {3, 36900}, {4, 37200}}}},
        {5, {5, "A006", "A", {{0, 39600}, {1, 39900}, {2, 40200}, {3, 40500}, {4, 40800}}}},
        {6, {6, "A007", "A", {{0, 43200}, {1, 43500}, {2, 43800}, {3, 44100}, {4, 44400}}}},
        {7, {7, "A008", "A", {{0, 46800}, {1, 47100}, {2, 47400}, {3, 47700}, {4, 48000}}}},
        {8, {8, "A009", "A", {{0, 50400}, {1, 50700}, {2, 51000}, {3, 51300}, {4, 51600}}}},
        {9, {9, "A010", "A", {{0, 54000}, {1, 54300}, {2, 54600}, {3, 54900}, {4, 55200}}}},
        {10, {10, "B001", "B", {{5, 21600}, {6, 21900}, {7, 22200}, {8, 22500}, {9, 22800}}}},
        {11, {11, "B002", "B", {{5, 25200}, {6, 25500}, {7, 25800}, {8, 26100}, {9, 26400}}}},
        {12, {12, "B003", "B", {{5, 28800}, {6, 29100}, {7, 29400}, {8, 29700}, {9, 30000}}}},
        {13, {13, "B004", "B", {{5, 32400}, {6, 32700}, {7, 33000}, {8, 33300}, {9, 33600}}}},
        {14, {14, "B005", "B", {{5, 36000}, {6, 36300}, {7, 36600}, {8, 36900}, {9, 37200}}}},
        {15, {15, "B006", "B", {{5, 39600}, {6, 39900}, {7, 40200}, {8, 40500}, {9, 40800}}}},
        {16, {16, "B007", "B", {{5, 43200}, {6, 43500}, {7, 43800}, {8, 44100}, {9, 44400}}}},
        {17, {17, "B008", "B", {{5, 46800}, {6, 47100}, {7, 47400}, {8, 47700}, {9, 48000}}}},
        {18, {18, "B009", "B", {{5, 50400}, {6, 50700}, {7, 51000}, {8, 51300}, {9, 51600}}}},
        {19, {19, "B010", "B", {{5, 54000}, {6, 54300}, {7, 54600}, {8, 54900}, {9, 55200}}}},
        {20, {20, "C001", "C", {{10, 21600}, {11, 21900}, {12, 22200}, {13, 22500}, {14, 22800}}}},
        {21, {21, "C002", "C", {{10, 25200}, {11, 25500}, {12, 25800}, {13, 26100}, {14, 26400}}}},
        {22, {22, "C003", "C", {{10, 28800}, {11, 29100}, {12, 29400}, {13, 29700}, {14, 30000}}}},
        {23, {23, "C004", "C", {{10, 32400}, {11, 32700}, {12, 33000}, {13, 33300}, {14, 33600}}}},
        {24, {24, "C005", "C", {{10, 36000}, {11, 36300}, {12, 36600}, {13, 36900}, {14, 37200}}}},
        {25, {25, "C006", "C", {{10, 39600}, {11, 39900}, {12, 40200}, {13, 40500}, {14, 40800}}}},
        {26, {26, "C007", "C", {{10, 43200}, {11, 43500}, {12, 43800}, {13, 44100}, {14, 44400}}}},
        {27, {27, "C008", "C", {{10, 46800}, {11, 47100}, {12, 47400}, {13, 47700}, {14, 48000}}}},
        {28, {28, "C009", "C", {{10, 50400}, {11, 50700}, {12, 51000}, {13, 51300}, {14, 51600}}}},
        {29, {29, "C010", "C", {{10, 54000}, {11, 54300}, {12, 54600}, {13, 54900}, {14, 55200}}}},
    };

    int main()
    {
        // Manager m(__test__Railway_s, __test__Station_s, __test__Train_s);
        // m.display();
        return 0;
    }
} // namespace model


    // int main()
    // {
    //     railway::DB railway_db;
    //     // station::DB station_db;
    //     // train::DB train_db;
    //     railway_db.display();
    //     // station_db.display();
    //     // train_db.display();

    //     std::vector<Railway_> Railway_s;
    //     for (std::pair<railway::DB::Key, railway::DB::Object> railway : railway_db.show())
    //     {
    //         // std::cout << railway.first.first << " " << railway.first.second << " " << railway.second.short_id << " " << railway.second.long_id << std::endl;
    //         Railway_s.push_back({
    //             railway.first.first,
    //             railway.second.short_id,
    //             railway.second.long_id,
    //         });
    //     }

    //     return 0;
    // }
 // namespace model