/**
 *  ProtocolInGetIterator.h
 *
 *  This is an internal helper class that is used when iterating over a keys
 *
 *  Thus, when you do c++ things like "for (auto &iter : prot_in)", internally
 *  a ProtocolInGetIterator object is being used.
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

namespace RocksServer {

    /**
     *  Forward declarations
     */
    //class ProtocolInGetIteratorImpl;

    class ProtocolInGetIterator
    {
    public:

        typedef std::string::size_type size_type;

        /**
         *  Constructor
         *  @param  impl        Implementation iterator
         */
        ProtocolInGetIterator(const std::string &uri, size_type len,size_type startpos) :
            str(uri),
            strlen(len),
            lpos(startpos),
            rpos(str.find('&', lpos))
        {
            setCurrent();
        }

        // trivial constructor
        ProtocolInGetIterator() :
            str(""),
            strlen(0),
            lpos(npos),
            rpos(npos)
        {}

        ~ProtocolInGetIterator() {}

        /**
         *  Increment position (pre-increment)
         */
        ProtocolInGetIterator &operator++()
        {
            if(rpos == npos) {
                lpos = npos;
                return *this;
            }

            lpos = rpos+1;
            rpos = str.find('&', lpos);

            setCurrent();
            return *this;
        }
        
        /**
         *  Increment position (post-increment)
         */
        ProtocolInGetIterator operator++(int)
        {
            // make a copy
            ProtocolInGetIterator copy(*this);
            
            // increment current object
            ++(*this);
            
            // return the original
            return copy;
        }
        
        /**
         *  Compare with other iterator
         */
        bool operator!=(const ProtocolInGetIterator &that) const
        {
            return (lpos != that.lpos);
        }

        /**
         *  Derefecence, this returns a rocksdb::Slice with the current key
         */
        const rocksdb::Slice & operator*() const
        {
            return current;
        }
        
        /**
         *  Derefecence, this returns a rocksdb::Slice with the current key
         */
        const rocksdb::Slice * operator->() const
        {
            return &current;
        }

    private:

        /**
         *  Retrive current
         */
        void setCurrent()
        {
            current = (rpos == npos) ?
                rocksdb::Slice(str.data()+lpos, strlen-lpos) :
                rocksdb::Slice(str.data()+lpos, rpos-lpos);
        }

        const size_type &npos = std::string::npos;
        
        const std::string &str;

        const size_type strlen;

        // startpos
        size_type lpos;
        size_type rpos;

        rocksdb::Slice current;

    };

}

