#include "val.h"
#include <cmath>

Value Value::operator*(const Value& other) const {
    if (IsInt() && other.IsInt()) {
        return Value(GetInt() * other.GetInt());
    } else if (IsReal() && other.IsReal()) {
        return Value(GetReal() * other.GetReal());
    } else if (IsInt() && other.IsReal()) {
        return Value(GetInt() * other.GetReal());
    } else if (IsReal() && other.IsInt()) {
        return Value(GetReal() * other.GetInt());
    // }else if (IsString()&&other.GetInt()){
        
    // return Value(GetString() * other.GetInt());

        
    }else {
        // Handle error: invalid operation
        return Value();
    }
}

Value Value::operator+(const Value& other) const {
    if (IsInt() && other.IsInt()) {
        return Value(GetInt() + other.GetInt());
    } else if (IsReal() && other.IsReal()) {
        return Value(GetReal() + other.GetReal());
    } else if (IsInt() && other.IsReal()) {
        return Value(GetInt() + other.GetReal());
    } else if (IsReal() && other.IsInt()) {
        return Value(GetReal() + other.GetInt());
    }else {
        // Handle error: invalid operation
        return Value();
    }
}

Value Value::operator-(const Value& other) const {
    if (IsInt() && other.IsInt()) {
        return Value(GetInt() - other.GetInt());
    } else if (IsReal() && other.IsReal()) {
        return Value(GetReal() - other.GetReal());
    } else if (IsInt() && other.IsReal()) {
        return Value(GetInt() - other.GetReal());
    } else if (IsReal() && other.IsInt()) {
        return Value(GetReal() - other.GetInt());
    }else {
        // Handle error: invalid operation
        return Value();
    }
}

Value Value::operator/(const Value& other) const {
    if (IsInt() && other.IsInt()) {
        return Value(GetInt() / other.GetInt());
    } else if (IsReal() && other.IsReal()) {
        return Value(GetReal() / other.GetReal());
    } else if (IsInt() && other.IsReal()) {
        return Value(GetInt() / other.GetReal());
    } else if (IsReal() && other.IsInt()) {
        return Value(GetReal() / other.GetInt());
    }else {
        // Handle error: invalid operation
        return Value();
    }
}



Value Value::operator<(const Value& other) const {
    if (IsInt() && other.IsInt()) {
        return Value(GetInt() < other.GetInt());
    } else if (IsReal() && other.IsReal()) {
        return Value(GetReal() < other.GetReal());
    }else if (IsInt()&&other.IsReal()){
        return Value(GetInt() < other.GetReal());
    }else if (IsReal()&&other.IsInt()){
        return Value(GetReal() < other.GetInt());
    }else if (IsString() && other.IsString()) {
        return Value();
    } else {
        // Handle error: invalid operation
    return Value();
    }
}

Value Value::operator>(const Value& other) const {
    if (IsInt() && other.IsInt()) {
        return Value(GetInt() > other.GetInt());
    } else if (IsReal() && other.IsReal()) {
        return Value(GetReal() > other.GetReal());
    }else if (IsInt()&&other.IsReal()){
        return Value(GetInt() > other.GetReal());
    }else if (IsReal()&&other.IsInt()){
        return Value(GetReal() > other.GetInt());
    }else if (IsString() && other.IsString()) {
        return Value();
    } else {
        // Handle error: invalid operation
    return Value();
    }
}

Value Value::Catenate(const Value& other) const {
    if (IsString() && other.IsString()) {
        return Value(GetString() + other.GetString());
    } else {
        // Handle error: invalid operation
        return Value();
    }
}


Value Value::operator==(const Value& op) const {
    if(IsInt()&&op.IsInt()){
         return Value(GetInt() == op.GetInt());
    }else if(IsReal()&& op.IsReal()){
         return Value(GetReal() == op.GetReal());
    }else if(IsInt() && op.IsReal()){
        return Value(GetInt()==op.GetReal());
    }else if(IsReal() && op.IsInt()){
        return Value(GetReal()==op.GetInt());
    }else{

        return Value(false);
    }
    
}



Value Value::Power(const Value& other) const {
    if (IsReal() && other.IsReal()) {
        return Value(pow(GetReal(), other.GetReal()));
    }if (IsInt() && other.IsInt()) {
        return Value(pow(GetInt(), other.GetInt()));
    } if (IsInt() && other.IsReal()) {
        return Value(pow(GetInt(), other.GetReal()));
    } if (IsReal() && other.IsInt()) {
        return Value(pow(GetReal(), other.GetInt()));
    }  else {
        // Handle error: invalid operation
        return Value();
    }

}
