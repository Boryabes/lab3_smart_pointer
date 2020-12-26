// Copyright 2020 Your Name <your_email>
#include "atomic"
#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
template <typename T> //делаем шардптр шаблонным, чтобы мы могли подавать на вход разные типы данных (Т - что угодно)
class SharedPtr {
 private: //в прайвит секции хранится указатель,оболчку для которого мы строим и счетчик ссылок на объект
  T* pointerToObject; //указатель обобщенного типа Т
  std::atomic_uint* counter; //атомарный тип данных для многопоточности(операция либо завершено либо нет)

 public:
  SharedPtr() : pointerToObject{nullptr}, counter(nullptr){}; //конструктор инициализирует два поля нулптрами
  explicit SharedPtr(T* ptr) //передаю указатель на какую-либо переменную
      : pointerToObject{ptr}, counter(new std::atomic_uint) { //:список инициализации
    *counter = 1; //
  };
  SharedPtr(const SharedPtr& r) //конструктор копирования, передаем элвэлью ссылку, нужен для копирования данных об одном объекте в другой
      : pointerToObject(r.pointerToObject), counter(r.counter) { //копирую значение из одного в другой
    ++(*counter); //инкрементирую каунтер, скобки чтобы сначала разыменовать а потом инкрементировать
  };
  SharedPtr(SharedPtr&& r) //передаю рвэлью ссылку,конструктор перемещения
      : pointerToObject(std::move(r.pointerToObject)), //муву передаю лвэлью она преобразует в рвэлью
        counter(std::move(r.counter)) {
    ++(*counter);
  };
  ~SharedPtr() { //деструктор,с помощью него очищаем память на которую указывает наш указатель
    if (*counter == 1) { //если ссылка одна то удаляем каунтер
      delete counter;
      counter = nullptr;
      pointerToObject = nullptr;
    } else {
      --(*counter); //декрментирую каунтер, тк. выхожу из области видимости то одна ссылка удаляется
    }
  };
  auto operator=(const SharedPtr& r) -> SharedPtr& { //трейлинг-способ объявлени функции, тип который после стрелки указывает авто какой тип
    this->reset(); //через стрелку обращаемся к методу ресет
    this->pointerToObject = r.pointerToObject; //копирование из р в объекст который стоит слева от равно
    this->counter = r.counter;
    if (counter != nullptr) {
      ++(*counter);
    }
    return *this; //разменование
  };
  auto operator=(SharedPtr&& r) -> SharedPtr& { //передаем эрвэлью ссылку
    this->reset();
    this->pointerToObject = r.pointerToObject;
    this->counter = r.counter;
    if (counter != nullptr) {
      ++(*counter);
    }
    return *this;
  };

  operator bool() const { return pointerToObject; }; //преобразование в булл(перегружаю оператор булл)

  auto operator*() const -> T& { return *pointerToObject; }; //разыменование указателя,возвращаю разыменованый указатель
  auto operator->() const -> T* { return pointerToObject; }; //возвращаю указатель

  auto get() -> T* { return pointerToObject; }; //возвращаю указатель, если каунтер не равен нулптр если кол-во ссылок не 1 то уменьшаю каунтер
  void reset() {                                 //а если равно 1 то удаляю поля поинтер и каунтер присваиваю нулптр
    if (counter != nullptr) {
      if (*counter != 1) {
        --(*counter);
      } else {
        delete pointerToObject;
        delete counter;
      }
      pointerToObject = nullptr;
      counter = nullptr;
    }
  };
  void reset(T* ptr) {  //передаю в аргумент указатель на объект и если каунтер не нулптр то -1
    if (counter != nullptr) {
      --(*counter);
    }
    else {counter = new std::atomic_uint; // если нулптр то выделяем память для нового,присваиваем 1, поинтеру указатель из аргумента
    *counter = 1;
    pointerToObject = ptr;}
  };
  void swap(SharedPtr& r) { //меняю местами данные в двух указателях
    SharedPtr<T> temp(r);
    r = *this;
    *this = temp;
  };

  auto use_count() const -> std::size_t { return *counter; }; //возвращаю количество ссылок на объект
};
#endif  // INCLUDE_HEADER_HPP_
