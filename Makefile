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


test_gen: release 
	make -C ./build
	cd ./build/ && ./cocogen/cocogen ../cocogen/main.ccn
	./scripts/test_new_gen.tcl ./build/ccngen ./


test_gen_debug: debug
	make -C ./build
	cd ./build/ && ./cocogen/cocogen ../cocogen/main.ccn
	./scripts/test_new_gen.tcl ./build/ccngen ./


clean:
	rm -rf build/
