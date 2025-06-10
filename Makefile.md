# Makefile

## 一、概念

​	Makefile 是 make 命令所读取的配置文件，包含了构建项目的规则。其主要作用是检查项目文件的依赖关系，自动执行必要的命令，从而更新目标文件。一般来说，Makefile 主要包括以下三部分内容：

* **目标（Target）**：需要生成的文件，例如可执行文件。

* **依赖（Dependencies）**：生成目标所依赖的文件或目标。

* **命令（Commands）**：构建目标时需要执行的命令。

  

## 二、语法规则

```c
//Target为生成的文件如.i .s .o 文件
//Dependencies为生成的文件所依赖的文件如.c文件
//Commands为生成目标文件所需要执行的命令，如gcc hello -o hello.c命令

Target: Dependencies
	Commands
```

例子：

​	创建一个**hello.c**文件

```c
#include <stdio.h>

int main()
{
    printf("hello");
    return 0;   
}
```

​	创建**makefile**文件

```makefile
hello: hello.c
	gcc hello.c -o hello
```

​	使用make

```终端
make ./hello
```

​	make 会根据 Makefile 中的规则，依次检查目标文件的时间戳和依赖文件的时间戳。如果依赖文件的时间戳比目标文件新，或者目标文件不存在，make就会执行对应的命令来更新目标文件。make 会递归检查依赖关系，直到所有目标都更新完成。



## 三、伪目标

​	创建一个**world.c**文件

```c
#include <stdio.h>

int main()
{
    printf("world");
    return 0;
}
```

​	**makefile**文件

```makefile
.PHONY: clean

hello: hello.o
	gcc main.o -o hello
	
world: world.o
	gcc main.o -o world
	
hello.o: hello.c
	gcc -c main.c
	
world.o: world.c
	gcc -c world.c
	
clean:
	rm -f *.o hello world
```

​	因为，并不生成“clean”这个文件。“伪目标”并不是一个文件，只是一个标签，由于“伪目标”不是文件，所以make无法生成它的依赖关系和决定它是否要执行。只有通过显示地指明这个“目标”才能让其生效。当然，“伪目标”的取名不能和文件名重名，不然其就失去了“伪目标”的意义了。

​	当然，为了避免和文件重名的这种情况，可以使用一个特殊的标记“.PHONY”来显示地指明一个目标是“伪目标”，向make说明，不管是否有这个文件，这个目标就是“伪目标”。

​	伪目标一般没有依赖的文件。但是，我们也可以为伪目标指定所依赖的文件。伪目标同样可以作为“默认目标”，只要将其放在第一个。一个示例就是，如果Makefile需要一口气生成若干个可执行文件，可以使用伪目标的特性：

```makefile
.PHONY: clean all
all: hello world

hello: hello.o
	gcc main.o -o hello
	
world: world.o
	gcc main.o -o world
	
hello.o: hello.c
	gcc -c main.c
	
world.o: world.c
	gcc -c world.c
	
clean:
	rm -f *.o hello world
```

使用make

```
make all
```

## 四、makefile变量

​	定义变量的格式为:

```makefile
var = value
```

​	引用变量：

```makefile
Dependencies = hello.c 
Target = hello
Swap = hello.o

all: $(Target)

$(Target): $(Swap)
	gcc $(Swap) -o $(Target)
	
$(Swap): $(Dependencies)
	gcc -c $(Denpendencies)
	
clean: 
	rm -f *.o $(Target)
```

​	常用的内置变量

- `$@`：表示目标文件。
- `$^`：表示所有的依赖文件。
- `$<`：表示第一个依赖文件。