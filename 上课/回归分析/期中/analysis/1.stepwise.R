#1.stepwise.R
#回归分析

library(readxl)

data <- read_excel("data.xls")
#rm(xlsx_example)

#x49,x50,x51为哑变量分别为负影响，无影响，正影响
tlm<-lm(y~x1+x2+x3+x4+x5+x6+x7+x8+x9+x10+
          x11+x12+x13+x14+x15+x16+x17+x18+x19+x20+
          x21+x22+x23+x34+x25+x26+x17+x28+x29+x30+
          x31+x32+x33+x34+x35+x36+x37+x38+x39+x40+
          x41+x42+x43+x44+x45+x46+x47+x48+x49+x50+
          x51,data=data)
#summary(tlm)
tstep<-step(tlm)
drop1(tstep)
summary(tstep)
