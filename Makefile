MAKEFLAGS += -rRs

# -------------------
# LIBRARIES
# -------------------

libraries = example_library matrices

# Required:
# LIBRARY_NAME_source_dirs = libs/LIBRARY_NAME
# LIBRARY_NAME_name = name
# 
# Optional:
# LIBRARY_NAME_depedencies = OTHER_LIBRARY_NAME_1 OTHER_LIBRARY_NAME_2 ...
# LIBRARY_NAME_cflags = -nicktamaman

example_library_source_dirs = libs/example_library
example_library_name = example_library

matrices_source_dirs = libs/matrices
matrices_name = matrices

# -------------------
# EXECUTABLES
# -------------------

executables = example_executable

# EXECUTABLE_NAME_source_dirs := executables/EXECUTABLE_NAME
# EXECUTABLE_NAME_executable_name := name_of_an_executable
# EXECUTABLE_NAME_depedencies := LIBRARY_NAME_1 LIBRARY_NAME_2

example_executable_source_dirs := executables/example_executable
example_executable_executable_name := example_executable
example_executable_depedencies := example_library

# -------------------
# (Build) PROFILES
# -------------------

CC = clang

c_warnings = -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-align \
             -Wmissing-declarations -Wconversion -Wstrict-prototypes \
			 -Wtautological-compare -Wtype-limits -Wconstant-evaluated \
			 -Wno-unused-command-line-argument
c_errors = -Werror=implicit-function-declaration
default_cflags = $(c_warnings) $(c_errors) -std=gnu17 -lm -g -Og
disable_asserts_cflags = -DNDEBUG -Wno-unused-variable -Wno-unused-parameter

profiles = valgrind-debug debug release

debug_cflags = $(default_cflags) -fsanitize=address -g -Og
valgrind-debug_cflags = $(default_cflags) -gdwarf-4 -g -Og $(disable_asserts_cflags)
release_cflags = $(default_cflags) -O3 $(disable_asserts_cflags)

# -------------------
# Normal rules
# -------------------

.DEFAULT_GOAL = all
ifeq ($(MODE), debug)
all: all-debug
else
MODE=release
all: all-release
endif

$(addprefix all-,$(profiles)): all-%: $(patsubst %,build-\%-%,$(libraries) $(executables))
clean: $(foreach profile,$(profiles),$(patsubst %,clean-$(profile)-%,$(libraries) $(executables)))
	rm build/generated.mk
	find build -type d 2> /dev/null | tac | xargs --no-run-if-empty rmdir --ignore-fail-on-non-empty

format: $(addprefix format-,$(libraries) $(executables))
dev: run-debug-xor_nn
test: run-debug-tests

.PHONY: all clean dev test $(addprefix all-,$(profiles)) format-all
build/generated.mk: library-template.mk executable-template.mk Makefile
	mkdir -p build
	touch $@
	echo "# THIS IS AN AUTO-GENERATED FILE" > $@
	echo "# DO NOT EDIT DIRECTLY" >> $@
	for PROFILE_NAME in $(profiles); do \
		for LIBRARY_NAME in $(libraries); do \
			cat library-template.mk | sed "s/^#.*$$//" | sed "s/¤/$$LIBRARY_NAME/g"  | sed "s/§/$$PROFILE_NAME/g" >> $@; \
			echo "# THIS IS AN AUTO-GENERATED FILE" >> $@; \
			echo "# DO NOT EDIT DIRECTLY" >> $@; \
		done; \
		for EXECUTABLE_NAME in $(executables); do \
			cat executable-template.mk | sed "s/^#.*$$//" | sed "s/¤/$$EXECUTABLE_NAME/g"  | sed "s/§/$$PROFILE_NAME/g" >> $@; \
			echo "# THIS IS AN AUTO-GENERATED FILE" >> $@; \
			echo "# DO NOT EDIT DIRECTLY" >> $@; \
		done \
	done

-include build/generated.mk
