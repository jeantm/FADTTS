#<p align="center"><img src="doc/Logo/logoFADTTSter_small.jpg" alt="LogoFADTTSer_small"/></p>


Functional Analysis of Diffusion Tensor Tract Statistics (FADTTS) is used in the Diffusion Tensor Imaging (DTI) fiber tract analysis [NAMIC framework] implemented by UNC/Utah.
FADTTS is Matlab (MathWorks Inc, MA, USA) based and previous knowledge of coding is necessary to operate it.
FADTTSter was created to overcome this issue and make the statistical analysis accessible to any non-technical researcher.

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


### Statistical data plotting
Used for the visualization of the data obtained after running the .m script, the statistical data plotting enables the user to customized her/his results.
* Change title
* Change line colors
* Change covariates name
* ...


> Not only is FADTTSter practical but it enables any investigator to perform DTI analysis efficiently.
> For complete information on how to how FADTTSter please refere to the [User Guide].


### Version
3.3.1

### License
Apache 2.0

[//]: #

   [NAMIC framework]: <http://journal.frontiersin.org/article/10.3389/fninf.2013.00051/full>
   [User Guide]: <https://github.com/jeantm/FADTTSter/blob/master/doc/UserGuide/UserGuide.txt>

