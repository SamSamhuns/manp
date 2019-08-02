# Configuration file for setting up the Makefile
# By default,
#          SOURCE_DIR = source_data
#          TARGET_FILE_EXTN = txt
#		   REPLACE_OLD_TARGET = True

# SOURCE_DIR is the location of the files to convert to raw string literal format
# These can be changed to the user's requirements
# The SOURCE_DIR should be an ABSOLUTE PATH unless the data source has been moved to `manp/`
SOURCE_DIR = source_data

# TARGET_FILE_EXTN is the extension of target files to document
# Different file extensions should be separated by a single space (without quotes)
# period(.) before extensions are optional
# i.e. txt .md .py
TARGET_FILE_EXTN = txt

# REPLACE_OLD_TARGET sets whether the old target_data dir should be replace or not
# True to replace all old target files, False to add new docs to existing target_data folder
REPLACE_OLD_TARGET = True
