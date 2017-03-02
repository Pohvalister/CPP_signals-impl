#ifndef CPP_SIGNALS_SIGNALS_H
#define CPP_SIGNALS_SIGNALS_H

#include <list>
#include <functional>
#include <memory>

template<typename X>
struct slot;

template<typename X>
struct my_connection;

//SLOT
template<typename SlotF>
struct slot {
    SlotF function;
    bool enabled;

    slot() {
        enabled = false;
    }

    slot(SlotF func) : function(func), enabled(true) { }

    template<typename ... SlotArgs>
    void operator()(SlotArgs ... args) {
        function(args...);
    }

    void disconnect() {
        enabled = false;
    }
};
//\SLOT

template<typename Fun, typename ... Args>
struct my_signal {

    typedef std::function<void(Args...)> F;
    typedef std::shared_ptr<slot<F>> ss_ptr;
    bool created;
    bool first;
    std::list<ss_ptr> data_;
    ss_ptr firstElement;

    my_signal() {
        first = false;
        created = false;
    }

    my_connection<F> connect(F func) {
        ss_ptr tmp = std::make_shared<slot<F>>(func);
        if (!first) {
            firstElement = tmp;
            first = true;
        } else {
            if (!created) {
                created = true;
                data_ = std::list<ss_ptr>();
            }
            data_.push_back(tmp);
        }
        return my_connection<F>(tmp);
    }

    template<typename ... SArgs>
    void operator()(SArgs... args) {
        if (first) {
            if (firstElement->enabled)
                (*firstElement)(args...);
            else
                first = false;
        }
        if (created) {
            std::list<ss_ptr> tmpData_;
            size_t fixedEnd = data_.size();
            for (auto tmpIter = data_.begin(); tmpIter != data_.end(); tmpIter++, fixedEnd--)
                if ((*tmpIter)->enabled) {
                    if (fixedEnd>0)
                        (*(*tmpIter))(args...);
                    tmpData_.push_back((*tmpIter));
                }
            std::swap(data_, tmpData_);
        }
    }
};

template<typename CFun>
struct my_connection {
    bool connected;
    std::shared_ptr<slot<CFun>> aim;

    my_connection(std::shared_ptr<slot<CFun>> refer) {
        aim = refer;
        connected = true;
    };

    void disconnect() {
        if (connected)
            aim->disconnect();
        connected = false;
    }
};

#endif //CPP_SIGNALS_SIGNALS_H