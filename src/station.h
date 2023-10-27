#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>

namespace station
{
    using namespace std;

    class StationBase
    {
    protected:
        std::string collectionName;
        std::ofstream dataFile;
        std::ifstream indexFile;

        StationBase(const std::string &collectionName)
            : collectionName(collectionName) {}

        virtual ~StationBase() {}

        void openFilesForWriting(const std::string &dataFileName, const std::string &indexFileName)
        {
            dataFile.open(dataFileName, std::ios::binary);
            indexFile.open(indexFileName, std::ios::binary);

            if (!dataFile.is_open() || !indexFile.is_open())
            {
                std::cerr << "Error opening file for writing" << std::endl;
                throw std::runtime_error("File open error");
            }
        }

        void openFilesForReading(const std::string &dataFileName, const std::string &indexFileName)
        {
            dataFile.open(dataFileName, std::ios::binary);
            indexFile.open(indexFileName, std::ios::binary);

            if (!dataFile.is_open() || !indexFile.is_open())
            {
                std::cerr << "Error opening file for reading" << std::endl;
                throw std::runtime_error("File open error");
            }
        }

        void closeFiles()
        {
            if (dataFile.is_open())
            {
                dataFile.close();
            }
            if (indexFile.is_open())
            {
                indexFile.close();
            }
        }

        // 他の共通のヘルパー関数をここに定義
    };

} // namespace station