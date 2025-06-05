#pragma once
#include <libds/amt/implicit_sequence.h>
#include <libds/amt/explicit_hierarchy.h>
#include <libds/adt/table.h>
#include <libds/adt/array.h>
#include <string>
#include <fstream>
#include "Obec.h"
#include "Iterator.h"
#include "Sort.h"


class ReadCSV
{
private:

	std::string path{};
	std::string line{};
	ds::amt::ImplicitSequence<Obec*> sequence{};
	ds::amt::ImplicitSequence<Obec*> sequence2{};
	ds::amt::MultiWayExplicitHierarchy<Obec*> hierarchy;
	ds::adt::HashTable<std::string, ds::amt::ImplicitSequence<Obec*>*> hashTable;
	Sort sort;

	std::string toLower(const std::string& input)
	{
		std::string output = input;
		std::transform(output.begin(), output.end(), output.begin(), ::tolower);
		return output;
	}
	bool filterByMax(const Obec& row, int num, int year)
	{
		const auto* pop = row.getPopulationDataByYear(year);
		if (pop == nullptr)
			return false;

		return (pop->getMen() + pop->getWomen()) <= num;
	}

	bool filterByMin(const Obec& row, int num, int year)
	{
		const auto* pop = row.getPopulationDataByYear(year);
		if (pop == nullptr)
			return false;

		return (pop->getMen() + pop->getWomen()) >= num;
	}

	bool filterByString(const Obec& row, std::string& str)
	{
		std::string name = row.getName();
		std::string query = str;

		return name.find(query) != std::string::npos;
	}

	bool filterByType(const Obec& row, std::string& type)
	{
		return type == row.getType();
	}


	ds::amt::MultiWayExplicitHierarchyBlock<Obec*>* getParentBlock(Iterator<Obec*>* it, const std::string& kodOtca)
	{
		if (it->getCurrent()->getCode() == kodOtca)
		{
			return it->getCurrentBlock();
		}

		size_t index = 0;
		while (it->goToSon(index))
		{
			auto result = getParentBlock(it, kodOtca);
			if (result != nullptr)
				return result;

			it->goToParent();
			++index;
		}

		return nullptr;
	}

	void readData()
	{
		ds::amt::ImplicitSequence<std::string> uzem2;
		uzem2.insertLast().data_ = "uzemie";
		uzem2.insertLast().data_ = "obce";

		ds::amt::ImplicitSequence<std::string> roky2;
		roky2.insertLast().data_ = "2020";
		roky2.insertLast().data_ = "2021";
		roky2.insertLast().data_ = "2022";
		roky2.insertLast().data_ = "2023";
		roky2.insertLast().data_ = "2024";

		for (const std::string uz : uzem2)
		{


			path = "data/" + uz + ".csv";
			std::ifstream file(path);

			if (!file.is_open())
			{
				std::cout << "Error: File not found!" << std::endl;
			}

			while (std::getline(file, line))
			{
				std::string name, kodObce, code, token;
				int tokenCount = 0;

				std::stringstream ss(line);

				while (std::getline(ss, token, ';'))
				{
					if (tokenCount == 0)
					{
						name = token;
					}
					else if (tokenCount == 1)
					{
						kodObce = token;
					}
					else if (tokenCount == 2)
					{
						code = token;
					}
					tokenCount++;
				}

				if (tokenCount >= 6)
				{
					kodObce.erase(std::remove(kodObce.begin(), kodObce.end(), '<'), kodObce.end());
					kodObce.erase(std::remove(kodObce.begin(), kodObce.end(), '>'), kodObce.end());

					std::string parentCode = kodObce.substr(0, kodObce.size() - 1);

					auto obec = new Obec(kodObce, name, parentCode);

					sequence.insertLast().data_ = obec;

				}
				if (tokenCount == 3)
				{
					kodObce.erase(std::remove(kodObce.begin(), kodObce.end(), '<'), kodObce.end());
					kodObce.erase(std::remove(kodObce.begin(), kodObce.end(), '>'), kodObce.end());

					auto obec = new Obec(code, name, kodObce, code);
					sequence.insertLast().data_ = obec;
				}

			}
		}

		for (const std::string& rok : roky2)
		{

			path = "data/" + rok + ".csv";
			std::ifstream file(path);

			if (!file.is_open())
			{
				std::cout << "Error: File not found!" << std::endl;
				continue;
			}

			while (std::getline(file, line))
			{
				std::string name, code, male, female, total, token;
				int tokenCount = 0;

				std::stringstream ss(line);

				while (std::getline(ss, token, ';'))
				{
					switch (tokenCount)
					{
					case 0: name = token; break;
					case 1: code = token; break;
					case 2: male = token; break;
					case 4: female = token; break;
					}
					tokenCount++;
				}

				if (tokenCount >= 6 && name != "" && code != "<0>")
				{
					code.erase(std::remove(code.begin(), code.end(), '<'), code.end());
					code.erase(std::remove(code.begin(), code.end(), '>'), code.end());


					bool exists = false;

					for (auto& obec : sequence)
					{
						if (obec->getKodObce() == code)
						{
							obec->addToPopulation(std::stoi(female), std::stoi(male), std::stoi(rok));
							exists = true;
							break;
						}
					}

					if (!exists)
					{
						Obec obec(code, name);
						obec.addToPopulation(std::stoi(female), std::stoi(male), std::stoi(rok));
						sequence.insertLast().data_ = &obec;
					}

				}
			}
		}
	}


    void printSequence(ds::amt::ImplicitSequence<Obec*> seq, int year=2024)
    {
		std::cout << "/--------------------------/" << std::endl;
        for (auto it = seq.begin(); it != seq.end(); ++it)
        {
            const Obec* row = *it;
            if (year != 0)
            {
                std::cout << std::setw(12) << std::left << row->getPopulationDataByYear(year)->getMen() + row->getPopulationDataByYear(year)->getWomen() << "Spolu" << " | ";
            }
			if (year != 0)
			{
				std::cout << std::setw(12) << std::left << row->getPopulationDataByYear(year)->getMen() << "Muzi" << " | ";
			}
			if (year != 0)
			{
				std::cout << std::setw(12) << std::left << row->getPopulationDataByYear(year)->getWomen() << "Zeny" << " | ";
			}
            std::cout << std::setw(5) << std::left << row->getName() << std::endl;
        }

		std::cout << "/--------------------------/" << std::endl;
        std::cout << "Pocet obci je: " << seq.size() << std::endl;
		std::cout << "/--------------------------/" << std::endl;
    }
	
	void createHierarchy()
	{
		readData();

		std::string kod = "AT";
		std::string nazov = "Rakusko";
		hierarchy.emplaceRoot().data_ = new Obec(kod, nazov);
		auto* root = hierarchy.accessRoot();

		for (auto& obec : sequence)
		{
			const std::string& kodOtca = obec->getKodOtca();
			Iterator<Obec*> it(&hierarchy);

			auto* parentBlock = getParentBlock(&it, kodOtca);


			if (parentBlock != nullptr)
			{
				size_t degree = 0;
				while (hierarchy.accessSon(*parentBlock, degree) != nullptr)
				{
					++degree;
				}
				hierarchy.emplaceSon(*parentBlock, degree).data_ = obec;

				for ( auto& pop : obec->getPopulationData())
				{
					auto* current = parentBlock;
					while (current != nullptr)
					{
						current->data_->addToPopulationByYear(pop.getWomen(), pop.getMen(), pop.getYear());
						current = hierarchy.accessParent(*current);
					}
				}


			}
		}
	}


public:
	ReadCSV() {

	};
	~ReadCSV() {
		for (auto it = hashTable.begin(); it != hashTable.end(); ++it)
		{
			delete (*it).data_;
		}
	};

	void printHierarchy()
	{
		createHierarchy();

		Iterator<Obec*> it(&hierarchy);

		char choice, typeChoice;
		int num, year;
		std::string nazov, type;

		do {

			std::cout << "--------------------------" << std::endl;
			it.printCurrentPath();
			std::cout << "--------------------------" << std::endl;
			it.printSons();
			std::cout << "--------------------------" << std::endl;

			std::cout << "[0] Chod na syna podla indexu" << std::endl;
			std::cout << "[1] Chod na rodica" << std::endl;
			std::cout << "[2] hasMinResidents" << std::endl;
			std::cout << "[3] hasMaxResidents" << std::endl;
			std::cout << "[4] containsStr" << std::endl;
			std::cout << "[5] hasType" << std::endl;
			std::cout << "[6] udaje" << std::endl;
			std::cout << "[7] compareAlpahabetical" << std::endl;
			std::cout << "[8] comparePopulation" << std::endl;
			std::cout << "[9] Koniec" << std::endl;
			std::cin >> choice;

			if (choice == '0') {
				size_t index;
				std::cout << "Zadaj index syna: ";
				std::cin >> index;
				system("cls");
				if (!it.goToSon(index)) {
					std::cout << "Neplatny index!" << std::endl;
				}
			}
			else if (choice == '1') {
				system("cls");
				if (!it.goToParent()) {
					std::cout << "Nema rodica!" << std::endl;
				}
			}
			else if (choice == '2') {
				std::cout << "Zadaj minimalny pocet obyvatelov: ";
				std::cin >> num;
				std::cout << "Zadaj rok: ";
				std::cin >> year;
				system("cls");
				filterResidents(num, year, it, false);
			}
			else if (choice == '3') {
				std::cout << "Zadaj maximalny pocet obyvatelov: ";
				std::cin >> num;
				std::cout << "Zadaj rok: ";
				std::cin >> year;

				system("cls");
				filterResidents(num, year, it, true);
			}
			else if (choice == '4') {
				std::cout << "Zadaj nazov: ";
				std::cin >> nazov;

				system("cls");
				containsStr(nazov, it);
			}
			else if (choice == '5') {
				std::cout << "Zadaj typ: " << std::endl;
				std::cout << "[0] Stat" << std::endl;
				std::cout << "[1] Geograficka lokacia" << std::endl;
				std::cout << "[2] Spolkova republika" << std::endl;
				std::cout << "[3] Region" << std::endl;
				std::cout << "[4] Obec" << std::endl;
				std::cin >> typeChoice;

				if (typeChoice == '0')
				{
					type = "Stat";
					system("cls");
					hasType(type, it);
				}
				else if (typeChoice == '1')
				{
					type = "Geo";
					system("cls");
					hasType(type, it);
				}
				else if (typeChoice == '2')
				{
					type = "Spolk";
					system("cls");
					hasType(type, it);
				}
				else if (typeChoice == '3')
				{
					type = "Region";
					system("cls");
					hasType(type, it);
				}
				else if (typeChoice == '4')
				{
					type = "Obec";
					system("cls");
					hasType(type, it);
				}

			}
			else if (choice == '6') {
				system("cls");
				for (const auto& pop : it.getCurrent()->getPopulationData())
				{
					std::cout << "Rok: " << pop.getYear()
						<< " | Muzi: " << pop.getMen()
						<< " | Zeny: " << pop.getWomen()
						<< " | Spolu: " << pop.getMen() + pop.getWomen()
						<< std::endl;
				}

			}
			else if (choice == '7')
			{
				system("cls");

				if (sequence2.size() < 1)
				{
					std::cout << "Ziadne obce neboli vyfiltrovane!" << std::endl;
					return;
				}
				else
				{
					auto sorted = sort.compareAlphabetical(sequence2);
					printSequence(sorted);
				}

			}
			else if (choice == '8')
			{
				system("cls");

				if (sequence2.size() < 1)
				{
					std::cout << "Ziadne obce neboli vyfiltrovane!" << std::endl;
				}
				else
				{
					char genderChoice;
					int yearChoice;

					std::cout << "Zadaj pohlavie: " << std::endl;
					std::cout << "[0] Muzi" << std::endl;
					std::cout << "[1] Zeny" << std::endl;
					std::cout << "[2] Spolu" << std::endl;
					std::cin >> genderChoice;
					std::cout << "Zadaj rok: (2020-2024)" << std::endl;
					std::cin >> yearChoice;
					auto sorted = sort.comparePopulation(sequence2, yearChoice, genderChoice);
					printSequence(sorted);
				}

			}
			else if (choice == '9') {
				system("cls");
				return;
			}
			else {
				system("cls");
				std::cout << "Neplatna volba!" << std::endl;
			}

		} while (choice != '10');
	}

	void filterResidents(int num, int year, Iterator<Obec*>& it, bool max)
	{
		sequence2.clear();

		std::function<void(Iterator<Obec*>&)> recursiveFilter = [&](Iterator<Obec*>& iter)
			{
				if ((max ? filterByMax(*iter.getCurrent(), num, year) : filterByMin(*iter.getCurrent(), num, year)))
				{
					sequence2.insertLast().data_ = iter.getCurrent();
				}

				size_t index = 0;
				while (iter.goToSon(index))
				{
					Iterator<Obec*> childIter = iter;
					recursiveFilter(childIter);
					iter.goToParent();
					index++;
				}
			};

		recursiveFilter(it);
		printSequence(sequence2, year);
	}

	void containsStr(std::string& str, Iterator<Obec*>& it)
	{
		sequence2.clear();
		std::function<void(Iterator<Obec*>&)> recursiveFilter = [&](Iterator<Obec*>& iter)
			{
				if (filterByString(*iter.getCurrent(), str))
				{
					sequence2.insertLast().data_ = iter.getCurrent();
				}

				size_t index = 0;
				while (iter.goToSon(index))
				{
					Iterator<Obec*> childIter = iter;
					recursiveFilter(childIter);
					iter.goToParent();
					++index;
				}
			};

		recursiveFilter(it);
		printSequence(sequence2, 0);
	}

	void hasType(std::string& type, Iterator<Obec*>& it)
	{
		sequence2.clear();
		std::function<void(Iterator<Obec*>&)> recursiveFilter = [&](Iterator<Obec*>& iter)
			{
				if (filterByType(*iter.getCurrent(), type))
				{
					sequence2.insertLast().data_ = iter.getCurrent();
				}

				size_t index = 0;
				while (iter.goToSon(index))
				{
					Iterator<Obec*> childIter = iter;
					recursiveFilter(childIter);
					iter.goToParent();
					++index;
				}
			};

		recursiveFilter(it);
		printSequence(sequence2, 0);
	}

	void populateHashTable()
	{
		createHierarchy();

		for (auto& obec : sequence)
		{
			std::string kluc = toLower(obec->getName());

			if (!hashTable.contains(kluc))
			{
				auto* novaSekvencia = new ds::amt::ImplicitSequence<Obec*>();
				novaSekvencia->insertLast().data_ = obec;
				hashTable.insert(kluc, novaSekvencia);
			}
			else
			{
				auto* sekvencia = hashTable.find(kluc);
				sekvencia->insertLast().data_ = obec;
			}
		}

		std::cout << "Tabulka bola vytvorena, pocet riadkov: [" << hashTable.size() << "]" << std::endl;
	}


	ds::amt::ImplicitSequence<Obec*> findByName(const std::string& name, const std::string& type) {
		sequence2.clear();

		std::string kluc = toLower(name);

		if (hashTable.contains(kluc)) {
			ds::amt::ImplicitSequence<Obec*>* sekvencia = hashTable.find(kluc);

			for (auto it = sekvencia->begin(); it != sekvencia->end(); ++it) {
				if (toLower((*it)->getType()) == toLower(type)) {
					sequence2.insertLast().data_ = *it;
				}
			}
		}

		return sequence2;
	}

	void printTable()
	{
		populateHashTable();

		std::string nazov, type;
		char typeChoice, choice;
		size_t obecChoice;
		Obec* resultObec = nullptr;

		while (true)
		{
			std::cout << "---------------------------------------------" << std::endl;

			std::cout << "[0] Vyber obce" << std::endl;
			std::cout << "[1] Koniec" << std::endl;
			std::cin >> choice;
			if (choice == '0')
			{
				system("cls");
				std::cout << "Zadaj nazov obce: ";
				std::getline(std::cin >> std::ws, nazov);
				std::cout << "Zadaj typ: " << std::endl;
				std::cout << "[0] Stat" << std::endl;
				std::cout << "[1] Geograficka lokacia" << std::endl;
				std::cout << "[2] Spolkova republika" << std::endl;
				std::cout << "[3] Region" << std::endl;
				std::cout << "[4] Obec" << std::endl;
				std::cin >> typeChoice;
				if (typeChoice == '0')
				{
					type = "Stat";
					system("cls");
				}
				else if (typeChoice == '1')
				{
					type = "Geo";
					system("cls");
				}
				else if (typeChoice == '2')
				{
					type = "Spolk";
					system("cls");
				}
				else if (typeChoice == '3')
				{
					type = "Region";
					system("cls");
				}
				else if (typeChoice == '4')
				{
					type = "Obec";
					system("cls");
				}

				auto result = findByName(nazov, type);
				if (!result.isEmpty()) {

					if (result.size() == 1) {
						resultObec = result.access(0)->data_;
					}
					else {
						std::cout << "Boli najdene obce s tymto nazvom a typom:" << std::endl;
						for (size_t i = 0; i < result.size(); ++i) {
							std::cout << "[" << i << "] " << result.access(i)->data_->getName()
								<< ", " << result.access(i)->data_->getKodObce() << std::endl;
						}
						std::cout << "Zadaj index obce: ";
						std::cin >> obecChoice;

						if (obecChoice >= 0 && static_cast<size_t>(obecChoice) < result.size()) {
							resultObec = result.access(obecChoice)->data_;
						}
						else {
							std::cout << "Neplatny index!" << std::endl;
							return;
						}
					}

					std::cout << "Najdena obec: " << resultObec->getName()
						<< ", " << resultObec->getType() << ", " << resultObec->getCode() << std::endl;

					for (const auto& pop : resultObec->getPopulationData()) {
						std::cout << "Rok: " << pop.getYear()
							<< " | Muzi: " << pop.getMen()
							<< " | Zeny: " << pop.getWomen()
							<< " | Spolu: " << pop.getMen() + pop.getWomen()
							<< std::endl;
					}
				}
				else {
					std::cout << "Obec neexistuje!" << std::endl;
				}
			}
			else if (choice == '1') {
				system("cls");
				return;
			}
			else {
				system("cls");
				std::cout << "Neplatna volba!" << std::endl;
			}




		}
	}

	void start()
	{
		while (true) {
			char choice;

			std::cout << "Vyber si zadanie:" << std::endl;
			std::cout << "[0] 1/2/4 SP" << std::endl;
			std::cout << "[1] 3 SP" << std::endl;
			std::cout << "[2] Exit" << std::endl;
			std::cout << "Zadanie: ";
			std::cin >> choice;

			if (choice == '0')
			{
				system("cls");
				printHierarchy();

			}
			else if (choice == '1') {
				system("cls");

				printTable();
			}
			else if (choice == '2') {
				break;
			}
			else {
				std::cout << "Neplatna volba!" << std::endl;
			}
		}
	}

	ds::amt::ImplicitSequence<Obec*> getSequence() {
		return sequence;
	}

};

