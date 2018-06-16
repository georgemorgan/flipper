#ifndef __usart_h__
#define __usart_h__

/* Include all types and macros exposed by the Flipper Toolbox. */
#include <flipper.h>

/* Declare the prototypes for all of the functions within this module. */
int usart_configure(void);
int usart_ready(void);
void usart_enable(void);
void usart_disable(void);
void usart_put(uint8_t byte);
uint8_t usart_get(void);
int usart_write(void *src, uint32_t length);
int usart_read(void *dst, uint32_t length);

#endif