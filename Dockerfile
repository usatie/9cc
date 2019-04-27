FROM ubuntu
RUN apt-get update && apt-get install -y gcc make git binutils libc6-dev && rm -rf /var/lib/apt/lists/*
