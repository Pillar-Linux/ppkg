| Branches: |
| Master |[![Build Status](https://drone.magnatox.com/api/badges/pillar-linux/ppkg/status.svg?ref=refs/heads/master)](https://drone.magnatox.com/pillar-linux/ppkg)|
| Autoconf |[![Build Status](https://drone.magnatox.com/api/badges/pillar-linux/ppkg/status.svg?ref=refs/heads/autoconf)](https://drone.magnatox.com/pillar-linux/ppkg)|

# Pillar package-script(ppkg)

## Release: 0.1.0

## Branch: Master

### Tony Martinez 2021

This program will handle compiling/building packages from a TOML configuration file.

## Dependencies

- [TOMLC89](https://github.com/tonymmm1/tomlc89)

## Default paths

- /usr/src/programs

## Example of a program config:

- [program config](docs/package-example.md)

## Build instructions

CMake:

```
mkdir build
cd build
cmake ..
make
make install
```

## Usage

```
Pillar package-script(ppkg):
Usage: ppkg [options] ...
    -h (package)      Show usage output
    -b (package)      Build package
    -i (package)      Install package
    -l (package)/all  List packages
    -r (package)      Remove package
    -s (package)      Show package conf file
    -v                Verbose output
```
