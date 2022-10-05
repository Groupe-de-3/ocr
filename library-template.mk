# § is replaced by the profile
# ¤ is replaced by the library's name

ifndef ¤_source_dirs 
$(error "INVALID MAKEFILE: Missing the ¤_source_dirs variable")
endif
ifndef ¤_name
$(error "INVALID MAKEFILE: Missing the ¤_executable_name variable")
endif

§_¤_library_file_path = build/§/¤/lib$(¤_name).a

# Only executed once per library (ignored for next profiles)
ifndef ¤_source_files
¤_source_files != find $(¤_source_dirs) -name "*.c" -type f
¤_header_files != find $(¤_source_dirs) -name "*.h" -type f
P=(
M=)
$(eval ¤_rec_depedencies_with_duplicates := $(¤_depedencies) $(addprefix $$$P,$(addsuffix _rec_depedencies$M,$(¤_depedencies))))
¤_rec_depedencies := $(¤_rec_depedencies_with_duplicates)

$(eval ¤_depedencies_include_dirs := $(addprefix $$$P,$(addsuffix _source_dirs$M,$(sort $(¤_rec_depedencies)))))

format-¤:
	clang-format -i --style=file $(¤_source_files) $(¤_header_files)
.PHONY: format-¤
endif

§_¤_obj_dir = build/§/¤/obj
§_¤_obj_files = $(patsubst %.c,$(§_¤_obj_dir)/%.o,$(¤_source_files))
§_¤_dep_files = $(patsubst %.c,$(§_¤_obj_dir)/%.d,$(¤_source_files))

§_¤_cflags = $(addprefix -I ,$(¤_depedencies_include_dirs) $(¤_source_dirs)) $(§_cflags) $(¤_cflags)

$(§_¤_obj_dir)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(§_¤_cflags) -MMD -c $< -o $@

$(§_¤_library_file_path): $(§_¤_obj_files)
	mkdir -p $(dir $@)
	ar rcs $@ $^
	ranlib $@

build-§-¤: $(§_¤_library_file_path)
clean-§-¤:
	rm -f $(§_¤_obj_files)
	rm -f $(§_¤_dep_files)
	rm -f $(§_¤_library_file_path)
	find build -type d 2> /dev/null | tac | xargs --no-run-if-empty rmdir --ignore-fail-on-non-empty

.PHONY: clean-¤ build-¤

include $(wildcard $(§_¤_dep_files))

