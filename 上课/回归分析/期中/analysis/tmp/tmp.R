library(MASS)  
data(oats)  
names(oats) = c('block', 'variety', 'nitrogen', 'yield')  

library(lme4)  
m1.lme4 = lmer(yield ~ variety*nitrogen + (1|block),  
               data = oats)  

summary(m1.lme4)  
anova(m1.lme4)  


library(lme4)  # load library
library(arm)  # convenience functions for regression in R
lmm.data <- read.table("http://www.unt.edu/rss/class/Jon/R_SC/Module9/lmm.data.txt", 
                       header = TRUE, sep = ",", na.strings = "NA", dec = ".", strip.white = TRUE)
# summary(lmm.data)
head(lmm.data)




library(readxl)
library(zoo)
library(FinTS)  
library(timeDate)
library(timeSeries)
library(fBasics)
library(fGarch) 
library(fGarch)  
library(rugarch)
library(TSA)
library(tseries)  
library(zoo) 
library(forecast)
library(psych)
library(ggplot2)
library(ccgarch)
library(tseries)


data1<-read.csv("price.csv")   
close<-data1[,2]   
n<-length(close)       
return<-log(close[2:n])-log(close[1:n-1])    # 收益率
return<-diff(log(close))


Close.ts<-ts(close,start=c(1999,1),freq=12) 
Return.ts<-ts(return,start=c(1999,1),freq=12)
#par(mfrow=c(2,1))          #建立一个以两行一列排列图形的图形窗口
#plot(Close.ts,type="l",main="(a) Daily Closing Price of       
#     000001.SH",xlab="Date", ylab="Price", cex.main=0.95,las=1)     
#plot(Return.ts,type="l",main="(b) Daily Rate of Return of  
#    000001.SH",xlab="Date", ylab="Rate", cex.main=0.95, las=1)


u<-sum(return)/n                  # 求均值
e<-sqrt(sum((return-u)^2)/(n-1))  # 求标准差
s<-sum((return-u)^3)/((n-1)*e^3)  # 求偏度
k<-sum((return-u)^4)/((n-1)*e^4)  # 求峰度
jarque.bera.test(return)         # JB正态性检验

#par(mfrow=c(2,1))   
#acf(return,main='',xlab='Lag (a)',ylab='ACF',las=1) #画自相关图 
#title(main='(a) the ACF of Return',cex.main=0.95)  #为图形加标题，并设置标题大小
#pacf(return,main='',xlab='Lag (b)',ylab='PACF',las=1) #画偏自相关图   
#title(main='(b) the PACF of Return',cex.main=0.95)

#par(mfrow=c(2,1))  
return.square<-return^2
#acf(return.square,main='',xlab='Lag (c)',ylab='ACF',las=1)               
#title(main='(a) the ACF of Return Square',cex.main=0.95)
#pacf(return.square,main='',xlab='Lag (d)',ylab='PACF',las=1)    
#title(main='(b) the PACF of Return Square',cex.main=0.95)



ArchTest(return,lag=12)  #滞后 12 期


m1<-garchFit(~1+garch(1,1),data=return,trace=F) #拟合GARCH（1,1）模型
m2<-garchFit(~1+garch(1,2),data=return,trace=F) #拟合GARCH（1,2）模型
m3<-garchFit(~1+garch(2,1),data=return,trace=F) #拟合GARCH（2,1）模型
m4<-garchFit(~1+garch(2,2),data=return,trace=F) #拟合GARCH（2,2）模型
summary(m1)   
summary(m2)
summary(m3)
summary(m4)

resi<-residuals(m1,standardize=T)   #获得标准化残差
res<-ts(resi,frequency=250,start=c(2000)) 
plot(res,xlab='Date',ylab='st.resi',type='l')
par(mfcol=c(2,2))
acf(resi,lag=24)
acf(resi^2,lag=24)
pacf(resi,lag=24)
pacf(resi^2,lag=24)


Box.test(resi^2,lag=10,type='Ljung')  #残差平方的滞后10阶自相关检验
Box.test(resi^2,lag=15,type='Ljung')  #残差平方的滞后15阶自相关检验
Box.test(resi^2,lag=20,type='Ljung')  #残差平方的滞后20阶自相关检验


