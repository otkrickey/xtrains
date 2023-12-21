#pragma once

#include <fstream>
#include <string>
#include <map>
#include <type_traits>
#include <cstddef>

#include "model.h"

namespace database_v3
{
    using namespace model;

    // toBinary: convert value to binary and write to outputFileStream
    // fromBinary: read from inputFileStream and convert to value

    // Base class for serializable types
    template <typename T>
    struct is_serializable : std::false_type
    {
    };

    // specialization for `std::string`
    template <>
    struct is_serializable<std::string> : std::true_type
    {
        static void toBinary(const std::string &value, std::ofstream &ofs)
        {
            size_t size = value.size();
            ofs.write(reinterpret_cast<const char *>(&size), sizeof(size));
            ofs.write(value.c_str(), size);
        }

        static std::string fromBinary(std::ifstream &ifs)
        {
            size_t size;
            ifs.read(reinterpret_cast<char *>(&size), sizeof(size));
            std::string value(size, '\0');
            ifs.read(&value[0], size);
            return value;
        }
    };

    // specialization for `int`
    template <>
    struct is_serializable<int> : std::true_type
    {
        static void toBinary(const int &value, std::ofstream &ofs)
        {
            ofs.write(reinterpret_cast<const char *>(&value), sizeof(value));
        }
        static int fromBinary(std::ifstream &ifs)
        {
            int value;
            ifs.read(reinterpret_cast<char *>(&value), sizeof(value));
            return value;
        }
    };

    // specialization for `std::map<Key, Value>`
    template <typename Key, typename Value>
    struct is_serializable<std::map<Key, Value>> : std::true_type
    {
        static void toBinary(const std::map<Key, Value> &value, std::ofstream &ofs)
        {
            size_t size = value.size();
            ofs.write(reinterpret_cast<const char *>(&size), sizeof(size));
            for (auto &i : value)
            {
                is_serializable<Key>::toBinary(i.first, ofs);
                is_serializable<Value>::toBinary(i.second, ofs);
            }
        }
        static std::map<Key, Value> fromBinary(std::ifstream &ifs)
        {
            size_t size;
            ifs.read(reinterpret_cast<char *>(&size), sizeof(size));
            std::map<Key, Value> value;
            for (size_t i = 0; i < size; i++)
            {
                Key key = is_serializable<Key>::fromBinary(ifs);
                Value val = is_serializable<Value>::fromBinary(ifs);
                value[key] = val;
            }
            return value;
        }
    };

    // General serialize function
    template <typename T>
    void toBinary(const T &value, std::ofstream &ofs)
    {
        static_assert(is_serializable<T>::value, "Type is not serializable");
        return is_serializable<T>::toBinary(value, ofs);
    }
    // General deserialize function
    template <typename T>
    T fromBinary(std::ifstream &ifs)
    {
        static_assert(is_serializable<T>::value, "Type is not deserializable");
        return is_serializable<T>::fromBinary(ifs);
    }
} // namespace database_v3