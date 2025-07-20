# System info
OS    := $(shell uname -s)
ARCH  := $(shell uname -m)
CMAKE := $(shell command -v cmake)
PWD   := $(shell pwd)

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

build-wasm:	clean
	emcmake $(CMAKE) -Bbuild -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	cp -r data build
	$(CMAKE) --build build

run:
	./build/OpenMugen

run-wasm:
	python -m http.server -d build

clean:
	rm -rf build *.tar.gz data.zip log.txt

prepare-release-files:
	echo "OpenMugen $(TAG_NAME)\n\nExecute:\n./run-$(OS).*" > build/README.txt	
	cp -r run-$(OS).* build/

targz/openmugen: prepare-release-files
	cd build && \
	tar -czvf "../OpenMugen-$(OS)-$(ARCH).tar.gz" OpenMugen README.txt run-*.*

targz/data:
	zip -9 -r data.zip data

ci:
	docker build -t openmugen .
	docker run --rm -v "$(PWD)":/app -w /app openmugen sh -c "make clean build"

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./build/OpenMugen

format:
	find src -name "*.cpp" -o -name "*.h" | xargs clang-format -i
