#include "data.h"

namespace data
{
    namespace station
    {
        vector<IndexType> indexTypes = {
            IndexType::ShortID,
            IndexType::LongID,
        };
        map<IndexType, string> index_map = {
            {IndexType::ShortID, "short"},
            {IndexType::LongID, "long"},
        };
        map<IndexType, string> filename_map = {
            {IndexType::ShortID, "short.idx"},
            {IndexType::LongID, "long.idx"},
        };
        DB::DB(const string &collectionName, const vector<IndexType> &indexTypes)
            : collectionName(collectionName), indexTypes(indexTypes)
        {
            DB::dataFile = ofstream(collectionName + ".bin", ios::binary);

            // printf("%s\n", (collectionName + ".bin").c_str());
            // for (const auto &indexType : indexTypes)
            // {
            //     printf("%s.%s\n", collectionName.c_str(), filename_map[indexType].c_str());
            // }
        }

        template <typename T>
        T DB::readIndex(ifstream &indexFile)
        {
            T index;
            ID id;
            IndexEntry indexEntry;
            while (indexFile.read(reinterpret_cast<char *>(&id.first), sizeof(id.first)) &&
                   indexFile.read(reinterpret_cast<char *>(&id.second), sizeof(id.second)) &&
                   indexFile.read(reinterpret_cast<char *>(&indexEntry.pos), sizeof(indexEntry.pos)) &&
                   indexFile.read(reinterpret_cast<char *>(&indexEntry.size), sizeof(indexEntry.size)))
            {
                index[id] = indexEntry;
            }
            return index;
        }

        Data DB::read(const streampos &pos)
        {
            ifstream dataFile(collectionName + ".bin", ios::binary);

            if (!dataFile.is_open())
            {
                cerr << "Error opening file for reading" << endl;
                return Data();
            }

            // Seek to the position in the data file
            dataFile.seekg(pos);

            // Read ID
            ID id;
            dataFile.read(reinterpret_cast<char *>(const_cast<int *>(&id.first)), sizeof(id.first));
            dataFile.read(reinterpret_cast<char *>(const_cast<int *>(&id.second)), sizeof(id.second));

            Values mappingData;

            // Read short_id
            size_t size;
            dataFile.read(reinterpret_cast<char *>(&size), sizeof(size));
            mappingData.short_id.resize(size);
            dataFile.read(mappingData.short_id.data(), size);

            // Read long_id
            dataFile.read(reinterpret_cast<char *>(&size), sizeof(size));
        }

        Data DB::get(const IndexType &by, const string &id)
        {
            for (const IndexType _indexType : indexTypes)
            {
                if (by == _indexType)
                {
                    ifstream indexFile(collectionName + "." + index_map[_indexType] + ".idx", ios::binary);

                    if (!indexFile.is_open())
                    {
                        cerr << "Error opening file for reading" << endl;
                        return Data();
                    }

                    // Search for the index entry
                    string tmpID;
                    char ch;
                    IndexEntry indexEntry;
                    bool found = false;

                    while (indexFile.get(ch))
                    {
                        if (ch == '\0')
                        {
                            if (!indexFile.read(reinterpret_cast<char *>(&indexEntry.pos), sizeof(indexEntry.pos)) ||
                                !indexFile.read(reinterpret_cast<char *>(&indexEntry.size), sizeof(indexEntry.size)))
                            {
                                cerr << "Error reading index" << endl;
                                return {};
                            }
                            if (tmpID == id)
                            {
                                found = true;
                                break;
                            }
                            tmpID.clear();
                        }
                        else
                        {
                            tmpID += ch;
                        }
                    }

                    if (!found)
                    {
                        cerr << "ID not found in index" << endl;
                        return Data();
                    }

                    return read(indexEntry.pos);
                }
            }
        }
    }
}

namespace _station
{
    using namespace std;

    void save(const Data &data, const string &collectionName)
    {
        ofstream dataFile(collectionName + ".bin", ios::binary);
        ofstream indexFile(collectionName + ".id.idx", ios::binary);
        ofstream indexShortFile(collectionName + ".short.idx", ios::binary);
        ofstream indexLongFile(collectionName + ".long.idx", ios::binary);

        if (!dataFile.is_open() || !indexFile.is_open() || !indexShortFile.is_open() || !indexLongFile.is_open())
        {
            cerr << "Error opening file for writing" << endl;
            return;
        }

        IndexID index;
        IndexShortID indexShort;
        IndexLongID indexLong;

        for (const auto &[id, mappingData] : data)
        {
            // Save data position and size
            IndexEntry indexEntry;
            indexEntry.pos = dataFile.tellp();

            // Save ID
            dataFile.write(reinterpret_cast<const char *>(&id.first), sizeof(id.first));
            dataFile.write(reinterpret_cast<const char *>(&id.second), sizeof(id.second));

            // Save short_id
            size_t size = mappingData.short_id.size();
            dataFile.write(reinterpret_cast<const char *>(&size), sizeof(size));
            dataFile.write(mappingData.short_id.c_str(), size);

            // Save long_id
            size = mappingData.long_id.size();
            dataFile.write(reinterpret_cast<const char *>(&size), sizeof(size));
            dataFile.write(mappingData.long_id.c_str(), size);

            indexEntry.size = dataFile.tellp() - indexEntry.pos;
            index[id] = indexEntry;
            indexShort[mappingData.short_id] = indexEntry;
            indexLong[mappingData.long_id] = indexEntry;
        }

        // Save index
        for (const auto &[id, indexEntry] : index)
        {
            // printf("%d, %d, %d, %d\n", id.first, id.second, indexEntry.pos, indexEntry.size);
            indexFile.write(reinterpret_cast<const char *>(&id.first), sizeof(id.first));
            indexFile.write(reinterpret_cast<const char *>(&id.second), sizeof(id.second));
            indexFile.write(reinterpret_cast<const char *>(&indexEntry.pos), sizeof(indexEntry.pos));
            indexFile.write(reinterpret_cast<const char *>(&indexEntry.size), sizeof(indexEntry.size));
        }
        for (const auto &[short_id, indexEntry] : indexShort)
        {
            // printf("%s, %d, %d\n", short_id.c_str(), indexEntry.pos, indexEntry.size);
            indexShortFile.write(short_id.c_str(), short_id.size());
            indexShortFile.write("\0", 1);
            indexShortFile.write(reinterpret_cast<const char *>(&indexEntry.pos), sizeof(indexEntry.pos));
            indexShortFile.write(reinterpret_cast<const char *>(&indexEntry.size), sizeof(indexEntry.size));
        }
        for (const auto &[long_id, indexEntry] : indexLong)
        {
            indexLongFile.write(long_id.c_str(), long_id.size());
            indexLongFile.write("\0", 1);
            indexLongFile.write(reinterpret_cast<const char *>(&indexEntry.pos), sizeof(indexEntry.pos));
            indexLongFile.write(reinterpret_cast<const char *>(&indexEntry.size), sizeof(indexEntry.size));
        }

        dataFile.close();
        indexFile.close();
        indexShortFile.close();
        indexLongFile.close();
    }

    Data load(const string &collectionName)
    {
        printf("%s\n", (collectionName + ".bin").c_str());
        ifstream dataFile(collectionName + ".bin", ios::binary);
        ifstream indexFile(collectionName + ".id.idx", ios::binary);

        if (!dataFile.is_open() || !indexFile.is_open())
        {
            cerr << "Error opening file for reading" << endl;
            return Data();
        }

        Data data;
        IndexID index;

        // Load index
        ID id;
        IndexEntry indexEntry;
        while (indexFile.read(reinterpret_cast<char *>(&id.first), sizeof(id.first)) &&
               indexFile.read(reinterpret_cast<char *>(&id.second), sizeof(id.second)) &&
               indexFile.read(reinterpret_cast<char *>(&indexEntry.pos), sizeof(indexEntry.pos)) &&
               indexFile.read(reinterpret_cast<char *>(&indexEntry.size), sizeof(indexEntry.size)))
        {
            index[id] = indexEntry;
        }

        // Load data
        for (const auto &[id, indexEntry] : index)
        {
            dataFile.seekg(indexEntry.pos);

            MappingData mappingData;

            // Load ID (not really necessary here since we already have it from the index)
            dataFile.read(reinterpret_cast<char *>(const_cast<int *>(&id.first)), sizeof(id.first));
            dataFile.read(reinterpret_cast<char *>(const_cast<int *>(&id.second)), sizeof(id.second));

            // Load short_id
            size_t size;
            dataFile.read(reinterpret_cast<char *>(&size), sizeof(size));
            mappingData.short_id.resize(size);
            dataFile.read(mappingData.short_id.data(), size);

            // Load long_id
            dataFile.read(reinterpret_cast<char *>(&size), sizeof(size));
            mappingData.long_id.resize(size);
            dataFile.read(mappingData.long_id.data(), size);

            data[id] = mappingData;
        }

        dataFile.close();
        indexFile.close();

        return data;
    }

    pair<ID, MappingData> get(const streampos &pos, const string &collectionName)
    {
        ifstream dataFile(collectionName + ".bin", ios::binary);

        if (!dataFile.is_open())
        {
            cerr << "Error opening file for reading" << endl;
            return {};
        }

        // Seek to the position in the data file
        dataFile.seekg(pos);

        // Read ID
        ID id;
        dataFile.read(reinterpret_cast<char *>(const_cast<int *>(&id.first)), sizeof(id.first));
        dataFile.read(reinterpret_cast<char *>(const_cast<int *>(&id.second)), sizeof(id.second));

        MappingData mappingData;

        // Read short_id
        size_t size;
        dataFile.read(reinterpret_cast<char *>(&size), sizeof(size));
        mappingData.short_id.resize(size);
        dataFile.read(mappingData.short_id.data(), size);

        // Read long_id
        dataFile.read(reinterpret_cast<char *>(&size), sizeof(size));
        mappingData.long_id.resize(size);
        dataFile.read(mappingData.long_id.data(), size);

        return {id, mappingData};
    }

    MappingData getByID(const ID &id, const string &collectionName)
    {
        ifstream indexFile(collectionName + ".id.idx", ios::binary);

        if (!indexFile.is_open())
        {
            cerr << "Error opening file for reading" << endl;
            return MappingData();
        }

        // Search for the index entry
        ID tmpID;
        IndexEntry indexEntry;
        bool found = false;
        while (indexFile.read(reinterpret_cast<char *>(&tmpID.first), sizeof(tmpID.first)) &&
               indexFile.read(reinterpret_cast<char *>(&tmpID.second), sizeof(tmpID.second)) &&
               indexFile.read(reinterpret_cast<char *>(&indexEntry.pos), sizeof(indexEntry.pos)) &&
               indexFile.read(reinterpret_cast<char *>(&indexEntry.size), sizeof(indexEntry.size)))
        {
            if (tmpID == id)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            cerr << "ID not found in index" << endl;
            return MappingData();
        }

        auto [_id, mappingData] = get(indexEntry.pos, collectionName);

        return mappingData;
    }

    // template <typename T>
    // MappingData getByValue(const string &collectionName, const T &key)
    // {
    //     auto index
    // }

    MappingData getByShortID(const string &short_id, const string &collectionName)
    {
        ifstream dataFile(collectionName + ".bin", ios::binary);
        ifstream indexShortFile(collectionName + ".short.idx", ios::binary);

        if (!dataFile.is_open() || !indexShortFile.is_open())
        {
            cerr << "Error opening file for reading" << endl;
            return MappingData();
        }

        // Search for the index entry
        string tmpShortID;
        char ch;
        IndexEntry indexEntry;
        bool found = false;

        while (indexShortFile.get(ch))
        {
            if (ch == '\0')
            {
                if (!indexShortFile.read(reinterpret_cast<char *>(&indexEntry.pos), sizeof(indexEntry.pos)) ||
                    !indexShortFile.read(reinterpret_cast<char *>(&indexEntry.size), sizeof(indexEntry.size)))
                {
                    cerr << "Error reading index" << endl;
                    return {};
                }
                if (tmpShortID == short_id)
                {
                    found = true;
                    break;
                }
                tmpShortID.clear();
            }
            else
            {
                tmpShortID += ch;
            }
        }

        if (!found)
        {
            cerr << "Short ID not found in index" << endl;
            return MappingData();
        }

        auto [_id, mappingData] = get(indexEntry.pos, collectionName);

        return mappingData;
    }

    MappingData getByLongID(const string &long_id, const string &collectionName)
    {
        ifstream dataFile(collectionName + ".bin", ios::binary);
        ifstream indexFile(collectionName + ".long.idx", ios::binary);

        if (!dataFile.is_open() || !indexFile.is_open())
        {
            cerr << "Error opening file for reading" << endl;
            return MappingData();
        }

        // Search for the index entry
        string tmpLongID;
        char ch;
        IndexEntry indexEntry;
        bool found = false;
        while (indexFile.get(ch))
        {
            if (ch == '\0')
            {
                if (!indexFile.read(reinterpret_cast<char *>(&indexEntry.pos), sizeof(indexEntry.pos)) ||
                    !indexFile.read(reinterpret_cast<char *>(&indexEntry.size), sizeof(indexEntry.size)))
                {
                    cerr << "Error reading index" << endl;
                    return {};
                }
                if (tmpLongID == long_id)
                {
                    found = true;
                    break;
                }
                tmpLongID.clear();
            }
            else
            {
                tmpLongID += ch;
            }
        }

        if (!found)
        {
            cerr << "Long ID not found in index" << endl;
            return MappingData();
        }

        auto [_id, mappingData] = get(indexEntry.pos, collectionName);

        return mappingData;
    }
} // namespace station
