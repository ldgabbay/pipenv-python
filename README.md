# `pipenv-python`

Anyone who's used `pipenv` has quickly discovered that it is not trivial to have a python script automatically load its `pipenv` environment. The expectation is that you are already in your pipenv environment before you execute any python scripts.

This program acts as a shebang (`#!`) interpreter for python scripts.


## Usage

Suppose you have written a python script called `script`. Normally, the first line of `script` would be:

```bash
#!/usr/bin/env python
```

Instead, change this to:
```bash
#!/usr/bin/env pipenv-python
```

That's it. Make sure `pipenv-python` is somewhere in your path, or this will fail.


## Under the covers

According to typical unix kernel rules, calling `script args...` will be transformed into:

```bash
/usr/bin/env pipenv-python /path/to/script args...
```

`/usr/bin/env` will search the environment's path for `pipenv-python` and transform into:

```bash
pipenv-python /path/to/script args...
```

At this point, `pipenv-python` is in charge. It looks at `script` and looks for a `Pipfile` according to the same rules that `pipenv` uses. If it finds one, this transforms into:

```bash
PIPENV_PIPFILE=/path/to/Pipfile
pipenv run python /path/to/script args...
```

If a `Pipfile` is not found, this falls back to `python` and becomes:

```bash
python /path/to/script args...
```
