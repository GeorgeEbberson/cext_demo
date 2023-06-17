# Python C-Extension Demonstration
The files here aim to demonstrate the basic machinery needed for a Python C-Extension, whilst also
giving some indications of what performance improvements should (and should not) be expected. There
is also some consideration of alternative ways to improve performance.

## Setup and Installation
First clone this repo locally with `git clone`, then from the directory containing this code:

1. Create a Python 3 virtual environment (venv) - I used [Python 3.10](https://www.python.org/downloads/release/python-3100/).
2. Activate the virtual environment - by going to `<venv root folder>/Scripts/activate` (it may be
`activate.bat` or `activate.ps1` depending on your terminal type).
3. With the venv now active, navigate to the root of this repo and run `python setup.py install`
4. Now run `python compare_methods.py`

Note that in order for the `install` command to work, you will need a working C compiler installed.
