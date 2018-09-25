#2.fluctration.R
#波动预测

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


WTLData <- read_excel("sdata.xls")
time<-as.Date(WTLData$'time',format="%Y/%m/%d")  
WTLPrice<-WTLData$'price'  
data<-zoo(WTLPrice,time)  
data

selectdata=window(data, start = as.Date("1999-01-01"), end = as.Date("2010-12-01"))  
plot(selectdata,xlab="time",ylab="index",main="WTL Price")  
rlog=log(selectdata)  
rlogdiff=diff(log(selectdata))*100 
rlogdifftime=index(rlogdiff)  
rlogdiffdata=coredata(rlogdiff)  

par(mfrow=c(1,3),oma=c(0.2,0.2,0.2,0.2))  
hist(rlogdiff,main="price",col="yellow",xlab="",ylim=c(0,0.4),probability=T)  
lines(density(rlogdiff),lwd=1);rug(rlogdiff)
qqnorm(rlogdiff);qqline(rlogdiff)  
plot(rlogdiff,ylab="value");abline(h=0,lty=2)


shapiro.test(rlogdiffdata)#值越大，越表示不是正态，P越小越非正态  

describe(rlogdiffdata)  
jb.test(rlogdiffdata)  


  
adf.test(rlog,alt="stationary")
adf.test(rlogdiffdata,alt="stationary")#这是差分后的数据  

#得先arima拟合模型，对残差进行LM检验  
armamodel=auto.arima(rlogdiff)
armamodel  
plot(residuals(armamodel))  
par(mfrow=c(1,1))  
lmresult=McLeod.Li.test(y=residuals(armamodel))


myspec=ugarchspec(  
  variance.model = list(model = "eGARCH", garchOrder = c(1, 1)),  
  mean.model = list(armaOrder = c(0,0), include.mean = TRUE),  
  distribution.model = "std"  
)  

myfit=ugarchfit(myspec,data=rlogdiff,solver="gosolnp")  
myfit  


#残差正态性检验  
plot(myfit,which=8)  
plot(myfit,which=9)  
shapiro.test(coredata(residuals(myfit)))

#残差相关性检验  
acf(coredata(residuals(myfit)))  
acf(residuals(myfit))  
plot(myfit,which=10)  
plot(myfit,which=11)  

#系数是否显著  
myfit 


#拟合效果  残差如何  
plot(myfit,which=3)  
plot(residuals(myfit))
plot(myfit,which=12)  

spec = ugarchspec(variance.model = list(model = "sGARCH"), distribution.model = "std")  
cl = makePSOCKcluster(10)  
cl

#滚动
roll = ugarchroll(spec, rlogdiff, n.start =10,refit.every = 1,  
                  refit.window = "moving", solver = "hybrid", calculate.VaR = TRUE,  
                  VaR.alpha = c(0.01, 0.05), cluster = cl,keep.coef = TRUE)  


report(roll, type = "fpm")  

plot(roll,which=4,VaR.alpha=0.01) 


