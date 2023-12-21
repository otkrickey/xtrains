#include "../include/database.h"

namespace database_v3
{
    // init static variables
    template <typename Key, typename Model>
    std::unique_ptr<Database<Key, Model>> Database<Key, Model>::instance = nullptr;
    template <typename Key, typename Model>
    std::mutex Database<Key, Model>::mutex;

    // createDB: create database and return pointer to it
    template <typename Key, typename Model>
    void Database<Key, Model>::extract()
    {
        std::map<Key, Model> empty;
        this->extract(empty);
    }

    // specializations for RailwayDB
    template class Database<railway_, Railway_>;
    template <>
    std::string Database<railway_, Railway_>::databaseName = "railway_v3";
    template <>
    void Database<railway_, Railway_>::extract(std::map<railway_, Railway_> &data)
    {
        db.createCollection<std::string>("rw_code", data, [](const Railway_ &model) { return model.rw_code; });
        db.createCollection<std::string>("name", data, [](const Railway_ &model) { return model.name; });
    }
    template <>
    void Database<railway_, Railway_>::update()
    {
        auto rw_code = db.getCollection<std::string>("rw_code").data();
        auto name = db.getCollection<std::string>("name").data();
        for (auto &rw : rw_code) { db.data[rw.first].rw_code = rw.second; }
        for (auto &rw : name) { db.data[rw.first].name = rw.second; }
    }

    // specializations for StationDB
    template class Database<station_, Station_>;
    template <>
    std::string Database<station_, Station_>::databaseName = "station_v3";
    template <>
    void Database<station_, Station_>::extract(std::map<station_, Station_> &data)
    {
        db.createCollection<std::string>("rw_code", data, [](const Station_ &model) { return model.rw_code; });
        db.createCollection<int>("rw_st_num", data, [](const Station_ &model) { return model.rw_st_num; });
        db.createCollection<std::string>("name", data, [](const Station_ &model) { return model.name; });
    }
    template <>
    void Database<station_, Station_>::update()
    {
        auto rw_code = db.getCollection<std::string>("rw_code").data();
        auto rw_st_num = db.getCollection<int>("rw_st_num").data();
        auto name = db.getCollection<std::string>("name").data();
        for (auto &st : rw_code) { db.data[st.first].rw_code = st.second; }
        for (auto &st : rw_st_num) { db.data[st.first].rw_st_num = st.second; }
        for (auto &st : name) { db.data[st.first].name = st.second; }
    }

    // specializations for TrainDB
    template class Database<train_, Train_>;
    template <>
    std::string Database<train_, Train_>::databaseName = "train_v3";
    template <>
    void Database<train_, Train_>::extract(std::map<train_, Train_> &data)
    {
        db.createCollection<std::string>("code", data, [](const Train_ &model) { return model.code; });
        db.createCollection<std::string>("rw_code", data, [](const Train_ &model) { return model.rw_code; });
        db.createCollection<std::map<station_, time_>>("stops", data, [](const Train_ &model) { return model.stops; });
    }
    template <>
    void Database<train_, Train_>::update()
    {
        auto code = db.getCollection<std::string>("code").data();
        auto rw_code = db.getCollection<std::string>("rw_code").data();
        auto stops = db.getCollection<std::map<station_, time_>>("stops").data();
        for (auto &tr : code) { db.data[tr.first].code = tr.second; }
        for (auto &tr : rw_code) { db.data[tr.first].rw_code = tr.second; }
        for (auto &tr : stops) { db.data[tr.first].stops = tr.second; }
    }

    int test()
    {
        auto RailwayDB = Database<railway_, Railway_>::loadDB();
        std::cout << "RailwayDB: " << RailwayDB->data.size() << std::endl;
        auto StationDB = Database<station_, Station_>::loadDB();
        std::cout << "StationDB: " << StationDB->data.size() << std::endl;
        auto TrainDB = Database<train_, Train_>::loadDB();
        std::cout << "TrainDB: " << TrainDB->data.size() << std::endl;

        return 0;
    }
} // namespace database_v3
