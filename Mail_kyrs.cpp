// Kyrsovaya.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <stdlib.h>
#include <Shlwapi.h>
#include <fstream> 
#include <sstream> 
#include <iomanip>
#include <time.h>
#include <stdio.h>

using namespace std;

class Fields
{
public:
	short  s_nzap;			// Номер записи
	short  s_nadr;			// Номер записи для группы
	short  s_nomp;			// Номер п/п в выборках
	short  s_notp;			// Отправлено ли письмо
	char   s_from[50];
	char   s_to[50];
	char   s_FIO_from[25]; // ФИО отправителя
	char   s_FIO_to[25]; // ФИО получателя
	char   s_group[10];	// Группа
	char   s_adress_or_mail[2];	// 5 - адресная книга, 0 - письмо
	char   s_actuality[2];	// 0 - актуален, 2 - скрыт, 4 - архив
	char   s_mode[2];	// входящие - 1, исходящие - 2
	char   s_date[11];	// дата
	char   s_time[6];	// время
	char   s_theme[50];	// тема
	char   s_text[254];// текст
};

class Mail
{
public:
	Mail(){}
	Mail(string T_nzap, string T_nadr, string T_nomp, string T_notp, string T_from, string T_to, string T_FIO_from,
		string T_FIO_to, string T_group, string T_adress_or_mail, string T_actuality, string T_mode,
		string T_date, string T_time, string T_theme, string T_text)
	{
		nzap = T_nzap;
		nadr = T_nadr;
		nomp = T_nomp;
		notp = T_notp;
		from = T_from;
		to = T_to;
		FIO_from = T_FIO_from;
		FIO_to = T_FIO_to;
		group = T_group;
		adress_or_mail = T_adress_or_mail;
		actuality = T_actuality;
		mode = T_mode;
		date = T_date;
		time = T_time;
		theme = T_theme;
		text = T_text;
	}
	void f_inp(string hd_name, string &inp_str, int s_len)
	{
		bool bp = true;
		while (bp)
		{
			cout << hd_name;
			getline(cin, inp_str, '\n');
			inp_str = TrimS(inp_str, "r");
			int len = inp_str.length();
			if (len > s_len)
			{
				cout << " *Ошибка. Максимальная длинна записи " << s_len << " символов" << endl;
				cout << "  Вводите данные снова или нажмите клавишу Вверх" << endl;
				cout << "  Для вставки сохраненных данных с ошибкой и последующего их исправления\n" << endl;
			}
			else bp = false;
		}
	}

	void f_inn(string hd_name, string &inp_str, int len_value, int min_value, int max_value)
	{
		bool bp = true;
		while (bp)
		{
			cout << hd_name;
			getline(cin, inp_str, '\n');
			inp_str = TrimS(inp_str, "r");
			int len = inp_str.length();
			if (len > len_value)
			{
				cout << " *Ошибка. Допустимая длина данных " << len_value << " симв." << endl;
				cout << "  Вводите данные снова или нажмите клавишу Стрелка Вверх" << endl;
				cout << "  для вставки сохраненных данных с ошибкой и последующего их исправления\n" << endl;
			}
			else bp = false;

			if (!bp)
			{
				bp = true;
				int nflag = 0;
				string st;
				for (int i = 0; i < len; i++)
				{
					st = inp_str.substr(i, 1);
					if ((st < "0") || (st > "9"))
					{
						nflag++;
					}
				}
				if (nflag > 0)
				{
					cout << " *Ошибка. Данные содержат нецифровые символы" << endl;
					cout << "  Вводите данные снова или нажмите клавишу Стрелка Вверх " << endl;
					cout << "  для вставки сохраненных данных с ошибкой и последующего их исправления \n" << endl;
				}
				else bp = false;

				if (!bp)
				{
					bp = true;
					int val = StrToIntDT(inp_str);
					if ((val < min_value) || (val > max_value))
					{
						cout << " *Ошибка. Разрешенный диапазон значения данных: " << min_value << " ... " << max_value << "" << endl;
						cout << "  Вводите данные снова или нажмите клавишу Стрелка Вверх " << endl;
						cout << "  для вставки сохраненных данных с ошибкой и последующего их исправления \n" << endl;
					}
					else bp = false;
				}
			}
		}
	}


	void f_ind(string hd_name, string &inp_str)
	{
		bool bp = true;
		while (bp)
		{
			cout << hd_name;
			getline(cin, inp_str, '\n');
			inp_str = TrimS(inp_str, "r");
			int len = inp_str.length();
			if (len > 10)
			{
				cout << " *Ошибка. Допустимая длина данных 10 симв. ( ДД.ММ.ГГГГ ) " << endl;
				cout << "  Вводите данные снова или нажмите клавишу Стрелка Вверх" << endl;
				cout << "  для вставки сохраненных данных с ошибкой и последующего их исправления\n" << endl;
			}
			else bp = false;

			if (!bp)
			{
				bp = true;
				int pos1 = inp_str.find(".");
				int pos2 = inp_str.find(".", pos1 + 1);
				if ((pos1 != 2) || (pos2 != 5) || (len<10))
				{
					cout << " *Ошибка. Формат даты ДД.ММ.ГГГГ" << endl;
					cout << "  Вводите данные снова или нажмите клавишу Стрелка Вверх " << endl;
					cout << "  для вставки сохраненных данных с ошибкой и последующего их исправления \n" << endl;
				}
				else bp = false;

				if (!bp)
				{
					bp = true;
					int d1 = StrToIntDT(inp_str.substr(0, 2));
					int m1 = StrToIntDT(inp_str.substr(3, 2));
					int y1 = StrToIntDT(inp_str.substr(6, 4));
					int n_chk = TstDate(d1, m1, y1);

					if (n_chk != 0)
					{
						cout << " *Ошибка. Введенная дата отсутствует в календаре" << endl;
						cout << "  Вводите данные снова или нажмите клавишу Стрелка Вверх " << endl;
						cout << "  для вставки сохраненных данных с ошибкой и последующего их исправления \n" << endl;
					}
					else bp = false;
				}
			}
		}
	}

	void f_int(string hd_name, string &inp_str)
	{
		bool bp = true;
		while (bp)
		{
			cout << hd_name;
			getline(cin, inp_str, '\n');
			inp_str = TrimS(inp_str, "r");
			int len = inp_str.length();
			if (len > 5)
			{
				cout << " *Ошибка. Допустимая длина данных 5 симв. ( ЧЧ:ММ ) " << endl;
				cout << "  Вводите данные снова или нажмите клавишу Стрелка Вверх" << endl;
				cout << "  для вставки сохраненных данных с ошибкой и последующего их исправления\n" << endl;
			}
			else bp = false;

			if (!bp)
			{
				bp = true;
				int pos = inp_str.find(":");
				if ((pos != 2) || (len<5))
				{
					cout << " *Ошибка. Формат времени ЧЧ:ММ" << endl;
					cout << "  Вводите данные снова или нажмите клавишу Стрелка Вверх " << endl;
					cout << "  для вставки сохраненных данных с ошибкой и последующего их исправления \n" << endl;
				}
				else bp = false;

				if (!bp)
				{
					bp = true;
					int tm1, tm2;
					tm1 = StrToIntDT(inp_str.substr(0, 2));
					tm2 = StrToIntDT(inp_str.substr(3, 2));
					if (((tm1 < 0) || (tm1 > 23)) || ((tm2 < 0) || (tm2 > 59)))
					{
						cout << " *Ошибка. Формат времени ЧЧ:ММ, диапазон: часы 00-23 минуты 00-59" << endl;
						cout << "  Вводите данные снова или нажмите клавишу Стрелка Вверх " << endl;
						cout << "  для вставки сохраненных данных с ошибкой и последующего их исправления \n" << endl;
					}
					else bp = false;
				}
			}
		}
	}

	void f_ina(string hd_name, string &inp_str, int s_len)
	{ 	
		int len;
		string a_str[10];
		int nflag2, nflag4, ret, n_adr;
		bool bp2;
		bool bp = true;
		while (bp)
		{
			// проверка на превышение длины поля
			cout << hd_name;
			getline(cin, inp_str, '\n');
			inp_str = TrimS(inp_str, "r");
			len = inp_str.length();

			if (len > s_len)
			{
				cout << " *Ошибка. Допустимая длина данных " << s_len << " симв." << endl;
				cout << "  Вводите данные снова или нажмите клавишу Стрелка Вверх" << endl;
				cout << "  для вставки сохраненных данных с ошибкой и последующего их исправления\n" << endl;
			}
			else bp = false; 

			
			if (!bp)
			{
				bp = true;
				n_adr = 0;
			
				bp2 = true;
				while (bp2)
				{
					nflag2 = 0;
					nflag4 = 0;
					ret = 0;
					string st = inp_str + ";";
					int i = 0;
					
					if (!inp_str.empty())
					{
						int pos;
						bool bpp = true;
						while (bpp)
						{
							pos = st.find(";");
							if (pos != std::string::npos)
							{
								a_str[i] = st.substr(0, pos);
								st.erase(0, pos + 1);
								if (!a_str[i].empty()) i++;
							}
							else
							{
								bpp = false;
							}
							n_adr = i;

							// просматриваем массив с адресами
							for (int i = 0; i < n_adr; i++)
							{
								a_str[i] = TrimS(a_str[i], "all");
							}

							// проверка синтаксиса адреса
							for (int i = 0; i < n_adr; i++)
							{
								ret = check_adr(a_str[i]);
								if (ret == 0) nflag4 = 1;
								if (nflag4 == 1)
								{
									cout << " *Ошибка. Неверный формат электронного адреса " << endl;
									cout << "  Вводите данные снова или нажмите клавишу Стрелка Вверх " << endl;
									cout << "  для вставки сохраненных данных с ошибкой и последующего их исправления \n" << endl;
									bp2 = false;
									bp = true;
									bpp = false;
									break;
								}
							}
						}
					}
					else bp = false;

					if (nflag4 == 0)
					{
						bp2 = false;
						bp = false;
					}

				}
			}
		}
	}

	void press_any_key()
	{ 
		cout << "Для подолжения нажмите любую клавишу ...";
		return;
	}

	char * S_cvt(const string a){
		
		int len = a.size() + 1;
		char *ch = new char[len];
		
		return strcpy(ch, a.c_str());
	}
	int Str_Find(string s1, string s2)
	{
		int pos = s1.find(s2);
		if (pos != string::npos) return 1; 
		else return 0;
	}
	void Input(vector <Mail> &vSource, short &kolvo, short &n_nadr)
	{
		string	Temp_nzap;
		string	Temp_nadr;
		string	Temp_nomp;
		string	Temp_notp;
		string	Temp_from;
		string	Temp_to;
		string	Temp_FIO_from;
		string	Temp_FIO_to;
		string	Temp_group;
		string	Temp_adress_or_mail;
		string	Temp_actuality;
		string	Temp_mode;
		string	Temp_date;
		string	Temp_time;
		string	Temp_theme;
		string	Temp_text;

		

		
		Temp_nzap = IntToStr(kolvo);

		cout << " Добавить новое письмо:\n" << endl;
		cout << endl;

		f_ina(" От кого         : ", Temp_from, 50);
		f_ina(" Кому            : ", Temp_to, 50);
		f_inp(" ФИО отправителя : ", Temp_FIO_from, 25);
		f_inp(" ФИО получателя  : ", Temp_FIO_to, 25);
		f_inn(" Отправлено (1,0): ", Temp_notp, 1, 0, 1);
		f_inn(" Признак АК (5,0): ", Temp_adress_or_mail, 1, 0, 5);
		f_inn(" Мультиадрес     : ", Temp_nadr, 4, 0, 9999);
		f_inn(" В Архив    (1,0): ", Temp_actuality, 1, 0, 1);
		f_inn(" Вх=1, Исх=2     : ", Temp_mode, 1, 1, 2);
		f_ind(" Дата ДД.ММ.ГГГГ : ", Temp_date);
		f_int(" Время     ЧЧ:ММ : ", Temp_time);
		f_inp(" Тема письма     : ", Temp_theme, 50);
		f_inp(" Текст письма    : ", Temp_text, 254);

		char key = '0';
		cout << endl;
		cout << " Выберите группу :  " << endl;
		cout << " 1.Work     " << endl;
		cout << " 2.Family   " << endl;
		cout << " 3.Friends  " << endl;
		cout << " 4.Familiar " << endl;
		cout << " 5.Other      ";
		cout << endl;
		cout << "                 : ";
		cin >> key;

		if (key == '1') Temp_group = "Work";
		else if (key == '2') Temp_group = "Family";
		else if (key == '3') Temp_group = "Friends";
		else if (key == '4') Temp_group = "Familiar";
		else if (key == '5') Temp_group = "Other";

		// Дополнительные адреса
		string inp_str = Temp_to; // строка уже проверена ранее при вводе
		string a_str[10];
		inp_str = TrimS(inp_str, "r");
		int len = inp_str.length();
		int n_adr = 0; // количество адресов
		

		// извлекаем в массив адреса из строки
		string st = inp_str + ";";
		int i = 0;
		if (!inp_str.empty())
		{
			int pos;
			bool bpp = true;
			while (bpp)
			{
				pos = st.find(";");
				if (pos != std::string::npos)
				{
					a_str[i] = st.substr(0, pos);
					st.erase(0, pos + 1);
					if (!a_str[i].empty()) i++;
				}
				else
				{
					bpp = false;
				}
			}
			n_adr = i;

			// просматриваем массив с адресами
			for (int i = 0; i < n_adr; i++) a_str[i] = TrimS(a_str[i], "all");
		}

		
		if (n_adr > 1)
		{
			n_nadr = n_nadr + 1;
			Temp_nadr = IntToStr(n_nadr);
			Temp_mode = "3"; // первая запись
		}
		else Temp_nadr = "0"; // одиночный адрес

		// Запись первого письма
		Temp_nzap = IntToStr(kolvo);

		vSource.push_back(Mail(Temp_nzap, Temp_nadr, Temp_nomp, Temp_notp, Temp_from, Temp_to, Temp_FIO_from,
			Temp_FIO_to, Temp_group, Temp_adress_or_mail, Temp_actuality, Temp_mode, Temp_date, Temp_time, Temp_theme, Temp_text));

		// Запись второго адресов
		if (n_adr > 1)
		{
			for (int i = 0; i < n_adr; i++)
			{
				kolvo = kolvo + 1;
				Temp_nzap = IntToStr(kolvo);
				Temp_to = a_str[i]; // массив с адресами
				Temp_mode = "4";  // вторая и последующие запись адреса

				vSource.push_back(Mail(Temp_nzap, Temp_nadr, Temp_nomp, Temp_notp, Temp_from, Temp_to, Temp_FIO_from,
					Temp_FIO_to, Temp_group, Temp_adress_or_mail, Temp_actuality, Temp_mode, Temp_date, Temp_time, Temp_theme, Temp_text));
			}
		}
	}

	void Input4(vector <Mail> &vSource, int mode, short &kolvo, short &n_nadr)
	{
		string	Temp_nzap;
		string	Temp_nadr;
		string	Temp_nomp;
		string	Temp_notp;
		string	Temp_from;
		string	Temp_to;
		string	Temp_FIO_from;
		string	Temp_FIO_to;
		string	Temp_group;
		string	Temp_adress_or_mail;
		string	Temp_actuality;
		string	Temp_mode;
		string	Temp_date;
		string	Temp_time;
		string	Temp_theme;
		string	Temp_text;

		Temp_nzap = IntToStr(kolvo);

		Temp_nadr = '0';
		Temp_nomp = '0';
		Temp_notp = '0';				// не отправленное
		Temp_from = "wee3dood@gmail.com";  
		Temp_FIO_from = "Kuzmin K.S.";			
		Temp_adress_or_mail = '0';
		Temp_actuality = '0';
		Temp_mode = '2'; // исходящее

		cout << " Написать письмо:" << endl;
		cout << endl;
		f_ina(" Кому            : ", Temp_to, 50);
		f_inp(" ФИО получателя  : ", Temp_FIO_to, 25);
		f_inp(" Тема письма     : ", Temp_theme, 50);
		f_inp(" Текст письма    : ", Temp_text, 254);

		char key = '0';
		cout << endl;
		cout << " Выберите группу :" << endl;
		cout << " 1.Work     " << endl;
		cout << " 2.Family   " << endl;
		cout << " 3.Friends  " << endl;
		cout << " 4.Familiar " << endl;
		cout << " 5.Other      ";
		cout << endl;
		cout << "                 : ";
		cin >> key;

		if (key == '1') Temp_group = " Work";
		else if (key == '2') Temp_group = " Family";
		else if (key == '3') Temp_group = " Friends";
		else if (key == '4') Temp_group = " Familiar";
		else if (key == '5') Temp_group = " Other";

		int k;
		cout << "\nВведите 1 - если надо отправить письмо" << endl;
		cout << "Введите 2 - если письмо надо сохранить, но отправлять не надо" << endl;
		cout << "Введите 3 - отмена, письмо отправить в корзину у стола" << endl;
		cin >> k;

		// Дата и Время

		SYSTEMTIME info;
		GetLocalTime(&info);
		int day = info.wDay;
		int month = info.wMonth;
		int year = info.wYear;
		int hour = info.wHour;
		int minute = info.wMinute;
		string date_str, time_str;
		date_str = IntToStrDT(day) + "." + IntToStrDT(month) + "." + IntToStrDT(year);
		time_str = IntToStrDT(hour) + ":" + IntToStrDT(minute);
		Temp_date = date_str;
		Temp_time = time_str;

		// Отправить
		if (k == 1)
		{
			Temp_notp = '1';
			cout << "Письмо отправлено...";
		}

		// Только сохранить
		if (k == 2)
		{
			cout << "Письмо сохранено, но не отправлено...";
		}

		// Не записывать
		if (k == 3)
		{
			kolvo = kolvo - 1;
			cout << "Письмо не сохранено...";
		}
		if (k != 3) k = 2;

		// Мультиадреса
		string inp_str = Temp_to;
		string a_str[10];
		
		inp_str = TrimS(inp_str, "r");
		int len = inp_str.length();
		int n_adr = 0; // количество мультиадресов

		// извлекаем в массив адреса из строки
		string st = inp_str + ";";
		int i = 0;
		if (!inp_str.empty())
		{
			int pos;
			bool bpp = true;
			while (bpp)
			{
				pos = st.find(";");
				if (pos != std::string::npos)
				{
					a_str[i] = st.substr(0, pos);
					st.erase(0, pos + 1);
					if (!a_str[i].empty()) i++;
				}
				else
				{
					bpp = false;
				}
			}
			n_adr = i;

			// просматриваем массив с адресами
			for (int i = 0; i < n_adr; i++) a_str[i] = TrimS(a_str[i], "all");
		}

		if (n_adr > 1)
		{
			n_nadr = n_nadr + 1;
			Temp_nadr = IntToStr(n_nadr);
			Temp_mode = "3"; // первая запись мультиадреса
		}
		else Temp_nadr = "0"; // одиночный адрес

		Temp_nzap = IntToStr(kolvo);

		// Запись первого письма
		vSource.push_back(Mail(Temp_nzap, Temp_nadr, Temp_nomp, Temp_notp, Temp_from, Temp_to, Temp_FIO_from,
			Temp_FIO_to, Temp_group, Temp_adress_or_mail, Temp_actuality, Temp_mode, Temp_date, Temp_time, Temp_theme, Temp_text));

		// Запись второго и письма
		if (n_adr > 1)
		{
			for (int i = 0; i < n_adr; i++)
			{
				kolvo = kolvo + 1;
				Temp_nzap = IntToStr(kolvo);
				Temp_to = a_str[i]; // массив с адресами
				Temp_mode = "4";  // вторая и последующие запись адреса

				vSource.push_back(Mail(Temp_nzap, Temp_nadr, Temp_nomp, Temp_notp, Temp_from, Temp_to, Temp_FIO_from,
					Temp_FIO_to, Temp_group, Temp_adress_or_mail, Temp_actuality, Temp_mode, Temp_date, Temp_time, Temp_theme, Temp_text));
			}
		}
	}

	// написать письмо
	void Input2(vector <Mail> &vSource, string nzap, string nadr, string nomp, string notp, string from,
		string to, string FIO_from, string FIO_to, string group, string adress_or_mail,
		string actuality, string mode, string date, string time, string theme, string text)
	{
		string	Temp_nzap;
		string	Temp_nadr;
		string	Temp_nomp;
		string	Temp_notp;
		string	Temp_from;
		string	Temp_to;
		string	Temp_FIO_from;
		string	Temp_FIO_to;
		string	Temp_group;
		string	Temp_adress_or_mail;
		string	Temp_actuality;
		string	Temp_mode;
		string	Temp_date;
		string	Temp_time;
		string	Temp_theme;
		string	Temp_text;

		Temp_nzap = nzap;
		Temp_nadr = nadr;
		Temp_nomp = nomp;
		Temp_notp = notp;
		Temp_from = from;
		Temp_to = to;
		Temp_FIO_from = FIO_from;
		Temp_FIO_to = FIO_to;
		Temp_group = group;
		Temp_adress_or_mail = adress_or_mail;
		Temp_actuality = actuality;
		Temp_mode = mode;
		Temp_date = date;
		Temp_time = time;
		Temp_theme = theme;
		Temp_text = text;

		vSource.push_back(Mail(Temp_nzap, Temp_nadr, Temp_nomp, Temp_notp, Temp_from, Temp_to, Temp_FIO_from,
			Temp_FIO_to, Temp_group, Temp_adress_or_mail, Temp_actuality, Temp_mode, Temp_date, Temp_time, Temp_theme, Temp_text));
	}

	void PrintMail(vector <Mail> &vSource, string mode, int &nfnd)
	{ // просмотр входящих и исходящих писем
		if (vSource.empty())
		{
			cout << "Почтовый ящик пуст";
			return;
		}

		vector <Mail>::iterator iterSource = vSource.begin();
		short tmp_np = 0;
		while (iterSource != vSource.end())
		{
			if ((iterSource->Getadress_or_mail() != "5") && (iterSource->GetMode() != "4") && (iterSource->Getactuality() != "2"))
			{
				if (mode == "3")
				{
					tmp_np++;
					nfnd = 1;
					if (tmp_np == 1) { cout << "  Все письма" << endl;	cout << "____________________________________________" << endl; }
					cout.setf(ios::left);
					cout << " " << tmp_np << ").Письмо №: " << setw(4) << iterSource->GetNzap()
						<< "\n      От кого: " << setw(15) << iterSource->Getfrom() << " ФИО отп: " << setw(20) << iterSource->GetFIO_from()
						<< "\n      Кому   : " << setw(15) << iterSource->Getto()
						<< "\n      Группа : " << iterSource->Getgroup() << '\n';
					cout.unsetf(ios::left);
				}
				if (mode == "1")
				{
					if (iterSource->GetMode() == "1")
					{
						tmp_np++;
						nfnd = 1;
						if (tmp_np == 1)
						{
							if (mode == "1") { cout << "  Входящие письма" << endl;	cout << "____________________________________________" << endl; }
						}
						cout.setf(ios::left);
						cout << " " << tmp_np << ").Письмо №: " << setw(4) << iterSource->GetNzap()
							<< "\n      От кого: " << setw(15) << iterSource->Getfrom() << " ФИО отп: " << setw(20) << iterSource->GetFIO_from()
							<< "\n      Кому   : " << setw(15) << iterSource->Getto() << iterSource->GetFIO_to()
							<< "\n      Группа : " << iterSource->Getgroup() << '\n';
						cout.unsetf(ios::left);
					}
				}
				if (mode == "2")
				{
					if ((iterSource->GetMode() == "2") || (iterSource->GetMode() == "3"))
					{
						tmp_np++;
						nfnd = 1;
						if (tmp_np == 1)
						{
							if (mode == "2") { cout << "  Исходящие письма" << endl;	cout << "____________________________________________" << endl; }
						}
						cout.setf(ios::left);
						cout << " " << tmp_np << ").Письмо №: " << setw(4) << iterSource->GetNzap()
							<< "\n      От кого: " << setw(15) << iterSource->Getfrom() << " ФИО отп: " << setw(20) << iterSource->GetFIO_from()
							<< "\n      Кому   : " << setw(15) << iterSource->Getto() << " ФИО пол: " << setw(20) << iterSource->GetFIO_to()
							<< "\n      Группа : " << iterSource->Getgroup() << '\n';
						cout.unsetf(ios::left);
					}
				}

				if (mode == "4")
				{
					if (iterSource->GetNotp() == "1")
					{
						tmp_np++;
						nfnd = 1;
						if (tmp_np == 1) cout << "  Отправленные" << endl;	cout << "____________________________________________" << endl;
						cout.setf(ios::left);
						cout << " " << tmp_np << ").Письмо №: " << setw(4) << iterSource->GetNzap()
							<< "\n      От кого: " << setw(15) << iterSource->Getfrom() << " ФИО отп: " << setw(20) << iterSource->GetFIO_from()
							<< "\n      Кому   : " << setw(15) << iterSource->Getto() << " ФИО пол: " << setw(20) << iterSource->GetFIO_to()
							<< "\n      Группа : " << iterSource->Getgroup() << '\n';
						cout.unsetf(ios::left);
					}
				}
			}
			iterSource++;
		}
		cout << "\n____________________________________________";

		if (mode == "1") { cout << "\nКоличество входяших писем в почтовом ящике: " << tmp_np << "\n" << endl; }
		if (mode == "2") { cout << "\nКоличество исходящих писем в почтовом ящике: " << tmp_np << "\n" << endl; }
		if (mode == "3") { cout << "\nКоличество писем в почтовом ящике: " << tmp_np << "\n" << endl; }
		if (mode == "4") { cout << "\nКоличество отправленных писем: " << tmp_np << "\n" << endl; }

	}   // просмотр всех писем
	// открыть все письма , входящие , исходящие, отпраленные
	void Print_Addres_book(vector <Mail> &vSource, short &kolvo)
	{
		vector <Mail>::iterator iterSource = vSource.begin();
		int tmp_np = 0;
		while (iterSource != vSource.end())
		{
			if ((iterSource->Getadress_or_mail() == "5") && (iterSource->Getactuality() != "2"))

			{
				tmp_np++;
				if (tmp_np == 1)
				{
					cout << "                 Список контактов";
					cout << "\n_________________________________________________" << endl;
				}

				cout.setf(ios::left);
				cout << "\n " << tmp_np << "). " << setw(15) << iterSource->GetFIO_from() << setw(25) << iterSource->Getfrom()
					<< "Контакт №: " << setw(5) << iterSource->GetNzap();
				cout.unsetf(ios::left);
			}
			iterSource++;
		}
		if (tmp_np > 0)
		{
			cout << "\n_________________________________________________";
			cout << "\nКоличество контактов в Списке контактов: " << tmp_np;

			cout << "" << endl;
			int k;
			cout << "\nВведите 1 - если надо удалить контакт" << endl;
			cout << "Введите 2 - чтобы выйти в главное меню" << endl;
			cin >> k;
			if (k == 1)
			{
				string n_del = "";
				cout << "Следует удалить Контакт №: ";
				cin >> n_del;

				vector <Mail>::iterator iterSource = vSource.begin();
				short tmp_np = 0;
				int tmp_flg = 0;
				string tmp_nzap;
				// Находим
				int n_fnd = 0;
				while (iterSource != vSource.end())
				{
					string tmp_n = iterSource->GetNzap();
					if (tmp_n == n_del)
					{
						n_fnd = 1;
						break;
					}
					iterSource++;
				}

				if (n_fnd == 1)
				{
					string	Temp_nzap;
					string	Temp_nadr;
					string	Temp_nomp;
					string	Temp_notp;
					string	Temp_from;
					string	Temp_to;
					string	Temp_FIO_from;
					string	Temp_FIO_to;
					string	Temp_group;
					string	Temp_adress_or_mail;
					string	Temp_actuality;
					string	Temp_mode;
					string	Temp_date;
					string	Temp_time;
					string	Temp_theme;
					string	Temp_text;

					Temp_nzap = iterSource->GetNzap();
					Temp_nadr = iterSource->GetNadr();
					Temp_nomp = iterSource->GetNomp();
					Temp_notp = iterSource->GetNotp();
					Temp_from = iterSource->Getfrom();
					Temp_to = iterSource->Getto();
					Temp_FIO_from = iterSource->GetFIO_from();
					Temp_FIO_to = iterSource->GetFIO_to();
					Temp_group = iterSource->Getgroup();
					Temp_adress_or_mail = iterSource->Getadress_or_mail();
					Temp_actuality = iterSource->Getactuality();
					Temp_mode = iterSource->GetMode();
					Temp_date = iterSource->GetDate();
					Temp_time = iterSource->GetTime();
					Temp_theme = iterSource->Gettheme();
					Temp_text = iterSource->GetText();

					// чистим буфер клавиатуры
					cin.clear();
					cin.sync();

					// Редактируем поле Temp_actuality
					Temp_actuality = "2";

					char chv;
					cout << " Контакт будет удален. Подтвердите удаление y/n :";
					cin >> chv;
					if (chv == 'y')
					{
						// Удаляем старое письмо 
						vSource.erase(iterSource);

						// Добавляем обновленное
						vSource.push_back(Mail(Temp_nzap, Temp_nadr, Temp_nomp, Temp_notp, Temp_from, Temp_to, Temp_FIO_from,
							Temp_FIO_to, Temp_group, Temp_adress_or_mail, Temp_actuality, Temp_mode, Temp_date, Temp_time, Temp_theme, Temp_text));
						cout << "\nКонтакт с номером №: " << n_del << " удален из Списка Контактов." << endl;
						press_any_key();
					}
					else
					{
						cout << " Удаление отменено" << endl;
						press_any_key();
					}
				}

				else cout << "Контакт с таким номером не найден в Списке Контактов, контакт №: " << n_del;
			}
		}
		else cout << "\nСписок контактов пуст";
	}
	//открыть адресную книгу
	void PrintBook(vector <Mail> &vSource, string n_add, short &kolvo)
	{
		vector <Mail>::iterator iterSource = vSource.begin();

		short tmp_np = 0;
		int tmp_flg = 0;
		string tmp_from;

		// Находим отправителя в письмах
		while (iterSource != vSource.end())
		{
			if ((iterSource->Getadress_or_mail() != "5") && (iterSource->Getactuality() != "2"))
			{
				string tmp_n = iterSource->GetNzap();
				if (tmp_n == n_add)
				{
					tmp_from = iterSource->Getfrom();
					break;
				}
			}
			iterSource++;
		}

		// Ищем такое отправителя в Адресной книге
		iterSource = vSource.begin();
		while (iterSource != vSource.end())
		{
			if ((iterSource->Getadress_or_mail() == "5") && (iterSource->Getactuality() != "2"))
			{
				if (tmp_from == iterSource->Getfrom())
				{
					tmp_from = iterSource->Getfrom();
					tmp_flg = 1;
					break;
				}
			}
			iterSource++;
		}

		// Если надо - добавляем в Адресную книгу
		if (tmp_flg == 0)
		{
			iterSource = vSource.begin();
			while (iterSource != vSource.end())
			{
				string tmp_n = iterSource->GetNzap();
				if (tmp_n == n_add)
				{
					cout << " Адрес " << iterSource->Getfrom() << " добавлен в Адресную книгу...";
					break;
				}
				iterSource++;
			}

			string	Temp_nzap;
			string	Temp_nadr;
			string	Temp_nomp;
			string	Temp_notp;
			string	Temp_from;
			string	Temp_to;
			string	Temp_FIO_from;
			string	Temp_FIO_to;
			string	Temp_group;
			string	Temp_adress_or_mail;
			string	Temp_actuality;
			string	Temp_mode;
			string	Temp_date;
			string	Temp_time;
			string	Temp_theme;
			string	Temp_text;

			Temp_nzap = IntToStr(kolvo);
			Temp_adress_or_mail = '5';

			Temp_from = iterSource->Getfrom();
			Temp_to = iterSource->Getto();
			Temp_FIO_from = iterSource->GetFIO_from();
			Temp_FIO_to = iterSource->GetFIO_to();
			Temp_group = iterSource->Getgroup();
			Temp_actuality = iterSource->Getactuality();
			Temp_mode = iterSource->GetMode();
			Temp_date = iterSource->GetDate();
			Temp_time = iterSource->GetTime();
			Temp_theme = iterSource->Gettheme();
			Temp_text = iterSource->GetText();

			vSource.push_back(Mail(Temp_nzap, Temp_nadr, Temp_nomp, Temp_notp, Temp_from, Temp_to, Temp_FIO_from,
				Temp_FIO_to, Temp_group, Temp_adress_or_mail, Temp_actuality, Temp_mode, Temp_date, Temp_time, Temp_theme, Temp_text));
		}

		else
		{
			kolvo = kolvo - 1;
			cout << "\nАдресная книга уже содерхит этот адрес.";
		}
	}

	// добавить адрес в адресную книгу
	void DeleteAbook(vector <Mail> &vSource, string n_del, short &kolvo)
	{
		vector <Mail>::iterator iterSource = vSource.begin();

		short tmp_np = 0;
		int tmp_flg = 0;
		string tmp_nzap;

		// Находим
		int n_fnd = 0;
		while (iterSource != vSource.end())
		{
			string tmp_n = iterSource->GetNzap();
			if (tmp_n == n_del)
			{
				n_fnd = 1;
				break;
			}
			iterSource++;
		}

		if (n_fnd == 1)
		{
			string	Temp_nzap;
			string	Temp_nadr;
			string	Temp_nomp;
			string	Temp_notp;
			string	Temp_from;
			string	Temp_to;
			string	Temp_FIO_from;
			string	Temp_FIO_to;
			string	Temp_group;
			string	Temp_adress_or_mail;
			string	Temp_actuality;
			string	Temp_mode;
			string	Temp_date;
			string	Temp_time;
			string	Temp_theme;
			string	Temp_text;

			Temp_nzap = iterSource->GetNzap();
			Temp_nadr = iterSource->GetNadr();
			Temp_nomp = iterSource->GetNomp();
			Temp_notp = iterSource->GetNotp();
			Temp_from = iterSource->Getfrom();
			Temp_to = iterSource->Getto();
			Temp_FIO_from = iterSource->GetFIO_from();
			Temp_FIO_to = iterSource->GetFIO_to();
			Temp_group = iterSource->Getgroup();
			Temp_adress_or_mail = iterSource->Getadress_or_mail();
			Temp_actuality = iterSource->Getactuality();
			Temp_mode = iterSource->GetMode();
			Temp_date = iterSource->GetDate();
			Temp_time = iterSource->GetTime();
			Temp_theme = iterSource->Gettheme();
			Temp_text = iterSource->GetText();

			// чистим буфер клавиатуры
			cin.clear();
			cin.sync();

			// Редактируем поле Temp_actuality
			Temp_actuality = "2";

			char chv;
			cout << " Письмо будет удалено. Подтвердите удаление y/n :";
			cin >> chv;
			if (chv == 'y')
			{
				// Удаляем старое письмо
				vSource.erase(iterSource);

				// Добавляем обновленное
				vSource.push_back(Mail(Temp_nzap, Temp_nadr, Temp_nomp, Temp_notp, Temp_from, Temp_to, Temp_FIO_from,
					Temp_FIO_to, Temp_group, Temp_adress_or_mail, Temp_actuality, Temp_mode, Temp_date, Temp_time, Temp_theme, Temp_text));
				cout << "\nПисьмо №: " << n_del << " удалено из Адресной книги." << endl;
				press_any_key(); 
			}
			else
			{
				
				cout << " Удаление отменено" << endl;
				press_any_key();
			}
		}

		else cout << "Письмо с таким номером не найдено, № " << n_del;
	}

	//удаление писем
	void EditAbook(vector <Mail> &vSource, string n_edit)
	{
		vector <Mail>::iterator iterSource = vSource.begin();

		short tmp_np = 0;
		int tmp_flg = 0;
		string tmp_nzap;

		// Находим
		int n_fnd = 0;
		while (iterSource != vSource.end())
		{
			string tmp_n = iterSource->GetNzap();
			if (tmp_n == n_edit)
			{
				n_fnd = 1;
				break;
			}
			iterSource++;
		}

		if (n_fnd == 1)
		{
			string	Temp_nzap;
			string	Temp_nadr;
			string	Temp_nomp;
			string	Temp_notp;
			string	Temp_from;
			string	Temp_to;
			string	Temp_FIO_from;
			string	Temp_FIO_to;
			string	Temp_group;
			string	Temp_adress_or_mail;
			string	Temp_actuality;
			string	Temp_mode;
			string	Temp_date;
			string	Temp_time;
			string	Temp_theme;
			string	Temp_text;

			Temp_nzap = iterSource->GetNzap();
			Temp_nadr = iterSource->GetNadr();
			Temp_nomp = iterSource->GetNomp();
			Temp_notp = iterSource->GetNotp();
			Temp_from = iterSource->Getfrom();
			Temp_to = iterSource->Getto();
			Temp_FIO_from = iterSource->GetFIO_from();
			Temp_FIO_to = iterSource->GetFIO_to();
			Temp_group = iterSource->Getgroup();
			Temp_adress_or_mail = iterSource->Getadress_or_mail();
			Temp_actuality = iterSource->Getactuality();
			Temp_mode = iterSource->GetMode();
			Temp_date = iterSource->GetDate();
			Temp_time = iterSource->GetTime();
			Temp_theme = iterSource->Gettheme();
			Temp_text = iterSource->GetText();

			// чистим буфер клавиатуры
			cin.clear();
			cin.sync();

			// Редактируем письмо
			cout << " Редактироание письма №: " << n_edit << "" << endl;
			cout << " ----------------------------------------" << endl;
			cout << " Кому                                   : " << iterSource->Getto() << "" << endl;
			f_ina(" Кому - введите новое значение          : ", Temp_to, 50);
			cout << " ФИО получателя                         : " << iterSource->GetFIO_to() << "" << endl;
			f_inp(" ФИО получателя - введите новое значение: ", Temp_FIO_to, 25);
			cout << " Тема письма                            : " << iterSource->Gettheme() << "" << endl;
			f_inp(" Тема письма - введите новое значение   : ", Temp_theme, 50);
			cout << " Текст письма                           : " << iterSource->GetText() << "" << endl;
			f_inp(" Текст письма - введите новое значение  : ", Temp_text, 254);


			// Сохранить редактирование
			char chv;
			cout << " Сохранить сделанные изменения? y/n :";
			cin >> chv;
			if (chv == 'y')
			{
				// Удаляем старое письмо
				vSource.erase(iterSource);

				// Добавляем обновленное
				vSource.push_back(Mail(Temp_nzap, Temp_nadr, Temp_nomp, Temp_notp, Temp_from, Temp_to, Temp_FIO_from,
					Temp_FIO_to, Temp_group, Temp_adress_or_mail, Temp_actuality, Temp_mode, Temp_date, Temp_time, Temp_theme, Temp_text));
				cout << " Изменения сохранены" << endl;
				press_any_key();
			}
			else
			{
				cout << " Изменения отменены" << endl;
				press_any_key();
			}
		}
		else cout << " Письмо№: " << n_edit << " не найдено..." << endl;

	}

	//редактирование писем
	void Save_and_continue(vector <Mail> &vSource, short &kolvo)
	{
		if (vSource.empty())
		{
			cout << "Адресная книга пуста";
			return;
		}

		FILE *fout;
		fout = fopen("DATA.bin", "wb");

		//запись в бинарный файл
		fseek(fout, 0, SEEK_SET);
		fwrite(&kolvo, 2, 1, fout);

		vector <Mail>::iterator iterSource = vSource.begin();
		string str;
		while (iterSource != vSource.end())
		{
			Fields ms;
			char sv[7];
			char *ch1 = S_cvt(iterSource->GetNzap());	strncpy_s(sv, ch1, 7); 	ms.s_nzap = atoi(sv);
			char *ch2 = S_cvt(iterSource->GetNadr());	strncpy_s(sv, ch2, 7); 	ms.s_nadr = atoi(sv);
			char *ch3 = S_cvt(iterSource->GetNomp());	strncpy_s(sv, ch3, 7); 	ms.s_nomp = atoi(sv);
			char *ch4 = S_cvt(iterSource->GetNotp());	strncpy_s(sv, ch4, 7); 	ms.s_notp = atoi(sv);
			char *ch5 = S_cvt(iterSource->Getfrom());	strncpy_s(ms.s_from, ch5, 300);
			char *ch6 = S_cvt(iterSource->Getto());	strncpy_s(ms.s_to, ch6, 300);
			char *ch7 = S_cvt(iterSource->GetFIO_from());	strncpy_s(ms.s_FIO_from, ch7, 300);
			char *ch8 = S_cvt(iterSource->GetFIO_to());	strncpy_s(ms.s_FIO_to, ch8, 300);
			char *ch9 = S_cvt(iterSource->Getgroup());	strncpy_s(ms.s_group, ch9, 300);
			char *cha = S_cvt(iterSource->Getadress_or_mail());	strncpy_s(ms.s_adress_or_mail, cha, 300);
			char *chb = S_cvt(iterSource->Getactuality());	strncpy_s(ms.s_actuality, chb, 300);
			char *chc = S_cvt(iterSource->GetMode());	strncpy_s(ms.s_mode, chc, 300);
			char *chd = S_cvt(iterSource->GetDate());	strncpy_s(ms.s_date, chd, 300);
			char *che = S_cvt(iterSource->GetTime());	strncpy_s(ms.s_time, che, 300);
			char *chf = S_cvt(iterSource->Gettheme());	strncpy_s(ms.s_theme, chf, 300);
			char *chg = S_cvt(iterSource->GetText());	strncpy_s(ms.s_text, chg, 300);

			//запись структуры в бинарный файл
			fwrite(&ms, sizeof(Fields), 1, fout);
			iterSource++;
		}
		fclose(fout);
		cout << " Изменения сохранены " << endl;
	}

	// сохранить и продолжить, сохранение изменений
	void Find_from(vector <Mail> &vSource, int &nfnd)
	{
		if (vSource.empty())
		{
			cout << "Почтовый ящик пуст";
			return;
		}
		vector <Mail>::iterator iterSource = vSource.begin();
		short tmp_np = 0;
		short tmp_flg = 0;
		bool bIsFound = false;
		string Temp_from;
		cout << "Введите контекст От кого: ";
		cin >> Temp_from;
		cin.ignore();
		while (iterSource != vSource.end())
		{
			// поиск по контексту
			if ((iterSource->Getadress_or_mail() != "5") && (iterSource->Getactuality() != "2"))
			{
				if (Str_Find(iterSource->Getfrom(), Temp_from) == 1)
				{
					tmp_np++;
					tmp_flg++;
					nfnd++;
					if (tmp_flg == 1) cout << "\n____________________________________________" << endl;
					bIsFound = true;
					cout.setf(ios::left);
					cout << " " << tmp_np << ").Письмо №: " << setw(4) << iterSource->GetNzap()
						<< "\n      От кого: " << setw(15) << iterSource->Getfrom() << " ФИО отп: " << setw(20) << iterSource->GetFIO_from()
						<< "\n      Кому   : " << setw(15) << iterSource->Getto() << " ФИО пол: " << setw(20) << iterSource->GetFIO_to()
						<< "\n      Группа : " << iterSource->Getgroup() << '\n';
					cout.unsetf(ios::left);
				}
			}
			iterSource++;
		}
		if (tmp_np > 0) cout << "\n____________________________________________" << endl;
		if (!bIsFound)
			cout << "Ни одной записи с таким контекстом адреса не было найдено";
	}

	//поиск по адресу отправителя
	void Find_to(vector <Mail> &vSource, int &nfnd)
	{
		if (vSource.empty())
		{
			cout << "Почтовый ящик пуст";
			return;
		}
		vector <Mail>::iterator iterSource = vSource.begin();
		short tmp_np = 0;
		short tmp_flg = 0;
		bool bIsFound = false;
		string Temp_to;
		cout << "Введите контекст Кому: ";
		cin >> Temp_to;
		cin.ignore();
		while (iterSource != vSource.end())
		{
			// поиск по контексту
			if ((iterSource->Getadress_or_mail() != "5") && (iterSource->Getactuality() != "2"))
			{
				if (Str_Find(iterSource->Getto(), Temp_to) == 1)
				{
					tmp_np++;
					tmp_flg++;
					nfnd++;
					if (tmp_flg == 1) cout << "\n____________________________________________" << endl;
					bIsFound = true;
					cout.setf(ios::left);
					cout << " " << tmp_np << ").Письмо №: " << setw(4) << iterSource->GetNzap()
						<< "\n      От кого: " << setw(15) << iterSource->Getfrom() << " ФИО отп: " << setw(20) << iterSource->GetFIO_from()
						<< "\n      Кому   : " << setw(15) << iterSource->Getto() << " ФИО пол: " << setw(20) << iterSource->GetFIO_to()
						<< "\n      Группа : " << iterSource->Getgroup() << '\n';
					cout.unsetf(ios::left);
				}
			}
			iterSource++;
		}
		if (tmp_np > 0) cout << "\n____________________________________________" << endl;
		if (!bIsFound)
			cout << "Ни одной записи с таким контекстом адреса не было найдено";
	}

	//поиск по адресу получателя
	void Find_FIO_to(vector <Mail> &vSource, int &nfnd)
	{
		if (vSource.empty())
		{
			cout << "Почтовый ящик пуст";
			return;
		}
		vector <Mail>::iterator iterSource = vSource.begin();
		short tmp_np = 0;
		bool bIsFound = false;
		string Temp_FIO_to;
		cout << "Введите контекст ФИО: ";
		cin >> Temp_FIO_to;
		cin.ignore();
		nfnd = 0;
		while (iterSource != vSource.end())
		{
			if ((iterSource->Getadress_or_mail() != "5") && (iterSource->Getactuality() != "2"))
			{
				// поиск по контексту
				if (Str_Find(iterSource->GetFIO_to(), Temp_FIO_to) == 1)
				{
					tmp_np++;
					
					nfnd++;
					if (tmp_np == 1) cout << "\n____________________________________________" << endl;
					bIsFound = true;
					cout.setf(ios::left);
					cout << " " << tmp_np << ").Письмо №: " << setw(4) << iterSource->GetNzap()
						<< "\n      От кого: " << setw(15) << iterSource->Getfrom() << " ФИО отп: " << setw(20) << iterSource->GetFIO_from()
						<< "\n      Кому   : " << setw(15) << iterSource->Getto() << " ФИО пол: " << setw(20) << iterSource->GetFIO_to()
						<< "\n      Группа : " << iterSource->Getgroup() << '\n';
					cout.unsetf(ios::left);
				}
			}
			iterSource++;
		}
		if (tmp_np > 0) cout << "\n____________________________________________" << endl;
		if (!bIsFound)
			cout << "Ни одной записи с таким контекстом имени не было найдено";
	}
	// поиск по фио получателя
	int check_adr(string s_adr)
	{ // проверка синтаксиса электронного адреса
		int len = s_adr.size() + 1;
		char *adr = new char[len];
		strcpy(adr, s_adr.c_str());
		bool valid = true; int all_pr = 0; string s_sym = "-_.@"; // допустимые символы помимо латинских букв и цифр
		for (int i = 0; adr[i] != 0; i++)
		{
			if (!isalpha(adr[i]) && !isdigit(adr[i]) && s_sym.find(adr[i]) == std::string::npos)
			{
				valid = false; break;
			} // наличие недопустимого символа
			if ((i > 0 && adr[i] == '.') && (adr[i - 1] == '.')) { valid = false; break; } // две точки подряд
			else if ((i > 0 && adr[i] == '@') && (!all_pr)) { all_pr = 1; } // наличие @
			else if ((adr[i] == '@') && (all_pr == 1)) { valid = false; break; } // наличие двух @ в строчке
			else if ((isalpha(adr[i]) || isdigit(adr[i])) && all_pr == 1) { all_pr = 2; } // наличие буквы или цифры после @
			else if (adr[i] == '.' && all_pr == 2) { all_pr = 3; } // наличие точки после символа( который после @ )
			else if ((isalpha(adr[i])) && (all_pr == 3) && (adr[i + 1] == 0)) { all_pr = 4; } // наличие буквы в конце строчки
		}
		
		for (int i = 0; adr[i] != 0; i++)
		{
			if (isupper(adr[i]))
			{
				all_pr = all_pr - 1;
				break;
			}
		}

		if (all_pr == 4) return 1; 	else return 0;
	}



	void operator =(Mail obj)
	{
		nzap = obj.nzap;
		nadr = obj.nadr;
		nomp = obj.nomp;
		notp = obj.notp;
		from = obj.from;
		to = obj.to;
		FIO_from = obj.FIO_from;
		FIO_to = obj.FIO_to;
		group = obj.group;
		adress_or_mail = obj.adress_or_mail;
		actuality = obj.actuality;
		mode = obj.mode;
		date = obj.date;
		time = obj.time;
		theme = obj.theme;
		text = obj.text;
	}

	string GetNzap()	{ return nzap; }
	string GetNadr()	{ return nadr; }
	string GetNomp()	{ return nomp; }
	string GetNotp()	{ return notp; }
	string Getfrom()	{ return from; }
	string Getto()	{ return to; }
	string GetFIO_from()	{ return FIO_from; }
	string GetFIO_to()	{ return FIO_to; }
	string Getgroup()	{ return group; }
	string Getadress_or_mail()	{ return adress_or_mail; }
	string Getactuality()	{ return actuality; }
	string GetMode()	{ return mode; }
	string GetDate()	{ return date; }
	string GetTime()	{ return time; }
	string Gettheme()	{ return theme; }
	string GetText()	{ return text; }

private:
	string TrimS(string &st, string parm)
	{	// удаление лидирующих и/или завершающих пробелов и символов табуляции в строке: 
		// "r" - справа, "l" - слева, "all" - справа и слева 
		if (st.size()) //пустая строка остается без изменений
		{
			if (parm == "r" || parm != "l")
			{
				int i = st.size();
				while (i-- >= 0)	if (st[i] != ' ' && st[i] != '\t')	break;
				st.resize(++i); 	if (parm == "r") return st;
			}

			if (parm == "l" || parm != "r")
			{
				int i = 0, ln = st.size();
				while (i < ln) { if (st[i] != ' ' && st[i] != '\t')			break;	i++; }
				st.erase(0, i);
				return st;
			}
		}
		return st;
	}

	int TstDate(int &d, int &m, int &y)
	{	// Проверка даты по календарю
		int daymon2[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // если год не кратен четырем
		int daymon4[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // если год кратен четырем
		
		if ((y < 1901) || (y > 2099))	return 1;
		if ((m < 1) || (m > 12))		return 1;
		if ((y / 4.0) - (abs(y / 4.0)) > 0)	if ((d < 0) || (d > daymon2[m - 1])) return 1;
		else if ((d < 0) || (d > daymon4[m - 1])) return 1;
		return 0;
	}

	string IntToStr(int x)
	{ // преобразование int в string для обычного числа
		stringstream r;
		r << x;
		return r.str();
	}

	string IntToStrDT(int x)
	{ // преобразование для часов и минут int в string
		stringstream r;
		r << x;
		if (x > 9) return r.str();
		else return "0" + r.str(); // числа меньшие 9-ти преобразуетя в "00" ... "09"
	}

	int StrToIntDT(string r)
	{
		// string to int
		istringstream buf(r);
		int x;
		buf >> x;
		return x;
	}



protected:
	string nzap;
	string nadr;
	string nomp;
	string notp;
	string from;
	string to;
	string FIO_from;
	string FIO_to;
	string group;
	string adress_or_mail;
	string actuality;
	string mode;
	string date;
	string time;
	string theme;
	string text;
};

class Menu
{
	struct MenuItem
	{
		string title;

		void(*proc)();

		MenuItem(string aTitle, void(*procLink)())
		{
			title = aTitle;
			proc = procLink;
		}
	};

	string menuTitle;
	vector<MenuItem> menuItems;


	void GetMenuView();
public:

	
	Menu()
	{
		menuTitle = "Main menu";
	}
	Menu(string menuTitle)
	{
		this->menuTitle = menuTitle;
	}

	
	void AddMenuItem(string itemTitle, void(*procLink)());
	void ShowMenu();
};



void Menu::GetMenuView()
{
	system("cls");

	for (unsigned int i = 0; i<menuTitle.size() + 10; i++)
		cout << "=";
	cout << endl;
	cout << menuTitle << endl;
	for (unsigned int i = 0; i<menuTitle.size() + 10; i++)
		cout << "=";
	cout << endl;

	int ItemNumber = 1;
	for (vector<MenuItem>::iterator it = menuItems.begin(); it != menuItems.end(); it++, ItemNumber++)
	{
		cout << ItemNumber << " ->) " << it->title << endl;
	}
	cout << endl << "Type \"q\" for Exit " << endl;

	cout << endl;
	cout << endl;

	cout << "Type item num..." << endl;
}

void Menu::AddMenuItem(string itemTitle, void(*procLink)())
{
	menuItems.push_back(*(new MenuItem(itemTitle, procLink)));
}

void Menu::ShowMenu()
{
	
	vector <Mail> DataBase;
	size_t file_size = 0;
	Mail obj;
	int cKey;
	int n_flg;
	short kolvo = 0; // адресная книга может быть пустой
	short n_nadr = 0, n_nzap = 0;

	// Прочитать бинарный файл в вектор
	FILE *fin;
	fin = fopen("DATA.bin", "rb");

	if (fin != NULL)
	{  // Длина файла
		fseek(fin, 0, SEEK_END);
		file_size = ftell(fin);
		if (file_size == 0) fclose(fin); // закрываем пустой файл
	}

	// Чтение непустого файла
	if (file_size != 0) // курсор в начало
	{
		fseek(fin, 0, SEEK_SET);
		// читаем 2 байта
		fread(&kolvo, 2, 1, fin);

		// читаем поля объекта класса Fields
		Fields ms2;
		string cp1, cp2, cp3, cp4;
		for (int i = 0; i < kolvo; i++)
		{
			fread(&ms2, sizeof(Fields), 1, fin);
			if (!feof(fin))
			{
				n_nzap = n_nzap + 1;
				if (ms2.s_nadr > n_nadr) n_nadr = ms2.s_nadr;
				char num1[6] = { '\0' };	cp1 = itoa(ms2.s_nzap, num1, 6);
				char num2[6] = { '\0' };	cp2 = itoa(ms2.s_nadr, num2, 6);
				char num3[6] = { '\0' };	cp3 = itoa(ms2.s_nomp, num3, 6);
				char num4[6] = { '\0' };	cp4 = itoa(ms2.s_notp, num4, 6);

				obj.Input2(DataBase, cp1, cp2, cp3, cp4, ms2.s_from, ms2.s_to, ms2.s_FIO_from, ms2.s_FIO_to,
					ms2.s_group, ms2.s_adress_or_mail, ms2.s_actuality, ms2.s_mode, ms2.s_date, ms2.s_time, ms2.s_theme, ms2.s_text);
			}
		}
		fclose(fin);
	}


	do{
		system("cls");
		cout << "         ЭЛЕКТРОННАЯ ПОЧТА          " << endl;
		cout << endl;
		cout << endl;
		cout << "      1.Список Контактов   		" << endl << endl;
		cout << "      РАБОТА С ПИСЬМАМИ:			" << endl;
		cout << "      2.Написать письмо       		" << endl;
		cout << "      3.Отредактировать письмо		" << endl;
		cout << "      4.Удалить письмо				" << endl << endl;
		cout << "      5.Все письма					" << endl;
		cout << "      6.Входящие              		" << endl;
		cout << "      7.Исходящие					" << endl;
		cout << "      8.Отправленные				" << endl << endl;
		cout << "      9.Сохранить и продолжить		" << endl << endl;
		cout << "      10.Поиск в Адресной Книге	" << endl << endl;
		cout << "	   0.Выход						" << endl << endl;

		do
		{
			cin.clear();
			cin.sync();
			cin >> cKey;
			if (cin.fail())
			{
				cout << " Ошибка. Пункт - целое число в диапозоне..";
				cout << " Пожалуйста, введите заново  ";
				system("pause");
			}
		} while (cin.fail());
		cin.sync();

		n_flg = 0;


		// 1.Открыть адресную книгу
		if (cKey == 1)
		{
			obj.Print_Addres_book(DataBase, kolvo);
		}

		// 2.Написать письмо       
		else if (cKey == 2)
		{
			kolvo = kolvo + 1;
			obj.Input4(DataBase, 2, kolvo, n_nadr);
		}

		// 3.Отредактировать письмо			
		else if (cKey == 3)
		{
			int k;
			cout << "Введите 1 - если поиск письма для редактирования по ФИО получателя" << endl;
			cout << "Введите 2 - если поиск письма для редактирования по адресу От кого" << endl;
			cout << "Введите 3 - если поиск письма для редактирования по адресу Кому" << endl;
			cin >> k;
			if (k == 1)
			{
				int nfnd = 0;
				obj.Find_FIO_to(DataBase, nfnd);
				if (nfnd > 0)
				{
					int k;
					cout << "\nВведите 1 - если надо отредактировать письмо из адресной книги" << endl;
					cout << "Введите 2 - чтобы выйти в главное меню" << endl;
					cin >> k;
					if (k == 1)
					{
						string k4 = "";
						cout << "В адресной книге следует отредактировать Письмо №: " << endl;
						cin >> k4;
						obj.EditAbook(DataBase, k4);
					}
				}
			}
			if (k == 2)
			{
				int nfnd = 0;
				obj.Find_from(DataBase, nfnd);
				if (nfnd > 0)
				{
					int k;
					cout << "\nВведите 1 - если надо отредактировать письмо из адресной книги" << endl;
					cout << "Введите 2 - чтобы выйти в главное меню" << endl;
					cin >> k;
					if (k == 1)
					{
						string k4 = "";
						cout << "В адресной книге следует отредактировать Письмо №: " << endl;
						cin >> k4;
						obj.EditAbook(DataBase, k4);
					}
					if (k == 2)
					{
						k = 0;
					}
				}
			}
			if (k == 3)
			{
				int nfnd = 0;
				obj.Find_to(DataBase, nfnd);
				if (nfnd > 0)
				{
					int k;
					cout << "\nВведите 1 - если надо отредактировать письмо из адресной книги" << endl;
					cout << "Введите 2 - чтобы выйти в главное меню" << endl;
					cin >> k;
					if (k == 1)
					{
						string k4 = "";
						cout << "В адресной книге следует отредактировать Письмо №: " << endl;
						cin >> k4;
						obj.EditAbook(DataBase, k4);
					}
				}
			}
		}

		// 4.Удалить письмо			
		else if (cKey == 4)
		{
			int k;
			cout << "Введите 1 - если поиск письма для удаления по ФИО получателя" << endl;
			cout << "Введите 2 - если поиск письма для удаления по отправителю" << endl;
			cout << "Введите 3 - если поиск письма для удаления по адресу получателю" << endl;
			cin >> k;
			if (k == 1)
			{
				int nfnd = 0;
				obj.Find_FIO_to(DataBase, nfnd);
				if (nfnd > 0)
				{
					int k;
					cout << "Введите 1 - если надо удалить письмо из адресной книги" << endl;
					cout << "Введите 2 - чтобы выйти в главное меню" << endl;
					cin >> k;
					if (k == 1)
					{
						string k4 = "";
						cout << "Из адресной книги следует удалить Письмо №: ";
						cin >> k4;

						obj.DeleteAbook(DataBase, k4, kolvo);
					}
				}
			}
			if (k == 2)
			{
				int nfnd = 0;
				obj.Find_from(DataBase, nfnd);
				if (nfnd > 0)
				{
					int k;
					cout << "\nВведите 1 - если надо удалить письмо из адресной книги" << endl;
					cout << "Введите 2 - чтобы выйти в главное меню" << endl;
					cin >> k;
					if (k == 1)
					{
						string k4 = "";
						cout << "Из адресной книги следует удалить Письмо №: ";
						cin >> k4;
						
						obj.DeleteAbook(DataBase, k4, kolvo);
					}
					if (k == 2)
					{
						k = 0;
					}
				}
			}
			if (k == 3)
			{
				int nfnd = 0;
				obj.Find_to(DataBase, nfnd);
				if (nfnd > 0)
				{
					int k;
					cout << "Введите 1 - для удаления письма из адресной книги" << endl;
					cout << "Введите 2 - чтобы выйти в главное меню" << endl;
					cin >> k;
					if (k == 1)
					{
						string k4 = "";
						cout << "Из Адресной Книги следует удалить Письмо №: ";
						cin >> k4;

						obj.DeleteAbook(DataBase, k4, kolvo);
					}
				}
			}
		}

		// 5.Все письма			
		else if (cKey == 5)
		{
			int nfnd = 0;
			obj.PrintMail(DataBase, "3", nfnd);
			if (nfnd > 0)
			{
				int k;
				cout << "\nВведите 1 - если надо добавить письмо в адресную книгу" << endl;
				cout << "Введите 2 - чтобы выйти в главное меню" << endl;
				cin >> k;
				if (k == 1)
				{
					string k4 = "";
					cout << "В Адресную Книгу следует добавить Письмо №: ";
					cin >> k4;
					kolvo = kolvo + 1;
					obj.PrintBook(DataBase, k4, kolvo);
				}
				if (k == 2)
				{
					obj.press_any_key();
				}
			}
		}

		// 6.Входящие               
		else if (cKey == 6)
		{
			int nfnd = 0;
			obj.PrintMail(DataBase, "1", nfnd);
		}

		// 7.Исходящие			
		else if (cKey == 7)
		{
			int nfnd = 0;
			obj.PrintMail(DataBase, "2", nfnd);
		}

		// 8.Отправленные			
		else if (cKey == 8)
		{
			int nfnd = 0;
			obj.PrintMail(DataBase, "4", nfnd);
		}

		

		// 9.Сохранить и продолжить
		else if (cKey == 9)
		{
			obj.Save_and_continue(DataBase, kolvo);
		}

		// 10.Начать поиск			
		else if (cKey == 10)
		{
			int k;
			cout << "Введите 1 - если поиск по ФИО получателя" << endl;
			cout << "Введите 2 - если поиск по адресу отправителя" << endl;
			cout << "Введите 3 - если поиск по адресу получателя" << endl;
			cin >> k;
			if (k == 1)
			{
				int nfnd = 0;
				obj.Find_FIO_to(DataBase, nfnd);
				if (nfnd > 0)
				{
					int k;
					cout << "\nВведите 1 - если надо добавить письмо в адресную книгу" << endl;
					cout << "Введите 2 - чтобы выйти в главное меню" << endl;
					cin >> k;
					if (k == 1)
					{
						string k4 = "";
						cout << "В Адресную Книгу следует добавить Письмо №: ";
						cin >> k4;
						kolvo = kolvo + 1;
						obj.PrintBook(DataBase, k4, kolvo);
					}
				}
			}
			if (k == 2)
			{
				int nfnd = 0;
				obj.Find_from(DataBase, nfnd);
				if (nfnd > 0)
				{
					int k;
					cout << "\nВведите 1 - если надо добавить письмо в адресную книгу" << endl;
					cout << "Введите 2 - чтобы выйти в главное меню" << endl;
					cin >> k;
					if (k == 1)
					{
						string k4 = "";
						cout << "В Адресную Книгу следует добавить Письмо №: ";
						cin >> k4;
						kolvo = kolvo + 1;
						obj.PrintBook(DataBase, k4, kolvo);
					}
					if (k == 2)
					{
						k = 0;
					}
				}
			}
			if (k == 3)
			{
				int nfnd = 0;
				obj.Find_to(DataBase, nfnd);
				if (nfnd > 0)
				{
					int k;
					cout << "\nВведите 1 - если надо добавить письмо в адресную книгу" << endl;
					cout << "Введите 2 - чтобы выйти в главное меню" << endl;
					cin >> k;
					if (k == 1)
					{
						string k4 = "";
						cout << "В Адресную Книгу следует добавить Письмо №: ";
						cin >> k4;
						kolvo = kolvo + 1;
						obj.PrintBook(DataBase, k4, kolvo);
					}
				}
			}
		}

		

		

		// 0.Выход				
		else if (cKey == 0)
		{
			char chv;
			cout << " Сохранить сделанные изменения? y-да : n - нет :" << endl;
			cin >> chv;
			if (chv == 'y')
			{
				obj.Save_and_continue(DataBase, kolvo);
				system("pause");
				break;
				
			}
			else
			{
				cout << " Изменения отменены" << endl << endl;
				cKey = 12;
				system("pause");
				break;
				
			}
		}
		else if (cKey > 11)
		{
			cout << " Try ones more time ..." << endl;
			
		}
		else

			continue;

		_getch();

	} while (cKey != 0);

	system("pause");

	
}


int main()
{
	setlocale(LC_ALL, "Russian");
	Menu someMenu("Main menu");


	someMenu.ShowMenu();

	return 0;
}




