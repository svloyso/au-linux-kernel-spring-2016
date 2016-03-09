#include "stack.h"
#include <linux/slab.h>

stack_entry_t* create_stack_entry(void *data)
{
    stack_entry_t* ent = kmalloc(sizeof(stack_entry_t), GFP_KERNEL);
    if(ent == NULL) {
        goto out;
    }
    ent->data = data;
    INIT_LIST_HEAD(&ent->lh);
out:
    return ent;
}

void delete_stack_entry(stack_entry_t *entry)
{
    kfree(entry);
}

void stack_push(struct list_head *stack, stack_entry_t *entry)
{
    list_add(&entry->lh, stack);
}

stack_entry_t* stack_pop(struct list_head *stack)
{
    stack_entry_t* entry = NULL;
    if(list_empty(stack)) {
        goto out;
    }
    entry = list_first_entry(stack, stack_entry_t, lh);
    list_del(stack->next);
out:
    return entry;
}

