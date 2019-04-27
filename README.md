# Let's make a C compiler!
This repo is workspace for [低レイヤを知りたい人のためのCコンパイラ作成入門](https://www.sigbus.info/compilerbook/).

## Requirements
- [Docker](https://www.docker.com) environment.

## Build and start a container
1. Run `./setup.sh`
2. :tada:

* Ctrl+P, Ctrl+Q to exit from the container.


## 2. Execute on a existing container
```
$ docker exec -it {container-id} /bin/sh
```

* Use `docker ps` to get container's id.


## 3. Utility command for development

```~/.zshrc or ~/.bashrc
alias compile="docker exec -it compiler gcc -o /home/9cc /home/9cc.c"
alias run="docker exec -it compiler /home/9cc"
```


```
$ compile
$ run {args}
```
