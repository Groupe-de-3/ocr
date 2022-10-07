# ¤ is replaced by the executable's name

ifndef ¤_source_dirs 
$(error "INVALID MAKEFILE: Missing the ¤_source_dirs variable")
endif
ifndef ¤_executable_name
$(error "INVALID MAKEFILE: Missing the ¤_executable_name variable")
endif

¤_source_files != find $(¤_source_dirs) -name "*.c" -type f
P=(
M=)
$(eval ¤_rec_depedencies_with_duplicates := $(¤_depedencies) $(addprefix $$$P,$(addsuffix _rec_depedencies$M,$(¤_depedencies))))
¤_rec_depedencies := $(¤_rec_depedencies_with_duplicates)

$(eval ¤_depedencies_include_dirs := $(addprefix $$$P,$(addsuffix _source_dirs$M,$(sort $(¤_rec_depedencies)))))

format-¤:
	clang-format -i --style=file $(¤_source_files)
.PHONY: format-¤
