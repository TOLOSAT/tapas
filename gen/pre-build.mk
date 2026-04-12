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
	@echo "$(YELLOW)Files to pre-build:$(RESET) $(words $(AUTOCONF_SRC) $(BSP_CONF_SRCS))"
	@echo "$(BLUE)Start pre-building...$(RESET)"
endef

define KERNEL_PRE_BUILD_END_VERBOSE
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"
	@echo ""
endef

# Pre-build header
pre-build-start :
	$(if $(PARALLEL_BUILD),$(QUIET_RECIPE),$(KERNEL_PRE_BUILD_START_VERBOSE))

# Autoconf recipes
autoconf : $(AUTOCONF_SRC)

$(AUTOCONF_SRC) : $(CONFIG_FILE)
	@echo "  PY  $(@F)"
	@mkdir -p $(@D)
	@${PYTHON} $(TOOLS_DIR)/config-parser.py -i $^ -o $(@D)

# Configuration files generation
conf-files : $(PRE_BUILD_DIR)/bsp-conf.stamp
$(BSP_CONF_SRCS): $(PRE_BUILD_DIR)/bsp-conf.stamp

$(PRE_BUILD_DIR)/bsp-conf.stamp : $(BSP_JSON)
	@mkdir -p $(@D)
	@echo "  PY  peripherals_conf.c, peripherals_conf.h"; echo "peripherals_conf.c, peripherals_conf.h" >> $@
	@echo "  PY  system_peripherals_conf.c, system_peripherals_conf.h"; echo "system_peripherals_conf.c, system_peripherals_conf.h" >> $@
	@echo "  PY  memories_conf.c, memories_conf.h"; echo "memories_conf.c, memories_conf.h" >> $@
	@${PYTHON} $(TOOLS_DIR)/bsp-parser.py -i $(BSP_JSON) -o $(PRE_BUILD_DIR)

# Pre-build footer
pre-build-end :
	$(if $(PARALLEL_BUILD),$(QUIET_RECIPE),$(KERNEL_PRE_BUILD_END_VERBOSE))

# Pre-build clean recipes
pre-build-clean :
	@echo "$(BLUE)Cleaning pre-build directory...$(RESET)"
	@rm -rf $(PRE_BUILD_DIR)
	@echo "$(BOLD)$(GREEN)Done.$(RESET)"

endif # KERNEL_AUTOCONF_MK #
