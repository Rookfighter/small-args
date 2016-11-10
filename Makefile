.PHONY: all cloc format

all:
	@echo "specify target"


format:
	@astyle --options=.astylerc include/*.h test/*.c sample/*.c

cloc:
	@cloc include/*.hpp include/*.h
