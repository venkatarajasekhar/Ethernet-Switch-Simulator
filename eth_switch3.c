/*
 * eth_switch3.c
 * Author:
 */

#include <stdio.h>
#include <stdlib.h>

#include "switch.h"

/* This function initializes the state of the switch.
 */
void initialize_switch(switch_state *state) {
  
  // TODO Implement this function. You may use state->p3.value to
  // reference fields for part 3.
}

/* This function is called every time a new frame is received. It
 * updates the table based on information obtained from the frame, and
 * adds the frame to the output buffer of the proper port(s).
 */
void forward_frame(switch_state *state, unsigned int port, unsigned int source_addr,
                   unsigned int dest_addr, unsigned int frame_id) {

  // TODO Implement this function. You may use state->p3.value to
  // reference fields for part 3.
}

/* This function is called every time a timer tick is processed. Each
 * switch port is expected to output one frame per tick.
 */
void process_tick(switch_state *state) {

  // TODO Implement this function. You may use state->p3.value to
  // reference fields for part 3.
}

/* Prints the current state of the switch.
 */
void print_switch_state(switch_state *state) {

  // TODO Implement this function. You may use state->p3.value to
  // reference fields for part 3.
}

/* Destroys all memory dynamically allocated through this state (such
 * as the switch table) and frees all resources used by the switch.
 */
void destroy_switch(switch_state *state) {

  // TODO Implement this function. You may use state->p3.value to
  // reference fields for part 3.
}
