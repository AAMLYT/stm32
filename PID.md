

# PID

## 一.简单了解PID原理

小球为例子

误差=目标值-反馈值

目标就是小球需要保持的状态

反馈为小球的实时状态

 

分为三个环节

### 1、比例环节

proportion  比例
$$
F_p=k_p*Error
$$


这一步就是将小球不断调节到目标位置，但是在此过程中，小球不可能直接准确停止到目标位置上，它会因为自身惯性的原因在目标位置上不断运动，无法停止到目标位置，此时就需要下一个环节来调整小球位置。

 

### 2、微分环节

differential  微分
$$
F_d=k_d*\frac{d(Error)}{dt}
$$


这一步通过不断阻碍小球运动使得小球停止下来

小球向左运动时Fp方向想左，Fd方向向右，阻碍小球向左运动。小球向右运动时，反之。

可以通过调整比例系数kd使得小球停止消耗的时间更少。

原本2步就可以解决小球停止到目标位置的问题，但是当小球受到一些外界的无关力作用时小球的虽然会停止，但是由于有其他力的作用，它不会停止到目标位置上，会有一些偏差，此时就需要进行第三个环

 

### 3、积分环节

integral  积分
$$
F_i=k_i*\int Errordt
$$


当在小球上作用有一个干扰力时小球不会停止到目标位置，假如向小球提供一个向左的恒定干扰力，当小球停止时，小球不会停止在目标位置，此时小球有2个力作用，一个为干扰力，另一个为比例力，此时二力平衡，此时需要改变小球位置到目标位置上，我们向小球添加一个积分力，它与误差的积分成正比，当小球有误差时，积分力会不断增加直到小球停止到目标位置，此时误差为0，积分力才停止不变，0的积分还是0。

 

到此，就完成了一个一纬小球的pid调参

 

## 二.   更专业的理解pid原理

### 1、常见术语

**被控对象**：需要调惨的对象

**目标值**：需要达到的状态量

**反馈值**：对象的实时状态

**输出量**：pid计算后的输出

**误差**：目标-反馈

**稳态误差**：系统稳定状态下的误差

**阶跃输入**：在稳定状态下，目标突然发生变化

**阶跃响应**：在阶跃信号输入后，被控对象的跟随状态，能够反应系统的控制性能

**响应速度**：阶跃输入后被控对象再次到达目标值的速度

**超调量**：阶跃输入后，被控对象到达目标值后超出目标值的距离个峰值与目标值的距离

 

### 2、pid计算过程

1. 为系统指定一个目标值

2. PID将目标值与被控对象当前的反馈量作差得到误差

3. PID将误差值分别经过三个环节计算得到输出分量，三个分量求和得到PID的输出

4. 将PID的输出施加到被控对象上，使反馈量向目标值靠拢

 

### 3、pid的三个步骤作用

1. 比例环节：起主要控制作用，使反馈量向目标值靠拢，但可能导致振荡

2. 积分环节：消除稳态误差，但会增加超调量

3. 微分环节：产生阻尼效果，抑制振荡和超调，但会降低响应速度

 

### 4、pid中物理量的设计

目标值和反馈值通常为同种物理量，就是需要控制的物理量

输出值通常是直接驱动被控对象的控制量

输出量作用在被控对象上需要经过时间积累才会产生反馈量的变化，换言之输出值通常为反馈值对于时间的低阶物理量。比如：目标值和反馈值为位置，则输出值可以为速度或加速度

 

### 5、调参

在完成控制器代码编写后，就要连接好系统进行调参了，我们需要确定最合适的kp，ki，kd使控制效果最优。

通常还是使用经验法调参，通俗而言就是“试参数”，测试多个参数选取最好的控制效果，一般的步骤如下： 

1. 先将所有参数置零

2. 将输出限幅设为执行机构能接受的最大值

3. 增大p参数，使响应速度达到比较好的水平

4. 若存在稳态误差，逐渐增加i参数和积分限幅，使稳态误差消失

5. 若希望减少超调或振荡，逐渐增加d参数，在保证响应速度的前提下尽可能降低超调

###  6、实现

```c
//定义PID结构体用于存放一个PID的数据
typedef struct
{
   	float kp, ki, kd; //三个系数
    float error, lastError; //误差、上次误差
    float integral, maxIntegral; //积分、积分限幅
    float output, maxOutput; //输出、输出限幅
}PID;
 
//用于初始化pid参数的函数
void PID_Init(PID *pid, float p, float i, float d, float maxI, float maxOut)
{
    pid->kp = p;
    pid->ki = i;
    pid->kd = d;
    pid->maxIntegral = maxI;
    pid->maxOutput = maxOut;
}
 
//进行一次pid计算
//参数为(pid结构体,目标值,反馈值)，计算结果放在pid结构体的output成员中
void PID_Calc(PID *pid, float reference, float feedback)
{
 	//更新数据
    pid->lastError = pid->error; //将旧error存起来
    pid->error = reference - feedback; //计算新error
    //计算微分
    float dout = (pid->error - pid->lastError) * pid->kd;
    //计算比例
    float pout = pid->error * pid->kp;
    //计算积分
    pid->integral += pid->error * pid->ki;
    //积分限幅
    if(pid->integral > pid->maxIntegral) pid->integral = pid->maxIntegral;
    else if(pid->integral < -pid->maxIntegral) pid->integral = -pid->maxIntegral;
    //计算输出
    pid->output = pout+dout + pid->integral;
    //输出限幅
    if(pid->output > pid->maxOutput) pid->output =   pid->maxOutput;
    else if(pid->output < -pid->maxOutput) pid->output = -pid->maxOutput;
}
 
```

 

## 三.   串级pid

### 1、使用串级pid的原因

当反馈值与目标值差距过大时，进行输出计算时可能会产生很大的超调，不管怎么修改超调都很难让系统的表现变好，所以就产生了串级pid

 

### 2、原理

串级PID其实就是两个单级PID“串”在一起组成的，

外环和内环就分别是一个单级PID，每个单级PID就如我们之前所说，需要获取一个目标值和一个反馈值，然后产生一个输出值。串级PID中两个环相“串”的方式就是将外环的输出作为内环的目标值。

### 3、实现

```c
 //串级PID的结构体，包含两个单级PID
typedef struct
{
    PID inner; //内环
    PID outer; //外环
    float output; //串级输出，等于inner.output
}CascadePID;
 
//串级PID的计算函数
//参数(PID结构体,外环目标值,外环反馈值,内环反馈值)
void PID_CascadeCalc(CascadePID *pid, float outerRef, float outerFdb, float innerFdb)
{
    PID_Calc(&pid->outer, outerRef, outerFdb); //计算外环
    PID_Calc(&pid->inner, pid->outer.output, innerFdb); //计算内环
    pid->output = pid->inner.output; //内环输出就是串级PID的输出
}
 
```

