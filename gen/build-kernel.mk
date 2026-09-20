# Makefile including all build recipes for kernel

ifndef BUILD_KERNEL_MK
BUILD_KERNEL_MK := yes

##############################################
############# DIRECTORIES & FILES ############
##############################################

# Directories
KERNEL_OBJDIR	= $(BUILD_DIR)/kernel

# Files
KERNEL_COMPONENTS = core drivers fdir file-system monitoring platform time
KERNEL_SRCS       = $(foreach component,$(KERNEL_COMPONENTS),$(wildcard $(KERNEL_COMPONENTS_DIR)/$(component)/src/*.c)) \
			        $(wildcard $(KERNEL_COMPONENTS_DIR)/drivers/src/*/wrapper-$(CHIP_VENDOR)/*.c) \
			        $(wildcard $(KERNEL_DIR)/bsp/$(BOARD)-BSP/src/*.c) \
			        $(BSP_CONF_SRCS)
KERNEL_OBJS       = $(patsubst $(KERNEL_DIR)/%.c,$(KERNEL_OBJDIR)/%.o, \
			        $(patsubst $(PRE_BUILD_DIR)/%.c,$(PRE_BUILD_DIR)/%.o, \
			        $(KERNEL_SRCS)))
KERNEL_LIB 	      = $(LIBS_DIR)/libkernel.a
KERNEL_FLAGS_FILE = $(BUILD_STATE_DIR)/kernel.flags

##############################################
#################### FLAGS ###################
##############################################

# System defines
SYSTEM_DEFINES  = -DSYSTEM_NAME=\"TAPAS\"
SYSTEM_DEFINES += -DPROGRAM_NAME=\"$(PROJ_NAME)\"
SYSTEM_DEFINES += -DMAJOR=$(MAJOR)
SYSTEM_DEFINES += -DMINOR=$(MINOR)
SYSTEM_DEFINES += -DPATCH=$(PATCH)
SYSTEM_DEFINES += -DBOARD=\"$(BOARD)\"

# Flags
KERNEL_CFLAGS   = 	$(CFLAGS) \
				   	-D$(CHIP) -D$(CHIP_FAMILLY) $(CORE_SELECT)
KERNEL_INCDIRS  =	$(KERNEL_HEADERS) $(KERNEL_COMPONENTS_DIR) $(BSP_INCDIR) \
					$(FREERTOS_INCLUDES) $(FREERTOS_ARM_DIR) \
					$(HAL_INCDIR) $(HAL_INCDIR)/Legacy \
					$(FATFS_INCDIR) \
					$(CMSIS_INCDIR) $(CMSIS_INCDIR_DEVICE) \
					$(THIRD_PARTIES_CONFDIR) \
					$(PRE_BUILD_DIR)
KERNEL_PRIVATE_INCDIRS = $(foreach component,$(KERNEL_COMPONENTS),$(wildcard $(KERNEL_COMPONENTS_DIR)/$(component)/inc))
KERNEL_INCFLAGS = $(addprefix -I,$(KERNEL_INCDIRS))

##############################################
################ BUILD RECIPES ###############
##############################################

.PHONY : kernel kernel-start kernel-end kernel-clean build-state-force
kernel : kernel-end
kernel-end : $(KERNEL_LIB)
$(KERNEL_OBJS) : | $(KERNEL_PRE_BUILD_PREREQUISITE) kernel-start
build-state-force :

$(KERNEL_FLAGS_FILE) : build-state-force
	@mkdir -p $(@D)
	@state_tmp="$@.tmp.$$$$"; \
	{ \
		printf '%s\n' 'compiler=$(CC)'; \
		printf '%s\n' 'compiler_version=$(CC_VERSION)'; \
		printf '%s\n' 'archiver=$(AR)'; \
		printf '%s\n' 'cflags=$(KERNEL_CFLAGS)'; \
		printf '%s\n' 'system_defines=$(SYSTEM_DEFINES)'; \
		printf '%s\n' 'include_dirs=$(KERNEL_INCDIRS)'; \
		printf '%s\n' 'private_include_dirs=$(KERNEL_PRIVATE_INCDIRS)'; \
		printf '%s\n' 'sources=$(KERNEL_SRCS)'; \
	} > "$$state_tmp"; \
	if cmp -s "$$state_tmp" "$@"; then rm -f "$$state_tmp"; else mv -f "$$state_tmp" "$@"; fi

define KERNEL_START_VERBOSE
	@echo "$(BOLD)=============================$(RESET)"
	@echo "$(BOLD)===         KERNEL        ===$(RESET)"
	@echo "$(BOLD)=============================$(RESET)"
	@echo "$(YELLOW)Kernel Version:$(RESET) v$(MAJOR).$(MINOR).$(PATCH)"
	@echo "$(YELLOW)Files to compile:$(RESET) $(words $(KERNEL_SRCS))"
	@echo "$(YELLOW)Compilation Flags:$(RESET)"
	@echo $(KERNEL_CFLAGS)
	@echo "$(YELLOW)Include Paths:$(RESET)"
	@$(foreach dir,$(patsubst $(WORKSPACE)/%,%,$(KERNEL_INCDIRS)),echo "  - $(dir)";)
	@$(foreach dir,$(patsubst $(WORKSPACE)/%,%,$(KERNEL_PRIVATE_INCDIRS)),echo "  - $(dir) (private)";)
	@echo "$(BLUE)Start building...$(RESET)"
endef

define KERNEL_END_VERBOSE
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"
	@echo ""
endef

# Include dependencies
-include $(KERNEL_OBJS:.o=.d)

# Build header
kernel-start :
	$(if $(PARALLEL_BUILD),$(QUIET_RECIPE),$(KERNEL_START_VERBOSE))

# Building recipes
$(KERNEL_OBJDIR)/bsp/%.o : $(KERNEL_DIR)/bsp/%.c $(KERNEL_FLAGS_FILE)
	@echo "  CC  [kernel/bsp] $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(KERNEL_CFLAGS) $(KERNEL_INCFLAGS) $< -o $@

define KERNEL_COMPONENT_RULE
$(KERNEL_OBJDIR)/components/$(1)/src/%.o : $(KERNEL_COMPONENTS_DIR)/$(1)/src/%.c $(KERNEL_FLAGS_FILE)
	@echo "  CC  [kernel/$(1)] $$(@F)"
	@mkdir -p $$(@D)
	@$(CC) $(KERNEL_CFLAGS) $(KERNEL_INCFLAGS) $(if $(wildcard $(KERNEL_COMPONENTS_DIR)/$(1)/inc),-iquote $(KERNEL_COMPONENTS_DIR)/$(1)/inc) $$< -o $$@
endef

$(foreach component,$(KERNEL_COMPONENTS),$(eval $(call KERNEL_COMPONENT_RULE,$(component))))

$(PRE_BUILD_DIR)/%.o  : $(PRE_BUILD_DIR)/%.c $(KERNEL_FLAGS_FILE)
	@echo "  CC  [kernel/generated] $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(KERNEL_CFLAGS) $(KERNEL_INCFLAGS) $< -o $@

$(KERNEL_OBJDIR)/components/monitoring/src/info.o : $(KERNEL_COMPONENTS_DIR)/monitoring/src/info.c $(KERNEL_FLAGS_FILE)
	@echo "  CC  [kernel/monitoring] $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(KERNEL_CFLAGS) $(SYSTEM_DEFINES) $(KERNEL_INCFLAGS) $(if $(wildcard $(KERNEL_COMPONENTS_DIR)/monitoring/inc),-iquote $(KERNEL_COMPONENTS_DIR)/monitoring/inc) $< -o $@

# Library generation
$(KERNEL_LIB) : $(KERNEL_OBJS) $(KERNEL_FLAGS_FILE)
	@echo "  AR  [kernel] $(@F)"
	@mkdir -p $(@D)
	@rm -f $@
	@$(AR) rcs $@ $(KERNEL_OBJS)

# Build footer
kernel-end :
	$(if $(PARALLEL_BUILD),$(QUIET_RECIPE),$(KERNEL_END_VERBOSE))

# Clean recipe
kernel-clean :
	@printf "$(BLUE)Cleaning KERNEL build directory...$(RESET)"
	@rm -rf $(KERNEL_OBJDIR)
	@rm -rf $(KERNEL_LIB)
	@rm -f $(KERNEL_FLAGS_FILE)
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"

endif # BUILD_KERNEL_MK #
