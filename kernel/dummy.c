#include <linux/pid.h>
// #include <linux/sched/signal.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE0(dummy)
{
	pr_err("test\n");
	return task_tgid_vnr(current);
}
