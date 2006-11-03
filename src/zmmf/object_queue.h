/*MT*
    
    MediaTomb - http://www.mediatomb.org/
    
    object_queue.h - this file is part of MediaTomb.
    
    Copyright (C) 2005 Gena Batyan <bgeradz@mediatomb.org>,
                       Sergey Bostandzhyan <jin@mediatomb.org>
    Copyright (C) 2006 Gena Batyan <bgeradz@mediatomb.org>,
                       Sergey Bostandzhyan <jin@mediatomb.org>,
                       Leonhard Wimmer <leo@mediatomb.org>
    
    MediaTomb is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    MediaTomb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with MediaTomb; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
    
    $Id$
*/

/// \file object_queue.h

#ifndef __ZMMF_OBJECT_QUEUE_H__
#define __ZMMF_OBJECT_QUEUE_H__

#include "zmm/zmm.h"
#include "base_queue.h"

namespace zmm
{
    /// \brief a simple stack for a base type. NOT thread safe!
    template <class T>
    class ObjectQueue : public BaseQueue<Object *>
    {
    public:
        ObjectQueue(int initialCapacity) : BaseQueue<Object *>(initialCapacity, NULL)
        {
        }
        
        ~ObjectQueue()
        {
            Object *obj;
            while(NULL != (obj = BaseQueue<Object *>::dequeue()))
            {
                obj->release();
            }
        }
        
        inline void enqueue(Ref<T> element)
        {
            Object *obj = element.getPtr();
            obj->retain();
            BaseQueue<Object *>::enqueue(obj);
        }
        
        inline Ref<T> dequeue()
        {
            Object *obj = BaseQueue<Object *>::dequeue();
            if (obj == NULL)
                return nil;
            Ref<T> ret((T *)obj);
            obj->release();
            return ret;
        }
    };
}

#endif // __ZMMF_OBJECT_QUEUE_H__

