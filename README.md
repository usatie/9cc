# Let's make a C compiler!
This repo is workspace for [低レイヤを知りたい人のためのCコンパイラ作成入門](https://www.sigbus.info/compilerbook/).

## Requirements
- [Docker](https://www.docker.com) environment.

## Build and start a container
1. Run `./setup.sh`
2. :tada:

## 2. Utility aliases for development
Add these alias to your `~/.bashrc` or `~/.zshrc`.
```
alias dockercc="docker exec -w /home -it 9cc"
alias entercc="dockercc /bin/sh"
alias makecc="dockercc gcc make"
alias testcc="dockercc make test"
alias cleancc="dockercc make clean"
alias runcc="dockercc ./9cc"
```
