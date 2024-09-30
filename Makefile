#----------
# Compiler
#----------

CC = g++

#-------------
# Directories
#-------------

INCLUDES = includes
SRCDIR = src
BUILD = build
DEPDIR := $(BUILD)/deps

#-------
# Flags
#-------

CFLAGS = \
	-Wall \
	-Werror \
	-Wextra \
	-std=c++11 \
	-O0 \
	-fpermissive \
	-g \
	-lgtest \
	-pthread

# Add include directory
CFLAGS += -I $(abspath $(INCLUDES))

# Ask compiler for dependencies
DEPFLAGS = \
	-MT $@ \
	-MMD \
	-MP \
	-MF $(DEPDIR)/$*.Td

# move temporary depenendecy files to .d files
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

#--------------------------
# Colors (ANSI color codes)
#--------------------------

BRED    = \033[1;31m
BGREEN  = \033[1;32m
BYELLOW = \033[1;33m
GREEN   = \033[1;35m
BCYAN   = \033[1;36m
RESET   = \033[0m

#-------
# Files
#-------

RUN = main

SOURCES = $(notdir $(wildcard $(SRCDIR)/*.cpp))

# Object files:
RUN_OBJ = $(BUILD)/$(RUN).o
OBJECTS = $(filter-out $(RUN_OBJ), $(SOURCES:%.cpp=$(BUILD)/%.o))

# Executable files
RUN_EXECUTABLE = $(BUILD)/$(RUN)

#---------------
# Build process
#---------------

all: $(RUN_EXECUTABLE)

# Link object files together
$(RUN_EXECUTABLE) : $(RUN_OBJ) $(OBJECTS)
	@printf "$(BYELLOW)Linking executable test $(BCYAN)$@$(RESET)\n"
	$(CC) $(LDFLAGS) $^ -o $@


# Build object files
$(BUILD)/%.o: $(SRCDIR)/%.cpp
$(BUILD)/%.o: $(SRCDIR)/%.cpp $(DEPDIR)/%.d Makefile | $(DEPDIR)
	@printf "$(BYELLOW)Building object file $(BCYAN)$@$(RESET)\n"
	@mkdir -p $(BUILD)
	$(CC) $(DEPFLAGS) $(CFLAGS) $< -c -o $@
	$(POSTCOMPILE)

# Create directory
$(DEPDIR):
	@mkdir -p $(DEPDIR)

# Dependency files
DEPFILES := $(SOURCES:%.cpp=$(DEPDIR)/%.d)

# Mention every .d file as a target so that Make won't fail if it doesn't exist
$(DEPFILES):

# Include all created dependencies in current makefile
include $(wildcard $(DEPFILES))

#-------------------
# Passing arguments
#-------------------

ifeq (solved, $(firstword $(MAKECMDGOALS)))
  SOLVED_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(SOLVED_ARGS):;@:)
endif


ifeq (stash, $(firstword $(MAKECMDGOALS)))
  STASH_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(STASH_ARGS):;@:)
endif


#-----------------
# Run the program
#-----------------

run: $(RUN_EXECUTABLE) Makefile
	./$<

gdb: $(RUN_EXECUTABLE) Makefile
	gdb ./$<

SOLVED = solved

solved: all Makefile
	mkdir -p $(SOLVED)/
	cp	$(SRCDIR)/$(RUN).cpp $(SOLVED)/
	mv 	$(SOLVED)/$(RUN).cpp $(SOLVED)/$(SOLVED_ARGS).cpp
	git add $(SOLVED)/$(SOLVED_ARGS).cpp
	git commit -m "[AUTOMATIC] solved: $(SOLVED_ARGS)"
	git push origin master

STASH = stash

stash: all Makefile
	mkdir -p $(STASH)/
	cp	$(SRCDIR)/$(RUN).cpp $(STASH)/
	mv 	$(STASH)/$(RUN).cpp $(STASH)/$(STASH_ARGS).cpp
	git add $(STASH)/$(STASH_ARGS).cpp
	git commit -m "$(STASH): $(STASH_ARGS)"
	git push origin master

sync: all Makefile
	git add --all
	git commit -m "[AUTOMATIC] Synchronize remote and local"
	git pull origin master
	git merge -m "[AUTOMATIC] Merge remote and local" master origin/master
	git push origin master

#-------
# Other
#-------

clean:
	@printf "$(BYELLOW)Cleaning build and resource directories$(RESET)\n"
	rm -rf res
	rm -rf $(BUILD)
	rm -f programs/*.bcode

# List of non-file targets:
.PHONY: clean run solved stash sync push gdb