#Алгоритм Эрли
## *Запустите проект на Linux с помощью:*
>$ mkdir build && cd build && cmake .. && make 
> 
###Алгоритм принимает на вход:
* количество нетерминалов, количество терминалов, количество правил
* правила вида: *S(нетерминал)->aB*
* начальный символ
* количество слов, которые нужно проверить
* слова, которые нужно проверить

###Алгоритм выдаёт:
* _**Yes**_, если слово принадлежит грамматике
* _**No**_, иначе
