include makefile_configuration.mk
# make can also be invoked as make TARGET_FILE_EXTN="py txt" SOURCE_DIR=custom dir

IDIR =include# directory for header files
SDIR =src# source directory
ODIR =obj# object file directory
CPP =g++# compiler used
CFLAGS =-std=c++11 -Wall -Wshadow -Werror -I$(IDIR)# compiler flags
LDFLAGS =# -lm library flags
TARGET =manp# file executable generated
# The TARGET_DIR where the files converted to raw string are created is set to target_data
TARGET_DIR =target_data# location of the files coverted to raw string literal format

# User defined variables loaded from makefile_configuration.mk
# SOURCE_DIR =         # location of the files to convert to raw string literal format
# TARGET_FILE_EXTN =   # extension of target files to document
# REPLACE_OLD_TARGET = # flag to decide if to replace old TARGET_DIR

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

all : text_to_raw_string raw_string_to_text run_text_to_raw_string generate_combined_artifacts_headers $(TARGET)

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
	@mkdir -p $(TARGET_DIR)
	if [ "${REPLACE_OLD_TARGET}" == "True" ]; then rm -fr ${TARGET_DIR}/*;fi;
	./text_to_raw_string $(TARGET_FILE_EXTN) $(SOURCE_DIR) $(TARGET_DIR)

# Generate the combined_txt_header and doc_artifacts_list file for header includes
generate_combined_artifacts_headers: run_text_to_raw_string
	chmod u+x generate_combined_artifacts_headers.sh
	./generate_combined_artifacts_headers.sh $(TARGET_DIR) $(TARGET_FILE_EXTN)

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
