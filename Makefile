$(guile (load "make_internal/utils.scm"))
MAKEFLAGS += -rR

# -------------------
# LIBRARIES
#
# Libraries are a collection of files all built together for them to be
# used by another library and/or executable.
# -------------------

$(guile (define-target "example_library" \
	`(enable-tests)\
	`(target-type "dynlib")\
))

$(guile (define-target "matrices" \
	`(deps "utils")\
))

$(guile (define-target "sudoku"))

$(guile (define-target "ia"))

$(guile (define-target "utils"))

# -------------------
# EXECUTABLES
#
# Executables are a collection of files all built together with depedencies
# to create an executable file.
# -------------------

$(guile (define-target "example_executable"\
	`(target-type "executable")\
	`(deps "example_library")\
))

$(guile (define-target "tests"\
	`(target-type "executable")\
	`(source-dirs "tests")\
))

$(guile (define-target "matrix_tests"\
	`(target-type "executable")\
	`(deps "matrices")\
))

$(guile (define-target "sudoku_tests"\
	`(target-type "executable")\
	`(deps "sudoku")\
))

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

# Available but not required variables for a profile
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

# Creates every all-profile rules
$(addprefix all-,$(profiles)): all-%: $(patsubst %,build-\%-%,$(targets))

clean: $(foreach profile,$(profiles),$(patsubst %,clean-$(profile)-%,$(targets)))
	rm build/generated.mk
	rm -rf build/doc
# 	Delete every non empty directory in the build folder
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
# all targets.
# To do this it takes the file target-template.mk
# and replaces all occurences of the characters '§' and '¤' with the profile
# name and target name respectively.
# And does that for evey target/profile pair.
build/generated.mk: make_internal/target-template.mk make_internal/target-profile-template.mk Makefile
	mkdir -p build
	touch $@
	echo "" > $@
	
	# Normal templates of libraries
	for TARGET_NAME in $(targets); do \
		echo "# THIS IS AN AUTO-GENERATED FILE" >> $@; \
		echo "# DO NOT EDIT DIRECTLY" >> $@; \
		cat make_internal/target-template.mk | sed "s/^#.*$$//" | sed "s/¤/$$TARGET_NAME/g" >> $@; \
	done
	for PROFILE_NAME in $(profiles); do \
		for TARGET_NAME in $(targets); do \
			echo "# THIS IS AN AUTO-GENERATED FILE" >> $@; \
			echo "# DO NOT EDIT DIRECTLY" >> $@; \
			cat make_internal/target-profile-template.mk | sed "s/^#.*$$//" | sed "s/¤/$$TARGET_NAME/g"  | sed "s/§/$$PROFILE_NAME/g" >> $@; \
		done; \
	done

# Including the generated makefile will trigged its generation if not present
-include build/generated.mk
