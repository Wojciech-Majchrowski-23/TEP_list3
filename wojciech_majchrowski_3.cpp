#include <iostream>
#include <string>
#include "CTree.h"
#include "CMySmartPointer.h"

int main()
{
    CTree tree1;
    CError err;
    tree1.vBuildTree("+ - * - 5 1 3 7 2", err);
    CTree tree2;

    std::cout << "Przed przeniesieniem:" << std::endl;
    std::cout << "tree1: " << tree1.sGetFormulaString() << std::endl;
    std::cout << "tree2: " << tree2.sGetFormulaString() + "\n" << std::endl;

    tree2 = std::move(tree1);

    std::cout << "Po przeniesieniu:" << std::endl;
    std::cout << "tree1: " << tree1.sGetFormulaString() << std::endl;
    std::cout << "tree2: " << tree2.sGetFormulaString() + "\n" << std::endl;

    CTree tree3;
    std::cout << "Przed kopiowaniem:" << std::endl;
    std::cout << "tree1: " << tree1.sGetFormulaString() << std::endl;
    std::cout << "tree2: " << tree2.sGetFormulaString() + "\n" << std::endl;

    tree3 = tree2;
    std::cout << "Pp kopiowaniu:" << std::endl;
    std::cout << "tree2: " << tree2.sGetFormulaString() << std::endl;
    std::cout << "tree3: " << tree3.sGetFormulaString() + "\n" << std::endl;

    
    std::cout << "=== Inteligentny wskaznik typu CTree ===" << std::endl;
    CMySmartPointer<CTree> ptr1(new CTree());

    CError cErr;
    (*ptr1).vBuildTree("+ 1 2", cErr);

    std::cout << "Drzewo 1: " << ptr1->sGetFormulaString() << std::endl;
    CMySmartPointer<CTree> ptr2 = ptr1;

    std::cout << "\nKopiowanie inteligentnego wskaznika ptr2 = ptr1" << std::endl;

    std::cout << "Licznik ptr1:  " <<ptr1.iGetCounter() << std::endl;
    std::cout << "Licznik ptr2:  " << ptr2.iGetCounter() << std::endl;

    std::cout << "Drzewo 2: " << ptr2->sGetFormulaString() << std::endl;

    (*ptr2).vBuildTree("* 5 5", cErr);

    std::cout << "\n--- Po zmianie w ptr2 ---" << std::endl;
    std::cout << "Drzewo 1: " << (*ptr1).sGetFormulaString() << std::endl;
    std::cout << "Drzewo 2: " << (*ptr2).sGetFormulaString() << std::endl;

    return 0;
}