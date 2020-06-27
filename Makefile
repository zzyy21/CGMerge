# ################################################################
# FilePath     : /Makefile
# Project Name : CGMerge
# Author       : zzyy21
# Create Time  : 2020-06-27 19:07:46
# Modifed by   : zzyy21
# Last Modify  : 2020-06-27 19:07:46
# Description  : Makefile for MinGW make on Windows
# Revision     : 
# ################################################################

default_target: all
.PHONY : default_target

CC = g++

STD_FLAG = c++11

TARGET = CGMerge

BUILD_PATH = build

SOURCE_PATH = src

INCLUDE_PATH = include

COMPILE_FLAGS = -Wall -Wextra -g -static


SOURCES = $(shell for %%i in ($(SOURCE_PATH)\*) do echo %%i)

all:
	if not exist "$(BUILD_PATH)" mkdir "$(BUILD_PATH)"
	$(CC) -o $(BUILD_PATH)\$(TARGET) -I$(INCLUDE_PATH) $(SOURCES) -std=$(STD_FLAG) $(COMPILE_FLAGS)

.PHONY :clean
clean:
	rd /s /q $(BUILD_PATH)
