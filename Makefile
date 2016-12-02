.PHONY: all cloc format test

all:
	@echo "specify target"


format:
	@astyle --options=.astylerc include/*.h test/*.c sample/*.c
	
test:
	@./build/sarg_test_c

cloc:
	@cloc include/*.hpp include/*.h
