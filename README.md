# manp

Submission for tiancai-cup from @SamSamhuns

[![Build Status](https://travis-ci.com/SamSamhuns/manp.svg?token=Z5sqtVdygQce8gKWkdrq&branch=master)](https://travis-ci.com/SamSamhuns/manp)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

<img src='img/manp_demo_high.gif'>

Tired of Googling common Python modules and function documentations. `manp` is a command-line utility for quickly loading manual pages for all Python Standard Library functions. Similar to how `man printf` works for C/C++ functions. Documentation for other PyPI packages will also be added as an extension.

If incorrect modules are entered, `manp` also has a feature for auto-suggesting most similar named module.

## Installation

**For OSX and Linux Only**

1.  Clone the repository.
2.  Use the following command to compile the `manp` executable and set execute permissions.

```bash
$ make all
$ chmod a+x manp
```

3.  Add the `manp` executable to `PATH` var for faster execution so that `manp` can be called using a `bash` shell from any directory and speeds documentation lookup.

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

1.   The `text_to_raw_string.cpp` and `raw_string_to_text.cpp` modules are compiled before main. The `text_to_raw_string.cpp` converts all the `.txt` files inside the `manp/data/` folder to raw string literal txt files padded with extra `CPP_MAP_KEY_VALUE_DELIM` and `CPP_MAP_MODULE_DELIM` (defined in `common.h`) delimiters for creating C++ maps.

2.   After this, the `combine_headers.sh` will iterate through the `manp/data/` folder and create `combined_txt_include.h` that includes all the `txt` raw string literal files in one header file.

3.   The `generate_modules_list.sh` will then iterate through the `manp/data/modules` folder again and create a `modules_list.txt` raw string literal txt file containing list of all available modules that will be used for the SpellingCorrector and prediction function.

3.   The `generate_functions_list.sh` will also iterate through the `manp/data/std_functions` folder again and create a `functions_list.txt` raw string literal txt file containing list of all available std lib functions that will be used for the SpellingCorrector and prediction function.

4.   The `main.cpp` function will finally be compiled along with `SpellingCorrector.cpp` and `manp` will be available for execution.

5.   Once the `manp` is added to the `PATH` var one way or another, it can be used as `manp abs`.

## Developer instructions

-   To add documentation for other Python modules, a txt file with the name of the module in question should be added to the `manp/data/modules` folder and the project should be recompiled.

-   To add function documentations, txt files with the name of the function in question should be similarly added to `manp/data/std_functions` folder and the project be recompiled.

-   To change the `.txt` documentation files inside `manp/data/` back to normal text format, run `./raw_string_to_text data`.

### Contributions

[![License](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat.svg)](https://github.com/SamSamhuns/manp/pulls)

### Acknowledgements

-   All documentations imported from the Python Software Foundation Official Documentation <https://www.python.org/psf/>
-   Peter Norvig's <a href='https://norvig.com/spell-correct.html'>*How to Write a Spelling Corrector*</a>
-   Felipe Farinon's C++ implementation of Peter Norvig's Spell Correcter <felipe.farinon@gmail.com>

### Disclaimer

I am not the creator of the SpellingCorrector C++ modules. The SpellingCorrector module used in this project was first written by Peter Norvig in Python and then converted to C++ by Felipe Farinon.
