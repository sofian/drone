#ifndef __ABSTRACTTYPE_INCLUDED
#define __ABSTRACTTYPE_INCLUDED


#include <qcolor.h>
#include <string>
#include <vector>

class AbstractType
{
public:
  AbstractType() {}
  virtual ~AbstractType() {}

  virtual std::string name() const = 0;
  virtual QColor color() const = 0;

  int nSubTypes() const { return _elements.size();}
  AbstractType* getSubType(int i) { return _elements[i];}
  AbstractType* addSubType(AbstractType *elem)
  {
    _elements.push_back(elem);
    return elem;
  }

protected:
  std::vector<AbstractType*> _elements;
};

#endif //__ABSTRACTTYPE_INCLUDED

/* template <class T>                                  */
/* class GenericType: public AbstractType              */
/* {                                                   */
/* public:                                             */
/*   GenericType() {}                                  */
/*                                                     */
/*   std::string name() const { return "GenericType";} */
/*   QColor color() const { return QColor(0,0,0);}     */
/*                                                     */
/*   T& getVal() const { return _val;}                 */
/*   void setVal() { _val = val;}                      */
/*                                                     */
/* protected:                                          */
/*   T _val;                                           */
/* };                                                  */
/*                                                     */
/* template<class T>                                   */
/* class ArrayType: public AbstractType                */
/* {                                                   */
/* public:                                             */
/*   ArrayType(int size = 0) : _array(0)               */
/*   {                                                 */
/*     resize(size);                                   */
/*   }                                                 */
/*                                                     */
/*   virtual ~ArrayType()                              */
/*   {                                                 */
/*     if (_array)                                     */
/*       free(_array);                                 */
/*   }                                                 */
/*                                                     */
/*   std::string name() const { return "ArrayType";}   */
/*   QColor color() const { return QColor(10,10,20);}  */
/*                                                     */
/*   void resize(int newSize)                          */
/*   {                                                 */
/*     _size = newSize;                                */
/*     _array = (T*) realloc(_array, _size*sizeof(T)); */
/*   }                                                 */
/*                                                     */
/*   int nElements() const { return _size;}            */
/*   T getElement(int i) { return _elements[i];}       */
/*   T setElement(int i, T elem)                       */
/*   {                                                 */
/*     _array[i] = elem;                               */
/*     return elem;                                    */
/*   }                                                 */
/*                                                     */
/* protected:                                          */
/*   T *_array;                                        */
/*   int _size;                                        */
/* };                                                  */
/*                                                     */

