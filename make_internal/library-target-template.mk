# § is replaced by the profile
# ¤ is replaced by the library's name

ifndef ¤_source_dirs 
$(error "INVALID MAKEFILE: Missing the ¤_source_dirs variable")
endif
ifndef ¤_name
$(error "INVALID MAKEFILE: Missing the ¤_executable_name variable")
endif

§_¤_obj_dir = build/§/¤/obj
§_¤_obj_files = $(patsubst %.c,$(§_¤_obj_dir)/%.o,$(¤_source_files))
§_¤_dep_files = $(patsubst %.c,$(§_¤_obj_dir)/%.d,$(¤_source_files))

§_¤_cflags = $(addprefix -I ,$(¤_depedencies_include_dirs) $(¤_source_dirs)) $(§_cflags) $(¤_cflags)

§_¤_library_folder = build/§/¤
ifeq ($(¤_target_type),dynlib)
§_¤_library_file_path = $(§_¤_library_folder)/lib$(¤_name).so
§_¤_cflags += -fPIC
else
§_¤_library_file_path = $(§_¤_library_folder)/lib$(¤_name).a
endif

$(§_¤_obj_dir)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(§_¤_cflags) -MMD -c $< -o $@

$(§_¤_library_file_path): $(§_¤_obj_files)
	mkdir -p $(dir $@)
ifeq ($(¤_target_type),dynlib)
	$(CC) $(§_¤_cflags) -shared $^ -o $@
else
	ar rcs $@ $^
	ranlib $@
endif

build-§-¤: $(§_¤_library_file_path)
clean-§-¤:
	rm -f $(§_¤_obj_files)
	rm -f $(§_¤_dep_files)
	rm -f $(§_¤_library_file_path)
	find build -type d 2> /dev/null | tac | xargs --no-run-if-empty rmdir --ignore-fail-on-non-empty

.PHONY: clean-¤ build-¤

include $(wildcard $(§_¤_dep_files))

