#include "poi_list.h"

POIList POIList_create()
{
    POIList p;
    p.head = (Node *)malloc(sizeof(Node));

    if(p.head == NULL)
    {
        //write(1,LIST_CREATE_ERROR,strlen(LIST_CREATE_ERROR));
        writeToScreen(LIST_CREATE_ERROR);
    }
    else
    {
        p.prev = p.head;
        p.head->next = NULL;
        p.head->e = NULL;
    }
    return p;
}

int POIList_insert (POIList *p, void* e, int size)
{
   	Node *aux;
	aux = (Node *)malloc(sizeof(Node));
	if(aux == NULL)
    {
        //write(1,LIST_INSERT_ERROR,strlen(LIST_INSERT_ERROR));
        writeToScreen(LIST_CREATE_ERROR);

        return 0;
	}
	else
    {
        aux->e = malloc(size);
        memcpy(aux->e, e, size); // Revisar hacerlo así. Puede ser más costoso. igual ya pasarle la memoria creada
        aux->next = p->prev->next; 
        p->prev->next = aux; 
        p->prev = aux;
	}
    return 1;
}

void* POIList_consult (POIList *p) 
{
    void* e = NULL;

    if(p->prev->next != NULL)
    {
        e = p->prev->next->e;
    }
    else
    {
        //write(1,LIST_CONSULT_ERROR,strlen(LIST_CONSULT_ERROR));
        writeToScreen(LIST_CONSULT_ERROR);
    }

    return e;
}

void POIList_delete (POIList *p)
{
    Node *aux;
	
    if(p->prev->next==NULL)
    {
        //write(1,LIST_DELETE_ERROR,strlen(LIST_DELETE_ERROR));
        free(p->head->e); //Think it's not necessary cause there's not malloc done there
        p->head->e = NULL;

        free(p->head);
        p->head = NULL;
	}
	else
    {
		aux = p->prev->next;
		p->prev->next = aux->next;
        free(aux->e);
		free(aux);
	}
}

void POIList_forward (POIList *p)
{
    if(p->prev->next == NULL)
    {
        //write(1,LIST_FORWARD_ERROR,strlen(LIST_FORWARD_ERROR));
        writeToScreen(LIST_FORWARD_ERROR);
    }
    else
    {
        p->prev=p->prev->next;
    }
}

void POIList_head (POIList *p)
{
    p->prev = p->head;
}

int POIList_tail (POIList *p)
{
    return p->prev->next == NULL;
}

int POIList_empty(POIList *p)
{
    return p->head->next == NULL;
}

void POIList_destroy (POIList *p)
{
    POIList_head(p);
    while(p->head != NULL)
    {
        POIList_delete(p);
    }
    p->prev = NULL;
}




