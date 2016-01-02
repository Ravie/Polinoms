#include <stdio.h>
#include <tchar.h>
#include <string>

using namespace std;

class Monom
{
	int coef; // числовой коэффициент монома
	int sv; // свертка 
	Monom *next;
public:
	Monom(int _coef = 0, int _sv = -1)
	{
		coef = _coef;
		sv = _sv;
		next = 0;
	}

	~Monom() {}

	Monom(Monom *tmp)
	{
		coef = tmp->coef;
		sv = tmp->sv;
		next = 0;
	}

	Monom &operator=(Monom tmp)
	{
		coef = tmp.coef;
		sv = tmp.sv;
		return *this;
	}

	void SetNext(Monom *tmp)
	{
		next = tmp;
	}

	Monom *GetNext()
	{
		return next;
	}

	int GetSV()
	{
		return sv;
	}

	int GetCoef()
	{
		return coef;
	}

	void SetCoef(int _coef)
	{
		coef = _coef;
	}

	void SetSV(int _sv)
	{
		sv = _sv;
	}
};
class CList
{
	Monom *head;
public:
	CList()
	{
		head = new Monom;
		head->SetNext(head);
	}

	~CList()
	{
		Monom *current = head->GetNext();
		while (current != head)
		{
			head->SetNext(current->GetNext());
			current->SetNext(NULL);
			delete current;
			current = head->GetNext();
		}
		head->SetNext(NULL);
		delete head;
	}
	// delete all monoms except head link in circle list 
	void CDelete()
	{
		Monom *current = head->GetNext();
		while (current != head)
		{
			head->SetNext(current->GetNext());
			current->SetNext(NULL);
			delete current;
			current = head->GetNext();
		}
	}

	CList(CList &tmp)
	{
		head = new Monom;
		head->SetNext(head);
		Monom *current = tmp.head->GetNext();
		Monom *last = head;
		Monom *box;
		while (current != tmp.head)
		{
			box = new Monom(*current);
			box->SetNext(head);
			last->SetNext(box);
			last = last->GetNext();
			current = current->GetNext();
		}
	}

	CList &operator=(CList &tmp)
	{
		CDelete();
		Monom *current = tmp.head->GetNext();
		Monom *last = head;
		Monom *box;
		while (current != tmp.head)
		{
			box = new Monom(*current);
			box->SetNext(head);
			last->SetNext(box);
			last = last->GetNext();
			current = current->GetNext();
		}
		return *this;
	}
	// addition monom to circle list
	void AddMonom(int sv, int coef)
	{
		Monom *tmp;
		tmp = new Monom(coef, sv);
		Monom *prev = head;
		Monom *current = head->GetNext();
		while (current->GetSV() > sv)
		{
			prev = current;
			current = current->GetNext();
		}
		if (sv > current->GetSV())
		{
			tmp->SetNext(current);
			prev->SetNext(tmp);
		}
		else
		{
			coef = coef + current->GetCoef();
			if (coef == 0)
			{
				prev->SetNext(current->GetNext());
				current->SetNext(nullptr);
				delete current;
			}
			else
			{
				current->SetCoef(coef);
				delete tmp;
			}
		}
	}
	// combination of 2 circle lists
	void Combine(CList tmp)
	{
		Monom *current = tmp.head->GetNext();
		while (current != tmp.head)
		{
			AddMonom(current->GetSV(), current->GetCoef());
			current = current->GetNext();
		}
	}
	// make polinom from clist
	string GetPolinom(int maxSt)
	{
		Monom*current = head->GetNext();
		string polinom = "";
		string s, m, l;
		int a = 0, b = 0, c = 0, k;
		while (current != head)
		{
			a = (current->GetSV()) / (maxSt*maxSt);
			b = (current->GetSV()) / maxSt % maxSt;
			c = (current->GetSV()) % maxSt;
			k = current->GetCoef();
			if (k < 0)
				polinom = polinom + to_string(k);
			else
				polinom = polinom + "+" + to_string(k);
			polinom = polinom + "x^" + to_string(a) + "y^" + to_string(b) + "z^" + to_string(c);
			current = current->GetNext();
		}
		return polinom;
	}
	// multiplication the number on list
	CList operator*(int c)
	{
		Monom *current = head->GetNext();
		while (current != head)
		{
			current->SetCoef((current->GetCoef())*c);
			current = current->GetNext();
		}
		return *this;
	}

	void MultiMonom(int _maxSt, int _coef, int _sv)
	{
		int power[3] = { 0, 0, 0 };
		power[0] = _sv / (_maxSt*_maxSt);
		power[1] = (_sv / _maxSt) % _maxSt;
		power[2] = _sv % _maxSt;
		Monom *current = head->GetNext();
		while (current != head)
		{
			int curr_power[3] = { 0, 0, 0 };
			int curr_sv = current->GetSV();

			curr_power[0] = curr_sv / (_maxSt*_maxSt);
			curr_power[1] = (curr_sv / _maxSt) % _maxSt;
			curr_power[2] = curr_sv % _maxSt;

			for (int i = 0; i < 3; i++)
				if ((curr_power[i] + power[i])>_maxSt)
				{
					CDelete();
					current = head;
				}

			current->SetSV(current->GetSV() + _sv);
			current->SetCoef((current->GetCoef())*_coef);
			current = current->GetNext();
		}
	}

	CList MultiList(int _maxSt, CList tmp)
	{
		CList _copy(*this);
		CList newList;
		Monom *current = tmp.head->GetNext();
		while (current != tmp.head)
		{
			_copy.MultiMonom(_maxSt, current->GetCoef(), current->GetSV());
			newList.Combine(_copy);
			current = current->GetNext();
			_copy = *this;
		}
		return newList;
	}
};
class Polinom
{
	int maxSt;
	CList List;
public:
	void StrToMonoms(string spolinom, int &k, string *monoms)
	{
		int len = spolinom.length();
		k = 0;
		monoms[0] = "";
		for (int i = 0; i < len; i++)
		{
			if ((spolinom[i] == '+') || (spolinom[i] == '-'))
				if (monoms[k] != "")
				{
					k++;
					monoms[k] = "";
				}
			monoms[k] = monoms[k] + spolinom[i];
		}
	}

	void MakeMonom(string in)
	{
		const string st = "xyz";
		int power[3] = { 0, 0, 0 };
		string tmp = "";
		for (int i = 0; i < 3; i++)
		{
			int pos = in.find(st[i]);
			if (pos >= 0)
				if (in[pos + 1] != '^')
				{
					power[i] = 1;
					in.erase(pos, 1);
				}
				else
				{
					tmp = "";
					for (int j = pos + 2; (j < in.length()) && ('0' <= in[j]) && ('9' >= in[j]); j++)
						tmp = tmp + in[j];
					power[i] = stoi(tmp);
					in.erase(pos, tmp.length() + 2);
				}
		}
		if (in == "" || in == "+" || in == "-")
			in = in + "1";
		int coef = stoi(in);
		int sv = power[0] * maxSt*maxSt + power[1] * maxSt + power[2];
		if (((power[0]) < maxSt) && ((power[1]) < maxSt) && ((power[2]) < maxSt))
			List.AddMonom(sv, coef);
	}

	Polinom(int _maxSt = 10, string _st = "")
	{
		maxSt = _maxSt;
		string *a;
		a = new string[_st.length() / 2 + 1];
		int n;
		StrToMonoms(_st, n, a);
		for (int i = 0; i <= n; i++)
			MakeMonom(a[i]);
	}

	Polinom(Polinom &tmp)
	{
		maxSt = tmp.maxSt;
		List = tmp.List;
	}

	Polinom &operator=(Polinom &tmp)
	{
		maxSt = tmp.maxSt;
		List = tmp.List;
		return *this;
	}

	~Polinom() {}

	void Input(string spolinom)
	{
		Polinom tmp(maxSt, spolinom);
		List = tmp.List;
	}

	string Output()
	{
		return List.GetPolinom(maxSt);
	}

	Polinom operator+(Polinom tmp)
	{
		Polinom result(*this);
		result.List.Combine(tmp.List);
		return result;
	}

	Polinom operator*(int c)
	{
		Polinom result(*this);
		result.List = (result.List)*c;
		return result;
	}

	Polinom operator-(Polinom tmp)
	{
		Polinom result(*this);
		result.List.Combine((tmp.List)*(-1));
		return result;
	}

	Polinom operator*(Polinom tmp)
	{
		Polinom a(*this);
		a.List = a.List.MultiList(a.maxSt, tmp.List);
		return a;
	}
};