#include <libds/amt/implicit_sequence.h>
#include <libds/adt/sorts.h>
#include "Obec.h"
#include <locale>
#include <codecvt>

#pragma once
class Sort
{
private:
	ds::adt::QuickSort<Obec*> qsort;

    int compareAlphabetical3Way(Obec* a, Obec* b) {
        std::locale german("de_DE.UTF-8");
        const std::collate<char>& coll = std::use_facet<std::collate<char>>(german);

        const std::string& strA = a->getName();
        const std::string& strB = b->getName();

        int result = coll.compare(strA.data(), strA.data() + strA.size(), strB.data(), strB.data() + strB.size());

        if (result < 0) return -1;
        if (result > 0) return 1;
        return 0;
    }

    int comparePopulation3Way(Obec* a, Obec* b, int year, const char gender) {
        const Obyvatelstvo* popA = a->getPopulationDataByYear(year);
        const Obyvatelstvo* popB = b->getPopulationDataByYear(year);

        int valA = 0, valB = 0;

        if (popA) {
            if (gender == '0') valA = popA->getMen();
            else if (gender == '1') valA = popA->getWomen();
            else valA = popA->getMen() + popA->getWomen();
        }

        if (popB) {
            if (gender == '0') valB = popB->getMen();
            else if (gender == '1') valB = popB->getWomen();
            else valB = popB->getMen() + popB->getWomen();
        }

        if (valA < valB) return -1;
        if (valA > valB) return 1;
        return 0;
    }


public:

    ds::amt::ImplicitSequence<Obec*> compareAlphabetical(ds::amt::ImplicitSequence<Obec*>& sequence)
    {
        ds::amt::ImplicitSequence<Obec*> sortedSequence;

        for (auto it = sequence.begin(); it != sequence.end(); ++it) {
            sortedSequence.insertLast().data_ = *it;
        }

        auto comparator = [this](Obec* a, Obec* b) {
            return compareAlphabetical3Way(a, b) < 0;
            };

        qsort.sort(sortedSequence, comparator);

        return sortedSequence;
    }

    ds::amt::ImplicitSequence<Obec*> comparePopulation(ds::amt::ImplicitSequence<Obec*>& sequence, int year, const char gender)
    {
        ds::amt::ImplicitSequence<Obec*> sortedSequence;

        for (auto it = sequence.begin(); it != sequence.end(); ++it) {
            sortedSequence.insertLast().data_ = *it;
        }

        auto comparator = [=](Obec* a, Obec* b) {
            return comparePopulation3Way(a, b, year, gender) < 0;
            };

        qsort.sort(sortedSequence, comparator);

        return sortedSequence;
    }
};

