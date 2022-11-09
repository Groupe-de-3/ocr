.SECONDEXPANSION:
MAKEFLAGS += -rR

$(guile (load "make_internal/utils.scm"))

# -------------------
# TARGETS
#
# Targets are a bunch of C files compiled into either an executable, a dynamic
# library or a static library. They can be depending on other targets, meaning
# they can include their files, and reference functions defined in them.
# -------------------
#
# Target example:
# 
# $(guile (define-target "example_library" \
#      # If given, another target named __example_library_tests will also be
#      # created. It will use as source folder, the one of the current target
#      # to which is appended '/tests'. It will have as target-type 'dynlib'.
#      # This also adds the 'ignore' option to the current target for the
#      # tests folder(s).
#     `(enable-tests)\         # Optional
#
#      # Specified what kind of artefact should be created by the target.
#      # Optional, defaults to staticlib
#      # Possible values are 'dynlib', 'staticlib' or 'executable'
#     `(target-type "dynlib")\
#
#      # If set to true, the final artefact will have all its depedencies
#      # linked into it. Making depedents of the target not needing to link
#      # them. Automatically enabled if target-type is executable, but can be
#      # overwritten.
#      # Optional, default to #f (false)
#      # If no value is given, it default to #t.
#     `(link-in-deps #t)
#
#      # List of one of more folder in which source files should be looked for,
#      # this also will be the include folder for this target and its depedents.
#      # Optional, defaults to:
#      #   If target-type is executable         -> executables/NAME
#      #   Otherwise (for dynlib and staticlib) -> libs/NAME
#      # Where NAME is the target's name given at the begigning.
#     `(source-dirs "source/folder" "other/source/folder")
#      
#      # List of targets which this target will depend on. This has for effect
#      # for any other target depending directly, or not, on this target, to
#      # link these depedencies if link-in-deps is true.
#      # Optional, defaults to an empty list.
#     `(deps "dep-one" "dep-two")
#      
#     `(cflags --flag-one -flags)
#     
#      # Blob of files to ignore while looking for source files in the given
#      # source-dirs.
#      # Optional, default to nothing.
#     `(ignore ignore/*/fil?)
#
#      # Only does something for executable targets. Gives the given arguments
#      # to the executable when running using the run-PROFILE-TARGET make rules.
#      # Optional, default to empty string.
#     `(run-args "arg1 --arg2 arg3" "-arg4")
# ))
#

# LIBRARIES

$(guile (define-target "example_library" \
	`(enable-tests)\
))

$(guile (define-target "matrices" \
	`(enable-tests)\
	`(deps "utils")\
))

$(guile (define-target "sudoku" \
	`(deps "vec")\
	`(enable-tests)\
))

$(guile (define-target "ia" \
	`(deps "matrices")\
))

$(guile (define-target "utils"))

$(guile (define-target "vec"))

$(guile (define-target "images" \
	`(deps "utils")\
))

$(guile (define-target "image_filters" \
	`(deps "images" "matrices" "vec" "linear_algebra")\
))

$(guile (define-target "linear_algebra" \
	`(deps "matrices") \
	`(enable-tests) \
))

$(guile (define-target "test_lib" \
	`(source-dirs "tests/lib")\
))

# EXECUTABLES

$(guile (define-target "example_executable"\
	`(target-type "executable")\
	`(deps "example_library")\
))

$(guile (define-target "tests"\
	`(run-args "$$(patsubst %,lib%.so,$$(test_targets))")\
	`(deps "test_lib")\
	`(target-type "executable")\
	`(source-dirs "tests/runner")\
))

$(guile (define-target "matrix_tests"\
	`(target-type "executable")\
	`(deps "matrices")\
))

$(guile (define-target "sudoku_tests"\
	`(target-type "executable")\
	`(deps "sudoku")\
))

$(guile (define-target "images_tests"\
	`(target-type "executable")\
	`(deps "images" "image_filters")\
))

$(guile (define-target "ia_test"\
	`(target-type "executable")\
	`(deps "ia" "matrices")\
))

$(guile (define-target "least_squares_tests"\
	`(target-type "executable")\
	`(deps "linear_algebra")\
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

format: $(addprefix format-,$(targets))
dev: run-debug-xor_nn

$(addprefix all-tests-,$(profiles)): all-tests-%: $(addprefix build-%-,$(test_targets))

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
