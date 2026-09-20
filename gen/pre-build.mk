# Makefile including autoconf build recipes

ifndef KERNEL_AUTOCONF_MK
KERNEL_AUTOCONF_MK := yes

##############################################
############# DIRECTORIES & FILES ############
#############################################

# Directories
PRE_BUILD_DIR	= $(BUILD_DIR)/kernel/conf

# Files
BSP_JSON 		= $(BSP_DIR)/bsp.json
AUTOCONF_SRC 	= $(PRE_BUILD_DIR)/autoconf.h
BSP_CONF_SRCS	= $(PRE_BUILD_DIR)/peripherals_conf.c \
				  $(PRE_BUILD_DIR)/system_peripherals_conf.c \
				  $(PRE_BUILD_DIR)/memories_conf.c
BSP_CONF_HEADERS = $(PRE_BUILD_DIR)/peripherals_conf.h \
				   $(PRE_BUILD_DIR)/system_peripherals_conf.h \
				   $(PRE_BUILD_DIR)/memories_conf.h
BSP_CONF_FILES = $(BSP_CONF_SRCS) $(BSP_CONF_HEADERS)
AUTOCONF_STAMP = $(PRE_BUILD_DIR)/autoconf.stamp
BSP_CONF_STAMP = $(PRE_BUILD_DIR)/bsp-conf.stamp
AUTOCONF_GENERATOR = $(TOOLS_DIR)/config-parser.py
BSP_CONF_GENERATOR = $(TOOLS_DIR)/bsp-parser.py

# The workspace may run this phase before invoking the kernel build. Standalone
# kernel builds still keep pre-build as an order-only prerequisite.
ifeq ($(KERNEL_PRE_BUILD_DONE),1)
KERNEL_PRE_BUILD_PREREQUISITE =
else
KERNEL_PRE_BUILD_PREREQUISITE = pre-build
endif

# A stamp alone cannot detect a manually removed generated file. Force the
# generator once when at least one declared output is missing.
ifeq ($(wildcard $(AUTOCONF_SRC)),)
.PHONY : autoconf-missing
$(AUTOCONF_STAMP) : autoconf-missing
endif
BSP_CONF_MISSING := $(filter-out $(wildcard $(BSP_CONF_FILES)),$(BSP_CONF_FILES))
ifneq ($(BSP_CONF_MISSING),)
.PHONY : bsp-conf-missing
$(BSP_CONF_STAMP) : bsp-conf-missing
endif

##############################################
################ BUILD RECIPES ###############
##############################################

.PHONY : pre-build pre-build-start autoconf conf-files pre-build-end
pre-build : pre-build-end
pre-build-end : autoconf conf-files
autoconf conf-files : | pre-build-start

define KERNEL_PRE_BUILD_START_VERBOSE
	@echo "$(BOLD)=============================$(RESET)"
	@echo "$(BOLD)===    KERNEL PRE BUILD   ===$(RESET)"
	@echo "$(BOLD)=============================$(RESET)"
	@echo "$(YELLOW)Files to pre-build:$(RESET) $(words $(AUTOCONF_SRC) $(BSP_CONF_FILES))"
	@echo "$(BLUE)Start pre-building...$(RESET)"
endef

define KERNEL_PRE_BUILD_END_VERBOSE
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"
	@echo ""
endef

# Pre-build header
pre-build-start :
	$(if $(or $(PARALLEL_BUILD),$(filter verif,$(MAKECMDGOALS))),$(QUIET_RECIPE),$(KERNEL_PRE_BUILD_START_VERBOSE))

# Autoconf recipes
autoconf : $(AUTOCONF_SRC)

$(AUTOCONF_SRC) : | $(AUTOCONF_STAMP)

$(AUTOCONF_STAMP) : $(CONFIG_FILE) $(AUTOCONF_GENERATOR)
	@echo "  PY  [kernel/generated] $(notdir $(AUTOCONF_SRC))"
	@mkdir -p $(@D)
	@${PYTHON} $(AUTOCONF_GENERATOR) -i $(CONFIG_FILE) -o $(@D)
	@stamp_tmp="$@.tmp.$$$$"; \
		printf '%s\n' $(notdir $(AUTOCONF_SRC)) > "$$stamp_tmp"; \
		mv -f "$$stamp_tmp" "$@"

# Configuration files generation
conf-files : $(BSP_CONF_FILES)
$(BSP_CONF_FILES) : | $(BSP_CONF_STAMP)

$(BSP_CONF_STAMP) : $(BSP_JSON) $(BSP_CONF_GENERATOR)
	@mkdir -p $(@D)
	@echo "  PY  [kernel/generated] peripherals_conf.c, peripherals_conf.h"
	@echo "  PY  [kernel/generated] system_peripherals_conf.c, system_peripherals_conf.h"
	@echo "  PY  [kernel/generated] memories_conf.c, memories_conf.h"
	@${PYTHON} $(BSP_CONF_GENERATOR) -i $(BSP_JSON) -o $(PRE_BUILD_DIR)
	@stamp_tmp="$@.tmp.$$$$"; \
		printf '%s\n' $(notdir $(BSP_CONF_FILES)) > "$$stamp_tmp"; \
		mv -f "$$stamp_tmp" "$@"

# Pre-build footer
pre-build-end :
	$(if $(or $(PARALLEL_BUILD),$(filter verif,$(MAKECMDGOALS))),$(QUIET_RECIPE),$(KERNEL_PRE_BUILD_END_VERBOSE))

# Pre-build clean recipes
pre-build-clean :
	@echo "$(BLUE)Cleaning pre-build directory...$(RESET)"
	@rm -rf $(PRE_BUILD_DIR)
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"

endif # KERNEL_AUTOCONF_MK #
