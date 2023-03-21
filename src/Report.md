## Part 1. Настройка gitlab-runner  
#### Поднимаем виртуальную машину  
![Part_1_1.jpg](Screenshots/Part_1_1.jpg)  

#### Скачиваем и устанавливаем gitlab-runner  
![Part_1_2.jpg](Screenshots/Part_1_2.jpg)  
Получаем ошибку 0000, которая показана на следующем скриншоте и переустаналиваем gitlab-runner с использованием команд:  
`sudo curl -L --output /usr/local/bin/gitlab-runner "https://gitlab-runner-downloads.s3.amazonaws.com/latest/binaries/gitlab-runner-linux-amd64"`

`sudo chmod +x /usr/local/bin/gitlab-runner`

`sudo useradd --comment 'GitLab Runner' --create-home gitlab-runner --shell /bin/bash`

`sudo gitlab-runner install --user=gitlab-runner --working-directory=/home/gitlab-runner`

`sudo gitlab-runner start`

`sudo systemctl enable --now gitlab-runner`

#### Регистрируем gitlab-runner  
![Part_1_3.jpg](Screenshots/Part_1_3.jpg)  

## Part 2. Сборка  

#### Создаём файл `.gitlab-ci.yml` в корне проекта и вписываем туда следующее:  

![Part_2_1.jpg](Screenshots/Part_2_1.jpg)  

#### Докидываем `s21_cat` и `s21_grep` из проекта `SimpleBash`, пушим и проверяем работу:  

![Part_2_2.jpg](Screenshots/Part_2_2.jpg)  

## Part 3. Тест кодстайла  
#### Не забываем дописать этап в начало `.gitlab-ci.yml`  
![Part_3_1.jpg](Screenshots/Part_3_1.jpg)  

#### Дописываем стадию кодстайла:  
![Part_3_2.jpg](Screenshots/Part_3_2.jpg)  

#### Пушим, проверяем:  

![Part_3_3.jpg](Screenshots/Part_3_3.jpg)  

#### Теперь добавим ошибки по кодстайлу в файлы, запушим и посмотрим результат:  

![Part_3_4.jpg](Screenshots/Part_3_4.jpg)  

#### Посмотрим на пайплайн:  

![Part_3_5.jpg](Screenshots/Part_3_5.jpg)  
Всё как надо.  

## Part 4. Интеграционные тесты  
#### Добавим этап в начало `.gitlab-ci.yml`  
![Part_4_1.jpg](Screenshots/Part_4_1.jpg)  

#### Допишем стадию:  
![Part_4_2.jpg](Screenshots/Part_4_2.jpg)  

#### Содержимое скрипта test.sh:  
![Part_4_3.jpg](Screenshots/Part_4_3.jpg)  

#### Пушим, смотрим результат:  
![Part_4_4.jpg](Screenshots/Part_4_4.jpg)  

#### Пайплайн выполнен успешно:  
![Part_4_5.jpg](Screenshots/Part_4_5.jpg)  

#### Немного поломаем cat, чтобы проверить фейл:  
![Part_4_6.jpg](Screenshots/Part_4_6.jpg)  

#### Пайплайн успешно зафейлен:  
![Part_4_7.jpg](Screenshots/Part_4_7.jpg)  

#### Протестируем скип работы при фейле предыдущих:  
![Part_4_8.jpg](Screenshots/Part_4_8.jpg)  

## Part 5. Этап деплоя  
#### Поднимаем вторую машину:  
![Part_5_1.jpg](Screenshots/Part_5_1.jpg)  

#### Изменяем файл /etc/netplan/00-installer-config.yaml на обеих машинах:  
![Part_5_2.jpg](Screenshots/Part_5_2.jpg)  

#### Изменяем настройки адаптеров:  
![Part_5_3.jpg](Screenshots/Part_5_3.jpg)  

#### Пропингуем вторую машину с первой, чтобы убедиться, что соединение есть:  
![Part_5_4.jpg](Screenshots/Part_5_4.jpg)  

#### Добавим этап в начало `.gitlab-ci.yml`  
![Part_5_5.jpg](Screenshots/Part_5_5.jpg)  

#### Допишем стадию:  
![Part_5_6.jpg](Screenshots/Part_5_6.jpg)  

#### Содержимое скрипта deploy.sh:  
![Part_5_7.jpg](Screenshots/Part_5_7.jpg)  

#### Далее зайдём из-под пользователя gitlab-runner предварительно задав пароль командой `sudo passwd gitlab-runner`  
#### Затем добавим пользователя gitlab-runner в группу sudoers
#### Далее сгенерируем ssh ключ командой `ssh-keygen` и скопируем его на вторую машину командой `ssh-copy-id chaurodr@10.10.0.2`  
#### Теперь мы можем подключаться ко второй машине через ssh без необходимости ввода пароля:  
![Part_5_8.jpg](Screenshots/Part_5_8.jpg)  

#### На второй машине дадим права для папки `/usr/local/bin/` командой `sudo chmod -R 777 /usr/local/bin`  

#### После пуша стадия деплой ждёт ручного запуска:  
![Part_5_9.jpg](Screenshots/Part_5_9.jpg)  

#### Деплой прошёл успешно, файлы появились в нужной папке на второй машине:  
![Part_5_10.jpg](Screenshots/Part_5_10.jpg)  

## Part 6. Дополнительно. Уведомления  

#### Напишем @botfather, чтобы создать бота и получить его токен:
![Part_6_3.jpg](Screenshots/Part_6_3.jpg)  

#### Добавим по вызову скрипта `notify.sh` после каждой стадии:
![Part_6_1.jpg](Screenshots/Part_6_1.jpg)  

#### Содержимое скрипта `notify.sh`:  
![Part_6_2.jpg](Screenshots/Part_6_2.jpg)  

#### Пушим, проверяем работу:  
![Part_6_4.jpg](Screenshots/Part_6_4.jpg)  

