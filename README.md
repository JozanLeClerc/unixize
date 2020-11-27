
# Table of Contents

1.  [Dependencies](#org0b55cd2)
2.  [Installation](#orgc36ed01)
3.  [Usage](#org06b1425)
4.  [End note](#orgd171c54)

**unixize** is a small tool to bulk rename files into a cool UNIX-friendly
standard. Basically, everything becomes lowercase and there is no need to
escape characters anymore when entering your filenames in a terminal. It
also removes unicode characters.

**unixize** is configurable through options. Refer to

    man unixize

to get a list of options.


<a id="org0b55cd2"></a>

# Dependencies

-   A C compiler
-   GNU Make


<a id="orgc36ed01"></a>

# Installation

You can get **unixize** either through `git` or `fossil`.

For `fossil` use:

    mkdir unixize
    cd unixize
    fossil clone https://chiselapp.com/user/JozanLeClerc/repository/unixize unixize.fossil
    fossil open --force unixize.fossil

or for `git`:

    git clone git://jozanleclerc.xyz/jozan/unixize.git
    cd unixize

Then build the program:

    make
    sudo make install clean

Note that on BSD systems you want to use `gmake` instead of `make`:

    gmake
    sudo gmake install clean


<a id="org06b1425"></a>

# Usage

**USE WITH CAUTION**.

Without the `-p` option, **unixize** will commit to do exactly
what it's supposed to do, rename all the badly formatted in the chosen
directory. To deunixize your files, you'll have to rename everything
manually.

    cd messy_directory
    unixize -v -r .


<a id="orgd171c54"></a>

# End note

Please note that unixize was tested only on FreeBSD at the moment.
Linux and macOS tests are coming soon.  You can report bug on
[GitHub](https://github.com/JozanLeClerc/unixize.git).

Thanks for checking **unixize**.

