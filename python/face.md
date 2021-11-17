Открываем PyCharm и нажимаем New project, выбираем папку, где хотим, чтобы был наш проект, и окружение Virtualenv, слева появятся папки
File - Settings - Project Interpretator - шестеренка(add) - new enviroment. После этого Apply. Слева появится папка venv. Теперь все установленные библиотеки будут в виртуальном окружении и не будут засорять память
![image](https://user-images.githubusercontent.com/52165649/142145799-69c1b4a3-9da1-4bba-8d96-b9447d0a62d1.png)
Там же нажимаем на +, вводим opencv-python, install. 
![image](https://user-images.githubusercontent.com/52165649/142145878-44cbfe5b-2b32-4d01-9c6a-6599b6aa4ad6.png)
![image](https://user-images.githubusercontent.com/52165649/142145941-04758e7b-749d-4f9b-b944-13f44d794aff.png)

Две проблемы, которые НАДО решить
1. Не видит файл, если абсолютный путь, только относительный "./people.jpg" "./haarcascade_frontalface_default.xml"
Файл xml берем из opencv C:\opencv\sources\data\haarcascades_cuda
Изображение из интернета, у всех ДОЛЖНЫ быть разные
2. Окно пишет не на русском название
 ![image](https://user-images.githubusercontent.com/52165649/142146452-62dc850b-b15d-4a85-9d60-3cedaefaa85a.png)

В теории написать:
-что такое файл xml
-как мы находим лица
-за счет чего рисуем квадрат

Источники:
https://habr.com/ru/post/260741/
https://tproger.ru/translations/opencv-python-guide/#install
