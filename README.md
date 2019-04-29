# Let's make a C compiler!
This repo is workspace for [低レイヤを知りたい人のためのCコンパイラ作成入門](https://www.sigbus.info/compilerbook/).

## Requirements
- [Docker](https://www.docker.com) environment.

## Setup
1. Run `./setup.sh`
2. :tada:

## Format
1. Install [clang-format](http://clang.llvm.org/docs/ClangFormat.html) `brew install clang-format`
2. Run `clang-format *.c *.h -i -style=llvm`
3. :tada:

## Convenient Alias
Add these alias to your `~/.bashrc` or `~/.zshrc`.
```
alias dockercc="docker exec -w /home -it 9cc"
alias entercc="dockercc /bin/sh"
alias makecc="dockercc make"
alias testcc="dockercc make test"
alias cleancc="dockercc make clean"
alias runcc="dockercc ./9cc"
```

## Build
Run `makecc`

## Test
Run `testcc`

## Clean
Run `cleancc`

## Enter the container
Run `entercc`

## Run the compiler
Run `runcc`
