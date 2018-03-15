#' A function to draw the relationship between 2 lists
#'
#' This function takes 2 lists o1 and o2 and build a diagram showing the relationship between every items.
#' @param o1 the first list. Must be 2 columns called V1 and V2. V1 being the name
#' of the item, V2 its position
#' @param o2 the second list. Must be 2 columns called V1 and V2. V1 being the name
#' of the item, V2 its position
#' @param tokeep a vector of item to keep on the graphic. All item not present in this list
#' will be discarded. By default, all markers are kept.
#' @param showName wether or not you want to show item names on the graphic.If you're working
#' with a huge number of item, you probably don't want to show them
#' @param highlight by default each connection is represented with a specific color. This option
#' allows to plot every connection in grey, except connection betwwen a specific set of markers.
#' @keywords plot connection list
#' @author Lisa de Matteo
#' @export
#' @examples
#' # Create 2 lists
#' o1 <- data.frame( 
#'    V1=c("g","h","c","f","e","d","m","q","r","a","b","n","o","p"), 
#'    V2=c(1.1,1.1,3.4,3.4,3.4,3.4,3.5,3.5,6.6,6.6,7.2,7.2,8,8)
#'    )
#' o2 <- data.frame( 
#'    V1=c("k","a","b","l","c","e","d","f","i","j","h","g"), 
#'    V2=c(0.1,1.2,1.2,4.2,4.2,5,5,5,5.3,5.3,6.7,6.7)
#'    )
#'
#' # show the connection between all items
#' show_connection(o1,o2)
#'
#' # Run LCS and plot only the remaining markers
#' res <- LCS(o1,o2)
#' show_connection(o1, o2, tokeep=res$LCS)

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
  if( !is.na(highlight) ){
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
        geom_text_repel( data=don %>% filter(ord=="order1"), direction = "y" , segment.color = "grey", box.padding = 0.001, color="black", xlim=c(0,.95)) +
        geom_text_repel( data=don %>% filter(ord=="order2"), direction = "y" , segment.color = "grey", box.padding = 0.001, color="black", xlim=c(2.05,NA))
  }
    
  p
  
}








