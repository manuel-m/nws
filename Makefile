
current_dir := $(shell pwd)

BUILD_DIR=$(current_dir)/build
BUILD_DEBUG=$(BUILD_DIR)/debug
BUILD_RELEASE=$(BUILD_DIR)/release
TOOLCHAIN=toolchain

.PHONY: nothing
nothing:
	@echo "specify a valid target"

.PHONY: clean
clean:
	@ rm -rvf $(BUILD_DIR)

.PHONY: toolchain_clean
toolchain_clean:
	@ rm -rf  $(TOOLCHAIN)/node_modules

.PHONY: configure
configure: clean
	@ mkdir -vp $(BUILD_DEBUG) && cd $(BUILD_DEBUG) && cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja ../../src
	@ mkdir -vp $(BUILD_RELEASE) && cd $(BUILD_RELEASE) && cmake -DCMAKE_BUILD_TYPE=Release G Ninja  ../../src
	@ cd $(TOOLCHAIN) && npm i

.PHONY: build
build: release debug
	@echo "build target"

release:
	@ cd $(BUILD_RELEASE) && make

debug:
	@ ninja -v -C $(BUILD_DEBUG) all

.PHONY: watch
watch:
	@node $(TOOLCHAIN)/watchApp.js


