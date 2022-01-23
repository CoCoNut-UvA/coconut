.PHONY: help debug release default builddir

default: release
	make -C build/

help:
	@echo "Targets:"
	@echo "  debug  : Generate build artifacts for a debug build in build/"
	@echo "  release: Generate build artifacts for a release build in build/"


# On older cmake -B does not create the directory.
debug: builddir
	@cmake -DCMAKE_BUILD_TYPE=Debug -S ./ -B build/

release: builddir
	@cmake -DCMAKE_BUILD_TYPE=Release -S ./ -B build/

builddir:
	mkdir -p build/


.PHONY: test_gen
test_gen:
	./scripts/test_new_gen.tcl ./ release

.PHONY: test_gen_debug
test_gen_debug:
	./scripts/test_new_gen.tcl ./ debug

clean:
	rm -rf build/
