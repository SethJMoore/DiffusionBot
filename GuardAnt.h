#pragma once

#include "ant.h"

class GuardAnt :
	public Ant
{
public:
	GuardAnt(void);
	~GuardAnt(void);
	void chooseMove(State & state);
	virtual int calculateValue(Location & loc, State & state);
};

