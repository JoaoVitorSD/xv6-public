#include "list.h"
#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
struct listproc createListProc()
{
    struct listproc list;
    list.head = 0;
    list.tail = 0;
    list.size = 0;
    return list;
}

void insertProc(struct listproc *list, struct proc *proc)
{
    procnode node;
    node.proc = proc;
    node.next = 0;
    if (list->size == 0)
    {
        *list->head = node;
        *list->tail = node;
    }
    else
    {
        *list->tail->next = node;
        *list->tail = node;
    }
    list->size++;
    cprintf("insert in list proc%d\n", list->size);
}

void removeProc(struct listproc *list, struct proc *proc)
{
    struct procnode *aux = list->head;

    if (aux->proc == proc)
    {
        list->head = aux->next;
        list->size--;
        return;
    }
    
    struct procnode *auxPrev = list->head;
    for (int i = 1; i < list->size; i++)
    {
        aux = aux->next;
        if (aux->proc == proc)
        {
            auxPrev->next = aux->next;
            // free(aux);
            list->size--;
            return;
        }
        auxPrev = aux;
    }   
}