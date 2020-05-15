#ifndef __OG_TARGETS_SYMBOL_H__
#define __OG_TARGETS_SYMBOL_H__

#include <string>
#include <memory>
#include <cdk/types/basic_type.h>

namespace og {

  class symbol {
    int _qualifier;
    std::shared_ptr<cdk::basic_type> _type;
    std::string _name;
    bool _initialized;
    bool _function;
    long _value = 0; // hack!
  
    int _offset = 0; // 0 (zero) means global variable/function
    
  public:
    symbol(int qualifier, std::shared_ptr<cdk::basic_type> type, const std::string &name, bool initialized, bool function) :
       _qualifier(qualifier), _type(type), _name(name),  _initialized(initialized), _function(function){
    }

    virtual ~symbol() {
      // EMPTY
    }

    int qualifier() const {
      return _qualifier;
    }
    int qualifier(int q) {
      return _qualifier = q;
    }
    std::shared_ptr<cdk::basic_type> type() const {
      return _type;
    }
    bool is_typed(cdk::typename_type name) const {
      return _type->name() == name;
    }
    const std::string &name() const {
      return _name;
    }
    bool initialized() const {
      return _initialized;
    }
    bool isFunction() const {
      return _function;
    }
    bool isVariable() const {
      return !_function;
    }
    long value() const {
      return _value;
    }
    long value(long v) {
      return _value = v;
    }
    int offset() const {
      return _offset;
    }
    void set_offset(int offset) {
      _offset = offset;
    }
    bool global() const {
      return _offset == 0;
    }
  };

} // og

#endif
