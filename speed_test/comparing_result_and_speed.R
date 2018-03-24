# Useful libraries
library(tidyverse)
library(ggrepel)

# Load qualV library used in GenMapComp
library(qualV)

# Load LCIS
library(LCSLCIS)

# values to test

nbMarkers <- c(100, 500, 1000,5000, 10000,50000,75000,100000)
#nbMarkers <- c(100)
#avgBuckSizes <- c(1, 10, 100, 1000)
avgBuckSizes <- c(10)

# Create a result data frame
nbRes <- length(nbMarkers)*length(avgBuckSizes)
resInfo <- c("n", "avgBuck","qualV_LCS", "LCSLCIS_LCS", "LCSLCIS_LCIS","qualV_LCS_time","LCSLCIS_LCS_time","LCSLCIS_LCIS_time" )
result <- data.frame( matrix(0, nbRes, length(resInfo)))
colnames(result) <- resInfo

# Try with several item numbers
numTest <- 0
percentBucketSwap <- 0.05 # how many bucket swaps? here 10% as each swap move 2 buckets we use 0.05 to get 10% of buckets having different positions in the two orders
percentMArkerMove <- 0.05 # move 5% of the markers in other buckets (including potentially some not used previously)

for(n in nbMarkers){
  for (avgBucketSize in avgBuckSizes )
  {
    numTest <- numTest +1
    numBucket <- n/avgBucketSize
    
    # create a first bucket order
    o1 <- data.frame(
      V1 = paste("item", seq(1:n), sep="-"),
      V2 = sample(1:numBucket,n, replace = T)
    )
    
    # create a second bucket order by altering the first one in 2 steps
    o2 <- o1
    
    # step 1: swap some buckets
    buckets <- unique(o1$V2)# get the list of existing buckets (each unique position)
    nmodifB <- length(buckets) * percentBucketSwap
     
    if(nmodifB >=1) # otherwise always do at least one swap
    {
      for (s in 1:nmodifB) # for each swap
      {
        toswap <-sample(buckets,2, replace = F) # select two differents busket
        id1 <-o2$V2==toswap[1] # identify rows/markers corresponding to each buccket
        id2 <-o2$V2==toswap[2]
        o2$V2[id1]<-rep(toswap[2], length(o2$V2[id1]))# modifify makers bucket numbers
        o2$V2[id2]<-rep(toswap[1], length(o2$V2[id2]))
      }
    }
    
    # step 2 move some markers anywhere
    nmodif <- n * percentMArkerMove
    if(nmodif >=1) # otherwise always do at least one swap
    {
      tomodif <- sample(seq(1,n), nmodif)
      o2$V2[tomodif] <- sample(seq(1,n), nmodif)
    }
    # Have a look (optionnal)
    #show_connection(o1, o2)
    
    # 1 ------- Make the calculation with qualV, ignore preparation and sort in time estimation
    don <- merge(o1, o2, by.x="V1", by.y="V1", all=T) %>%
      rename(name=V1, order1=V2.x, order2=V2.y) %>%
      na.omit()
    
    # Find the non-problematic markers and count them
    M1=don$name[order(don[,2], don[,3])]
    M2=don$name[order(don[,3], don[,2])]
    
    if(n<=30000)
    {
      a <- Sys.time()
      res <- qualV::LCS( as.character(M1),as.character(M2) )$LCS
    
      b <- Sys.time()
      qualV_LCS_time <- b - a
      qualV_LCS <- length(res)
    }
    else
    {
      qualV_LCS_time <- -1
      qualV_LCS <- 0
    }
    # 2 ------- Make the calculation with LCSLCIS
    o1_sorted <- arrange(o1, V2)
    o2_sorted <- arrange(o2, V2)
    
    a <- Sys.time()
    res <- LCSLCIS::LCS(o1_sorted,o2_sorted)
    b <- Sys.time()
    LCSLCIS_LCS <- length(res$LCS)
    LCSLCIS_LCS_time <- b - a
    
    
    a <- Sys.time()
    res <- LCSLCIS::LCIS(o1_sorted,o2_sorted)
    b <- Sys.time()
    LCSLCIS_LCIS <- length(res$LCIS)
    LCSLCIS_LCIS_time <- b - a
    
    
    # 3 ------- Fill result table
    
    result[numTest,] <- c(n, avgBucketSize,qualV_LCS, LCSLCIS_LCS, LCSLCIS_LCIS,qualV_LCS_time,LCSLCIS_LCS_time,LCSLCIS_LCIS_time )

  }
}



#show_connection(o1, o2, tokeep=res$LCS)

# Plot the result
#png("speed_test_result.png")
#result %>%
#  gather(algo, time, -1) %>%
#  ggplot( aes(x=n, y=time, color=algo)) +
#  geom_line() +
#  geom_point() +
#  xlab("# of items") +
#  ylab("time ellapsed")
#dev.off()

#png("speed_test_result.png")
png("speed_test_result.png")
result %>% 
  select(n, avgBuck, "LCS"=LCSLCIS_LCS_time, "LCIS"=LCSLCIS_LCIS_time, "qualV.LCS"=qualV_LCS_time) %>% 
  gather("method","time",LCS,LCIS,qualV.LCS)%>%
  arrange(time)%>%
  filter(time>=0)%>%
  ggplot( aes(x=n, time, color=method)) +
  #geom_point(aes(shape=as.factor(avgBuck)))+
  geom_point(aes(shape=method))+
  geom_line(size=1.1, aes(linetype=method))+
  scale_linetype_manual(values=c("solid","dotdash", "dotted"))+
  xlab("number of markers") +
  ylab("time (in secondes)")
dev.off()
# Show result
write.table(result, file="speed_test_result.csv", sep=",", row.names=FALSE)

# Key number, with 12800 markers, LCSLCIS is x time faster. x= 21
result$qualV[8] /  result$LCSLCIS[8]

