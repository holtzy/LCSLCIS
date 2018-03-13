[![Project Status: Active The project has reached a stable, usable state and is being actively developed.](http://www.repostatus.org/badges/latest/active.svg)](http://www.repostatus.org/#active) [![Licence](https://img.shields.io/badge/licence-GPL--3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.en.html)
[![minimal R version](https://img.shields.io/badge/R%3E%3D-3.2.4-6666ff.svg)](https://cran.r-project.org/) [![packageversion](https://img.shields.io/badge/Package%20version-0.1.0-orange.svg?style=flat-square)](commits/develop) [![Last-changedate](https://img.shields.io/badge/last%20change-2018--03--13-yellowgreen.svg)](/commits/master)

LCSLCIS
-------

An R library to compute the longuest common sequences. It contains 2 functions:

-   **LCS**: Longest Common Sequence
-   **LCIS**: Longest Common I Sequence

<br><br>

Installation
------------

The LCSLCIS library is not on CRAN yet. However you can easily install it from this github repository:

    #install.packages("devtools")
    library(devtools) 
    install_github("holtzy/LCSLCIS")
    library(LCSLCIS)

Example
-------

Let's consider 2 lists of elements. Each element has a specific position in each list. We can build such a dataset and represent it in the following diagram:

    o1 <- data.frame( 
        V1=c("g","h","c","f","e","d","m","q","r","a","b","n","o","p"), 
        V2=c(1.1,1.1,3.4,3.4,3.4,3.4,3.5,3.5,6.6,6.6,7.2,7.2,8,8)
        )
    o2 <- data.frame( 
        V1=c("k","a","b","l","c","e","d","f","i","j","h","g"), 
        V2=c(0.1,1.2,1.2,4.2,4.2,5,5,5,5.3,5.3,6.7,6.7)
        )

The library provides a function that allows to visualize the relationship between these 2 lists.

    show_connection(o1, o2)

<img align="center" src="img/fig1.png">

We can now try to find the biggest set of elements without having any mismatch in their order. This can be done using the `LCS` or the `LCIS` function. Here is an example using LCS:

    # run the LCS function
    res <- LCS(o1,o2)

    # The number of elements we can keep without order mismatch is 
    res$LLCS

    # The elements we can keep are:
    res$LCS

    # We can represent that on the diagram:
    show_connection(o1, o2, tokeep=res$LCS)

<img align="center" src="img/fig2.png">

Citing
------

If you find LCSLCSI useful, please cite:
in press.

Authors
-------

Vincent Ranwez: [homepage](https://sites.google.com/site/ranwez/) Other:
