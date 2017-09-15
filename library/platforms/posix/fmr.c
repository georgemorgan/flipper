#define __private_include__
#include <flipper/carbon.h>
#include <flipper/posix/posix.h>
#include <dlfcn.h>

const void *const fmr_modules[] = {
	&button,
	&fs,
	&gpio,
	&led,
	&uart0,
	&wdt
};

uint32_t fmr_call(const uint32_t (* function)(void), uint8_t argc, uint16_t argt, void *argv) {
	/* Grab the symbol name of the function for debugging purposes. */
	// Dl_info info;
	// dladdr(function, &info);
	// printf("Calling local function '%s', with %i arguments, arg types %i, and va_list %p.\n", info.dli_sname, argc, argt, argv);
	return function();
}

fmr_return fmr_push(struct _fmr_push_pull_packet *packet) {
	return -1;
}

fmr_return fmr_pull(struct _fmr_push_pull_packet *packet) {
	return -1;
}