#include "db.h"

namespace base
{

    template class base::DB<station::Object>;
} // namespace base

namespace railway
{
    int test()
    {
        std::cout << "railway::test()" << std::endl;
        railway::DB db;
        // for (const auto &[k, v] : db.show())
        // {
        //     std::cout << k.first << " " << k.second << " " << v.short_id << " " << v.long_id << std::endl;
        // }

        const auto &[k1, v1] = db.read("", {1, 0});
        std::cout << "index: , key: {1, 0}" << std::endl;
        std::cout << k1.first << " " << k1.second << " " << v1.short_id << " " << v1.long_id << std::endl
                  << std::endl;

        const auto &[k2, v2] = db.read("short", "Y");
        std::cout << "index: short, key: Y" << std::endl;
        std::cout << k2.first << " " << k2.second << " " << v2.short_id << " " << v2.long_id << std::endl
                  << std::endl;

        const auto &[k3, v3] = db.read("long", "odpt.Railway:TokyoMetro.Ginza");
        std::cout << "index: long, key: odpt.Railway:TokyoMetro.Ginza" << std::endl;
        std::cout << k3.first << " " << k3.second << " " << v3.short_id << " " << v3.long_id << std::endl
                  << std::endl;

        return 0;
    }
} // namespace railway

namespace station
{
    int test()
    {
        std::cout << "station::test()" << std::endl;
        station::DB db;
        // for (const auto &[k, v] : db.show())
        // {
        //     std::cout << k.first << " " << k.second << " " << v.short_id << " " << v.long_id << std::endl;
        // }

        const auto &[k1, v1] = db.read("", {1, 2});
        std::cout << "index: , key: {1, 2}" << std::endl;
        std::cout << k1.first << " " << k1.second << " " << v1.short_id << " " << v1.long_id << std::endl
                  << std::endl;

        const auto &[k2, v2] = db.read("short", "Y03");
        std::cout << "index: short, key: Y03" << std::endl;
        std::cout << k2.first << " " << k2.second << " " << v2.short_id << " " << v2.long_id << std::endl
                  << std::endl;

        const auto &[k3, v3] = db.read("long", "odpt.Station:TokyoMetro.Ginza.AoyamaItchome");
        std::cout << "index: long, key: odpt.Station:TokyoMetro.Ginza.AoyamaItchome" << std::endl;
        std::cout << k3.first << " " << k3.second << " " << v3.short_id << " " << v3.long_id << std::endl
                  << std::endl;

        return 0;
    }
} // namespace station

namespace train
{
    int test()
    {
        std::cout << "train::test()" << std::endl;
        train::DB db;
        // for (const auto &[k, v] : db.show())
        // {
        //     std::cout << k.first << " " << k.second << " " << v.short_id << " " << v.long_id << std::endl << std::endl;
        // }

        const auto &[k1, v1] = db.read("", {5, 1});
        std::cout << "index: , key: {5, 1}" << std::endl;
        std::cout << k1.first << " " << k1.second << " " << v1.id << std::endl;
        for (const auto &stop : v1.stops)
        {
            std::cout << "    " << stop.railway << " " << stop.station << " " << stop.time << std::endl;
        }
        std::cout << std::endl;

        // const auto &[k2, v2] = db.read("name", "A1013S");
        // std::cout << "index: name, key: A1013S" << std::endl;
        // std::cout << k2.first << " " << k2.second << " " << v2.id << std::endl;
        // for (const auto &stop : v2.stops)
        // {
        //     std::cout << "    " << stop.railway << " " << stop.station << " " << stop.time << std::endl;
        // }

        // const auto &[k3, v3] = db.read("long", "odpt.Station:TokyoMetro.Tozai.Otemachi");
        // std::cout << k3.first << " " << k3.second << " " << v3.short_id << " " << v3.long_id << std::endl << std::endl;

        return 0;
    }
} // namespace train