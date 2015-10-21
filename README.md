# trans_image_filter

simple application which can perform removal of salt-and-peper noize and detect edges.
Work only with JPEG still image compliance streams.

build steps:

1. get OpenCV and Boost libraries.

2. get cmake build tool

3. run cmake from root dir to create make files.

4. run make, binary will be located in /bin dir, as a couple test files.

example usage:

sample -i test2.jpd -o dst.jpg -m 3    ;1 - noize removal, 2 - edge detection, 3 - both
