#pragma once
class Obyvatelstvo
{
private:
	int women{};
	int men{};
	int year{};
public:

	Obyvatelstvo() = default;

	Obyvatelstvo(int women, int men, int year) {
		this->women = women;
		this->men = men;
		this->year = year;
	}

	const int getWomen() const {
		return this->women;
	}

	const int getMen() const {
		return this->men;
	}

	const int getYear() const {
		return this->year;
	}

	void addObyvatelstvo(int women, int men) {
		this->women += women;
		this->men += men;
	}

	bool operator==(const Obyvatelstvo& other) const
	{
		return this->getYear() == other.getYear();
	}
	bool operator!=(const Obyvatelstvo& other) const
	{
		return this->getYear() != other.getYear();
	}
};

