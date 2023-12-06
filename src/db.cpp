#include "../include/db.h"

namespace database
{
    // toBinary: convert value to binary and write to outputFileStream
    template <>
    void toBinary<std::string>(const std::string &value, std::ofstream &outputFileStream)
    {
        size_t size = value.size();
        outputFileStream.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
        outputFileStream.write(value.c_str(), size);
    }
    template <>
    void toBinary<int>(const int &value, std::ofstream &outputFileStream)
    {
        outputFileStream.write(reinterpret_cast<const char *>(&value), sizeof(value));
    }
    template <>
    void toBinary<std::map<station_, time_>>(const std::map<station_, time_> &value, std::ofstream &outputFileStream)
    {
        size_t size = value.size();
        outputFileStream.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
        for (auto &i : value)
        {
            outputFileStream.write(reinterpret_cast<const char *>(&i.first), sizeof(station_));
            outputFileStream.write(reinterpret_cast<const char *>(&i.second), sizeof(time_));
        }
    }

    // fromBinary: read from inputFileStream and convert to value
    template <>
    std::string fromBinary<std::string>(std::ifstream &inputFileStream)
    {
        size_t size;
        inputFileStream.read(reinterpret_cast<char *>(&size), sizeof(size_t));
        std::string value(size, '\0');
        inputFileStream.read(&value[0], size);
        return value;
    }
    template <>
    int fromBinary<int>(std::ifstream &inputFileStream)
    {
        int value;
        inputFileStream.read(reinterpret_cast<char *>(&value), sizeof(value));
        return value;
    }
    template <>
    std::map<station_, time_> fromBinary<std::map<station_, time_>>(std::ifstream &inputFileStream)
    {
        size_t size;
        inputFileStream.read(reinterpret_cast<char *>(&size), sizeof(size_t));
        std::map<station_, time_> value;
        for (size_t i = 0; i < size; i++)
        {
            station_ key;
            time_ val;
            inputFileStream.read(reinterpret_cast<char *>(&key), sizeof(station_));
            inputFileStream.read(reinterpret_cast<char *>(&val), sizeof(time_));
            value[key] = val;
        }
        return value;
    }
} // namespace database