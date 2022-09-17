# § is replaced by the profile
# ¤ is replaced by the executable's name

ifndef ¤_source_dirs 
$(error "INVALID MAKEFILE: Missing the ¤_source_dirs variable")
endif
ifndef ¤_executable_name
$(error "INVALID MAKEFILE: Missing the ¤_executable_name variable")
endif

§_¤_executable_file_path = build/§/¤/$(¤_executable_name)
ifndef ¤_source_files
¤_source_files != find $(¤_source_dirs) -name "*.c" -type f
P=(
M=)
$(eval ¤_rec_depedencies_with_duplicates := $(¤_depedencies) $(addprefix $$$P,$(addsuffix _rec_depedencies$M,$(¤_depedencies))))
¤_rec_depedencies := $(¤_rec_depedencies_with_duplicates)

$(eval ¤_depedencies_include_dirs := $(addprefix $$$P,$(addsuffix _source_dirs$M,$(¤_depedencies))))
endif

§_¤_obj_dir = build/§/¤/obj
§_¤_obj_files = $(patsubst %.c,$(§_¤_obj_dir)/%.o,$(¤_source_files))
§_¤_dep_files = $(patsubst %.c,$(§_¤_obj_dir)/%.d,$(¤_source_files))

§_¤_cflags = $(addprefix -I ,$(¤_depedencies_include_dirs) $(¤_source_dirs)) $(§_cflags) $(¤_cflags)

$(eval §_¤_depedencies_files := $(addprefix $$$P§_,$(addsuffix _library_file_path$M,$(¤_rec_depedencies))))

$(§_¤_obj_dir)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(§_¤_cflags) -MMD -c $< -o $@

$(§_¤_executable_file_path): $(§_¤_obj_files) $(§_¤_depedencies_files)
	mkdir -p $(dir $@)
	$(CC) $(§_¤_cflags) $(§_¤_obj_files) $(§_¤_depedencies_files) -o $@

build-§-¤: $(§_¤_executable_file_path)
run-§-¤: $(§_¤_executable_file_path)
	./$(§_¤_executable_file_path)
clean-§-¤:
	rm -f $(§_¤_obj_files)
	rm -f $(§_¤_dep_files)
	rm -f $(§_¤_executable_file_path)
	find build -type d 2> /dev/null | tac | xargs --no-run-if-empty rmdir --ignore-fail-on-non-empty

.PHONY: clean-¤ build-¤

include $(wildcard $(§_¤_dep_files))

