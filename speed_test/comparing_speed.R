# Useful libraries
library(tidyverse)
library(ggrepel)

# Load qualV library used in GenMapComp
library(qualV)

# Load LCIS
library(LCSLCIS)

# Create a result data frame
result <- data.frame( matrix(0, 8, 3))
colnames(result) <- c("n", "qualV", "LCSLCIS")

# Try with several item numbers
num <- 0
for(i in c(100, 200, 400, 800, 1600, 3200, 6400, 12800)){

  # Number of item in the bucket orders:
  n <- i
  num <- num +1

  # create a first bucket order
  o1 <- data.frame(
    V1 = paste("item", seq(1:n), sep="-"),
    V2 = seq(1,n) + rnorm(n)
  )
  
  # create a second bucket order: almost the same, juste modify 10% of the markers
  nmodif <- n * 0.01
  tomodif <- sample(seq(1,n), nmodif)
  o2 <- o1
  o2$V2[tomodif] <- sample(seq(1,n), nmodif)
  
  # Have a look (optionnal)
  #show_connection(o1, o2, showName = FALSE)
  
  # 1 ------- Make the calculation with qualV
  don <- merge(o1, o2, by.x="V1", by.y="V1", all=T) %>%
    rename(name=V1, order1=V2.x, order2=V2.y) %>%
    na.omit()
  
  # Find the non-problematic markers and count them
  M1=don$name[order(don[,2], don[,3])]
  M2=don$name[order(don[,3], don[,2])]
  a <- Sys.time()
  my_notprobmarkers <- qualV::LCS( as.character(M1),as.character(M2) )$LCS
  b <- Sys.time()
  qualV <- b - a
  
  # 2 ------- Make the calculation with LCSLCIS
  a <- Sys.time()
  res <- LCIS(o1,o2)
  b <- Sys.time()
  LCSLCIS <- b - a
  
  # 3 ------- Fill result table
  result[num,] <- c(n, qualV, LCSLCIS)

}

# Plot the result
png("speed_test_result.png")
result %>%
  gather(algo, time, -1) %>%
  ggplot( aes(x=n, y=time, color=algo)) +
    geom_line() +
    geom_point() +
    xlab("# of items") +
    ylab("time ellapsed")
dev.off()

# Show result
write.table(result, file="speed_test_result.csv", sep=",", row.names=FALSE)

# Key number, with 12800 markers, LCSLCIS is x time faster. x= 21
result$qualV[8] /  result$LCSLCIS[8]







