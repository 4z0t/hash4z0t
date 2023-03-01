# H4z0t 

The encoder project named by me is created for studying basics of information theory and encoding.
The executable is a program for compressing, error-detection/correction and encryption of data.

# Структура формата

## Заголовок формата

| Данные: | строка "4z0t" | версия формата | тип данных   | число файлов | Всего байт |
|-------|-----:|---------------:|------------:|------------:|------:|
| Кол-во байт:| 4    |   4            |  4          |  4          | 16    |

Текущая версия формата: 1
Типы данных:

* Files - хранятся файлы 

## Данные формата

После заголовка формата идут данные для закодированных файлов, описанные в данном виде:

### Заголовок файла

| Данные: | исходный размер файла | длина имени файла |  тип сжатия | тип шифрования | тип защиты от помех | доп. данные | Всего байт |
|-------|----------:|---------------:|------------------:|----------------:|----------------:|----------------:|------:|
| Кол-во байт:| 8         |   4            |  4                |4                | 4               | 8               | 32    |

Типы сжатия:

* None - отсутвует (по умолчанию)
* LZ77 - сжатие алгоритмом lz77
* SF - сжатие алгоритмом Шеннона-Фано

Типы защиты:

* None - отсутвует(по умолчанию)
* Hamming - защита от помех алгоритмом Хэмминга

Типы шифрования: *в разработке*

Доп данные используются выше описанными алгоритмами.

* LZ77  использует первый байт для хранения символа-разделителя


### Данные файла

После заголовка файла идет его имя в относительном пути при кодировании (сохранена структура папок) и данные.
В зависимости от типа сжатия/шифрования/защиты записаны данные файла.

* LZ77: данные записываются в один проход, учитывая символ-разделитель
* Шеннон-Фано: записываются нормализованные частоты символов 
    * число частот - 1 (0-255) 
    * [символ ,частота] в количестве числа частот
    * закодированные данные


# Флаги и Аргументы

На данный момент определены 3 флага и 3 аргумента:

* `--decode` и `--encode` это аргументы для указания пути(ей) к файлу(ам) которые будут обработанны кодировщиком
* `--into` это аргумент для указания пути, где расположить закодированные/декодированные данные
* `-lz77` and `-shanon-fano` это флаги для указания типа сжатия
* `-hamming` это флаг для указатия типа защиты от помех