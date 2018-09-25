#3.influence.R
#影响分析

library(readxl)
data <- read_excel("data.xls")
options(max.print=1000000)

lm.sol<-lm(y~., data=data)
summary(lm.sol)


#找影响点
influence.measures(lm.sol)#得到cook

#找异常值
stdres<-rstudent(lm.sol);   #学生化方差
t=sqrt((144-51-1)*stdres^2/(144-51-stdres^2))  #t检验法
res<-t>abs(qt(.025, df=92))
res
