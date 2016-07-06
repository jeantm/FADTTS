# What is FADTTSter?

<p align="center"><img src="doc/Logo/logoFADTTSter_small.jpg" alt="LogoFADTTSer_small"/></p>

FADTTSter is a user-friendly version of FADTTS directed to users without advanced coding skills.
FADTTS is Matlab (MathWorks Inc, MA, USA) based and to use this software, coding knowledge is necessary to operate it.
FADTTSter was created to overcome this issue and make the statistical analysis accessible to any non-technical researcher.
FADTTSter is a command line based module as well as an GUI based tool.
Not only is FADTTSter practical but it enables any investigator to perform DTI analysis efficiently.

This tool can be divided in two main parts, each one working independently.

### Matlab script generation
The .m script, as well as its inputs, is automatically generated based on the information provided (diffusion profiles, subjects, qc threshold, nbr of permutations, p-value threshold, ...).

* Inputs
    - Profile fiber(s) (at least one)
    - Covariate file
    - Subject list (optional)
    - FADTTS matlab toolbox


* Important Outputs
    - .m script
    - .csv inputs (profile fiber(s) and covariate file) for the script
    - statistical data .csv file (only if the script has been run)
    - matlab plots (only if the script has been run)

> Matlab R2013b or later version is required to run the .m script.


### Statistical data plotting
Used for the visualization of the data obtained after running the .m script, the statistical data plotting enables the user to customized her/his results.
* Change title
* Change line colors
* Change covariates name
* ...


> For complete information on how to use FADTTSter please refer to the [User Guide].


# Build instructions

### Requirements
* Qt5
* SlicerExecutionModel
* VTK


### Build
* Get files from Github
```sh
$ git clone git@github.com:NIRALUser/FADTTSter.git
```

* Make an out-of-source build directory
```sh
$ mkdir ./FADTTSter-build
$ cd FADTTSter-build
```

* Configure the build
```sh
$ ccmake ../FADTTSter/src
$ make
```

* Get the executable
```sh
$ ./bin/FADTTSter
```

> To intall FADTTSter and all its dependencies with a superBuild refer to [DTI Fiber Tract Statistics].



# Version
3.3.1

# License
Apache 2.0

# More
The tool can also be found on [NITRC]

[//]: #

   [DTI Fiber Tract Statistics]: <https://github.com/NIRALUser/DTIFiberTractStatistics>
   [User Guide]: <https://github.com/jeantm/FADTTSter/blob/master/doc/UserGuide/UserGuide.txt>
   [NITRC]: <http://www.nitrc.org>

