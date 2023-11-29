#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>

namespace data
{
    using namespace std;

    namespace station
    {

        typedef pair<int, int> ID;
        struct Values
        {
            string short_id, long_id;
        };
        typedef map<ID, Values> Data;

        struct IndexEntry
        {
            streampos pos;
            streamoff size;
        };
        enum class IndexType
        {
            ShortID,
            LongID,
        };
        extern map<IndexType, string> index_map;
        extern map<IndexType, string> filename_map;

        class DB
        {
        private:
            const string collectionName;
            ofstream dataFile;
            vector<IndexType> indexTypes;
            map<IndexType, ofstream> indexFiles;

            // template <typename T>
            // void saveIndex(const T &index, const string &filename);
            template <typename T>
            T readIndex(ifstream &indexFile);

        public:
            DB(const string &collectionName, const vector<IndexType> &indexTypes);
            // ~DB();
            // void save(const string &short_id, const string &long_id);

            Data read(const streampos &pos);
            Data get(const IndexType &by, const string &id);
        };
    }
} // namespace data

namespace _station
{
    using namespace std;

    typedef pair<int, int> ID;
    struct MappingData
    {
        string short_id, long_id;
    };
    typedef map<ID, MappingData> Data;

    struct IndexEntry
    {
        streampos pos;
        streamoff size;
    };
    typedef map<ID, IndexEntry> IndexID;
    typedef map<string, IndexEntry> IndexShortID;
    typedef map<string, IndexEntry> IndexLongID;
    // using IndexID = unordered_map<ID, IndexEntry>;
    // using IndexShortID = unordered_map<string, IndexEntry>;
    // using IndexLongID = unordered_map<string, IndexEntry>;

    void save(const Data &data, const string &collectionName);
    Data load(const string &collectionName);
    // template <typename T>
    // T loadIndex(const string &filename);
    MappingData getByID(const ID &id, const string &collectionName);
    MappingData getByShortID(const string &short_id, const string &collectionName);
    MappingData getByLongID(const string &long_id, const string &collectionName);
} // namespace station