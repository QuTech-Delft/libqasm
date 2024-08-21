libQASM can be tested in a container with the bare minimum requirements.

```shell
docker build .
```

!!! note
    The above might fail on Windows due to the `autocrlf` transformation that git does.
    If you are having trouble with this just create new clone of this repository using:

    ```
    git clone --config core.autocrlf=input git@github.com:QuTech-Delft/libqasm.git
    ```
