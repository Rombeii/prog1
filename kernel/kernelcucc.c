#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/fdtable.h>

MODULE_DESCRIPTION ("Ha tudnám mi ez, ide írnám");
MODULE_AUTHOR ("Bátfai Norbert (nbatfai@gmail.com)");
MODULE_LICENSE ("GPL");

static int
elso_init_module (void)
{
  struct task_struct *task;
  struct list_head *p;
  int i = 0;
  
  printk (KERN_NOTICE "eleje\n");
  
  list_for_each (p, current->tasks.next)
    {
      task = list_entry (p, struct task_struct, tasks);
      i = (task->files)->next_fd;
      printk (KERN_NOTICE "%s %i %i\n", task->comm,
	      task->pid, i);
    }
  return 0;
}

static void
elso_exit_module (void)
{
  printk (KERN_NOTICE "vége\n");
}

module_init (elso_init_module);
module_exit (elso_exit_module);