#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/ktime.h>
#include <linux/types.h>
#include <linux/printk.h>


static int __init days_till_new_year_init(void) {
	struct tm tm;
	time64_t current_time, new_year_time;
	long days_remaining;

	current_time = ktime_get_real_seconds();
	time64_to_tm(current_time, 0, &tm);

	new_year_time = mktime64(tm.tm_year + 1900 + 1, 1, 1, 0, 0, 0);

	days_remaining = (new_year_time - current_time) / 86400;

	pr_info("Days until New Year: %ld\n", days_remaining);
	return 0;
}

static void __exit days_till_new_year_exit(void) {
	pr_info("Days untill New Year unloaded");
}

MODULE_LICENSE("GPL");

module_init(days_till_new_year_init);
module_exit(days_till_new_year_exit);
