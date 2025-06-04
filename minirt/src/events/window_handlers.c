#include "../../includes/main.h"
#include "../../includes/events.h"
#include <stdio.h>

int	close_window_x(t_vars *vars)
{
	(void)vars;
	exit(0);
	return (0);
}

int	close_window_esc(int keycode, t_vars *vars)
{
	(void)vars;
	if (keycode == KEY_ESC || keycode == KEY_ESC_MAC)
		exit(0);
	return (0);
}

void	print_controls_help(void)
{
	printf("\n=== MiniRT Transform Controls ===\n");
	printf("CAMERA MOVEMENT:\n");
	printf("  W/S - Move forward/backward\n");
	printf("  A/D - Move left/right\n");
	printf("  Q/E - Move down/up\n");
	printf("\nCAMERA ROTATION:\n");
	printf("  I/K - Look up/down\n");
	printf("  J/L - Look left/right\n");
	printf("\nOBJECT CONTROLS:\n");
	printf("  P/O - Select object (next/previous)\n");
	printf("  Arrow keys - Move object (left/right/up/down)\n");
	printf("  +/- - Scale object up/down\n");
	printf("  R/F - Rotate object around X-axis (forward/reverse)\n");
	printf("  T/G - Rotate object around Y-axis (forward/reverse)\n");
	printf("\nOTHER:\n");
	printf("  SPACE - Show this help\n");
	printf("  ESC - Exit\n");
	printf("==============================\n\n");
}