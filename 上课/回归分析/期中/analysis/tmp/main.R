y=c(160,260,210,265,240,220,275,160,275,250)
x1=c(70,75,65,74,72,68,78,66,70,65)
x2=c(35,40,40,42,38,45,42,36,44,42)
x3=c(1,2.4,2,3,1.2,1.5,4,2,3.2,3)
shuju<-data.frame(y,x1,x2,x3)
shuju
shuju.reg=lm(y~.,data=shuju)#(2)求三元线性回归方程(3)拟合优度检验
summary(shuju.reg)


#异常值与强影响点
#残差
shuju.stdres<-rstudent(shuju.reg);shuju.stdres     
#中心化杠杆值
ch<-hatvalues(shuju.reg)-1/10;ch
#Cook距离
shuju.cooks<-cooks.distance(shuju.reg);shuju.cooks
#找异常值
isoutlier<-abs(shuju.stdres)>3;isoutlier  #残差选择正负3倍标准差以外的
isch<-ch>2*1/10;isch     #杠杆值大于两倍的hba
iscooks<-shuju.cooks>1;iscooks    #库克距离大于1的
isch*iscooks       #杠杆值和库克距离结合起来判断



library(readxl)
data <- read_excel("data.xls")
#rm(xlsx_example)
intellect<-data.frame(
  y=c(95,71,83,91,102,87,93,100,104,94,113,96,83,84,102,100,105,57,121,86,100),
  x=c(15,26,10,9,15,20,18,11,8,20,7,9,10,11,11,10,12,42,17,11,10)
) 
lm.sol<-lm(y~., data=data)
summary(lm.sol)
stdres<-rstudent(lm.sol);   #学生化方差
t=sqrt((144-51-1)*stdres^2/(144-51-stdres^2))  #t检验法
res<-t>abs(qt(.025, df=92))
res
influence.measures(lm.sol)


#op <- par(mfrow=c(2,2), mar=0.4+c(4,4,1,1), 
#          oma= c(0,0,2,0))
#plot(lm.sol, 1:4)
#par(op)