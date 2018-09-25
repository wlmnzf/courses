library(readxl)
library(fGarch)  
library(rugarch)#garch拟合与预测  
library(TSA)#BIC准则确定arma阶数  eacf确定garch阶数  
library(tseries)  
library(zoo)#转换成时间序列类型  
library(forecast)#auto.arima() arma阶数确定方法  
library(psych)#数据描述统计分析  
library(ggplot2)#绘图  
library(ccgarch)#JB统计量  
###NASDAQ  
#数据预处理  
#StockData<-read.csv("sdata.csv",header = T)  #header = Falese   
StockData <- read_excel("sdata.xls")
time<-as.Date(StockData$'time',format="%Y/%m/%d")  
closeprice<-StockData$'closeprice'  
data<-zoo(closeprice,time)  
data
#选中局部数据  
selectdata=window(data, start = as.Date("1999-01-01"), end = as.Date("2010-12-01"))  
plot(selectdata,xlab="time",ylab="index",main="Shanghai Composite Index")  
rlog=log(selectdata)  
rlogdiff=diff(log(selectdata))*100 #数据转换  
rlogdifftime=index(rlogdiff)  
rlogdiffdata=coredata(rlogdiff)  

par(mfrow=c(1,3),oma=c(0.2,0.2,0.2,0.2))  
hist(rlogdiff,main="price",col="yellow",xlab="",ylim=c(0,0.4),probability=T)  
lines(density(rlogdiff),lwd=1);rug(rlogdiff)#first graph  
qqnorm(rlogdiff);qqline(rlogdiff)#second graph  
plot(rlogdiff,ylab="value");abline(h=0,lty=2)#third graph  


shapiro.test(rlogdiffdata)#值越大，越表示不是正态，P越小越非正态  

describe(rlogdiffdata)  
jb.test(rlogdiffdata)  


#若 ADF  p<0.0  认为序列是均值回归的  
adf.test(rlog,alt="stationary")#注意  这是未差分的数据  
adf.test(rlogdiffdata,alt="stationary")#这是差分后的数据  

#得先arima拟合模型，对残差进行LM检验  
armamodel=auto.arima(rlogdiff)#自动基于AIC最小准则，寻找最佳拟合模型  
armamodel  
plot(residuals(armamodel))  
par(mfrow=c(1,1))  
lmresult=McLeod.Li.test(y=residuals(armamodel))#残差arch效应很显著  


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
shapiro.test(coredata(residuals(myfit)))#值越大，越表示不是正态，P越小越非正态  
#残差相关性检验  
acf(coredata(residuals(myfit)))  
acf(residuals(myfit))  
plot(myfit,which=10)  
plot(myfit,which=11)  
#系数是否显著  
myfit #看P值是否够小  
#拟合效果  残差如何  
plot(myfit,which=3)  
plot(residuals(myfit)) #看残差  


plot(myfit,which=12)  

spec = ugarchspec(variance.model = list(model = "sGARCH"), distribution.model = "std")  
cl = makePSOCKcluster(10)  
cl
#滚动预测  
roll = ugarchroll(spec, rlogdiff, n.start =10,refit.every = 1,  
                  refit.window = "moving", solver = "hybrid", calculate.VaR = TRUE,  
                  VaR.alpha = c(0.01, 0.05), cluster = cl,keep.coef = TRUE)  



report(roll, type = "fpm")  

plot(roll,which=4,VaR.alpha=0.01) 