#include <hermit/syscall.h>
#include <hermit/logging.h>
#include <asm/page.h>
#include <asm/uhyve.h>
#include <hermit/minifs.h>

typedef struct {
	char *oldpath;
	char *newpath;
	int ret;
} __attribute__((packed)) uhyve_rename_t;

int sys_rename(const char *oldpath, const char *newpath) {
	uhyve_rename_t arg;

	if(minifs_enabled)
		return minifs_rename(oldpath, newpath);

	arg.path = (char *)virt_to_phys((size_t)oldpath);
	arg.path = (char *)virt_to_phys((size_t)newpath);
	arg.ret = -1;

	uhyve_send(UHYVE_PORT_RENAME, (unsigned)virt_to_phys((size_t)&arg));

	return arg.ret;
}

