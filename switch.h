/*
 *  switch.h
 *  Author: Jonatan Schroeder
 */

#ifndef _SWITCH_H_
#define _SWITCH_H_

#include "capacity.h"
#define sizeof(struct switch_table) SIZEOFSTRUCTSWITCHTABLE;


typedef struct switch_table_S {
	unsigned int port;
	unsigned int addr;
	struct switch_table *next;
}switch_table;

typedef struct forwarding_status_S {
	int port_decs[NUM_PORTS];
	unsigned int frame_id;
	struct forwarding_status *next;
}forwarding_status;

typedef struct switch_state_p1_S {
	switch_table table;
	forwarding_status status;
	int *port_status;
}switch_state_p1;

typedef struct buffer_S {
	unsigned int frame_id;
	struct buffer *next;
}buffer;

typedef struct switch_state_p2_S {
	int current_size;
	buffer buffer;
} switch_state_p2;

typedef struct switch_state_p3_S {
	buffer buffers[NUM_PORTS];
	int ports_size[NUM_PORTS];
	switch_table table;
} switch_state_p3;

typedef union switch_state_S {
  switch_state_p1 p1;
  switch_state_p2 p2;
  switch_state_p3 p3;
} switch_state;


//functions for part1
switch_table switch_table_create(void);
switch_table switch_table_insert(switch_table head, unsigned int port, unsigned int addr);
forwarding_status forwarding_status_create(void);
void forwarding_status_insert(switch_state *state,unsigned int dport, unsigned int sport,unsigned int frame_id);
void destroy_switch_table(switch_table table);
void destroy_forwarding_status(forwarding_status status);
void print_ports_status_title();
void print_forwarding_status(forwarding_status head);
void print_ports_status(switch_state *state);
void print_switch_table(switch_table head);
void update_switch_table(switch_state *state, unsigned int port, unsigned int source_addr);
int * initialize_ports_status(void);
void update_output_ports(switch_state *state, unsigned int port, unsigned int dest_addr, unsigned int frame_id);
switch_table switch_table_remove(switch_table head, unsigned int port, unsigned int addr);
unsigned int is_dest_addr_in_table(switch_state *state, unsigned int dest_addr);

//functions for part2
buffer buffer_create(void);
buffer buffer_insert(buffer head, unsigned int frame_id);

//functions for part3
void destroy_buffer(buffer cbuffer);
void print_bufferstatue(switch_state *state);
void print_p3_titles(void);


void initialize_switch(switch_state *state);
void forward_frame(switch_state *state, unsigned int port,
                   unsigned int source_addr, unsigned int dest_addr,
                   unsigned int frame_id);
void process_tick(switch_state *state);
void print_switch_state(switch_state *state);
void destroy_switch(switch_state *state);

#define ADDRESS_FORMAT "%04x"
#define FRAME_ID_FORMAT "%03u"
#define PORT_FORMAT "P%u"

#endif
