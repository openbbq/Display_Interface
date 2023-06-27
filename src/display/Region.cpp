
#include <Arduino.h>
#include "Region.h"

namespace display
{
    enum SameEdge
    {
        SAME_LEFT = 0x01,
        SAME_TOP = 0x02,
        SAME_RIGHT = 0x04,
        SAME_BOTTOM = 0x08,
    };

    class Checker
    {
    public:
        Checker(const Rect &existing, const Rect &proposed)
            : _redundant(existing.intersection(proposed))
        {
            _same = (proposed.left() == _redundant.left() ? SAME_LEFT : 0) |
                    (proposed.top() == _redundant.top() ? SAME_TOP : 0) |
                    (proposed.right() == _redundant.right() ? SAME_RIGHT : 0) |
                    (proposed.bottom() == _redundant.bottom() ? SAME_BOTTOM : 0);
            // Serial.printf(
            //     "Checker existing %s proposed %s same %d\n",
            //     existing.toString().c_str(),
            //     proposed.toString().c_str(),
            //     _same);
        }

        Rect _redundant;
        int _same;

        bool unrelated() const { return _redundant.empty(); }
        bool existingContained() const { return _same == 0; }
        bool proposedContained() const { return _same == (SAME_LEFT | SAME_TOP | SAME_RIGHT | SAME_BOTTOM); }

        void leftward(Rect &rc) const { rc = Rect(rc.left(), _redundant.top(), _redundant.left(), _redundant.bottom()); }
        void topward(Rect &rc) const { rc = Rect(_redundant.left(), rc.top(), _redundant.right(), _redundant.top()); }
        void rightward(Rect &rc) const { rc = Rect(_redundant.right(), _redundant.top(), rc.right(), _redundant.bottom()); }
        void bottomward(Rect &rc) const { rc = Rect(_redundant.left(), _redundant.bottom(), _redundant.right(), rc.bottom()); }

        Rect topwardWide(Rect &rc) const { return Rect(rc.left(), rc.top(), rc.right(), _redundant.top()); }
        Rect bottomwardWide(Rect &rc) const { return Rect(rc.left(), _redundant.bottom(), rc.right(), rc.bottom()); }
    };

    void Region::include(const Rect &rc)
    {
        log_v("region %s", rc.toString().c_str());

        std::vector<Rect> proposed;
        proposed.push_back(rc);

        int i = 0;
        for (int i_size = this->size(); i_size; --i_size) // = this->begin(); original && it != this->end(); --original)
        {
            auto it = begin().operator+(i);
            int j = 0;
            for (int j_size = proposed.size(); j_size != 0; --j_size)
            {
                Checker chk(*it, proposed[j]);
                Rect redundant = it->intersection(proposed[j]);

                if (chk.unrelated())
                {
                    // Serial.println("non-overlapping, keep proposed");
                    ++j;
                    continue;
                }
                if (chk.proposedContained())
                {
                    // Serial.println("proposed rect landed in existing, remove it and go");
                    proposed.erase(proposed.begin() + j);
                    continue;
                }
                if (chk.existingContained())
                {
                    // Serial.println("existing rect fully contained, empty so it's erased stop testing proposed on it");
                    *it = Rect();
                    break;
                }

                switch (chk._same)
                {
                // three side match - proposed rect is "in a side" of existing rect and gets clipped by it
                case SAME_TOP | SAME_RIGHT | SAME_BOTTOM: // not SAME_LEFT
                    chk.leftward(proposed[j]);
                    break;
                case SAME_LEFT | SAME_RIGHT | SAME_BOTTOM: // not SAME_TOP
                    chk.topward(proposed[j]);
                    break;
                case SAME_LEFT | SAME_TOP | SAME_BOTTOM: // not SAME_RIGHT
                    chk.rightward(proposed[j]);
                    break;
                case SAME_LEFT | SAME_TOP | SAME_RIGHT: // not SAME_BOTTOM
                    chk.bottomward(proposed[j]);
                    break;

                    // one side matches - existing rect is "in the side" of proposed and gets clipped in the other way
                case SAME_LEFT:
                    chk.leftward(*it);
                    break;
                case SAME_TOP:
                    chk.topward(*it);
                    break;
                case SAME_RIGHT:
                    chk.rightward(*it);
                    break;
                case SAME_BOTTOM:
                    chk.bottomward(*it);
                    break;

                // corner-over-corner match: proposal breaks into two parts
                case SAME_LEFT | SAME_TOP:
                    proposed.push_back(chk.bottomwardWide(proposed[j]));
                    chk.rightward(proposed[j]);
                    break;
                case SAME_LEFT | SAME_BOTTOM:
                    proposed.push_back(chk.topwardWide(proposed[j]));
                    chk.rightward(proposed[j]);
                    break;
                case SAME_RIGHT | SAME_TOP:
                    proposed.push_back(chk.bottomwardWide(proposed[j]));
                    chk.leftward(proposed[j]);
                    break;
                case SAME_RIGHT | SAME_BOTTOM:
                    proposed.push_back(chk.topwardWide(proposed[j]));
                    chk.leftward(proposed[j]);
                    break;

                    // proposed split in two vertically
                case SAME_LEFT | SAME_RIGHT:
                    proposed.push_back(chk.topwardWide(proposed[j]));
                    chk.bottomward(proposed[j]);
                    break;
                    // proposed split in two horizontally
                case SAME_TOP | SAME_BOTTOM:
                    Rect rightward = proposed[j];
                    chk.rightward(rightward);
                    proposed.push_back(rightward);
                    chk.leftward(proposed[j]);
                    break;
                }
                ++j;
            }

            if (it->empty())
            {
                erase(it);
            }
            else
            {
                ++i;
            }
        }

        for (const Rect &prop : proposed)
        {
            // Serial.printf("Region::include added %s\n", prop.toString().c_str());

            push_back(prop);
        }
    }

    void Region::exclude(const Rect &rc)
    {
        //log_v("region %s", rc.toString().c_str());

        int index = 0;
        for (int original = this->size(); original; --original) // = this->begin(); original && it != this->end(); --original)
        {
            auto it = begin().operator+(index);
            Rect removed = it->intersection(rc);
            if (!removed)
            {
                ++index;
                continue;
            }

            Rect at = *it;
            this->erase(it);

            if (at.top() != removed.top())
            {
                this->push_back(Rect(at.left(), at.top(), at.right(), removed.top()));
            }
            if (at.left() != removed.left())
            {
                this->push_back(Rect(at.left(), removed.top(), removed.left(), removed.bottom()));
            }
            if (at.right() != removed.right())
            {
                this->push_back(Rect(removed.right(), removed.top(), at.right(), removed.bottom()));
            }
            if (at.bottom() != removed.bottom())
            {
                this->push_back(Rect(at.left(), removed.bottom(), at.right(), at.bottom()));
            }
        }
    }
} // namespace display
