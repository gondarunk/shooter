## Shooter

Сборка проекта:
```
cmake -S . -B build
cd build 
make
```

Сборка проекта и запуск через терминал ubuntu
```
g++ -std=c++17 main.cpp -o shooter -I$HOME/sfml3/include -L$HOME/sfml3/lib -lsfml-graphics -lsfml-window -lsfml-system

Включить XLaunch. В терминале ubuntu провести команды:
# Проверка DISPLAY
echo $DISPLAY
# Должно быть :0 или IP:0.0 

cd /mnt/c/Users/1/Desktop/B04-402_Strelnikova/project/shooter
export DISPLAY=:0
export LD_LIBRARY_PATH=$HOME/sfml3/lib:$LD_LIBRARY_PATH
./shooter
```

Запуск проекта:
```
./MySFMLProject
```
