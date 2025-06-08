// SPDX-License-Identifier: GPL-2.0-only
/*  
 *  inode.c - A simple filesystem.
 */
#include    <linux/init.h>      /* Needed for __init/__exit */
#include    <linux/module.h>    /* Needed for module_init/module_exit/MODULE_* */
#include    <linux/fs.h>	/* Needed for filesystem operations */

#define MC504FS_MAGIC 0x4D43353034

// lessons learned:
// - mounting will not work if we don't assign i_op and i_fop to the inodes
//   (maybe only the root inode?) (why?)
static struct inode *mc504fs_get_inode(struct super_block *sb,
				       const struct inode *dir, umode_t mode,
				       dev_t dev)
{
	struct inode * inode = new_inode(sb);

	if(inode)
	{
		inode->i_ino = get_next_ino();
		inode_init_owner(&nop_mnt_idmap, inode, dir, mode);
		simple_inode_init_ts(inode);

		switch (mode & S_IFMT)
		{
		case S_IFREG:
			break;
		case S_IFDIR:
			inode->i_op = &simple_dir_inode_operations;
			inode->i_fop = &simple_dir_operations;
			inc_nlink(inode);
			break;
		default:
			pr_err("mc504fs can not create this type of file\n");
			return NULL;
		}
	}
	return inode;
}

static int mc504fs_fill_super(struct super_block *sb, void *data, int silent)
{
	struct inode *inode;

	/* An identificator for our filesystem type */
	sb->s_magic = MC504FS_MAGIC;
	sb->s_blocksize = PAGE_SIZE;
	sb->s_blocksize_bits = PAGE_SHIFT;

	inode = mc504fs_get_inode(sb, NULL, S_IFDIR, 0);
	sb->s_root = d_make_root(inode);
	if(!sb->s_root)
		return -ENOMEM;

	return 0;
}

static struct dentry *mc504fs_mount(struct file_system_type *fs_type,
				    int flags, const char *dev_name,
				    void *data)
{
	struct dentry *ret;

	ret = mount_bdev(fs_type, flags, dev_name, data, mc504fs_fill_super);

	if (IS_ERR(ret))
		pr_err("Error mounting mc504fs\n");
	else
		pr_info("mc504fs is succesfully mounted on [%s]\n",
			dev_name);

	return ret;
}

static void mc504fs_kill_superblock (struct super_block *s)
{
	pr_info("mc504fs is succesfully unmounted\n");

	return;
}

struct file_system_type mc504fs_fs_type = {
	.owner = THIS_MODULE,
	.name = "mc504fs",
	.mount = mc504fs_mount,
	.kill_sb = mc504fs_kill_superblock,
};

static int __init mc504fs_init(void)
{
	int ret;

	ret = register_filesystem(&mc504fs_fs_type);
	if (ret)
		pr_err("Error registering mc504fs. Error: [%d]\n", ret);
	else
		pr_info("Succesfully register mc504fs\n");

	return ret; // A non 0 return value means init_module failed; module can't be loaded.
}

static void __exit mc504fs_exit(void)
{
	int ret;

	ret = unregister_filesystem(&mc504fs_fs_type);
	if (ret)
		pr_err("Error unregistering mc504fs. Error: [%d]\n", ret);
	else
		pr_info("Succesfully unregister mc504fs\n");
}

module_init(mc504fs_init);
module_exit(mc504fs_exit);

MODULE_AUTHOR("LKCAMP <gbittencourt@lkcamp.dev>");
MODULE_DESCRIPTION("A simple filesystem");
MODULE_LICENSE("GPL");
