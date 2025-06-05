#pragma once

#include <complexities/complexity_analyzer.h>
#include <libds/adt/table.h>
#include <random>
#include <string>
#include <memory>

namespace ds::utils
{
    class HashTableInsertAnalyzer : public ComplexityAnalyzer<ds::adt::HashTable<int, int>>
    {
    public:
        HashTableInsertAnalyzer()
            : ComplexityAnalyzer("HashTable-Insert"), rng_(144), distKey_(1, 1000000), distValue_(1, 1000000) {
        }

        void growToSize(ds::adt::HashTable<int, int>& structure, size_t size) override
        {
            while (structure.size() < size)
            {
                int key = distKey_(rng_);
                int value = distValue_(rng_);
                if (!structure.contains(key))
                {
                    structure.insert(key, value);
                }
            }
        }

        void executeOperation(ds::adt::HashTable<int, int>& structure) override
        {
            int key = distKey_(rng_);
            int value = distValue_(rng_);
            if (!structure.contains(key))
            {
                structure.insert(key, value);
            }
        }

    private:
        std::default_random_engine rng_;
        std::uniform_int_distribution<int> distKey_;
        std::uniform_int_distribution<int> distValue_;
    };

    class HashTableFindAnalyzer : public ComplexityAnalyzer<ds::adt::HashTable<int, int>>
    {
    public:
        HashTableFindAnalyzer()
            : ComplexityAnalyzer("HashTable-Find"), rng_(144), distKey_(1, 1000000) {
        }

        void growToSize(ds::adt::HashTable<int, int>& structure, size_t size) override
        {
            while (structure.size() < size)
            {
                int key = distKey_(rng_);
                int value = key * 10;
                if (!structure.contains(key))
                {
                    structure.insert(key, value);
                }
            }
        }

        void executeOperation(ds::adt::HashTable<int, int>& structure) override
        {
            int key = distKey_(rng_);
            int* result = nullptr;
            structure.tryFind(key, result);
        }

    private:
        std::default_random_engine rng_;
        std::uniform_int_distribution<int> distKey_;
    };

}