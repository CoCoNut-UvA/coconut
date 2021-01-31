.PHONY: help debug release default

default: release
	make -C build/

help:
	@echo "Targets:"
	@echo "  debug  : Generate build artifacts for a debug build in build-debug"
	@echo "  release: Generate build artifacts for a release build in build-release"


debug:
	@cmake -DCMAKE_BUILD_TYPE=Debug -S ./ -B build/

release:
	@cmake -DCMAKE_BUILD_TYPE=Release -S ./ -B build/
