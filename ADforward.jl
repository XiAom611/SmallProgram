# Design by Xiaoye Mo 2017-10-24
# import the Base which going to be used.
import Base.sin
import Base.cos
import Base.+
import Base.*

# define a type based on ADforward
# a = the origin function
# b = the derivative of the origin function
type ADV
    a
    b
end

# define "+" operation, when A and B are both ADV type
function +(A::ADV, B::ADV)
    return ADV(A.a+B.a, A.b+B.b);
end

# define "+" operation, when A is a Number but B is ADV type
function +(A::Number, B::ADV)
    return ADV(A+B.a, B.b);
end

# define "+" operation, when B is a Number but A is ADV type
function +(A::ADV, B::Number)
    return ADV(A.a+B, A.b);
end

# define "*" operation, when A and B are both ADV type
function *(A::ADV, B::ADV)
    return ADV(A.a*B.a, A.b*B.a+A.a*B.b);
end

# define "*" operation, when A is a Number but B is ADV type
function *(A::Number, B::ADV)
    return ADV(A*B.a, A*B.b);
end

# define "*" operation, when B is a Number but A is ADV type
function *(A::ADV, B::Number)
    return ADV(A.a*B, A.b*B);
end

# define "sin" operation
function sin(A::ADV)
    return ADV(sin(A.a), cos(A.a)*A.b);
end

# define "cos" operation
function cos(A::ADV)
    return ADV(cos(A.a), -sin(A.a)*A.b);
end

# to test if the function is just a Number.
function constant_function(f)
    typeof(f(ADV(1,1))) == ADV;
end

# ADforward function
function ADforward(f)
    # test if the function is just a Number
    if constant_function(f) == false
        # then the derivative of it is 0
        h(x) = 0
        return h;
    else
        # otherwise, b is the output
        g(x) = f(ADV(x,1)).b;
        return g;
    end
end