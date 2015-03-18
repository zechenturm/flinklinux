#ifndef FLINK_DEBUG_H_
#define FLINK_DEBUG_H_


#define printka(fmt, ...)	printk(KERN_ALERT   pr_fmt("[" KBUILD_MODNAME "]: " fmt "\n"), ##__VA_ARGS__)
#define printke(fmt, ...)	printk(KERN_ERR     pr_fmt("[" KBUILD_MODNAME "]: " fmt "\n"), ##__VA_ARGS__)
#define printkw(fmt, ...)	printk(KERN_WARNING pr_fmt("[" KBUILD_MODNAME "]: " fmt "\n"), ##__VA_ARGS__)
#define printkn(fmt, ...)	printk(KERN_NOTICE  pr_fmt("[" KBUILD_MODNAME "]: " fmt "\n"), ##__VA_ARGS__)
#define printki(fmt, ...)	printk(KERN_INFO    pr_fmt("[" KBUILD_MODNAME "]: " fmt "\n"), ##__VA_ARGS__)
#define printkd(fmt, ...)	printk(KERN_DEBUG   pr_fmt("[" KBUILD_MODNAME "]: " fmt "\n"), ##__VA_ARGS__)


#endif // FLINK_DEBUG_H_

