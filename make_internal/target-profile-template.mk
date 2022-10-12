# § is replaced with the profile
# ¤ is replaced with the target's name

§_¤_obj_dir = build/§/¤/obj
§_¤_obj_files = $(patsubst %.c,$(§_¤_obj_dir)/%.o,$(¤_source_files))
§_¤_dep_files = $(patsubst %.c,$(§_¤_obj_dir)/%.d,$(¤_source_files))

§_¤_cflags = $(addprefix -I ,$(¤_depedencies_include_dirs) $(¤_source_dirs)) $(§_cflags) $(¤_cflags)

ifeq ($(¤_target_type),executable)
§_¤_output_folder = build/§
§_¤_output_file_path = $(§_¤_output_folder)/$(¤_name).out
else ifeq ($(¤_target_type),dynlib)
§_¤_output_folder = build/§
§_¤_output_file_path = $(§_¤_output_folder)/lib$(¤_name).so
§_¤_cflags += -fPIC -fvisibility=default
else
§_¤_output_folder = build/§/¤
§_¤_output_file_path = $(§_¤_output_folder)/lib$(¤_name).a
endif

# if ¤_link_in_deps is set to true, depedencies must be linked into the output
# file, so we add the required flags to §_¤_cflags
ifeq ($(¤_link_in_deps),true)
$(eval §_¤_depedencies_files := $(addprefix $$$P§_,$(addsuffix _output_file_path$M,$(¤_rec_depedencies))))

# Adding depedencies search paths
$(eval §_¤_depedencies_flags += $(addprefix -L$$$P§_,$(addsuffix _output_folder$M,$(¤_rec_depedencies))))
# Remove duplicates (which will happen if all depedencies are in the same folder)
§_¤_depedencies_flags := $(sort $(§_¤_depedencies_flags))
# Adding libraries
§_¤_depedencies_flags += $(addprefix -l,$(¤_rec_depedencies))

§_¤_cflags += $(§_¤_depedencies_flags)
endif

$(§_¤_obj_dir)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(§_¤_cflags) -MMD -c $< -o $@

$(§_¤_output_file_path): $(§_¤_obj_files) $(§_¤_depedencies_files)
	mkdir -p $(dir $@)
ifeq ($(¤_target_type),executable)
	$(CC) $(§_¤_cflags) -Wl,-rpath,'$$ORIGIN' $^ -o $@
else ifeq ($(¤_target_type),dynlib)
	$(CC) $(§_¤_cflags) -shared $^ -o $@
else
	ar rcs $@ $^
	ranlib $@
endif

build-§-¤: $(§_¤_output_file_path)
clean-§-¤:
	rm -f $(§_¤_obj_files)
	rm -f $(§_¤_dep_files)
	rm -f $(§_¤_output_file_path)
	find build -type d 2> /dev/null | tac | xargs --no-run-if-empty rmdir --ignore-fail-on-non-empty

.PHONY: clean-¤ build-¤

include $(wildcard $(§_¤_dep_files))

