// SPDX-License-Identifier: GPL-2.0-only
/*  
 *  inode.c - A simple filesystem.
 */
#include    <linux/init.h>      /* Needed for __init/__exit */
#include    <linux/module.h>    /* Needed for module_init/module_exit/MODULE_* */

static int __init mc504fs_init(void)
{
    pr_info("Hello world\n");

    return 0; // A non 0 return value means init_module failed; module can't be loaded.
}

static void __exit mc504fs_exit(void)
{
    pr_info("Goodbye world\n");
}

module_init(mc504fs_init);
module_exit(mc504fs_exit);

MODULE_AUTHOR("LKCAMP <gbittencourt@lkcamp.dev>");
MODULE_DESCRIPTION("A simple filesystem");
MODULE_LICENSE("GPL");
