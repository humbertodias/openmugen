# System info
OS    := $(shell uname -s)
ARCH  := $(shell uname -m)
CMAKE := $(shell command -v cmake)
PWD   := $(shell pwd)
EXE   := OpenMugen

# Git versioning
TAG_NAME := $(shell git describe --tags --exact-match 2>/dev/null || git rev-parse --short HEAD)

# Build type (can override via `make build BUILD_TYPE=Debug`)
BUILD_TYPE ?= Release

.PHONY: build

# Default target
all: build

build:
	$(CMAKE) -Bbuild -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	$(CMAKE) --build build -- -j

build/wasm:	clean
	emcmake $(CMAKE) -Bbuild -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	cp wasm.index.html build/index.html
	$(CMAKE) --build build

run:
	./build/$(EXE)

run/wasm:
	python -m http.server -d build

clean:
	rm -rf build *.tar.gz data.zip log.txt $(EXE) cmake-build-*

release-files:
	echo "OpenMugen $(TAG_NAME)\n\nExecute:\n./run-$(OS).*" > build/README.txt	
	cp -r run-$(OS).* build/

targz/openmugen: release-files
	cd build && \
	tar -czvf "../$(EXE)-$(OS)-$(ARCH).tar.gz" $(EXE) README.txt run-*.*

targz/data:
	zip -9 -r data.zip data

ci:
	docker build -t openmugen .
	docker run --rm -v "$(PWD)":/app -w /app openmugen sh -c "make clean build validate-log"

validate-log:
	Xvfb :99 -screen 0 1024x768x24 > /dev/null 2>&1 &
	export DISPLAY=:99
	./build/$(EXE) &
	sleep 5
	@if grep -q 'VideoSystem Init OK' log.txt; then \
		echo "✅ $(EXE) ran successfully"; \
	else \
		echo "❌ $(EXE) did not produce expected output"; \
		echo "----- log.txt -----"; \
		cat log.txt; \
		exit 1; \
	fi
	pkill -9 $(EXE)
	pkill -9 Xvfb

valgrind:
	ulimit -n 4096
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./build/$(EXE)

format:
	find src -name "*.cpp" -o -name "*.h" | xargs clang-format -i

format-check:
	find src -name "*.cpp" -o -name "*.h" | xargs clang-format -n