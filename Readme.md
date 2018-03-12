LCSLCIS
--------

An R library to compute the longuest common sequences. It contains 2 functions: 

- **LCS**
- **LCIS**

## Installation
The LCSLCIS library is not on CRAN yet. However you can easily install it from this github repository:
```
#install.packages("devtools")
library(devtools) 
install_github("holtzy/LCSLCIS")
library(LCSLCIS)
```



## Example
Let's consider 2 list of elements. Each element has a specific position in each list. We can build such a dataset and represent it in the following diagram:  

```
o1 <- data.frame( V1=c("g","h","c","f","e","d","m","q","r","a","b","n","o","p"), V2=c(1.1,1.1,3.4,3.4,3.4,3.4,3.5,3.5,6.6,6.6,7.2,7.2,8,8))
o2 <- data.frame( V1=c("k","a","b","l","c","e","d","f","i","j","h","g"), V2=c(0.1,1.2,1.2,4.2,4.2,5,5,5,5.3,5.3,6.7,6.7))
```

<img align="center" src="img/fig1.png">

We can now try to find..

## Authors

bla
