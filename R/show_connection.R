#' A Cat Function
#'
#' This function allows you to express your love of cats.
#' @param love Do you love cats? Defaults to TRUE.
#' @keywords cats
#' @export
#' @examples
#' cat_function()

# A function that plot the relationship between 2 lists:
show_connection <- function(o1, o2, tokeep=NA){
  
  # Prepare the data: merge the 2 lists and make a 'long' or 'tidy' format:
  don <- merge(o1, o2, by.x="V1", by.y="V1", all=T) %>%
    rename(name=V1, order1=V2.x, order2=V2.y) %>%
    gather(ord, position, -1) %>%
    na.omit()
  
  # It is possible to keep only a sample of the elements:
  if( all(is.na(tokeep))==FALSE ){ don <- don %>% filter(name %in% tokeep) }
  
  # Make the plot
  ggplot( don, aes(x=ord, y=position, label=name, group=name, color=name)) +
    geom_segment( x=1, xend=1, y=-max(o1$V2), yend=0, color="black", size=0.3) +
    geom_segment( x=2, xend=2, y=-max(o2$V2), yend=0, color="black", size=0.3) +
    geom_point() +
    geom_text_repel( data=don %>% filter(ord=="order1"), direction = "y" , segment.color = "grey", box.padding = 0.001, color="black", xlim=c(0,.95)) +
    geom_text_repel( data=don %>% filter(ord=="order2"), direction = "y" , segment.color = "grey", box.padding = 0.001, color="black", xlim=c(2.05,NA)) +
    geom_line() +
    theme_void() +
    theme(legend.position="none") +
    #scale_y_reverse() +
    xlab("") +
    ylim( max(c(o1$V2, o2$V2)), 0)
}
