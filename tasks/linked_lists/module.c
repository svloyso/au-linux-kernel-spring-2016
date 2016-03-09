#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>

#include "stack.h"
#include "assert.h"

static void __init test_stack(void)
{
    LIST_HEAD(data_stack);
    stack_entry_t *tos = NULL;
    const char *tos_data = NULL;
    const char* test_data[] = { "1", "2", "3", "4" };
    long i = 0;

    pr_alert("Testing basic stack");

    for (i = 0; i != ARRAY_SIZE(test_data); ++i) {
        stack_push(&data_stack,
            create_stack_entry((void*)test_data[i])
        );
    }

    for (i = ARRAY_SIZE(test_data) - 1; i >= 0; --i) {
        tos = stack_pop(&data_stack);
        tos_data = STACK_ENTRY_DATA(tos, const char*);
        printk(KERN_ALERT "%s == %s\n", tos_data, test_data[i]);
        delete_stack_entry(tos);
        assert(!strcmp(tos_data, test_data[i]));
    }

    assert(stack_empty(&data_stack));
}

static void __init print_processes_backwards(void)
{
    LIST_HEAD(proc_stack);
    stack_entry_t *ent = NULL;
    char* proc_name = NULL;
    struct task_struct *task;
    for_each_process(task) {
        proc_name = kmalloc(255, GFP_KERNEL);
        if(!proc_name) {
            break;
        }
        get_task_comm(proc_name, task);
        ent = create_stack_entry((void*)proc_name);
        if(!ent) {
            break;
        }
        stack_push(&proc_stack, ent);
    }
    while(!list_empty(&proc_stack)) {
        ent = stack_pop(&proc_stack);
        proc_name = STACK_ENTRY_DATA(ent, char*);
        pr_alert("%s\n", proc_name);
        kfree(proc_name);
        delete_stack_entry(ent);
    }
}

static int __init ll_init(void)
{
    printk(KERN_ALERT "Hello, linked_lists\n");
    test_stack();
    print_processes_backwards();
    return 0;
}

static void __exit ll_exit(void)
{
    printk(KERN_ALERT "Goodbye, linked_lists!\n");
}

module_init(ll_init);
module_exit(ll_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Linked list exercise module");
MODULE_AUTHOR("Kernel hacker!");
