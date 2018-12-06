Something to start with

Assumes a folder structure of:

```
base
    src
    src_test
    include
```

Creates folders as needed during build:

```
base
    <symlink to app>
    <symlink to test_app>
    src
    src_test
    include
    build
        <app *.o>
        bin
            <app>
        test
            <test_app *.o>
            bin
                <test_app>
```

Set the App name in the Makefile by changing `BIN_NAME`:

```make
BIN_NAME = find_ids
```

All the other source and objects are discovered automatically.

Based on a skeleton from [this gist](https://gist.github.com/mauriciopoppe/de8908f67923091982c8c8136a063ea6)