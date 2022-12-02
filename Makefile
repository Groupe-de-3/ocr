.SECONDEXPANSION:
MAKEFLAGS += -rR

# LIBRARIES

targets += example_library

example_library_name = example_library
example_library_target_type = staticlib
example_library_source_dirs += libs/example_library
example_library_depedencies = utils

targets += matrices

matrices_name = matrices
matrices_target_type = staticlib
matrices_source_dirs += libs/matrices
matrices_depedencies = utils

targets += sudoku

sudoku_name = sudoku
sudoku_target_type = staticlib
sudoku_source_dirs += libs/sudoku
sudoku_depedencies = vec

targets += ia

ia_name = ia
ia_target_type = staticlib
ia_source_dirs += libs/ia
ia_depedencies = matrices

targets += utils

utils_name = utils
utils_target_type = staticlib
utils_source_dirs += libs/utils

targets += vec

vec_name = vec
vec_target_type = staticlib
vec_source_dirs += libs/vec

targets += images

images_name = images
images_target_type = staticlib
images_source_dirs += libs/images
images_depedencies = utils

targets += image_filters

image_filters_name = image_filters
image_filters_target_type = staticlib
image_filters_source_dirs += libs/image_filters
image_filters_depedencies = images matrices vec linear_algebra

targets += linear_algebra

linear_algebra_name = linear_algebra
linear_algebra_target_type = staticlib
linear_algebra_source_dirs += libs/linear_algebra
linear_algebra_depedencies = matrices utils

targets += mnist_loader

mnist_loader_name = mnist_loader
mnist_loader_target_type = staticlib
mnist_loader_source_dirs += libs/mnist_loader
mnist_loader_depedencies = images

targets += test_lib

test_lib_name = test_lib
test_lib_target_type = staticlib
test_lib_source_dirs += tests/lib

# EXECUTABLES

targets += example_executable

example_executable_name = example_executable
example_executable_target_type = executable
example_executable_link_in_deps = true
example_executable_source_dirs += executables/example_executable
example_executable_depedencies = example_library

targets += tests

tests_name = tests
tests_target_type = executable
tests_link_in_deps = true
tests_source_dirs += tests/runner
tests_depedencies = test_lib
tests_run_arguments = $(patsubst %,lib%.so,$(test_targets))

targets += matrix_tests

matrix_tests_name = matrix_tests
matrix_tests_target_type = executable
matrix_tests_link_in_deps = true
matrix_tests_source_dirs += executables/matrix_tests
matrix_tests_depedencies = matrices

targets += sudoku_tests

sudoku_tests_name = sudoku_tests
sudoku_tests_target_type = executable
sudoku_tests_link_in_deps = true
sudoku_tests_source_dirs += executables/sudoku_tests
sudoku_tests_depedencies = sudoku

targets += images_tests

images_tests_name = images_tests
images_tests_target_type = executable
images_tests_link_in_deps = true
images_tests_source_dirs += executables/images_tests
images_tests_depedencies = images image_filters

targets += ia_test

ia_test_name = ia_test
ia_test_target_type = executable
ia_test_link_in_deps = true
ia_test_source_dirs += executables/ia_test
ia_test_depedencies = ia matrices

targets += least_squares_tests

least_squares_tests_name = least_squares_tests
least_squares_link_in_deps = true
least_squares_tests_target_type = executable
least_squares_tests_source_dirs += executables/least_squares_tests
least_squares_tests_depedencies = linear_algebra

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

debug_cflags = $(default_cflags) -fsanitize=address -static-libsan -g -Og
valgrind-debug_cflags = $(default_cflags) -gdwarf-4 -g -Og
release_cflags = $(default_cflags) -Ofast $(disable_asserts_cflags)

# -------------------
# SPECIAL RULES
# -------------------

.DEFAULT_GOAL = all

# `all` will use the release profile by default, but can use debug if the
# env variable `MODE` is set to debug.
ifeq ($(MODE), debug)
DEFAULT_PROFILE=debug
else
MODE=release
DEFAULT_PROFILE=release
all: all-release
endif

# Creates every all-profile rules
$(addprefix all-,$(profiles)): all-%: $(patsubst %,build-\%-%,$(targets))

clean: $(foreach profile,$(profiles),$(patsubst %,clean-$(profile)-%,$(targets)))
	rm build/generated.mk
	rm -rf build/doc
# 	Delete every non empty directory in the build folder
	find build -type d 2> /dev/null | tac | xargs --no-run-if-empty rmdir --ignore-fail-on-non-empty

format: $(addprefix format-,$(targets))
dev: run-debug-xor_nn

$(addprefix all-tests-,$(profiles)): all-tests-%: $(addprefix build-%-,$(test_targets))

all: all-$(DEFAULT_PROFILE)

doc:
	mkdir -p build/doc
	doxygen Doxyfile

open-html-doc: doc
	xdg-open build/doc/html/index.html

.PHONY: all doc open-doc clean dev $(addprefix all-tests-,$(profiles)) \
        $(addprefix all-,$(profiles)) format format-all
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
