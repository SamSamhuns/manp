# manp

Command-line utility for generating a `man-page` equivalent of your project.

Submission for tiancai-cup from @SamSamhuns

[![Build Status](https://travis-ci.com/SamSamhuns/manp.svg?token=Z5sqtVdygQce8gKWkdrq&branch=master)](https://travis-ci.com/SamSamhuns/manp)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

<img src='img/manp_demo_high.gif'>

<br>Tired of looking throughout your build directory for the functions you wrote before and don't remember their parameters or even tired of Googling common Python modules and function documentations?

`manp` is a command-line utility that lets your create a `man pages` equivalent of your project. This is similar to how `man printf` works for C/C++ functions. But, now you can `manp [YOUR_FUNCTIONS]` from your source code instead.

If incorrect modules are entered, `manp` also has a feature for auto-suggesting most similar named module.

## Installation

*Running make without setting any SOURCE_DIR or TARGET_FILE_EXTN creates a default `manp` with documentation for the Python standard library modules and functions.*

**Requirements:**
-   Python 3.5+
-   g++ compiler
-   bash shell

**For OSX and Linux Only**

1.  Clone the repository.
2.  Set the `SOURCE_DIR` (The absolute path to your build directory) and `TARGET_FILE_EXTN`(File extensions to add to `manp`, separated by a single space inside double quotes) inside the `makefile_configuration.mk` file or the `make` command can be run with:

```bash
$ make SOURCE_DIR=/usr/project/src ARGET_FILE_EXTN="txt py"
```

3.  Use the following command to compile the `manp` executable and set execute permissions. (The git builds the Python documentation `manp`)

```bash
$ make all
$ chmod a+x manp
```

4.  Add the `manp` executable to `PATH` var for faster execution so that `manp` can be called using a `bash` shell from any directory and speeds documentation lookup.

    -   **Recommended:**

        `manp` can be indirectly added to the current `PATH` variable by creating a symlink to `/usr/local/bin` using the absoute path of the `manp` executable.

        ```bash
        $ ln -s /[ABSOLUTE_PATH]/manp /usr/local/bin
        ```

        \* If the original location of the `manp` executable is changed, the soft link must be updated again with the above command.

    -   **Alternative:**

        copy `manp` to `/usr/local/bin` with:

        ```bash
        $ cp manp /usr/local/bin
        ```

        \* Every time, there is a change to the source code, the `manp` binary will have to be recopied to `usr/local/bin`.

**Note:** If there are any permission issues, use `sudo`. The `manp` executable can be directly added to the `PATH` var if `sudo` access is not present.

### Utility functions

**Only for Python project as of now**

The `python generate_txt_from_python_progs.py` function can be used to generate txt files containing information on individual functions and classes, the source folder is denoted by `[SOURCE_FILE_DIR]` while the target folder is denoted by `[TARGET_DIRECTORY]`.

```bash
$ python generate_funcs_from_modules [SOURCE_FILENAME] [TARGET_DIRECTORY]
$ python generate_txt_from_python_progs.py [SOURCE_FILE_DIR] [TARGET_DIRECTORY]
```

## Uninstallation

 To remove the `manp` symlink from `/usr/local/bin`:

```bash
$ rm /usr/local/bin/manp
```

or,

```bash
$ unlink /usr/local/bin/manp
```

**Note:** If there are permission errors, use `sudo`. The symlink can be manually deleted as well

## Usage

To get documentation for a standard lib module or a function:

```bash
$ manp [PYTHON_FUNCTION_NAME]
$ manp [PYTHON_MODULE_NAME]
```

Example:

```bash
$ manp -m hashlib
$ manp ascii
```

To display `help`:

```bash
$ manp -h
```

To display list of all available modules:

```bash
$ manp -lm
```

To display list of all available functions:

```bash
$ manp -lf
```

## Workflow

A detailed explanation of how the program functions:

The program requires compilation and running of certain files and script before others which is handled by the `Makefile`.

1.   The `text_to_raw_string.cpp` and `raw_string_to_text.cpp` modules are compiled before main. The `text_to_raw_string.cpp` converts all the files inside the `SOURCE_DIR` folder to `manp/target_data/` folder as raw string literal txt files padded with extra `CPP_MAP_KEY_VALUE_DELIM` and `CPP_MAP_MODULE_DELIM` (defined in `common.h`) delimiters for creating C++ maps.

2.   After this, the `generate_doc_artifacts_list.sh` will iterate through the `manp/target_data/` folder and create `combined_txt_include.h` that includes all the `txt` raw string literal files in one header file and `doc_artifacts_list` header include file containing just the names of all the modules from the `target_data` folder.

4.   The `main.cpp` function will finally be compiled along with `SpellingCorrector.cpp` and `manp` will be available for execution.

5.   Once the `manp` is added to the `PATH` var one way or another, it can be used as `manp abs`.

## Developer instructions

-   To add documentations manually, txt files with the name of the module in question should be added to the `SOURCE_DIR` folder and the project should be recompiled.

-   To change documentation or source code files back to normal text format, run `./raw_string_to_text data`.

### Contributions

[![License](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat.svg)](https://github.com/SamSamhuns/manp/pulls)

### Acknowledgements

-   All documentations imported from the Python Software Foundation Official Documentation <https://www.python.org/psf/>
-   Peter Norvig's <a href='https://norvig.com/spell-correct.html'>*How to Write a Spelling Corrector*</a>
-   Felipe Farinon's C++ implementation of Peter Norvig's Spell Correcter <felipe.farinon@gmail.com>

### Disclaimer

I am not the creator of the SpellingCorrector C++ modules. The SpellingCorrector module used in this project was first written by Peter Norvig in Python and then converted to C++ by Felipe Farinon.
