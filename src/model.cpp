#include "../include/model.h"

namespace model
{
    time_ Edge::weight(time_ prev_arrival)
    {
        // search for some available train by time
        return 0;
    }

    int main()
    {
        railway::DB railway_db;
        // station::DB station_db;
        // train::DB train_db;
        railway_db.display();
        // station_db.display();
        // train_db.display();

        std::vector<Railway_> Railway_s;
        for (std::pair<railway::DB::Key, railway::DB::Object> railway : railway_db.show())
        {
            // std::cout << railway.first.first << " " << railway.first.second << " " << railway.second.short_id << " " << railway.second.long_id << std::endl;
            Railway_s.push_back({
                railway.first.first,
                railway.second.short_id,
                railway.second.long_id,
            });
        }

        return 0;
    }
} // namespace model