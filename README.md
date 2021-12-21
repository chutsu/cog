# cog [![Build Status](https://github.com/chutsu/cog/actions/workflows/ci.yml/badge.svg)]

**cog** is a common library in C (similar to Boost for C++, but for C). It is
heavily based on the code [Zed Shaw][2] produced in [Learn C the Hard Way][3].

Features:

**Data Structures**:
- Dynamic Array
- List
- Queue
- Stack
- Hashmap

**Utility**:
- General Copy function
- Malloc Integer
- Malloc Float
- Malloc Double
- Malloc String
- General free but with if NULL check
- Free 1D array
- Free 2D array

**String**:
- Trim

**Random**:
- Random integer
- Random float
- Sample a 2D array

**Comparators**:
- Integer comparator
- Float comparator
- General comparator


## Installation
Enter the following command in the terminal:


    curl -L http://git.io/vUWo3 > install.sh && sh install.sh


Or alternatively


    git clone https://github.com/chutsu/cog.git
    cd cog
    make
    make install


## LICENCE
MIT LICENCE Copyright (C) <2012> Chris Choi

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

[1]: https://github.com/chutsu/cog/actions/workflows/ci.yml
[2]: http://en.wikipedia.org/wiki/Zed_Shaw
[3]: http://c.learncodethehardway.org/book/
