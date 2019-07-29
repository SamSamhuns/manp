IDIR =include# directory for header files
SDIR =src# source directory
ODIR =obj# object file directory
CPP =g++# compiler used
CFLAGS =-std=c++11 -Wall -Wshadow -Werror -I$(IDIR)# compiler flags
LDFLAGS =# -lm library flags
TARGET = manp # file executable generated
RAW_TXT_DIR =data/modules # location of the txt files to convert to raw string literals

# Getting the list of all cpp and object files
# except text_to_raw_string.cpp and raw_string_to_text
SOURCES := $(wildcard $(SDIR)/*.cpp)
SOURCES := $(filter-out src/text_to_raw_string.cpp, $(SOURCES))
SOURCES := $(filter-out src/raw_string_to_text.cpp, $(SOURCES))

OBJECTS := $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(SOURCES))

# TO manually list the header files
# _DEPS = common.h # dependency header files

# Getting the list of header files
HEADERS = $(wildcard $(IDIR)/*.h)

# To print the value of a variable
# $(info VAR is $(HEADERS))

# Pattern substitution
# $(patsubst pattern, substitution, text_to_insert)
# DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

all : text_to_raw_string raw_string_to_text run_text_to_raw_string run_combine_headers_sh run_generate_modules_list_sh $(TARGET)

$(ODIR)/text_to_raw_string.o: $(SDIR)/text_to_raw_string.cpp $(HEADERS)
	@mkdir -p $(@D)
	$(CPP) -c -o $@ $< $(CFLAGS)

text_to_raw_string: $(ODIR)/text_to_raw_string.o
	$(CPP) -o $@ $^ $(CFLAGS)

$(ODIR)/raw_string_to_text.o: $(SDIR)/raw_string_to_text.cpp $(HEADERS)
	@mkdir -p $(@D)
	$(CPP) -c -o $@ $< $(CFLAGS)

raw_string_to_text: $(ODIR)/raw_string_to_text.o
	$(CPP) -o $@ $^ $(CFLAGS)

# Generate the raw string literals which are required for compiling $(TARGET)
# The txt files must always be present in the data folder
run_text_to_raw_string: text_to_raw_string
	./text_to_raw_string $(RAW_TXT_DIR)

run_combine_headers_sh: run_text_to_raw_string
	chmod u+x combine_headers.sh
	./combine_headers.sh

run_generate_modules_list_sh: run_text_to_raw_string
	chmod u+x generate_modules_list.sh
	./generate_modules_list.sh

$(ODIR)/%.o: $(SDIR)/%.cpp $(HEADERS)
	@mkdir -p $(@D)
	$(CPP) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJECTS)
	$(CPP) -o $@ $^ $(CFLAGS)

# Any file with the name clean will not interrupt the cmd clean
.PHONY: clean all run_text_to_raw_string

clean: clean-obj clean-mac-fsys clean-build

clean-obj:
	rm -rf $(ODIR)

clean-mac-fsys:
	find . -name ".DS_Store" -delete

clean-build:
	rm -rf $(TARGET)
	rm -rf text_to_raw_string
	rm -rf raw_string_to_text
