docker build -t test-z3 .
docker run -it --rm --name TTTT -v $(pwd)/src:/app test-z3
