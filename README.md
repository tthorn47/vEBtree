# vEBtree

This is an implementation of the Van Emde Boas Tree algorithm in C/C++. Two programs are available to test it, a correctness test "veb" and a benchmark against std::set "bench". Both take a number of elements to insert into the structure as a single command line argument. Please note that C++20 features are used, and the makefile may need to be edited for older versions of gcc/g++.
```
  make bench
  ./bench 1000000

  make veb
  ./veb 1000000
```
