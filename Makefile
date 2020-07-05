# ################################################################
# FilePath     : Makefile
# Project Name : CGMerge
# Author       : zzyy21
# Create Time  : 2020-06-27 19:07:46
# Modifed by   : zzyy21
# Last Modify  : 2020-07-05 18:35:29
# Description  : Makefile for MinGW make on Windows
# Revision     : v1.0 - first release
#                v2.0 - Updated directory structure and Makefile 
#                       for integrating third-party libraries
# ################################################################

.PHONY : default_target
default_target: exe-static

# rules for build all
# choose item for build all
.PHONY :all
all: exe exe-static

# build target filename
TARGET = CGMerge

# compiler and cpp standard setting
CC = g++
STD_FLAG = c++11

# clean command, set according to shell
CLEAN_CMD = del /q

# file extension definition
# source file
SOURCE_FILEEXT = cpp
# dynamic link library
DLIB_FILEEXT = dll
# static library
SLIB_FILEEXT = a

# set extra comple flags
COMPILE_FLAGS = -Wall -Wextra -g

# extra include directory
# format: <path_to_include1> <path_to_include2>
# example: EXTRA_INCLUDE_PATH = foo/include bar/include
EXTRA_INCLUDE_PATH = 

# extra library directory
# format: <path_to_library1> <path_to_library2>
# example: EXTRA_LIBRARY_PATH = foo/lib bar/lib
EXTRA_LIBRARY_PATH = 

# extra libraries
# format: <library1_name> <library2_name>
# example: EXTRA_LIBRARIES = foo bar
EXTRA_LIBRARIES = tlg2png png zlib

# project path definition
BUILD_PATH = build
OBJECT_PATH = $(BUILD_PATH)/obj
PUBLIC_INCLUDE_PATH = include
SOURCE_PATH = src


###################################################
#   Generally no need to change the lines below   #
###################################################

# file list
SOURCES = $(wildcard $(SOURCE_PATH)/*.$(SOURCE_FILEEXT))
OBJECTS = $(patsubst %.$(SOURCE_FILEEXT),$(OBJECT_PATH)/%.o, $(notdir $(SOURCES)))

# 3rdparty library auto import
LIBRARY_PATH = 3rdparty/lib
LIBRARY_INCLUDE_PATH = 3rdparty/include
#LIBRARIES = $(wildcard $(LIBRARY_PATH)/*)


################ generate compile flags ################
# include path link flags -I
INCLUDE_DIRS = $(EXTRA_INCLUDE_PATH) $(PUBLIC_INCLUDE_PATH) $(LIBRARY_INCLUDE_PATH)
IFLAGS = ${foreach IFLAG,${INCLUDE_DIRS},-I${IFLAG}}

# library path link flags -L
LIB_DIRS = $(EXTRA_LIBRARY_PATH) $(LIBRARY_PATH)
LDFLAGS = ${foreach LDFLAG,${LIB_DIRS},-L${LDFLAG}}

LIBS = $(EXTRA_LIBRARIES) $(patsubst lib%,%, $(basename $(notdir $(LIBRARIES))))
LLFLAGS = ${foreach LLFLAG,${LIBS},-l${LLFLAG}}

# general compile flags
CFLAGS = -std=$(STD_FLAG) $(COMPILE_FLAGS)


################ build rules ################

# rules for build dynamic link executable file
.PHONY :exe
exe: $(BUILD_PATH)/$(TARGET).exe
$(BUILD_PATH)/$(TARGET).exe: $(OBJECTS)
	$(CC) -o $(BUILD_PATH)/$(TARGET).exe $(OBJECTS) $(LDFLAGS) $(LLFLAGS) $(CFLAGS)

# rules for build static executable file
.PHONY :exe-static
exe-static: $(BUILD_PATH)/$(TARGET)_static.exe
$(BUILD_PATH)/$(TARGET)_static.exe: $(OBJECTS)
	$(CC) -static -o $(BUILD_PATH)/$(TARGET)_static.exe $(OBJECTS) $(LDFLAGS) $(LLFLAGS) $(CFLAGS)

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

# rules for build objective
$(OBJECT_PATH)/%.o: $(SOURCE_PATH)/%.$(SOURCE_FILEEXT)
	$(CC) -c -o $@ $< $(IFLAGS) $(CFLAGS)
	$(CC) -MM -MP -MT $@ -o $(@:.o=.d) $< $(IFLAGS) $(CFLAGS)


# print all flags, sources, libs, etc. for check
.PHONY :check
check:
	@echo Source files:  $(SOURCES)
	@echo Include flags: $(IFLAGS)
	@echo Lib dir flags: $(LDFLAGS)
	@echo Library flags: $(LLFLAGS)
	@echo Compile flags: $(CFLAGS)


# delete all builded things
.PHONY :clean
clean:
	cd $(BUILD_PATH) && $(CLEAN_CMD) *.exe
	cd $(OBJECT_PATH) && $(CLEAN_CMD) *.o
	cd $(OBJECT_PATH) && $(CLEAN_CMD) *.d
