/*
 * eth_switch1.c
 * Author: Yucheng Zhang
 */

#include <stdio.h>
#include <stdlib.h>

#include "switch.h"


switch_table* switch_table_create(void) {
	switch_table SwitchArr[SIZEOFSTRUCTSWITCHTABLE];
        switch_table *Switch_Ptr = (switch_table) malloc(sizeof(struct switch_table));
        if(Switch_Ptr){
	Switch_Ptr->next = NULL;
	}
	else{
	 Switch_Ptr = SwitchArr;
	 Switch_Ptr->next = NULL;
	}	
    return Switch_Ptr;
}


// Remove the node that has port and addr given
switch_table* switch_table_remove(switch_table *switch_head, unsigned int port, unsigned int addr) {
    switch_table *SwitchNode = switch_head; 
    switch_table *preNode = NULL;

    if(switch_head == NULL) 
	    return SwitchNode;
    while(switch_head != NULL){
	    preNode = switch_head;
	if((switch_head->addr == addr) && (switch_head->port == port)){
		//if List had one element
	  if(switch_head->next == NULL){
		    // If there is only one node in the list
		    switch_head = NULL;
		    return SwitchNode;
		}
		 //If there is more than one node 
		else{
		    preNode = switch_head;
		    switch_head = switch_head->next;
		    return SwitchNode;
		}
	    } 
	    //Else part of the Logic, Not able to find the elemnent from the List  
		else{
		if(switch_head->next != NULL){
		    preNode->next = switch_head->next;			
		}else{
		    // If it is the last element in the list
		    preNode->next = NULL;
		}
		return SwitchNode;
	    }
	}// Closed While
	preNode = switch_head; // Save for previous node
	switch_head = switch_head->next;
        return preNode;
}


switch_table switch_table_insert(switch_table *swhead, unsigned int port, unsigned int addr) {
    switch_table *SwitchTCreate = switch_table_create();
    switch_table temp = swhead;
    SwitchTCreate->port = port;
    SwitchTCreate->addr = addr;
    if(swhead==NULL) {
		swhead = SwitchTCreate;
		return head;
    }
	
	if(SwitchTCreate->addr < swhead->addr) {
		SwitchTCreate->next = swhead;
		return SwitchTCreate;
	}
	
	switch_table pre = swhead;
	switch_table nxt = swhead->next;
    while (nxt!=NULL) {
		if(new->addr>=pre->addr && SwitchTCreate->addr<=nxt->addr) {
			pre->next = SwitchTCreate;
			SwitchTCreate->next = nxt;
			return temp;
		}
		pre = pre->next;
		nxt = nxt->next;
    }
	pre->next = SwitchTCreate;
	SwitchTCreate->next = NULL;
    return temp;
}

forwarding_status forwarding_status_create(void) {
    forwarding_status *Fsnew = (forwarding_status) malloc(sizeof(struct forwarding_status));
	if(Fsnew)
    Fsnew->next = NULL;
    return Fsnew;
}

void forwarding_status_insert(switch_state *state, unsigned int dport, unsigned int sport, unsigned int frame_id) {
    forwarding_status *head = state->p1.status;
    forwarding_status *fstanew = forwarding_status_create();
    fstanew->frame_id = frame_id;
    fstanew->next = NULL;
    // We found a des port to send
    if(dport >= 0 && dport < NUM_PORTS){
	if(dport == sport){
	    for(int i=0; i< NUM_PORTS; i++){
		new->port_decs[i] = 0;
	    }
	}else{
	    for(int i=0; i < NUM_PORTS; i++){
		if( i == dport ){
		    new->port_decs[i] = 1;
		    *(state->p1.port_status + i) += 1;
		}else{
		    new->port_decs[i] = 0;
		}
	    }
	}
    }else if(dport == NUM_PORTS){
	for(int i=0; i < NUM_PORTS; i++){
	    if(i == sport){
		new->port_decs[i] = 0;
	    }else{
		new->port_decs[i] = 1;
		*(state->p1.port_status + i) += 1;
	    }

	}
    }

    forwarding_status temp = head;
    if(head==NULL){ state->p1.status = new;}
    else{
	while(head->next!=NULL){
	    head = head->next;
	}
	head->next = new;
	state->p1.status = temp;
    }
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

//return NUM_PORTS if dest_addr is not in switch table
//return port number if dest_addr is in switch table
unsigned int is_dest_addr_in_table(switch_state *state, unsigned int dest_addr) {
	if(dest_addr==65535) return NUM_PORTS;
    switch_table table = state->p1.table;
    if (table==NULL) return NUM_PORTS;
    if (table->addr==dest_addr) return table->port;
    while (table->next!=NULL) {
	if (table->next->addr==dest_addr) return table->next->port;
	table = table->next;
    }
    return NUM_PORTS;
}

void update_output_ports(switch_state *state, unsigned int port, unsigned int dest_addr, unsigned int frame_id) {
    unsigned int dport = is_dest_addr_in_table(state,dest_addr);
    forwarding_status_insert(state, dport, port, frame_id);
}

void update_switch_table(switch_state *state, unsigned int port, unsigned int source_addr) {
    unsigned int port_num_in_table = is_dest_addr_in_table(state, source_addr);
    if(port_num_in_table==NUM_PORTS) {
	state->p1.table = switch_table_insert(state->p1.table, port, source_addr);
    }
    else if (port_num_in_table==port) return;
    else {
	state->p1.table = switch_table_remove(state->p1.table, port_num_in_table, source_addr);
	state->p1.table = switch_table_insert(state->p1.table, port, source_addr);
    }
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

void print_p3_titles(void){}

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
