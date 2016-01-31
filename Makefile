
current_dir := $(shell pwd)

BUILD_DIR=$(current_dir)/build
BUILD_DEBUG=$(BUILD_DIR)/debug
BUILD_RELEASE=$(BUILD_DIR)/release

.PHONY: nothing
nothing:
	@echo "specify a valid target"

.PHONY: bootstrap
bootstrap:
	@ rm -rvf $(BUILD_DIR)
	@ mkdir -vp $(BUILD_DEBUG) && cd $(BUILD_DEBUG) && cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja ../../src
	@ mkdir -vp $(BUILD_RELEASE) && cd $(BUILD_RELEASE) && cmake -DCMAKE_BUILD_TYPE=Release G Ninja  ../../src

.PHONY: build
build: release debug
	@echo "build target"

release:
	@ cd $(BUILD_RELEASE) && make

debug:
	@ ninja -v -C $(BUILD_DEBUG) all

.PHONY: watch
watch:
	@node toolchain/watchApp.js


