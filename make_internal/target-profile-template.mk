# § is replaced with the profile
# ¤ is replaced with the target's name

§_¤_obj_dir = build/§/¤/obj
§_¤_obj_files = $(patsubst %.c,$(§_¤_obj_dir)/%.o,$(¤_source_files))
§_¤_dep_files = $(patsubst %.c,$(§_¤_obj_dir)/%.d,$(¤_source_files))

§_¤_cflags = $(addprefix -I ,$(¤_depedencies_include_dirs) $(¤_source_dirs)) $(§_cflags) $(¤_cflags)
ifneq ($(¤_rec_pkgs),"")
§_¤_cflags += $(shell pkg-config --cflags --libs $(¤_rec_pkgs))
endif

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
§_¤_cflags += -fPIC
endif

# if ¤_link_in_deps is set to true, depedencies must be linked into the output
# file, so we add the required flags to §_¤_cflags
ifeq ($(¤_link_in_deps),true)
§_¤_depedencies_files = $(foreach tname,$(¤_rec_depedencies),\
	$(§_$(tname)_output_file_path) \
)

# Adding depedencies search paths and remove duplicates
# (which will happen if all depedencies are in the same folder)
§_¤_depedencies_flags = $(foreach tname,$(sort $(¤_rec_depedencies)),\
	-L$(§_$(tname)_output_folder) \
)
# Adding libraries
§_¤_depedencies_flags += $(addprefix -l,$(¤_rec_depedencies))

§_¤_cflags += $(§_¤_depedencies_flags)
endif

$(§_¤_obj_dir)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(§_¤_cflags) -MMD -c $< -o $@

$(§_¤_output_file_path): $$(§_¤_obj_files) $$(§_¤_depedencies_files)
	mkdir -p $(dir $@)
ifeq ($(¤_target_type),executable)
	$(CC) $(§_¤_cflags) -Wl,-rpath,'$$ORIGIN' $^ -o $@
else ifeq ($(¤_target_type),dynlib)
	$(CC) $(§_¤_cflags) -shared $^ -o $@
else
	ar rcs $@ $^
	ranlib $@
endif

build-§-¤: $$(§_¤_output_file_path)
clean-§-¤:
	rm -f $(§_¤_obj_files)
	rm -f $(§_¤_dep_files)
	rm -f $(§_¤_output_file_path)
	find build -type d 2> /dev/null | tac | xargs --no-run-if-empty rmdir --ignore-fail-on-non-empty
ifeq ($(¤_target_type),executable)
run-§-¤: $(§_¤_output_file_path)
	./$(§_¤_output_file_path) $(¤_run_arguments)
endif


.PHONY: clean-¤ build-¤ run-§-¤

include $(wildcard $(§_¤_dep_files))

