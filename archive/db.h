#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <functional>
#include <variant>

#include "model.h"

namespace base
{
    int main();
    // DB [key, object, pair<key, object>, map<key, object>]
    using Key = std::pair<int, int>;
    struct Object
    {
    };
    template <typename V>
    using Pair = std::pair<Key, V>;
    template <typename V>
    using Data = std::map<Key, V>;

    // Index [key, entry, map<key, entry>]
    using IndexKey = std::variant<Key, std::string, int>;
    struct IndexEntry
    {
        std::streampos pos;
        std::streamoff size;
    };
    using Index = std::map<IndexKey, IndexEntry>;

    // Generate index key from DB key and value
    template <typename V>
    using ExtractKey = std::function<IndexKey(const Key &, const V &)>;

    // DB class
    template <typename V>
    class DB
    {
    public:
        using Key = std::pair<int, int>;
        using Object = V;
        using Data = std::map<Key, Object>;

        std::string collectionName;

    protected:
        bool isLoaded = false;
        Data data;

        std::vector<std::string> indexNames;
        std::map<std::string, ExtractKey<Object>> extractKeys;
        std::map<std::string, Index> indexes;
        std::map<std::string, std::ofstream> indexOutputFiles;
        std::map<std::string, std::ifstream> indexInputFiles;

    public:
        DB(const std::string collectionName, Data data = {})
            : collectionName(collectionName), data(data)
        {
        }

        virtual void toBinary(const Object &value, std::ofstream &dataFile) = 0;
        virtual Object fromBinary(std::ifstream &dataFile) = 0;

        void setIndex(const std::string &indexName, const ExtractKey<Object> &extractKey)
        {
            indexNames.push_back(indexName);
            extractKeys[indexName] = extractKey;
        }

        void save()
        {
            std::ofstream dataFile("data/" + collectionName + ".bin", std::ios::binary);
            if (!dataFile.is_open())
            {
                std::cerr << "Failed to open file." << std::endl;
                throw std::runtime_error("File open error");
            }

            for (std::string &indexName : indexNames)
            {
                std::string suffix = indexName.empty() ? ".idx" : "_" + indexName + ".idx";
                indexOutputFiles[indexName].open("data/" + collectionName + suffix, std::ios::binary);
                if (!indexOutputFiles[indexName].is_open())
                {
                    std::cerr << "Error opening file for writing" << std::endl;
                    throw std::runtime_error("File open error");
                }
            }

            for (const auto &[k, v] : data)
            {
                IndexEntry indexEntry;
                indexEntry.pos = dataFile.tellp();

                dataFile.write(reinterpret_cast<const char *>(&k.first), sizeof(k.first));
                dataFile.write(reinterpret_cast<const char *>(&k.second), sizeof(k.second));

                toBinary(v, dataFile);

                indexEntry.size = dataFile.tellp() - indexEntry.pos;
                for (auto &[indexName, extractKey] : extractKeys)
                {
                    indexes[indexName][extractKey(k, v)] = indexEntry;
                }
            }
            for (auto &[indexName, index] : indexes)
            {
                for (const auto &[key, indexEntry] : index)
                {
                    if (std::holds_alternative<std::pair<int, int>>(key))
                    {
                        const auto &[first, second] = std::get<std::pair<int, int>>(key);
                        indexOutputFiles[indexName].write(reinterpret_cast<const char *>(&first), sizeof(first));
                        indexOutputFiles[indexName].write(reinterpret_cast<const char *>(&second), sizeof(second));
                    }
                    else if (std::holds_alternative<std::string>(key))
                    {
                        const auto &str = std::get<std::string>(key);
                        indexOutputFiles[indexName].write(reinterpret_cast<const char *>(str.c_str()), str.size());
                        indexOutputFiles[indexName].write("\0", 1);
                    }
                    else
                    {
                        throw std::runtime_error("Unsupported key type");
                    }
                    indexOutputFiles[indexName].write(reinterpret_cast<const char *>(&indexEntry.pos), sizeof(indexEntry.pos));
                    indexOutputFiles[indexName].write(reinterpret_cast<const char *>(&indexEntry.size), sizeof(indexEntry.size));
                }
                indexOutputFiles[indexName].close();
            }
        }

        void load()
        {
            if (isLoaded)
            {
                return;
            }

            std::ifstream dataFile("data/" + collectionName + ".bin", std::ios::binary);

            if (!dataFile.is_open())
            {
                std::cerr << "Failed to open file." << std::endl;
                return;
            }

            while (dataFile.peek() != EOF)
            {
                Key key;
                Object object;

                dataFile.read(reinterpret_cast<char *>(&key.first), sizeof(key.first));
                dataFile.read(reinterpret_cast<char *>(&key.second), sizeof(key.second));

                object = fromBinary(dataFile);

                data[key] = object;
            }

            isLoaded = true;
        }

        void add(const Key &key, const Object &value)
        {
            data[key] = value;
        }

        Pair<Object> read(const std::string &indexName, const IndexKey &key)
        {
            std::string suffix = indexName.empty() ? ".idx" : "_" + indexName + ".idx";
            std::ifstream indexFile("data/" + collectionName + suffix, std::ios::binary);

            if (!indexFile.is_open())
            {
                std::cerr << "Failed to open file." << std::endl;
                throw std::runtime_error("File open error");
            }

            IndexKey tmpKey;
            IndexEntry indexEntry;
            bool found = false;

            if (indexName == "" && std::holds_alternative<std::pair<int, int>>(key))
            {
                std::pair<int, int> tmpKey;
                std::pair<int, int> _key = std::get<std::pair<int, int>>(key);
                while (indexFile.peek() != EOF)
                {
                    indexFile.read(reinterpret_cast<char *>(&tmpKey.first), sizeof(tmpKey.first));
                    indexFile.read(reinterpret_cast<char *>(&tmpKey.second), sizeof(tmpKey.second));
                    indexFile.read(reinterpret_cast<char *>(&indexEntry.pos), sizeof(indexEntry.pos));
                    indexFile.read(reinterpret_cast<char *>(&indexEntry.size), sizeof(indexEntry.size));
                    if (tmpKey == _key)
                    {
                        found = true;
                        break;
                    }
                }
            }
            else if (std::holds_alternative<std::string>(key))
            {
                std::string tmpKey;
                std::string _key = std::get<std::string>(key);
                char c;
                while (indexFile.peek() != EOF)
                {
                    indexFile.get(c);
                    if (c == '\0')
                    {
                        indexFile.read(reinterpret_cast<char *>(&indexEntry.pos), sizeof(indexEntry.pos));
                        indexFile.read(reinterpret_cast<char *>(&indexEntry.size), sizeof(indexEntry.size));
                        if (tmpKey == _key)
                        {
                            found = true;
                            break;
                        }
                        tmpKey.clear();
                    }
                    else
                    {
                        tmpKey += c;
                    }
                }
            }
            else
            {
                throw std::runtime_error("Unsupported key type");
            }

            if (!found)
            {
                std::cerr << "Key not found in index" << std::endl;
                return Pair<Object>();
            }

            std::ifstream dataFile("data/" + collectionName + ".bin", std::ios::binary);

            if (!dataFile.is_open())
            {
                std::cerr << "Failed to open file." << std::endl;
                throw std::runtime_error("File open error");
            }

            dataFile.seekg(indexEntry.pos);

            // read key
            Key _key;
            dataFile.read(reinterpret_cast<char *>(&_key.first), sizeof(_key.first));
            dataFile.read(reinterpret_cast<char *>(&_key.second), sizeof(_key.second));

            // read object
            Object object = fromBinary(dataFile);

            return Pair<Object>(_key, object);
        }

        Pair<Object> read(const std::string &indexName, const Key &key)
        {
            return read(indexName, IndexKey(key));
        }

        Data show()
        {
            load();
            return data;
        }

        // Pair<Object> get(const std::string &indexName, const IndexKey &key)
    };

    int main();
} // namespace base

namespace railway
{
    // DB [key, object, pair<key, object>, map<key, object>]
    using Key = base::Key;
    struct Object
    {
        std::string short_id, long_id;
    };
    using Pair = base::Pair<Object>;
    using Data = base::Data<Object>;

    // Index [key, entry, map<key, entry>]
    using IndexKey = base::IndexKey;
    struct IndexEntry
    {
        std::streampos pos;
        std::streamoff size;
    };
    using Index = std::map<IndexKey, IndexEntry>;

    // Generate index key from DB key and value
    using ExtractKey = base::ExtractKey<Object>;

    // DB class
    class DB : public base::DB<Object>
    {
    public:
        DB(Data data = {})
            : base::DB<Object>::DB("railways", data)
        {
            setIndex("", [](const Key &key, const Object &value) -> IndexKey
                     { return key; });
            setIndex("short", [](const Key &key, const Object &value) -> IndexKey
                     { return value.short_id; });
            setIndex("long", [](const Key &key, const Object &value) -> IndexKey
                     { return value.long_id; });
        }

        void toBinary(const Object &value, std::ofstream &dataFile)
        {
            size_t short_id_size = value.short_id.size();
            dataFile.write(reinterpret_cast<const char *>(&short_id_size), sizeof(short_id_size));
            dataFile.write(value.short_id.c_str(), short_id_size);

            size_t long_id_size = value.long_id.size();
            dataFile.write(reinterpret_cast<const char *>(&long_id_size), sizeof(long_id_size));
            dataFile.write(value.long_id.c_str(), long_id_size);
        }

        Object fromBinary(std::ifstream &dataFile)
        {
            size_t short_id_size;
            dataFile.read(reinterpret_cast<char *>(&short_id_size), sizeof(short_id_size));
            std::string short_id(short_id_size, '\0');
            dataFile.read(&short_id[0], short_id_size);

            size_t long_id_size;
            dataFile.read(reinterpret_cast<char *>(&long_id_size), sizeof(long_id_size));
            std::string long_id(long_id_size, '\0');
            dataFile.read(&long_id[0], long_id_size);

            return {short_id, long_id};
        }

        void display()
        {
            load();
            for (const auto &[k, v] : data)
            {
                std::cout << k.first << " " << k.second << " " << v.short_id << " " << v.long_id << std::endl;
            }
        }
    };
    int test();
}

namespace station
{
    // DB [key, object, pair<key, object>, map<key, object>]
    using Key = base::Key;
    struct Object
    {
        std::string short_id, long_id;
    };
    using Pair = base::Pair<Object>;
    using Data = base::Data<Object>;

    // Index [key, entry, map<key, entry>]
    using IndexKey = base::IndexKey;
    struct IndexEntry
    {
        std::streampos pos;
        std::streamoff size;
    };
    using Index = std::map<IndexKey, IndexEntry>;

    // Generate index key from DB key and value
    using ExtractKey = base::ExtractKey<Object>;

    // DB class
    class DB : public base::DB<Object>
    {
    public:
        DB(Data data = {})
            : base::DB<Object>::DB("stations", data)
        {
            setIndex("", [](const Key &key, const Object &value) -> IndexKey
                     { return key; });
            setIndex("short", [](const Key &key, const Object &value) -> IndexKey
                     { return value.short_id; });
            setIndex("long", [](const Key &key, const Object &value) -> IndexKey
                     { return value.long_id; });
        }

        void toBinary(const Object &value, std::ofstream &dataFile)
        {
            size_t short_id_size = value.short_id.size();
            dataFile.write(reinterpret_cast<const char *>(&short_id_size), sizeof(short_id_size));
            dataFile.write(value.short_id.c_str(), short_id_size);

            size_t long_id_size = value.long_id.size();
            dataFile.write(reinterpret_cast<const char *>(&long_id_size), sizeof(long_id_size));
            dataFile.write(value.long_id.c_str(), long_id_size);
        }

        Object fromBinary(std::ifstream &dataFile)
        {
            size_t short_id_size;
            dataFile.read(reinterpret_cast<char *>(&short_id_size), sizeof(short_id_size));
            std::string short_id(short_id_size, '\0');
            dataFile.read(&short_id[0], short_id_size);

            size_t long_id_size;
            dataFile.read(reinterpret_cast<char *>(&long_id_size), sizeof(long_id_size));
            std::string long_id(long_id_size, '\0');
            dataFile.read(&long_id[0], long_id_size);

            return {short_id, long_id};
        }

        void display()
        {
            load();
            for (const auto &[k, v] : data)
            {
                std::cout << k.first << " " << k.second << " " << v.short_id << " " << v.long_id << std::endl;
            }
        }
    };

    int test();
} // namespace station

namespace train
{
    // DB [key, object, pair<key, object>, map<key, object>]
    using Key = base::Key;
    struct Stop
    {
        int railway;
        int station;
        int time;
    };
    struct Object
    {
        std::string id;
        std::vector<Stop> stops;
    };
    using Pair = base::Pair<Object>;
    using Data = base::Data<Object>;

    // Index [key, entry, map<key, entry>]
    using IndexKey = base::IndexKey;
    struct IndexEntry
    {
        std::streampos pos;
        std::streamoff size;
    };
    using Index = std::map<IndexKey, IndexEntry>;

    // Generate index key from DB key and value
    using ExtractKey = base::ExtractKey<Object>;

    // DB class
    class DB : public base::DB<Object>
    {
    public:
        DB(Data data = {})
            : base::DB<Object>::DB("trains", data)
        {
            setIndex("", [](const Key &key, const Object &value) -> IndexKey
                     { return key; });
            setIndex("name", [](const Key &key, const Object &value) -> IndexKey
                     { return value.id; });
            // setIndex("short", [](const Key &key, const Object &value) -> IndexKey
            //          { return value.short_id; });
            // setIndex("long", [](const Key &key, const Object &value) -> IndexKey
            //          { return value.long_id; });
        }

        void toBinary(const Object &object, std::ofstream &dataFile)
        {
            size_t id_size = object.id.size();
            dataFile.write(reinterpret_cast<const char *>(&id_size), sizeof(id_size));
            dataFile.write(object.id.c_str(), id_size);

            size_t stops_size = object.stops.size();
            dataFile.write(reinterpret_cast<const char *>(&stops_size), sizeof(stops_size));

            for (const auto &stop : object.stops)
            {
                dataFile.write(reinterpret_cast<const char *>(&stop.railway), sizeof(stop.railway));
                dataFile.write(reinterpret_cast<const char *>(&stop.station), sizeof(stop.station));
                dataFile.write(reinterpret_cast<const char *>(&stop.time), sizeof(stop.time));
            }
        }

        Object fromBinary(std::ifstream &dataFile)
        {
            Object object;

            size_t id_size;
            dataFile.read(reinterpret_cast<char *>(&id_size), sizeof(id_size));
            object.id.resize(id_size);
            dataFile.read(&object.id[0], id_size);

            size_t stops_size;
            dataFile.read(reinterpret_cast<char *>(&stops_size), sizeof(stops_size));
            object.stops.resize(stops_size);

            for (auto &stop : object.stops)
            {
                dataFile.read(reinterpret_cast<char *>(&stop.railway), sizeof(stop.railway));
                dataFile.read(reinterpret_cast<char *>(&stop.station), sizeof(stop.station));
                dataFile.read(reinterpret_cast<char *>(&stop.time), sizeof(stop.time));
            }

            return object;
        }

        void display()
        {
            load();
            for (const auto &[k, v] : data)
            {
                std::cout << k.first << " " << k.second << " " << v.id << " " << v.stops.size() << std::endl;
            }
        }
    };

    int test();
};