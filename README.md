[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)


# fqseek
#### Approximate string matching in fastq files. 


## Motivation
You just ran your favorite variant calling program and expected to find your favorite variant called in the results. But your variant wasn't called. Where to being? A good place to start looking might be the unaligned, input sequence. `fqseek` can quickly search fastq files for a variant with 16 base pairs of context sequence, allowing for fuzz, as follows:

```
  A seq          B seq
 (8bps)  (1bp)  (8bps)       
--------   *   --------   
NNNNNNNN [var] NNNNNNNN
```
The variant base `[var]`  must match exactly, but `fqseek` allows for a user-defined number of mismatches in the `N` bases flanking the variant.


## Method
This problem requires calculating the edit distance, for all windows in a sequence, for every sequence in the file. To do this efficiently, `fqseek` pre-computes the edit distances between each flaking sequence `A` and `B`, and all possible 8bp sequences. First, `A` and `B` are converted to unsigned 16-bit integers `A'`, `B'`, where the base in the sequence `A[i]` is stored by bits at position `2i, 2i + 1` in the integer `A'`. Edit distances between `A'` and all other integers in the range `[0, 65535]` are then obtained using binary XOR and right shifts and stored at position `A'` in an array of unsigned 8-bit integers (`arrA[65536]`). The edit distances for `B'` are calculated and indexed accordingly and stored in `arrB[65536]`.

`fqseek` reads each sequence from the input fastq file and converts the bases in a given window `W` to a bit stream consisting of a single unsigned, 64-bit integer `W'`. To incorporate the next base `b` into the window `W'`, `W'` is left-shifted 2 positions `(W' << 2 )`, and the incoming 2 bits `b` are added to the least significant positions of `W' (W' | b)`. After each sliding event, the edit distances for sequences `A'`, `B'` in `W'` are retrieved. A match is returned when `arrA[A'] + arrB[B'] <= max_distance` and the base at the variant position matches exactly.

## Requirements
A C++11 is compiler is required to run `fqseek`
* gcc 4.8+

### Build
```
cd fqseek
g++ -std=c++11 -O3 -o fqseek src/fqseek.cpp src/seq_scanner.cpp src/util.cpp
```

### Run
```
Usage: ./fqseek fastq [OPTION...] > outfile
```

### Build and run the tests
```
g++ -std=c++11 -O3 -o test_fqseek -I src src/seq_scanner.cpp src/util.cpp test/test_seq_scanner.cpp
./test_fqseek
```

### Run some small demo data
```
./fqseek test/test.fq -n=2 -seq=GGTTCTAGAGAATCAGA
```
