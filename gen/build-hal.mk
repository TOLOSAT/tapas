# Makefile including all build recipes for kernel third parties

ifndef BUILD_HAL_MK
BUILD_HAL_MK := yes

##############################################
############# DIRECTORIES & FILES ############
##############################################

# Directories
HALs_DIR		= $(THIRD_PARTIES_DIR)/HALs
HAL_DIR			= $(HALs_DIR)/HAL-$(CHIP_FAMILLY)
HAL_INCDIR		= $(HAL_DIR)/Inc
HAL_SRCDIR		= $(HAL_DIR)/Src
HAL_OBJDIR		= $(BUILD_DIR)/third-parties/hal

# Files
include $(THIRD_PARTIES_CONFDIR)/HAL_SRCS_$(CHIP_FAMILLY).mk
HAL_OBJS       = $(subst $(HAL_SRCDIR)/,$(HAL_OBJDIR)/,$(HAL_SRCS:.c=.o))
HAL_LIB        = $(LIBS_DIR)/libhal.a
HAL_FLAGS_FILE = $(BUILD_STATE_DIR)/hal.flags

##############################################
#################### FLAGS ###################
##############################################

HAL_CFLAGS		=	$(CFLAGS) \
					-D$(CHIP) -D$(CHIP_FAMILLY) $(CORE_SELECT) \
					-Wno-unused-variable -Wno-unused-parameter
HAL_INCDIRS		=	$(HAL_INCDIR) $(HAL_INCDIR)/Legacy $(THIRD_PARTIES_CONFDIR) \
					$(CMSIS_INCDIR) $(CMSIS_INCDIR_DEVICE) \
					$(PRE_BUILD_DIR)
HAL_INCFLAGS	=	$(addprefix -I,$(HAL_INCDIRS))

##############################################
################ BUILD RECIPES ###############
##############################################

.PHONY : hal hal-start hal-end hal-clean build-state-force
hal : hal-end
hal-end : $(HAL_LIB)
$(HAL_OBJS) : | $(KERNEL_PRE_BUILD_PREREQUISITE) hal-start
build-state-force :

$(HAL_FLAGS_FILE) : build-state-force
	@mkdir -p $(@D)
	@state_tmp="$@.tmp.$$$$"; \
	{ \
		printf '%s\n' 'compiler=$(CC)'; \
		printf '%s\n' 'compiler_version=$(CC_VERSION)'; \
		printf '%s\n' 'archiver=$(AR)'; \
		printf '%s\n' 'cflags=$(HAL_CFLAGS)'; \
		printf '%s\n' 'include_dirs=$(HAL_INCDIRS)'; \
		printf '%s\n' 'sources=$(HAL_SRCS)'; \
	} > "$$state_tmp"; \
	if cmp -s "$$state_tmp" "$@"; then rm -f "$$state_tmp"; else mv -f "$$state_tmp" "$@"; fi

define HAL_START_VERBOSE
	@echo "$(BOLD)=============================$(RESET)"
	@echo "$(BOLD)===          HAL          ===$(RESET)"
	@echo "$(BOLD)=============================$(RESET)"
	@echo "$(YELLOW)Files to compile:$(RESET) $(words $(HAL_SRCS))"
	@echo "$(YELLOW)Compilation Flags:$(RESET)"
	@echo $(HAL_CFLAGS)
	@echo "$(YELLOW)Include Paths:$(RESET)"
	@$(foreach dir,$(patsubst $(WORKSPACE)/%,%,$(HAL_INCDIRS)),echo "  - $(dir)";)
	@echo "$(BLUE)Start building...$(RESET)"
endef

define HAL_END_VERBOSE
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"
	@echo ""
endef

# Include dependencies
-include $(HAL_OBJS:.o=.d)

# Build header
hal-start :
	$(if $(PARALLEL_BUILD),$(QUIET_RECIPE),$(HAL_START_VERBOSE))

# Building recipes
$(HAL_OBJDIR)/%.o : $(HAL_SRCDIR)/%.c $(HAL_FLAGS_FILE)
	@echo "  CC  [kernel/hal] $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(HAL_CFLAGS) $(HAL_INCFLAGS) $< -o $@

# Library generation
$(HAL_LIB) : $(HAL_OBJS) $(HAL_FLAGS_FILE)
	@echo "  AR  [kernel/hal] $(@F)"
	@mkdir -p $(@D)
	@rm -f $@
	@$(AR) rcs $@ $(HAL_OBJS)

# Build footer
hal-end :
	$(if $(PARALLEL_BUILD),$(QUIET_RECIPE),$(HAL_END_VERBOSE))

# Clean recipe
hal-clean :
	@printf "$(BLUE)Cleaning HAL build directory...$(RESET)"
	@rm -rf $(HAL_OBJDIR)
	@rm -rf $(HAL_LIB)
	@rm -f $(HAL_FLAGS_FILE)
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"

endif # BUILD_HAL_MK #
