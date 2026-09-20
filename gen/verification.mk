# Makefile with verification rules

ifndef VERIFICATION_MK
VERIFICATION_MK := yes

##############################################
################ STATIC CHECK ################
##############################################

.PHONY : verif

CHECKER_ERROR_MSG = "\033[1;31mCode checked: errors have been found. Please correct them before continuing.\033[0m"

# Checker files and includes
CONF_MISRA = $(GEN_DIR)/MISRA/misra.json
CHECKER_SRCS = $(foreach component,$(KERNEL_COMPONENTS),$(wildcard $(KERNEL_COMPONENTS_DIR)/$(component)/src/*.c)) \
			   $(wildcard $(KERNEL_COMPONENTS_DIR)/drivers/src/*/wrapper-$(CHIP_VENDOR)/*.c) \
			   $(wildcard $(KERNEL_DIR)/bsp/$(BOARD)-BSP/src/*.c)
CHECKER_INCS = -I$(KERNEL_COMPONENTS_DIR) -I$(KERNEL_HEADERS) -I$(BSP_INCDIR) \
			   $(addprefix -I,$(KERNEL_PRIVATE_INCDIRS)) \
			   -I$(THIRD_PARTIES_CONFDIR) \
			   -I$(PRE_BUILD_DIR)
CHECKER_DEFS = -D$(CHIP) -D$(CHIP_FAMILLY)
CHECKER_LOGS = $(KERNEL_OBJDIR)/code-checking.log

# Add define indicating which core is used when dual core
ifdef CONFIG_DUAL_CORE
CHECKER_DEFS += -D$(CONFIG_CORE_SELECT)
endif

# Checker commands
CHECKER_CMDS  = --enable=all # Enables all warnings
CHECKER_CMDS += --suppress=missingInclude # Disables missing includes warnings
CHECKER_CMDS += --suppress=unusedFunction # Disables unused function warnings
CHECKER_CMDS += --inline-suppr # Allows to add checker suppress inside the code (inline)
CHECKER_CMDS += --addon=$(CONF_MISRA) # Check MISRA C compliancee if misra settings are added
CHECKER_CMDS += --output-file=$(CHECKER_LOGS) # Print the result in a log file
CHECKER_CMDS += --error-exitcode=1 # Returns 1 if cppcheck has encountered an error
CHECKER_CMDS += --suppress=misra-c2012-11.5 # Suppression of this rule because its often use to pass parameters for callbacks inside the kernel

# Checker recipes
verif : autoconf conf-files
	@mkdir -p $(KERNEL_OBJDIR)
	@$(CHECKER) $(CHECKER_CMDS) $(CHECKER_DEFS) $(CHECKER_INCS) $(CHECKER_SRCS) || (cat $(CHECKER_LOGS); echo $(CHECKER_ERROR_MSG) ; exit 1)

##############################################
################### FORMAT ###################
##############################################

.PHONY : format

format :
	$(TOOLS_DIR)/format-code.sh

endif # VERIFICATION_MK #