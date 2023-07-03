#pragma once

class Kind
{
public:
    template <typename T>
    static int ID()
    {
        static int id = ++_id;
        return id;
    }

private:
    static int _id;
};
