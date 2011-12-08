#pragma once

/*
    struct for representing locations in the grid.
*/
class Location
{
public:
    int row, col;

    Location()
    {
        row = col = 0;
    };

	Location(const Location & original)
	{
		col = original.col;
		row = original.row;
	};

    Location(int r, int c)
    {
        row = r;
        col = c;
    };

	void operator = (const Location & original)
	{
		col = original.col;
		row = original.row;
	}

	bool operator == (Location & rhs) const
	{
		if ((col == rhs.col) && (row == rhs.row))
		{
			return true;
		}
		return false;
	};

	bool operator != (Location & rhs)
	{
		if ((col == rhs.col) && (row == rhs.row))
		{
			return false;
		}
		return true;
	};

	bool operator < (const Location & rhs) const
	{
		if (row < rhs.row)
		{
			return true;
		}
		else if ((row == rhs.row) && (col < rhs.col))
		{
			return true;
		}
		else
		{
			return false;
		}
	};
};

