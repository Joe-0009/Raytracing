#include "../../includes/events_bonus.h"
#include "../../includes/minirt_app_bonus.h"
#include <stdio.h>

// Functions moved to events_bonus.c to avoid duplication

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
	printf("\nROTATION (context-sensitive):\n");
	printf("  For SPHERES: R/T/F/G - Rotate texture and bump map\n");
	printf("  For OTHER OBJECTS: R/T/F/G - Rotate object geometry\n");
	printf("    R/F - Rotate around X-axis (forward/reverse)\n");
	printf("    T/G - Rotate around Y-axis (forward/reverse)\n");
	printf("\nOTHER:\n");
	printf("  SPACE - Show this help\n");
	printf("  ESC - Exit\n");
	printf("==============================\n\n");
}
