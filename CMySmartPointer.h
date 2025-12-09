#pragma once

class CRefCounter
{
public:
    CRefCounter() { i_count = 0; }
    int iAdd() { return(++i_count); }
    int iDec() { return(--i_count); }
    int iGet() { return(i_count); }
private:
    int i_count;
};

template <typename T>
class CMySmartPointer
{
private:
    CRefCounter* pc_counter;
    T* pc_pointer;

public:
    CMySmartPointer(T* pcPointer)
    {
        pc_pointer = pcPointer;
        pc_counter = new CRefCounter();
        pc_counter->iAdd();
    }

    CMySmartPointer(const CMySmartPointer& pcOther)
    {
        pc_pointer = pcOther.pc_pointer;
        pc_counter = pcOther.pc_counter;
        pc_counter->iAdd();
    }

    ~CMySmartPointer()
    {
        if (pc_counter->iDec() == 0)
        {
            delete pc_pointer;
            delete pc_counter;
        }
    }

    CMySmartPointer& operator=(const CMySmartPointer& pcOther)
    {
        if (this == &pcOther)
        {
            return *this;
        }

        if (pc_counter->iDec() == 0)
        {
            delete pc_pointer;
            delete pc_counter;
        }

        pc_pointer = pcOther.pc_pointer;
        pc_counter = pcOther.pc_counter;
        pc_counter->iAdd();

        return *this;
    }

    T& operator*() { return(*pc_pointer); }
    T* operator->() { return(pc_pointer); }

    int iGetCounter()
    {
        if (pc_counter != NULL) return pc_counter->iGet();
        return 0;
    }
};
