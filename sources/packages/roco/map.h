
#ifndef _map_
#define _map_

#include "list.h"

template<class From, class To> class Map;
template<class From, class To> class Mapiter;


template<class From, class To> class Link {
  friend class Map<From, To>;
  friend class Mapiter<From, To>;
private:
  From from;
  To to;
  Link* next;
public:
  Link(const From& f, const To& t) : from(f), to(t) { next = 0; };
  ~Link() { delete next; };
};


template<class From, class To> class Map {
  friend class Mapiter<From, To>;
private:  
  Link<From, To>* map;
  From defFrom;
  To defTo;
  int size;
  
  void init() { size = 0; map = 0; };
public:
  Map() { init(); };
  Map(const Map&); 
  Map(const From& f, const To& t) : defFrom(f), defTo(t) { init(); };
  ~Map() { delete map; };
  Map& operator=(const Map&);
  To& operator[](const From&);
  int defined(const From&);
  void clear() { delete map; init(); };
  int length() { return size; };
  void remove(const From& f);
  List<From> keylist();
};


template<class From, class To> Map<From,To>::Map(const Map& m)
  : defFrom(m.defFrom), defTo(m.defTo)
{
  init();
  for (Link<From, To>* p = m.map; p; p = p->next)
    operator[](p->from) = p->to; 
}


template<class From, class To> Map<From,To>& Map<From,To>::operator=(const Map& m)
{
  delete map;
  defFrom = m.defFrom;
  defTo = m.defTo;
  size = 0;
  for (Link<From, To>* p = m.map; p; p = p->next)
    operator[](p->from) = p->to; 
  
  return(*this);  
}


template<class From, class To> To& Map<From,To>::operator[](const From& f)
{
  if (map == 0) {
    map = new Link<From, To>(f, defTo);
    size++;
    return map->to;
  }
  Link<From, To>* p = map;
  while (true) {
    if (p->from == f) {
      return p->to;
    }
    Link<From, To>* n = p->next;
    if (n == 0) {
      size++;
      n = new Link<From, To>(f, defTo);
      p->next = n;
      return n->to;
    }
    p = n;
  }
}


template<class From, class To> int Map<From,To>::defined(const From& f)
{
  for (Link<From, To>* p = map; p; p = p->next)
    if (p->from == f)
      return(true);

  return(false);
}


template<class From, class To> void Map<From,To>::remove(const From& f)
{
  Link<From, To>* pr, *p = map;
  if (p->from == f) {
    map = p->next;
    p->next = 0;
    delete p;
    return;
  }
  for (pr = p, p = p->next; p; pr = p, p = p->next)
    if (p->from == f) {
      pr->next = p->next;
      p->next = 0;
      delete p;
      return;
    }
}


template<class From, class To> List<From> Map<From,To>::keylist()
{
  List<From> kl;
  Link<From, To>* p;
  
  for (p = map; p; p = p->next)
    kl.append(p->from);
  
  return kl;
}


template<class From, class To> class Mapiter {
  friend class Map<From, To>;
  
  Map<From, To>* m;
  Link<From, To>* l;
  int p;
  
  Mapiter(Map<From, To>* mm, Link<From, To>* ll) { m = m; l = ll; };
public:
  Mapiter()           { m = 0; l = 0; };
  Mapiter(Map<From, To>& mm) { m = &mm; l = m->map; p = 0; };
  operator void*()     { return l; };
  const From& key() { if (l) return l->from; else return m->defFrom; };
  const To& value() { if (l) return l->to; else return m->defTo; };
  const To& value(const To& t)
    { if (l) { l->to = t; return l->to; } else return m->defTo; };
  const int pos() { return p; };
  void operator++(int) { if (l) { l = l->next; p++; } };
  void remove();
};

template<class From, class To> void Mapiter<From, To>::remove()
{
  Link<From, To> *help; // *before;
  
  if (l != NULL) {
    if (l == m->map) {
      help = m->map;
      m->map = m->map->next;
      l = m->map;
      help->next = 0;
      delete help;
      m->size--;
    } else {
      for (help = m->map; help->next != l; help = help->next) 
        ;
      help->next = l->next;
      l->next = 0;
      delete l;
      m->size--;
      l = help->next;
    }
  }
}


#endif
