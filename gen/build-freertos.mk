# Makefile including all build recipes for kernel third parties

ifndef BUILD_FREERTOS_MK
BUILD_FREERTOS_MK := yes

##############################################
############# DIRECTORIES & FILES ############
##############################################

# Directories
FREERTOS_DIR			= $(THIRD_PARTIES_DIR)/OS/FreeRTOS
FREERTOS_INCLUDES		= $(FREERTOS_DIR)/include
FREERTOS_SRCDIR			= $(FREERTOS_DIR)
FREERTOS_COMMON_DIR		= $(FREERTOS_DIR)/portable/Common
FREERTOS_MEMMANG_DIR	= $(FREERTOS_DIR)/portable/MemMang
FREERTOS_ARM_DIR		= $(FREERTOS_DIR)/portable/GCC/$(FREERTOS_PORTABLE)
FREERTOS_OBJDIR			= $(BUILD_DIR)/third-parties/freertos

# Files
FREERTOS_SRCS       = $(wildcard $(FREERTOS_SRCDIR)/*.c $(FREERTOS_ARM_DIR)/*.c $(FREERTOS_COMMON_DIR)/*.c $(FREERTOS_MEMMANG_DIR)/heap_1.c)
FREERTOS_OBJS       = $(subst $(FREERTOS_SRCDIR)/,$(FREERTOS_OBJDIR)/,$(FREERTOS_SRCS:.c=.o))
FREERTOS_LIB        = $(LIBS_DIR)/libfreertos.a
FREERTOS_FLAGS_FILE = $(BUILD_STATE_DIR)/freertos.flags

##############################################
#################### FLAGS ###################
##############################################

FREERTOS_CFLAGS		=	$(CFLAGS) \
						-D$(CHIP) -D$(CHIP_FAMILLY) $(CORE_SELECT) \
						-Wno-unused-variable -Wno-unused-parameter -Wno-pedantic
FREERTOS_INCDIRS	=	$(FREERTOS_INCLUDES) $(FREERTOS_ARM_DIR) $(THIRD_PARTIES_CONFDIR) \
						$(CMSIS_INCDIR) $(CMSIS_INCDIR_DEVICE) \
						$(PRE_BUILD_DIR)
FREERTOS_INCFLAGS	=	$(addprefix -I,$(FREERTOS_INCDIRS))

##############################################
################ BUILD RECIPES ###############
##############################################

.PHONY : freertos freertos-start freertos-end freertos-clean build-state-force
freertos : freertos-end
freertos-end : $(FREERTOS_LIB)
$(FREERTOS_OBJS) : | $(KERNEL_PRE_BUILD_PREREQUISITE) freertos-start
build-state-force :

$(FREERTOS_FLAGS_FILE) : build-state-force
	@mkdir -p $(@D)
	@state_tmp="$@.tmp.$$$$"; \
	{ \
		printf '%s\n' 'compiler=$(CC)'; \
		printf '%s\n' 'compiler_version=$(CC_VERSION)'; \
		printf '%s\n' 'archiver=$(AR)'; \
		printf '%s\n' 'cflags=$(FREERTOS_CFLAGS)'; \
		printf '%s\n' 'include_dirs=$(FREERTOS_INCDIRS)'; \
		printf '%s\n' 'sources=$(FREERTOS_SRCS)'; \
	} > "$$state_tmp"; \
	if cmp -s "$$state_tmp" "$@"; then rm -f "$$state_tmp"; else mv -f "$$state_tmp" "$@"; fi

define FREERTOS_START_VERBOSE
	@echo "$(BOLD)=============================$(RESET)"
	@echo "$(BOLD)===        FREERTOS       ===$(RESET)"
	@echo "$(BOLD)=============================$(RESET)"
	@echo "$(YELLOW)Files to compile:$(RESET) $(words $(FREERTOS_SRCS))"
	@echo "$(YELLOW)Compilation Flags:$(RESET)"
	@echo $(FREERTOS_CFLAGS)
	@echo "$(YELLOW)Include Paths:$(RESET)"
	@$(foreach dir,$(patsubst $(WORKSPACE)/%,%,$(FREERTOS_INCDIRS)),echo "  - $(dir)";)
	@echo "$(BLUE)Start building...$(RESET)"
endef

define FREERTOS_END_VERBOSE
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"
	@echo ""
endef

# Include dependencies
-include $(FREERTOS_OBJS:.o=.d)

# Build header
freertos-start :
	$(if $(PARALLEL_BUILD),$(QUIET_RECIPE),$(FREERTOS_START_VERBOSE))

# Building recipes
$(FREERTOS_OBJDIR)/%.o : $(FREERTOS_SRCDIR)/%.c $(FREERTOS_FLAGS_FILE)
	@echo "  CC  [kernel/freertos] $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(FREERTOS_CFLAGS) $(FREERTOS_INCFLAGS) $< -o $@

# Library generation
$(FREERTOS_LIB) : $(FREERTOS_OBJS) $(FREERTOS_FLAGS_FILE)
	@echo "  AR  [kernel/freertos] $(@F)"
	@mkdir -p $(@D)
	@rm -f $@
	@$(AR) rcs $@ $(FREERTOS_OBJS)

# Build footer
freertos-end :
	$(if $(PARALLEL_BUILD),$(QUIET_RECIPE),$(FREERTOS_END_VERBOSE))

# Clean recipe
freertos-clean :
	@printf "$(BLUE)Cleaning FREERTOS build directory...$(RESET)"
	@rm -rf $(FREERTOS_OBJDIR)
	@rm -rf $(FREERTOS_LIB)
	@rm -f $(FREERTOS_FLAGS_FILE)
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"

endif # BUILD_FREERTOS_MK #
