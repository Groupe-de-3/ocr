$(guile (load "make_internal/utils.scm"))
MAKEFLAGS += -rR

# -------------------
# LIBRARIES
#
# Libraries are a collection of files all built together for them to be
# used by another library and/or executable.
# -------------------

# Required:
# LIBRARY_NAME_source_dirs = libs/LIBRARY_NAME
# LIBRARY_NAME_name = name
# 
# Optional:
# LIBRARY_NAME_depedencies = OTHER_LIBRARY_NAME_1 OTHER_LIBRARY_NAME_2 ...
# LIBRARY_NAME_cflags = -FLAG1 -FLAG2 value2
# LIBRARY_NAME_ignore = path/to*/ignore/*
# LIBRARY_NAME_target_type = dynlib # Defaults to static

$(guile (define-library "example_library" \
	`(target-type "dynlib")))
$(guile (enable-library-tests "example_library"))

$(guile (define-library "matrices" \
	`(deps "utils")))

$(guile (define-library "sudoku"))

$(guile (define-library "ia"))

$(guile (define-library "utils"))

# -------------------
# EXECUTABLES
#
# Executables are a collection of files all built together with depedencies
# to create an executable file.
# -------------------

executables = example_executable tests matrix_tests sudoku_tests

# EXECUTABLE_NAME_source_dirs := executables/EXECUTABLE_NAME
# EXECUTABLE_NAME_executable_name := name_of_an_executable
# EXECUTABLE_NAME_depedencies := LIBRARY_NAME_1 LIBRARY_NAME_2

example_executable_source_dirs := executables/example_executable
example_executable_executable_name := example_executable
example_executable_depedencies := example_library

tests_source_dirs := tests
tests_executable_name := tests

matrix_tests_source_dirs := executables/matrix_tests
matrix_tests_executable_name := matrix_tests
matrix_tests_depedencies := matrices

sudoku_tests_source_dirs := executables/sudoku_tests
sudoku_tests_executable_name := sudoku_tests
sudoku_tests_depedencies := sudoku

# -------------------
# (Build) PROFILES
#
# Profiles are a set of build flags that can be easily switched.
# -------------------

CC = clang

# All profiles share the same warnings (kinda)
c_warnings = -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-align \
             -Wmissing-declarations -Wconversion -Wstrict-prototypes \
			 -Wtautological-compare -Wtype-limits -Wconstant-evaluated \
			 -Wno-unused-command-line-argument
c_errors = -Werror=implicit-function-declaration
# Flags all profiles share
default_cflags = $(c_warnings) $(c_errors) -std=gnu17 -lm
# Some flags required to remove all asserts()
disable_asserts_cflags = -DNDEBUG -Wno-unused-variable -Wno-unused-parameter

profiles = valgrind-debug debug release
# Targets gave to libraries or executables that don't precise them
default_profiles = valgrind-debug debug release

# Avaible but not required variables for a profile
# PROFILE_NAME_cflags - List of flags to give to $CC

debug_cflags = $(default_cflags) -fsanitize=address  -static-libsan -g -Og
valgrind-debug_cflags = $(default_cflags) -gdwarf-4 -g -Og $(disable_asserts_cflags)
release_cflags = $(default_cflags) -O3 $(disable_asserts_cflags)

# -------------------
# SPECIAL RULES
# -------------------

.DEFAULT_GOAL = all

# `all` will use the release profile by default, but can use debug if the
# env variable `MODE` is set to debug.
ifeq ($(MODE), debug)
all: all-debug
else
MODE=release
all: all-release
endif

$(addprefix all-,$(profiles)): all-%: $(patsubst %,build-\%-%,$(libraries) $(executables))
clean: $(foreach profile,$(profiles),$(patsubst %,clean-$(profile)-%,$(libraries) $(executables)))
	rm build/generated.mk
	rm -rf build/doc
	find build -type d 2> /dev/null | tac | xargs --no-run-if-empty rmdir --ignore-fail-on-non-empty

format: $(addprefix format-,$(libraries) $(executables))
dev: run-debug-xor_nn
test: run-debug-tests

doc:
	mkdir -p build/doc
	doxygen Doxyfile

open-html-doc: doc
	xdg-open build/doc/html/index.html

.PHONY: all doc open-doc clean dev test $(addprefix all-,$(profiles)) format-all
.SILENT: build/generated.mk

# Generates a new Makefile (build/generated.mk) that have rules for building 
# all libraries and executables.
# To do this it takes the files executable-template.mk and library-template.mk
# and replaces all occurences of the characters '§' and '¤' with the profile
# name and executable or library name respectively.
# And does that for evey library or executable respectively times all profiles.
build/generated.mk: make_internal/library-target-template.mk make_internal/executable-target-template.mk Makefile
	mkdir -p build
	touch $@
	echo "# THIS IS AN AUTO-GENERATED FILE" > $@
	echo "# DO NOT EDIT DIRECTLY" >> $@
	
	# Normal templates of libraries
	for LIBRARY_NAME in $(libraries); do \
		cat make_internal/library-template.mk | sed "s/^#.*$$//" | sed "s/¤/$$LIBRARY_NAME/g" >> $@; \
	done
	# Normal templates of executables
	for EXECUTABLE_NAME in $(executables); do \
		cat make_internal/executable-template.mk | sed "s/^#.*$$//" | sed "s/¤/$$EXECUTABLE_NAME/g"  | sed "s/§/$$PROFILE_NAME/g" >> $@; \
	done
	for PROFILE_NAME in $(profiles); do \
		for LIBRARY_NAME in $(libraries); do \
			cat make_internal/library-target-template.mk | sed "s/^#.*$$//" | sed "s/¤/$$LIBRARY_NAME/g"  | sed "s/§/$$PROFILE_NAME/g" >> $@; \
		done; \
		for EXECUTABLE_NAME in $(executables); do \
			cat make_internal/executable-target-template.mk | sed "s/^#.*$$//" | sed "s/¤/$$EXECUTABLE_NAME/g"  | sed "s/§/$$PROFILE_NAME/g" >> $@; \
		done \
	done

# Including the generated makefile will trigged its generation if not present
-include build/generated.mk
