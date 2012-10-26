/*
 * eth_switch1.c
 * Author: Yucheng Zhang
 */

#include <stdio.h>
#include <stdlib.h>

#include "switch.h"


switch_table switch_table_create(void) {
	 switch_table new = (switch_table) malloc(sizeof(struct switch_table));
	 new->next = NULL;
	 return new;
}

switch_table switch_table_insert(switch_table head) {
	switch_table new = switch_table_create();
	if(head==NULL) {
		head = new;
		return new;
	}
	while (head->next!=NULL) {
		head = head->next;
	}
	head->next = new;
	return new;
}

forwarding_status forwarding_status_create(void) {
	forwarding_status new = (forwarding_status) malloc(sizeof(struct forwarding_status));
	new->next = NULL;
	return new;
}

forwarding_status forwarding_status_insert(forwarding_status head) {
	forwarding_status new = forwarding_status_create();
	while (head->next!=NULL) {
		head = head->next;
	}
	head->next = new;
	return new;
}

int * initialize_ports_status(void) {
	int *ports_status = (int *) malloc(NUM_PORTS*sizeof(int));
	return ports_status;
}

/* This function initializes the state of the switch.
 */
void initialize_switch(switch_state *state) {
	//state = (switch_state *) malloc(sizeof(union switch_state));
	state->p1.table = NULL;
	state->p1.status = NULL;
	
	state->p1.port_status = initialize_ports_status();
	
	for (int i=0; i<NUM_PORTS; i++) {
		state->p1.port_status[i] = 0;
	}
}

void update_output_ports(switch_state *state, unsigned int port, unsigned int dest_addr, unsigned int frame_id) {
	switch_table table = state->p1.table; 
	unsigned int port_num = NUM_PORTS;
	if (table==NULL) return;
	while (table->next!=NULL) {
		if(table->addr==dest_addr) port_num=table->port;
		table = table->next;
	}
	if(table->addr==dest_addr) port_num=table->port;
	
	forwarding_status new = forwarding_status_create();
	new->frame_id = frame_id;
	new->next = NULL;
	if(port==port_num) {
		for(int i=0; i<NUM_PORTS; i++) {
			new->port_decs[i] = 0;
		}
	}
	else if (port_num==NUM_PORTS) {
		for(int i=0; i<NUM_PORTS; i++) {
			if(i==port) new->port_decs[i] = 0;
			else {
				new->port_decs[i] = 1;
				int * buffers = state->p1.port_status;
				*(buffers+i) = *(buffers+i)+1;
			}
		}
	}
	else {
		for(int i=0; i<NUM_PORTS; i++) {
			if(i==port_num) {
				new->port_decs[i] = 1;
				int * buffers = state->p1.port_status;
				*(buffers+i) = *(buffers+i)+1;
			}
			else new->port_decs[i] = 0;
		}
	}
		
	forwarding_status status = state->p1.status;
	forwarding_status temp = status;
	if(status==NULL) {
		state->p1.status = new;
		return;
	}
	while(status->next!=NULL) {
		status = status->next;
	}
	status->next = new;
	state->p1.status = temp;
	
	
	
	
}

void update_switch_table(switch_state *state, unsigned int port, unsigned int source_addr) {
	switch_table table = state->p1.table;
	if (table==NULL) {
		table = switch_table_create();
		table->port = port;
		table->addr = source_addr;
		table->next = NULL;
		state->p1.table = table;
		return;
	}
	switch_table temp = table;
	while (table->next!=NULL) {
		switch_table pre = table;
		table = table->next;
		if (table->addr==source_addr) {
			if (table->port == port) return;
			else {
				if(table->next==NULL) pre->next = NULL;
				else pre->next = table->next;
			}
		}
	}
	table->next = switch_table_create();
	table->next->port = port;
	table->next->addr = source_addr;
	table->next->next = NULL;
	state->p1.table = temp;
}

/* This function is called every time a new frame is received. It
 * updates the table based on information obtained from the frame, and
 * adds the frame to the output buffer of the proper port(s).
 */
void forward_frame(switch_state *state, unsigned int port, unsigned int source_addr,
                   unsigned int dest_addr, unsigned int frame_id) {
	update_switch_table(state, port, source_addr);
	update_output_ports(state, port, dest_addr, frame_id);
}

/* This function is called every time a timer tick is processed. Each
 * switch port is expected to output one frame per tick.
 */
void process_tick(switch_state *state) {
	
	// For part 1 this function has no processing.
	return;
}

/* Prints the current state of the switch.
 */
void print_switch_state(switch_state *state) {
	print_ports_status_title();
	print_forwarding_status(state->p1.status);
	printf("\n");
	print_ports_status(state);
	printf("\n");
	print_switch_table(state->p1.table);
	printf("\n");
}

void print_ports_status_title() {
	for (int i=0; i<NUM_PORTS; i++) {
		printf("P%d  ", i);
	}
	printf("FRAME_ID");
	printf("\n");
}

void print_forwarding_status(forwarding_status head) {
	if(head==NULL) return;
	while (head!=NULL) {
		for (int i=0; i<NUM_PORTS; i++) {
			if(head->port_decs[i] == 0) printf(" ^  ");
			else printf(" +  ");
		}
		printf("%03u", head->frame_id);
		printf("\n");
		head = head->next;
	}
}

void print_ports_status(switch_state *state) {
	int *ports_status = state->p1.port_status;
	for (int i=0; i<NUM_PORTS; i++) {
		printf(" %d  ", *(ports_status+i));
	}
	printf("\n");
}

void print_switch_table(switch_table head) {
	if(head==NULL) return;
	while (head!=NULL) {
		printf("%04x", head->addr);
		printf(" ");
		printf("P%u", head->port);
		printf("\n");
		head = head->next;
	}
}

/* Destroys all memory dynamically allocated through this state (such
 * as the switch table) and frees all resources used by the switch.
 */
void destroy_switch(switch_state *state) {
	switch_table table = state->p1.table;
	destroy_switch_table(table);
	forwarding_status status = state->p1.status;
	destroy_forwarding_status(status);
	free(state->p1.port_status);
	
	//free(state);
}

void destroy_switch_table(switch_table table) {
	if(table==NULL) return;
	switch_table temp = table;
	while (table->next!=NULL) {
		temp = table->next;
		free(table);
		table = temp;
	}
	free(table);
}

void destroy_forwarding_status(forwarding_status status) {
	if(status==NULL) return;
	forwarding_status temp = status;
	while (status->next!=NULL) {
		temp = status->next;
		free(status);
		status = temp;
	}
	free(status);
}