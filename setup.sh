#!/bin/bash
docker build . -t 9cc
docker run -v $(pwd)/workspace:/home --name 9cc -w /home -d -it 9cc /bin/sh
