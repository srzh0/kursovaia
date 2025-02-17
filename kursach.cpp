// kursach.cpp: главный файл проекта.
 
#include "stdafx.h" //подключение библиотек
#include <stdio.h> 
#include <stdlib.h> 
#include <conio.h> 
#include <string.h> 
#include <malloc.h> 
#include <clocale> 
#include <windows.h> 
#include < math.h >

using namespace std; 
using namespace System; 
using namespace System::IO; 
 
#define ENTER 13 //обозначение кнопок
#define ESC 27 
#define UP 72 
#define DOWN 80 
#define HOME 71
#define END 79
 
struct z { 
	char name[30]; //название фильма
	int vremya; //длительность фильма в минутах
	char rezhisser[20]; //имя и фамилия режиссёра
	float reiting; //рейтинг фильма по десятибалльной шкале
	char genre[15]; //жанр фильма
	char data[11]; //дата выхода фильма в прокат в числовом формате
}; //шаблон структуры исхлдных данных

struct sp {
	char imya[30];
	char rezh[20];
	struct sp* sled;
	struct sp* pred;
} *spisok; //шаблон, глобальное описание структуры алфавитного списка

struct obrsp {
	char imya[30];
	char rezh[20];
	struct obrsp* next;
} *obrspisok; //шаблон, глобальное описание структуры списка в обратном алфавитном порядке

int menu(int); //указание шаблонов используемых функций
void maxim(struct z*, int);
void text_data(char*, char*);
void stariy (struct z*, int);
void uzhasy (struct z*, int);
void sovp (struct z*, int);
void vstavka(struct z*, char*, char*); 
void alfalist (struct z*, int);
void diagram (struct z*, int);
 
int main(){ 
	//на локальном уровне осуществляем открытие файла,
	//считываем данные из файла в выделенную память
	int i, n; 
	FILE *file; 
	struct z *films; 
	setlocale(LC_CTYPE,"Russian"); //поддержка русского языка
	int WindowWidth=80; //стандартные размеры окна
	int WindowHeight=25; 
	char BlankLine[]="                                                        "; 
	int NF; //количество позиций 
	Console::CursorVisible::set(false); //отключаем выдимость курсора
	Console::BufferHeight=Console::WindowHeight; //установка размера буфера по высоте
	Console::BufferWidth=Console::WindowWidth; //установка размера буфера по ширине
	Console::ForegroundColor=ConsoleColor::Green; //установка цвета текста
	Console::BackgroundColor=ConsoleColor::Black; //установка цвета фона
	if((file=fopen("movies.txt","r"))==NULL) { //попытка открыть файл
		printf("\nфайл movies.txt не открыт !"); //при неудачном открытии выводим соообщение “Файл не открыт!”
		getch(); exit(1); 
	} 
	fscanf(file,"%d",&NF); 
	films=(struct z*)malloc(NF*sizeof(struct z)); //выделяем память
	for(i=0;i<NF;i++) //считываем данные из списка
		fscanf(file,"%s%d%s%f%s%s",films[i].name, &films[i].vremya, films[i].rezhisser,  &films[i].reiting, films[i].genre, films[i].data); 
	for(i=0;i<NF;i++) //выводим данные из списка на экран
		printf("\n  %-30s %-8d %-20s %-6.1f %-15s %-11s", films[i].name, films[i].vremya, films[i].rezhisser, films[i].reiting, films[i].genre, films[i].data); 
	getch(); 
	
	char dan[7][55]={ 
		"фильм с наибольшим рейтингом                          ", 
		"самый старый фильм                                    ", 
		"количество и список фильмов с жанром ужасы            ",
		"есть ли одинаковый рейтинг у разных режиссёров?       ",
		"алфавитный список фильмов                             ", 
		"диаграмма относительно рейтингов фильмов              ",
		"выход                                                 "
	}; //задание списка вопросов - пунктов меню
	while(1) //в цикле создаем область для вопросов и окрашиваем еѐ в цыета
	//устанавливаем для букв цвет и подсветку для выбранного вопроса
	{ 
		Console::ForegroundColor=ConsoleColor::Red; 
		Console::BackgroundColor=ConsoleColor::Black; 
		Console::Clear(); 
		Console::ForegroundColor=ConsoleColor::Black; 
		Console::BackgroundColor=ConsoleColor::Red; 
		Console::CursorLeft=10; //первая точка, откуда будем закрашивать область меню
		Console::CursorTop=4; 
		printf(BlankLine); 
		for(i=0;i<6;i++) { //размещение вопросов
			Console::CursorLeft=10; 
			Console::CursorTop=i+5; 
			printf(" %s ",dan[i]);
		}
		Console::CursorLeft=10; //последняя точка,где будет заканчиваться выделенная область под меню
		Console::CursorTop=5+7;
		printf(BlankLine);
		n=menu(7);  //выбор вопроса в меню

		switch(n) {
		case 1: maxim(films, NF); break;
		case 2: stariy(films, NF); break;
		case 3: uzhasy(films, NF); break;
		case 4: sovp(films, NF); break;
		case 5: alfalist(films, NF); break;
		case 6: diagram(films, NF); break;
		case 7: exit(0);
		}
	} // конец while(1)
	return 0; 
} // конец main()

int menu(int n) { //n=8
	char dan[7][55]={ 
		"фильм с наибольшим рейтингом                          ", 
		"самый старый фильм                                    ", 
		"количество и список фильмов с жанром ужасы            ",
		"есть ли одинаковый рейтинг у разных режиссёров?       ",
		"алфавитный список фильмов                             ", 
		"диаграмма относительно рейтингов фильмов              ",
		"выход                                                 "
	}; //задание списка вопросов - пунктов меню
	int y1=0,y2=n-1; //устанавливаем выделение для строчки
	char c=1; 
	while (c!=ESC) { //при нажатии кнопки ESC, меню закроется
		switch(c) { 
			case DOWN: y2=y1; y1++; break; //программирование кнопки DOWN которая позволяет перемещаться вниз по меню
			case UP: y2=y1; y1--; break; //программирование кнопки UP, которая позволяет перещаться вверх по меню
			case ENTER: return y1+1; //программирование кнопки ENTER, которая позволяет выбрать вопрос
			case HOME: y2=y1; y1=0; break; //переход в начало списка
			case END: y2=y1; y1=n-1; break; //переход в конец списка

		} 
		if(y1>n-1){ //условие,когда срабатывает кнопка DOWN
			y2=n-1;y1=0;
		} 
		if(y1<0){ //условие,когда срабатывает кнопка UP
			y2=0;y1=n-1;
		} 
		Console::ForegroundColor=ConsoleColor::White; //при выборе вопроса буквы окрашиваются в белый цвет
		Console::BackgroundColor=ConsoleColor::Green; //при выборе вопроса план за буквами подсвечивается зелёным
		Console::CursorLeft=10; 
		Console::CursorTop=y1+5; 
		printf(" %s ",dan[y1]); 
		Console::ForegroundColor=ConsoleColor::Black; 
		Console::BackgroundColor=ConsoleColor::Red; 
		Console::CursorLeft=10; 
		Console::CursorTop=y2+5; 
		printf(" %s ",dan[y2]); 
		c=getch(); 
	} // конец while(c!=ESC)
	exit(0);
}

//фильм с наибольшим рейтингом (функция поиска максимума)
void maxim(struct z* films, int NF) {
	SetConsoleCP(1251); 
	SetConsoleOutputCP(1251); 
	int i; 
	char imya[30];
	float reyt=0; //за минимальный рейтинг в начале берём 0
	for (i=0; i<NF; i++) //поиск максимального рейтинга фильма
		if (films[i].reiting>reyt){ 
			reyt=films[i].reiting;
			strcpy(imya, films[i].name); //сохраняем название фильма с текущим наибольшим рейтингом
		}
	Console::ForegroundColor=ConsoleColor::Green; 
	Console::BackgroundColor=ConsoleColor::Black; 
	Console::Clear(); 
	Console::CursorLeft=10; //задаём область вывода
	Console::CursorTop=10; 
	printf(" Фильм с наибольшим рейтингом это %s, рейтинг которого %1.1f", imya, reyt); //вывод названия и рейтинга фильма с максимальным рейтингом
	getch();
}

//функция перевода даты из числового формата в текстовый
void text_data(char *s, char *sd) {
	char s0[3],month[12][9]={ //месяцы в порядке числового исчисления
	"января","февраля","марта","апреля","мая","июня", "июля","августа","сентября","октября","ноября","декабря"};
	strcpy(s,sd+8);
	strcat(s," ");
	strncpy(s0,sd+5,2); s0[2]=0;
	strcat(s,month[ atoi(s0)-1]);
	strcat(s," ");
	strncat(s,sd,4);
	return; //функция возвращает строку с текстовой датой
}

//функция поиска фильма с самой давней датой выхода
void stariy(struct z* films, int NF){
	int i;
	char s[17];
	char datmin[11]; 
	char imya[30];
	strcpy(datmin, films[0].data); //в переменную структуры записываем нулевой элемент
	for(i=0;i<NF;i++)
		if (strcmp(films[i].data,datmin)<0){ //сравнение текущей даты и даты в переменной datmit
			strcpy(datmin, films[i].data);
			strcpy(imya, films[i].name);
		}
	text_data(s,datmin);
	Console::ForegroundColor=ConsoleColor::Green; 
	Console::BackgroundColor=ConsoleColor::Black; 
	Console::Clear(); 
	Console::CursorLeft=10; 
	Console::CursorTop=10; 
	printf(" Самый старый фильм вышел %s, и это %s", s, imya);
	getch();
}

//функция, выводящая список и количество фильмов с жанром ужасы
void uzhasy (struct z* films, int NF){
	int i, k=0;
	Console::ForegroundColor=ConsoleColor::Green; 
	Console::BackgroundColor=ConsoleColor::Black; 
	Console::Clear(); 
	Console::CursorLeft=10; 
	Console::CursorTop=10; 
	for (i=0; i<NF; i++)
		if (strcmp(films[i].genre, "ужасы")==0) {
			Console::CursorLeft=10; 
			Console::CursorTop=10+k;
			printf(" %s", films[i].name);
			k++;
		}
	Console::CursorLeft=10; 
	Console::CursorTop=12+k;
	printf(" Фильмов с жанром ужасы %d", k);
	getch();
}

// функция поиска одинаковых рейтингов у разных режиссёров
void sovp (struct z* films, int NF) {
	int i=0, j, b=1;
	char rezh[15];
	float reit;
	Console::ForegroundColor=ConsoleColor::Green; 
	Console::BackgroundColor=ConsoleColor::Black; 
	Console::Clear(); 
	Console::CursorLeft=10; 
	Console::CursorTop=10; 
	while (b){
		strcpy(rezh, films[i].rezhisser);
		reit=films[i].reiting;
		for (j=i+1; j<NF; j++) {
			if (films[j].reiting==reit){
				printf("у режиссёров %s и %s есть фильмы с одинаковым рейтингом: %1.1f", rezh, films[j].rezhisser, reit);
				getch();
				b=0;
			}
		}
		i++;
	}
}

//вставка в алфавитный список
void vstavka (struct z* films, char *imya, char *rezh) {
	struct sp *nov,*nt,*z=0;
	for(nt=spisok; nt!=0 && strcmp(nt->imya,imya)<0; z=nt, nt=nt->sled)
		if(nt && strcmp(nt->imya,imya)==0)
			return;
	nov=(struct sp *) malloc(sizeof(struct sp));
	strcpy(nov->imya,imya);
	strcpy(nov->rezh, rezh);
	nov->sled=nt;
	nov->pred=z;
	if (nt) 
		nt->pred=nov;
	if(!z) 
		spisok=nov;
	else 
		z->sled=nov;
	return;
}

// сортировка всех фильмов по алфавиту относительно названия, их режиссёры
void alfalist (struct z* films, int NF) { 
	int i, counter=0;
	struct sp *nt, *nt2, *z;
	Console::ForegroundColor=ConsoleColor::Green; 
	Console::BackgroundColor=ConsoleColor::Black; 
	Console::Clear();
	if(!spisok)
		for(i=0;i<NF;i++) //формирование алфавитного списка
			vstavka(films,films[i].name, films[i].rezhisser);
	Console::Clear();
	printf("\n Алфавитный список фильмов");
	printf("\n ============================================================================================================\n");
	for(nt2=spisok,z=0; nt2!=0;z=nt2, nt2=nt2->sled);
	for(nt=spisok, nt2=z; nt!=0, nt2!=0; nt=nt->sled, nt2=nt2->pred){
		Console::CursorLeft=2; 
		Console::CursorTop=4+counter;
		printf("%-30s %-20s",nt->imya,nt->rezh);
		Console::CursorLeft=60;
		printf("%-30s %-20s",nt2->imya,nt2->rezh);
		counter++;
	}
	getch();
}

// диаграмма, показывающая соотношение рейтингов между фильмами
void diagram (struct z* films, int NF) { //создаем диаграмму
	int i, j, reit, NColor; //i - для номера строки
	System::ConsoleColor Color; //задаѐт константы, которые определяют основной цвет и цвет фона консоли
	Console::ForegroundColor=ConsoleColor::Green; //цвет символов
	Console::BackgroundColor=ConsoleColor::Black; //цвет фона
	Console::Clear();
	Color=ConsoleColor::Black; NColor=1;
	for (i=0; i<NF; i++) {
		reit=int(5*films[i].reiting); //оригинальный рейтинг умножен на 5 для более приятного вида диаграмы
		Console::ForegroundColor=ConsoleColor::Green;
		Console::BackgroundColor= ConsoleColor::Black;
		Console::CursorLeft=5;
		Console::CursorTop=i+1;
		printf("\n %-30s %5.1f", films[i].name, films[i].reiting);
		Console::BackgroundColor=++Color; 
		NColor++;
		Console::CursorLeft=45;
		for (j=0; j<reit; j++)
			printf(" ");
		if(NColor==0){ //выделяем на диаграмму 14 цветов, исключая цвет фона
			Color=ConsoleColor::Black; NColor=1; 
		}
	}
	getch();
}
