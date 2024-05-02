# Christian Prather joshai-challenge

# Overview

This is a basic cli written to interact with the [Josh.ai](https://github.com/jstarllc/JoshCodingChallenge?tab=readme-ov-file) lights server API. Its function is a one way communication and data display application. When ran it will print all info on current lights seen by the server as well as notify the consol of any changes (updates to existing light, removal of a light, addition of a light).

This application works in a continuous polling fashion constantly asking the server if there is any updates.

## Code

### Execution Instructions

To run the program you may either build the executable from source (see [Build Instructions](#build-instructions) below) or if running on a compatible system you may download a bin from the release page.

Once you have a bin you may run it with the command
`./josh-ai-interface`

This will run the cli with default arguments. To get a list / help of all available arguments run.

`./josh-ai-interface -h`

You should get output similar to the below

```
USAGE:

   ./josh-ai-interface  [-k] [-r <int>] [-p <string>] [-a <string>] [--]
                        [--version] [-h]


Where:

   -k,  --keep-alive
     Make connections with keepAlive header, defaults to true

   -r <int>,  --rate <int>
     Rate for client to poll server for updates. (Not a guarantee, if
     client can not keep up)

   -p <string>,  --port <string>
     Port number.

   -a <string>,  --server-address <string>
     The server address.

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.


   Josh.ai CLI
```

<b>By default the cli looks for a server at address `localhost:8080`, will poll as fast as your system allows, and will use the keepAlive http header.</b>

### Build Instructions

<b>The project is built with C++17 and CMake 3.8. It has only been tested against Ubuntu 22.04.</b>
In order to build follow the instructions below.

```
1. git clone git@github.com:Christian-Prather/joshai-challenge.git
2. cd joshai-challenge
3. mkdir build
4. cd build
5. cmake ..
6. make -jn (n = number of threads)
```

This will make a binary in the `build` folder called `josh-ai-interface`

Run it with `./josh-ai-interface`

### Install

If you wish to install the program system wide you may run
`sudo make install`
which will place the bin in your binary folder.

## Documentation

### Building API docs

All code here is documented with Doxygen. In order to build the docs you must first have Doxygen and graphviz installed. Both are available through the debian/ubuntu repositories. After this, cd to the `docs/` directory, and run `doxygen`. This should generate both HTML and LaTeX output. To view the HTML, simply navigate to `file:///path/to/joshai-challenge/docs/html/index.html` in your web browser of choice.

You can also go to the website here:

### Module Decomposition

This program was broken into multiple modules to allow for the decouplign of dependencies as well as scalability.

The main modules and their roles are as follows

| Module             | Description                                                                                                                                                                                                                                                                   |
| ------------------ | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Device Interface   | This is the primary logic of the application, this module is device specific and handles the device specific logic.                                                                                                                                                           |
| DataStore          | This is a module responsible for the access and validity of data from the server. Used to keep other subsystems from needing a direct client connection to the server.                                                                                                        |
| Connection Manager | This is the module responsible for the direct connection to the server. It is split out to allow for non reliance on any given http or web connection library / technology. If a change was made to gRPC for instance this is the only module that would need to be modified. |


### Third Party
This application relies onn a few third party libraries. As they are all header only they have been included in the `third-party` folder of this repo so there is no need to install them to build or run this application.

* [cpp-httplib](https://github.com/yhirose/cpp-httplib)
* [nlohmann/json](https://github.com/nlohmann/json?tab=readme-ov-file#conversion-from-stl-containers) 
* [tclap](https://tclap.sourceforge.net/)