docker build . -t compiler
docker volume create compiler
docker run -v compiler:/home -it compiler /bin/sh
