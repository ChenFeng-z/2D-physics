# 1. 让 Make 自己去扫描这些目录下的所有 .cpp 文件，存入 SRC 变量
SRC = $(wildcard ./src/*.cpp) $(wildcard ./src/Physics/*.cpp)

# 2. 编译规则
build:
	g++ -std=c++17 -Wall $(SRC) -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_gfx -lm -o app.exe

# 3. 运行规则
run:
	./app.exe

# 4. 清理规则 (Windows 下使用 del)
clean:
	del app.exe