TOP_DIR = .

## includes

include config.mk


## subdirectories

DIRS = $(LIB_DIR) $(SRC_DIR) $(TOOLS_DIR) #$(AQUA-GEN_DIR)
ifdef HAVE_GLUT
DIRS += $(DEMO_DIR)
endif


## archive

# TAR = tar -jcvf
# TOP_DIR_NAME = Aqua
# ARCHIVE_NAME = libaqua_$$(date -I).tar.bz2
# ARCHIVE_EXCLUDE = --exclude .emacs.desktop --exclude *~  #  --exclude=''?


## main rules

all: $(DIRS)


#$(AQUA-GEN_DIR): $(LIB_DIR) $(SRC_DIR)
$(SRC_DIR): $(LIB_DIR)
$(DEMO_DIR): $(LIB_DIR) $(SRC_DIR)
$(TOOLS_DIR): $(LIB_DIR) $(SRC_DIR)


## (dirs/dep/objects/clean/phony) rules

include $(TOP_DIR)/config_rules.mk



# backup: clean
# 	cd $(TOP_DIR)/.. && \
# 	$(TAR) $(ARCHIVE_NAME) $(ARCHIVE_EXCLUDE) $(TOP_DIR_NAME)
