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
        {0, {0, "A001"}},
        {1, {1, "A002"}},
        {2, {2, "A003"}},
        {3, {3, "A004"}},
        {4, {4, "A005"}},
        {5, {5, "A006"}},
        {6, {6, "A007"}},
        {7, {7, "A008"}},
        {8, {8, "A009"}},
        {9, {9, "A010"}},
        {10, {10, "B001"}},
        {11, {11, "B002"}},
        {12, {12, "B003"}},
        {13, {13, "B004"}},
        {14, {14, "B005"}},
        {15, {15, "B006"}},
        {16, {16, "B007"}},
        {17, {17, "B008"}},
        {18, {18, "B009"}},
        {19, {19, "B010"}},
        {20, {20, "C001"}},
        {21, {21, "C002"}},
        {22, {22, "C003"}},
        {23, {23, "C004"}},
        {24, {24, "C005"}},
        {25, {25, "C006"}},
        {26, {26, "C007"}},
        {27, {27, "C008"}},
        {28, {28, "C009"}},
        {29, {29, "C010"}},
    };

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
} // namespace model