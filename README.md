# smplImg

Exercise to implement what I've learned of image processing.

Only median and highboost (and its diagonal counterpart) filters are currently implemented.

Supports .png files using [LodePNG](https://github.com/lvandeve/lodepng) (...fetched via [fpng](https://github.com/richgel999/fpng/)).

## Usage

```
-i [INPUT FILE] -o [OUTPUT FILE] -h [FILTER] [OPTION FOR FILTER]
```
### Filters

```
median [size of mask]

highboost [coefficient]

dhighboost [coefficient]
```
## Build with CMake
```
mkdir build
cd mkdir
cmake ..
make
./terminal10
```
