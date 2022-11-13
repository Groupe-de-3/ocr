# ¤ is replaced by the target's name

ifndef ¤_source_dirs 
$(error "INVALID MAKEFILE: Missing the ¤_source_dirs variable")
endif
ifndef ¤_name
$(error "INVALID MAKEFILE: Missing the ¤_name variable")
endif
ifndef ¤_target_type
$(error "INVALID MAKEFILE: Missing the ¤_target_type variable")
endif
ifneq ($(¤_target_type),executable)
ifneq ($(¤_target_type),dynlib)
ifneq ($(¤_target_type),staticlib)
$(error "INVALID MAKEFILE: Invalid variable ¤_target_type, accepted values are\
		'executable', 'dynlib' or 'staticlib', given $(¤_target_type)")
endif
endif
endif

¤_source_files != find $(¤_source_dirs) -name "*.c" -type f \
				  -not -path "$(¤_ignore)"
¤_header_files != find $(¤_source_dirs) -name "*.h" -type f

# Could not find how to escape parenthesis x)
P=(
M=)
¤_rec_depedencies += $(¤_depedencies) 
¤_rec_depedencies += $(foreach target,$(¤_depedencies), \
	$($(target)_rec_depedencies) \
)

¤_rec_pkgs += $(sort $(¤_pkgs) $(foreach target,$(¤_depedencies), \
	$($(target)_pkgs) \
))

¤_depedencies_include_dirs = $(foreach target,$(sort $(¤_rec_depedencies)), \
	$($(target)_source_dirs) \
)

¤: build-$(DEFAULT_PROFILE)-¤

format-¤:
	clang-format -i --style=file $(¤_source_files) $(¤_header_files)
.PHONY: ¤ format-¤
