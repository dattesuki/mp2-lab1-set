// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <algorithm>
#include <string>

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0) throw std::underflow_error("underflow_error"); //обработка ошибки, если кол-во эл-ов не положительное
    BitLen = len; //количество элементов
    MemLen = (len / (sizeof(TELEM) * 8)) + 1;//к-во эл-тов Мем для представления бит.поля = количество элементов / количество элементов в TELEM (+1 для хвоста)
    pMem = new TELEM[MemLen];
    if (pMem == nullptr) throw domain_error("domain_error");
    for (int i = 0; i < MemLen; i++) pMem[i] = (TELEM)0;
}


TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    if (pMem == nullptr) throw domain_error("domain_error");
    for (int i = 0; i < (MemLen); i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0) throw std::underflow_error("underflow_error"); //обработка ошибки, если элемент отрицательный
    if (n >= BitLen) throw out_of_range("out_of_range"); //обработка ошибки, если происходит попытка доступа к элементу за границей заданного диапазона
    return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0) throw std::underflow_error("underflow_error"); //обработка ошибки, если элемент отрицательный
    if (n >= BitLen) throw out_of_range("out_of_range"); //обработка ошибки, если происходит попытка доступа к элементу за границей заданного диапазона
    return ((TELEM)1 << ((n % (sizeof(TELEM) * 8))));
}

// доступ к битам битового поля
int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return (BitLen);
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0) throw std::underflow_error("underflow_error"); //обработка ошибки, если элемент отрицательный
    if (n >= BitLen) throw out_of_range("out_of_range"); //обработка ошибки, если происходит попытка доступа к элементу за границей заданного диапазона
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0) throw std::underflow_error("underflow_error"); //обработка ошибки, если элемент отрицательный
    if (n >= BitLen) throw out_of_range("out_of_range"); //обработка ошибки, если происходит попытка доступа к элементу за границей заданного диапазона
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}


int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0) throw std::underflow_error("underflow_error"); //обработка ошибки, если элемент отрицательный
    if (n >= BitLen) throw out_of_range("out_of_range"); //обработка ошибки, если происходит попытка доступа к элементу за границей заданного диапазона
    if ((pMem[GetMemIndex(n)] & GetMemMask(n)) == (TELEM)0) return 0;
    else return 1;
}


// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf) {
        BitLen = bf.BitLen;
        if (MemLen != bf.MemLen) {
            MemLen = bf.MemLen;
            delete[] pMem;
            pMem = new TELEM[MemLen];
        }
        if (pMem == nullptr) throw domain_error("domain_error");
        for (int i = 0; i < MemLen + 1; i++) {
            pMem[i] = bf.pMem[i];
        }
        return *this;
    }
}

int TBitField::operator==(const TBitField& bf) const {// сравнение;
    if (BitLen != bf.BitLen) return 0; 
    for (int i = 0; i < MemLen; i++) if (pMem[i]!=bf.pMem[i]) return 0;
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    TBitField Result(max(BitLen, bf.BitLen));
    for (int i = 0; i < min(MemLen, bf.MemLen); i++) Result.pMem[i] = (pMem[i] | bf.pMem[i]);

    for (int i = min(MemLen, bf.MemLen); i < max(MemLen, bf.MemLen); i++) {
        if (MemLen > bf.MemLen) Result.pMem[i] = pMem[i];
        else Result.pMem[i] = bf.pMem[i];
    }
    return Result;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    TBitField Result(max(BitLen, bf.BitLen));
    for (int i = 0; i < min(MemLen, bf.MemLen); i++) Result.pMem[i] = pMem[i] & bf.pMem[i];
    //остальные pMem'ы равны нулю
    return Result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField Result(*this);
    for (int i = 0; i < MemLen - 1; i++) {
        Result.pMem[i] = ~pMem[i];
    }
    
    for (int i = ((MemLen-1)*sizeof(TELEM)*8); i < GetLength(); i++) {
        if (GetBit(i) == 0) Result.SetBit(i);
        else Result.ClrBit(i);
    }

    return Result;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    string InputData;
    istr >> InputData;

    for (int i = 0; i < bf.MemLen; i++) bf.pMem[i] = 0;

    for (int i = 0; i < bf.GetLength(); i++) {
        if (InputData[i] == '1') bf.SetBit(bf.GetLength() - i);
    }

    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = bf.BitLen - 1; i >= 0; i--) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}