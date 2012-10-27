/*
 *  switch.h
 *  Author: Jonatan Schroeder
 */

#ifndef _SWITCH_H_
#define _SWITCH_H_

#include "capacity.h"

typedef struct switch_table {
	unsigned int port;
	unsigned int addr;
	struct switch_table *next;
} *switch_table;

typedef struct forwarding_status {
	int port_decs[NUM_PORTS];
	unsigned int frame_id;
	struct forwarding_status *next;
} *forwarding_status;

typedef struct switch_state_p1 {
  // TODO Add the members needed for part 1 here
	switch_table table;
	forwarding_status status;
	//int port_status[NUM_PORTS];
	int *port_status;
} switch_state_p1;

typedef struct switch_state_p2 {
  
  // TODO Add the members needed for part 2 here
  
  int remove_me; // Field added so code compiles, may be removed.
} switch_state_p2;

typedef struct switch_state_p3 {
  
  // TODO Add the members needed for part 3 here
  
  int remove_me; // Field added so code compiles, may be removed.
} switch_state_p3;

typedef union switch_state {
  switch_state_p1 p1;
  switch_state_p2 p2;
  switch_state_p3 p3;
} switch_state;


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
//unsigned int get_output_port(switch_table table, unsigned int dest_addr);
int * initialize_ports_status(void);
void update_output_ports(switch_state *state, unsigned int port, unsigned int dest_addr, unsigned int frame_id);
switch_table switch_table_remove(switch_table head, unsigned int port, unsigned int addr);
unsigned int is_dest_addr_in_table(switch_state *state, unsigned int dest_addr);



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
