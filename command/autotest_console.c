#include <stddef.h>

#include "autotest_command.h"

static int ctrlc_disabled = 0;	/* see disable_ctrl() */
static int ctrlc_was_pressed = 0;

int had_ctrlc (void)
{
	return ctrlc_was_pressed;
}

void uart_init(void)
{

}

void clear_ctrlc(void)
{
	ctrlc_was_pressed = 0;
}

