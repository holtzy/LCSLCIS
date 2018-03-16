\name{LCSLCIS-package}
\alias{LCSLCIS-package}
\alias{LCSLCIS}
\docType{package}
\title{
  Longest Common (Induced) Subsequence of two bucket orders.
}
\description{
  LCSLCIS allows to identify and visualize the longest  common subsequences of two input orders.
}
\details{
  Three functions are offered: 
  - LCS: Longest Common Subsequence that arbitrary break ties 
  - LCIS: Longest Common Subsequence that do not arbitrary break ties
  - show_connection: to visually compare two orders
}
\author{
Lisa de Matteo, Yan Holtz, Vincent Ranwez, Severine Berard
}
\references{
  paper under submission
}
\keyword{subsequence}
\keyword{order}
\keyword{geneticmap}
\keyword{Subsequence}
\keyword{bucket order}
\keyword{Longest Common Subsequence}
\keyword{LCS}
\keyword{fast}
\seealso{
  the genetic map comparator
}
\examples{
  \dontrun{
  # create 2 orders of markers. Each order is represented by a dataframe made of two columns, the first (V1) contains marker names (e.g. a, e, f) the second (V2) contains marker position in this order (e.g. 1.1, 5, 6.7, 8) 
  o1 <- data.frame( 
     V1=c("g","h","c","f","e","d","m","q","r","a","b","n","o","p"), 
    V2=c(1.1,1.1,3.4,3.4,3.4,3.4,3.5,3.5,6.6,6.6,7.2,7.2,8,8)
    )
  o2 <- data.frame( 
    V1=c("k","a","b","l","c","e","d","f","i","j","h","g"), 
    V2=c(0.1,1.2,1.2,4.2,4.2,5,5,5,5.3,5.3,6.7,6.7)
    )

  # show the connection between marker positions in o1 and o2
  show_connection(o1,o2)

  # Run LCS and compare the position of those LCS markers in o1 and o2
  res <- LCS(o1,o2)
  show_connection(o1, o2, tokeep=res$LCS)

  }
}
