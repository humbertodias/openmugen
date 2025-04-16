OS    := $(shell uname -s)
ARCH  := $(shell uname -m)
CMAKE := $(shell which cmake)

# if there is no current tag use the git commit hash
TAG_NAME := $(shell git describe --tags --exact-match 2>/dev/null || git rev-parse --short HEAD)

.PHONY: build
BUILD_TYPE=Debug
BUILD_TYPE=Release
build:
	$(CMAKE) -Bbuild -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
	$(CMAKE) --build build -v

run:
	./build/OpenMugen

clean:
	rm -rf build *.tar.gz data.zip log.txt

copy-release-files:
	echo "OS: ${OS}"
	cp release/${OS}/* build/

targz/openmugen:	copy-release-files
	cd build && \
	tar -czvf "../OpenMugen-${TAG_NAME}-${OS}-${ARCH}.tar.gz" OpenMugen launcher.*

targz/data:
	zip -9 -r data.zip data

ci:
	docker build -t openmugen .
	docker run -v ${PWD}:/app -w /app openmugen sh -c "make clean build"