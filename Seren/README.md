## Seren

This repository contains the implementation of Seren, a publicly-verifiable non-interactive dynamic searchable symmetric encryption (DSSE) scheme. This is the first DSSE scheme that achieves public verification on the search result while supporting forward and Type-II backward privacy[1] and non-interactive queries. The detailed construction and security proof are presented in our ACISP'24 paper [2].

## Requirements

* Git
* Ubuntu 18.04
* clang++15.0.0
* cmake 3.17
* openssl 1.1.1
* Apache Thrift 0.13.0
* RocksDB 8.10.0

## Building

```bash
git clone https://github.com/MonashCybersecurityLab/Aura.git
cd Seren
mkdir build
cd build
# use cmake/make to build the code
cmake ..
make
```

## Usage

After compiling the project, two executable files of Aura will be generated in `build`. The  two files (`SerenServer` and `SerenClient`)  can be used to execute Seren in a networked environment. - -

- Particularly, the `SerenServer` starts an Apache Thrift server which can process the setup, update, query operations from the `SerenClient`. The results from these two executable file reflect the performance of Seren in real-world networks.
- The operations and parameters of SerenClient can be set and configure in main.cpp.
- All the above executable files can be executed without extra parameters.

## Feedback

- Email the authors: jerry001@sjtu.edu.cn

## Reference

[1] Raphaël Bost, Brice Minaud, and Olga Ohrimenko. 2017. Forward and Backward Private Searchable Encryption from Constrained Cryptographic Primitives. In *Proceedings of the 2017 ACM SIGSAC Conference on Computer and Communications Security* (*CCS '17*). Association for Computing Machinery, New York, NY, USA, 1465–1482. DOI: https://doi.org/10.1145/3133956.3133980

[2] Luo, Z., Sun, S. F., Wang, Z., & Gu, D. (2024, July). Non-interactive Publicly Verifiable Searchable Encryption with Forward and Backward Privacy. In *Australasian Conference on Information Security and Privacy* (pp. 281-302). Singapore: Springer Nature Singapore.
