# ad-hook

ad-hook is an advanced, cross-architecture hooking framework, for you and your family.

## Build Instructions

As of this version, an `ARCH` variable is required when building.

```sh
cmake -S . -B build -DARCH=$ARCH
cmake --build build
```

## Running Unit Tests

```sh
cd build
ctest
```

## Features

### Multiple handlers per hook

Multiple handlers can subscribe to a single hook, than be called sequentially.
Each handler can either yield, or continue to the next handle via `call_next()`

### Implementation independent functionality

The hooking logic is independent of the actual patching logic.
That is, more patching methods can be implemented using the core hooking logic.

## Supported Patching methods

- [x] jumpto patching
- [ ] address table overriding

## Supported architectures

- [x] i386
- [x] x86_64
- [ ] arm32le
- [ ] arm32be
- [ ] arm64le
- [ ] arm64be
- [ ] mipsbe
- [ ] mipsle
