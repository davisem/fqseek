# fqseek
#### Approximate string matching in fastq files. 

### Motivation
You just ran program `X` and expected to identify variant `Y` in the results. But variant `Y` was not reported in the output. So where did it go? Was it even present in the input sequence? `fqseek` can quickly search the fastq file for a variant with 8 base pairs of context sequence on either side of the variant as follows:
```
  8bps    1bp    8bps       
--------   *   --------   
NNNNNNNN [var] NNNNNNNN
```
The variant base `[var]`  must match exactly, but `fqseek` allows for an aribtrary number of mismatches in the `N` bases flanking the variant.
### Requirements

A C++11 is compiler is required to run `fqseek`.

* gcc 4.8+

### Build
```
cd fqseek
g++ -std=c++11 -O3 -o fqseek -I src src/fqseek.cpp src/seq_scanner.cpp src/util.cpp
```

### Run
```
Usage: ./fqseek fastq [OPTION...] > outfile
```

### Build and run the tests
```
g++ -std=c++11 -O3 -o test_fqseek -I src src/seq_scanner.cpp src/util.cpp src/test_seq_scanner.cpp
./test_fqseek
```

### Run some small demo data
```
./fqseek test/test.fq -n=2 -seq=GGTTCTAGAGAATCAGA
```
