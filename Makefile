# Root Makefile for Raytracing project

all:
	$(MAKE) -C minirt

clean:
	$(MAKE) -C minirt clean

fclean:
	$(MAKE) -C minirt fclean

re:
	$(MAKE) -C minirt re

.PHONY: all clean fclean re
