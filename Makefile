include prelude.mk

this_dir    := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
src_dir     := $(this_dir)
build_dir   := $(this_dir)build
boost_dir   := $(build_dir)/boost
cmake_dir   := $(build_dir)/cmake
install_dir := $(build_dir)/install

TOOLSET         ?= mingw
PLATFORM        ?= auto
CONFIGURATION   ?= Debug
BOOST_VERSION   ?= 1.81.0
BOOST_LIBRARIES := --with-test
CMAKE_FLAGS     ?= -D WINAPI_UTF8_TESTS=ON
INSTALL_PREFIX  ?= $(install_dir)

$(eval $(call noexpand,TOOLSET))
$(eval $(call noexpand,PLATFORM))
$(eval $(call noexpand,CONFIGURATION))
$(eval $(call noexpand,BOOST_VERSION))
$(eval $(call noexpand,CMAKE_FLAGS))
$(eval $(call noexpand,INSTALL_PREFIX))

.PHONY: all
all: build

.PHONY: clean
clean:
	rm -rf -- '$(call escape,$(build_dir))'

$(boost_dir)/:
	cd cmake && python3 -m project.boost.download \
		--cache '$(call escape,$(build_dir))' \
		-- \
		'$(call escape,$(BOOST_VERSION))' \
		'$(call escape,$(boost_dir))'

.PHONY: deps
deps: $(boost_dir)/
	cd cmake && python3 -m project.boost.build \
		--toolset '$(call escape,$(TOOLSET))' \
		--platform '$(call escape,$(PLATFORM))' \
		--configuration '$(call escape,$(CONFIGURATION))' \
		-- \
		'$(call escape,$(boost_dir))' \
		$(BOOST_LIBRARIES)

.PHONY: build
build:
	cd cmake && python3 -m project.build \
		--toolset '$(call escape,$(TOOLSET))' \
		--platform '$(call escape,$(PLATFORM))' \
		--configuration '$(call escape,$(CONFIGURATION))' \
		--install '$(call escape,$(INSTALL_PREFIX))' \
		--boost '$(call escape,$(boost_dir))' \
		-- \
		'$(call escape,$(src_dir))' \
		'$(call escape,$(cmake_dir))' \
		$(CMAKE_FLAGS)

.PHONY: install
install: build

.PHONY: test
test:
	cd -- '$(call escape,$(cmake_dir))' && ctest -C '$(call escape,$(CONFIGURATION))' --verbose

xdg-open := $(shell command -v xdg-open 2> /dev/null)

.PHONY: docs
docs:
	cmake --build '$(call escape,$(cmake_dir))' --target winapi_utf8_docs
ifndef CI
ifdef xdg-open
	xdg-open '$(call escape,$(cmake_dir))/html/index.html' &> /dev/null
endif
endif

clang-tidy := run-clang-tidy
ifeq (1,$(shell test -e /usr/share/clang/run-clang-tidy.py && echo 1))
clang-tidy := /usr/share/clang/run-clang-tidy.py
endif

.PHONY: check
check:
	$(clang-tidy) -p '$(call escape,$(cmake_dir))' -quiet
