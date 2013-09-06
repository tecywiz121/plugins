print("lua: Hello, World!")

function banana_math(a, b)
    print("lua: doing some math")
    return a*b
end

function banana(a, b)
    print("lua: Figuring out what to call")
    local name = "do_something"
    local do_something = get_function(name)

    if not do_something then
        name = "banana_math"
        do_something = get_function(name)
    end

    print("lua: calling " .. name)
    ret = do_something(a, b)
    print ("lua: " .. name .. " returned " .. ret)
    return ret
end

register_function("banana", "ff)i")
register_function("banana_math", "ff)i")
