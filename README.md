# Batch replace
[![Build Status](https://travis-ci.org/runfalk/batch-replace.svg?branch=master)](https://travis-ci.org/runfalk/batch-replace)

Batch replce allows non-trivial refactoring where a simple `awk` or find replace
command doesn't work. A typical scenario is to replace `foo` with `bar` and
`bar` with `foo`. The replacements need to be performed simultaneously or one
will totally replace the other. Batch replace scans the original file line by
line and performs replacements on the fly.


## Usage example
Imagine we want to `foo` with `bar` and `bar` with `foo` in the following text
file (`example.txt`):

```
foo is sillier than bar
```

We then need to create another text file (`replacements.txt`) with replacements.
The replacements are grouped pair-wise by line:

```
foo
bar

bar
foo
```

Pairs do not need to have a blank line between them but can have for additional
clarity. To run the program simple call it with this command:

```
batchr replacements.txt example.txt
```

`example.txt` now looks like this:

```
bar is sillier than foo
```


## Technical notes
The program is written in C++11 and requires headers for the experimental
filesystem API (`<experimental/filesystem`>). Compilation requires at least
GCC 5.3 or Clang 3.9.
