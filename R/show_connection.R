#' A function to draw the relationship between 2 (bucket) orders
#'
#' This function takes 2 marker orders o1 and o2 and build a diagram to ease their comparison. The two orders are represented by two parallel vertical lines, and marker are positoned along those lines based on their positions within o1 and o2. Every time a marker is present in both o1 and o2 (same name) a new line is drawn between: the position of this marker on the vertical line representing o1 and its position on o2 (by default a different color is used for each marker).
#' @param o1 the first order, provided as a dataframe made of two columns named V1 and V2; V1 contains marker names while V2 contains their position
#' @param o2 the second order, provided as a dataframe made of two columns named V1 and V2; V1 contains marker names while V2 contains their position
#' @param tokeep a vector containg the names of the markers to be kept for this graphic, other markers are disgarded. By default, all markers are kept.
#' @param showName a boolean indicating wether or not marker names should be displayed (for large datasets displaying marker names often result in overly cluttered charts)
#' @param highlight a vector containg the names of the markers to be highlight for this graphic. The lines linking those markers will be drawn in dark grey, while other (e.g. those not part of the LCS) will be drawn in red instead of being masked as with the tokeep option.
#' @keywords connection list plot
#' @author Yan Holtz, Lisa de Matteo, Vincent Ranwez, Severine Berard
#' @export
#' @examples
#' # create 2 orders of markers. Each order is represented by a dataframe made of two columns, the first (V1) contains marker names (e.g, a, e, f) the second (V2) contains marker position in this order (e.g 1.1, 5, 6.7, 8) 
#' o1 <- data.frame( 
#'    V1=c("g","h","c","f","e","d","m","q","r","a","b","n","o","p"), 
#'    V2=c(1.1,1.1,3.4,3.4,3.4,3.4,3.5,3.5,6.6,6.6,7.2,7.2,8,8)
#'    )
#' o2 <- data.frame( 
#'    V1=c("k","a","b","l","c","e","d","f","i","j","h","g"), 
#'    V2=c(0.1,1.2,1.2,4.2,4.2,5,5,5,5.3,5.3,6.7,6.7)
#'    )
#' 
#' # show the connection between the markers of o1 and o2
#' show_connection(o1,o2)
#' 
#' # Run LCS and plot only the connection between o1 and o2 for their LCS markers (tokeep=res$LCS) or higlight those connections (highlight=res$LCS)
#' res <- LCS(o1,o2)
#' show_connection(o1, o2, tokeep=res$LCS)
#' show_connection(o1, o2, highlight =res$LCS)

# A function that plot the relationship between 2 lists:
show_connection <- function(o1, o2, tokeep=NA, showName=TRUE, highlight=NA){
  
  # Prepare the data: merge the 2 lists and make a 'long' or 'tidy' format:
  don <- merge(o1, o2, by.x="V1", by.y="V1", all=T) %>%
    rename(name=V1, order1=V2.x, order2=V2.y) %>%
    gather(ord, position, -1) %>%
    na.omit()
  
  # Keep only a sample of the elements if the tokeep option is activated:
  if( all(is.na(tokeep))==FALSE ){ don <- don %>% filter(name %in% tokeep) }
  
  # Make the plot
  p <- ggplot( don, aes(x=ord, y=position, label=name, group=name, color=name))

  # If user want to highlight a few connection, I do:
  if( all(!is.na(highlight)) ){
    don$mycolor <- ifelse( don$name %in% highlight, "part of LCS", "excluded of LCS")
    p <- ggplot( don, aes(x=ord, y=position, label=name, group=name, color=mycolor)) +
        scale_color_manual(values=c("red", "black"))
  }

  p <- p +
    geom_segment( x=1, xend=1, y=-max(o1$V2), yend=0, color="black", size=0.3) +
    geom_segment( x=2, xend=2, y=-max(o2$V2), yend=0, color="black", size=0.3) +
    geom_point() +
    geom_line() +
    theme_void() +
    theme(legend.position="none") +
    xlab("") +
    ylim( max(c(o1$V2, o2$V2)), 0)

  # Add item names
  if(showName==TRUE){
    p <- p +
        geom_text_repel( data=don %>% filter(ord=="order1"), direction = "y" , segment.color = "grey", box.padding = unit(0.001, "lines"), color="black", xlim=c(0,.95)) +
        geom_text_repel( data=don %>% filter(ord=="order2"), direction = "y" , segment.color = "grey", box.padding = unit(0.001, "lines"), color="black", xlim=c(2.05,NA))
  }
    
  p
  
}








