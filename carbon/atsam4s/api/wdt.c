#include <flipper/wdt.h>

LF_FUNC("wdt") int wdt_configure(void) {
	return lf_success;
}

LF_FUNC("wdt") void wdt_fire(void) {

}