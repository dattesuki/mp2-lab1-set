// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

using namespace std;

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0) throw std::underflow_error("underflow_error"); //обработка ошибки, если кол-во эл-ов не положительное
    BitLen = len - 1; //количество элементов
    MemLen = (len / (sizeof(TELEM) * 8)) + 1;//к-во эл-тов Мем для представления бит.поля = количество элементов / количество элементов в TELEM (+1 для хвоста)
    pMem = new TELEM[MemLen]{};
    if (pMem == nullptr) throw domain_error("domain_error");
    for (int i = 0; i < (MemLen); i++) {
        pMem[i] = 0;
    }
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
    if (n > BitLen) throw out_of_range("out_of_range"); //обработка ошибки, если происходит попытка доступа к элементу за границей заданного диапазона
    int NumberMem = MemLen - (n / (sizeof(TELEM) * 8)) - 1;
    int NumberElement = ((sizeof(TELEM) * 8) - 1) - (n % (sizeof(TELEM) * 8)); //фиктивное
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
    return (BitLen + 1); //0 учитывается, поэтому +1
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
    return (pMem[NumberMem] & (1 << NumberElement));
}

// воспомогательный метод
TBitField TBitField::DifferentLength(const TBitField& bf1, const TBitField& bf2) const {
    TBitField Result(bf1.GetLength());//считаем, что длина bf1 > длина bf2
    for (int i = 0; i < bf2.MemLen; i++) {
        Result.pMem[Result.MemLen - 1 - i] = bf2.pMem[bf2.MemLen - i - 1];
    }
    return Result;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf) {
        delete[] pMem;
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        if (pMem == nullptr) throw domain_error("domain_error");
        for (int i = 0; i < (MemLen); i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    //приведение двух полей к одной длине
    TBitField bf1(0), bf2(0);
    if (BitLen > bf.BitLen) {
        bf2 = DifferentLength(*this, bf);
        bf1 = *this;
    }
    else {
        bf2 = DifferentLength(bf, *this);
        bf1 = bf;
    }

    //обработка хвоста(на случай, если хвост разный, чтобы он не повлиял)
    unsigned int StartNumberElement = (sizeof(TELEM) * 8) - (bf1.GetLength() % (sizeof(TELEM) * 8)) - 1;
    for (int shift = StartNumberElement; shift < (sizeof(TELEM) * 8); shift++) if (((bf1.pMem[0] >> shift) & 1) != ((bf2.pMem[0] >> shift) & 1)) return 0;

    //сравнение остальных элементов pMem
    for (int i = 1; i < bf1.MemLen; i++) {
        if (bf1.pMem[i] != bf2.pMem[i]) return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    /*    //приведение двух полей к одной длине
    TBitField bf1(0), bf2(0);
    if (BitLen > bf.BitLen) {
        bf2 = DifferentLength(*this, bf); //меньшее поле приводится к длине большего
        bf1 = *this;
    }
    else {
        bf2 = DifferentLength(bf, *this);
        bf1 = bf;
    }

    //обработка хвоста
    unsigned int StartNumberElement = (sizeof(TELEM) * 8) - (bf1.GetLength() % (sizeof(TELEM) * 8)) - 1;
    for (int shift = StartNumberElement; shift < (sizeof(TELEM) * 8); shift++) if (((bf1.pMem[0] >> shift) & 1) != ((bf2.pMem[0] >> shift) & 1)) return 1;

    //сравнение остальных pMem(за исключением нулевого)
    for (int i = 1; i < MemLen; i++) {
        if (bf1.pMem[i] != bf2.pMem[i]) return 1;
    }*/
    return !(*this==bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    //приведение двух полей к одной длине
    TBitField bf1(0), bf2(0);
    if (BitLen > bf.BitLen) {
        bf2 = DifferentLength(*this, bf);
        bf1 = *this;
    }
    else {
        bf2 = DifferentLength(bf, *this);
        bf1 = bf;
    }
    TBitField Result(bf1.GetLength());
    for (int i = 0; i < bf1.MemLen; i++) Result.pMem[i] = bf1.pMem[i] | bf2.pMem[i];
    return Result;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    //приведение двух полей к одной длине
    TBitField bf1(0), bf2(0);
    if (BitLen > bf.BitLen) {
        bf2 = DifferentLength(*this, bf);
        bf1 = *this;
    }
    else {
        bf2 = DifferentLength(bf, *this);
        bf1 = bf;
    }
    TBitField Result(bf1.GetLength());
    for (int i = 0; i < bf1.MemLen; i++) Result.pMem[i] = bf1.pMem[i] & bf2.pMem[i];
    return Result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField Result(*this);

    for (int i = 0; i < Result.GetLength(); i++) {
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

    //зануляем исходное поле
    TBitField Temp(InputData.length() - 1);
    bf = Temp;
    ~Temp;

    for (int i = 0; i < bf.GetLength(); i++) {
        if (InputData[i] == '1') bf.SetBit(bf.GetLength() - i);
    }

    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    //обработка хвоста
    unsigned int StartNumberElement = (sizeof(TELEM) * 8) - (bf.BitLen % (sizeof(TELEM) * 8)) - 1;
    for (int shift = StartNumberElement; shift < (sizeof(TELEM) * 8); shift++) ostr << ((bf.pMem[0] >> shift) & 1);

    if (bf.MemLen > 0) {
        for (int i = 1; i < bf.MemLen; i++) {
            for (int shift = 0; shift < (sizeof(TELEM) * 8); shift++) ostr << ((bf.pMem[i] >> shift) & 1);
        }
    }
    return ostr;
}
