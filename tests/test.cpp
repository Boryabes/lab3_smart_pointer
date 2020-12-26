// Copyright 2020 Your Name <your_email>
#include <gtest/gtest.h>

#include "smart_pointer.hpp"
TEST(Example, EmptyTest) { EXPECT_TRUE(true); }


TEST(SharedPtr, constructorWithArgument) { //если конструктор без аргументов
int a = 8;
SharedPtr<int> var(&a); //создаю вар - умный указатель на переменную а
ASSERT_EQ(*var, a); //разыменовываю вар и возврашаем значение лежащее по указателю(оно должно быть равно 8)
}

TEST(SharedPtr, constructorWithLValLink) { //передаю лвэлью ссылку
int a = 8;
SharedPtr var(&a); //создаю объекст класса шарптр следящий за а
SharedPtr l(var); //используем констр копирования, копируем данные указателя из вар в л
ASSERT_EQ(var.use_count(), 2); //проверяем что количество ссылок на вар равно двум (на нее указывают и л и вар)
}

TEST(SharedPtr, constructorWithRValLink) {
int a = 9;
SharedPtr var(&a);
SharedPtr r(std::move(var)); //использую мув чтобы рвэлью передать
ASSERT_EQ(var.use_count(), 2);
}

TEST(SharedPtr, isMoveConstr){
ASSERT_EQ(std::is_move_constructible<SharedPtr<int>>::value,true);
}

TEST(SharedPtr, isMoveAssign){
ASSERT_EQ(std::is_move_assignable<SharedPtr<int>>::value,true);
}

TEST(SharedPtr, operatorRavnoWithLValLink){ //то же самое что и констр копирования только через равно
int a = 32;
SharedPtr var(&a);
SharedPtr var1 = var;
ASSERT_EQ(var.use_count(),2);
}


TEST(ShredPtr, operatorRavnoWithRValLink){ //то же самое что и прошлом только с лвэлью
int a = 12;
SharedPtr var(&a);
SharedPtr var1 = std::move(var);
ASSERT_EQ(var.use_count(),2);
}




TEST(SharedPtr, funcSwap){
int a = 1;
int b = 2;
SharedPtr<int> A(&a);
SharedPtr<int> B(&b);
B.swap(A); //к бэ применяю метод свап
ASSERT_EQ(*B, 1);
}

