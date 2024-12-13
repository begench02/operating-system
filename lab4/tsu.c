#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/time.h>
#include <linux/ktime.h>
#include <linux/types.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#define procfs_name "tsu"
static struct proc_dir_entry *our_proc_file = NULL;


static int calculate_days_till_new_year(void) {
	struct tm tm;
	time64_t current_time, new_year_time;
	long days_remaining;

	current_time = ktime_get_real_seconds();
	time64_to_tm(current_time, 0, &tm);

	new_year_time = mktime64(tm.tm_year + 1900 + 1, 1, 1, 0, 0, 0);

	days_remaining = (new_year_time - current_time) / 86400;

	return days_remaining;
}

static ssize_t procfile_read(struct file *file_pointer, char __user *buffer, size_t buffer_length, loff_t *offset) {
	int days_till_new_year = calculate_days_till_new_year();

	static int finished = 0;
	if (finished) {
		finished = 1;
		return 0;
	}
	finished = 1;

	char s[5];
	sprintf(s, "%d\n", days_till_new_year);
	copy_to_user(buffer, s, 5);
	return 5;
}

static const struct proc_ops proc_file_fops = {
	.proc_read = procfile_read,
};

static int __init procfs1_init(void) {
	our_proc_file = proc_create(procfs_name, 0644, NULL, &proc_file_fops);
	return 0;
}

static void __exit procfs1_exit(void) {
	proc_remove(our_proc_file);
}

MODULE_LICENSE("GPL");

module_init(procfs1_init);
module_exit(procfs1_exit);
