#pragma once

#include "Location.h"

struct State;

class Ant
{
public:
	Ant(void);
	~Ant(void);

	Ant(Location loc, State & state);

	void chooseMove(State & state);
	int calculateValue(Location & loc, State & state);
	Location location;
};

