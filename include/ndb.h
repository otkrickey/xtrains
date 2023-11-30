#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <variant>

#include "model.h"

namespace database
{
    using namespace model;
    int main();

    template <typename Value>
    void toBinary(const Value &value, std::ofstream &outputFileStream);
    template <typename Value>
    Value fromBinary(std::ifstream &inputFileStream);

    template <typename Value>
    class baseDB
    {
    public:
        std::string collectionName;
        std::string databaseName;
        bool isLoaded = false;

    protected:
        std::map<int, Value> data;
        std::ofstream outputFileStream;
        std::ifstream inputFileStream;

    public:
        baseDB(std::string databaseName, std::string collectionName)
            : databaseName(databaseName), collectionName(collectionName)
        {
        }
        ~baseDB()
        {
            if (outputFileStream.is_open())
            {
                outputFileStream.close();
            }
            if (inputFileStream.is_open())
            {
                inputFileStream.close();
            }
        }

        void set(std::map<int, Value> data)
        {
            this->data = data;
        }

        void add(int key, Value value)
        {
            data[key] = value;
        }

        void save()
        {
            // create directory if not exists
            std::string command = "mkdir -p data/" + databaseName;
            system(command.c_str());

            // open file
            outputFileStream.open("data/" + databaseName + "/" + collectionName + ".db", std::ios::binary);
            if (!outputFileStream.is_open())
            {
                std::cout << "Error: cannot open file " << collectionName << ".db" << std::endl;
                throw std::runtime_error("Error: cannot open file " + databaseName + "/" + collectionName + ".db");
            }

            for (const auto &item : data)
            {
                toBinary(item.first, outputFileStream);
                toBinary(item.second, outputFileStream);
            }
        }

        void load()
        {
            // open file
            inputFileStream.open("data/" + databaseName + "/" + collectionName + ".db", std::ios::binary);
            if (!inputFileStream.is_open())
            {
                std::cout << "Error: cannot open file " << collectionName << ".db" << std::endl;
                throw std::runtime_error("Error: cannot open file " + databaseName + "/" + collectionName + ".db");
            }

            std::streampos position;
            while (inputFileStream.peek() != EOF)
            {
                int key = fromBinary<int>(inputFileStream);
                Value value = fromBinary<Value>(inputFileStream);
                data[key] = value;
            }

            isLoaded = true;
        }

        std::map<int, Value> get()
        {
            if (!isLoaded)
            {
                load();
            }
            return data;
        }
    };

    class railwayDB
    {
    public:
        std::string databaseName = "railway";
        bool isLoaded = false;

    protected:
        baseDB<std::string> __char_id;
        baseDB<std::string> __name;
        std::map<railway_, Railway_> data;

    public:
        railwayDB() : __char_id(databaseName, "rw_code"), __name(databaseName, "name")
        {
        }

        void set(std::map<railway_, Railway_> data)
        {
            this->data = data;
            std::map<railway_, std::string> __char_id_map = {};
            std::map<railway_, std::string> __name_map = {};
            for (const auto &item : data)
            {
                __char_id_map[item.first] = item.second.rw_code;
                __name_map[item.first] = item.second.name;
            }
            __char_id.set(__char_id_map);
            __name.set(__name_map);
        }

        void add(Railway_ &object)
        {
            data[object.id] = object;
            __char_id.add(object.id, object.rw_code);
            __name.add(object.id, object.name);
        }

        void save()
        {
            __char_id.save();
            __name.save();
        }

        void load()
        {
            for (const auto &item : __char_id.get())
            {
                data[item.first].rw_code = item.second;
            }
            for (const auto &item : __name.get())
            {
                data[item.first].name = item.second;
            }
        }

        std::map<railway_, Railway_> get()
        {
            if (!isLoaded)
            {
                load();
            }
            return data;
        }

        void display()
        {
            if (!isLoaded)
            {
                load();
            }
            for (const auto &item : data)
            {
                std::cout << item.first << " " << item.second.rw_code << " " << item.second.name << std::endl;
            }
        }
    };

    class stationDB
    {
    public:
        std::string databaseName = "station";
        bool isLoaded = false;

    protected:
        baseDB<std::string> __rw_code;
        baseDB<int> __rw_st_num;
        baseDB<std::string> __name;
        std::map<station_, Station_> data;

    public:
        stationDB() : __rw_code(databaseName, "rw_code"), __rw_st_num(databaseName, "rw_st_num"), __name(databaseName, "name")
        {
        }

        void set(std::map<station_, Station_> data)
        {
            this->data = data;
            std::map<station_, std::string> __rw_char_map = {};
            std::map<station_, int> __rw_st_num_map = {};
            std::map<station_, std::string> __name_map = {};
            for (const auto &item : data)
            {
                __rw_char_map[item.first] = item.second.rw_code;
                __rw_st_num_map[item.first] = item.second.rw_st_num;
                __name_map[item.first] = item.second.name;
            }
            __rw_code.set(__rw_char_map);
            __rw_st_num.set(__rw_st_num_map);
            __name.set(__name_map);
        }

        void add(Station_ &object)
        {
            data[object.id] = object;
            __rw_code.add(object.id, object.rw_code);
            __rw_st_num.add(object.id, object.rw_st_num);
            __name.add(object.id, object.name);
        }

        void save()
        {
            __rw_code.save();
            __rw_st_num.save();
            __name.save();
        }

        void load()
        {
            for (const auto &item : __rw_code.get())
            {
                data[item.first].rw_code = item.second;
            }
            for (const auto &item : __rw_st_num.get())
            {
                data[item.first].rw_st_num = item.second;
            }
            for (const auto &item : __name.get())
            {
                data[item.first].name = item.second;
            }
        }

        std::map<station_, Station_> get()
        {
            if (!isLoaded)
            {
                load();
            }
            return data;
        }

        void display()
        {
            for (const auto &item : data)
            {
                std::cout << item.first << " " << item.second.rw_code << " " << item.second.rw_st_num << " " << item.second.name << std::endl;
            }
        }
    };

    class trainDB
    {
    public:
        std::string databaseName = "train";
        bool isLoaded = false;

    protected:
        baseDB<std::string> __code;
        baseDB<std::string> __rw_code;
        baseDB<std::map<station_, time_>> __stops;
        std::map<train_, Train_> data;

    public:
        trainDB() : __code(databaseName, "code"), __rw_code(databaseName, "rw_code"), __stops(databaseName, "stops")
        {
        }

        void set(std::map<train_, Train_> data)
        {
            this->data = data;
            std::map<train_, std::string> __code_map = {};
            std::map<train_, std::string> __rw_char_map = {};
            std::map<train_, std::map<station_, time_>> __stops_map = {};
            for (const auto &item : data)
            {
                __code_map[item.first] = item.second.code;
                __rw_char_map[item.first] = item.second.rw_code;
                __stops_map[item.first] = item.second.stops;
            }
            __code.set(__code_map);
            __rw_code.set(__rw_char_map);
            __stops.set(__stops_map);
        }

        void add(Train_ &object)
        {
            data[object.id] = object;
            __code.add(object.id, object.code);
            __rw_code.add(object.id, object.rw_code);
            __stops.add(object.id, object.stops);
        }

        void save()
        {
            __code.save();
            __rw_code.save();
            __stops.save();
        }

        void load()
        {
            for (const auto &item : __code.get())
            {
                data[item.first].code = item.second;
            }
            for (const auto &item : __rw_code.get())
            {
                data[item.first].rw_code = item.second;
            }
            for (const auto &item : __stops.get())
            {
                data[item.first].stops = item.second;
            }
        }

        std::map<train_, Train_> get()
        {
            if (!isLoaded)
            {
                load();
            }
            return data;
        }

        void display()
        {
            for (const auto &item : data)
            {
                std::string stops = "";
                for (const auto &stop : item.second.stops)
                {
                    stops += std::to_string(stop.first) + ":" + std::to_string(stop.second) + " ";
                }
                std::cout << item.first << " " << item.second.code << " " << item.second.rw_code << " " << stops << std::endl;
            }
        }
    };
} // namespace database