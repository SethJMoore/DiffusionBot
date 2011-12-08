#pragma once

struct State;

class AntArmy
{
public:
	AntArmy(void);
	~AntArmy(void);

	void updateAnts(State & state);
};

