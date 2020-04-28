# Requirement:
 <br>
 OS: Ubuntu 16.04 / 18.04
 
 <br>
 
 Lib: [Protobuf 3.5.1](https://github.com/protocolbuffers/protobuf)
 
 <br>
 
 Driver: [MariaDB Server](https://downloads.mariadb.org/mariadb/10.5.1/) + [MariaDB ODBC Connector 3.1](https://downloads.mariadb.org/connector-odbc/3.1.6/)
 
 <br>
 
 Qt Version > 5.11 
 <br>
# Installation of Protobuf 3.5.1

[Protobuf 3.5.1 C++ Installation](https://github.com/protocolbuffers/protobuf/blob/master/src/README.md)

To build protobuf from source, the following tools are needed:

  * autoconf
  * automake
  * libtool
  * make
  * g++
  * unzip

On Ubuntu, you can install them with:

    $ sudo apt-get install autoconf automake libtool curl make g++ unzip

To get the source, download one of the release .tar.gz or .zip packages in the
release page:

[https://github.com/protocolbuffers/protobuf/releases/tag/v3.5.1](https://github.com/protocolbuffers/protobuf/releases/tag/v3.5.1)

For example: if you only need C++, download `protobuf-cpp-[VERSION].tar.gz`; if
you need C++ and Java, download `protobuf-java-[VERSION].tar.gz` (every package
contains C++ source already); if you need C++ and multiple other languages,
download `protobuf-all-[VERSION].tar.gz`.

> **We download protobuf-cpp-3.5.1.zip.**

To build and install the C++ Protocol Buffer runtime and the Protocol
Buffer compiler (protoc) execute the following:

```shell
$ cd protobuf
$ ./autogen.sh
    
$ ./configure
$ make -j4
$ make check
$ sudo make install -j4
$ sudo ldconfig # refresh shared library cache.
```
If "make check" fails, you can still install, but it is likely that
some features of this library will not work correctly on your system.
Proceed at your own risk.

# SQL Driver

Look at `controlunit.cpp` and `controlunit.h`.

To Connect Driver:
```
connectDatabase(hostName,port,user,password,driverVersion);
//Note All the parameters are QString except port (int)
```


# QR Code | Barcode Detection

[Detecting Barcodes in Images with OpenCV](https://www.pyimagesearch.com/2014/11/24/detecting-barcodes-images-python-opencv/)


[Detect a QR code from an image and crop using OpenCV](https://stackoverflow.com/questions/60359398/python-detect-a-qr-code-from-an-image-and-crop-using-opencv)
