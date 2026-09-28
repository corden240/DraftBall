#include <math.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"

/*
* NAME:        TableWeightTotalWeight
* DESCRIPTION: return total weight of enabled values
* INPUTS:      table
* OUTPUTS:     None
*/
int TableWeightTotalWeight(struct table_weight* table)
{
	int total_weight = 0;
	for (int i = 0; i < MAX_WEIGHTS_IN_TABLE; i++)
	{
		if (table[i].weight < 0)
			break;
		if (table[i].on_off)
			total_weight += table[i].weight;
	}
	
	return total_weight;
}

/*
* NAME:        TableWeightSet
* DESCRIPTION: set a value in a table to true or false
* INPUTS:      table, val, bool
* OUTPUTS:     None
*/
void TableWeightSet(struct table_weight* table, int val, bool on_off)
{
	for (int i = 0; i < MAX_WEIGHTS_IN_TABLE; i++)
	{
		if (table[i].weight < 0)
			break;
		if (table[i].val == val)
			table[i].on_off = on_off;
	}
}

/*
* NAME:        TableWeightEnableAll
* DESCRIPTION: sets all values in a table to true
* INPUTS:      table
* OUTPUTS:     None
*/
void TableWeightEnableAll(struct table_weight* table)
{
	for (int i = 0; i < MAX_WEIGHTS_IN_TABLE; i++)
	{
		if (table[i].weight < 0)
			break;
		table[i].on_off = true;
	}
}

/*
* NAME:        TableWeightDecision
* DESCRIPTION: makes a table weight decision, can enable all
* INPUTS:      table
* OUTPUTS:     None
*/
int TableWeightDecision(struct table_weight* table, bool enable_all)
{
	if (enable_all)
		TableWeightEnableAll(table);
	int total_weight = 0;
	for (int i = 0; i < MAX_WEIGHTS_IN_TABLE; i++)
	{
		if (table[i].weight < 0)
			break;
		if (table[i].on_off)
			total_weight += table[i].weight;
	}
	int random_value = rand() % total_weight;
	int cumulative_weight = 0;
	for (int i = 0; i < MAX_WEIGHTS_IN_TABLE; i++)
	{
		if (table[i].weight < 0)
			break;
		if (table[i].on_off)
			cumulative_weight += table[i].weight;
		if (random_value < cumulative_weight)
			return table[i].val;
	}
	return -1; // should never reach here if the table is valid
}