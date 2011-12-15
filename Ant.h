#pragma once

#include "Location.h"

struct State;

class Ant
{
public:
	Ant(void);
	virtual ~Ant(void);

	Ant(Location loc, State & state);

	virtual void chooseMove(State & state);
	virtual int calculateValue(Location & loc, State & state);
	Location location;
};

