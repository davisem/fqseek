[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![CircleCI](https://circleci.com/gh/davisem/fqseek.svg?style=shield)](https://circleci.com/gh/davisem/fqseek/master)


# fqseek
#### Approximate string matching in fastq files. 


## Motivation
You just ran your favorite variant calling program and expected to find your favorite variant called in the results. But your variant wasn't called. Where to being? A good place to start looking might be the unaligned, input sequence. `fqseek` can searches fastq files for a variant with 16 base pairs of context sequence, allowing for fuzz.

```
  A seq          B seq
 (8bps)  (1bp)  (8bps)       
--------   *   --------   
NNNNNNNN [var] NNNNNNNN
```
The variant base `[var]`  must match exactly, but `fqseek` allows for a user-defined number of mismatches in the `N` bases flanking the variant.

## Requirements
A C++11 is compiler is required to run `fqseek`
* gcc 4.8+

### Build
Use the [Meson Build System](https://mesonbuild.com/index.html).
You will also need to install [ninja](https://ninja-build.org).
```
pip3 install meson
pip3 install ninja
```

To build and install `fqseek`
```
cd fqseek
meson --buildtype release build
INSTALL_DIR=your_bin_path ninja -C build install
```

### Run
```
fqseek
Usage: fqseek fastq [OPTION...] > outfile
```

### Build and run the tests
```
g++ -std=c++11 -o test_fqseek -I src src/seq_scanner.cpp src/util.cpp test/test_seq_scanner.cpp
./test_fqseek
```

### Run some small demo data
```
fqseek test/test.fq -n=2 -seq=GGTTCTAGAGAATCAGA
```

## Method
Finding an approxiamte match to a sequence requires calculating the edit distance for all windows in a sequence, for every sequence in the file. To do this efficiently `fqseek` pre-computes the edit distances between each flaking sequence `A` and `B`, and all possible 8bp sequences. First, `A` and `B` are converted to unsigned 16-bit integers `A'`, `B'`, where the base in the sequence `A[i]` is stored by bit positions `2i, 2i + 1` in the integer `A'`. Edit distances between `A'` and all other integers in the range `[0, 65535]` are obtained using binary XOR, and stored at position `A'` in an array of unsigned 8-bit integers (`arrA[65536]`). The edit distances for `B'` are calculated and indexed accordingly and are stored in `arrB[65536]`.

`fqseek` reads each sequence from the input fastq file and converts the bases in a window `W` to a bit stream `W'`. The window scans along the sequence using binary left-shift to add the next base in the sequence. After each base is added, the edit distances for sequences `A'`, `B'` are retrieved. A match is returned when `arrA[A'] + arrB[B'] <= max_distance` and the base at the variant position matches exactly.

