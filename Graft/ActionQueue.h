/*  -*-c++-*-
 *  Copyright (C) 2015 Thomas Hogarth <tom@hogbox.com>
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/

#ifndef GRAFT_ACTIONQUEUE
#define GRAFT_ACTIONQUEUE 1

#include <Graft/Action.h>

namespace graft {

class GRAFT_EXPORT ActionQueue : public osg::Object
{
public:
    ActionQueue();

    ActionQueue(const ActionQueue&,const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);

    META_Object(graft, ActionQueue)

    virtual bool process(ActionData* aData);

    // add list to the queue
    virtual bool addAction(Action* anAction);

    // inter an operation at a specific index
    virtual bool insertAction(unsigned int index, Action *anAction);

    // remove occurance of specific operation
    virtual bool removeAction(Action *anAction);

    // remove range of operations from pos to pos+numToRemove
    inline bool removeAction(unsigned int pos, unsigned int numChildrenToRemove=1)
    {
        if (pos<_children.size()) return removeActions(pos,numChildrenToRemove);
        else return false;
    }

    // remove range of operations from pos to pos+numToRemove
    virtual bool removeActions(unsigned int pos,unsigned int numChildrenToRemove);

    // replace occurance of origAction with newAction
    bool replaceAction( Action *origAction, Action* newAction);

    // return the number of operations
    virtual unsigned int getNumActions() const;

    // set operation at index to anAction
    virtual bool setAction(unsigned  int i, Action* anAction);

    // retrun operation at index
    inline Action* getAction(unsigned int i) { return _children[i].get(); }

    // return operation at position i
    inline const Action* getAction(unsigned int i) const { return _children[i].get(); }

    // return true if operation is contained within queue
    inline bool containsAction( const Action* anAction ) const
    {

        for (ActionList::const_iterator itr=_children.begin();
             itr!=_children.end();
             ++itr)
        {
            if (itr->get()==anAction) return true;
        }
        return false;
    }

    // get index of operation, return a value between 0 and _children.size()-1 if found,
    // if not found then return _children.size()
    inline unsigned int getActionIndex(const Action* anAction) const
    {
        for (unsigned int childNum=0;childNum<_children.size();++childNum)
        {
            if (_children[childNum]==anAction) return childNum;
        }
        return static_cast<unsigned int>(_children.size()); // node not found.
    }

protected:

    virtual ~ActionQueue();

    virtual void operationRemoved(unsigned int /*pos*/, unsigned int /*numChildrenToRemove*/) {}
    virtual void operationInserted(unsigned int /*pos*/) {}

    ActionList _children;
};

} // end graft namespace

#endif // GRAFT_ACTIONQUEUE
