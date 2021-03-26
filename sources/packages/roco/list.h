
#ifndef _list_
#define _list_

#ifndef NULL
#define NULL (void *)0
#endif

template<class Typ> class List;
template<class Typ> class Listiter;


template<class Typ> class Element {
  friend class List<Typ>;
  friend class Listiter<Typ>;
private:
  Typ content;
  Element *next;
};


template<class Typ> class List {
  friend class Listiter<Typ>;
  Element<Typ> *first;

  Element<Typ> *last;
  Typ defElem;
  int size;
public:
  List();
  List(const List&);
  ~List();
  List& operator=(const List&);
  void append(Typ c);
  Typ& get();
  void clear();
  int member(const Typ& m);
  int length() { return size; };
};


template<class Typ> List<Typ>::List()
{
  last = NULL;
  first = NULL;
  size = 0;
}

template<class Typ> List<Typ>::List(const List& l)
{
  Element<Typ> *help;
  
  last = NULL;
  first = NULL;
  
  for(help = l.first; help != NULL; help = help->next)
    append(help->content);
}

template<class Typ> List<Typ>::~List()
{
  clear();
}

template<class Typ> void List<Typ>::clear()
{
  Element<Typ> *help;
  
  while (first != NULL) {
    help = first;
    first = first->next;
    delete help;
  }
  first = NULL;
  last = NULL;
  size = 0;
}

template<class Typ> List<Typ>& List<Typ>::operator=(const List& l)
{
  Element<Typ> *help;

  while (first != NULL) {
    help = first;
    first = first->next;
    delete help;
  }    
  last = NULL;
  first = NULL;
  
  for(help = l.first; help != NULL; help = help->next)
    append(help->content);
    
  return(*this);
}


template<class Typ> void List<Typ>::append(Typ c)
{
  Element<Typ> *newline;
  
  newline = new Element<Typ>;
  newline->content = c;
  newline->next = NULL;
  if (last != NULL) {
    last->next = newline;
    last = last->next;
  } else {
    first = newline;
    last = first;
  }
  size++;
}

#ifdef OBSOLET
/* diese Methode braucht wohl niemand, aber sie stört den bcc und seit 7/06 auch den g++ wj  */
template<class Typ> Typ& List<Typ>::get()
{
  if (act != NULL)
    return(act->content);
  else
    return(defElem);
}
#endif /* OBSOLET */

template<class Typ> int List<Typ>::member(const Typ& m)
{
  Element<Typ> *help;
  
  for (help = first; help != NULL; help = help->next)
    if (help->content == m)
      return(true);
  
  return(false);
}


template<class Typ> class Listiter {
  friend class List<Typ>;
  
  List<Typ>* l;
  Element<Typ>* e;
  
  Listiter(List<Typ>* ll, Element<Typ>* ee) { l = ll; e = ee; };
public:
  Listiter()           { l = 0; e = 0; };
  Listiter(List<Typ>& ll) { l = &ll; e = l->first; };
  Listiter<Typ>& operator=(List<Typ>& ll) { l = &ll; e = l->first; return *this; };
  operator void*()     { return e; };
  const Typ& value() { if (e) return e->content; else return l->defElem; };
  void operator++(int) { if (e) e = e->next; };
  void remove();
};

template<class Typ> void Listiter<Typ>::remove()
{
  Element<Typ> *help, *before;
  
  if (e != NULL) {
    if (e == l->first) {
      help = l->first;
      l->first = l->first->next;
      e = l->first;
      if (l->first == NULL)
        l->last = NULL;
      delete help;
      l->size--;
    } else {
      for (help = l->first; help->next != e; help = help->next) 
        ;
      if (e == l->last)
        l->last = help;
      help->next = e->next;
      delete e;
      l->size--;
      e = help->next;
    }
  }
}

#endif
