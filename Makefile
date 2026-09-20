# Kernel Makefile

##############################################
################ CONFIGURATION ###############
##############################################

# Kernel Version
MAJOR = 2
MINOR = 7
PATCH = 6

##############################################
################### MAKE #####################
##############################################

.PHONY : all clean

# Main recipe
all : pre-build hal fatfs freertos kernel

clean : pre-build-clean hal-clean fatfs-clean freertos-clean kernel-clean

print-%:
	@printf '%s = %s\n' '$*' '$($*)'

##############################################
################## INCLUDES ##################
##############################################

include gen/paths.mk
include gen/configuration.mk
include gen/pre-build.mk
include gen/build-hal.mk
include gen/build-fatfs.mk
include gen/build-freertos.mk
include gen/build-kernel.mk
include gen/verification.mk
include gen/help.mk

##############################################
################## COLORS ####################
##############################################

# Colors for terminal output
GREEN = \033[92m
YELLOW = \033[93m
RED = \033[91m
BLUE = \033[94m

# Fonts settings
BOLD = \033[1m
NORMAL = \033[0m
ITALIC = \033[3m
UNDERLINE = \033[4m

# Reset effect
RESET = \033[0m
