#ifndef RECORDSLOT_H_INCLUDED
#define RECORDSLOT_H_INCLUDED

#include"Slot.h"

class RecordSlot: public Slot
{
protected:
    RecordInfo* ri;

public:
    RecordSlot(BufType cache, RecordInfo* ri);

    bool checkNull(std::map<int, bool>& nulls);

    void write(std::vector<void*>& data);

    void read(std::vector<void*>& data);

    bool check(SearchInfo& si);

    bool checkValueNotEqual(std::map<int, std::vector<void*> >& info);

    bool checkValue(std::map<int, void*>& info, int op);

    bool checkFields(std::map<int, std::vector<int> >& info, int op);

    void update(UpdateInfo& ui);

    static void copy(RecordSlot* src, RecordSlot* dest, int length);

    void print();
};

#endif // RECORDSLOT_H_INCLUDED
