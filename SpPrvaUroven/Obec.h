#pragma once
#include <string>
#include <vector>
#include "Obyvatelstvo.h"
#include <libds/amt/implicit_sequence.h>
class Obec
{
private:
	std::string code{};
	std::string kodObce{};
	std::string name{};
	std::string kodOtca{};
	std::string type{};
	std::vector <Obyvatelstvo> sequence{};
	ds::amt::ImplicitSequence<Obyvatelstvo> sequence2{};

public:
	Obec() = default;
	virtual ~Obec() = default;

	Obec(std::string& code, std::string& name)
	{
		this->code = code;
		this->name = name;
		switch (code.size())
		{
		case 2:
			this->type = "Stat";
			break;
		case 3:
			this->type = "Geo";
			break;
		case 4:
			this->type = "Spolk";
			break;
		case 5:
			this->type = "Region";
			break;
		default:
			break;
		}


	}

	Obec(std::string& code, std::string& name, std::string& kodOtca)
	{
		this->code = code;
		this->name = name;

		switch (code.size())
		{
		case 2:
			this->type = "Stat";
			break;
		case 3:
			this->type = "Geo";
			break;
		case 4:
			this->type = "Spolk";
			break;
		case 5:
			this->type = "Region";
			break;
		default:
			break;
		}
		this->kodOtca = kodOtca;
	}

	Obec(std::string& code, std::string& name, std::string& kodObce, std::string& kodOtca)
	{
		this->code = code;
		this->name = name;
		this->kodObce = kodObce;
		this->kodOtca = kodOtca;
		this->type = "Obec";
	}

	std::string getCode() const
	{
		return this->code;
	}

	std::string getName() const
	{
		return this->name;
	}

	std::string getKodObce() const
	{
		return this->kodObce;
	}

	std::string getKodOtca() const
	{
		return this->kodOtca;
	}

	std::string getType() const
	{
		return type;
	}

	void addToPopulation(int women, int men, int year)
	{
		sequence.push_back(Obyvatelstvo(women, men, year));
	}

	void addToPopulationByYear(int women, int men, int year)
	{
		for (auto& pop : sequence)
		{
			if (pop.getYear() == year)
			{
				pop.addObyvatelstvo(women, men);
				return;
			}
		}
		sequence.push_back(Obyvatelstvo(women, men, year));
	}



	const std::vector<Obyvatelstvo>& getPopulationData() const
	{
		return sequence;
	}


	const Obyvatelstvo* getPopulationDataByYear(int year) const{

		for (const auto& pop : sequence)
		{
			if (pop.getYear() == year)
			{
				return &pop;
			}
		}
		return nullptr; 
	}


	bool operator==(const Obec& other) const
	{
		return this->code == other.code;
	}
	bool operator!=(const Obec& other) const
	{
		return this->code != other.code;
	}
};

