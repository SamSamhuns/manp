# manp

[![Build Status](https://travis-ci.com/SamSamhuns/manp.svg?token=Z5sqtVdygQce8gKWkdrq&branch=master)](https://travis-ci.com/SamSamhuns/manp)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

<img src='img/manp_demo_high.gif'>

Tired of Googling common Python modules and function documentations. `manp` is a command-line utility for quickly loading manual pages for all Python Standard Library functions. Similar to how `man printf` works for C/C++ functions. Documentation for other PyPI packages will also be added as an extension.

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

```bash
$ manp hashlib
```

## Workflow

## Developer instructions

## Contributions

[![License](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat.svg)](https://github.com/SamSamhuns/manp/pulls)

## Acknowledgements

-   Python Software Foundation <https://www.python.org/psf/>
