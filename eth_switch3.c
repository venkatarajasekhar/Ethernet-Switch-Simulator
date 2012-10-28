/*
 * eth_switch3.c
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

switch_table switch_table_create(void) {
    switch_table new = (switch_table) malloc(sizeof(struct switch_table));
    new->next = NULL;
    return new;
}


// Remove the node that has port and addr given
switch_table switch_table_remove(switch_table head, unsigned int port, unsigned int addr) {
    // If it is an empty list
    if(head == NULL) return head;
    // temp is the real head
    switch_table realHead = head; 
    switch_table preNode = NULL;
	
    while(head != NULL){
		if(head->addr == addr && head->port == port){
			// Remove the node
			if(preNode == NULL){
				// If this is the first element(real head)
				if(head->next == NULL){
					// If there is only one head in the list
					head = NULL;
					return head;
				}else{
					head = head->next;
					return head;
				}
			}else{
				if(head->next != NULL){
					preNode->next = head->next;
				}else{
					// If it is the last element in the list
					preNode->next = NULL;
				}
				return realHead;
			}
		}
		preNode = head; // Save for previous node
		head = head->next;
    }
    return realHead;
}


switch_table switch_table_insert(switch_table head, unsigned int port, unsigned int addr) {
    switch_table new = switch_table_create();
    switch_table temp = head;
    new->port = port;
    new->addr = addr;
    if(head==NULL) {
		head = new;
		return head;
    }
	
	if(new->addr < head->addr) {
		new->next = head;
		return new;
	}
	
	switch_table pre = head;
	switch_table nxt = head->next;
    while (nxt!=NULL) {
		if(new->addr>=pre->addr && new->addr<=nxt->addr) {
			pre->next = new;
			new->next = nxt;
			return temp;
		}
		pre = pre->next;
		nxt = nxt->next;
    }
	pre->next = new;
	new->next = NULL;
    return temp;
}

unsigned int is_dest_addr_in_table(switch_state *state, unsigned int dest_addr) {
	if(dest_addr==65535) return NUM_PORTS;
    switch_table table = state->p3.table;
    if (table==NULL) return NUM_PORTS;
    if (table->addr==dest_addr) return table->port;
    while (table->next!=NULL) {
		if (table->next->addr==dest_addr) return table->next->port;
		table = table->next;
    }
    return NUM_PORTS;
}

void update_switch_table(switch_state *state, unsigned int port, unsigned int source_addr) {
    unsigned int port_num_in_table = is_dest_addr_in_table(state, source_addr);
    if(port_num_in_table==NUM_PORTS) {
		state->p3.table = switch_table_insert(state->p3.table, port, source_addr);
    }
    else if (port_num_in_table==port) return;
    else {
		state->p3.table = switch_table_remove(state->p3.table, port_num_in_table, source_addr);
		state->p3.table = switch_table_insert(state->p3.table, port, source_addr);
    }
}

/* This function initializes the state of the switch.
 */
void initialize_switch(switch_state *state) {
	state->p3.table = NULL;
	for (int i=0; i<NUM_PORTS; i++) {
		state->p3.buffers[i] = NULL;
		state->p3.ports_size[i] = 0;
	}
}

/* This function is called every time a new frame is received. It
 * updates the table based on information obtained from the frame, and
 * adds the frame to the output buffer of the proper port(s).
 */
void forward_frame(switch_state *state, unsigned int port, unsigned int source_addr,
                   unsigned int dest_addr, unsigned int frame_id) {
    update_switch_table(state, port, source_addr);
  
	unsigned int port_num = is_dest_addr_in_table(state, dest_addr);
	//not found
	if(port_num==NUM_PORTS) {
		for(int i=0; i<NUM_PORTS; i++) {
			if(i!=0) printf(" ");
			if(i==port) {
				printf("^^^^");
			}
			else {
				if(state->p3.ports_size[i]>=BUFFER_SIZE) {
					printf("<%03u", frame_id);
				}
				else {
					state->p3.buffers[i] = buffer_insert(state->p3.buffers[i], frame_id);
					state->p3.ports_size[i] += 1;
					printf("+%03u", frame_id);
				}
			}
		}
	} 
	//found
	else {
		//drop
		if(port_num==port) {
			for(int j=0; j<NUM_PORTS; j++) {
				if(j!=0) printf(" ");
				printf("^^^^");
			}
		} 
		//send port
		else {
			for(int k=0; k<NUM_PORTS; k++) {
				if(k!=0) printf(" ");
				if(k!=port_num) {
					printf("^^^^");
				}
				else {
					if(state->p3.ports_size[k]>=BUFFER_SIZE) {
						printf("<%03u", frame_id);
					}
					else {
						state->p3.buffers[k] = buffer_insert(state->p3.buffers[k], frame_id);
						state->p3.ports_size[k] += 1;
						printf("+%03u", frame_id);
					}
				}
			}
		}
	}
	printf("\n");
}

/* This function is called every time a timer tick is processed. Each
 * switch port is expected to output one frame per tick.
 */
void process_tick(switch_state *state) {
	for (int i=0; i<NUM_PORTS; i++) {
		if(i!=0) printf(" ");
		if(state->p3.ports_size[i]<=0)	printf("e^^^");
		else {
			printf("-%03u", state->p3.buffers[i]->frame_id);
			if(state->p3.buffers[i]->next == NULL) {
				state->p3.buffers[i] = NULL;
				state->p3.ports_size[i]=0;
			}
			else {
				state->p3.buffers[i] = state->p3.buffers[i]->next;
				state->p3.ports_size[i] -= 1;
			}
		}
	}
	printf("\n");
}

void print_p3_titles(void) {
	for (unsigned int i=0; i<NUM_PORTS; i++) {
		if(i!=0) printf(" ");
		printf("  P%u", i);
	}
	printf("\n");
}

/* Prints the current state of the switch.
 */
void print_switch_state(switch_state *state) {
	printf("\n");
	print_bufferstatue(state);
	printf("\n");
	switch_table table = state->p3.table;
	print_switch_table(table);
	printf("\n");
}

void print_bufferstatue(switch_state *state) {
	buffer cbuffers[NUM_PORTS];
	for(int k=0; k<NUM_PORTS; k++) {
		cbuffers[k] = state->p3.buffers[k];
	}
	
	for(int i=0; i<BUFFER_SIZE; i++) {
		for(int j=0; j<NUM_PORTS; j++) {
			buffer cbuffer = cbuffers[j];
			if(cbuffer == NULL) {
				if(j!=0) {
					printf(" ");
				}
				printf(" ");
				printf("---");
			}
			else {
				if(j!=0) {
					printf(" ");
				}
				printf(" ");
				printf("%03u", cbuffer->frame_id);
				cbuffer = cbuffer->next;
				cbuffers[j] = cbuffer;
			}
		}
		printf("\n");
	}
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
	switch_table table = state->p3.table;
    destroy_switch_table(table);
	for (int i=0; i<NUM_PORTS; i++) {
		buffer cbuffer = state->p3.buffers[i];
		destroy_buffer(cbuffer);
	}
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

void destroy_buffer(buffer cbuffer) {
	if(cbuffer==NULL) return;
	buffer temp = cbuffer;
	while(cbuffer->next!=NULL) {
		temp = cbuffer->next;
		free(cbuffer);
		cbuffer = temp;
	}
	free(cbuffer);
}