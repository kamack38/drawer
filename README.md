# Drawer

A simple drawer program that draws images using a set of commands.

## Building

```bash
gcc -O3 draw.c image.c line.c vector.c main.c -lm -o drawer
```

## Usage

```
❯ ./drawer -h
Usage: ./drawer [-b] [output.ppm]
❯ ./drawer -b out.ppm <tests/ai.txt
```

The program takes input from stdin (so you can redirect a file to read from file) in the following
format:

First it reads the dimensions of the image in pxiels:

```
<width> <height>
```

And then any number of the following objects to draw:

```
ellipse <x> <y> <width> <height> [angle in deg]
rectangle <x> <y> <width> <height> [angle in deg]
polygon <x1> <y1> <x2> <y2> <x3> <y3> <x4> <y4> ...
```

Note that `<>` denotes required arguments and `[]` denotes an optional argument that is by default
`0`. The `polygon` command draws convex polygon that has to have at least 4 corners.

More examples you can find in the `tests` directory.

## Examples

```
500 500
rectangle white 250 250 500 500 0
polygon blue 250 50 423 150 423 350 250 450 77 350 77 150
```
