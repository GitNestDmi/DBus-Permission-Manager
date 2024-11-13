# DBus Permission Manager

## Концепция

В популярных мобильных ОС присутствует динамическое управление разрешениями. Один из способов его реализации - централизованный сервис, у которого стороннее приложение может запросить какое-либо разрешение, а другой сервис может узнать, запрашивало ли приложение, которое к нему обращается, определенный вид разрешения.

## Описание

Данный проект включает в себя три консольных приложения, осуществляющих процесс управления разрешениями и вывод системного времени, проект реализован под `UNIX` (Ubuntu 20.04) с помощью системы межпроцессного взаимодействия `D-Bus(QtDbus)` на сессионной шине, библиотеки `Qt`, написан на языке `C++`.

 Приложения:
- DBus сервис **`Permission_service`** c именем на шине `com.system.permissions`, который имеет 2 публичныхметода:

  - `void RequestPermission(int permissionEnumCode)` - получает путь до исполняемого файла, вызвавшего данный метод, и сохраняет запрос.
  - `bool CheckApplicationHasPermission(String applicationExecPath, int permissionEnumCode)` - проверяет имеется ли у приложения заданное разрешение. 
  
- DBus сервис **`Time_service`** c именем на шине `com.system.time`, который имеет 1 публичный метод:
  - `uint64 GetSystemTime()` - получает путь до исполняемого файла, вызвавшего данный метод, и проверяет разрешение у сервиса `com.system.permissions`, в случае успеха возврашает **timestamp** текущего системного времени, иначе возврашает ошибку **UnauthorizedAccess**.

- Приложение **`Time_service`**, которое:
  - Пробует запросить у сервиса `com.system.time` текущий **timestamp**, в случае ошибки `UnauthorizedAccess` запрашивает разрешение у сервиса `com.system.permissions`, после чего снова запрашивает текущий **timestamp**, далее выводит время в формате "dd.MM.yyyy hh:mm:ss".

## Инструкции по сборке и использованию

### Требования

Для сборки и запуска проекта вам потребуется следующее программное обеспечение:

- git
- qmake (версия >= 3.10)
- gcc/g++ (версия >= 9.40)  

___

### Сборка проекта

Данная инструкция пошаговая.

1. Зайдите в терминал и клонируйте репозиторий.
    ```
    git clone https://github.com/GitNestDmi/DBus-Permission-Manager.git
    ```

2. Перейдите в директорию проекта.
    ```
    cd DBus-Permission-Manager/
    ```

3. Создайте директории для сборки.
    ```
    mkdir build && cd build && mkdir Permission_service && mkdir Time_service && mkdir Time_client

    ```
   
4. Перейдите к каталогам с .pro файлами и с помощью qmake сгенерируйте Makefile'ы в соответствующие директории.
   
   1. Создайте Makefile приложения `Permission_service`.
        ```
        cd .. && cd Permission_service/ && qmake -o ../build/Permission_service/Makefile Permission_service.pro
        ```

   2. Создайте Makefile приложения `Time_service`.
        ```
        cd .. && cd Time_service/ && qmake -o ../build/Time_service/Makefile Time_service.pro
        ```
 
   2. Создайте Makefile приложения `Time_client`.
        ```
        cd .. && cd Time_client/ && qmake -o ../build/Time_client/Makefile Time_client.pro
        ```

5. Перейдите к каталогам сборки с Makefile'ми и с соберите проекты.
   
   1. Перейдите в каталог сборки `Permission_service` и собирите приложение.
        ```
        cd .. && cd build/Permission_service/ && make
        ```  

   2. Перейдите в каталог сборки `Time_service` и собирите приложение.
        ```
        cd .. && cd Time_service/ && make
        ``` 

   3. Перейдите в каталог сборки `Time_client` и собирите приложение.
        ```
        cd .. && cd Time_client/ && make
        ```    
   

6. Теперь у вас есть три исполняемых файла: `Permission_service`, `Time_service` и `Time_client`.

___

### Использование

Для правильной работы приложений необходимо запустить их в определенном порядке, сначала сервисы `Permission_service` и `Time_service`, а после этого приложение `Time_client`.

#### Запуск приложений

Перейдите в директорию `build/`, откройте три терминала в этом каталоге и запустите приложения по очереди, каждый в отдельном терминале.


1. В первом терминале перейдите в каталог к приложению `Permission_service` и запустите его в фоновом режиме.
   
    ```
    cd Permission_service/ && ./Permission_service &
    ``` 

2. В втором терминале перейдите в каталог к приложению `Time_service` и запустите его в фоновом режиме.

    ```
    cd Time_service/ && ./Time_service &
    ``` 

3. Наконец в третьем терминале перейдите в каталог к приложению `Time_client` и запустите его.
    ```
    cd Time_client/ && ./Time_client
    ``` 

#### Пример работы

![work](https://github.com/GitNestDmi/DBus-Permission-Manager/blob/main/other/example.jpg)


___

### Документация

Техническое задание доступно в формате PDF в файле [DBusPermissionManager.pdf](https://github.com/GitNestDmi/DBus-Permission-Manager/blob/main/other/DBusPermissionManager.pdf). Этот документ содержит все требования проекта.

___

### Лицензия

Этот проект распространяется под лицензией MIT License. См. файл LICENSE для получения дополнительной информации.

---
