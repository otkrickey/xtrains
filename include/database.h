#pragma once

#include <string>
#include <map>
#include <fstream>
#include <functional>
#include <variant>
#include <memory>
#include <mutex>
#include <iostream>
#include <filesystem>

#include "serialization.h"
#include "model.h"

namespace database_v3
{
    using namespace model;

    template <typename Model, typename Value>
    using ExtractorFunc = std::function<Value(const Model &)>;

    template <typename Key, typename Model, typename Value>
    class Collection
    {
    public:
        std::string databaseName;
        std::string collectionName;
        bool isLoaded = false;

    private:
        std::map<Key, Value> data_;
        ExtractorFunc<Model, Value> extractor_;

    public:
        Collection(const std::string &db_name, const std::string &coll_name, ExtractorFunc<Model, Value> extractor) : databaseName(db_name), collectionName(coll_name), extractor_(extractor) {}
        void set(const std::map<Key, Model> &data)
        {
            for (auto &i : data)
            {
                data_[i.first] = extractor_(i.second);
            }
        }
        std::map<Key, Value> data()
        {
            if (!isLoaded)
            {
                this->load();
            }
            return data_;
        }
        void add(const Key &key, const Model &model) { data_[key] = extractor_(model); }
        void summary() { std::cout << "Count of `data/" << databaseName << "/" << collectionName << ".bin`: " << data_.size() << std::endl; }
        void save()
        {
            std::string databaseDirectory = "data/" + databaseName;
            std::filesystem::create_directories(databaseDirectory);
            std::ofstream outputFileStream(databaseDirectory + "/" + collectionName + ".bin", std::ios::binary);
            if (!outputFileStream.is_open())
            {
                std::cerr << "Error: Cannot open file " << databaseDirectory + "/" + collectionName + ".bin" << std::endl;
                throw std::runtime_error("[database_v3::Collection::save] Cannot open file " + databaseDirectory + "/" + collectionName + ".bin");
            }
            std::cout << "[database_v3::Collection(" << databaseName << "::" << collectionName << ")::save] Save data to file" << std::endl;
            toBinary<std::map<Key, Value>>(data_, outputFileStream);
        }
        void load()
        {
            std::string databaseDirectory = "data/" + databaseName;
            std::ifstream inputFileStream(databaseDirectory + "/" + collectionName + ".bin", std::ios::binary);
            if (!inputFileStream.is_open())
            {
                std::cerr << "Error: Cannot open file " << databaseDirectory + "/" + collectionName + ".bin" << std::endl;
                throw std::runtime_error("[database_v3::Collection::load] Cannot open file " + databaseDirectory + "/" + collectionName + ".bin");
            }
            this->data_ = fromBinary<std::map<Key, Value>>(inputFileStream);
            isLoaded = true;
        }
    };

    template <typename Key, typename Model>
    class BaseDatabase
    {
    public:
        std::string databaseName;
        std::map<Key, Model> data;

    private:
        struct ICollection
        {
            virtual ~ICollection() = default;
            virtual void add(const Key &key, const Model &model) = 0;
            virtual void save() = 0;
            virtual void load() = 0;
        };
        template <typename Value>
        struct CollectionWrapper : ICollection
        {
            Collection<Key, Model, Value> collection;
            CollectionWrapper(const std::string &db_name, const std::string &coll_name, ExtractorFunc<Model, Value> extractor) : collection(db_name, coll_name, extractor) {}
            void add(const Key &key, const Model &model) override { collection.add(key, model); }
            void save() override { collection.save(); }
            void load() override { collection.load(); }
        };
        std::map<std::string, std::shared_ptr<ICollection>> collections;

    public:
        BaseDatabase(const std::string &databaseName) : databaseName(databaseName) {}
        template <typename Value>
        void createCollection(
            const std::string &collectionName,
            std::map<Key, Model> &data,
            ExtractorFunc<Model, Value> extractor)
        {
            auto wrapper = std::make_shared<CollectionWrapper<Value>>(databaseName, collectionName, extractor);
            wrapper->collection.set(data);
            collections[collectionName] = wrapper;
        }
        template <typename Value>
        void loadCollection(
            const std::string &collectionName,
            ExtractorFunc<Model, Value> extractor)
        {
            auto wrapper = std::make_shared<CollectionWrapper<Value>>(databaseName, collectionName, extractor);
            wrapper->collection.load();
            collections[collectionName] = wrapper;
        }
        template <typename Value>
        Collection<Key, Model, Value> &getCollection(const std::string &collectionName)
        {
            auto wrapper = std::dynamic_pointer_cast<CollectionWrapper<Value>>(collections[collectionName]);
            if (!wrapper)
            {
                std::cerr << "Error: Cannot get collection " << collectionName << std::endl;
                throw std::runtime_error("[database_v3::BaseDatabase::getCollection] Cannot get collection " + collectionName);
            }
            return wrapper->collection;
        }
        template <typename Value>
        std::map<Key, Value> getData(const std::string &collectionName)
        {
            auto wrapper = std::dynamic_pointer_cast<CollectionWrapper<Value>>(collections[collectionName]);
            if (!wrapper)
            {
                std::cerr << "Error: Cannot get collection " << collectionName << std::endl;
                throw std::runtime_error("[database_v3::BaseDatabase::getData] Cannot get collection " + collectionName);
            }
            return wrapper->collection.data();
        }
        void set(std::map<Key, Model> &data) { this->data = data; }
        void add(const Key &key, const Model &model)
        {
            data[key] = model;
            for (auto &coll : collections)
            {
                coll.second->add(key, model);
            }
        }
        void save()
        {
            for (auto &coll : collections)
            {
                coll.second->save();
            }
        }
        void load()
        {
            for (auto &coll : collections)
            {
                coll.second->load();
            }
        }
    };

    template <typename Key, typename Model>
    class Database
    {
    private:
        static std::unique_ptr<Database<Key, Model>> instance;
        static std::mutex mutex;
        static std::string databaseName;
        BaseDatabase<Key, Model> db;
        Database() : db(databaseName) {}

    public:
        Database(const Database &) = delete;
        Database &operator=(const Database &) = delete;
        static void deleteInstance()
        {
            std::lock_guard<std::mutex> lock(mutex);
            instance.reset(nullptr);
        }
        static BaseDatabase<Key, Model> *createDB()
        {
            std::map<Key, Model> empty;
            return createDB(empty);
        }
        static BaseDatabase<Key, Model> *createDB(std::map<Key, Model> &data)
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (instance == nullptr)
            {
                instance = std::unique_ptr<Database<Key, Model>>(new Database<Key, Model>());
                instance->extract(data);
                instance->db.set(data);
            }
            return &instance->db;
        }

        static BaseDatabase<Key, Model> *loadDB()
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (instance == nullptr)
            {
                instance = std::unique_ptr<Database<Key, Model>>(new Database<Key, Model>());
                instance->extract();
                instance->update();
            }
            return &instance->db;
        }

    private:
        void createDB_(std::map<Key, Model> &data);
        void openDB_();
        void loadDB_();

        void extract();
        void extract(std::map<Key, Model> &data);
        void update();
    };

    int test();
} // namespace database_v3
