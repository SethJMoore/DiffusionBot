#pragma once

#include "ant.h"

class GuardAnt :
	public Ant
{
public:
	GuardAnt(void);
	GuardAnt(Location loc, State & state) : Ant(loc, state) {};
	~GuardAnt(void);
	void chooseMove(State & state);
	virtual int calculateValue(Location & loc, State & state);
};

