# cmake-vcpkg-starter

This is a sample starter project showing how to use vcpkg with cmake. It uses 
websocketpp as an example dependency, and connects to the bitmex websocket and 
prints book updates as an example.

## Usage

```
git clone https://github.com/neov5/cmake-vcpkg-starter.git --recurse-submodules && cd cmake-vcpkg-starter
cmake -B build -S . --prefix debug
cd build && make
./main
```

If all goes well, you should see a stream of market data. Press ctrl-C to exit.

## Using vcpkg

Edit the `vcpkg.json` file to add your dependencies. You would also need to add 
the dependency to `CMakeLists.txt` (instructions on how to do this are printed 
on installing the package, or can be found in `build/vcpkg_installed/<arch>/share/<pkg>/usage`)

## Overlays

Note that we use the system version of OpenSSL, as using a preconfigured
dependency doesn't work on some systems. To work around this with vcpkg, we use 
an [overlay](https://learn.microsoft.com/en-us/vcpkg/concepts/overlay-ports),
containing the following two files:

portfile.cmake
```
set(VCPKG_POLICY_EMPTY_PACKAGE enabled)
```

vcpkg.json
```
{
    "name": "openssl",
    "version": "3.2"
}
```

This configures vcpkg to not install the dependency, and cmake will find the 
system OpenSSL instead.
