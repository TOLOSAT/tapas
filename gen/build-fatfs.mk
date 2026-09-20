# Copyright (c) TOLOSAT 2026
# SPDX-License-Identifier: Apache-2.0

# Makefile including all build recipes for kernel third parties

ifndef BUILD_FATFS_MK
BUILD_FATFS_MK := yes

##############################################
############# DIRECTORIES & FILES ############
##############################################

# Directories
FATFS_DIR		= $(THIRD_PARTIES_DIR)/FS/FatFs
FATFS_INCDIR	= $(FATFS_DIR)/source
FATFS_SRCDIR	= $(FATFS_DIR)/source
FATFS_OBJDIR	= $(BUILD_DIR)/third-parties/fatfs

# Files
FATFS_SRCS       = $(filter-out $(FATFS_SRCDIR)/ffsystem_%.c, $(wildcard $(FATFS_SRCDIR)/*.c))
FATFS_OBJS       = $(subst $(FATFS_SRCDIR)/,$(FATFS_OBJDIR)/,$(FATFS_SRCS:.c=.o))
FATFS_LIB        = $(LIBS_DIR)/libfatfs.a
FATFS_FLAGS_FILE = $(BUILD_STATE_DIR)/fatfs.flags

##############################################
#################### FLAGS ###################
##############################################

FATFS_CFLAGS	=	$(CFLAGS) \
					-D$(CHIP) -D$(CHIP_FAMILLY) $(CORE_SELECT) \
					-Wno-unused-variable -Wno-unused-parameter -Wno-stringop-overflow -Wno-unused-function
FATFS_INCDIRS	=	$(FATFS_INCDIR) $(THIRD_PARTIES_CONFDIR) \
					$(CMSIS_INCDIR) $(CMSIS_INCDIR_DEVICE) \
					$(PRE_BUILD_DIR)
FATFS_INCFLAGS	=	$(addprefix -I,$(FATFS_INCDIRS))

##############################################
################ BUILD RECIPES ###############
##############################################

.PHONY : fatfs fatfs-start fatfs-end fatfs-clean build-state-force
fatfs : fatfs-end
fatfs-end : $(FATFS_LIB)
$(FATFS_OBJS) : | $(KERNEL_PRE_BUILD_PREREQUISITE) fatfs-start
build-state-force :

$(FATFS_FLAGS_FILE) : build-state-force
	@mkdir -p $(@D)
	@state_tmp="$@.tmp.$$$$"; \
	{ \
		printf '%s\n' 'compiler=$(CC)'; \
		printf '%s\n' 'compiler_version=$(CC_VERSION)'; \
		printf '%s\n' 'archiver=$(AR)'; \
		printf '%s\n' 'cflags=$(FATFS_CFLAGS)'; \
		printf '%s\n' 'include_dirs=$(FATFS_INCDIRS)'; \
		printf '%s\n' 'sources=$(FATFS_SRCS)'; \
	} > "$$state_tmp"; \
	if cmp -s "$$state_tmp" "$@"; then rm -f "$$state_tmp"; else mv -f "$$state_tmp" "$@"; fi

define FATFS_START_VERBOSE
	@echo "$(BOLD)=============================$(RESET)"
	@echo "$(BOLD)===         FATFS         ===$(RESET)"
	@echo "$(BOLD)=============================$(RESET)"
	@echo "$(YELLOW)Files to compile:$(RESET) $(words $(FATFS_SRCS))"
	@echo "$(YELLOW)Compilation Flags:$(RESET)"
	@echo $(FATFS_CFLAGS)
	@echo "$(YELLOW)Include Paths:$(RESET)"
	@$(foreach dir,$(patsubst $(WORKSPACE)/%,%,$(FATFS_INCDIRS)),echo "  - $(dir)";)
	@echo "$(BLUE)Start building...$(RESET)"
endef

define FATFS_END_VERBOSE
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"
	@echo ""
endef

# Include dependencies
-include $(FATFS_OBJS:.o=.d)

# Build header
fatfs-start :
	$(if $(PARALLEL_BUILD),$(QUIET_RECIPE),$(FATFS_START_VERBOSE))

# Building recipes
$(FATFS_OBJDIR)/%.o : $(FATFS_SRCDIR)/%.c $(FATFS_FLAGS_FILE)
	@echo "  CC  [kernel/fatfs] $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(FATFS_CFLAGS) $(FATFS_INCFLAGS) $< -o $@

# Library generation
$(FATFS_LIB) : $(FATFS_OBJS) $(FATFS_FLAGS_FILE)
	@echo "  AR  [kernel/fatfs] $(@F)"
	@mkdir -p $(@D)
	@rm -f $@
	@$(AR) rcs $@ $(FATFS_OBJS)

# Build footer
fatfs-end :
	$(if $(PARALLEL_BUILD),$(QUIET_RECIPE),$(FATFS_END_VERBOSE))

# Clean recipe
fatfs-clean :
	@printf "$(BLUE)Cleaning FATFS build directory...$(RESET)"
	@rm -rf $(FATFS_OBJDIR)
	@rm -rf $(FATFS_LIB)
	@rm -f $(FATFS_FLAGS_FILE)
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"

endif # BUILD_FATFS_MK #
