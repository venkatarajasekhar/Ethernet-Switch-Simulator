/*
 * eth_switch2.c
 * Author:
 */

#include <stdio.h>
#include <stdlib.h>

#include "switch.h"

buffer buffer_create(void) {
	buffer new = (buffer) malloc(sizeof(struct buffer));
	new->next = NULL;
	return new;
}

buffer buffer_insert(buffer head, unsigned int frame_id) {
	buffer temp = head;
	buffer new = buffer_create();
	new->frame_id = frame_id;
	if(head == NULL) {
		head = new;
		return head;
	}
	while (head->next!=NULL) {
		head = head->next;
	}
	head->next = new;
	return temp;
}

/* This function initializes the state of the switch.
 */
void initialize_switch(switch_state *state) {
	state->p2.buffer = NULL;
	state->p2.current_size = 0;
}

/* This function is called every time a new frame is received. It
 * updates the table based on information obtained from the frame, and
 * adds the frame to the output buffer of the proper port(s).
 */
void forward_frame(switch_state *state, unsigned int port, unsigned int source_addr,
                   unsigned int dest_addr, unsigned int frame_id) {
	if(state->p2.current_size<BUFFER_SIZE) {
		state->p2.buffer = buffer_insert(state->p2.buffer, frame_id);
		state->p2.current_size += 1;
		printf("+");
		printf("%03u", frame_id);
		printf("\n");
	}
	else {
		printf("<");
		printf("%03u", frame_id);
		printf("\n");
	}
  // TODO Implement this function. You may use state->p2.value to
  // reference fields for part 2.
}

/* This function is called every time a timer tick is processed. Each
 * switch port is expected to output one frame per tick.
 */
void process_tick(switch_state *state) {
	if (state->p2.current_size<=0) {
		printf("e^^^");
		printf("\n");
	}
	else {
		printf("-");
		printf("%03u", state->p2.buffer->frame_id);
		printf("\n");
		if(state->p2.buffer->next == NULL) {
			state->p2.buffer = NULL;
			state->p2.current_size = 0;
			return;
		}
		state->p2.buffer = state->p2.buffer->next;
		state->p2.current_size -= 1;
	}
}

/* Prints the current state of the switch.
 */
void print_switch_state(switch_state *state) {
	printf("\n");
	if(state->p2.buffer!=NULL) {
		buffer cbuffer = state->p2.buffer;
		while (cbuffer->next!=NULL) {
			printf(" ");
			printf("%03u", cbuffer->frame_id);
			printf("\n");
			cbuffer = cbuffer->next;
		}
		printf(" ");
		printf("%03u", cbuffer->frame_id);
		printf("\n");
	}
	int size = state->p2.current_size;
	for(int i=0; i<BUFFER_SIZE-size; i++) {
		printf(" ---");
		printf("\n");
	}
  // TODO Implement this function. You may use state->p2.value to
  // reference fields for part 2.
}

void print_p3_titles(void){}

/* Destroys all memory dynamically allocated through this state (such
 * as the switch table) and frees all resources used by the switch.
 */
void destroy_switch(switch_state *state) {
	buffer cbuffer = state->p2.buffer;
	if(cbuffer==NULL) return;
	buffer temp = cbuffer;
	while (cbuffer->next!=NULL) {
		temp = cbuffer->next;
		free(cbuffer);
		cbuffer = temp;
    }
    free(cbuffer);
}
