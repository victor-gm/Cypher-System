/************************ DYNAMIC AND AGNOSTIC POILIST ************************
 *                                                                            *
 * Author: Víctor Garrido Martínez                                            *
 *                                                                            *
*******************************************************************************/
//Needs to update when it goes to head to go over the ghost
#ifndef _POILIST_H
#define _POILIST_H

#include "lib.h"
#include "utils/general_utils.h"

/******************************************* TYPES *******************************************/

typedef struct node
{
    void* e;
    struct node* next;
}Node;

typedef struct poiList
{
    Node* head;
    Node* prev;
}POIList;

/******************************* SPECIAL VARIABLES FOR THIS CASE ******************************/
/* Note: I don't want to use global variables but I'm forced since I can't pass values to signals
   Even if it looks redudant, I'll still use the module using references and only reference i a 
   global way when it's strictly necessary
*/
POIList client; // Forced to use global variables for the signal situation, not something I like
POIList server; // Forced to use global variables for the signal situation, not something I like
//POIList thread;
/******************************************* FUNCTIONS *******************************************/

POIList POIList_create();
int POIList_insert (POIList *p, void* e, int size);
void* POIList_consult (POIList *p);
void POIList_delete (POIList *p);
void POIList_forward (POIList *p);
void POIList_head (POIList *p);
int POIList_tail (POIList *p);
int POIList_empty(POIList *p);
void POIList_destroy (POIList *p);


#define LIST_CREATE_ERROR "Error creating list for connections\n"
#define LIST_INSERT_ERROR "Error inserting list for connections\n"
#define LIST_DELETE_ERROR "Error deeleting element from list for connections\n"
#define LIST_FORWARD_ERROR "Error movig forward list for connections\n"
#define LIST_CONSULT_ERROR "Error consulting list. Null element\n"

#endif