#4.forecast.R
#预测分析

#HoltWinters
library(readxl)
library(forecast)
data<-read.csv("price.csv")   
infy.ts<-ts(data$WTI.Price,start=c(1999,1),frequency=12)
infy.hw<-HoltWinters(infy.ts)
plot(infy.hw,col="blue",col.predicted="red");

infy.forcast<-forecast(infy.hw,h=5);
infy.forcast
plot(infy.forcast);


#arima
library(readxl)
library(forecast)
data<-read.csv("price.csv")   
infy.ts<-ts(data$WTI.Price,start=c(1999,1),frequency=12)
infy.arima<-auto.arima(infy.ts)
infy.forcast<-forecast(infy.arima,h=5)
infy.forcast
plot(infy.forcast)