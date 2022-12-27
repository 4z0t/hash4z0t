# H4z0t 

The encoder project named by me is created for studying basics of information theory and encoding.
The executable is a program for compressing, error-detection/correction and encryption of data.

# Format structure

## Format Header

| Data: | 4z0t | format version | files count | Total |
|-------|-----:|---------------:|------------:|------:|
| Bytes:| 4    |   4            |  4          | 12    |

## Format Data

After header files' data is written in this format:

### File Header

| Data: | data size | file name size |  compression type | encryption type | protection type | additional data | Total |
|-------|----------:|---------------:|------------------:|----------------:|----------------:|----------------:|------:|
| Bytes:| 8         |   4            |  4                |4                | 4               | 8               | 32    |

### File Data

After file header filename is written with its relative path in folder being encoded.
Depending on the compression type, encryption type and protection type file's data is written.


# Flags and Arguments

There are 3 arguments and 3 flags defined right now:

* `--decode` and `--encode` are arguments for defining path(s) for file(s) to be processed
* `--into` is argument for path where to store encoded/decoded data
* `-lz77` and `--shanon-fano` are flags for compression types
* `-hamming` is flag for protection type
