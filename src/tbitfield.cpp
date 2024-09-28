// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

/*int  BitLen; // длина битового поля - макс. к-во битов
  TELEM *pMem; // память для представления битового поля
  int  MemLen; // к-во эл-тов Мем для представления бит.поля*/


#include "tbitfield.h"

using namespace std;

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0) throw std::underflow_error("underflow_error"); //обработка ошибки, если кол-во эл-ов не положительное
    BitLen = len; //количество элементов
    MemLen = (len / (sizeof(TELEM) * 8)) + 1;//к-во эл-тов Мем для представления бит.поля = количество элементов / количество элементов в TELEM
    pMem = new TELEM[MemLen]{};
    for (int i = 0; i < (MemLen); i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
        BitLen = bf.BitLen; //проверка на кол-во эл-ов уже выполнялась
        MemLen = BitLen / (sizeof(TELEM) * 8);
        pMem = new TELEM[MemLen];
        for (int i = 0; i < (MemLen); i++) {
            pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0) throw std::underflow_error("underflow_error"); //обработка ошибки, если элемент отрицательный
    if (n > BitLen) throw out_of_range("out_of_range"); //обработка ошибки, если происходит попытка доступа к элементу за границей заданного диапазона
    int NumberMem = MemLen - (n / (sizeof(TELEM) * 8)) - 1;
    int NumberElement = ((sizeof(TELEM) * 8) - 1) - (n % (sizeof(TELEM) * 8));
    return NumberMem;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0) throw std::underflow_error("underflow_error"); //обработка ошибки, если элемент отрицательный
    if (n > BitLen) throw out_of_range("out_of_range"); //обработка ошибки, если происходит попытка доступа к элементу за границей заданного диапазона
    int NumberMem = MemLen - (n / (sizeof(TELEM) * 8)) - 1;
    return pMem[NumberMem];
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0) throw std::underflow_error("underflow_error"); //обработка ошибки, если элемент отрицательный
    if (n > BitLen) throw out_of_range("out_of_range"); //обработка ошибки, если происходит попытка доступа к элементу за границей заданного диапазона
    int NumberMem = MemLen - (n / (sizeof(TELEM) * 8)) - 1;
    int NumberElement = ((sizeof(TELEM) * 8) - 1) - (n % (sizeof(TELEM) * 8));
    pMem[NumberMem] |= (1 << NumberElement);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0) throw std::underflow_error("underflow_error"); //обработка ошибки, если элемент отрицательный
    if (n > BitLen) throw out_of_range("out_of_range"); //обработка ошибки, если происходит попытка доступа к элементу за границей заданного диапазона
    int NumberMem = MemLen - (n / (sizeof(TELEM) * 8)) - 1;
    int NumberElement = ((sizeof(TELEM) * 8) - 1) - (n % (sizeof(TELEM) * 8));
    pMem[NumberMem] &= ~(1 << NumberElement);
}


int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0) throw std::underflow_error("underflow_error"); //обработка ошибки, если элемент отрицательный
    if (n > BitLen) throw out_of_range("out_of_range"); //обработка ошибки, если происходит попытка доступа к элементу за границей заданного диапазона
    int NumberMem = MemLen - (n / (sizeof(TELEM) * 8)) - 1;
    int NumberElement = ((sizeof(TELEM) * 8) - 1) - (n % (sizeof(TELEM) * 8));
    return (pMem[NumberMem]&(1 << NumberElement));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this != &bf) {
        BitLen = bf.BitLen; //проверка на кол-во эл-ов уже выполнялась
        MemLen = BitLen / (sizeof(TELEM) * 8);
        pMem = new TELEM[MemLen];
        for (int i = 0; i < (MemLen); i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return FAKE_INT;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    return FAKE_BITFIELD;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    return FAKE_BITFIELD;
}

TBitField TBitField::operator~(void) // отрицание
{
    return FAKE_BITFIELD;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    return ostr;
}
