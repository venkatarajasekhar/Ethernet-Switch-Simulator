/*
 *  main.c
 *  Author: Jonatan Schroeder
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "switch.h"

/* Largest line in the input file. */
#define MAXLINE 1000

int main(int argc, char *argv[]) {
  
  FILE *input;
  char *filename;
  char line[MAXLINE];
  unsigned int port, source_addr, dest_addr, frame_id;
  switch_state state;
  
  /* The provided code includes support for an argument corresponding
     to the input file with the commands. Note that this functionality
     is not required in the specification, but can help you debug your
     own code.
   */
  if (argc == 1) {
    filename = "/dev/stdin";
  }
  else {
    filename = argv[1];
    if (!strcmp(filename, "-"))
      filename = "/dev/stdin";
  }
  
  input = fopen(filename, "r");
  if (!input) {
    perror("Could not open file for reading");
    return 2;
  }
  
  initialize_switch(&state);
  
  while(fgets(line, MAXLINE, input)) {
    
    if (toupper(line[0]) == 'P') {
      
      if (sscanf(line, "P%u%x%x%u",
                 &port, &dest_addr, &source_addr, &frame_id) < 4)
        fprintf(stderr, "Invalid frame input: %s", line);
      else
        forward_frame(&state, port, source_addr, dest_addr, frame_id);
    }
    else if (!strncasecmp(line, "TICK", 4)) {
      
      // No parameters
      process_tick(&state);
    }
    else {
      
      fprintf(stderr, "Invalid input line: %s\n", line);
    }
  }
  
  if (ferror(input)) {
    perror("Could not read from input file");
    destroy_switch(&state);
    fclose(input);
    return 3;
  }

  print_switch_state(&state);
  destroy_switch(&state);
  
  fclose(input);
  
  return EXIT_SUCCESS;
}
