# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# The next 2 lines (only 2 ) need to changed by the developer to reflect
# the projects location on his machine. This needs to be done 
# whenever the repository is cloned into a new loation ...
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

SANDBOX     := /media/ext/argos
PRJ_NAME    := argos

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Definition of tools and default settings for the project
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

GCC         = gcc
GPP         = g++

OPTIMIZE    = -O0
DEBUG       = -g
WARNINGS    = -Wall
CFLAGS      = $(WARNINGS) $(DEBUG) $(OPTIMIZE) -fPIC -I $(INC_DIR)
DEPFLAGS    = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.Td
LIBS        = -L $(LIB_DIR)/SHARED 

AR          = ar
ARFLAGS     = -rc

COMPILER    = $(GCC)


# &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
#
# --------------- Change NOTHING below here  --------------------------
#
# &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
PRJ_DIR     := $(SANDBOX)/$(PRJ_NAME)
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# The next veriable is defined to show a source code file's position
# in the source tree. This is useful so that the same structure can
# be used for the object modules and the dependency files, In other
# words the .o and .d files mimic the tree structure of the source.
# Then files with the same name as other artifacts can be kept 
# separate and don't end up in the wrong libraries or linked into 
# the wrong executable.
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
TREE         = $(subst $(PRJ_DIR)/SOURCE,,$(realpath .))
DEP_DIR      = $(PRJ_DIR)/DEPENDS$(TREE)
OBJ_DIR      = $(PRJ_DIR)/OBJECT$(TREE)
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Binary products built by the system. 
# NOTHING in these directories should be versioned because they
# can be re-built easily but the (empty) structure should be kept
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BIN_DIR     := $(PRJ_DIR)/BIN
LIB_DIR     := $(PRJ_DIR)/LIB
3RD_DIR     := $(PRJ_DIR)/THIRD_PARTY

# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# All the artifacts in the following directories should be version
# Controlled since they are the product of work and labour and are
# not easily replaceable if not versioned.
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
SRC_DIR     := $(PRJ_DIR)/SOURCE
INC_DIR     := $(SRC_DIR)/INCLUDE
DAT_DIR     := $(PRJ_DIR)/DAT
DOC_DIR     := $(PRJ_DIR)/DOCUMENTATION

