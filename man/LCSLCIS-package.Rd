\name{LCSLCIS-package}
\alias{LCSLCIS-package}
\alias{LCSLCIS}
\docType{package}
\title{
  Find the Longuest Common Sequence between 2 lists of items.
}
\description{
  LCSLCIS takes 2 list of items as input. Each items having a position. It computes the 
  biggest common sequence between these 2 lists.
}
\details{
  Two functions are offered. When several items are at the same position, LCS will return 
  all of them. LCIS will return only one item per position.
}
\author{
Lisa de Mattéo

Maintainer: Lisa de Mattéo <dematteo@openmailbox.org>
}
\references{
  in press
}
\keyword{ package }
\seealso{
  the genetic map comparator
}
\examples{
  \dontrun{
  # Create 2 lists
  o1 <- data.frame( 
     V1=c("g","h","c","f","e","d","m","q","r","a","b","n","o","p"), 
    V2=c(1.1,1.1,3.4,3.4,3.4,3.4,3.5,3.5,6.6,6.6,7.2,7.2,8,8)
    )
  o2 <- data.frame( 
    V1=c("k","a","b","l","c","e","d","f","i","j","h","g"), 
    V2=c(0.1,1.2,1.2,4.2,4.2,5,5,5,5.3,5.3,6.7,6.7)
    )

  # show the connection between all items
  show_connection(o1,o2)

  # Run LCS and plot only the remaining markers
  res <- LCS(o1,o2)
  show_connection(o1, o2, tokeep=res$LCS)

  }
}
