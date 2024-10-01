// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля
// тест коммита на windows
// тест коммита на в Xcode+github desktop

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);
using namespace std;

TSet::TSet(int mp) : BitField(mp)
{
    if (mp < 0) throw std::underflow_error("underflow_error"); //обработка ошибки, если кол-во эл-ов не положительное
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.GetMaxPower())
{
    MaxPower = s.GetMaxPower();
    BitField = s.BitField;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf.GetLength())
{
    MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    return FAKE_BITFIELD;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem >= MaxPower || Elem<0) throw std::underflow_error("underflow_error"); //обработка ошибки, если кол-во эл-ов не положительное //элементы: от 0 до MaxPower-1
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem >= MaxPower || Elem < 0) throw std::underflow_error("underflow_error"); //обработка ошибки, если кол-во эл-ов не положительное //элементы: от 0 до MaxPower-1
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem>=MaxPower || Elem < 0) throw std::underflow_error("underflow_error"); //обработка ошибки, если кол-во эл-ов не положительное //элементы: от 0 до MaxPower-1
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this != &s) {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if (MaxPower != s.MaxPower) return 0;
    return (s.BitField==BitField);
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    if (MaxPower != s.MaxPower) return 1;
    return (s.BitField != BitField);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TSet Result(*this);
    if (s.MaxPower > MaxPower) Result = s;
    Result.BitField = BitField | s.BitField;
    return Result;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem >= MaxPower || Elem < 0) throw std::underflow_error("underflow_error"); //обработка ошибки, если кол-во эл-ов не положительное //элементы: от 0 до MaxPower-1
    TSet Result(*this);
    Result.BitField.SetBit(Elem);
    return Result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if (Elem >= MaxPower || Elem < 0) throw std::underflow_error("underflow_error"); //обработка ошибки, если кол-во эл-ов не положительное //элементы: от 0 до MaxPower-1
    TSet Result(*this);
    Result.BitField.ClrBit(Elem);
    return Result;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TSet Result(*this);
    if (s.MaxPower > MaxPower) Result = s;
    Result.BitField = BitField & s.BitField;
    return Result;
}

TSet TSet::operator~(void) // дополнение
{
    TSet Result(*this);
    Result.BitField = (~BitField);
    return Result;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    istr >> s.BitField;
    s.MaxPower = s.BitField.GetLength();
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    ostr << s.BitField;
    return ostr;
}
