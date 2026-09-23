#include <math.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"

void TableWeightSet(struct table_weight* table, int val, bool on_off)
{

}

void TableWeightEnableAll(struct table_weight* table)
{
	for (int i = 0; i < MAX_WEIGHTS_IN_TABLE; i++)
	{
		if (table[i].weight < 0)
			break;
		table[i].on_off = true;
	}
}

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