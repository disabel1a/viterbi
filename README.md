
# Кодер и декодер Витерби для произвольного сверточного кода

Реализация класса кодера и декодера Витерби для произвольного сверточного кода с построением графиков зависимости вероятности ошибки на бит (BERR) от вероятности ошибки в двоичном симметричном канале.


## Особенности реализации

Для удобства создан класс my_bitset для работы с числами в поле GF(2).
Класс trellis содержит реализацию кодера и декодера.
Матрица порождающих многочленов задается в виде массива массивов (при задании полинома порядок битов меняется, если сравнивать модель с реклизацией на Matlab: полином 1101 (13 в десятичном виде) в Matlab является полиномом 1011 (11 в десятичном виде)).

Для проверки модели используются следующие параметры:

Длинна сообщения - 20 бит;
Количество итераций на деление (шаг вероятности): 1000;

Матрица one_by_two
![Matrix one_by_two](https://github.com/disabel1a/viterbi/blob/main/images/m1by2.jpg)

Матрица one_by_five
![Matrix one_by_five](https://github.com/disabel1a/viterbi/blob/main/images/m1by5.jpg)

Матрица two_by_three
![Matrix two_by_three](https://github.com/disabel1a/viterbi/blob/main/images/m2by3.jpg)

## Графики для вероятности ошибки на бит (BERR) от вероятности ошибки в двоичном симметричном канале

Для графиков ниже вероятность ошибки двоичного симметричного канала установлена от 0.0 до 0.2 с шагом 0.001.

Оси графика:
BERR - вероятность ошибки на бит для сообщения;
Duration - вероятность ошибки двоично симметричного канала

![График для матрицы 1 на 2](https://github.com/disabel1a/viterbi/blob/main/plots/to02/one_by_two.png)

![График для матрицы 1 на 5](https://github.com/disabel1a/viterbi/blob/main/plots/to02/one_by_five.png)

![График для матрицы 2 на 3](https://github.com/disabel1a/viterbi/blob/main/plots/to02/two_by_three.png)

![Объединенный график](https://github.com/disabel1a/viterbi/blob/main/plots/to02/united.png)

Для графиков ниже вероятность ошибки двоичного симметричного канала установлена от 0.0 до 0.1 с шагом 0.01.

![График для матрицы 1 на 2](https://github.com/disabel1a/viterbi/blob/main/plots/to1/one_by_two.png)

![График для матрицы 1 на 5](https://github.com/disabel1a/viterbi/blob/main/plots/to1/one_by_five.png)

![График для матрицы 2 на 3](https://github.com/disabel1a/viterbi/blob/main/plots/to1/two_by_three.png)

![Объединенный график](https://github.com/disabel1a/viterbi/blob/main/plots/to1/united.png)

*Графики расположены в папке /plots: /to02 - от 0.0 до 0.2, /to1 - от 0.0 до 1.0
