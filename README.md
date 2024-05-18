# manp

![Build](https://github.com/SamSamhuns/manp/actions/workflows/build.yml/badge.svg)

A portable command-line utility for generating a `man-page` equivalent for your custom project.

Submission for tiancai-cup from @SamSamhuns

<img src='img/manp_demo_high.gif'>

<br>Tired of looking throughout the build directory of your project for the functions you wrote weeks before or don't remember their parameters. Want to find a quick way of querying functions and modules while displaying them like the `on-line man pages`?

`manp` is a command-line utility that lets you create a `man pages` equivalent documentation of your project. This is similar to how `man printf` works for C/C++ functions. But, now you can `manp [YOUR_FUNCTIONS]` from your source code instead.

All files of extension `TARGET_FILE_EXTN` are recursively searched inside the `manp/target_data` folder and added to the data section of the `manp` in the form of header includes.

If incorrect modules are entered, `manp` also has a feature for auto-suggesting most similar named module.

## Installation

*Running make without setting any SOURCE_DIR or TARGET_FILE_EXTN creates a default `manp` portable executable with documentation for the Python standard library modules and functions.*

**Requirements:**
-   Python 3.5+
-   g++ compiler
-   bash shell

**For OSX and Linux Only**

1.  Clone the repository.
2.  Set the `SOURCE_DIR` (Absolute path to your build directory) and `TARGET_FILE_EXTN`(File extensions added to `manp`, separated by a single space inside double quotes) inside the `makefile_configuration.mk` file. The `SOURCE_DIR` can be specified to any directory, the make commands recursively search through the source to find all files of the extension `TARGET_FILE_EXTN`. The `REPLACE_OLD_TARGET` variable, if set to `True` will clean the `target_data` folder before make.

3.  After the Use `makefile_configuration.mk` config file has been properly set, `manp` can be compiled and the execute permissions be set. ( The original download from GitHub builds a `manp` Python documentation )

```bash
$ make all
$ chmod a+x manp
```

*Note:* If the `.mk` config file is not changed, the `make` command can also be run like so:
```bash
$ make SOURCE_DIR=/usr/project/src TARGET_FILE_EXTN="txt py md"
```

4.  Add the `manp` executable to `PATH` var for faster execution so that `manp` can be called using a `bash` shell from any directory and speeds documentation lookup.

    -   **Recommended:**

        `manp` can be indirectly added to the current `PATH` variable by creating a symlink to `/usr/local/bin` using the absoute path of the `manp` executable.

        ```bash
        $ ln -s /[ABSOLUTE_PATH]/manp /usr/local/bin
        ```

        *Note:* If the original location of the `manp` executable is changed, the soft link must be updated again with the above command.

    -   **Alternative:**

        copy `manp` to `/usr/local/bin` with:

        ```bash
        $ cp manp /usr/local/bin
        ```

        *Note:* Every time, there is a change to the source code, the `manp` binary will have to be recopied to `usr/local/bin`.

**Note:** If there are any permission issues, use `sudo`. The `manp` executable can be directly added to the `PATH` var if `sudo` access is not present.

### Utility functions

**Only for Python projects as of now:**

The `python generate_txt_from_python_progs.py` function can be used to generate txt files containing information on individual functions and classes, the source folder is denoted by `[SOURCE_FILE_DIR]` while the target folder is denoted by `[TARGET_DIRECTORY]`.

```bash
$ python generate_funcs_from_modules [SOURCE_FILENAME] [TARGET_DIRECTORY]
$ python generate_txt_from_python_progs.py [SOURCE_FILE_DIR] [TARGET_DIRECTORY]
```

**Support for more projects will be added in the future**

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

To get documentation for a artifact present in the `SOURCE_DIR`:

```bash
$ manp [MODULE_OR_FUNCTION_NAME]
```

Example: (For the default Python std lib documentation)

```bash
$ manp zlib
```

To display `help`:

```bash
$ manp -h
```

To display list of all available documented artifacts:

```bash
$ manp -l
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
