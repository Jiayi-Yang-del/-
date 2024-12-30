/********************************************************************
filename: util.cpp
author: onesuper
email: onesuperclark@gmail.com

some helper, like loggers

*********************************************************************/
#include <stdio.h>
#include "graph.h"
/*
 * NOTES: gen_level_log() must be called before
 * gen_test_log() because the num_of_levels can only be caught
 * after calling gen_level_log
 */
void gen_level_log(void)
{
	 FILE* fp = fopen("./level.log", "w");
	 if (!fp) {
		  printf("log file cannot be opened.\n");
		  return;
	 }

	 for (int i=1; i<MAX_LEVEL; i++) {
		  if(counter[i] == 0) {
			   num_of_levels = i - 1; 
			   printf("The num of levels is %d\n", num_of_levels);
			   break;
		  }
		  fprintf(fp, "%d\n", counter[i]);
	 }

	 fclose(fp);
	 return;
}


void gen_test_log(float time_used, char* graph_name, char* graph_type) 
{
	 FILE* fp = fopen("./../test.log", "a");
	 if (!fp) {
		  printf("Cannot open the test.log.\n");
		  return;
	 }
	 
	 fprintf(fp, "%s\t%s\t%u\t%u\t%u\t%f\n", graph_type, graph_name,
			 num_of_nodes, num_of_edges, num_of_levels, time_used);
	 
	 fclose(fp);
	 return;
}

void gen_test_log_cpu(float time_used, char* graph_name,
					  char graph_type, int num_of_threads) 
{
	 FILE* fp = fopen("./../../test.log", "a");
	 if (!fp) {
		  printf("Cannot open the test.log.\n");
		  return;
	 }
	 
	 fprintf(fp, "%s\t%s\t%u\t%u\t%u\t%f\t%d\n", graph_type, graph_name,
			 num_of_nodes, num_of_edges, time_used, num_of_levels, num_of_threads);
	 
	 fclose(fp);
	 return;
}


void calculate_counter()
{
     
	 for (int i=0; i<num_of_nodes; i++) {
		  counter[cost[i]] ++;
          //printf("%d\t%d\n", i, cost[i]);
	 }
     
	 return;
}
