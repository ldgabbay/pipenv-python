# `pipenv-python`

Anyone who's used `pipenv` has quickly discovered that it is not trivial to have a python script automatically load its `pipenv` environment. The expectation is that you are already in your `pipenv` environment before you execute any python scripts.

This program acts as a shebang (`#!`) interpreter for python scripts under `pipenv`.


## Usage

For any executable python script that uses `pipenv`, where the first line would typically be:

```bash
#!/usr/bin/env python
```

replace this line with:
```bash
#!/usr/bin/env pipenv-python
```

That's it.


## Installation

To compile `pipenv-python`:

```bash
gcc -o pipenv-python pipenv-python.c
```

Place `pipenv-python` somewhere in your path.


## Under the covers

Suppose you have written an executable python script called `pyscript`, and its first line is:

```bash
#!/usr/bin/env pipenv-python
```

When you call `pyscript args...`, the kernel will transform this into:

```bash
/usr/bin/env pipenv-python /path/to/pyscript args...
```

`/usr/bin/env` will search the environment's path for `pipenv-python` and transform into:

```bash
pipenv-python /path/to/pyscript args...
```

At this point, `pipenv-python` is in charge. It looks at the location of `pyscript` and looks for a `Pipfile` according to the same rules that `pipenv` follows.

If a `Pipfile` is found, this transforms into:

```bash
PIPENV_PIPFILE=/path/to/Pipfile
pipenv run python /path/to/pyscript args...
```

which is precisely what we want!  And if a `Pipfile` is not found, `pipenv-python` falls back to `python` and transforms the command into:

```bash
python /path/to/pyscript args...
```
